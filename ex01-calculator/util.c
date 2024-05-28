#include <stdio.h>
#include <stdlib.h>

void menu() 
{
    extern char op;
    printf("Select the option to proceed:\n");
    printf("Q - Quit\n");
    printf("I - Input Number\n");
    printf("Input Option: "); 
    scanf("\n%c", &op);
    op = toupper(op);
}

int input(char numSN) 
{
    int num;
    printf("Input the integer %c: ", numSN);
    scanf("%d", &num);
    return num;
}

void disp_res(int num1, int num2, char op, int res) 
{
    printf("%d %c %d = %d\n", num1, op, num2, res);
}

