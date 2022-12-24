#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))
#define ARG_NUM1 3
#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define ERROR_PARAM "Usage: there should be 2 param"
#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define DECIMAL 10
#define END_GAME 100
#define PATH_NUM 2
#define SEED 1

/**
 * allocation of space to the new object.
 * @param val1 a data to copy
 * @return the new allocated object.
 */
static int compint (void *val1, void *val2);

/**
 * free the data.
 * @param val1 a data to free.
 */
static void freeint (void *val1);

/**
 * allocation of space to the new object.
 * @param val1 a data to copy
 * @return the new allocated object.
 */
static void *copyint (void *val1);

/**
 *
 * @param val1 a data to free.
 */
static void printint (void *val1);

/**
 *
 * @param first_data a data of type void*
 * @return true if the data is the last possible data false otherwise.
 */
static bool is_lastint (void *first_data);

/**
 * initial all the relevant data.
 * @param main_chain markov chain.
 */
static void intial_markov (MarkovChain *main_chain);

/**
 * get a stable seed.
 * @param arg_param number of seed from the cml..
 */
static void get_seed (char *arg_param);

/**
 *
 * @param main_chain markov chain.
 * @return the markoc chain with the linked list inside.
 */
static int insert_linked_list (MarkovChain *main_chain);

/**
 * create paths as number of paths.
 * @param markov_chain markov chain.
 * @param path_num number of paths.
 */
static void create_path (MarkovChain *markov_chain, char *num_of_walks);

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number;
    int ladder_to;
    int snake_to;
} Cell;

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
  printf ("%s", error_msg);
  if (database != NULL)
    {
      free_markov_chain (database);
    }
  return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
  for (int i = 0; i < BOARD_SIZE; i++)
    {
      cells[i] = malloc (sizeof (Cell));
      if (cells[i] == NULL)
        {
          for (int j = 0; j < i; j++)
            {
              free (cells[j]);
            }
          handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
          return EXIT_FAILURE;
        }
      *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

  for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
      int from = transitions[i][0];
      int to = transitions[i][1];
      if (from < to)
        {
          cells[from - 1]->ladder_to = to;
        }
      else
        {
          cells[from - 1]->snake_to = to;
        }
    }
  return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database (MarkovChain *markov_chain)
{
  Cell *cells[BOARD_SIZE];
  if (create_board (cells) == EXIT_FAILURE)
    {
      return EXIT_FAILURE;
    }
  MarkovNode *from_node = NULL, *to_node = NULL;
  size_t index_to;
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      add_to_database (markov_chain, cells[i]);
    }
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      from_node = get_node_from_database (markov_chain, cells[i])->data;
      if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
          index_to = MAX(cells[i]->snake_to, cells[i]->ladder_to) - 1;
          to_node = get_node_from_database (markov_chain,
                                            cells[index_to])
              ->data;
          add_node_to_counter_list (from_node, to_node,
                                    markov_chain);
        }
      else
        {
          for (int j = 1; j <= DICE_MAX; j++)
            {
              index_to = ((Cell *) (from_node->data))->number + j - 1;
              if (index_to >= BOARD_SIZE)
                {
                  break;
                }
              to_node = get_node_from_database
                  (markov_chain, cells[index_to])
                  ->data;
              add_node_to_counter_list
                  (from_node, to_node, markov_chain);
            }
        }
    }
  for (size_t i = 0; i < BOARD_SIZE; i++)
    {
      free (cells[i]);
    }
  return EXIT_SUCCESS;
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
  if (argc != ARG_NUM1)
    {
      fprintf (stdout, ERROR_PARAM);
      return EXIT_FAILURE;
    }

  get_seed (argv[SEED]);
  MarkovChain *main_chain = malloc (sizeof (MarkovChain));
  if (free_markov_chain_local (main_chain))
    {
      return EXIT_FAILURE;
    }

  intial_markov (main_chain);
  if (insert_linked_list (main_chain))
    {
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      return EXIT_FAILURE;
    }

  if (fill_database (main_chain))
    {
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      return EXIT_FAILURE;
    }

  create_path (main_chain, argv[PATH_NUM]);
  free_markov_chain (&main_chain);
  return EXIT_SUCCESS;
}

static void create_path (MarkovChain *markov_chain, char *num_of_walks)
{
  char *num_of_paths_modelu;
  int num_of_paths = strtol (num_of_walks,
                             &num_of_paths_modelu, DECIMAL);
  int counter = 1;
  while (num_of_paths)
    {
      fprintf (stdout, "Random Walk %i: ", counter);
      generate_random_sequence (markov_chain,
                                markov_chain->database->first->data,
                                MAX_GENERATION_LENGTH);
      fprintf (stdout, "\n");
      num_of_paths--;
      counter++;
    }
}

static void get_seed (char *arg_param)
{
  char *s_modelu;
  unsigned int s = strtol (arg_param, &s_modelu, DECIMAL);
  srand (s);
}

static void intial_markov (MarkovChain *main_chain)
{
  main_chain->is_last = is_lastint;
  main_chain->free_data = freeint;
  main_chain->comp_func = compint;
  main_chain->copy_func = copyint;
  main_chain->print_func = printint;
}

static int compint (void *val1, void *val2)
{
  Cell *num1 = (Cell *) val1;
  Cell *num2 = (Cell *) val2;
  return num1->number - num2->number;
}

static void freeint (void *val)
{
  Cell *num1 = val;
  free (num1);
}

static void *copyint (void *val)
{
  if (!val)
    {
      return NULL;
    }

  Cell *pointer = (Cell *) val;
  Cell *new_pointer = malloc (sizeof (Cell));
  if (!new_pointer)
    {
      return NULL;
    }

  new_pointer->number = pointer->number;
  new_pointer->ladder_to = pointer->ladder_to;
  new_pointer->snake_to = pointer->snake_to;
  return (void *) new_pointer;
}

static void printint (void *val)
{
  Cell *num1 = (Cell *) val;
  if (num1->snake_to != -1)
    {
      fprintf (stdout, "[%d]-snake to %d -> ",
               num1->number, num1->snake_to);
    }

  else if (num1->ladder_to != -1)
    {
      fprintf (stdout, "[%d]-ladder to %d -> ",
               num1->number, num1->ladder_to);
    }

  else if (is_lastint (val))
    {
      fprintf (stdout, "[%d]", num1->number);
    }

  else
    {
      fprintf (stdout, "[%d] -> ", num1->number);
    }

}

static bool is_lastint (void *first_data)
{
  Cell *num1 = (Cell *) first_data;
  return num1->number == END_GAME;
}

static int insert_linked_list (MarkovChain *main_chain)
{
  LinkedList *linked_markov = malloc (sizeof (LinkedList));
  if (free_linked_list (linked_markov))
    {
      free (main_chain);
      return 1;
    }
  initial_linked_list (linked_markov);
  main_chain->database = linked_markov;
  return 0;
}