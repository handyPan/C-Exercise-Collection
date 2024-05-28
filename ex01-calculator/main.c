/**
 * @file main.c
 * @author Handy Pan (https://github.com/handyPan/)
 * @brief To practice:
 *          - the organization of codes by function declaration in header files and implementation in c files
 *          - overloading in c
 *          - compile multiple c files
 *        Bug:
 *          - input of the option will automatically detect empty after execution of the 1st correct input
 * @version 0.1
 * @date 2024-03-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "add.h"
#include "sub.h"
#include "mul.h"
#include "_div.h"
#include "mod.h"
#include "_pow.h"

char op;

int main()
{   
    int num1, num2;

    while (1) {
        menu();

        if (op == 'Q') break;
        if (op != 'I') {
            printf("\nInput error, re-enter the option.\n");
            continue;
        }

        num1 = input('1');
        num2 = input('2');
        
        disp_res(num1, num2, '+', add(num1, num2));
        disp_res(num1, num2, '-', sub(num1, num2));
        disp_res(num1, num2, '*', mul(num1, num2));
        disp_res(num1, num2, '/', _div(num1, num2));
        disp_res(num1, num2, '%', mod(num1, num2));
        disp_res(num1, num2, '^', _pow(num1, num2));
        printf("\n");
    }


}