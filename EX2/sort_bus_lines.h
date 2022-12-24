#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
/**
 * a struct of bus lines.
 */
typedef struct BusLine {
    int line_number, distance, duration;
} BusLine;

void swap (BusLine *num1, BusLine *num2);
/**
 * receives to pointers to two bus line and change there place in the array.
 */
void bubble_sort (BusLine *start, BusLine *end);

/**
 * receives two pointers to the start and to the end of the array.
 * its sort them with bubble sort algorithm according to their distance
 */
void quick_sort (BusLine *start, BusLine *end);

/**
 * receives two pointers to the start and to the end of the array.
 * its sort them with quick sort algorithm according to their duration.
 */

BusLine *partition (BusLine *start, BusLine *end);
/**
 * receives two pointers to the start and to the end of the array.
 * put the end bus line in his correct place. and return pointer to the
 * index that is in the correct place of the array.
 */
#endif