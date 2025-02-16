/* iaed24 - ist1110263 - project 
This file contains the functions that are related to the parking lots and their 
names*/
#ifndef parks_and_park_names_c
#include "project.h"

/*Park related functions*/

/*
Checks if the arguments provided for creating a parking lot are valid.

Args:
    name (str): The name of the parking lot.
    capacity (int): The capacity of the parking lot.
    price15 (float): The price for the first 15 minutes of parking.
    price15After1 (float): The price for each additional 15 minutes after the 
    first hour.
    maxValue (float): The maximum price per 24 hours.
    global (Global): The global data structure containing information about the 
    parking system.

Returns:
    int: TRUE if the arguments are valid, FALSE otherwise.
*/
int invalidPArguments(char *name, int capacity, float price15, 
float price15After1, float maxValue, Global *global){
    if (comparePark(global, name) == TRUE){
        printf("%s: parking already exists.\n", name);
        free(name);
        return FALSE;
    }

    if (capacity <= 0){
        printf("%d: invalid capacity.\n", capacity);
        free(name);
        return FALSE;
    }

    if ((price15 <= 0) || (price15After1 <= 0) || (maxValue <= 0) || 
    (price15After1 < price15) || (maxValue < price15After1)){
        printf("invalid cost.\n");
        free(name);
        return FALSE;
    }

    if (global->ParkingLotsNumber >= MAXPARKNUM){
        printf("too many parks.\n");
        free(name);
        return FALSE;
    }

    else{
        return TRUE;
    }
}


/*
Adds a parking lot to the global linked list of parking lots.

Args:
    global (Global): The global data structure containing information about 
    the parking system.
    park (ParkingLot): The parking lot to be added to the list.

Returns:
    Void
*/
void addParkingLotToList(Global *global, ParkingLot *park){
    if (global->head == NULL){
        global->head = park;
        global->tail = park;
    }
    else{
        global->tail->next = park;
        global->tail = park;
    }
}


/*
Prints information about all available parking lots in the global linked list.

Args:
    global (Global): The global data structure containing information about 
    the parking system.

Returns:
    Void
*/
void seeAllAvailableParks(Global *global){
    ParkingLot *park = (global->head);

    while (park){
        printf("%s %d %d\n", park->name, park->capacity, park->availableSpots);
        park = park->next;
    }
}

/*
Compares the name of a parking lot with all park names stored in the global 
linked list.

Args:
    global (Global): The global data structure containing information about the 
    parking system.
    name (str): The name of the parking lot to compare.

Returns:
    int: Returns TRUE if the parking lot with the given name is found in the 
    global linked list, otherwise returns FALSE.
*/
int comparePark(Global *global, char *name){
    ParkingLot *park = (global->head);
    int foundpark = FALSE;
    while (park != NULL){
        /*Compare the name from the argument to all the park names 
        stored and check if there's a match.*/
        if (strcmp(park->name, name) == 0){
            foundpark = TRUE;
        }
        park = park->next;
        }
    return foundpark;
}


/*
Finds a parking lot with the given name in the global linked list.

Args:
    global (Global): The global data structure containing information about the 
    parking system.
    name (str): The name of the parking lot to find.

Returns:
    ParkingLot: A pointer to the ParkingLot structure if a parking lot with the 
    given name is found, otherwise returns NULL.
*/
ParkingLot *findParkingLot(Global *global, char* name){
    ParkingLot *park = (global->head);
    while (park != NULL){
        if (strcmp(park->name, name) == 0){
            return park;
        }
        park = park->next;
    }
    return NULL;
}

/*Reading and analizing the park name*/

/*
Reads the name of a parking lot from the input string.

Args:
    name (str): The input string containing the name of the parking lot.

Returns:
    str: The name of the parking lot extracted from the input string, excluding 
    quotation marks if present.
*/
char *readParkName(char name[]){
    int charCounter = 0, i = 0, foundQuotationMark = FALSE;
    if (name == NULL){
        return NULL;
    }
    for(i = 0; i < (int) strlen(name); ++i){
        if(foundQuotationMark && name[i] == '\"'){
            break;
        }
        else if(foundQuotationMark == FALSE && (name[i] == ' ' || 
        name[i] == '\n')){
            break;
        }
        else if(name[i] == '\"'){
            foundQuotationMark = TRUE;
        }
        else{
            ++charCounter;
        }
    }
    char *parkName = (char*) malloc(sizeof(char)*charCounter + 1);
    if (parkName == NULL){
        return NULL;
    }
    strncpy(parkName, name + foundQuotationMark, charCounter);
    parkName[charCounter] = '\0';
    return parkName;
}

/*
Determines the number of characters to skip in the input string to exclude the 
park name.

Args:
    input (str): The input string containing the park name and possibly 
    quotation marks.
    name (str): The park name extracted from the input string.

Returns:
    int: The number of characters to skip in the input string to exclude the 
    park name.
*/
int removeNameString(char input[], char *name){
    int quotationMarks = 0;
    if(input[0] == '\"'){
        quotationMarks = 2;
    }
    return strlen(name) + quotationMarks;
}

/*
Compares the names of two parking lots for alphabetical ordering.

Args:
    reg1 (Registry): The first registry containing the parking lot name to 
    compare.
    reg2 (Registry): The second registry containing the parking lot name to 
    compare.

Returns:
    int: A positive value if the name of reg1 comes after the name of reg2 
    alphabetically, a negative value if it comes before, and 0 if they are 
    equal.
*/
int compareNames(Registry *reg1, Registry *reg2){
    return strcmp(reg1->parkingLot->name, reg2->parkingLot->name) > 0;
}



#endif