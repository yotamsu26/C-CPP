
#include "markov_chain.h"
#include "string.h"
#define DELIMITERS " \r\n"
#define ARG_NUM1 4
#define ARG_NUM2 5
#define TWEETS_NUM 2
#define FILE_IN 3
#define ERROR_PARAM "Usage: there should be 3 or 4 param"
#define ERROR_FILE "Error: not valid file"
#define BUFFER 1000
#define WORD_NUM 4
#define DECIMAL 10
#define SEED 1
#define MAX_TWEETS 20

/**
 *
 * @param val1 void* data
 * @param val2 void* data
 * @return convert the data to cell struct and compare between the numbers
 * in the cells
 */
static int compstr (void *val1, void *val2);

/**
 * free the data.
 * @param val1 a data to free.
 */
static void freestr (void *val1);

/**
 * allocation of space to the new object.
 * @param val1 a data to copy
 * @return the new allocated object.
 */
static void *copystr (void *val1);

/**
 *
 * @param val1 a data to free.
 */
static void printstr (void *val1);

/**
 * initial all the relevant data.
 * @param main_chain markov chain.
 */
static void intial_markov (MarkovChain *main_chain);

/**
 *
 * @param first_word pointer to array of char.
 * @return 0 if there is no dot in the string, and 1 if is.
 */
static bool dot_in_string (void *first_data);

/**
 * open a given file.
 * @param in_file pointer to the file.
 * @return 1 if failed 0 if succeed.
 */
static int open_file (FILE *in_file);

/**
 * get a stable seed.
 * @param arg_param number of seed from the cml..
 */
static void get_seed (char *arg_param);

/**
 * fill all the linked list with the relevant data.
 * @param fp pointer to the file.
 * @param words_to_read number of max words.
 * @param markov_chain pointer to the main markov chain..
 */
static int
fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain);

/**
 * convert a given string to int.
 * @param argv pointer to the given param from the cml.
 * @param argc number of given cml args.
 * @return the converted int.
 */
static int convert_to_int (char **argv, int argc);

/**
 * fill all the linked list with the relevant data.
 * @param fp pointer to the file.
 * @param words_to_read number of max words.
 * @param markov_chain pointer to the main markov chain..
 */
static int fill_database_helper (FILE *fp, int words_to_read,
                                 MarkovChain *markov_chain);

/**
 * print the given number of tweets to stdout.
 * @param markov_chain pointer to markov chain.
 * @param tweet_num number of tweets that the user want to generate.
 */
static void tweet_maker (MarkovChain *markov_chain, char *tweet_num);

/**
 *adding the first markov to the database.
 * @param markov_chain pointer to pointer to markov chain.
 * @param first_node pointer to pointer to markov node.
 * @param token pointer to pointer to char.
 * @return the first markov node after adding to data base.
 */
static MarkovNode *prev_null (MarkovChain **markov_chain,
                              MarkovNode **first_node, char **token);

/**
 *adding the markov node to database and to the markov next array.
 * @param markov_chain pointer to pointer to markov chain.
 * @param first_node pointer to pointer to markov node.
 * @param prev pointer to pointer to markov node.
 * @param token pointer to pointer to char.
 */
static int prev_not_null (MarkovChain **markov_chain, MarkovNode **first_node,
                          MarkovNode **prev, char **token);

int main (int argc, char *argv[])
{
  if (argc != ARG_NUM1 && argc != ARG_NUM2)
    {
      fprintf (stdout, ERROR_PARAM);
      return EXIT_FAILURE;
    }
  FILE *tweet_file = fopen (argv[FILE_IN], "r");
  if (open_file (tweet_file))
    {
      return EXIT_FAILURE;
    }
  get_seed (argv[SEED]);

  MarkovChain *main_chain = malloc (sizeof (MarkovChain));
  if (free_markov_chain_local (main_chain))
    {
      fclose (tweet_file);
      return EXIT_FAILURE;
    }
  intial_markov (main_chain);

  if (fill_database (tweet_file, convert_to_int
      (argv, argc), main_chain))
    {
      fclose (tweet_file);
      free_markov_chain (&main_chain);
      fprintf (stdout, ALLOCATION_ERROR_MASSAGE);
      return EXIT_FAILURE;
    }
  tweet_maker (main_chain, argv[TWEETS_NUM]);
  free_markov_chain (&main_chain);
  fclose (tweet_file);
  return EXIT_SUCCESS;
}

static int
fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  LinkedList *linked_markov = malloc (sizeof (LinkedList));
  if (free_linked_list (linked_markov))
    {
      return 1;
    }

  initial_linked_list (linked_markov);
  markov_chain->database = linked_markov;

  if (fill_database_helper (fp, words_to_read,
                            markov_chain))
    {
      return 1;
    }
  return 0;
}

static int fill_database_helper (FILE *fp, int words_to_read,
                                 MarkovChain *markov_chain)
{
  MarkovNode *prev = NULL, *first_node = NULL;
  char long_word[BUFFER];
  char *word;
  while (fgets (long_word, BUFFER, fp) && words_to_read != 0)
    {
      word = strtok (long_word, DELIMITERS);
      while (words_to_read != 0 && word != NULL)
        {
          if (prev == NULL)
            {
              if (!(prev = prev_null (&markov_chain,
                                      &first_node, &word)))
                {
                  return 1;
                }
            }
          else
            {
              if (prev_not_null (&markov_chain,
                                 &first_node, &prev, &word))
                {
                  return 1;
                }
            }
          words_to_read--;
          word = strtok (NULL, DELIMITERS);
        }
    }
  return 0;
}

static MarkovNode *
prev_null (MarkovChain **markov_chain, MarkovNode **first_node, char **token)
{
  Node *temp = (add_to_database (*markov_chain, *token));
  if (!temp)
    {
      return NULL;
    }

  *first_node = temp->data;
  return *first_node;
}

static int prev_not_null (MarkovChain **markov_chain, MarkovNode **first_node,
                          MarkovNode **prev, char **token)
{
  *prev = *first_node;
  Node *temp = (add_to_database (*markov_chain, *token));
  if (!temp)
    {
      return 1;
    }

  *first_node = temp->data;
  if (!dot_in_string ((*prev)->data))
    {
      add_node_to_counter_list (*prev,
                                *first_node,
                                *markov_chain);
    }
  return 0;
}

static void tweet_maker (MarkovChain *markov_chain, char *tweet_num)
{
  char *num_of_tweets_modelu;
  int num_of_tweets = strtol (tweet_num,
                              &num_of_tweets_modelu, DECIMAL);
  int counter = 1;
  while (num_of_tweets)
    {
      fprintf (stdout, "Tweet %i: ", counter);
      MarkovNode *temp = get_first_random_node (markov_chain);
      generate_random_sequence (markov_chain,
                                temp,
                                MAX_TWEETS);
      num_of_tweets--;
      counter++;
    }
}

static int open_file (FILE *in_file)
{
  if (in_file == NULL)
    {
      fprintf (stdout, ERROR_FILE);
      return 1;
    }
  return 0;
}

static void get_seed (char *arg_param)
{
  char *s_modelu;
  unsigned int s = strtol (arg_param, &s_modelu, DECIMAL);
  srand (s);
}

static int convert_to_int (char **argv, int argc)
{
  if (argc == ARG_NUM1)
    {
      return -1;
    }

  else
    {
      char *k_modelu;
      int k = strtol (argv[WORD_NUM], &k_modelu, DECIMAL);
      return k;
    }
}

static bool dot_in_string (void *first_data)
{
  char *word = (char *) first_data;
  unsigned int i = 0;
  while (word[i] != '\0')
    {
      i++;
    }
  if (word[i - 1] == '.')
    {
      return 1;
    }

  return 0;
}

static void intial_markov (MarkovChain *main_chain)
{
  main_chain->is_last = dot_in_string;
  main_chain->free_data = freestr;
  main_chain->comp_func = compstr;
  main_chain->copy_func = copystr;
  main_chain->print_func = printstr;
}

static int compstr (void *val1, void *val2)
{
  char *word1 = val1;
  char *word2 = val2;
  return strcmp (word1, word2);
}

static void *copystr (void *val)
{
  char *copy_word = val;
  char *data = malloc (strlen (copy_word) + 1);
  if (!data)
    {
      return NULL;
    }

  strcpy (data, copy_word);
  return data;
}

static void freestr (void *val)
{
  char *free_word = val;
  free (free_word);
}

static void printstr (void *val)
{
  char *print_str = val;
  (dot_in_string (print_str)) ? fprintf (stdout,
                                         "%s\n", print_str)
                              : fprintf (stdout, "%s ",
                                         print_str);
}