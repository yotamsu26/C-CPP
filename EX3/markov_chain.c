#include "stdio.h"
#include "markov_chain.h"
#include "linked_list.h"
#include "string.h"
#include "stdlib.h"

Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  return get_markov_node (markov_chain, data_ptr);
}

Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  if (check_if_in_list (markov_chain, data_ptr))
    {
      return get_node_from_database (markov_chain, data_ptr);
    }

  MarkovNode *temp_markov = malloc (sizeof (MarkovNode));
  if (temp_markov == NULL)
    {
      return NULL;
    }

  temp_markov->data = markov_chain->copy_func (data_ptr);
  if (!temp_markov->data)
    {
      free (temp_markov);
      return NULL;
    }

  temp_markov->next_c = NULL;
  temp_markov->size = 0;
  temp_markov->is_last = markov_chain->is_last (temp_markov->data);
  add (markov_chain->database, temp_markov);
  return markov_chain->database->last;
}

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node,
                               MarkovChain *markov_chain)
{
  if (first_node->next_c == NULL)
    {
      if (add_counter_list_first (first_node, second_node))
        {
          return false;
        }
    }
  else
    {
      int i = check_if_in_markov (first_node,
                                  second_node->data, markov_chain);
      if (i >= 0)
        {
          first_node->total_size += 1;
          first_node->next_c[i].frequency += 1;
        }
      else
        {
          if (update_counter_list (first_node, second_node))
            {
              return false;
            }
        }
    }
  return true;
}

int check_if_in_markov (MarkovNode *check_node, void *data,
                        MarkovChain *markov_chain)
{
  int i = 0;
  unsigned int diff = check_node->size;
  while (diff > 0)
    {
      if ((markov_chain->comp_func (check_node->next_c[i].next_m->data, data))
          == 0)
        {
          return i;
        }
      diff--;
      i++;
    }
  return -1;
}

int check_if_in_list (MarkovChain *markov_chain, void *data)
{
  if (markov_chain->database->first == NULL)
    {
      return 0;
    }
  Node *temporary = markov_chain->database->first;
  while (markov_chain->database->last != temporary)
    {
      if ((markov_chain->comp_func (temporary->data->data, data)) == 0)
        {
          return 1;
        }
      temporary = temporary->next;
    }

  if ((markov_chain->comp_func (temporary->data->data, data)) == 0)
    {
      return 1;
    }
  return 0;
}

Node *get_markov_node (MarkovChain *markov_chain, char *word)
{
  Node *temp = markov_chain->database->first;
  while (temp != markov_chain->database->last)
    {
      if ((markov_chain->comp_func (temp->data->data, word)) == 0)
        {
          return temp;
        }
      temp = temp->next;
    }
  return temp;
}

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int r = get_random_number (markov_chain->database->size);
  Node *first_node = NULL;
  int flag = 1;
  while (flag)
    {
      first_node = markov_chain->database->first;
      while (r > 0)
        {
          first_node = first_node->next;
          r--;
        }
      flag = markov_chain->is_last (first_node->data->data);
      if (flag)
        {
          r = get_random_number (markov_chain->database->size);
        }
    }
  return first_node->data;
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  unsigned int r = get_random_number (state_struct_ptr->total_size);
  NextNodeCounter *last_node =
      state_struct_ptr->next_c + state_struct_ptr->size;
  NextNodeCounter *check_node = state_struct_ptr->next_c;
  unsigned int i = 0;
  unsigned int j = 0;

  while (check_node < last_node)
    {
      while (j < check_node->frequency)
        {
          if (i == r)
            {
              return check_node->next_m;
            }
          j++;
          i++;
        }
      j = 0;
      check_node++;
    }
  return check_node->next_m;
}

void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  int counter = max_length - 1;
  markov_chain->print_func (first_node->data);
  int flag = first_node->is_last;
  while (counter > 0 && flag == 0)
    {
      MarkovNode *temp = first_node;
      first_node = get_next_random_node (temp);
      markov_chain->print_func (first_node->data);
      flag = first_node->is_last;
      counter--;
    }
}

void free_markov_chain (MarkovChain **ptr_chain)
{
  if (!(*ptr_chain)->database)
    {
      free (*ptr_chain);
      (*ptr_chain) = NULL;
      return;
    }
  Node *temp = (*ptr_chain)->database->first;
  for (int i = 0; i < (*ptr_chain)->database->size; i++)
    {
      Node *delete = temp->next;
      if (temp->data->data != NULL)
        {
          (*ptr_chain)->free_data (temp->data->data);
          temp->data->data = NULL;
        }
      if (temp->data->next_c != NULL)
        {
          free (temp->data->next_c);
          temp->data->next_c = NULL;
        }
      if (temp->data != NULL)
        {
          free (temp->data);
          temp->data = NULL;
        }
      free (temp);
      temp = NULL;
      temp = delete;
    }
  if (temp)
    {
      free (temp);
      temp = NULL;
    }
  free ((*ptr_chain)->database);
  (*ptr_chain)->database = NULL;
  free (*ptr_chain);
  (*ptr_chain) = NULL;
}

int add_counter_list_first (MarkovNode *first_node, MarkovNode *second_node)
{
  first_node->next_c = malloc (sizeof (NextNodeCounter));
  if (free_next_node_param (first_node->next_c))
    {
      return 1;
    }
  first_node->size = 1;
  first_node->total_size = 1;
  first_node->next_c->frequency = 1;
  first_node->next_c->next_m = second_node;
  return 0;
}

int update_counter_list (MarkovNode *first_node, MarkovNode *second_node)
{
  first_node->size++;
  first_node->next_c = realloc (first_node->next_c,
                                sizeof (NextNodeCounter)
                                * first_node->size);
  if (free_next_node_param (first_node->next_c))
    {
      return 1;
    }
  first_node->next_c[first_node->size - 1].next_m = second_node;
  first_node->next_c[first_node->size - 1].frequency = 1;
  first_node->total_size += 1;
  return 0;
}

int get_random_number (int max_number)
{
  return rand () % max_number;
}

int free_markov_chain_local (MarkovChain *markov_chain)
{
  if (markov_chain == NULL)
    {
      free (markov_chain);
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

int free_linked_list (LinkedList *main_list)
{
  if (main_list == NULL)
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

void initial_linked_list (LinkedList *main_list)
{
  main_list->first = NULL;
  main_list->last = NULL;
  main_list->size = 0;
}

int free_next_node_param (NextNodeCounter *node)
{
  if (node == NULL)
    {
      free (node);
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
