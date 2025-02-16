/* iaed24 - ist1110263 - project 
This file contains the main command functions*/
#include "project.h"

/*Command related functions*/
/*
Process user commands and execute corresponding actions.

This function reads user commands from the standard input and executes
the corresponding actions based on the command. The supported commands are:
    'q': Quit the program.
    'p': Create a new parking lot.
    'e': Register the entry of a vehicle into the parking lot.
    's': Register the exit of a vehicle from the parking lot.
    'v': List vehicle logs for a specific vehicle.
    'f': List the payments received by specific parks, either all of them or 
    by day.
    'r': Placeholder for future functionality.
*/
void processCommand(Global *global){
    char input[BUFSIZ], command;
    while (TRUE){
        fgets(input, BUFSIZ, stdin);
        command = input[0];
        switch (command){
        case 'q':
            exit_q(global);
            exit(0); // Quit the program
            break;
        case 'p':
            /*input + 2 leaves out the command letter*/
            createParkingLot(global, input + 2); 
            break;
        case 'e':
            carEntry(global, input + 2);
            break;
        case 's':
            carExit(global, input + 2);
            break;
        case 'v':
            listVehicleLogs(global, input + 2);
            break;
        case 'f':
            analiseFInput(global, input + 2);
            break;
        case 'r':
            /*deleteParkingLot(global, input + 2);*/
            break;
        }
            
    }
}

/*
Exit the program and free allocated memory.

This function deallocates memory associated with registry and parking lot
structures before exiting the program. It iterates over each parking lot,
freeing the memory associated with its registry list, and then frees the
memory associated with the parking lot itself. Finally, it sets the global
parking lot head pointer to NULL.

*/
void exit_q(Global *global) {
    ParkingLot *park = global->head;

    /*Free all memory associated to registry structs*/
    while (park != NULL) { 
        Registry *head = park->entryExitRegistry;
        while (head != NULL) {
            Registry *tmp = head;
            head = head->nextRegList;
            free(tmp);
        }
        park->entryExitRegistry = NULL; /* Reset the registry list pointer to 
        NULL after freeing all entries*/

    /*Free all memory associated to parking lot structs*/
        ParkingLot *temp = park;
        park = park->next;
        free(temp->name);
        free(temp);
    }
}

/*Main command functions*/

/*P*/
/*
Creates a new parking lot based on the provided input string.

If the input string is empty, it displays all available parks.

Args:
    global (Global): The global data structure containing information about the 
    parking system.
    input (str): The input string containing information about the new parking 
    lot.

Returns:
    Void
*/
void createParkingLot(Global *global, char input[]){
    int index = strlen(input);
    /*If the command was just "p", this function will only receive an 
    empty string.*/
    if(index == 0){ 
        seeAllAvailableParks(global);
        return;
    }
    
    ParkingLot *newParkingLot = (ParkingLot *)malloc(sizeof(struct ParkingLot));
    newParkingLot->name = readParkName(input);
    int stringChars = removeNameString(input, newParkingLot->name);
    /*Assign the information from the input strings to the various 
    struct fields.*/
    sscanf((input + stringChars), "%d %f %f %f",
       &newParkingLot->capacity, &newParkingLot->price15, 
       &newParkingLot->price15After1, &newParkingLot->maxValue);
    /*Errors*/
    if (invalidPArguments(newParkingLot->name,newParkingLot->capacity, 
    newParkingLot->price15, newParkingLot->price15After1, 
    newParkingLot->maxValue, global) == FALSE){
        free(newParkingLot);
        return;
    }
    
    newParkingLot->availableSpots = newParkingLot->capacity;
    newParkingLot->entryExitRegistry = NULL;
    newParkingLot->next = NULL;

    addParkingLotToList(global, newParkingLot);
    global->ParkingLotsNumber++;
    return;
}

/*E*/
/*
    Records the entry of a vehicle into the parking lot based on the provided 
    input string.

    Args:
        global (Global): The global data structure containing information about 
        the parking system.
        input (str): The input string containing information about the vehicle 
        entry.

    Returns:
        Void
*/
void carEntry(Global *global, char input[]){
    Registry *newRegistry = (Registry *)malloc(sizeof(struct Registry));
    char *parkName = readParkName(input);
    int stringChars = removeNameString(input, parkName);

    sscanf((input + stringChars), "%s %d-%d-%d %d:%d", newRegistry->vehicle, 
    &newRegistry->entryDate.day, &newRegistry->entryDate.month, 
    &newRegistry->entryDate.year, &newRegistry->entryTime.hours, 
    &newRegistry->entryTime.minutes);

    if (invalidEArguments(global, parkName, newRegistry->vehicle, 
    newRegistry->entryDate.day, newRegistry->entryDate.month, 
    newRegistry->entryDate.year, newRegistry->entryTime.hours, 
    newRegistry->entryTime.minutes) == FALSE){
        free(parkName);
        free(newRegistry);
        return;
    }

    /*Initialize all struct fields*/
    newRegistry->type = ENTRY;
    newRegistry->parkingLot = findParkingLot(global, parkName);
    newRegistry->parkingLot->availableSpots -= 1;
    newRegistry->exitDate.year = 0;
    newRegistry->exitDate.month = 0;
    newRegistry->exitDate.day = 0;
    newRegistry->exitTime.hours = 0;
    newRegistry->exitTime.minutes = 0;
    newRegistry->payment = 0;
    newRegistry->nextHash = NULL;
    newRegistry->nextRegList = NULL;
    addRegistryToList(newRegistry, newRegistry->parkingLot);
    updateLastRegistry(global, newRegistry);
    hashTableInsert(newRegistry, global);
    printf("%s %d\n", newRegistry->parkingLot->name, 
    newRegistry->parkingLot->availableSpots);
    free(parkName);
}

/*S*/
/*
Records the exit of a vehicle from the parking lot based on the provided input 
string.

Args:
    global (Global): The global data structure containing information about the 
    parking system.
    input (str): The input string containing information about the vehicle exit.

Returns:
    Void
*/
void carExit(Global *global, char input[]){
    Registry *newRegistry = (Registry *)malloc(sizeof(struct Registry));
    char *parkName = readParkName(input);
    ParkingLot *currentPark = findParkingLot(global, parkName);
    int stringChars = removeNameString(input, parkName);

    sscanf((input + stringChars), "%s %d-%d-%d %d:%d", newRegistry->vehicle, 
    &newRegistry->exitDate.day, &newRegistry->exitDate.month, 
    &newRegistry->exitDate.year, &newRegistry->exitTime.hours, 
    &newRegistry->exitTime.minutes);

    if (invalidSArguments(global, parkName, currentPark, newRegistry->vehicle, 
    newRegistry->exitDate.day, newRegistry->exitDate.month, 
    newRegistry->exitDate.year, newRegistry->exitTime.hours, 
    newRegistry->exitTime.minutes) == FALSE){
        free(parkName);
        free(newRegistry);
        return;
    }
    Registry *entryRegistry = obtainEntryRegistry(global, newRegistry->vehicle);

    copyEntryDate(newRegistry, entryRegistry);
    newRegistry->type = EXIT;
    newRegistry->parkingLot = currentPark;
    currentPark->availableSpots += 1;
    newRegistry->payment = calculateCost(global, newRegistry,  currentPark);
    newRegistry->nextHash = NULL;
    newRegistry->nextRegList = NULL;
    addRegistryToList(newRegistry, currentPark);
    updateLastRegistry(global, newRegistry);
    hashTableInsert(newRegistry, global);

    printf("%s %02d-%02d-%d %02d:%02d %02d-%02d-%02d %02d:%02d %.2f\n", 
    newRegistry->vehicle, newRegistry->entryDate.day,
    newRegistry->entryDate.month, newRegistry->entryDate.year, 
    newRegistry->entryTime.hours, newRegistry->entryTime.minutes,
    newRegistry->exitDate.day, newRegistry->exitDate.month, 
    newRegistry->exitDate.year, newRegistry->exitTime.hours, 
    newRegistry->exitTime.minutes,newRegistry->payment);

    free(parkName);
}

/*V*/
/*
Lists the log of entries and exits for a specific vehicle.

Args:
    global (Global): The global data structure containing information about the 
    parking system.
    input (str): The input string containing the vehicle information.

Returns:
    Void
*/
void listVehicleLogs(Global *global, char input[]){
    char vehicle[9];
    sscanf(input, "%s", vehicle);

    if (invalidVArguments(global, vehicle) == FALSE){
        return;
    }
    /*If the last movement from the vehicle was an entry, 
    I need to temporarily change it to check the non-parked vehicles' 
    information.*/
    Registry *entryRegistry = obtainEntryRegistry(global, vehicle);
    if (entryRegistry->type == ENTRY){
        entryRegistry->type = EXIT;
    }
    Registry **registries = NULL;

    int sizeArray = hashTableLookup(&registries, vehicle, global);
    bubblesort(registries, sizeArray, compareDates);
    bubblesort(registries, sizeArray, compareNames);
    for (int i = 0; i < sizeArray; i++){
        /*Ignores the ENTRY registries*/
        if (registries[i]->type == ENTRY){
            continue;
        }
        /*Since it initially was an entry log, the exit dates are all 0.*/
        else if(registries[i]->exitDate.month == 0){
            printf("%s %02d-%02d-%d %02d:%02d\n", 
            registries[i]->parkingLot->name, registries[i]->entryDate.day, 
            registries[i]->entryDate.month, registries[i]->entryDate.year, 
            registries[i]->entryTime.hours,registries[i]->entryTime.minutes);
        }
        else{
            printf("%s %02d-%02d-%d %02d:%02d %02d-%02d-%d %02d:%02d\n", 
            registries[i]->parkingLot->name, registries[i]->entryDate.day, 
            registries[i]->entryDate.month, registries[i]->entryDate.year, 
            registries[i]->entryTime.hours,registries[i]->entryTime.minutes, 
            registries[i]->exitDate.day, registries[i]->exitDate.month, 
            registries[i]->exitDate.year, registries[i]->exitTime.hours, 
            registries[i]->exitTime.minutes);
        }
    }
    /*Revert the last registry type*/
    entryRegistry->type = ENTRY;
    free(registries);
}

/*F*/
/*
Lists the payment information from a parking lot.

Args:
    global (Global): The global data structure containing information about 
    the parking system.
    input (str): The input string containing the park name and date 
    (if specified).

Returns:
    Void
*/
void analiseFInput(Global *global, char input[]){
    char *parkName = readParkName(input);
    int nameCounter = removeNameString(input, parkName);
    Date date;
    ParkingLot *parkingLot;
    /*If the command is asking for the payment info on a specific date, 
    there will be a space after the park name.*/
    if (input[nameCounter] == ' '){
        parkingLot = findParkingLot(global, parkName);
        if (parkingLot == NULL){
            printf("%s: no such parking.\n", parkName);
            return;
        }
        sscanf((input + nameCounter), "%d-%d-%d", &date.day, &date.month, 
        &date.year);
        if (!invalidDateTimeForF(global, date.day, date.month, date.year)){
            printf("invalid date.\n");
            return;
        }
        listDayPaymentWithVehicle(parkingLot, date.day, date.month, date.year);
        free(parkName);
        return; 
        }
    else{
        parkingLot = findParkingLot(global, parkName);
        if (parkingLot == NULL){
            printf("%s: no such parking.\n", parkName);
            return;
        }
        listDailyPayment(parkingLot);
        free(parkName);
        return;  
    }
}

/*R*/
/*
void deleteParkingLot(Global *global, char input[]){
    char *parkName = readParkName(input);
    ParkingLot *parkingLot = findParkingLot(global, parkName);

    while (parkingLot->tail != NULL){
        deleteRegistryHash(global, parkingLot->tail->vehicle, parkingLot->tail);
    }
    ParkingLot *tmp;
    for (tmp = global->head; tmp != NULL; tmp = parkingLot->next) {
        if (strcmp(tmp->name, parkName) == 0){
            parkingLot->prev = parkingLot->next;
            free(tmp->name);
            free(tmp);
            break;
        }
    }
    free(parkName);
}
*/

/*Main*/
int main(){
    Global global;
    global.head = NULL;
    global.tail = NULL;
    global.ParkingLotsNumber = 0;
    initHashTable(&global);
    global.dateoflastregistry.day = 0;
    global.dateoflastregistry.month = 0;
    global.dateoflastregistry.year = 0;
    global.timeoflastregistry.hours = 0;
    global.timeoflastregistry.minutes = 0;
    processCommand(&global);
    return 0;
}
