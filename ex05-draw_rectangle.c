/**
 * @file ex05-draw_rectangle.c
 * @author Handy Pan (https://github.com/handyPan/)
 * @brief To practice:
 *          - basics of graphics.h
 * @version 0.1
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*

In this program a rectangle will be defined by two points: botLeft point and topRight point of the rectangle.
For example, botLeft = (200, 20) and  topRight = (300, 40) are the two points of the rectangle.
Your program MUST have a manu. Here are the menu options that you must offer to the user.
1.       Get two valid rectangles from the user.
2.       Randomly create two valid rectangles.
3.       Display both rectangles
4.       Find the area of both rectangles.
5.       Find the perimeter of both rectangles.
6.       Find the union of both rectangles.
7.       Find the intersection of both rectangles.
8.       Quit
 
You MUST repeatedly ask for the user’s menu selection and do the selected task until the user selects the QUIT option. At this time your program will end.

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graphics.h>

// xmax = 639, ymax = 479

#define POSITION_X_LOW -310     
#define POSITION_X_HIGH 310
#define POSITION_Y_LOW -230     
#define POSITION_Y_HIGH 230

struct Rectangle
{
    int botLeftX;
    int botLeftY;
    int topRightX;
    int topRightY;
};

struct Rectangle rect1 = { -10000, -10000, -10000, -10000};
struct Rectangle rect2 = { -10000, -10000, -10000, -10000};
struct Rectangle rect3 = { -10000, -10000, -10000, -10000};

void menu();
struct Rectangle inputRect(int idx);
int validatePositionX(int posX);
int validatePositionY(int posY);
int validateRect(struct Rectangle rect);
struct Rectangle genRect(int idx);
int rdInt(int min, int max);  // return a random integer in [min, max]
int isRectExisting(struct Rectangle rect);
void drawRect(struct Rectangle rect1, struct Rectangle rect2, char mode);
void disRect(int idx, struct Rectangle rect);
int findArea(struct Rectangle rect);
int findPeri(struct Rectangle rect);
void findUnionInter(char mode);
void disUnionInfo(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y,
                  int p5x, int p5y, int p6x, int p6y, int p7x, int p7y, int p8x, int p8y);

#define FIND_INTERSECTION   else \
                            { \
                                printf("- Points: (%d, %d), (%d, %d), (%d, %d), (%d, %d)\n", r3x1, r3y2, r3x2, r3y2, r3x2, r3y1, r3x1, r3y1); \
                                printf("- Area: %d\n", (int)findArea(rect3)); \
                                printf("- Perimeter: %d\n", (int) findPeri(rect3)); \
                                drawRect(rect1, rect2, 'i'); \
                            }
                        

int main()
{  
    srand(time(NULL)); //use current time as seed for random generator

    int isQuitSelected = 0;
    while (1)
    {
        menu();

        char op;
        scanf("\n%c", &op);

        switch (op)
        {
            case '1':
                rect1 = inputRect(1);
                rect2 = inputRect(2);
                printf("Rectangles input done!\n");
                break;
            case '2':
                rect1 = genRect(1);
                rect2 = genRect(2);
                printf("Rectangles generated done!\n");
                break;
            case '3':
                if (isRectExisting(rect1))
                {
                    disRect(1, rect1);
                }
                else
                {
                    printf("Rectangle %d does not exist!\n", 1);
                }
                if (isRectExisting(rect2))
                {
                    disRect(2, rect2);
                }
                else
                {
                    printf("Rectangle %d does not exist!\n", 2);
                }
                if (isRectExisting(rect1) && isRectExisting(rect2))
                {
                    drawRect(rect1, rect2, ' ');
                }
                break;
            case '4':
                if (isRectExisting(rect1))
                {
                    printf("Rectangle %d: Area = %d\n", 1, findArea(rect1));
                }
                else
                {
                    printf("Rectangle %d does not exist!\n", 1);
                }
                if (isRectExisting(rect2))
                {
                    printf("Rectangle %d: Area = %d\n", 2, findArea(rect2));
                }
                else
                {
                    printf("Rectangle %d does not exist!\n", 2);
                }
                break;
            case '5':
                if (isRectExisting(rect1))
                {
                    printf("Rectangle %d: Perimeter = %d\n", 1, findPeri(rect1));
                }
                else
                {
                    printf("Rectangle %d does not exist!\n", 1);
                }
                if (isRectExisting(rect2))
                {
                    printf("Rectangle %d: Perimeter = %d\n", 2, findPeri(rect2));
                }
                else
                {
                    printf("Rectangle %d does not exist!\n", 2);
                }
                break;
            case '6':
                if (isRectExisting(rect1) && isRectExisting(rect2))
                {
                    findUnionInter('u');
                }
                else
                {
                    isRectExisting(rect1) == 1 ? printf("") : printf("Rectangle %d does not exist!\n", 1);
                    isRectExisting(rect2) == 1 ? printf("") : printf("Rectangle %d does not exist!\n", 2);
                    printf("Can't find union!\n");
                }
                break;
            case '7':
                if (isRectExisting(rect1) && isRectExisting(rect2))
                {
                    findUnionInter('i');
                }
                else
                {
                    isRectExisting(rect1) == 1 ? printf("") : printf("Rectangle %d does not exist!\n", 1);
                    isRectExisting(rect2) == 1 ? printf("") : printf("Rectangle %d does not exist!\n", 2);
                    printf("Can't find intersection!\n");
                }
                break;
            case '8':
                isQuitSelected = 1;
                break;
            default:
                printf("Selection error!\n");
                break;
        }

        if (isQuitSelected) break;
    }
}

void menu()
{
    printf("\nRectangle Program\n\n");
    printf("1. Input two rectangles\n");
    printf("2. Randomly create two rectangles\n");
    printf("3. Display rectangles\n");
    printf("4. Find the area of rectangles\n");
    printf("5. Find the perimeter of rectangles\n");
    printf("6. Find the union of rectangles\n");
    printf("7. Find the intersection of rectangels\n");
    printf("8. Quit\n");
    printf("Select an option: ");
}

struct Rectangle inputRect(int idx)
{   
    // determine whether the rectangle already existed
    char op;

    if (idx == 1 && isRectExisting(rect1))
    {
        printf("Rect 1 already exists, still input? (y/n): ");
        scanf("\n%c", &op);
        if (toupper(op) == 'N')
        {
            printf("Use the existing rectangle 1.\n");
            return rect1;
        }
    }
    //fflush(stdout);
    if (idx == 2 && isRectExisting(rect2))
    {
        printf("Rect 2 already exists, still input? (y/n): ");
        scanf("\n%c", &op);
        if (toupper(op) == 'N')
        {
            printf("Use the existing rectangle 2.\n");
            return rect2;
        }
    }
    
    // input new rectangle

    struct Rectangle rect;

    while (1)
    {
        printf("Input position of rectangle %d, Note: \n \
                - For X, the range is %d to %d\n \
                - For Y, the range is %d to %d\n \
                - Top Right X > Bottom Left X, Top Right Y > Bottom Left Y\n", 
                idx, POSITION_X_LOW, POSITION_X_HIGH, POSITION_Y_LOW, POSITION_Y_HIGH);
        printf("Bottom Left X: ");
        scanf("\n%d", &rect.botLeftX);
        if (!validatePositionX(rect.botLeftX)) 
        {
            printf("Rectangle position invalid, re-enter!\n");
            continue;
        }
        printf("Bottom Left Y: ");
        scanf("\n%d", &rect.botLeftY);
        if (!validatePositionY(rect.botLeftY)) 
        {
            printf("Rectangle position invalid, re-enter!\n");
            continue;
        }
        printf("Top Right X: ");
        scanf("\n%d", &rect.topRightX);
        if (!validatePositionX(rect.topRightX)) 
        {
            printf("Rectangle position invalid, re-enter!\n");
            continue;
        }
        printf("Top Right Y: ");
        scanf("\n%d", &rect.topRightY);
        if (!validatePositionY(rect.topRightY)) 
        {
            printf("Rectangle position invalid, re-enter!\n");
            continue;
        }
        if (validateRect(rect))   break;
        printf("Rectangle positions invalid, re-enter!\n");
    }

    return rect;
}

int validatePositionX(int posX)
{
    if (posX < POSITION_X_LOW || posX > POSITION_X_HIGH)
    {
        return 0;
    }

    return 1;
}

int validatePositionY(int posY)
{
    if (posY < POSITION_Y_LOW || posY > POSITION_Y_HIGH)
    {
        return 0;
    }

    return 1;
}

// for both X and Y, topRight > botLeft
int validateRect(struct Rectangle rect)
{
    if (rect.topRightX > rect.botLeftX && rect.topRightY > rect.botLeftY) return 1;
    
    return 0;
}

struct Rectangle genRect(int idx)
{   
    // determine whether the rectangle already existed
    char op;

    if (idx == 1 && isRectExisting(rect1))
    {
        printf("Rect 1 already exists, still generate? (y/n): ");
        scanf("\n%c", &op);
        if (toupper(op) == 'N')
        {
            printf("Use the existing rectangle 1.\n");
            return rect1;
        }
    }
    //fflush(stdout);
    if (idx == 2 && isRectExisting(rect2))
    {
        printf("Rect 2 already exists, still generate? (y/n): ");
        scanf("\n%c", &op);
        if (toupper(op) == 'N')
        {
            printf("Use the existing rectangle 2.\n");
            return rect2;
        }
    }

    struct Rectangle rect;

    while (1)
    {   
        rect.botLeftX = rdInt(POSITION_X_LOW, POSITION_X_HIGH);
        if (!validatePositionX(rect.botLeftX)) continue;
        rect.botLeftY = rdInt(POSITION_Y_LOW, POSITION_Y_HIGH);
        if (!validatePositionY(rect.botLeftY)) continue;
        rect.topRightX = rdInt(POSITION_X_LOW, POSITION_X_HIGH);
        if (!validatePositionX(rect.topRightX)) continue;
        rect.topRightY = rdInt(POSITION_Y_LOW, POSITION_Y_HIGH);
        if (!validatePositionY(rect.topRightY)) continue;
        if (validateRect(rect))   break;
    }

    return rect;
}

int rdInt (int min, int max)
{   
    time_t t;
    /* Intializes random number generator */
    //srand((unsigned) time(&t));
    
    return rand() % (max - min + 1) + min;
}

int isRectExisting(struct Rectangle rect)
{
    if (rect.botLeftX != -10000)
    {   
        return 1;
    }

    return 0;
}

void disRect(int idx, struct Rectangle rect)
{   
    printf("Rectangle %d: Bottom Left at (%d, %d), Top Right at (%d, %d)\n", idx, rect.botLeftX, rect.botLeftY, rect.topRightX, rect.topRightY);
}

void drawRect(struct Rectangle rect1, struct Rectangle rect2, char mode)
{
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "");

    int left1, top1, right1, bottom1, width1, height1;

    // rect1: int botLeftX; int botLeftY; int topRightX; int topRightY;
    // coordinates converted to distance to left and top of the window
    left1 = getmaxx()/2 + rect1.botLeftX; 
    top1 = getmaxy()/2 - rect1.topRightY;
    width1 = rect1.topRightX - rect1.botLeftX;
    height1 = rect1.topRightY - rect1.botLeftY;
    right1 = left1 + width1;    // left + width
    bottom1 = top1 + height1;    // top + height
    
    int left2, top2, right2, bottom2, width2, height2;
    left2 = getmaxx()/2 + rect2.botLeftX; 
    top2 = getmaxy()/2 - rect2.topRightY;
    width2 = rect2.topRightX - rect2.botLeftX;
    height2 = rect2.topRightY - rect2.botLeftY;
    right2 = left2 + width2;    // left + width
    bottom2 = top2 + height2;    // top + height

    float scaleRatio = 1.0;
    float rightLimit = 0.9 * getmaxx();
    float bottomLimit = 0.9 * getmaxy();  // getmaxy(), 450
    if (right1 > rightLimit)
    {
        scaleRatio = rightLimit / right1;
    }
    else if (bottom1 > bottomLimit && bottomLimit / bottom1 < scaleRatio)
    {
        scaleRatio = bottomLimit / bottom1;
    }
    else if (right2 > rightLimit && rightLimit / right2 < scaleRatio)
    {
        scaleRatio = rightLimit / right2;
    }
    else if (bottom2 > bottomLimit && bottomLimit / bottom2 < scaleRatio)
    {
        scaleRatio = bottomLimit / bottom2;
    }
    else {}

    printf("Scale Ratio = %f\n", scaleRatio);
    left1 = (int) left1 * scaleRatio;       top1 = (int) top1 * scaleRatio;
    right1 = (int) right1 * scaleRatio;     bottom1 = (int) bottom1 * scaleRatio; 
    left2 = (int) left2 * scaleRatio;       top2 = (int) top2 * scaleRatio;
    right2 = (int) right2 * scaleRatio;     bottom2 = (int) bottom2 * scaleRatio;

    // fill if necessary
    if (mode == 'u')
    {
        printf("Rectangle 1 drawn at left = %d, top = %d, right = %d, bottom = %d\n", left1, top1, right1, bottom1);
        setfillstyle(SLASH_FILL, GREEN);
        //rectangle(left1, top1, right1, bottom1);
        //floodfill(left1+1, top1+1, 15);
        int poly1[] = {left1, top1, right1, top1, right1, bottom1, left1, bottom1};
        fillpoly(4, poly1);
        printf("Rectangle 2 drawn at left = %d, top = %d, right = %d, bottom = %d\n", left2, top2, right2, bottom2);
        setfillstyle(SLASH_FILL, GREEN);
        //rectangle(left2, top2, right2, bottom2);
        //floodfill(left2+1, top2+1, 15);
        int poly2[] = {left2, top2, right2, top2, right2, bottom2, left2, bottom2};
        fillpoly(4, poly2);

        rectangle(left1, top1, right1, bottom1);
        rectangle(left2, top2, right2, bottom2);
    }
    else if (mode == 'i')
    {
        printf("Rectangle 1 drawn at left = %d, top = %d, right = %d, bottom = %d\n", left1, top1, right1, bottom1);
        setfillstyle(SLASH_FILL, GREEN);
        //rectangle(left1, top1, right1, bottom1);
        //floodfill(left1+1, top1+1, 15);
        int poly1[] = {left1, top1, right1, top1, right1, bottom1, left1, bottom1};
        fillpoly(4, poly1);
        printf("Rectangle 2 drawn at left = %d, top = %d, right = %d, bottom = %d\n", left2, top2, right2, bottom2);
        setfillstyle(SLASH_FILL, GREEN);
        //rectangle(left2, top2, right2, bottom2);
        //floodfill(left2+1, top2+1, 15);
        int poly2[] = {left2, top2, right2, top2, right2, bottom2, left2, bottom2};
        fillpoly(4, poly2);

        rectangle(left1, top1, right1, bottom1);
        rectangle(left2, top2, right2, bottom2);
        
        if (rect3.botLeftX != -10000)
        {
            int left3, top3, right3, bottom3, width3, height3;
            left3 = getmaxx()/2 + rect3.botLeftX; 
            top3 = getmaxy()/2 - rect3.topRightY;
            width3 = rect3.topRightX - rect3.botLeftX;
            height3 = rect3.topRightY - rect3.botLeftY;
            right3 = left3 + width3;    // left + width
            bottom3 = top3 + height3;    // top + height
            left3 = (int) left3 * scaleRatio;       top3 = (int) top3 * scaleRatio;
            right3 = (int) right3 * scaleRatio;     bottom3 = (int) bottom3 * scaleRatio;
            setfillstyle(BKSLASH_FILL, RED);
            //rectangle(left3, top3, right3, bottom3);
            //floodfill(left3+1, top3+1, 15);
            int poly3[] = {left3, top3, right3, top3, right3, bottom3, left3, bottom3};
            fillpoly(4, poly3);
            rectangle(left3, top3, right3, bottom3);
        }
    }
    else
    {
        printf("Rectangle 1 drawn at left = %d, top = %d, right = %d, bottom = %d\n", left1, top1, right1, bottom1);
        rectangle(left1, top1, right1, bottom1);
        printf("Rectangle 2 drawn at left = %d, top = %d, right = %d, bottom = %d\n", left2, top2, right2, bottom2);
        rectangle(left2, top2, right2, bottom2);
    }

    // draw the coordinates
    line(getmaxx()/2, 0, getmaxx()/2, getmaxy());
    line(0, getmaxy()/2, getmaxx(), getmaxy()/2);

    getch(); 
    closegraph(); 
}

int findArea(struct Rectangle rect)
{
    return (rect.topRightX - rect.botLeftX) * (rect.topRightY - rect.botLeftY);
}

int findPeri(struct Rectangle rect)
{
    return (rect.topRightX - rect.botLeftX) * 2 + (rect.topRightY - rect.botLeftY) * 2;
}

void findUnionInter(char mode)
{   
    int r1x1, r1x2, r1y1, r1y2, r2x1, r2x2, r2y1, r2y2;
    r1x1 = rect1.botLeftX; r1x2 = rect1.topRightX; r1y1 = rect1.botLeftY; r1y2 = rect1.topRightY;
    r2x1 = rect2.botLeftX; r2x2 = rect2.topRightX; r2y1 = rect2.botLeftY; r2y2 = rect2.topRightY;

    printf("%s\n", mode == 'u' ? "Union\n" : "Intersection\n");
    // 1 - in horizontal, r1 is over r2, or r2 is over r1, r1 and r2 has no intersection
    if (r1y1 > r2y2 || r2y1 > r1y2)
    {   
        if (mode == 'u')
        {   
            struct Rectangle rectTmp = {-10000, -10000, -10000, -10000};
            rect3 = rectTmp;
            disUnionInfo(r1x1, r1y2, r1x2, r1y2, r1x2, r1y1, r1x1, r1y1, 
                         r2x1, r2y2, r2x2, r2y2, r2x2, r2y1, r2x1, r2y1);
        }
        else 
        {
            printf("- Points: null\n");
            printf("- Area: null\n");
            printf("- Perimeter: null\n");
        }
    }

    // 2 - in vertical, r1 is at left of r2, or r2 is at left of r1, r1 and r2 has no intersection
    else if (r1x2 < r2x1 || r2x2 < r1x1)
    {   
        if (mode == 'u')
        {   
            struct Rectangle rectTmp = {-10000, -10000, -10000, -10000};
            rect3 = rectTmp;
            disUnionInfo(r1x1, r1y2, r1x2, r1y2, r1x2, r1y1, r1x1, r1y1, 
                         r2x1, r2y2, r2x2, r2y2, r2x2, r2y1, r2x1, r2y1);
        }
        else 
        {
            printf("- Points: null\n");
            printf("- Area: null\n");
            printf("- Perimeter: null\n");
        }
    }

    // 3 - in horizontal, r1 is on top of r2, or r2 is on top of r1, r1 and r2 has intersection
    else if (r2y1 <= r1y1 && r1y1 <= r2y2 && r2y2 <=r1y2)
    {
        // case 1 
        if (r1x1 <= r2x1 && r2x1 <= r1x2 && r1x2 <= r2x2)
        {   
            struct Rectangle rectTmp = {r2x1, r1y1, r1x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {   
                disUnionInfo(r1x1, r1y2, r1x2, r1y2, r3x2, r3y2, r2x2, r2y2, 
                             r2x2, r2y1, r2x1, r2y1, r3x1, r3y1, r1x1, r1y1);
            }

            FIND_INTERSECTION
        }

        // case 2 
        else if (r1x1 <= r2x1 && r2x2 <= r1x2)
        {
            struct Rectangle rectTmp = {r2x1, r1y1, r2x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {   
                disUnionInfo(r1x1, r1y2, r1x2, r1y2, r1x2, r1y1, r3x2, r3y1, 
                             r2x2, r2y1, r2x1, r2y1, r3x1, r3y1, r1x1, r1y1);
            }
            FIND_INTERSECTION
        }

        // case 3
        else if (r2x1 <= r1x1 && r1x2 <= r2x2)
        {
            struct Rectangle rectTmp = {r1x1, r1y1, r1x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r1x1, r1y2, r1x2, r1y2, r3x2, r3y2, r2x2, r2y2, 
                             r2x2, r2y1, r2x1, r2y1, r2x1, r2y2, r3x1, r3y2);
            }
            FIND_INTERSECTION
        }

        // case 4
        else if (r2x1 <= r1x1 && r1x1 <= r2x2 && r2x2 <= r1x2)
        {
            struct Rectangle rectTmp = {r1x1, r1y1, r2x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r1x1, r1y2, r1x2, r1y2, r1x2, r1y1, r3x2, r3y1, 
                             r2x2, r2y1, r2x1, r2y1, r2x1, r2y2, r3x1, r3y2);
            }
            FIND_INTERSECTION
        }

        else {}

    }

    // swap r1 and r2
    else if (r1y1 <= r2y1 && r2y1 <= r1y2 && r1y2 <=r2y2)
    {
        // case 1 
        if (r2x1 <= r1x1 && r1x1 <= r2x2 && r2x2 <= r1x2)
        {   
            struct Rectangle rectTmp = {r1x1, r2y1, r2x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r2x2, r2y2, r3x2, r3y2, r1x2, r1y2, 
                             r1x2, r1y1, r1x1, r1y1, r3x1, r3y1, r2x1, r2y1);
            }
            FIND_INTERSECTION
        }

        // case 2 
        else if (r2x1 <= r1x1 && r1x2 <= r2x2)
        {
            struct Rectangle rectTmp = {r1x1, r2y1, r1x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r2x2, r2y2, r2x2, r2y1, r3x2, r3y1, 
                             r1x2, r1y1, r1x1, r1y1, r3x1, r3y1, r2x1, r2y1);
            }
            FIND_INTERSECTION
        }

        // case 3
        else if (r1x1 <= r2x1 && r2x2 <= r1x2)
        {
            struct Rectangle rectTmp = {r2x1, r2y1, r2x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r2x2, r2y2, r3x2, r3y2, r1x2, r1y2, 
                             r1x2, r1y1, r1x1, r1y1, r1x1, r1y2, r3x1, r3y2);
            }
            FIND_INTERSECTION
        }

        // case 4
        else if (r1x1 <= r2x1 && r2x1 <= r1x2 && r1x2 <= r2x2)
        {
            struct Rectangle rectTmp = {r2x1, r2y1, r1x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r2x2, r2y2, r2x2, r2y1, r3x2, r3y1, 
                             r1x2, r1y1, r1x1, r1y1, r1x1, r1y2, r3x1, r3y2);
            }
            FIND_INTERSECTION
        }

        else {}
    }
    // else {}

    // 4 - in vertical, r1 is at the left of r2, or r2 is at the left of r1, r1 and r2 has intersection
    else if (r1x1 <= r2x1 && r2x1 <= r1x2 && r1x2 <= r2x2)
    {
        // case 1 
        if (r2y1 <= r1y1 && r1y1 <= r2y2 && r2y2 <= r1y2)
        {
            struct Rectangle rectTmp = {r2x1, r1y1, r1x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r1x1, r1y2, r1x2, r1y2, r3x2, r3y2, r2x2, r2y2, 
                             r2x2, r2y1, r2x1, r2y1, r3x1, r3y1, r1x1, r1y1);
            }
            FIND_INTERSECTION
        }

        // case 2
        else if (r1y1 <= r2y1 && r2y2 <= r1y2)
        {
            struct Rectangle rectTmp = {r2x1, r2y1, r1x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r1x1, r1y2, r1x2, r1y2, r3x2, r3y2, r2x2, r2y2, 
                             r2x2, r2y1, r3x2, r3y1, r1x2, r1y1, r1x1, r1y1);
            }
            FIND_INTERSECTION
        }

        // case 3
        else if (r2y1 <= r1y1 && r1y2 <= r2y2)
        {
            struct Rectangle rectTmp = {r2x1, r1y1, r1x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r1x1, r1y2, r3x1, r3y2, r2x1, r2y2, r2x2, r2y2, 
                             r2x2, r2y1, r2x1, r2y1, r3x1, r3y1, r1x1, r1y1);
            }
            FIND_INTERSECTION
        }

        // case 4
        else if (r1y1 <= r2y1 && r2y1 <= r1y2 && r1y2 <= r2y2)
        {
            struct Rectangle rectTmp = {r2x1, r2y1, r1x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r2x2, r2y2, r2x2, r2y1, r3x2, r3y1, 
                             r1x2, r1y1, r1x1, r1y1, r1x1, r1y2, r3x1, r3y2);
            }
            FIND_INTERSECTION
        }

        else {}
    }

    // swap r1 and r2
    else if (r2x1 <= r1x1 && r1x1 <= r2x2 && r2x2 <= r1x2)
    {
        // case 1 
        if (r1y1 <= r2y1 && r2y1 <= r1y2 && r1y2 <= r2y2)
        {
            struct Rectangle rectTmp = {r1x1, r2y1, r2x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r2x2, r2y2, r3x2, r3y2, r1x2, r1y2, 
                             r1x2, r1y1, r1x1, r1y1, r3x1, r3y1, r2x1, r2y1);
            }
            FIND_INTERSECTION
        }

        // case 2
        else if (r2y1 <= r1y1 && r1y2 <= r2y2)
        {
            struct Rectangle rectTmp = {r1x1, r1y1, r2x2, r1y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r2x2, r2y2, r3x2, r3y2, r1x2, r1y2, 
                             r1x2, r1y1, r3x2, r3y1, r2x2, r2y1, r2x1, r2y1);
            }
            FIND_INTERSECTION
        }

        // case 3
        else if (r1y1 <= r2y1 && r2y2 <= r1y2)
        {
            struct Rectangle rectTmp = {r1x1, r2y1, r2x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r2x1, r2y2, r3x1, r3y2, r1x1, r1y2, r1x2, r1y2, 
                             r1x2, r1y1, r1x1, r1y1, r3x1, r3y1, r2x1, r2y1);
            }
            FIND_INTERSECTION
        }

        // case 4
        else if (r2y1 <= r1y1 && r1y1 <= r2y2 && r2y2 <= r1y2)
        {
            struct Rectangle rectTmp = {r1x1, r1y1, r2x2, r2y2};
            rect3 = rectTmp;
            int r3x1, r3x2, r3y1, r3y2;
            r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
            if (mode == 'u')
            {
                disUnionInfo(r1x1, r1y2, r1x2, r1y2, r1x2, r1y1, r3x2, r3y1, 
                             r2x2, r2y1, r2x1, r2y1, r2x1, r2y2, r3x1, r3y2);
            }
            FIND_INTERSECTION
        }

        else {}
    }
    // else {}

    // 5 - r1 contains r2
    else if (r1x1 <= r2x1 && r2x2 <= r1x2 && r1y1 <= r2y1 && r2y2 <= r1y2)
    {   
        struct Rectangle rectTmp = {r2x1, r2y1, r2x2, r2y2};
        rect3 = rectTmp;
        int r3x1, r3x2, r3y1, r3y2;
        r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
        if (mode == 'u')
        {
            // output the points in the clockwise order, 
            printf("- Points: (%d, %d), (%d, %d), (%d, %d), (%d, %d)\n", 
                                r1x1, r1y2, r1x2, r1y2, r1x2, r1y1, r1x1, r1y1);    
            printf("- Area: %d\n", findArea(rect1));
            printf("- Perimeter: %d\n", findPeri(rect1));
            drawRect(rect1, rect2, 'u');
        }
        FIND_INTERSECTION
    }

    // or r2 contains r1
    else if (r2x1 <= r1x1 && r1x2 <= r2x2 && r2y1 <= r1y1 && r1y2 <= r2y2)
    {   
        struct Rectangle rectTmp = {r1x1, r1y1, r1x2, r1y2};
        rect3 = rectTmp;
        int r3x1, r3x2, r3y1, r3y2;
        r3x1 = rect3.botLeftX; r3x2 = rect3.topRightX; r3y1 = rect3.botLeftY; r3y2 = rect3.topRightY;
        if (mode == 'u')
        {
            // output the points in the clockwise order, 
            printf("- Points: (%d, %d), (%d, %d), (%d, %d), (%d, %d)\n", 
                                r2x1, r2y2, r2x2, r2y2, r2x2, r2y1, r2x1, r2y1);    
            printf("- Area: %d\n", findArea(rect2));
            printf("- Perimeter: %d\n", findPeri(rect2));
            drawRect(rect1, rect2, 'u');
        }
        FIND_INTERSECTION
    }
    
    else {}
}

void disUnionInfo(int p1x, int p1y, int p2x, int p2y, int p3x, int p3y, int p4x, int p4y,
                  int p5x, int p5y, int p6x, int p6y, int p7x, int p7y, int p8x, int p8y)
{
    // output the points at left-top, right-top, right-bottom, left-bottom
    printf("- Points: (%d, %d), (%d, %d), (%d, %d), (%d, %d), (%d, %d), (%d, %d), (%d, %d), (%d, %d)\n", 
                        p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, 
                        p5x, p5y, p6x, p6y, p7x, p7y, p8x, p8y);
    // no intersection
    if (rect3.botLeftX == -10000)
    {
        printf("- Area: %d\n", findArea(rect1) + findArea(rect2));
        printf("- Perimeter: %d\n", findPeri(rect1) + findPeri(rect2));
    }
    else
    {
        printf("- Area: %d\n", findArea(rect1) + findArea(rect2) - findArea(rect3));
        printf("- Perimeter: %d\n", findPeri(rect1) + findPeri(rect2) - findPeri(rect3));
    }
    
    drawRect(rect1, rect2, 'u');
}

