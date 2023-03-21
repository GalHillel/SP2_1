#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AdptArray.h"

typedef struct AdptArray_ {
    PElement* elements;
    int size;
    int capacity;
    COPY_FUNC copy_func;
    DEL_FUNC del_func;
    PRINT_FUNC print_func;
} AdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func) {
    PAdptArray arr = (PAdptArray)malloc(sizeof(AdptArray));
    if (!arr) {
        return NULL;
    }
    arr->size = 0;
    arr->capacity = 1;
    arr->copy_func = copy_func;
    arr->del_func = del_func;
    arr->print_func = print_func;
    arr->elements = (PElement*)malloc(sizeof(PElement) * arr->capacity);
    if (!arr->elements) {
        free(arr);
        return NULL;
    }
    return arr;
}

void DeleteAdptArray(PAdptArray arr) {
    if (!arr) {
        return;
    }
    for (int i = 0; i < arr->size; i++) {
        if (arr->elements[i]) {
            arr->del_func(arr->elements[i]);
        }
    }
    free(arr->elements);
    free(arr);
}

Result SetAdptArrayAt(PAdptArray arr, int index, PElement element) {
    if (!arr || index < 0) {
        return FAIL;
    }
    if (index >= arr->capacity) {
        int new_capacity = index + 1;
        PElement* new_elements = (PElement*)realloc(arr->elements, sizeof(PElement) * new_capacity);
        if (!new_elements) {
            return FAIL;
        }
        memset(new_elements + arr->capacity, 0, sizeof(PElement) * (new_capacity - arr->capacity));
        arr->elements = new_elements;
        arr->capacity = new_capacity;
    }
    if (arr->elements[index]) {
        arr->del_func(arr->elements[index]);
    }
    arr->elements[index] = arr->copy_func(element);
    if (!arr->elements[index]) {
        return FAIL;
    }
    if (index >= arr->size) {
        arr->size = index + 1;
    }
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray arr, int index) {
    if (!arr || index < 0 || index >= arr->size) {
        return NULL;
    }
    return arr->copy_func(arr->elements[index]);
}

int GetAdptArraySize(PAdptArray arr) {
    if (!arr) {
        return -1;
    }
    return arr->size;
}

void PrintDB(PAdptArray arr) {
    if (!arr) {
        return;
    }
    printf("Adaptive Array:\n");
    for (int i = 0; i < arr->size; i++) {
        printf("[%d]: ", i);
        if (arr->elements[i]) {
            arr->print_func(arr->elements[i]);
        } else {
            printf("NULL\n");
        }
    }
}
