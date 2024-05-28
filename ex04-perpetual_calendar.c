/**
 * @file ex04-perpetual_calendar.c
 * @author Handy Pan (https://github.com/handyPan/)
 * @brief To practice:
 *          - print out strings based on specific justification
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#define MONTH_WIDTH 20
#define MONTH_GAP_WIDTH 4

struct Month 
{
    char *month;
    int num_days;
    int weekly_print_start_date;
    int weekly_print_end_date;
};

struct Month myMonth[12] = {
    { "January", 31, -1, -1},
    { "February", 28, -1, -1},
    { "March", 31, -1, -1},
    { "April", 30, -1, -1},
    { "May", 31, -1, -1},
    { "June", 30, -1, -1},
    { "July", 31, -1, -1},
    { "August", 31, -1, -1},
    { "September", 30, -1, -1},
    { "October", 31, -1, -1},
    { "November", 30, -1, -1},
    { "December", 31, -1, -1}
};

void printSpace(int numSpace);
void printYear(int year, int numCol);
void setFebDays(int year);
void setWeeklyPrintEndDate(int year);

int main()
{   
    int year, numRow, numCol;

    printf("Enter a year (Integer, ex: 2002): ");
    scanf("%d", &year);

    printf("Enter number of columns (1, 2, 3, 4, 6, or 12): ");
    scanf("%d", &numCol);

    if (12 % numCol != 0)
    {
        printf("Please enter 1, 2, 3, 4, 6, or 12!");
        return 0;
    }

    numRow = 12 / numCol;

    // set February days
    setFebDays(year);

    // set weekly date range of 1st week, it will be updated after each print
    setWeeklyPrintDateRange(year);

    printYear(year, numCol);
    
    printf("\n");

    // print each row of months
    for (int i_r = 0; i_r < numRow; i_r++)
    {   
        // print months name
        for (int i_c = 0; i_c < numCol; i_c++)
        {
            printSpace(MONTH_GAP_WIDTH);
            int numSpaceOfMonthName = MONTH_WIDTH - strlen(myMonth[i_r * numCol + i_c].month);
            printSpace(numSpaceOfMonthName / 2);
            printf("%s", myMonth[i_r * numCol + i_c].month);
            printSpace(numSpaceOfMonthName - numSpaceOfMonthName / 2);
            
            if (i_c == numCol - 1)
            {
                printSpace(MONTH_GAP_WIDTH);
                printf("\n");
            }
        }

        // print weekdays name
        for (int i_c = 0; i_c < numCol; i_c++)
        {
            printSpace(MONTH_GAP_WIDTH);
            printf("%s", "Su Mo Tu We Th Fr Sa");

            if (i_c == numCol - 1)
            {
                printSpace(MONTH_GAP_WIDTH);
                printf("\n");
            }
        }

        // print weekly dates based on the stored weekly_print_start_date and weekly_print_end_date, terminate if all printed
        while (1)
        {   
            int isPrintDoneForRow = 1;

            // determine whether it's the last iteration to print the last week row
            for (int i_c = 0; i_c < numCol; i_c++)
            {
                if (i_c == numCol - 1)
                {
                    // determine whether all rows are printed
                    for (int idx = 0; idx < numCol; idx++)
                    {   
                       if (myMonth[i_r * numCol + idx].weekly_print_end_date < myMonth[i_r * numCol + idx].num_days)
                        {   
                            isPrintDoneForRow = 0;
                            break;
                        }
                    }
                }
            }
            
            for (int i_c = 0; i_c < numCol; i_c++)
            {
                printSpace(MONTH_GAP_WIDTH);

                int numSpaceInFrontOfWeeklyDates, numSpaceBehindWeeklyDates;
                if (myMonth[i_r * numCol + i_c].weekly_print_end_date < 7)
                {
                    numSpaceInFrontOfWeeklyDates = (7 - myMonth[i_r * numCol + i_c].weekly_print_end_date) * 3 - 1;
                    numSpaceBehindWeeklyDates = 0;
                }
                else if (myMonth[i_r * numCol + i_c].weekly_print_end_date >= myMonth[i_r * numCol + i_c].num_days &&
                        myMonth[i_r * numCol + i_c].weekly_print_start_date <= myMonth[i_r * numCol + i_c].num_days)
                {
                    numSpaceInFrontOfWeeklyDates = 0;
                    numSpaceBehindWeeklyDates = (7 - (myMonth[i_r * numCol + i_c].num_days 
                                                    - myMonth[i_r * numCol + i_c].weekly_print_start_date + 1)) * 3;
                }
                else if (myMonth[i_r * numCol + i_c].weekly_print_end_date > myMonth[i_r * numCol + i_c].num_days &&
                        myMonth[i_r * numCol + i_c].weekly_print_start_date > myMonth[i_r * numCol + i_c].num_days)
                {
                    numSpaceInFrontOfWeeklyDates = 0;
                    numSpaceBehindWeeklyDates = MONTH_WIDTH;
                }
                else
                {
                    numSpaceInFrontOfWeeklyDates = 0;
                    numSpaceBehindWeeklyDates = 0;
                }

                if (numSpaceInFrontOfWeeklyDates > 0)   printSpace(numSpaceInFrontOfWeeklyDates);

                for (int date = myMonth[i_r * numCol + i_c].weekly_print_start_date; 
                        date <= ((myMonth[i_r * numCol + i_c].weekly_print_end_date >= myMonth[i_r * numCol + i_c].num_days) ? 
                        myMonth[i_r * numCol + i_c].num_days : myMonth[i_r * numCol + i_c].weekly_print_end_date); 
                        date++)
                {
                    if (myMonth[i_r * numCol + i_c].weekly_print_end_date >= 7 && date == myMonth[i_r * numCol + i_c].weekly_print_start_date)
                    {
                        printf("%2d", date);
                    }
                    else
                    {
                        printf("%3d", date);
                    }
                }

                if (numSpaceBehindWeeklyDates > 0)  printSpace(numSpaceBehindWeeklyDates);

                if (i_c == numCol - 1)
                {
                    printSpace(MONTH_GAP_WIDTH);
                    printf("\n");
                }

                // update weekly print start and end dates
                myMonth[i_r * numCol + i_c].weekly_print_start_date = myMonth[i_r * numCol + i_c].weekly_print_end_date + 1;
                myMonth[i_r * numCol + i_c].weekly_print_end_date = myMonth[i_r * numCol + i_c].weekly_print_end_date + 7;
            
            }

            if (isPrintDoneForRow) break;
        }
    }
}

void printSpace(int numSpace)
{
    for (int i = 0; i < numSpace; i++)
    {
        printf(" ");
    } 
}

void printYear(int year, int numCol)
{   
    char yearStr[10];
    sprintf(yearStr, "%d", year);

    int numSpace = MONTH_GAP_WIDTH * (numCol + 1) + MONTH_WIDTH * numCol - strlen(yearStr);
    printSpace(numSpace / 2);

    printf("%s\n", yearStr);

    printSpace(numSpace - numSpace / 2);
}

void setFebDays(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        myMonth[1].num_days = 29;
    }
}

void setWeeklyPrintDateRange(int year)
{   
    int daycode_of_1st_day[12];
    for (int i = 0; i < 12; i++)
    {
        if (i == 0)
        {
            daycode_of_1st_day[i] = (year + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400) % 7;
        }
        else
        {
            daycode_of_1st_day[i] = (daycode_of_1st_day[i-1] + myMonth[i-1].num_days) % 7;
        }

        myMonth[i].weekly_print_start_date = 1;
        myMonth[i].weekly_print_end_date = 7 - daycode_of_1st_day[i];
    }  
}
