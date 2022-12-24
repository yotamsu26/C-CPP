#include "sort_bus_lines.h"
#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
/**
 * receives two pointers to the start and to the end of the array.
 * checks if the array is sorted by distance from the close one to the far one.
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * receives two pointers to the start and to the end of the array.
 * checks if the array is sorted by duration from the shortest one to
 * the longest one.
 */

int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
 * receives two pointers to the start and to the end of the initial array.
 * and another two pointers to the start of the sorted array and the end of
 * the sorted array.
 * checks if there are the same array before and after sorting.
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);

/**
 * receives two pointers to the start and to the end of the initial array.
 * and another pointer to the start of the sorted array.
 * check the number of num lines that's equals.
 */
unsigned int check_equality (BusLine *start_sorted, BusLine *start_original,
                             BusLine *end_original);

#endif
