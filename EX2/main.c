#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define TEST "test"
#define BUBBLE "bubble"
#define QUICK "quick"
#define COM 1
#define MAX_LINE 60
#define DECI 10
#define MAX_DIS 1000
#define MAX_LINE_BUS 999
#define MIN_DUR 10
#define MAX_DUR 100
#define MIN_LINE_BUS 1
#define MIN_DIS 0
/**
 * TODO add documentation
 * TODO check the \n
 */
int arg_check (int argc, char *str);
BusLine *valid_input (BusLine *arr_end, int line_counter);
int valid_line_num ();
void print_array (BusLine *start, BusLine *end);
int valid_parameters (int line, int dis, int dur);
void check_test (BusLine *start, BusLine *end, int num_line);
// declaration of all the function

/**
 *
 * @param start pointer to the start of the array
 * @param end pointer to the end pf the array
 * @param num_line the size of the array
 * This function run on all the relevant tests. and checks if they passed.
 */
void check_test (BusLine *start, BusLine *end, int num_line)
{
  BusLine *array_copy = malloc (num_line * sizeof (BusLine));
  memcpy (array_copy, start, sizeof (BusLine) * num_line);
  bubble_sort (start, end);
  is_sorted_by_distance (start, end);
  int test_2 = is_equal (start, end,
                         array_copy,
                         array_copy + num_line);
  if (test_2)
    {
      fprintf (stdout, "%s",
               "TEST 2 PASSED: The array has the same items after sorting\n");
    }
  else
    {
      fprintf (stdout, "%s",
               "TEST 2 FAILED: The array hasn't"
               " the same items after sorting\n");
    }
  quick_sort (start, end);
  is_sorted_by_duration (start, end);
  int test_4 = is_equal (start, end,
                         array_copy,
                         array_copy + num_line);
  if (test_4)
    {
      fprintf (stdout, "%s",
               "TEST 4 PASSED: The array has the same items after sorting\n");
    }
  else
    {
      fprintf (stdout, "%s",
               "TEST 4 FAILED: The array hasn't"
               " the same items after sorting\n");
    }
  free (start);
  free (array_copy);
}

/**
 * This function prints an array
 * @param start pointer to the start of the array
 * @param end pointer to the end of the array
 */
void print_array (BusLine *start, BusLine *end)
{
  unsigned int diff = end - start;
  while (diff > 0)
    {
      fprintf (stdout, "%i,%i,%i\n",
               start->line_number, start->distance, start->duration);
      start++;
      diff--;
    }
}

/**
 * checks if the input is valid according to the given bounds.
 * @param line the number of the bus line.
 * @param dis the distance from the place.
 * @param dur the duration to the station.
 * @return 1 if unvalid 0 if valid.
 */
int valid_parameters (int line, int dis, int dur)
{
  if (line < MIN_LINE_BUS || line > MAX_LINE_BUS)
    {
      fprintf (stdout, "%s", "ERROR: Line number should be"
                             " an integer between 1 and"
                             " 999 (includes).\n");
      return 1;
    }
  if (dis < MIN_DIS || dis > MAX_DIS)
    {
      fprintf (stdout, "%s", "ERROR: Distance should be"
                             " an integer between 0 and"
                             " 1000 (includes).\n");
      return 1;
    }
  if (dur < MIN_DUR || dur > MAX_DUR)
    {
      fprintf (stdout, "%s", "ERROR: Duration should be "
                             "an integer between 10 and 100"
                             " (includes).\n");
      return 1;
    }
  return 0;
}

/**
 * get the number of lines from the user.
 * @return ask the user until the nu, is valid and then returns it.
 */
int valid_line_num ()
{
  int flag = 0;
  int num_line;
  while (!flag)
    {
      flag++;
      fprintf (stdout, "%s",
               "Enter number of lines. Then enter\n");
      char str[MAX_LINE];
      fgets (str, MAX_LINE, stdin);
      char *num_mod;
      num_line = strtol (str, &num_mod, DECI);
      if (num_line <= 0)
        {
          flag--;
          fprintf (stdout, "%s",
                   "ERROR: Line number should be greater than 0\n");
        }
    }
  return num_line;
}

/**
 *
 * @param argc the number of args from the cml.
 * @param str the string that entered.
 * @return 1 if the arguments invalid, 0 if valid.
 */
int arg_check (int argc, char *str)
{
  if (argc != 2 || ((strcmp (TEST, str)) != 0 && (strcmp (BUBBLE, str)) != 0
                    && (strcmp (QUICK, str)) != 0))
    {
      return 1;
    }
  return 0;
}

/**
 * enter to the array all the line information.
 * @param arr_end pointer to the start of the array.
 * @param line_counter the number of lines that the user need to give.
 * @return pointer to the end of the array.
 */
BusLine *valid_input (BusLine *arr_end, int line_counter)
{
  while (line_counter > 0)
    {
      char input[MAX_LINE];
      fprintf (stdout, "%s", "Enter line info. Then enter\n");
      fgets (input, MAX_LINE, stdin);
      sscanf (input, "%d,%d,%d", &arr_end->line_number,
              &arr_end->distance, &arr_end->duration);
      int checker = valid_parameters (arr_end->line_number,
                                      arr_end->distance,
                                      arr_end->duration);
      if (!checker)
        {
          line_counter--;
          arr_end = arr_end + 1;
        }
    }
  return arr_end;
}

int main (int argc, char *argv[])
{
  if (arg_check (argc, argv[COM]) || argc == 1)
    {
      fprintf (stdout, "%s",
               "USAGE: Should be: test/quick/bubble");
      return EXIT_FAILURE;
    }
  int sum_line = valid_line_num ();
  BusLine *arr_start = malloc (sum_line * sizeof (BusLine));
  if (arr_start == NULL)
    {
      free (arr_start);
      return EXIT_FAILURE;
    }
  BusLine *end_array = valid_input (arr_start, sum_line);
  if ((strcmp (argv[COM], TEST)) == 0)
    {
      check_test (arr_start, end_array, sum_line);
    }
  else if ((strcmp (argv[COM], BUBBLE)) == 0)
    {
      bubble_sort (arr_start, end_array);
      print_array (arr_start, end_array);
      free (arr_start);
    }
  else if ((strcmp (argv[COM], QUICK)) == 0)
    {
      quick_sort (arr_start, end_array);
      print_array (arr_start, end_array);
      free (arr_start);
    }
  return 0;
}
