#include "test_bus_lines.h"
#include "sort_bus_lines.h"
#include "stdio.h"

int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  unsigned int diff = end - start;
  while (diff > 1)
    {
      printf ("%i\n", (start)->distance);
      if ((start + 1)->distance < start->distance)
        {
          fprintf (stdout, "%s",
                   "TEST 1 ""FAILED: The array isn't sorted by distance\n");
          return 0;
        }
      start += 1;
      diff -= 1;
    }
  fprintf (stdout, "%s", "TEST 1 PASSED: The array is sorted by distance\n");
  return 1;
}

int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  unsigned int diff = end - start;
  while (diff > 1)
    {
      if ((start + 1)->duration < start->duration)
        {
          fprintf (stdout, "%s",
                   "TEST 3 FAILED: The array isn't sorted by duration\n");
          return 0;
        }
      start += 1;
      diff -= 1;
    }
  fprintf (stdout, "%s", "TEST 3 PASSED: The array is sorted by duration\n");
  return 1;
}

int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original)
{
  if (end_sorted - start_sorted != end_original - start_original)
    {
      return 0;
    }
  unsigned int diff = end_original - start_original;
  unsigned int counter = check_equality (start_sorted, start_original,
                                         end_original);
  if (counter == diff)
    {
      return 1;
    }
  return 0;
}

unsigned int check_equality (BusLine *start_sorted, BusLine *start_original,
                             BusLine *end_original)
{
  unsigned int diff = end_original - start_original;
  unsigned int counter = 0;
  for (unsigned int i = 0; i < diff; i++)
    {
      for (unsigned int j = 0; j < diff; j++)
        {
          if (start_original->line_number == start_sorted->line_number)
            {
              counter++;
            }
          start_sorted++;
        }
      start_original++;
      start_sorted = start_sorted - diff;
    }
  return counter;
}