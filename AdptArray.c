#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdptArray.h"

typedef struct AdptArray_
{
    int size;
    PElement *elements;
    DEL_FUNC deleteFunc;
    COPY_FUNC copyFunc;
    PRINT_FUNC printFunc;
} AdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC deleteFunc, PRINT_FUNC printFunc)
{
    PAdptArray arr = (PAdptArray)malloc(sizeof(AdptArray));
    if (arr == NULL)
    {
        return NULL;
    }
    arr->size = 0;
    arr->elements = NULL;
    arr->deleteFunc = deleteFunc;
    arr->copyFunc = copyFunc;
    arr->printFunc = printFunc;
    return arr;
}

void DeleteAdptArray(PAdptArray arr)
{
    if (arr == NULL)
    {
        return;
    }
    for (int i = 0; i < arr->size; ++i)
    {
        if (arr->elements[i] != NULL)
        {
            arr->deleteFunc(arr->elements[i]);
        }
    }
    free(arr->elements);
    free(arr);
}

Result SetAdptArrayAt(PAdptArray arr, int index, PElement newElement)
{
    if (arr == NULL || newElement == NULL)
    {
        return FAIL;
    }
    PElement *newElements;
    if (index >= arr->size)
    {
        newElements = (PElement *)calloc((index + 1), sizeof(PElement));
        if (newElements == NULL)
        {
            return FAIL;
        }
        for (int i = 0; i < arr->size; i++)
        {
            if (arr->elements[i] != NULL)
            {
                newElements[i] = arr->elements[i];
            }
        }
        free(arr->elements);
        arr->elements = newElements;
    }
    if (arr->elements[index] != NULL)
    {
        arr->deleteFunc(arr->elements[index]);
    }
    arr->elements[index] = arr->copyFunc(newElement);
    arr->size = (index >= arr->size) ? (index + 1) : arr->size;
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray arr, int index)
{
    if (arr == NULL || index < 0 || index >= arr->size)
    {
        return NULL;
    }
    if (arr->elements[index] != NULL)
    {
        return arr->copyFunc(arr->elements[index]);
    }
    return NULL;
}

int GetAdptArraySize(PAdptArray arr)
{
    if (arr == NULL)
    {
        return -1;
    }
    return arr->size;
}

void PrintDB(PAdptArray arr)
{
    if (arr == NULL)
    {
        printf("Invalid array\n");
        return;
    }

    for (int i = 0; i < arr->size; i++)
    {
        if (arr->elements[i] != NULL)
        {
            printf("Element %d:\n", i);
            arr->printFunc(arr->elements[i]);
            printf("\n");
        }
    }
}