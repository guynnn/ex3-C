#include <stdio.h>
#include <stdlib.h>
#include "MyIntFunctions.h"

/**
 * @brief clone an int
 */
void* cloneInt(const void* i)
{
    if (i == NULL)
    {
        return NULL;
    }
    int * clone = (int*)malloc(sizeof(int));
    *clone = *(int*)i;
    return clone;
}

/**
 * @brief free an int
 */
void freeInt(void* i)
{
    if (i == NULL)
    {
        return;
    }
    free(i);
}

/**
 * @brief hash value of key for HashTable with size tableSize
 *  assuming key pointer to an int
 * @return number between 0-(tableSize-1)

 */
int intFcn (const void* key, size_t tableSize)
{
    int intKey = *(int*)key;
    int res = intKey % (int)tableSize;
    return res >= 0 ? res : res + (int)tableSize;
}

/**
 * @brief print a string
 *  assuming key pointer to an int
 */
void intPrint (const void* key)
{
    if (key == NULL)
    {
        return;
    }
    int intKey = *(int *) key;
    printf("%d", intKey);
}

/**
 *  intCompare - pointer to int comparison function:
 * @brief compare 2 ints
 *   returns zero int if both ints are equal, otherwise
 *   returns non-zero.
 *
 */
int intCompare (const void* key1, const void* key2)
{
    if (key1 == NULL || key2 == NULL)
    {
        return 1;
    }
    int intKey1 = *(int*)key1;
    int intKey2 = *(int*)key2;
    if (intKey1 == intKey2)
    {
        return 0;
    }
    return 1;
}