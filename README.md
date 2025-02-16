# IAED
Projeto da cadeira de introdução a algoritmos e estruturas de dados - 1º ano de faculdade

Parking Management System (IAED Project 1 - 2023/24)
Overview

This project implements a Parking Management System that allows the creation and management of parking lots, vehicle entries and exits, and billing functionalities. The system supports commands for creating parking lots, registering vehicle entries and exits, querying vehicle usage, displaying parking lot billing, and removing parking lots.
Features:

    Create Parking Lots: Define parking lots with a name, capacity, and billing system.
    Vehicle Entry and Exit: Record vehicle entries and exits with date and time.
    Vehicle Usage History: Query the history of entries and exits for a specific vehicle.
    Billing: Calculate and display the total billing of a parking lot or by specific date.
    Remove Parking Lots: Remove parking lots from the system along with their associated data.

Grade

Grade: 17.57/20

Commands

    q: Terminate the program.
    p [<name> <capacity> <rate1> <rate2> <max_daily>]: Create or list parking lots.
    e <lot> <license_plate> <date> <time>: Register a vehicle entry.
    s <lot> <license_plate> <date> <time>: Register a vehicle exit.
    v <license_plate>: List all entries and exits of a vehicle.
    f <lot> [<date>]: Show the billing of a parking lot (daily or specific date).
    r <lot>: Remove a parking lot from the system.

Requirements

    C language (standard libraries: stdio.h, stdlib.h, ctype.h, string.h)
    GCC for compilation with the flags: -O3 -Wall -Wextra -Werror -Wno-unused-result

Compilation

To compile the program, run:

gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj1 *.c

Running the Program

Execute the program with:

./proj1 < input_file > output_file

Test the output with:

diff expected_output_file output_file
