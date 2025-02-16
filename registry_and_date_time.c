/* iaed24 - ist1110263 - project 
This file contains functions that are related to registries and manipulation of
dates and times*/
#ifndef registry_and_date_time_c
#include "project.h"

const int _daysOfMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 
334};

/*Registry related functions*/

/*
Updates the global variables storing the date and time of the last registry.

Args:
    global (Global): Pointer to the global structure.
    newRegistry (Registry): Pointer to the newly added registry.

Returns:
    Void
*/
void updateLastRegistry(Global *global, Registry *newRegistry){
    if (newRegistry->type == ENTRY){
        global->dateoflastregistry.day = newRegistry->entryDate.day;
        global->dateoflastregistry.month = newRegistry->entryDate.month;
        global->dateoflastregistry.year = newRegistry->entryDate.year;
        global->timeoflastregistry.hours = newRegistry->entryTime.hours;
        global->timeoflastregistry.minutes = newRegistry->entryTime.minutes;
    }
    else{
        global->dateoflastregistry.day = newRegistry->exitDate.day;
        global->dateoflastregistry.month = newRegistry->exitDate.month;
        global->dateoflastregistry.year = newRegistry->exitDate.year;
        global->timeoflastregistry.hours = newRegistry->exitTime.hours;
        global->timeoflastregistry.minutes = newRegistry->exitTime.minutes;
    }
}


/*
Adds a new registry to the entry/exit registry list of a parking lot.

Args:
    newRegistry (Registry): Pointer to the new registry to be added.
    parkingLot (ParkingLot): Pointer to the parking lot structure.

Returns:
    Void
*/
void addRegistryToList(Registry *newRegistry, ParkingLot *parkingLot) {
    if (parkingLot->entryExitRegistry == NULL) {
        /* If the list is empty, set both head and tail to the new node*/
        parkingLot->entryExitRegistry = newRegistry;
        parkingLot->tail = newRegistry;
    } else {
        /*Otherwise, append the new node to the end and update the tail pointer*/
        parkingLot->tail->nextRegList = newRegistry;
        parkingLot->tail = newRegistry;
    }
}


/*
Finds and returns the most recent entry registry of a vehicle stored in the 
global hashtable.

Args:
    global (Global): Pointer to the global structure containing the hashtable.
    vehicle (str): The license plate of the vehicle to find the entry registry 
    for.

Returns:
    Registry: Pointer to the entry registry of the specified vehicle, or None 
    if not found.
*/
Registry *obtainEntryRegistry(Global *global, char vehicle[]) {
    Registry *head = global->hashtable[hash(vehicle)];
    Registry *entryRegistry = NULL; // Initialize to NULL
    Registry *tmp;
    
    for (tmp = head; tmp != NULL; tmp = tmp->nextHash) {
        if (strcmp(tmp->vehicle, vehicle) == 0) {
            entryRegistry = tmp;
            break;
        }
    }
    
    return entryRegistry;
}

/**/

/*
void changeTailRegistry(ParkingLot *parkingLot){
    Registry *prev;
    Registry *head = (parkingLot->entryExitRegistry);
    for(prev = head; prev->nextRegList->nextRegList != NULL; 
    prev = prev->nextRegList);
    parkingLot->tail = prev;
}

void deleteRegistryHash(Global *global, char vehicle[], Registry *registry){
    Registry *head = global->hashtable[hash(vehicle)];
    Registry *prev = NULL;
    Registry *tmp = NULL;
    for (tmp = head; tmp != NULL; tmp = tmp->nextHash) {
        // Check if the current registry matches the one to be deleted
        if (strcmp(tmp->parkingLot->name, registry->parkingLot->name) == 0) {
            // If the previous node is NULL, it means the matching node is the head
            if (prev == NULL) {
                // Update the head of the linked list
                global->hashtable[hash(vehicle)] = tmp->nextHash;
            } else {
                // Link the previous node to the next node, bypassing the current node
                prev->nextHash = tmp->nextHash;
            }
            
            // Delete the tail registry associated with the parking lot
            changeTailRegistry(tmp->parkingLot);
            free(tmp);
            printf("Registry deleted successfully.\n");
            return;
        }

        // Update the prev pointer for the next iteration
        prev = tmp;
    }
}
*/


/*Date and time related functions*/

/*
Checks if the given date and time are valid.

Args:
    global (Global): Pointer to the global structure containing last registered 
    registry date and time.
    day (int): The day of the month.
    month (int): The month of the year.
    year (int): The year.
    hours (int): The hour of the day.
    minutes (int): The minute of the hour.

Returns:
    int: TRUE if the date and time are valid, FALSE otherwise.
*/
int invalidDateTime(Global *global, int day, int month, int year, int hours, 
int minutes){
    /*Check for impossible date and time values.*/
    if(year < 0 || month <= 0 || month > 12){
        return FALSE;
    }
    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || 
    month == 10 || month == 12){
        if(day < 0 || day > 31){
            return FALSE;
        }
    }
    else if(month == 2){
        if(day < 0 || day > 28){
            return FALSE;
        }
    }
    else{
        if(day < 0 || day > 30){
            return FALSE;
        }
    }
    if(hours < 0 || hours > 23 || minutes < 0 || minutes > 59){
        return FALSE;
    }

    /*Check if it's after the last registered registry.*/
    if (year != global->dateoflastregistry.year) 
    return year > global->dateoflastregistry.year;
    if (month != global->dateoflastregistry.month) 
    return month > global->dateoflastregistry.month;
    if (day != global->dateoflastregistry.day) 
    return day > global->dateoflastregistry.day;
    if (hours != global->timeoflastregistry.hours) 
    return hours > global->timeoflastregistry.hours;
    if (minutes != global->timeoflastregistry.minutes) 
    return minutes > global->timeoflastregistry.minutes;
    return TRUE;
}


/*
Checks if the given date is valid for retrieving payment information.

This function is specifically used for validating dates when retrieving payment 
information for a specific date.

Args:
    global (Global): Pointer to the global structure containing last registered 
    registry date and time.
    day (int): The day of the month.
    month (int): The month of the year.
    year (int): The year.

Returns:
    int: TRUE if the date is valid, FALSE otherwise.
*/
int invalidDateTimeForF(Global *global, int day, int month, int year){
    /*Check for impossible date and time values.*/
    if(year < 0 || month <= 0 || month > 12){
        return FALSE;
    }
    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || 
    month == 10 || month == 12){
        if(day < 0 || day > 31){
            return FALSE;
        }
    }
    else if(month == 2){
        if(day < 0 || day > 28){
            return FALSE;
        }
    }
    else{
        if(day < 0 || day > 30){
            return FALSE;
        }
    }
    /*Check if it's after the last registered registry.*/
    if (year != global->dateoflastregistry.year) 
    return year < global->dateoflastregistry.year;
    if (month != global->dateoflastregistry.month) 
    return month < global->dateoflastregistry.month;
    if (day != global->dateoflastregistry.day) 
    return day < global->dateoflastregistry.day;
    return TRUE;
}


/*
This function calculates the time difference between the entry time of reg2 and 
the exit time of reg1.

Args:
    reg1 (Registry): The first registry containing exit time information.
    reg2 (Registry): The second registry containing entry time information.

Returns:
    int: The time difference in minutes.
*/
int calculateEntryExitTimeDifference(Registry *reg1, Registry *reg2) {
    int totalMinutesEntry = 0, totalMinutesExit = 0, totalDaysEntry = 0, 
    totalDaysExit = 0, totalEntry = 0, totalExit = 0;

    /*Convert both times to minutes*/
    totalMinutesEntry = reg2->entryTime.hours * MINUTESINHOUR + reg2->entryTime.minutes;
    totalMinutesExit = reg1->exitTime.hours * MINUTESINHOUR + reg1->exitTime.minutes;

    /*Convert both dates to days*/
    totalDaysEntry = reg2->entryDate.year * DAYSINYEAR + 
    _daysOfMonth[reg2->entryDate.month - 1] + reg2->entryDate.day;
    totalDaysExit = reg1->exitDate.year * DAYSINYEAR + 
    _daysOfMonth[reg1->exitDate.month - 1] + reg1->exitDate.day;

    /*Calculate the total minutes for both dates/times*/
    totalEntry = totalDaysEntry * HOURSINDAY * MINUTESINHOUR + totalMinutesEntry;
    totalExit = totalDaysExit * HOURSINDAY * MINUTESINHOUR + totalMinutesExit;

    /*Calculate the difference in minutes*/
    int differenceMinutes = abs(totalExit - totalEntry);

    return differenceMinutes;
}

/*
Compares two registry entries based on their dates.

This function compares two registry entries based on their dates. 
It returns TRUE if the entry time of reg1 is later than the entry time of reg2, 
and FALSE otherwise.

Args:
    reg1 (Registry): The first registry entry.
    reg2 (Registry): The second registry entry.

Returns:
    int: TRUE if the entry time of reg1 is later than the entry time of reg2, 
    FALSE otherwise.
*/
int compareDates(Registry *reg1, Registry *reg2){
    return calculateEntryTimeDifference(reg1, reg2) > 0;
}

/*
Calculates the time difference between the entry dates of two registry entries.

This function calculates the time difference, in minutes, between the entry 
dates of two registry entries. It considers both the date and time components 
of the entries.

Args:
    reg1 (Registry): The first registry entry.
    reg2 (Registry): The second registry entry.

Returns:
    int: The time difference, in minutes, between the entry dates of the two 
    registry entries.
*/
int calculateEntryTimeDifference(Registry *reg1, Registry *reg2){
/*Similar to `calculateEntryExitTimeDifference`, this function calculates 
the difference between two entry dates from different registries.*/
    int totalMinutesEntry = 0, totalMinutesExit = 0, totalDaysEntry = 0, 
    totalDaysExit = 0, totalEntry = 0, totalExit = 0;

    totalMinutesEntry = reg2->entryTime.hours * MINUTESINHOUR + reg2->entryTime.minutes;
    totalMinutesExit = reg1->entryTime.hours * MINUTESINHOUR + reg1->entryTime.minutes;

    totalDaysEntry = reg2->entryDate.year * DAYSINYEAR + 
    _daysOfMonth[reg2->entryDate.month - 1] + reg2->entryDate.day;
    totalDaysExit = reg1->entryDate.year * DAYSINYEAR + 
    _daysOfMonth[reg1->entryDate.month - 1] + reg1->entryDate.day;

    totalEntry = totalDaysEntry * HOURSINDAY * MINUTESINHOUR + totalMinutesEntry;
    totalExit = totalDaysExit * HOURSINDAY * MINUTESINHOUR + totalMinutesExit;

    int differenceMinutes = abs(totalExit - totalEntry);

    return differenceMinutes;
}

/*
Copies the entry date and time from one registry to another.

Args:
    reg1 (Registry*): Pointer to the destination registry where the entry 
    date and time will be copied.
    reg2 (Registry*): Pointer to the source registry from which the entry 
    date and time will be copied.

Returns:
    Void
*/
void copyEntryDate(Registry *reg1, Registry *reg2){
    reg1->entryDate.day = reg2->entryDate.day;
    reg1->entryDate.month = reg2->entryDate.month;
    reg1->entryDate.year = reg2->entryDate.year;
    reg1->entryTime.hours = reg2->entryTime.hours;
    reg1->entryTime.minutes = reg2->entryTime.minutes;
}

#endif