#ifndef _MY_STR_FUNCTIONS_H_
#define _MY_STR_FUNCTIONS_H_


#include "Key.h"

/**
 * @brief clone a string
 */
void* cloneStr(const void*  s);

/**
 * @brief free an string
 */
void freeStr(void* s);


/**
 * @brief hash value of key for HashTable with size tableSize
 *  assuming key pointer to string
 * @return number between 0-(tableSize-1)

 */
int strFcn (const void*  s, size_t tableSize);

/**
 * @brief print a string
 *  assuming key pointer to string
 * 
 */
void strPrint (const void*  s);

/**
 *  strCompare - pointer to int comparison function:
 * @brief compare 2 strings
 *   returns zero int if both strings are equal, otherwise
 *   returns non-zero.
 */
int strCompare (const void* key1, const void* key2);

#endif // _MY_STR_FUNCTIONS_H_
