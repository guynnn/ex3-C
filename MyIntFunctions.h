#ifndef _MY_INT_FUNCTIONS_H_
#define _MY_INT_FUNCTIONS_H_
#include "Key.h"	

/**
 * @brief clone an int
 */
void* cloneInt(const void* i);

/**
 * @brief free an int
 */
void freeInt( void* i);

/**
 * @brief hash value of key for HashTable with size tableSize
 * assuming key pointer to an int
 * @return number between 0-(tableSize-1)
 */
int intFcn (const void* key, size_t tableSize);

/**
 * @brief print a string
 *  assuming key pointer to an int
 * 
 */
void intPrint (const void* key);

/**
 *  intCompare - pointer to int comparison function:
 * @brief compare 2 ints
 *   returns zero int if both ints are equal, otherwise
 *   returns non-zero.
 * 
 */
int intCompare (const void* key1, const void* key2);

#endif // _MY_INT_FUNCTIONS_H_
