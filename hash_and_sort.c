/* iaed24 - ist1110263 - project 
This file contains the functions related to the hashtable and the bubblesort 
algorithm*/

#ifndef hash_and_sort_c
#include "project.h"

/*Hashtable related functions*/
/*
Generates a hash value for the given string.

Args:
    str (const char*): Pointer to the string for which the hash value is 
    calculated.

Returns:
    int: The calculated hash value.
*/
int hash(const char *str) {
    unsigned long hash_value = 5381; 
    int c;

    while ((c = *str++)) {
        hash_value = ((hash_value << 5) + hash_value) + c; 
    }

    return hash_value % TABLESIZE; 
}

/*
Initializes the hash table by setting all entries to NULL.

Args:
    global (Global*): Pointer to the global structure containing the hash table.

Returns: 
    Void
*/
void initHashTable(Global *global){
    for (int i = 0; i < TABLESIZE; i++){
        global->hashtable[i] = NULL;
    }
    /*Table is empty*/
}

/*
Inserts a registry into the hash table.

Args:
    addedRegistry (Registry*): Pointer to the registry to be inserted.
    global (Global*): Pointer to the global structure containing the hash table.

Returns:
    int: Returns TRUE if the insertion is successful, FALSE otherwise.
*/
int hashTableInsert(Registry *addedRegistry, Global *global){
    if (addedRegistry == NULL){
        return FALSE;
    }
    int index = hash(addedRegistry->vehicle);
    Registry *head = global->hashtable[index];
    
    addedRegistry->nextHash = head;
    global->hashtable[index] = addedRegistry;
    return TRUE;
}

/*
Looks up registries associated with a vehicle in the hash table.

Parameters:
    registries (Registry***): Pointer to a pointer to an array of Registry 
    pointers to store the found registries.
    vehicle (char[]): The vehicle string for which registries are being looked 
    up.
    global (Global*): Pointer to the global structure containing the hash table.

Returns:
    int: The number of registries found for the given vehicle.
*/
int hashTableLookup(Registry ***registries, char vehicle[], Global *global) {
    Registry *head = global->hashtable[hash(vehicle)];
    int count = 0, size = 0;

    if (head == NULL) {
            (*registries) = NULL;
            return 0;
    }

    for (Registry *tmp = head; tmp != NULL; tmp = tmp->nextHash) {
        if (strcmp(tmp->vehicle, vehicle) == 0) {
            size++;
        }
    }

    (*registries) = (Registry **)malloc(size * sizeof(Registry *));

    /*Fill registries with matching entries*/
    for (Registry *tmp = head; tmp != NULL; tmp = tmp->nextHash) {
        if (strcmp(tmp->vehicle, vehicle) == 0) {
            (*registries)[count++] = tmp;
        }
    }

    return size;
}

/*Bubblesort function*/

/*
Sorts an array of Registry pointers using the bubble sort algorithm.

Args:
    registries (Registry**): Pointer to an array of Registry pointers to be 
    sorted.
    arrayLength (int): The length of the array.
    cmp (int (*)(Registry*, Registry*)): Pointer to the comparison function 
    that defines the sorting order.

Returns:
    Void
*/
void bubblesort(Registry **registries, int arrayLength, 
int (*cmp)(Registry *reg1, Registry *reg2)){
    int swapped;
    Registry *tmp;
    for (int i = 0; i < arrayLength - 1; i++) {
        swapped = FALSE;
        for (int j = 0; j < arrayLength - i - 1; j++) {
            if ((*cmp)(registries[j], registries[j + 1])) {
                tmp = registries[j];
                registries[j] = registries[j + 1];
                registries[j + 1] = tmp;
                swapped = TRUE;
            }
        }
 
        /* If no two elements were swapped
        by inner loop, then break*/
        if (swapped == FALSE)
            break;
    }
}

#endif