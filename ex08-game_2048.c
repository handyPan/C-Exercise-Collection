#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int board[4][4] = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
};

int move = 0;
int score = 0;
int toAssign = 0;
int gameOver = 0;

int getRand(int min, int max);
void printArr(int arr[], int len);
void assignNumber();
void printBoard();
int add(int arr[4]);
int addReverse(int arr[4]);
void playerMove();
int checkGameOver();

int main()
{   
    toAssign = 1;

    srand(time(NULL));

    while (!gameOver)
    {   
        system("cls");

        // assign 2 or 4 to an available spot
        if (toAssign) assignNumber();

        printBoard();

        // player move
        playerMove();    

        gameOver = checkGameOver(); 
    }

    printf("Game Over!\n");

}

int getRand(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void printArr(int arr[], int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", arr[i]);
    }
}

void assignNumber()
{
    int availableSpot[16];
    for (int i = 0; i < 16; i++)
    {
        availableSpot[i] = -1;
    }

    int availableSpotCount = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] == 0)
            {   
                availableSpotCount++;
                availableSpot[availableSpotCount - 1] = i * 4 + j;  // the index i, j of board[][] is stored 
            }
        }
    }
    
    // printArr(availableSpot, availableSpotCount);
    // printf("\n");

    // randomly pick one available spot and assign 2 or 4
    int idxToAssign = availableSpot[getRand(0, availableSpotCount - 1)];
    int idxToAssign_i = idxToAssign / 4;
    int idxToAssign_j = idxToAssign - idxToAssign_i * 4;

    // printf("Assign to %d, %d\n", idxToAssign_i, idxToAssign_j);
    
    if (getRand(0, 1))
    {
        board[idxToAssign_i][idxToAssign_j] = 2;
    }
    else
    {
        board[idxToAssign_i][idxToAssign_j] = 4;
    }
}

void printBoard()
{
    printf("Game - 2048\n\n");
    printf("A - Left, W - Up, D - Right, S - Down, 0 - Exit\n");
    printf("Move: %d, Score: %d\n", move, score);

    printf("-----------------------------\n");
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] == 0)
            {
                printf("|      ");
            }
            else
            {
                printf("|%5d ", board[i][j]);
            }
        }
        printf("|\n");
        printf("-----------------------------\n");
    }
    printf("\n");
}

// for move left or up
int add(int arr[4])
{   
    int arrCopy[4] = {0, 0, 0, 0};
    int nonZeroCnt = 0;
    for (int i = 0; i < 4; i++)
    {
        if (arr[i] != 0)
        {
            arrCopy[nonZeroCnt] = arr[i];
            nonZeroCnt++;
        }
    }

    int _arr[4] = {0, 0, 0, 0};
    int i = 0, _i=0;
    int _score = 0;

    while (i < nonZeroCnt)
    {   
        if (i <= nonZeroCnt - 2 && arrCopy[i] == arrCopy[i+1])
        {   
            _arr[_i] = arrCopy[i] + arrCopy[i+1];
            _score = arrCopy[i] + arrCopy[i+1];
            i+=2;
        }
        else
        {
            _arr[_i] = arrCopy[i];
            i++;
        }
        _i++;
    }

    for (int i = 0; i < 4; i++)
    {
        arr[i] = _arr[i];
    }

    return _score;
}

// for move down or right
int addReverse(int arr[4])
{   
    int arrCopy[4] = {0, 0, 0, 0};
    int nonZeroCnt = 0;
    for (int i = 0; i < 4; i++)
    {
        if (arr[i] != 0)
        {
            arrCopy[nonZeroCnt] = arr[i];
            nonZeroCnt++;
        }
    }

    int _arr[4] = {0, 0, 0, 0};
    int i = nonZeroCnt - 1, _i=3;
    int _score = 0;

    while (i >= 0)
    {   
        if (i >=1 && arrCopy[i] == arrCopy[i-1])
        {   
            _arr[_i] = arrCopy[i] + arrCopy[i-1];
            _score = arrCopy[i] + arrCopy[i-1];
            i-=2;
        }
        else
        {
            _arr[_i] = arrCopy[i];
            i--;
        }
        _i--;
    }

    for (int i = 0; i < 4; i++)
    {
        arr[i] = _arr[i];
    }

    return _score;
}

void playerMove()
{
    char action = getch();
    int tmp[4];

    toAssign = 1;

    switch (toupper(action))
    {   
        // move Left, combine numbers for each row if applicable
        case 'A':
            for (int i = 0; i < 4; i++)
            {   
                // populate tmp[4] by each row
                for (int j = 0; j < 4; j++)
                {
                    tmp[j] = board[i][j];
                }

                // combine numbers 
                score += add(tmp);

                // restore values of each row
                for (int j = 0; j < 4; j++)
                {
                    board[i][j] = tmp[j];
                }
            }

            move++;
            break;
        // move up
        case 'W':
            for (int j = 0; j < 4; j++)
            {   
                
                // populate tmp[4] by each column
                for (int i = 0; i < 4; i++)
                {
                    tmp[i] = board[i][j];
                }
                
                // combine numbers 
                score += add(tmp);

                // restore values of each column
                for (int i = 0; i < 4; i++)
                {
                    board[i][j] = tmp[i];
                }
            }
            
            move++;
            break;
        // move right
        case 'D':
            for (int i = 0; i < 4; i++)
            {
                // populate tmp[4] by each row
                for (int j = 0; j < 4; j++)
                {
                    tmp[j] = board[i][j];
                }

                // combine numbers 
                score += addReverse(tmp);

                // restore values of each row
                for (int j = 0; j < 4; j++)
                {
                    board[i][j] = tmp[j];
                }
            }
            
            move++;
            break;
        // move down
        case 'S':
            for (int j = 0; j < 4; j++)
            {
                // populate tmp[4] by each column
                for (int i = 0; i < 4; i++)
                {
                    tmp[i] = board[i][j];
                }

                // combine numbers 
                score += addReverse(tmp);

                // restore values of each column
                for (int i = 0; i < 4; i++)
                {
                    board[i][j] = tmp[i];
                }
            }
            
            move++;
            break;
        case '0':
            printf("Continue to exit? (y/n) ");
            char input = getchar();
            if (toupper(input) == 'Y') exit(0);
            toAssign = 0;
            break;
        default:
            toAssign = 0;
            break;
    }
}

int checkGameOver()
{   
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (board[i][j] == 0) return 0;
        }
    }

    return 1;
}