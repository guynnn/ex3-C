#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyStringFunctions.h"

/**
 * @brief clone a string
 */
void* cloneStr(const void* s)
{
    if (s == NULL)
    {
        return NULL;
    }
    char * clone = (char*)malloc(sizeof(strlen(s)) + 1);
    strcpy(clone, s);
    return clone;
}

/**
 * @brief free an string
 */
void freeStr(void* s)
{
    if (s == NULL)
    {
        return;
    }
    free(s);
}

/**
 * @brief hash value of key for HashTable with size tableSize
 *  assuming key pointer to string
 * @return number between 0-(tableSize-1)
 */
int strFcn (const void* s, size_t tableSize)
{
    char * str = (char*)s;
    unsigned int i;
    int sum = 0;
    for (i = 0; i < strlen(str); ++i)
    {
        sum += str[i];
    }
    return sum % (int)tableSize;
}

/**
 * @brief print a string
 *  assuming key pointer to string
 *
 */
void strPrint (const void* s)
{
    if (s == NULL)
    {
        return;
    }
    printf("%s", (char*)s);
}

/**
 *  strCompare - pointer to int comparison function:
 * @brief compare 2 strings
 *   returns zero int if both strings are equal, otherwise
 *   returns non-zero.
 */
int strCompare (const void* s1, const void* s2)
{
    if (s1 == NULL || s2 == NULL)
    {
        return 1;
    }
    return strcmp(s1, s2);
}
