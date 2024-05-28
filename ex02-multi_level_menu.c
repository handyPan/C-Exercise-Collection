/**
 * @file ex02-multi_level_menu.c
 * @author Handy Pan (https://github.com/handyPan/)
 * @brief To practice:
 *          - use of malloc()
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>

char* op;
int* lv;
char* lv_path;

int main()
{   
    int menuLevelCount;
    printf("Enter the count of menu level (>=2): ");
    scanf("\n%d", &menuLevelCount);

    // allocate memory for the dynamic array
    op = (char*)malloc(menuLevelCount * sizeof(char));
    lv = (int*)malloc(menuLevelCount * sizeof(int));
    lv_path = (char*)malloc(menuLevelCount * 10);

    if (op == NULL || lv == NULL || lv_path == NULL)
    {
        printf("Fail to allocate memory. Press any key to quit.");
        system("pause");
        exit(0);
    }
    else
    {   
        // initialize arrays: op, lv
        for (int i = 0; i < menuLevelCount; i++)
        {
            op[i] = -1;
            lv[i] = -1;
        }

        menu(0, menuLevelCount);
    }

    return 0;
}

void menu(int lv_idx, int menu_level_count)
{
    while (1)
    {
        lv[lv_idx] = 1;
        printf("=============MENU=============\n");
        lv_path[0] = '\0';
        // based on the current LEVEL, configure lv_path
        for (int i = 0; i < lv_idx + 1; i++)
        {   
            char num_to_str[2];
            sprintf(num_to_str, "%d", i + 1);
            snprintf(lv_path, strlen(lv_path) + 10, "%s%s%s", lv_path, i==0?"LEVEL":" > LEVEL", num_to_str);
            if (i == lv_idx)
            {
                snprintf(lv_path, strlen(lv_path) + 2, "%s%s", lv_path, "\n");
            }
        }

        printf(lv_path);
        if (lv_idx == 0)
        {
            printf("1. Enter Level 2\n");
            printf("2. Quit Program\n");
        }
        else if (lv_idx == menu_level_count - 1)
        {
            printf("1. Return to Level %d\n", menu_level_count - 1);
            printf("2. Quit Program\n");
        }
        else
        {
            printf("1. Enter Level %d\n", lv_idx + 2);
            printf("2. Return to Level %d\n", lv_idx);
            printf("3. Quit Program\n");
        }

        printf("\nSelect an option: ");
        scanf("\n%c", &op[lv_idx]);

        system("cls");

        // enter next level
        if ((lv_idx == 0 || lv_idx != menu_level_count - 1) && op[lv_idx] == '1')
        {
            menu(lv_idx + 1, menu_level_count);
        }
        // return to last level
        else if ((lv_idx == menu_level_count - 1 && op[lv_idx] == '1') || ((lv_idx != 0 && lv_idx !=  menu_level_count - 1) && op[lv_idx] == '2'))
        {
            lv[lv_idx] = 0;
            break;
        }
        // quit program
        else if (((lv_idx == 0 || lv_idx == menu_level_count - 1) && op[lv_idx] == '2') || ((lv_idx != 0 && lv_idx !=  menu_level_count - 1) && op[lv_idx] == '3'))
        {   
            free(op);
            free(lv);
            free(lv_path);
            exit(0);
        }
        else
        {

        }
    }
}