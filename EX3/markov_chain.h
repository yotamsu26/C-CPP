#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool

#define ALLOCATION_ERROR_MASSAGE \
"Allocation failure: Failed to allocate new memory\n"


/***************************/
typedef void (*printFunc)(void *);
typedef int (*compFunc)(void *, void *);
typedef void (*freeFunc)(void *);
typedef void* (*copyFunc)(void *);
typedef bool (*islastFunc)(void *);
typedef struct NextNodeCounter NextNodeCounter;
/***************************/


/***************************/



/***************************/
/*        STRUCTS          */
/***************************/

typedef struct MarkovNode {
    NextNodeCounter *next_c;
    void *data;
    int size;
    int total_size;
    bool is_last;

} MarkovNode;

typedef struct NextNodeCounter {
    unsigned int frequency;
    MarkovNode *next_m;
} NextNodeCounter;

/* DO NOT ADD or CHANGE variable names in this struct */
typedef struct MarkovChain {
    LinkedList *database;

    // pointer to a func that receives data from a generic type and prints it
    // returns void.
    /* <fill_type> */
    printFunc print_func;

    // pointer to a func that gets 2 pointers of generic data type(same one)
    // and compare between them */
    // returns: - a positive value if the first is bigger
    //          - a negative value if the second is bigger
    //          - 0 if equal
    /* <fill_type> */
    compFunc comp_func;

    // a pointer to a function that gets a pointer of generic data type and
    // frees it.
    // returns void.
    /*<fill_type>*/
    freeFunc free_data;

    // a pointer to a function that  gets a pointer of generic data type and
    // returns a newly allocated copy of it
    // returns a generic pointer.
    /*<fill_type>*/
    copyFunc copy_func;

    //  a pointer to function that gets a pointer of generic data type and
    //  returns:
    //      - true if it's the last state.
    //      - false otherwise.
    /*<fill_type>*/
    islastFunc is_last;
} MarkovChain;

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with, if NULL- choose a random
 * markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain(MarkovChain **markov_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain);

/**
* Check if data_ptr is in database. If so, return the markov_node
 * wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr);

/**
* If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database
 */
Node* add_to_database(MarkovChain *markov_chain, void *data_ptr);

/**
 * check the memory allocation.
 * @param markov_chain pointer to the markov chain.
 * @return 1 if failed 0 if succeed.
 */
int free_markov_chain_local (MarkovChain *markov_chain);

/**
 * check the memory allocation.
 * @param main_list pointer to the linked list.
 * @return 1 if failed 0 if succeed.
 */
int free_linked_list (LinkedList *main_list);

/**
 * initialized all the param of the linked list to null.
 * @param main_list pointer to linked list.
 */
void initial_linked_list (LinkedList *main_list);

/**
 * check the memory allocation.
 * @param str pointer to array of void.
 * @return 1 if failed 0 if succeed.
 */
int free_void (void *str, MarkovChain *markov_chain);

/**
 *
 * @param main_list pointer to the linked list.
 * @param word pointer to array of chars.
 * @return the relevant markov node from the linked list.
 */
Node *get_markov_node(MarkovChain *markov_chain, char *word);

/**
 *
 * @param main_list pointer to the linked list.
 * @param word pointer to array of chars.
 * @return 1 if the word already in the linked list, 0 if not.
 */
int check_if_in_list (MarkovChain *markov_chain, void *word);

/**
 *
 * @param node pointer to next node counter.
 * @return 1 if failed 0 if succeed.
 */
int free_next_node_param (NextNodeCounter *node);

/**
 *
 * @param max_number the max number to random from.
 * @return random number.
 */
int get_random_number (int max_number);

/**
 *
 * @param check_node pointer to markov node.
 * @param data pointer to array of char.
 * @return the index of the markov if the string is in the markov already.
 * and -1 if it is not.
 */
int check_if_in_markov (MarkovNode *check_node, void *data,
    MarkovChain *markov_chain);

/**
 * add all the relevant data to the markov node.
 * @param first_node pointer to markov node.
 * @param second_node pointer to markov node.
 * @return 1 if the allocation failed. and 0 if the it succeed.
 */
int add_counter_list_first(MarkovNode *first_node, MarkovNode *second_node);

/**
 * add all the relevant data to the markov node.
 * @param first_node pointer to markov node.
 * @param second_node pointer to markov node.
 * @return 1 if the allocation failed. and 0 if the it succeed.
 */
int update_counter_list(MarkovNode *first_node, MarkovNode *second_node);



#endif
