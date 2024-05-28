/**
 * @file ex07-address_book.c
 * @author Handy Pan (https://github.com/handyPan/)
 * @brief This code is to practice:
 *          - struct
 *          - CRUD
 *          - file handling
 *          - signal handler
 * @version 0.1
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
    important findings:
    1 - for the binary files, the saved and read data should be the same format, e.g. array[LEN], linked list
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <signal.h>

#define LEN_1 20    // date of birth, phone no., ID no., course count
#define LEN_2 40    // course name, title, industry, company, first name, last name, department
#define LEN_3 60    // address, email

#define PERSON_FILE "person.bin"
#define COURSE_FILE "course.bin"
#define EMPLOYMENT_FILE "employment.bin"
#define PERSON_FILE_TMP "person_tmp.bin"
#define COURSE_FILE_TMP "course_tmp.bin"
#define EMPLOYMENT_FILE_TMP "employment_tmp.bin"

#define INTERRUPT_ALERT     printf("\nPress ENTER to continue, Ctrl + C + ENTER to exit...\n");   \
                            while((getchar()) != '\n');        \
                            fflush(stdin);                      \
                            fflush(stdout);
#define INTERRUPT_CHECK     if (exitFunction)       \
                            {                       \
                                exitFunction = 0;   \
                                system("cls");      \
                                return;             \
                            }

struct Course
{   
    char personIdNo[LEN_1];
    char name[LEN_2];
    char department[LEN_2];
    int year;
    int grade;
};

struct Employment
{
    char personIdNo[LEN_1];
    char title[LEN_2];
    char company[LEN_2];
    char industry[LEN_2];
    char isPermanent;
    float salary;
};

struct Person
{   
    char fName[LEN_2];
    char lName[LEN_2];
    char dob[LEN_1];
    char gender;
    char phNo[LEN_1];
    char address[LEN_3];
    char email[LEN_3];
    char idNo[LEN_1];
    char department[LEN_2];
    int courseAmount;
    char isEmployed;
};

struct PersonNode
{
    struct Person val;
    struct PersonNode *next;
};

struct CourseNode
{
    struct Course val;
    struct CourseNode *next;
};

struct EmploymentNode
{
    struct Employment val;
    struct EmploymentNode *next;
};

struct PersonInfo
{   
    struct Person p;
    int isPersonNull;
    struct Course c[LEN_1];
    struct Employment e;
};

static volatile sig_atomic_t exitFunction = 0;

struct PersonNode *pList, *pListHead;

void menu();
void listRecords();
void createRecord();
void readRecord();
struct PersonInfo _readRecord(char* idNoToSearch);
void updateRecord();
void _updateRecord(char* idNoToUpdate, struct PersonInfo pInfo);
void deleteRecord();
void searchRecords();
void _searchRecords(char* keywordToSearch);
void interruptHandler(int dummy);
int validateId(char* idNoToCreate);


int main()
{   
    // Register signals 
    signal(SIGINT, interruptHandler);

    pList = NULL;
    pListHead = pList;

    system("color 1a");

    menu();

    return 0;
}

void menu()
{   
    char op = '\0';

    fflush(stdin);

    system("cls");

    while (1)
    {   
        fflush(stdin);

        system("cls");

        printf("----- Address Book -----\n\n");

        listRecords();

        printf("Option: 1. Create Record     2. Read Record     3. UpdateRecord     4. Delete Record     5. Search Records     6. Quit\n");
        printf("Select an option: ");

        if (op != '\0') fflush(stdout);
        
        op = getche();

        switch(op)
        {
            case '1':
                createRecord();
                break;
            case '2':
                readRecord();
                break;
            case '3':
                updateRecord();
                break;
            case '4':
                deleteRecord();
                break;
            case '5':
                searchRecords();
                break;
            case '6':
                exit(0);
                break;
            default:
                printf("Input error! Please enter 1 - 6.\n");
        }
    }
}

void listRecords()
{   
    // clear pList before loading records, otherwise, the list will be duplicated
    pList = pListHead;
    struct PersonNode *pTmp;
    while (pList != NULL)
    {
        pTmp = pList;
        pList = pList -> next;
        free(pTmp);
    }
    pList = pListHead = NULL;

    FILE *f_p;
    
    f_p = fopen(PERSON_FILE, "rb");
    
    if (f_p==NULL)
    {
        printf("No records found.\n\n");
        return;
    }

    struct Person p;
    int idx = 0;
    
    printf("%-5s" "%-15s"       "%-15s"      "%-15s"          "%-7s"    "%-15s"      "%-30s"    "%-30s"  "%-15s"        "%-15s"       "%-20s"          "%-20s\n", 
            "SN", "First Name", "Last Name", "Date of Birth", "Gender", "Phone No.", "Address", "Email", "Person ID",  "Department", "Course Amount", "Is Employed");

    while(fread(&p, sizeof(p), 1, f_p) == 1)
    {   
        idx += 1;

        // allocate memory
        if (pList == NULL)
        {
            pList = (struct PersonNode*)malloc(sizeof(struct PersonNode));
            pListHead = pList;
            pList -> val = p;
            pList -> next = NULL;
        }
        else
        {
            pList -> next = (struct PersonNode*)malloc(sizeof(struct PersonNode));
            pList -> next -> val = p;
            pList -> next -> next = NULL;
            pList = pList -> next;
        }

        printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
               idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
    }

    printf("\n");

    if (idx == 0)
    {
        printf("\nNo records found.\n\n");
    }
    
    fflush(stdout);
    fclose(f_p);
}

void createRecord()
{
    system("cls");

    printf("----- Create Record -----\n");

    FILE *f_p, *f_c, *f_e;
    struct Person p;
    struct Course c[LEN_1];
    struct Employment e;

    INTERRUPT_ALERT                 printf("\nEnter first name: ");                             
    INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.fName);                                 
    INTERRUPT_ALERT                 printf("\nEnter last name: ");                              
    INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.lName);                                 
    INTERRUPT_ALERT                 printf("\nEnter date of birth (yyyy/mm/dd): ");             
    INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.dob);                                   
    INTERRUPT_ALERT                 printf("\nEnter gender: ");                                    
    INTERRUPT_CHECK                 scanf("\n%c", &(p.gender));                                    
    INTERRUPT_ALERT                 printf("\nEnter phone no.: ");                              
    INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.phNo);                                  
    INTERRUPT_ALERT                 printf("\nEnter address: ");                                
    INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.address);                               
    INTERRUPT_ALERT                 printf("\nEnter email: ");                                  
    INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.email);
    do
    {
        INTERRUPT_ALERT                 printf("\nEnter ID no.: ");                                 
        INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.idNo);  
    } while (validateId(p.idNo) == 0);
    INTERRUPT_ALERT                 printf("\nEnter department: ");                             
    INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.department);                            
    INTERRUPT_ALERT                 printf("\nEnter course amount (<=%d): ", LEN_1);            
    INTERRUPT_CHECK                 scanf("\n%d", &(p.courseAmount));                           
    
    if (p.courseAmount > 0)
    {   
        for (int i = 0; i < p.courseAmount; i++)
        {   
            strcpy(c[i].personIdNo, p.idNo);
            INTERRUPT_ALERT                 printf("\nEnter course name: ");
            INTERRUPT_CHECK                 scanf("\n%[^\n]s",c[i].name);
            INTERRUPT_ALERT                 printf("\nEnter department that offers the course: ");
            INTERRUPT_CHECK                 scanf("\n%[^\n]s",c[i].department);
            INTERRUPT_ALERT                 printf("\nEnter the year of the course: ");
            INTERRUPT_CHECK                 scanf("\n%d", &(c[i].year));
            INTERRUPT_ALERT                 printf("\nEnter the grade of the course: ");
            INTERRUPT_CHECK                 scanf("\n%d", &(c[i].grade));
        }

        for (int i = p.courseAmount; i < LEN_1; i++)
        {   
            strcpy(c[i].personIdNo, p.idNo);
            strcpy(c[i].name, "");
            strcpy(c[i].department, "");
            c[i].year = 0;
            c[i].grade = 0;
        }
    }

    do
    {
        printf("\nIs the person employed (Y/N): ");
        scanf("\n%c", &(p.isEmployed));
        if (p.isEmployed != 'Y' && p.isEmployed != 'N')
        {
            printf("Input error! Only Y and N allowed!");
        }
    } while (p.isEmployed != 'Y' && p.isEmployed != 'N');
    
    if (p.isEmployed == 'Y')
    {
        strcpy(e.personIdNo, p.idNo);
        INTERRUPT_ALERT                 printf("\nEnter employee title: ");
        INTERRUPT_CHECK                 scanf("\n%[^\n]s",e.title);
        INTERRUPT_ALERT                 printf("\nEnter employee company: ");
        INTERRUPT_CHECK                 scanf("\n%[^\n]s",e.company);
        INTERRUPT_ALERT                 printf("\nEnter employee industry: ");
        INTERRUPT_CHECK                 scanf("\n%[^\n]s",e.industry);
        do
        {
            printf("\nIs the employment permanent (Y/N): ");
            scanf("\n%c", &(e.isPermanent));
            if (e.isPermanent != 'Y' && e.isPermanent != 'N')
            {
                printf("Input error! Only Y and N allowed!");
            }
        } while (e.isPermanent != 'Y' && e.isPermanent != 'N');
        INTERRUPT_ALERT                 printf("\nEnter employee salary: ");
        INTERRUPT_CHECK                 scanf("\n%f", &(e.salary));
    }

    // open files and save data
    if (p.isEmployed == 'Y')
    {
        f_e = fopen(EMPLOYMENT_FILE, "ab+");
        if (f_e == NULL)
        {
            printf("Fail to open %s", EMPLOYMENT_FILE);
            return;
        }
        // save to EMPLOYMENT_FILE
        if (fwrite(&e, sizeof(e), 1, f_e) != 1)
        {
            printf("Fail to write to %s", EMPLOYMENT_FILE);
            fflush(stdin);
            fclose(f_e);
            return;
        }
        fflush(stdin);
        fclose(f_e);
    }

    if (p.courseAmount > 0)
    {
        f_c = fopen(COURSE_FILE, "ab+");
        if (f_c == NULL)
        {
            printf("Fail to open %s", COURSE_FILE);
            return;
        }
        // save to COURSE_FILE
        if (fwrite(&c, sizeof(c), 1, f_c) != 1)
        {
            printf("Fail to write to %s", COURSE_FILE);
            fflush(stdin);
            fclose(f_c);
            return;
        }
        fflush(stdin);
        fclose(f_c);
    }
    
    f_p = fopen(PERSON_FILE, "ab+");
    if (f_p == NULL)
    {
        printf("Fail to open %s", PERSON_FILE);
        return;
    }
    // save to PERSON_FILE
    if (fwrite(&p, sizeof(p), 1, f_p) != 1)
    {
        printf("Fail to write to %s", PERSON_FILE);
        fflush(stdin);
        fclose(f_p);
        return;
    }
    fflush(stdin);
    fclose(f_p);
        
    printf("\nRecords saved!");

    Sleep(2000);
}

void readRecord()
{
    system("cls");

    printf("----- Read Record -----\n");

    char idNoToSearch[LEN_1];
    printf("\nEnter ID no.: ");
    scanf("\n%[^\n]s",idNoToSearch);
    printf("\nSearch records for Person ID = %s\n\n", idNoToSearch);

    _readRecord(idNoToSearch);

    printf("\nPress any key to continue...\n");
    getche();
}

struct PersonInfo _readRecord(char* idNoToSearch)
{
    FILE *f_p, *f_c, *f_e;
    struct Person p;
    struct Course c[LEN_1];
    struct Employment e;
    struct PersonInfo p_info; 
    p_info.isPersonNull = 1; 

    // search record by ID
    int idx = 0;
    pList = pListHead;
    while(pList != NULL)
    {   
        idx += 1;
        if (strcmp(pList -> val.idNo, idNoToSearch) == 0)
        {   
            p = pList -> val;
            p_info.p = p;
            p_info.isPersonNull = 0;
            printf("- PROFILE\n");
            printf("%-5s" "%-15s"       "%-15s"      "%-15s"          "%-7s"    "%-15s"      "%-30s"    "%-30s"  "%-15s"       "%-15s"       "%-20s"          "%-20s\n", 
                    "SN", "First Name", "Last Name", "Date of Birth", "Gender", "Phone No.", "Address", "Email", "Person ID",  "Department", "Course Amount", "Is Employed");
            printf("%-5d" "%-15s"  "%-15s"  "%-15s" "%-7c"    "%-15s"  "%-30s"    "%-30s"   "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                    idx,  p.fName, p.lName, p.dob,  p.gender, p.phNo,  p.address, p.email,  p.idNo,  p.department, p.courseAmount, p.isEmployed);
            
            printf("\n");

            if (p.courseAmount > 0) 
            {   
                // read courses from COURSE_FILE
                FILE *f_c;
    
                f_c = fopen(COURSE_FILE, "rb");
                
                if (f_c==NULL)
                {
                    printf("Fail to open %s\n\n", COURSE_FILE);
                    return p_info;
                }

                struct Course c_tmp[LEN_1];

                while (fread(&c_tmp, sizeof(c_tmp), 1, f_c) == 1)
                {   
                    if (strcmp(c_tmp[0].personIdNo, idNoToSearch) == 0)
                    {
                        memcpy(c, c_tmp, sizeof(c_tmp));
                        break;
                    }
                }

                // array can't be assigned directly, use memcpy() to assign
                memcpy(p_info.c, c, sizeof(c));

                // display courses
                printf("- COURSE\n");
                printf("%-5s" "%-20s"        "%-20s"         "%-20s"        "%-10s"  "%-15s\n", 
                        "SN", "Person ID", "Course Name", "Department", "Year", "Grade");
                for (int i = 0; i < p.courseAmount; i++)
                {
                    printf("%-5d" "%-20s"            "%-20s"     "%-20s"           "%-10d"     "%-15d\n", 
                            i+1,  c[i].personIdNo, c[i].name, c[i].department, c[i].year, c[i].grade);
                }

                fflush(stdout);
                fclose(f_c);
            }

            printf("\n");

            if (p.isEmployed == 'Y')
            {
                // read employment from EMPLOYMENT_FILE
                FILE *f_e;
    
                f_e = fopen(EMPLOYMENT_FILE, "rb");
                
                if (f_e==NULL)
                {
                    printf("Fail to open %s\n\n", EMPLOYMENT_FILE);
                    return p_info;
                }

                struct Employment e_tmp;

                while (fread(&e_tmp, sizeof(e_tmp), 1, f_e) == 1)
                {   
                    if (strcmp(e_tmp.personIdNo, idNoToSearch) == 0)
                    {
                        e = e_tmp;
                        p_info.e = e;
                        break;
                    }
                }

                // display employment
                printf("- EMPLOYMENT\n");
                printf("%-5s" "%-20s"            "%-20s"         "%-20s"           "%-20s"           "%-15s"            "%-10s\n",
                       "SN", "Person ID",      "Title",       "Company",       "Industry",      "Is Permanent",   "Salary");
                printf("%-5d" "%-20s"            "%-20s"         "%-20s"           "%-20s"           "%-15c"            "%-15f\n", 
                        1,   e.personIdNo,     e.title,       e.company,       e.industry,      e.isPermanent,    e.salary);
                
                fflush(stdout);
                fclose(f_e);    
            }

            break;
        }

        pList = pList -> next;

    }

    

    if (pList == NULL)
    {
        printf("Total %d records. No records for Person ID = %s found.\n\n", idx, idNoToSearch);
        p_info.isPersonNull = 1; 
        pList = pListHead;

        Sleep(2000);

        return p_info;
    }
    
    pList = pListHead;
    return p_info;
}

void updateRecord()
{
    system("cls");

    printf("----- Update Record -----\n");

    char idNoToUpdate[LEN_1];
    printf("\nEnter ID no.: ");
    scanf("\n%[^\n]s",idNoToUpdate);
    printf("\nUpdate records for Person ID = %s\n\n", idNoToUpdate);

    // print the current record details
    struct PersonInfo pInfo = _readRecord(idNoToUpdate);

    if (pInfo.isPersonNull != 1)
    {
        _updateRecord(idNoToUpdate, pInfo);
    }
    else
    {
        printf("\nPress any key to continue...\n");
        getche();
    }
}

// input each property again like createRecord() does
// if courseAmount is changed, alert to input the courses again, if input
//      - 0, delete the courses under the personID
//      - <> 0, delete the original courses and input new courses
// if isEmployed is changed, alert to input isEmployed again, if input 
//      - N, delete the isEmployed under the personID
//      - Y, re-enter the employment info
void _updateRecord(char* idNoToUpdate, struct PersonInfo pInfo)
{   
    FILE *f_p, *f_c, *f_e;
    struct Person p;
    struct Course c[LEN_1]; 
    struct Employment e;
    int cntCourse;

    // initialization
    p = pInfo.p;
    if (p.courseAmount > 0)
    {
        memcpy(c, pInfo.c, sizeof(pInfo.c));
    }
    for (int i = p.courseAmount; i < LEN_1; i++)
    {   
        strcpy(c[i].personIdNo, p.idNo);
        strcpy(c[i].name, "");
        strcpy(c[i].department, "");
        c[i].year = 0;
        c[i].grade = 0;
    }
    if (p.isEmployed == 'Y')
    {
        e = pInfo.e;
    }
    else
    {
        strcpy(e.personIdNo, "");
        strcpy(e.title, "");
        strcpy(e.company, "");
        strcpy(e.industry, "");
        e.isPermanent = ' ';
        e.salary = 0;
    }
    cntCourse = pInfo.p.courseAmount;
    
    while (1)
    {   
        int modificationDone = 0;
        printf("\n%s"             "%s"              "%s"                  "%s"           "%s"               "%s"            "%s"          "%s"               "%s"                  "%s"                 "%s\n", 
               "1. First Name  ", "2. Last Name  ", "3. Date of Birth  ", "4. Gender  ", "5. Phone No.  ",  "6. Address  ", "7. Email  ", "8. Department  ", "9. Course Amount  ", "10. Is Employed  ", "11. Done or Cancel Update");
        printf("\nChoose which item to update: ");

        int op;
        scanf("\n%d", &op);

        if (op < 1 || op > 11) 
        {
            printf("\nError with selection. Exit now.\n");
            Sleep(2000);
            return;
        }

        switch (op)
        {
            case 1:
                INTERRUPT_ALERT                 printf("\nEnter first name (Current Value: %s): ", p.fName);
                INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.fName);
                break;
            case 2:
                INTERRUPT_ALERT                 printf("\nEnter last name (Current Value: %s): ", p.lName);
                INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.lName);
                break;
            case 3:
                INTERRUPT_ALERT                 printf("\nEnter date of birth (yyyy/mm/dd) (Current Value: %s): ", p.dob);
                INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.dob);
                break;
            case 4:
                INTERRUPT_ALERT                 printf("\nEnter gender (Current Value: %c): ", p.gender);
                INTERRUPT_CHECK                 scanf("\n%c", &(p.gender)); 
                break;
            case 5:
                INTERRUPT_ALERT                 printf("\nEnter phone no. (Current Value: %s): ", p.phNo);  
                INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.phNo);
                break;
            case 6:
                INTERRUPT_ALERT                 printf("\nEnter address (Current Value: %s): ", p.address);
                INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.address); 
                break;
            case 7:
                INTERRUPT_ALERT                 printf("\nEnter email (Current Value: %s): ", p.email);
                INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.email);
                break;
            case 8:
                INTERRUPT_ALERT                 printf("\nEnter department (Current Value: %s): ", p.department);
                INTERRUPT_CHECK                 scanf("\n%[^\n]s",p.department);
                break;
            case 9:
                printf("\n");

                // cases for courses
                // 1 - append extra courses
                // 2 - modify previous courses
                // 3 - delete some or all of previous courses
                
                while (1)
                {   
                    int courseModificationDone = 0;
                    // print current courses then ask for operations
                    printf("\nCurrently, there are %d courses:\n", cntCourse);

                    if (cntCourse > 0)
                    {
                        printf("%-5s" "%-20s"        "%-20s"         "%-20s"        "%-10s"  "%-15s\n", 
                        "SN", "Person ID", "Course Name", "Department", "Year", "Grade");
                        for (int i = 0; i < cntCourse; i++)
                        {
                            printf("%-5d" "%-20s"            "%-20s"     "%-20s"           "%-10d"     "%-15d\n", 
                                    i+1,  c[i].personIdNo, c[i].name, c[i].department, c[i].year, c[i].grade);
                        }
                    }

                    printf("1 - Done or Cancel\n");
                    printf("2 - Append a course\n");
                    if (cntCourse > 0)
                    {
                        printf("3 - Modify a course\n");
                        printf("4 - Delete a course\n");
                    }
                    
                    printf("Choose an option: ");
                    int op;
                    scanf("\n%d", &op);

                    switch (op)
                    {   
                        case 1:
                            courseModificationDone = 1;
                            break;
                        case 2:
                            if (cntCourse + 1 > LEN_1) 
                            {
                                printf("Maximum courses is %d", LEN_1);
                                break;
                            }
                            strcpy(c[cntCourse].personIdNo, p.idNo);
                            INTERRUPT_ALERT                 printf("\nEnter course name: ");
                            INTERRUPT_CHECK                 scanf("\n%[^\n]s",c[cntCourse].name);
                            INTERRUPT_ALERT                 printf("\nEnter department that offers the course: ");
                            INTERRUPT_CHECK                 scanf("\n%[^\n]s",c[cntCourse].department);
                            INTERRUPT_ALERT                 printf("\nEnter the year of the course: ");
                            INTERRUPT_CHECK                 scanf("\n%d", &(c[cntCourse].year));
                            INTERRUPT_ALERT                 printf("\nEnter the grade of the course: ");
                            INTERRUPT_CHECK                 scanf("\n%d", &(c[cntCourse].grade));
                            cntCourse++;
                            break;
                        case 3:
                            if (cntCourse == 0)
                            {
                                printf("\nError. No courses available to modify.\n");
                                break;
                            }
                            printf("\nEnter the SN of the course to modify (1 - %d): ", cntCourse);
                            int cSN;
                            scanf("\n%d", &cSN);
                            if (cSN < 1 || cSN > cntCourse)
                            {
                                printf("Error with selection. \n");
                                break;
                            }
                            strcpy(c[cSN-1].personIdNo, p.idNo);
                            INTERRUPT_ALERT                 printf("\nEnter course name (Current Value: %s): ", c[cSN-1].name);
                            INTERRUPT_CHECK                 scanf("\n%[^\n]s",c[cSN-1].name);
                            INTERRUPT_ALERT                 printf("\nEnter department that offers the course (Current Value: %s): ", c[cSN-1].department);
                            INTERRUPT_CHECK                 scanf("\n%[^\n]s",c[cSN-1].department);
                            INTERRUPT_ALERT                 printf("\nEnter the year of the course (Current Value: %d): ", c[cSN-1].year);
                            INTERRUPT_CHECK                 scanf("\n%d", &(c[cSN-1].year));
                            INTERRUPT_ALERT                 printf("\nEnter the grade of the course (Current Value: %d): ", c[cSN-1].grade);
                            INTERRUPT_CHECK                 scanf("\n%d", &(c[cSN-1].grade));
                            break;
                        case 4:
                            if (cntCourse == 0)
                            {
                                printf("\nError. No courses available to delete.\n");
                                break;
                            }
                            printf("\nEnter the SN of the course to delete (1 - %d): ", cntCourse);
                            int toDeleteCourseSN;
                            scanf("\n%d", &toDeleteCourseSN);
                            if (toDeleteCourseSN < 1 || toDeleteCourseSN > cntCourse)
                            {
                                printf("Error with selection. \n");
                                break;
                            }

                            // delete the course in c[LEN_1]
                            // case 1, in total 1 out of 20 courses, delete course 1, c[0] = c[1], ... , c[18] = c[19], c[19] = 0
                            // case 2, in total 7 out of 20 courses, delete course 3, c[2] = c[3], ... , c[18] = c[19], c[19] = 0
                            // case 3, in total 20 out of 20 courses, delete course 11, c[10] = c[11], ..., c[18] = c[19], c[19] = 0
                            for (int i = toDeleteCourseSN; i <= LEN_1 - 1; i++)
                            {
                                strcpy(c[i-1].personIdNo, c[i].personIdNo);
                                strcpy(c[i-1].name, c[i].name);
                                strcpy(c[i-1].department, c[i].department);
                                c[i-1].year = c[i].year;
                                c[i-1].grade = c[i].grade;
                            }
                            strcpy(c[LEN_1 - 1].personIdNo, p.idNo);
                            strcpy(c[LEN_1 - 1].name, "");
                            strcpy(c[LEN_1 - 1].department, "");
                            c[LEN_1 - 1].year = 0;
                            c[LEN_1 - 1].grade = 0;
                            cntCourse--;
                            break;
                        default:
                            break;
                    }

                    if (courseModificationDone) break;
                }  
                
                p.courseAmount = cntCourse;
                break;
            case 10:
                printf("\n");

                do
                {
                    printf("\nIs the person employed (Y/N) (Current Value: %c): ", p.isEmployed);
                    scanf("\n%c", &(p.isEmployed));
                    if (p.isEmployed != 'Y' && p.isEmployed != 'N')
                    {
                        printf("Input error! Only Y and N allowed!");
                    }
                } while (p.isEmployed != 'Y' && p.isEmployed != 'N');

                if (p.isEmployed == 'Y')
                {
                    strcpy(e.personIdNo, p.idNo);
                    INTERRUPT_ALERT                 printf("\nEnter employee title (Current Value: %s): ", e.title);
                    INTERRUPT_CHECK                 scanf("\n%[^\n]s",e.title);
                    INTERRUPT_ALERT                 printf("\nEnter employee company (Current Value: %s): ", e.company);
                    INTERRUPT_CHECK                 scanf("\n%[^\n]s",e.company);
                    INTERRUPT_ALERT                 printf("\nEnter employee industry (Current Value: %s): ", e.industry);
                    INTERRUPT_CHECK                 scanf("\n%[^\n]s",e.industry);
                    do
                    {
                        printf("\nIs the employment permanent (Y/N) (Current Value: %c): ", e.isPermanent);
                        scanf("\n%c", &(e.isPermanent));
                        if (e.isPermanent != 'Y' && e.isPermanent != 'N')
                        {
                            printf("Input error! Only Y and N allowed!");
                        }
                    } while (e.isPermanent != 'Y' && e.isPermanent != 'N');
                    INTERRUPT_ALERT                 printf("\nEnter employee salary (Current Value: %f): ", e.salary);
                    INTERRUPT_CHECK                 scanf("\n%f", &(e.salary));
                }
                break;
            case 11:
                modificationDone = 1;
                break;
            default:
                return;
        }

        printf("\nThe record will be updated to:\n");
        printf("%-15s"        "%-15s"      "%-15s"          "%-7s"    "%-15s"      "%-30s"    "%-30s"  "%-15s"        "%-15s"      "%-20s"         "%-20s\n", 
                "First Name", "Last Name", "Date of Birth", "Gender", "Phone No.", "Address", "Email", "Person ID",  "Department", "Course Amount", "Is Employed");
        printf("%-15s"  "%-15s"  "%-15s" "%-7c"    "%-15s"  "%-30s"    "%-30s"  "%-15s" "%-15s"       "%-20d"         "%-20c\n", 
               p.fName, p.lName, p.dob,  p.gender, p.phNo,  p.address, p.email, p.idNo, p.department, p.courseAmount, p.isEmployed);
        
        if (modificationDone) break;
    }

     // write data to files

     // case 1 - no course.bin, employment.bin exists
     // case 2 - .bin exists, no current id record exists
     // case 3 - .bin exists, current id record exists

    // employment
    if ((p.isEmployed != pInfo.p.isEmployed) ||(p.isEmployed == 'Y' && pInfo.p.isEmployed == 'Y'))
    {   
        FILE *f_e_tmp;
        f_e = fopen(EMPLOYMENT_FILE, "rb");

        if (f_e == NULL)
        {
            printf("%s does not exist, now create it.\n\n", EMPLOYMENT_FILE);
            Sleep(2000);
            f_e_tmp = fopen(EMPLOYMENT_FILE_TMP, "wb");
            if (f_e_tmp == NULL)
            {
                printf("Fail to open %s", EMPLOYMENT_FILE_TMP);
                return;
            }
            if (fwrite(&e, sizeof(e), 1, f_e_tmp) != 1)
            {
                printf("Fail to write to %s", EMPLOYMENT_FILE_TMP);
                fflush(stdin);
                fclose(f_e_tmp);
                return;
            }
            fflush(stdin);
            fclose(f_e_tmp);
        }
        else
        {
            // read e each time, skip the original record if it exists
            struct Employment e_tmp;
            int personIdFoundInFile = 0;
            while(fread(&e_tmp, sizeof(e_tmp), 1, f_e) == 1)
            {   
                if (strcmp(e_tmp.personIdNo, p.idNo) == 0)
                {   
                    // modify the content accordingly and then write
                    personIdFoundInFile = 1;
                    if (p.isEmployed == 'N')    continue;
                    e_tmp = e;
                }

                // write records of other personIdNo to EMPLOYMENT_FILE_TMP
                
                f_e_tmp = fopen(EMPLOYMENT_FILE_TMP, "ab+");
                if (f_e_tmp == NULL)
                {
                    printf("Fail to open %s", EMPLOYMENT_FILE_TMP);
                    return;
                }
                if (fwrite(&e_tmp, sizeof(e_tmp), 1, f_e_tmp) != 1)
                {
                    printf("Fail to write to %s", EMPLOYMENT_FILE_TMP);
                    fflush(stdin);
                    fclose(f_e_tmp);
                    return;
                }
                fflush(stdin);
                fclose(f_e_tmp);
            }

            // the case that one person originally has "N", so the ID is not in .bin (not found)
            if (p.isEmployed == 'Y' && personIdFoundInFile == 0)
            {   
                e_tmp = e;
                f_e_tmp = fopen(EMPLOYMENT_FILE_TMP, "ab+");
                if (f_e_tmp == NULL)
                {
                    printf("Fail to open %s", EMPLOYMENT_FILE_TMP);
                    return;
                }
                if (fwrite(&e_tmp, sizeof(e_tmp), 1, f_e_tmp) != 1)
                {
                    printf("Fail to write to %s", EMPLOYMENT_FILE_TMP);
                    fflush(stdin);
                    fclose(f_e_tmp);
                    return;
                }
                fflush(stdin);
                fclose(f_e_tmp);
            }
        }

        fflush(stdout);
        fclose(f_e);

        // remove original files and rename tmp files
        remove(EMPLOYMENT_FILE);    rename(EMPLOYMENT_FILE_TMP, EMPLOYMENT_FILE);
    }

    // course
    if (!(p.courseAmount == 0 && pInfo.p.courseAmount == 0))
    {   
        FILE *f_c_tmp;
        f_c = fopen(COURSE_FILE, "rb");

        if (f_c == NULL)
        {   
            printf("%s does not exist, now create it.\n\n", COURSE_FILE);
            Sleep(2000);
            f_c_tmp = fopen(COURSE_FILE_TMP, "wb");
            if (f_c_tmp == NULL)
            {
                printf("Fail to open %s", COURSE_FILE_TMP);
                return;
            }
            if (fwrite(&c, sizeof(c), 1, f_c_tmp) != 1)
            {
                printf("Fail to write to %s", COURSE_FILE_TMP);
                fflush(stdin);
                fclose(f_c_tmp);
                return;
            }
            fflush(stdin);
            fclose(f_c_tmp);
        }
        else
        {
            // read c[LEN_1] each time, skip the original record
            struct Course c_tmp[LEN_1];
            int personIdFoundInFile = 0;
            while(fread(&c_tmp, sizeof(c_tmp), 1, f_c) == 1)
            {   
                if (strcmp(c_tmp[0].personIdNo, p.idNo) == 0)
                {   
                    personIdFoundInFile = 1;
                    if (p.courseAmount == 0)    continue;
                    memcpy(c_tmp, c, sizeof(c));
                }

                // write records of other personIdNo to COURSE_FILE_TMP
                
                f_c_tmp = fopen(COURSE_FILE_TMP, "ab+");
                if (f_c_tmp == NULL)
                {
                    printf("Fail to open %s", COURSE_FILE_TMP);
                    return;
                }
                if (fwrite(&c_tmp, sizeof(c_tmp), 1, f_c_tmp) != 1)
                {
                    printf("Fail to write to %s", COURSE_FILE_TMP);
                    fflush(stdin);
                    fclose(f_c_tmp);
                    return;
                }
                fflush(stdin);
                fclose(f_c_tmp);
            }

            // the case that one person originally has "0", so the ID is not in .bin (not found)
            if (p.courseAmount > 0 && personIdFoundInFile == 0)
            {   
                memcpy(c_tmp, c, sizeof(c));
                f_c_tmp = fopen(COURSE_FILE_TMP, "ab+");
                if (f_c_tmp == NULL)
                {
                    printf("Fail to open %s", COURSE_FILE_TMP);
                    return;
                }
                if (fwrite(&c_tmp, sizeof(c_tmp), 1, f_c_tmp) != 1)
                {
                    printf("Fail to write to %s", COURSE_FILE_TMP);
                    fflush(stdin);
                    fclose(f_c_tmp);
                    return;
                }
                fflush(stdin);
                fclose(f_c_tmp);
            }
        }

        fflush(stdout);
        fclose(f_c);
        
        // remove original files and rename tmp files
        remove(COURSE_FILE);    rename(COURSE_FILE_TMP, COURSE_FILE);
    }
    
    // person
    // not like the above to delete and write, append to the bottom of TMP file
    // here modify the node in linked list of person and write to TMP file like deleteRecord() does
    // determine whether to update person

    if (strcmp(p.fName, pInfo.p.fName) != 0 || \
        strcmp(p.lName, pInfo.p.lName) != 0 || \
        strcmp(p.dob, pInfo.p.dob) != 0 || \
        p.gender != pInfo.p.gender || \
        strcmp(p.phNo, pInfo.p.phNo) != 0 || \
        strcmp(p.address, pInfo.p.address) != 0 || \
        strcmp(p.email, pInfo.p.email) != 0 || \
        strcmp(p.department, pInfo.p.department) != 0 || \
        p.courseAmount != pInfo.p.courseAmount || \
        p.isEmployed != pInfo.p.isEmployed)
    {   
        pList = pListHead;

        while (pList != NULL)
        {
            if (strcmp(pList -> val.idNo, p.idNo) == 0)
            {
                strcpy(pList -> val.fName, p.fName);
                strcpy(pList -> val.lName, p.lName);
                strcpy(pList -> val.dob, p.dob);
                pList -> val.gender = p.gender;
                strcpy(pList -> val.phNo, p.phNo);
                strcpy(pList -> val.address, p.address);
                strcpy(pList -> val.email, p.email);
                strcpy(pList -> val.department, p.department);
                pList -> val.courseAmount = p.courseAmount;
                pList -> val.isEmployed = p.isEmployed;

                break;
            }

            pList = pList -> next;
        }

        pList = pListHead;

        if (pList != NULL)
        {
            f_p = fopen(PERSON_FILE_TMP, "wb");
            if (f_p == NULL)
            {
                printf("Fail to open %s", PERSON_FILE_TMP);
                return;
            }

            while (pList != NULL)
            {
                p = pList -> val;
                if (fwrite(&p, sizeof(p), 1, f_p) != 1)
                {
                    printf("Fail to write to %s", PERSON_FILE_TMP);
                    fflush(stdin);
                    fclose(f_p);
                    return;
                }
                pList = pList -> next;
            }

            pList = pListHead;

            fflush(stdin);
            fclose(f_p);

            remove(PERSON_FILE);        rename(PERSON_FILE_TMP, PERSON_FILE);
        }
    }

    printf("Records updated!\n");

    Sleep(2000);
}

void deleteRecord()
{   
    system("cls");

    printf("----- Delete Record -----\n");

    FILE *f_p, *f_c, *f_e;
    struct Person p;
    struct Course c[LEN_1];
    struct Employment e;
    struct EmploymentNode *eList, *eListHead;

    char idNoToDelete[LEN_1];
    printf("\nEnter ID no.: ");
    scanf("\n%[^\n]s",idNoToDelete);
    printf("\nSearch records for Person ID = %s\n\n", idNoToDelete);

    // search record by ID, if found, delete in EMPLOYMENT_FILE, COURSE_FILE, and PERSON_FILE

    // no need to open PERSON_FILE again as it's populated in listRecords()
    int idx = 0;
    pList = pListHead;
    struct PersonNode *pList_pre ;
    pList_pre = pListHead;
    eListHead = eList = NULL;

    // delete the node in the PERSON linked list
    while(pList != NULL)
    {   
        idx += 1;

        // if found the node to delete, change next to the node after the found one
        if (strcmp(pList -> val.idNo, idNoToDelete) == 0)
        {   
            p = pList -> val;
            // if the 1st node is the one to delete
            if (idx == 1)
            {   
                pListHead = pList -> next;
            }
            else
            {
                pList_pre -> next = pList -> next;
            }

            // free the node to delete
            free(pList);
            
            break;
        }

        pList_pre = pList;
        pList = pList -> next;

    }
    
    if (pList == NULL) 
    {
        printf("No records for Person ID = %s found.\n\n", idNoToDelete);

        Sleep(2000);

        return;
    }

    pList = pListHead;
    
    // delete the record if it's found
    if (p.courseAmount > 0) 
    {
        f_c = fopen(COURSE_FILE, "rb");
    
        if (f_c==NULL)
        {
            printf("Fail to open %s\n\n", COURSE_FILE);
            return;
        }
        
        // read c[LEN_1] each time
        while(fread(&c, sizeof(c), 1, f_c) == 1)
        {   
            // determine whether to discard by c[0], skip the courses to delete
            if (strcmp(c[0].personIdNo, idNoToDelete) == 0)
            {
                continue;
            }
            
            // write c[LEN_1] to COURSE_FILE_TMP
            FILE *f_c_tmp;
            f_c_tmp = fopen(COURSE_FILE_TMP, "ab+"); 
            if (f_c_tmp == NULL)
            {
                printf("Fail to open %s", COURSE_FILE_TMP);
                return;
            }
            if (fwrite(&c, sizeof(c), 1, f_c_tmp) != 1)
            {
                printf("Fail to write to %s", COURSE_FILE_TMP);
                fflush(stdin);
                fclose(f_c_tmp);
                return;
            }
            fflush(stdin);
            fclose(f_c_tmp);
        }

        fflush(stdout);
        fclose(f_c);

        remove(COURSE_FILE);        rename(COURSE_FILE_TMP, COURSE_FILE);
    }
    else    // check whether the records in pList has courses, if all has not, delete course.bin
    {
        pList = pListHead;

        int toDeleteCourseFile = 1;

        while(pList != NULL)
        {
            if (pList -> val.courseAmount != 0)
            {   
                toDeleteCourseFile = 0;
                break;
            }

            pList = pList -> next;
        }

        pList = pListHead;

        if (toDeleteCourseFile) remove(COURSE_FILE); 
    }

    // delete the node in the EMPLOYMENT linked list
    // if p is not employed, no need to handle employment.bin
    if (p.isEmployed == 'Y') {
        f_e = fopen(EMPLOYMENT_FILE, "rb");
    
        if (f_e==NULL)
        {
            printf("Fail to open %s\n\n", EMPLOYMENT_FILE);
            return;
        }

        while(fread(&e, sizeof(e), 1, f_e) == 1)
        {   
            // skip the employment to delete
            if (strcmp(e.personIdNo, idNoToDelete) == 0)
            {
                continue;
            }

            // allocate memory
            if (eList == NULL)
            {
                eList = (struct EmploymentNode*)malloc(sizeof(struct EmploymentNode));
                eListHead = eList;
                eList -> val = e;
                eList -> next = NULL;
            }
            else
            {
                eList -> next = (struct EmploymentNode*)malloc(sizeof(struct EmploymentNode));
                eList -> next -> val = e;
                eList -> next -> next = NULL;
                eList = eList -> next;
            }
        }

        eList = eListHead;

        fflush(stdout);
        fclose(f_e);

        // if p is the only record in employment.bin, delete the file; otherwise, same a tmp file and rename
        if (eList == NULL)
        {
            remove(EMPLOYMENT_FILE);
        }
        else
        {
            f_e = fopen(EMPLOYMENT_FILE_TMP, "wb");
            if (f_e == NULL)
            {
                printf("Fail to open %s", EMPLOYMENT_FILE_TMP);
                return;
            }
            
            while (eList != NULL)
            {
                e = eList -> val;
                if (fwrite(&e, sizeof(e), 1, f_e) != 1)
                {
                    printf("Fail to write to %s", EMPLOYMENT_FILE_TMP);
                    fflush(stdin);
                    fclose(f_e);
                    return;
                }
                eList = eList -> next;
            }

            eList = eListHead;

            fflush(stdin);
            fclose(f_e);

            // remove original files and rename tmp files
            remove(EMPLOYMENT_FILE);    rename(EMPLOYMENT_FILE_TMP, EMPLOYMENT_FILE);
        }
    }

    if (pList != NULL)
    {
        f_p = fopen(PERSON_FILE_TMP, "wb");
        if (f_p == NULL)
        {
            printf("Fail to open %s", PERSON_FILE_TMP);
            return;
        }

        while (pList != NULL)
        {
            p = pList -> val;
            if (fwrite(&p, sizeof(p), 1, f_p) != 1)
            {
                printf("Fail to write to %s", PERSON_FILE_TMP);
                fflush(stdin);
                fclose(f_p);
                return;
            }
            pList = pList -> next;
        }

        pList = pListHead;

        fflush(stdin);
        fclose(f_p);

        remove(PERSON_FILE);        rename(PERSON_FILE_TMP, PERSON_FILE);
    }
    else    // pList == NULL, delete PERSON_FILE, otherwise, the last record can't be deleted
    {
        remove(PERSON_FILE);
    }

    // free the memory allocated to linked lists
    struct PersonNode *pTmp;
    struct EmploymentNode *eTmp;

    while (eList != NULL)
    {
        eTmp = eList;
        eList = eList->next;
        free(eTmp);
    }

    while (pList != NULL)
    {
        pTmp = pList;
        pList = pList->next;
        free(pTmp);
    }
            
    printf("\nRecords deleted!");

    Sleep(2000);
}

void searchRecords()
{
    system("cls");

    printf("----- Search Record -----\n");

    char keywordToSearch[LEN_1];
    
    printf("\nEnter the keyword: ");
    fgets(keywordToSearch, sizeof(keywordToSearch), stdin);
    if (keywordToSearch[0] == '\n') {
        strcpy(keywordToSearch, "(undefined)");
    }
    else
    {
        //keywordToSearch[strcspn(keywordToSearch, "\n")] = 0;
        keywordToSearch[strlen(keywordToSearch) - 1] = '\0';
    }
    printf("\nSearch records for  keyword = %s\n\n", keywordToSearch);
    
    printf("\n");

    if (strcmp(keywordToSearch, "(undefined)") == 0)
    {
        listRecords();
    }
    else
    {
        _searchRecords(keywordToSearch);
    }

    printf("\nPress any key to continue...\n");
    getche(); 
}

void _searchRecords(char* keywordToSearch)
{
    printf("\n%s"             "%s"              "%s"                  "%s"           "%s"               "%s"            "%s"          "%s"               "%s"             "%s"                   "%s"                 "%s\n", 
            "1. First Name  ", "2. Last Name  ", "3. Date of Birth  ", "4. Gender  ", "5. Phone No.  ",  "6. Address  ", "7. Email  ", "8. Person ID  ", "9. Department  ", "10. Course Amount  ", "11. Is Employed  ", "12. All above");
    printf("\nChoose which item to search in: ");

    int op;
    scanf("\n%d", &op);

    if (op < 1 || op > 12) 
    {
        printf("\nError with selection. Exit now.\n");
        Sleep(2000);
        return;
    }

    struct Person p;
    int idx = 0;

    printf("%-5s" "%-15s"       "%-15s"      "%-15s"          "%-7s"    "%-15s"      "%-30s"    "%-30s"  "%-15s"        "%-15s"       "%-20s"          "%-20s\n", 
            "SN", "First Name", "Last Name", "Date of Birth", "Gender", "Phone No.", "Address", "Email", "Person ID",  "Department", "Course Amount", "Is Employed");

    switch (op)
    {
        case 1:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.fName, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 2:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.lName, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 3:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.dob, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 4:
            pList = pListHead;
            while (pList != NULL)
            {   
                char gender_str[5] = {pList -> val.gender, '\0'};
                char *sub_str = strstr(gender_str, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 5:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.phNo, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 6:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.address, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 7:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.email, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 8:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.idNo, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 9:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str = strstr(pList -> val.department, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 10:
            pList = pListHead;
            while (pList != NULL)
            {   
                char course_amount_str[5];
                sprintf(course_amount_str, "%d", pList -> val.courseAmount);
                char *sub_str = strstr(course_amount_str, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 11:
            pList = pListHead;
            while (pList != NULL)
            {   
                char is_employed_str[5] = {pList -> val.isEmployed, '\0'};
                char *sub_str = strstr(is_employed_str, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                }
                pList = pList -> next;
            }
            pList = pListHead;
            break;
        case 12:
            pList = pListHead;
            while (pList != NULL)
            {   
                char *sub_str; 

                sub_str = strstr(pList -> val.fName, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                sub_str = strstr(pList -> val.lName, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                sub_str = strstr(pList -> val.dob, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                char gender_str[5] = {pList -> val.gender, '\0'};
                sub_str = strstr(gender_str, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                sub_str = strstr(pList -> val.phNo, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                sub_str = strstr(pList -> val.address, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                sub_str = strstr(pList -> val.email, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                sub_str = strstr(pList -> val.idNo, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                sub_str = strstr(pList -> val.department, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                char course_amount_str[5];
                sprintf(course_amount_str, "%d", pList -> val.courseAmount);
                sub_str = strstr(course_amount_str, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                char is_employed_str[5] = {pList -> val.isEmployed, '\0'};
                sub_str = strstr(is_employed_str, keywordToSearch);
                if (sub_str)
                {
                    idx += 1;
                    p = pList -> val;
                    printf("%-5d" "%-15s"   "%-15s"   "%-15s" "%-7c"    "%-15s" "%-30s"    "%-30s"  "%-15s"  "%-15s"       "%-20d"         "%-20c\n", 
                            idx,   p.fName,  p.lName,  p.dob,  p.gender, p.phNo, p.address, p.email, p.idNo,  p.department, p.courseAmount, p.isEmployed);
                    pList = pList -> next;
                    continue;
                }

                pList = pList -> next;
            }
            pList = pListHead;
            break;
        default:
            break;
    }

    if (idx == 0) printf("\nNo records found.\n\n");
    Sleep(2000);

    return;
}

void interruptHandler(int dummy)
{   
    exitFunction = 1;
    
    signal(SIGINT, interruptHandler);
}

int validateId(char* idNoToCreate)
{   
    pList = pListHead;
    while(pList != NULL)
    {   
        if (strcmp(pList -> val.idNo, idNoToCreate) == 0)
        {   
            printf("%s is not available, please re-enter an ID.\n", idNoToCreate);
            return 0;
        }
        pList = pList -> next;
    }

    return 1;
}