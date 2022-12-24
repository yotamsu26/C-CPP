#include "sort_bus_lines.h"

void bubble_sort (BusLine *start, BusLine *end)
{
  unsigned int iter = end - start;
  for (unsigned int i = 1; i < iter; i++)
    {
      int flag = 0;
      for (unsigned int j = 0; j < iter - i; j++)
        {
          if (start->distance > (start + 1)->distance)
            {
              swap (start, start + 1);
              flag = 1;
            }
          start = start + 1;
        }
      if (!flag)
        {
          break;
        }
      start = start - (iter - i);
    }
}

void swap (BusLine *num1, BusLine *num2)
{
  int temp = num1->duration;
  num1->duration = num2->duration;
  num2->duration = temp;
  int temp1 = num1->distance;
  num1->distance = num2->distance;
  num2->distance = temp1;
  int temp2 = num1->line_number;
  num1->line_number = num2->line_number;
  num2->line_number = temp2;
}

BusLine *partition (BusLine *start, BusLine *end)
{
  BusLine *pivot = end - 1;
  BusLine *l = start - 1;
  for(BusLine *i = start; i < end - 1; i ++)
    {
      if(i->duration < pivot -> duration)
        {
          l += 1;
          swap (l, i);
        }
    }
  swap (l + 1, pivot);
  return (l+1);
}

void quick_sort (BusLine *start, BusLine *end)
{
  if ((end - start) > 0)
    {
      BusLine *main_pivot = partition (start, end);
      quick_sort (start, main_pivot);
      quick_sort (main_pivot + 1, end);
    }
}
