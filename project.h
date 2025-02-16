#ifndef MAIN_H
#define MAIN_H

/*Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*Constants*/
#define TRUE 1
#define FALSE 0
#define DOES_NOT_EXIST -1
#define QUARTER_MIN 15
#define MAX_QUARTERS_FOR_X 4
#define MINUTES_IN_DAY 1440
#define TABLESIZE 10007
#define MAXPARKNUM 20
#define MINUTESINHOUR 60
#define HOURSINDAY 24
#define DAYSINYEAR 365

/*Defines the registry as an entry or an exit*/
enum TypeOfRegistry {ENTRY, EXIT};

/*Structs*/
typedef struct ParkingLot ParkingLot;

typedef struct Date{
    int day;
    int month;
    int year;
}Date;

typedef struct Time{
    int hours;
    int minutes;
}Time;

typedef struct Registry {
    enum TypeOfRegistry type; /*If it's an entry or an exit*/
    ParkingLot *parkingLot; /*Pointer to the parking lot that the car is/was 
    in*/
    char vehicle[9]; /*License plate*/
    Date entryDate;
    Time entryTime;
    Date exitDate;
    Time exitTime; /*In ENTRY registries, these are all 0*/
    float payment;
    struct Registry *nextHash; 
    struct Registry *nextRegList;
} Registry;

struct ParkingLot {
    char* name;
    int capacity;
    float price15;
    float price15After1;
    float maxValue;
    int availableSpots;
    Registry *entryExitRegistry;
    Registry *tail;
    struct ParkingLot* prev;
    struct ParkingLot* next;
};

typedef struct Global{
    struct ParkingLot *head;
    struct ParkingLot *tail;
    int ParkingLotsNumber;
    Registry *hashtable[TABLESIZE];
    Date dateoflastregistry;
    Time timeoflastregistry;

} Global;

/*Command related functions*/
void processCommand(Global *global);
void exit_q(Global *global);

/*Main command functions*/
/*found in "project.c"*/
/*P*/
void createParkingLot(Global *global, char input[]);
/*E*/
void carEntry(Global *global, char input[]);
/*S*/
void carExit(Global *global, char input[]);
/*V*/
void listVehicleLogs(Global *global, char vehicle[]);
/*F*/
void analiseFInput(Global *global, char input[]);
/*R*/
/*void deleteParkingLot(Global *global, char input[]);*/

/*Main*/
int main();

/*Park related functions*/
/*found in "parks_and_park_names"*/
int invalidPArguments(char *name, int capacity, float price15, 
float price15after1, float maxvalue, Global *global);
void addParkingLotToList(Global *global, ParkingLot *park);
void seeAllAvailableParks(Global *global);
int comparePark(Global *global, char *name);
ParkingLot *findParkingLot(Global *global, char* name);

/*Reading and analizing the park name.c*/
char* readParkName(char name[]);
int removeNameString(char input[],char *name);
int compareNames(Registry *reg1, Registry *reg2);

/*Vehicle related functions*/
/*found in "vehicle_and_payments.c"*/
int validateLicensePlate(char vehicle[]);
int invalidEArguments(Global *global, char *name,char vehicle[],int day, 
int month, int year, int hours, int minutes);
int invalidSArguments(Global *global, char *name, ParkingLot *parkinglot,
char vehicle[],int day, int month, int year, int hours, int minutes);
int invalidVArguments(Global *global, char vehicle[]);

/*Payment related functions*/
float calculateCost(Global *global, Registry *newRegistry, 
ParkingLot *parkingLot);
float calculateDailyPayment(ParkingLot *parkingLot, int targetDay, 
int targetMonth, int targetYear);
void listDailyPayment(ParkingLot *parkingLot);
void listDayPaymentWithVehicle(ParkingLot *parkingLot, int targetDay, 
int targetMonth, int targetYear);

/*Registry related functions*/
/*found in "registry_and_date_time.c"*/
void updateLastRegistry(Global *global, Registry *newRegistry);
void addRegistryToList(Registry *newRegistry, ParkingLot *parkingLot);
Registry *obtainEntryRegistry(Global *global, char vehicle[]);
/*void changeTailRegistry(ParkingLot *parkingLot);
void deleteRegistryHash(Global *global, char vehicle[], Registry *registry);*/

/*Date and time related functions*/
int invalidDateTime(Global *global, int day, int month, int year, int hours, 
int minutes);
int invalidDateTimeForF(Global *global, int day, int month, int year);
int calculateEntryExitTimeDifference(Registry *reg1, Registry *reg2);
int compareDates(Registry *reg1, Registry *reg2);
int calculateEntryTimeDifference(Registry *reg1, Registry *reg2);
void copyEntryDate(Registry *reg1, Registry *reg2);

/*Hashtable related functions*/
/*found in "hash_and_sort.c"*/
int hash(const char *str);
void initHashTable(Global *global);
int hashTableInsert(Registry *addedRegistry, Global *global);
int hashTableLookup(Registry ***registries, char vehicle[], Global *global);

/*Bubblesort function*/
void bubblesort(Registry **registriesArray, int arrayLength, 
int (*cmp)(Registry *reg1, Registry *reg2));

#endif