#ifndef _MY_KEY_H_
#define _MY_KEY_H_
#include <stddef.h>

/**
 * @brief Allocate memory for an object which contain duplicatoin of the given key.
 * If run out of memory, free all the memory that was already allocated by the function, 
 * report error MEM_OUT to the standard error and return NULL.
 */
 typedef void * (*CloneKeyFcn)(const void * key);


/**
 * @brief Free all the memory allocated for an object
 */
 typedef void(*FreeKeyFcn)(void * key);




/**
 * @brief hash value of key for HashTable with size tableSize
 * @return number between 0-(tableSize-1) or negative number in case of an error
 */
 typedef int(*HashFcn)(const void * key, size_t tableSize);



/**
 * @brief print function
 * 
 */
 typedef void(*PrintKeyFcn)(const void * key);

/**
 * ComparisonFcn - pointer to key comparison function:
 * @brief compare 2 keys
 *   returns zero int if both keys are equal, otherwise
 *   returns non-zero.
 * use void *
 */
typedef int(*ComparisonFcn)(const void * key1, const void * key2);


#endif