/**
 * @file ex03-parking_lot.c
 * @author Handy Pan (https://github.com/handyPan/)
 * @brief To practice:
 *          - array
 *          - time
 *          - random integer generation
 * @version 0.1
 * @date 2024-04-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
Automatic Parking Spot Assigning System

Requirements:
You will be using a 2D array to construct a parking lot with dimension 20*45, with a total 900 parking spaces. 

A car will show up every 1 to 10 seconds, (you will need to use rand function to generate random numbers) and your system will assign them to one of the empty parking spots. When a car shows up, your system will display that “a car has shown up, and it is being assigned to Spot…” 

One car will leave every 30 to 600 seconds, and your system will automatically remove them from their assigned parking spot. When a car leaves, your system will display that “A car has left, and Spot… is now empty.”

The Spot number should be a coordinate at least so that the driver could easily find the position. You can always tell the driver the spot is at which row and which column.

If the parking lot is full and a new car shows up, your system will automatically display that no space is available.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SHOW_UP_TIME_LOW 1
#define SHOW_UP_TIME_HIGH 10
#define LEAVE_TIME_LOW  5               // 30
#define LEAVE_TIME_HIGH 15              // 600
#define SPOT_ROW 5                       // 20
#define SPOT_COLUMN 6                    // 45

int rb (int min, int max);  // return a random integer in [min, max]
int isParkingLotAvailable(int availableSpot[SPOT_ROW * SPOT_COLUMN]);   // return 0 or 1, 0 - not available, 1 - available
int isParkingLotEmpty(int availableSpot[SPOT_ROW * SPOT_COLUMN]);    // return 0 or 1, 0 - not empty, 1 - empty
void assignSpot(int availableSpot[SPOT_ROW * SPOT_COLUMN], int parkingLot[SPOT_ROW][SPOT_COLUMN]);   
void releaseSpot(int availableSpot[SPOT_ROW * SPOT_COLUMN], int parkingLot[SPOT_ROW][SPOT_COLUMN]);
void showAvailableSpot(int availableSpot[SPOT_ROW * SPOT_COLUMN]);
void showParkingLot(int parkingLot[SPOT_ROW][SPOT_COLUMN]);

int main()
{
    int parkingLot[SPOT_ROW][SPOT_COLUMN];
    int availableSpot[SPOT_ROW * SPOT_COLUMN];  // stores the SN of available spots, ex. 3, 10, 21, 45, ...
    time_t startTime, endTime;  // startTime is the reference for each time difference ditermination
    time(&startTime);
    int showUpStartTime = 0, showUpEndTime = 0, leaveStartTime = 0, leaveEndTime = 0;
    int showUpAfterSeconds, leaveAfterSeconds;

    printf("\nAutomatic Parking Spot Assigning System\n\n");
    printf("Press ctrl + c to terminate the program.\n\n");
    
    printf("Initialization...\n");
    
    // initialize the parking lot, 0 - not occupied, 1 - occupied
    for (int i= 0; i < SPOT_ROW; i++)
    {
        for (int j = 0; j < SPOT_COLUMN; j++)
        {
            parkingLot[i][j] = 0;
            availableSpot[i * SPOT_COLUMN + j] = i * SPOT_COLUMN + j + 1;     // spots SN: 1, 2, 3, ...
        }
    }

    showParkingLot(parkingLot);
    showAvailableSpot(availableSpot);
    
    srand(time(NULL));
    sleep(3);
    time(&endTime);
    showUpStartTime = (int)difftime(endTime, startTime);
    showUpAfterSeconds = rb(SHOW_UP_TIME_LOW, SHOW_UP_TIME_HIGH);
    printf("\n\nShow up start time set to: %ld, show up after seconds set to: %d\n", showUpStartTime, showUpAfterSeconds);
    leaveStartTime = (int)difftime(endTime, startTime);
    leaveAfterSeconds = rb(LEAVE_TIME_LOW, LEAVE_TIME_HIGH);
    printf("\nLeave start time set to: %ld, leave after seconds set to: %d\n", leaveStartTime, leaveAfterSeconds);
    
    /*
        the workflow in the while loop is:
        - re-assign the start time and random seconds of "show up" and "leave" if necessary
        - check whether a car shows up and assign if applicable
        - check whether a car leave and release if applicable
    */

    while (1)
    {   
        // re-assign the start time and random seconds of "show up" and "leave" if necessary
        if (showUpStartTime == 0)
        {
            time(&endTime);
            showUpStartTime = (int)difftime(endTime, startTime);
            showUpAfterSeconds = rb(SHOW_UP_TIME_LOW, SHOW_UP_TIME_HIGH);
            printf("\n\nShow up start time set to: %ld, show up after seconds set to: %d\n", showUpStartTime, showUpAfterSeconds);
        }

        if (leaveStartTime == 0)
        {
            time(&endTime);
            leaveStartTime = (int)difftime(endTime, startTime);
            leaveAfterSeconds = rb(LEAVE_TIME_LOW, LEAVE_TIME_HIGH);
            printf("\n\nLeave start time set to: %ld, leave after seconds set to: %d\n", leaveStartTime, leaveAfterSeconds);
        }
        
        // check whether a car shows up
        time(&endTime);
        showUpEndTime = (int)difftime(endTime, startTime);
        
        if (showUpEndTime - showUpStartTime >= showUpAfterSeconds)
        {   
            printf("\nA car shows up after %d seconds from last assign.", showUpAfterSeconds);

            // check whether spots are available, if yes then assign, otherwise give alerts
            if (isParkingLotAvailable(availableSpot))
            {
                assignSpot(availableSpot, parkingLot);
            }
            else
            {
                // alert
                printf(" No space is available! Can't be assigned.\n");
            }

            showParkingLot(parkingLot);
            showAvailableSpot(availableSpot);

            // either the "show up" car is assigned or not, reset showUpStartTime and showUpAfterSeconds
            showUpStartTime = 0;
            showUpAfterSeconds = 0;
        }

        // check whether a car leave
        time(&endTime);
        leaveEndTime = (int)difftime(endTime, startTime);

        if (leaveEndTime - leaveStartTime >= leaveAfterSeconds)
        {   
            printf("\nSystem tries to release a car after %d seconds from last release. ", leaveAfterSeconds);

            // check whether the parking lot is empty, if not then release, otherwise give alerts
            if (isParkingLotEmpty(availableSpot))
            {
                // alert
                printf(" No space is occupied! Can't be released.\n");
            }
            else
            {
                releaseSpot(availableSpot, parkingLot);
            }

            showParkingLot(parkingLot);
            showAvailableSpot(availableSpot);

            // either the "show up" car is assigned or not, reset showUpStartTime and showUpAfterSeconds
            leaveStartTime = 0;
            leaveAfterSeconds = 0;
        }      
    }
}

int rb (int min, int max){
  return rand() % (max - min + 1) + min;
}

int isParkingLotAvailable(int availableSpot[SPOT_ROW * SPOT_COLUMN])
{
    return availableSpot[0] != 0 ? 1 : 0; // availableSpot[] stores the SNs of available spots, if availableSpot[0] == 0, no spots can be assigned
}

int isParkingLotEmpty(int availableSpot[SPOT_ROW * SPOT_COLUMN])
{
    return availableSpot[SPOT_ROW * SPOT_COLUMN - 1] != 0 ? 1 : 0;
}

void assignSpot(int availableSpot[SPOT_ROW * SPOT_COLUMN], int parkingLot[SPOT_ROW][SPOT_COLUMN])
{   
    // the 1st element must be available as the availability has been confirmed, 1st element can be SN: 1 to SPORT_ROW * SPOT_COLUMN
    int minIdx = 0, maxIdx = SPOT_ROW * SPOT_COLUMN - 1;
    for (int i = 0; i < SPOT_ROW * SPOT_COLUMN; i++)
    {
        if (availableSpot[i] == 0)
        {
            maxIdx = i - 1;     // elements 0 to i - 1 stores the available SNs: 1 to SPORT_ROW * SPOT_COLUMN  
            break;
        }
    }

    int toAssignIdx = rb(minIdx, maxIdx);           // ex. 10
    int toAssignSN = availableSpot[toAssignIdx];    // ex. availableSpot[10] stores SN = 27
    
    /*
        assign, ex. SN = 5, (5-1) / 45 = 0, (5-1) % 45 = 4, change parkingLot[0][4], 
                    SN = 45, (45-1) / 45 = 0, (45-1) % 45 = 44, change parkingLot[0][44],
                    SN = 57, (57-1) / 45 = 1, (57-1) % 45 = 11, change parkingLot[1][11]
    */
    parkingLot[(toAssignSN - 1) / SPOT_COLUMN][(toAssignSN - 1) % SPOT_COLUMN] = 1;
    printf(" The car is assigned to Spot(%d, %d).\n", (toAssignSN - 1) / SPOT_COLUMN + 1, (toAssignSN - 1) % SPOT_COLUMN + 1);   // the coordinates starts at 1 for rows and columns

    if (toAssignIdx != maxIdx)
    {
        for (int i = toAssignIdx + 1; i <= maxIdx; i++)
        {
            availableSpot[i - 1] = availableSpot[i];
        }
    }
    availableSpot[maxIdx] = 0;
}

// pick one SN from the ones that has been occupied
void releaseSpot(int availableSpot[SPOT_ROW * SPOT_COLUMN], int parkingLot[SPOT_ROW][SPOT_COLUMN])
{   
    int occupiedSpots[SPOT_ROW * SPOT_COLUMN];

    for (int i = 0; i < SPOT_ROW * SPOT_COLUMN; i++)
    {
        occupiedSpots[i] = 0;
    }

    // populate occupiedSpots
    int idx = 0;
    for (int i = 0; i < SPOT_ROW; i++)
    {
        for (int j = 0; j < SPOT_COLUMN; j++)
        {
            if (parkingLot[i][j] == 1)
            {
                occupiedSpots[idx] = i * SPOT_COLUMN + j + 1;  // store spot SN in occupiedSpots[]
                idx++;
            }
        }
    }

    // randomly pick one SN to release
    int toReleaseIdx = rb(0, idx - 1);
    int toReleaseSN = occupiedSpots[toReleaseIdx];

    // release, ex. SN = 27, 27 / 20 = 1, 27 % 20 = 7, change parkingLot[1][7]
    parkingLot[(toReleaseSN - 1) / SPOT_COLUMN][(toReleaseSN - 1) % SPOT_COLUMN] = 0;
    printf("A car has left, and Spot(%d, %d) is now empty.\n", (toReleaseSN - 1) / SPOT_COLUMN + 1, (toReleaseSN - 1) % SPOT_COLUMN + 1);

    // reorganize availableSpot, append toReleaseSN to availableSpot
    for (int i = 0; i < SPOT_ROW * SPOT_COLUMN; i++)
    {
        if (availableSpot[i] == 0)
        {   
            availableSpot[i] = toReleaseSN;
            break;
        }
    }
}

void showAvailableSpot(int availableSpot[SPOT_ROW * SPOT_COLUMN])
{   
    printf("\nAvailable spots are: \n");
    for (int i = 0; i < SPOT_ROW * SPOT_COLUMN; i++)
    {   
        if (availableSpot[i] == 0) return;
        printf("%d ", availableSpot[i]);
    }
}

void showParkingLot(int parkingLot[SPOT_ROW][SPOT_COLUMN])
{
    printf("\nParking Lot allocation is: \n");
    printf("R\\C");
    for (int i = 0; i < SPOT_COLUMN; i++)
    {
        printf("%3d", i + 1);
    }
    printf("\n");
    for (int i = 0; i < SPOT_ROW; i++)
    {   
        printf("%3d", i + 1);
        for (int j = 0; j < SPOT_COLUMN; j++)
        {
            printf("%3d", parkingLot[i][j]);
        }
        printf("\n");
    }
}
