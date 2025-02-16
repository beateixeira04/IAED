/* iaed24 - ist1110263 - project 
This file contains the functions that are related to the vehicles and payments*/
#ifndef vehicle_and_payments_c
#include "project.h"

/*Vehicle related functions*/

/*
Validates a license plate based on specific criteria.

Args:
    vehicle (str): The license plate to validate.

Returns:
    int: TRUE if the license plate is valid, FALSE otherwise.
*/
int validateLicensePlate(char *vehicle) {
    int i, counter = 0;
    int numDigits = 0, numLetters = 0;
    int hasDigits = FALSE, hasLetters = FALSE, itsLetter = FALSE, 
    itsDigit = FALSE;
    
    for (i = 0; vehicle[i] != '\0'; i++) {
        if (isdigit(vehicle[i])) {
            /*Invalidate pairs consisting of letters and numbers.*/
            if(itsLetter == TRUE) return FALSE;
            counter++;
            if(counter > 2) return FALSE;
            numDigits++;
            hasDigits = TRUE;
            itsDigit = TRUE;
        }
        else if (isupper(vehicle[i])) {
            if(itsDigit == TRUE) return FALSE;
            counter++;
            if(counter > 2) return FALSE;
            numLetters++;
            hasLetters = TRUE;
            itsLetter = TRUE;
        }
        else if (vehicle[i] != '-') {
            return FALSE;
        }
        else if (vehicle[i] == '-') {
            counter = 0;
            itsLetter = FALSE;
            itsDigit = FALSE;
        }
    }
    
    /*Must have at least one pair of letters and one pair of numbers.*/
    if (numDigits >= 2 && numLetters >= 2 && hasDigits && hasLetters) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
Checks if the arguments provided for a car entry are valid.

Args:
    global (Global): Pointer to the global structure.
    name (str): Name of the parking lot.
    vehicle (str): License plate of the vehicle.
    day (int): Day of the entry date.
    month (int): Month of the entry date.
    year (int): Year of the entry date.
    hours (int): Hours of the entry time.
    minutes (int): Minutes of the entry time.

Returns:
    int: TRUE if all arguments are valid, FALSE otherwise.
*/
int invalidEArguments(Global *global, char *name,char vehicle[],int day, 
int month, int year, int hours, int minutes){
    ParkingLot *parkingLot = findParkingLot(global, name);
    Registry *tmp;
    Registry *head = global->hashtable[hash(vehicle)];
    if (comparePark(global, name) == FALSE){
        printf("%s: no such parking.\n", name);
        return FALSE;
    }
    if (parkingLot->availableSpots == 0){
        printf("%s: parking is full.\n", name);
        return FALSE;
    }
    if (validateLicensePlate(vehicle) == FALSE){
        printf("%s: invalid licence plate.\n", vehicle);
        return FALSE;
    }
    for(tmp = head; tmp != NULL; tmp = tmp->nextHash){
        if (strcmp(tmp->vehicle, vehicle) == 0){
            /*If a vehicle doesn't have the "EXIT" type, it implies 
            it's already parked, and therefore cannot enter a park.*/
            if(tmp->type != EXIT){
                printf("%s: invalid vehicle entry.\n", vehicle);
                return FALSE;
            }
            else{
                break;
            }
        }
    }
    if(invalidDateTime(global, day, month, year, hours, minutes)== FALSE){
        printf("invalid date.\n");
        return FALSE;
    }
    return TRUE;
}

/*
Checks if the arguments provided for a car exit are valid.

Args:
    global (Global): Pointer to the global structure.
    name (str): Name of the parking lot.
    parkingLot (ParkingLot): Pointer to the parking lot structure.
    vehicle (str): License plate of the vehicle.
    day (int): Day of the exit date.
    month (int): Month of the exit date.
    year (int): Year of the exit date.
    hours (int): Hours of the exit time.
    minutes (int): Minutes of the exit time.

Returns:
    int: TRUE if all arguments are valid, FALSE otherwise.
*/
int invalidSArguments(Global *global, char *name, ParkingLot *parkingLot,
char vehicle[],int day, int month, int year, int hours, int minutes){
    Registry *head = global->hashtable[hash(vehicle)];
    Registry *tmp;
    if (parkingLot == NULL){
        printf("%s: no such parking.\n", name);
        return FALSE;
    }
    if (validateLicensePlate(vehicle) == FALSE){
        printf("%s: invalid licence plate.\n", vehicle);
        return FALSE;
    }

    if(head == NULL){
        printf("%s: invalid vehicle exit.\n", vehicle);
            return FALSE;
    }

    for(tmp = head; tmp != NULL; tmp = tmp->nextHash){
        if (strcmp(tmp->vehicle, vehicle) == 0){
            /*If a vehicle doesn't have the "ENTRY" type, it implies it's 
            not parked, and therefore cannot exit a park.*/
            if(tmp->type != ENTRY){
                printf("%s: invalid vehicle exit.\n", vehicle);
                return FALSE;
            }
            else{
                break;
            }
        }
    }
    if(invalidDateTime(global, day, month, year, hours, minutes)== FALSE){
        printf("invalid date.\n");
        return FALSE;
    }
    return TRUE;
}

/*
Checks if the arguments provided for listing vehicle logs are valid.

Args:
    global (Global): Pointer to the global structure.
    vehicle (str): License plate of the vehicle.

Returns:
    int: TRUE if all arguments are valid, FALSE otherwise.
*/
int invalidVArguments(Global *global, char vehicle[]){
    Registry *head = global->hashtable[hash(vehicle)];
    if (validateLicensePlate(vehicle) == FALSE){
        printf("%s: invalid licence plate.\n", vehicle);
        return FALSE;
    }
    if(head == NULL){
        printf("%s: no entries found in any parking.\n", vehicle);
            return FALSE;
    }
    return TRUE;
}

/*Payment related functions*/

/*
Calculates the parking cost for a vehicle based on the time parked and parking 
lot pricing.

Args:
    global (Global): Pointer to the global structure.
    newRegistry (Registry): Pointer to the new entry registry.
    parkingLot (ParkingLot): Pointer to the parking lot where the vehicle 
    is parked.

Returns:
    float: Total parking cost for the vehicle.
*/
float calculateCost(Global *global, Registry *newRegistry, 
ParkingLot *parkingLot){
    int timeParkedMin;
    Registry *entryRegistry = obtainEntryRegistry(global, newRegistry->vehicle);
    timeParkedMin = calculateEntryExitTimeDifference(newRegistry, entryRegistry);
    float cost = 0;

    cost += parkingLot->maxValue * (timeParkedMin / MINUTES_IN_DAY);
    timeParkedMin = timeParkedMin % MINUTES_IN_DAY;

    int numQuarterHours = timeParkedMin / QUARTER_MIN;
    float dayCost = 0;
    if (timeParkedMin % QUARTER_MIN != 0) numQuarterHours++;
    if (numQuarterHours >= MAX_QUARTERS_FOR_X){
        dayCost += parkingLot->price15 * MAX_QUARTERS_FOR_X;
        dayCost += parkingLot->price15After1 * (numQuarterHours - 
        MAX_QUARTERS_FOR_X);
    } 
    else {
        dayCost += parkingLot->price15 * numQuarterHours;
    }

    /* If the cost is higher than the daily cost, the daily cost is charged */
    if (dayCost > parkingLot->maxValue){
        dayCost = parkingLot->maxValue;
    }
    return cost+dayCost;
}

/*
Calculates the total payment for a specific day in a parking lot.

Args:
    parkingLot (ParkingLot): Pointer to the parking lot.
    targetDay (int): The day for which payment is to be calculated.
    targetMonth (int): The month for which payment is to be calculated.
    targetYear (int): The year for which payment is to be calculated.

Returns:
    float: Total payment for the specified day.
*/
float calculateDailyPayment(ParkingLot *parkingLot, int targetDay, 
int targetMonth, int targetYear){
    Registry *head = parkingLot->entryExitRegistry;
    float sumPayments = 0;

    while (head != NULL) {
        /*Check if the registry date matches the target day*/
        if (head->exitDate.day == targetDay && 
        head->exitDate.month == targetMonth && 
        head->exitDate.year == targetYear) {
            /*If it finds the destined date, it will enter another loop, 
            given that all dates are ordered.*/
            sumPayments += head->payment;
            head = head->nextRegList;
            while(head != NULL){
                if (head->exitDate.day == targetDay && 
                head->exitDate.month == targetMonth && 
                head->exitDate.year == targetYear){
                    sumPayments += head->payment;
                    head = head->nextRegList;
                }
                else if(head->type == EXIT && 
                (head->exitDate.day != targetDay || 
                head->exitDate.month != targetMonth || 
                head->exitDate.year != targetYear)){
                    return sumPayments;
                }
                else{
                    head = head->nextRegList;
                }
            }
        }
        /*Move to the next registry*/
        else{
            head = head->nextRegList;
        }
    }
    return sumPayments;
}

/*
Lists the daily payment for each day in the parking lot.

Args:
    parkingLot (ParkingLot): Pointer to the parking lot.

Returns:
    Void
*/
void listDailyPayment(ParkingLot *parkingLot){
    Registry *head = parkingLot->entryExitRegistry;
    Date currentDate;
    currentDate.day = head->entryDate.day;
    currentDate.month = head->entryDate.month;
    currentDate.year = head->entryDate.year;
    while (head != NULL) {
        if (head->exitDate.day == currentDate.day && 
        head->exitDate.month == currentDate.month && 
        head->exitDate.year == currentDate.year) {
            /*Print the sum of payments for the previous day*/
            float dayPayment = calculateDailyPayment(parkingLot, 
            currentDate.day, currentDate.month, currentDate.year);
            if (dayPayment != 0){
                printf("%02d-%02d-%02d %.2f\n", currentDate.day, 
                currentDate.month, currentDate.year, dayPayment);
            }
            while (head != NULL){
                if (head->type == EXIT && 
                (head->exitDate.day != currentDate.day || 
                head->exitDate.month != currentDate.month || 
                head->exitDate.year != currentDate.year)){
                    currentDate.day = head->exitDate.day;
                    currentDate.month = head->exitDate.month;
                    currentDate.year = head->exitDate.year;
                    break;
                }
                else{
                    head = head->nextRegList;
                }
            }
        }
        else if (head->type == EXIT && (head->exitDate.day != currentDate.day || 
        head->exitDate.month != currentDate.month || 
        head->exitDate.year != currentDate.year)){
            currentDate.day = head->exitDate.day;
            currentDate.month = head->exitDate.month;
            currentDate.year = head->exitDate.year;
        } 
        else{
            head = head->nextRegList;
        }
    }
}

/*
Lists the payment details for each vehicle on a specific day in the parking lot.

Args:
    parkingLot (ParkingLot): Pointer to the parking lot.
    targetDay (int): The day for which payment details are to be listed.
    targetMonth (int): The month for which payment details are to be listed.
    targetYear (int): The year for which payment details are to be listed.

Returns:
    Void
*/
void listDayPaymentWithVehicle(ParkingLot *parkingLot, int targetDay, 
int targetMonth, int targetYear){
    Registry *head = parkingLot->entryExitRegistry;

    while (head != NULL) {
        /*Check if the registry date matches the target day*/
        if (head->exitDate.day == targetDay && 
        head->exitDate.month == targetMonth && 
        head->exitDate.year == targetYear) {
            printf("%s %02d:%02d %.2f\n", head->vehicle, head->exitTime.hours, 
            head->exitTime.minutes, head->payment);
            head = head->nextRegList;

            while(head != NULL){
                if (head->exitDate.day == targetDay && 
                head->exitDate.month == targetMonth && 
                head->exitDate.year == targetYear){
                    printf("%s %02d:%02d %.2f\n", head->vehicle, 
                    head->exitTime.hours, head->exitTime.minutes, 
                    head->payment);
                    head = head->nextRegList;
                }
                else if(head->type == EXIT && 
                (head->exitDate.day != targetDay || 
                head->exitDate.month != targetMonth || 
                head->exitDate.year != targetYear)){
                    return;
                }
                else{
                    head = head->nextRegList;
                }
            }
        }
        // Move to the next registry
        else{
            head = head->nextRegList;
        }
    }
}

#endif