#include <stdio.h>
#include <stdlib.h>
#include "GenericHashTable.h"
#include "TableErrorHandle.h"

#ifndef MAX_ROW_ELEMENTS
#define MAX_ROW_ELEMENTS 2
#endif

static int reSize(TableP table);

/**
 * represents a single object in the table, which mean key and data.
 */
typedef struct Node
{
    void * key;
    DataP data;
} Node;

/**
 * represents a group in the table. each group holds certain amount of nodes.
 */
typedef struct Group
{
    int size;
    Node * elements[MAX_ROW_ELEMENTS];
} Group;

/**
 * represents a hash table.
 */
typedef struct Table
{
    size_t initialSize;
    size_t size;
    Group * group;
    CloneKeyFcn cloneKey;
    FreeKeyFcn freeKey;
    HashFcn hfun;
    PrintKeyFcn printKeyFun;
    PrintDataFcn printDataFun;
    ComparisonFcn fcomp;
} Table;

/**
 * @brief return a pointer to the data that exist in the table in cell number arrCell (where 0 is the
 * first cell), and placment at listNode in the list (when 0 is the
 * first node in the list, i.e. the node that is pointed from the table
 * itself).
 * If such data not exist return NULL
 */
DataP getDataAt(const TableP table, int arrCell, int listNode)
{
    if (table == NULL || table->group == NULL || table->group[arrCell].elements[listNode] == NULL)
    {
        return NULL;
    }
    return table->group[arrCell].elements[listNode]->data;
}

/**
 * @brief return the pointer to the key that exist in the table in cell number arrCell (where 0 is the
 * first cell), and placment at listNode in the list (when 0 is the
 * first node in the list, i.e. the node that is pointed from the table
 * itself).
 * If such key not exist return NULL
 */
ConstKeyP getKeyAt(const TableP table, int arrCell, int listNode)
{
    if (table == NULL || table->group == NULL || table->group[arrCell].elements[listNode] == NULL)
    {
        return NULL;
    }
    return table->group[arrCell].elements[listNode]->key;
}

/**
 * @brief Free all the memory allocated for the table.
 * It's the user responsibility to call this function before exiting the program.
 */
void freeTable(TableP table)
{
    if (table == NULL)
    {
        return;
    }
    unsigned int i;
    unsigned int j;
    for (i = 0; i < table->size; i++)
    {
        for (j = 0; j < MAX_ROW_ELEMENTS; j++)
        {
            if (table->group[i].elements[j] == NULL)
            {
                continue;
            }
            table->freeKey(table->group[i].elements[j]->key);
            free(table->group[i].elements[j]);
        }
    }
    free(table->group);
    free(table);
}

/**
 * @brief Allocate memory for a hash table with which uses the given functions.
 * tableSize is the number of cells in the hash table.
 * If run out of memory, free all the memory that was already allocated by the function,
 * report error MEM_OUT to the standard error and return NULL.
 */
TableP createTable(size_t tableSize, CloneKeyFcn cloneKey, FreeKeyFcn freeKey
                   , HashFcn hfun, PrintKeyFcn printKeyFun, PrintDataFcn printDataFun
                   , ComparisonFcn fcomp)
{
    TableP table = (TableP)malloc(sizeof(Table));
    if (table == NULL)
    {
        reportError(MEM_OUT);
        return NULL;
    }
    table->group = (Group*)malloc(tableSize * sizeof(Group));
    int i;
    int j;

    // initializing all the Nodes to be null with size 0.
    for (i = 0; i < (int)tableSize; ++i)
    {
        for (j = 0; j < MAX_ROW_ELEMENTS; ++j)
        {
            table->group[i].elements[j] = NULL;
        }
        table->group[i].size = 0;
    }

    if (table->group == NULL)
    {
        free(table);
        reportError(MEM_OUT);
        return NULL;
    }
    table->initialSize = tableSize;
    table->size = tableSize;
    table->cloneKey = cloneKey;
    table->freeKey = freeKey;
    table->hfun = hfun;
    table->printKeyFun = printKeyFun;
    table->printDataFun = printDataFun;
    table->fcomp = fcomp;
    return table;
}

/**
 * @brief Insert an object to the table with key.
 * If all the cells appropriate for this object are full, duplicate the table.
 * If run out of memory, report
 * MEM_OUT and do nothing (the table should stay at the same situation
 * as it was before the duplication).
 * If everything is OK, return true. Otherwise (an error occured) return false;
 */
int insert(TableP table, const void* key, DataP object)
{
    if (key == NULL || object == NULL)
    {
        return 0;
    }
    Node * current = (Node*)malloc(sizeof(Node));
    if (current == NULL)
    {
        reportError(MEM_OUT);
        return 0;
    }
    current->key = table->cloneKey(key);
    current->data = object;
    int arrCell, listNode;
    DataP data = findData(table, key, &arrCell, &listNode);
    // if key already exists:
    if (data != NULL)
    {
        table->group[arrCell].elements[listNode] = current;
        return 1;
    }
    int d = (int)(table->size / table->initialSize);
    int hash = table->hfun(key, table->initialSize);
    int index = d * hash;
    int i;
    int j;
    for (i = index; i < index + d; ++i)
    {
        for (j = 0; j < MAX_ROW_ELEMENTS; ++j)
        {
            if (table->group[i].elements[j] == NULL)
            {
                table->group[i].elements[j] = current;
                table->group[i].size++;
                return 1;
            }
        }
    }
    int isResized = reSize(table);
    // if resizing was not successful:
    if (isResized == 0)
    {
        reportError(MEM_OUT);
        return 0;
    }

    table->group[2 * index + 1].elements[0] = current;
    table->group[2 * index + 1].size++;
    return 1;
}

/**
 * This function is given a table, and it doubles the size of the table, and move all the old
 * groups to the new bigger table to a double index. If the old index is i, in the new table it
 * will be 2*i.
 * @param table the table to make its size double.
 * @return 1 if resize was successful, 0 otherwise.
 */
static int reSize(TableP table)
{
    size_t oldSize = table->size;
    size_t newSize = 2 * oldSize;
    Group * newGroup = (Group*)malloc(newSize * sizeof(Group));
    if (newGroup == NULL)
    {
        return 0;
    }
    unsigned int i;
    unsigned int j;
    for (i = 0; i < oldSize; ++i)
    {
        for (j = 0; j < MAX_ROW_ELEMENTS; ++j)
        {
            newGroup[2 * i].elements[j] = table->group[i].elements[j];
            // initializing the odd places to be null.
            newGroup[2 * i + 1].elements[j] = NULL;
        }
        newGroup[2 * i].size = table->group[i].size;
        newGroup[2 * i + 1].size = 0; // everyone but the last
    }
    free(table->group);
    table->group = newGroup;
    table->size = newSize;
    return 1;
}

/**
 * @brief Print the table (use the format presented in PrintTableExample).
 */
void printTable(const TableP table)
{
    if (table == NULL)
    {
        return;
    }
    unsigned int i;
    unsigned int j;
    for (i = 0; i < table->size; ++i)
    {
        printf("[%d]", i);
        Group * group = &(table->group[i]);
        if (group->size == 0)
        {
            printf("\t\n");
            continue;
        }
        printf("\t");
        for (j = 0; j < MAX_ROW_ELEMENTS; ++j)
        {
            DataP data = getDataAt(table, i, j);
            ConstKeyP key = getKeyAt(table, i, j);
            if (data != NULL && key != NULL)
            {
                table->printKeyFun(key);
                printf(",");
                table->printDataFun(data);
                printf("\t-->\t");
            }
        }
        printf("\n");
    }
}

/**
 * @brief Search the table and look for an object with the given key.
 * If such object is found fill its cell number into arrCell (where 0 is the
 * first cell), and its placement in the list into listNode (when 0 is the
 * first node in the list, i.e. the node that is pointed from the table
 * itself).
 * If the key was not found, fill both pointers with value of -1.
 * return pointer to the data or null
 */
DataP findData(const TableP table, const void* key, int* arrCell, int* listNode)
{
    *arrCell = -1;
    *listNode = -1;
    if (table == NULL || key == NULL)
    {
        return NULL;
    }
    int d = (int)(table->size / table->initialSize);
    int hash = table->hfun(key, table->initialSize);
    int index = d * hash;
    int i;
    int j;
    for (i = index; i < index + d; ++i)
    {
        for (j = 0; j < MAX_ROW_ELEMENTS; ++j)
        {
            ConstKeyP currentKey = getKeyAt(table, i, j);
            if (table->fcomp(key, currentKey) == 0)
            {
                *arrCell = i;
                *listNode = j;
                return table->group[i].elements[j]->data;
            }
        }
    }
    return NULL;
}

/**
 * @brief remove a data from the table.
 * If everything is OK, return the pointer to the ejected data. Otherwise return NULL;
 */
DataP removeData(TableP table, const void* key)
{
    if (table == NULL || key == NULL)
    {
        return NULL;
    }
    int arrCell;
    int listNode;
    DataP removed = findData(table, key, &arrCell, &listNode);
    if (removed == NULL)
    {
        return NULL;
    }
    table->group[arrCell].elements[listNode] = NULL;
    table->group[arrCell].size--;
    return removed;
}
