/**
 * @file ex06-battle_game.c
 * @author Handy Pan (https://github.com/handyPan/)
 * @brief To practice:
 *          - a simple battle game based on turns
 * @version 0.1
 * @date 2024-05-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
    2 players will be taking turns to to perform action with the end goal with destroying their opponent. 
    Each player begins with 1000 health. When a player's health reaches 0, they die, and the win is granted to the other player.
    Player 1 begins with 500 gold and player 2, due to being at a disadvantage for starting second, begins with 550 gold.
    Players gain gold passively during attack and defence turns, or can gain extra gold by using the rest action.

    4 types of action: 
    - Attack: Provided the player has a weapon, the attack action gives the player the option to attack an opponent.(Each weapon has its own damage, cooldown, hitchance, and cost.)
    - Defend: Provided the player has a defence, the defend action gives the player the option to defend using a defence.(Each defence has its own hitpoints, damage deduction, and cost.)
    - Purchase: Provided the player has enough gold, the purchase action allows a player to acquire new weapons and defences.
    - Rest: The rest option gives the player 150 gold, but it increases by 50 gold for every consecutively rested turn.
        //Here, Let's set up the rest action for the code.
        //The rest action allows the player to gain extra gold during that turn.
        //The amount of gold the user gains depends on the number of turns the user has been resting in a row.
        //For example, if the user rests for the first time, they gain 50 gold.
        //if the same player rests for another time next turn, they gain 100 gold next turn.
        //If the user takes a different action other than resting, the gold gain value resets to 0.
        //The gained gold should be added to the gold1 and gold2 values.
        //gold1 represents the amount of gold the player 1 has, gold2 represents the amount of gold the player 2 has.
        //You will first need to create an if sentence to see if the input from the user is 'r' or not.
        //Then, you have to decide which player is taking the action, and how much gold is being added.
        //you final print sentence should be "Player #, you gained %i gold. You now have total %i gold.\n"
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
//#include <unistd.h>
#include <stdlib.h>

#define HITCHANCE_BASE 3

//declaring 2 strucutres
struct weapon {
    char type[40];
    char name[40];
    int damage;
    int cooldown;
    int cost;
    int hitchance;
};

struct defence {
    char type[40];
    char name[40];
    int hitpoints;
    int DamageDeduction;
    int cost;
};

// the weapon can hit by a chance of 3 / hitchance property, ex 3 / 8
// every 8 actions, randomly hit target by 3 times
// to use one weapon, it's like this: use it -> cooldown -> use it -> cooldown -> ... -> use it -> cooldown
struct playerWeaponCollection {
    int isPurchased;            // 0 - not purchased and unavailable, 1 - available
    int coolDownTurnsLeft;      // if it's not yet cool, can't use
    int *hitAtTurns;            // initialized to be NULL, dynamic array that stores 0, 1, 1, 0, 1, ...
    int *currentHit;           // point to the turns, if currentHit = NULL, it means all hits are output, the *hitAtTurns should be regenerated
    int *firstHit;             // backup of the 1st hit pointer for free memory
};

struct playerDefenceCollection {
    int isPurchased;            // 0 - not purchased and unavailable, 1 - available
    int remainingHP;            // remaining HP after defence, if it drops to 0, player needs to purchase again
};

int isWeaponAvailableForAttack(int playerToAct);
int selectWeaponForAttack(int playerToAct);
int isDefenceAvailableForDefend(int playerToAct);
int selectDefenceForDefend(int playerToAct);
int minWeaponPurchaseGold(int playerToAct);
int minDefencePurchaseGold(int playerToAct);
char inputAction(int playerToAct);
void doAttack(int playerToAct, int opponentPlayer);
void doDefend(int playerToAct, int opponentPlayer);
char selectOptionForPurchase(int playerToAct);
int selectWeaponForPurchase(int playerToAct);
int selectDefenceForPurchase(int playerToAct);
void doPurchase(playerToAct);
void doRest(playerToAct);
void doAction(char action, int playerToAct, int opponentPlayer);
int getRandNum (int min, int max);
void printInfo(int player1, int player2);

// damage, cooldown, cost, hitchance
struct weapon weaponCollection[10] = {{"Energy", "Tachyon Lance", 3000, 20, 1500, 3}, 
                                        {"Energy", "Plasma Lanucher", 400, 2, 700, 6},
                                        {"Energy", "Zip Zap", 50, 1, 500, 6},
                                        {"Kinatic", "Guass Canon", 1000, 5, 1500, 3}, 
                                        {"Kinatic", "Cabine Gun", 300, 1, 1000, 6},
                                        {"Kinatic", "Hand Gun", 100, 2, 500, 6},
                                        {"Melee", "Dark Saber", 2000, 5, 1000, 8},
                                        {"Melee", "Plasma Knife", 300, 2, 500, 8},
                                        {"Melee", "Reaper's Scythe", 1500, 5, 800, 9},
                                        {"Mystic", "Aetherophasic Engine", 2000, 1, 800, 15}};
    
// hitpoints, damage deduction, and cost
struct defence defenceCollection[10] = {
                                        {"Energy", "Dark Matter Shield", 4000, 1, 1100},
                                        {"Energy", "Energy Deflector", 2000, 1, 800},
                                        {"Energy", "Basic Sheild", 700, 1, 500},
                                        {"Kinatic", "Gravitatioinal Refractor", 1000, 0.5, 1100},
                                        {"Kinatic", "Hyper Array", 1000, 0.8, 800},
                                        {"Kinetic", "Pulsing Array", 500, 0.9, 500},
                                        {"Melee", "Zero-point Armor", 300, 0.3, 1100},
                                        {"Melee", "Power Armor", 300, 0.4, 800},
                                        {"Melee", "Space Armor", 200, 0.6, 500},
                                        {"Mystic", "Dimensional Lens", 500, 0.5, 1000}};

int healthPoint[2] = {1000, 1000};
int gold[2] = {500, 550};

struct playerWeaponCollection pwc[2][10];
struct playerDefenceCollection pdc[2][10]; 

// damage will be changed when
// - the player do a new attack
// - the opponent do a defend
int damageApplied[2] = {0, 0};
int consecutiveRestCnt[2] = {0, 0};

char mode;

//main program
int main(){

    // initialize, no weapons and defences
    for(int i = 0; i < 10; i++){
        pwc[0][i].isPurchased = -1;
        pwc[0][i].coolDownTurnsLeft = 0;
        pwc[0][i].hitAtTurns = NULL;
        pwc[0][i].currentHit = NULL;
        pwc[0][i].firstHit = NULL;
        pwc[1][i].isPurchased = -1;
        pwc[1][i].coolDownTurnsLeft = 0;
        pwc[1][i].hitAtTurns = NULL;
        pwc[1][i].currentHit = NULL;
        pwc[1][i].firstHit = NULL;

        pdc[0][i].isPurchased = -1;
        pdc[0][i].remainingHP = 0;
        pdc[1][i].isPurchased = -1;
        pdc[1][i].remainingHP = 0;
    }

    int playerToAct, opponentPlayer;
    playerToAct = 0;    // 0 and 1 for players 1 and 2
    opponentPlayer = playerToAct == 0 ? 1 : 0;
    char action;
    int turnSN = 0;

    while (1)
    {
        printf("Select the mode to proceed, 'm' for manual, 'a' for automated: ");
        scanf("\n%c", &mode);
        if (mode != 'm' && mode != 'a')
        {
            printf("Invalid input, select again!\n");
            sleep(1);
            continue;
        }
        break;
    }

    printf("Game STARTED in %s mode!\n\n", mode == 'm' ? "manual" : "automated");
    sleep(1);
    printInfo(0, 1);
    sleep(1);

    while (1)
    {
        if (healthPoint[0] <= 0 || healthPoint[1] <= 0) break;

        action = ' ';
        
        turnSN++;
        printf("Turn #%d - Player %d's turn to act.\n", turnSN, playerToAct + 1);

        action = inputAction(playerToAct);
        
        doAction(action, playerToAct, opponentPlayer);

        printInfo(0, 1);

        playerToAct = opponentPlayer;    // 0 and 1 for players 1 and 2
        opponentPlayer = playerToAct == 0 ? 1 : 0;

        printf("\n\n");
    }

    printf("Game ENDED!\n\n");
    
    return 0;
}

// check whether any weapon is available for attack; 
// return 0 - no weapon available; 1 - available;
int isWeaponAvailableForAttack(int playerToAct)
{   
    for (int i = 0; i < 10; i++)
    {   
        if (pwc[playerToAct][i].isPurchased != -1 && pwc[playerToAct][i].coolDownTurnsLeft == 0) 
        {
            return 1;
        }
    }

    return 0;
}

// select weapon for attack
// return the index of the weapon in weaponCollection
int selectWeaponForAttack(int playerToAct)
{   
    // print info of weapon
    printf("Available weapons:\n");
    for (int wIdx = 0; wIdx < 10; wIdx++)
    {
        if (pwc[playerToAct][wIdx].isPurchased != -1 && pwc[playerToAct][wIdx].coolDownTurnsLeft == 0)
        {
            printf("%d - Type: %s, Name: %s, Damage: %d, Cool Down (turns): %d, Hit Chance (? turns / ? turns): %d/%d, Cost: %d\n", 
                                wIdx, weaponCollection[wIdx].type, weaponCollection[wIdx].name, 
                                weaponCollection[wIdx].damage, weaponCollection[wIdx].cooldown, 
                                3, weaponCollection[wIdx].hitchance,
                                weaponCollection[wIdx].cost);
        }
    }
    printf("Select a weapon to proceed: ");
    int op;
    while (1)
    {   
        if (mode == 'm')
        {
            scanf("%d", &op);
        }
        else
        {
            op = getRandNum(0, 9);
        }

        // check whether the weapon is purchased and "cool"
        if (pwc[playerToAct][op].isPurchased == -1)
        {
            printf("Invalid input, weapon not purchased. Select again!\n");
            sleep(1);
            continue;
        }

        if (pwc[playerToAct][op].coolDownTurnsLeft > 0)
        {
            printf("Invalid input, weapon not cool. Select again!\n");
            sleep(1);
            continue;
        }

        printf("Weapon %d selected.\n", op);
        printf("Type: %s, Name: %s, Damage: %d, Cool Down (turns): %d, Hit Chance (? turns / ? turns): %d/%d, Cost: %d\n", 
                weaponCollection[op].type, weaponCollection[op].name, 
                weaponCollection[op].damage, weaponCollection[op].cooldown, 
                3, weaponCollection[op].hitchance,
                weaponCollection[op].cost);
        return op;
    }
}

// check whether any defence is available for defend; 
// return 0 - no defence available; 1 - available;
int isDefenceAvailableForDefend(int playerToAct)
{
    for (int i = 0; i < 10; i++)
    {   
        // if remainingHP drops to 0, isPurchased will be reset to -1
        if (pdc[playerToAct][i].isPurchased == 1)
        {
            return 1;
        }
    }
    
    return 0;
}

// select defence for defend
// return the index of the defence in defenceCollection
int selectDefenceForDefend(int playerToAct)
{
    // print info of defence
    printf("Available defences:\n");
    for (int dIdx = 0; dIdx < 10; dIdx++)
    {
        if (pdc[playerToAct][dIdx].isPurchased != -1)
        {
            printf("%d - Type: %s, Name: %s, Hit Points: %d, Damage Reduction: %d\n", dIdx, defenceCollection[dIdx].type, defenceCollection[dIdx].name, defenceCollection[dIdx].hitpoints, defenceCollection[dIdx].DamageDeduction);
        }
    }
    printf("Select a defence to proceed: ");
    int op;
    while (1)
    {
        if (mode == 'm')
        {
            scanf("%d", &op);
        }
        else
        {
            op = getRandNum(0, 9);
        }

        // check whether the defence is purchased
        if (pdc[playerToAct][op].isPurchased == -1)
        {
            printf("Invalid input, defence not purchased. Select again!\n");
            sleep(1);
            continue;
        }

        printf("Defence %d selected.\n", op);
        printf("Type: %s, Name: %s, Hit Points: %d, Damage Reduction: %d\n", 
                defenceCollection[op].type, defenceCollection[op].name, defenceCollection[op].hitpoints, defenceCollection[op].DamageDeduction);
        return op;
    }
}

// find the minimum gold to purchase an unpurchased weapon
// if return = -1, all weapons are purchased
int minWeaponPurchaseGold(int playerToAct)
{   
    int min = -1;
    // weaponCollection
    for (int i = 0; i < 10; i++)
    {   
        if (pwc[playerToAct][i].isPurchased == -1) 
        {
            if (min == -1 || min > weaponCollection[i].cost)
            {
                min = weaponCollection[i].cost;
            }
        }
    }

    return min;
}

// find the minimum gold to purchase an unpurchased defence
// if return = -1, all defences are purchased
int minDefencePurchaseGold(int playerToAct)
{   
    int min = -1;
    // defenceCollection
    for (int i = 0; i < 10; i++)
    {   
        if (pdc[playerToAct][i].isPurchased == -1) 
        {
            if (min == -1 || min > defenceCollection[i].cost)
            {
                min = defenceCollection[i].cost;
            }
        }
    }

    return min;
}

// input action by player
char inputAction(int playerToAct)
{   
    char action;
    char actionCollection[5] = {'a', 'd', 'p', 'r', 'u'};

    while (1)
    {
        printf("\nPlayer %d, what action would you like to make? Type 'a' for attack, 'd' for defend, 'p' for purchase, or 'r' for rest\n", playerToAct + 1);
        
        if (mode == 'm')
        {
            scanf("\n%c", &action);
        }
        else
        {
            action = actionCollection[getRandNum(0, 4)];
        }
        
        if (action != 'a' && action != 'd' && action != 'p' && action != 'r')
        {
            printf("Invalid input!\n");
            sleep(1);
            continue;
        }

        // if 'a', verify whether "attack" can be selected
        // - check whether there is weapon in player weapon collection
        // - check whether the selected weapon meet the "cooldown" requirement
        if (action == 'a')
        {   
            // no weapons in player weapon collection is available
            if (isWeaponAvailableForAttack(playerToAct) == 0)
            {
                printf("No weapon available, input action again!\n");
                sleep(1);
                continue;
            }
        }

        // if 'd', verify whether 'defend' can be selected
        if (action == 'd')
        {   
            // no defences in player defence collection is available
            if (isDefenceAvailableForDefend(playerToAct) == 0)
            {
                printf("No defence available, input action again!\n");
                sleep(1);
                continue;
            }
        }

        // if 'p', verify whether 'purchase' cen be selected
        if (action == 'p')
        {   
            // not enough gold for purchasing weapon and defence
            if (gold[playerToAct] < minWeaponPurchaseGold(playerToAct) && gold[playerToAct] < minDefencePurchaseGold(playerToAct))
            {
                printf("Not enough gold, input action again!\n");
                sleep(1);
                continue;
            }
            if (minWeaponPurchaseGold(playerToAct) == -1)
            {
                printf("All weapons purchased.\n");
                sleep(1);
                continue;
            }
            if (minDefencePurchaseGold(playerToAct) == -1)
            {
                printf("All defences purchased.\n");
                sleep(1);
                continue;
            }
        }

        return action;
    }
}

// attack 
void doAttack(int playerToAct, int opponentPlayer)
{
    int wIdx = selectWeaponForAttack(playerToAct);
    // configure the weapon selected if currentHit = NULL
    if (pwc[playerToAct][wIdx].firstHit == NULL)
    {   
        int randNum = -1, hitCnt = 0;
        pwc[playerToAct][wIdx].hitAtTurns = (int*)malloc(sizeof(int) * weaponCollection[wIdx].hitchance);
        pwc[playerToAct][wIdx].currentHit = pwc[playerToAct][wIdx].hitAtTurns;
        pwc[playerToAct][wIdx].firstHit = pwc[playerToAct][wIdx].hitAtTurns;

        // generate the hitAtTurns based on the hit chance
        for (int i = 0; i < weaponCollection[wIdx].hitchance; i++)
        {   
            // if the remaining positions needs to be all 1s
            if (weaponCollection[wIdx].hitchance - i == HITCHANCE_BASE - hitCnt)
            {
                randNum = 1;
            }
            else
            {
                randNum = getRandNum(0, 1);
            }
            if (randNum) hitCnt++;

            *(pwc[playerToAct][wIdx].hitAtTurns) = randNum;
            
            // hitAtTurns finally points to the last hit
            pwc[playerToAct][wIdx].hitAtTurns++;
        }
    }

    // apply the damage on opponentPlayer
    damageApplied[playerToAct] = *(pwc[playerToAct][wIdx].currentHit) * weaponCollection[wIdx].damage;
    healthPoint[opponentPlayer] -= damageApplied[playerToAct];
    printf("Player %d used weapon %d. %d damage applied to player %d.\n",
            playerToAct+1, wIdx, damageApplied[playerToAct], opponentPlayer+1);

    pwc[playerToAct][wIdx].currentHit++;
    if (pwc[playerToAct][wIdx].currentHit > pwc[playerToAct][wIdx].hitAtTurns)
    {
        free(pwc[playerToAct][wIdx].firstHit);
        pwc[playerToAct][wIdx].hitAtTurns = NULL;
        pwc[playerToAct][wIdx].currentHit = NULL;
        pwc[playerToAct][wIdx].firstHit = NULL;
    } 

    pwc[playerToAct][wIdx].coolDownTurnsLeft = weaponCollection[wIdx].cooldown;
}

// defend 
void doDefend(int playerToAct, int opponentPlayer)
{
    int dIdx = selectDefenceForDefend(playerToAct);
    // reduce damage based on the opponent's last attack damage
    // the reduction will be added to healthPoint and subtracted from hitPoint of defence
    int damageReduced = (damageApplied[opponentPlayer] * defenceCollection[dIdx].DamageDeduction) > pdc[playerToAct][dIdx].remainingHP? pdc[playerToAct][dIdx].remainingHP :  damageApplied[opponentPlayer] * defenceCollection[dIdx].DamageDeduction;
    healthPoint[playerToAct] += damageReduced;
    printf("Player %d used defence %d. %d damage reduced applied to player %d.\n",
    playerToAct, dIdx, damageReduced, opponentPlayer+1);

    pdc[playerToAct][dIdx].remainingHP -= damageReduced;
    if (pdc[playerToAct][dIdx].remainingHP == 0) pdc[playerToAct][dIdx].isPurchased = -1;
    // reset damage of the opponent's recent attack
    damageApplied[opponentPlayer] = 0;
}

// select option for purchase
// return the option: Q - Quit, W - Weapon, D - Defence
char selectOptionForPurchase(int playerToAct)
{
    printf("Purchase, available options:\n");
    printf("Q - Quit\n");
    if (gold[playerToAct] >= minWeaponPurchaseGold(playerToAct))
    {
        printf("W - Weapon\n");
    }
    if (gold[playerToAct] >= minDefencePurchaseGold(playerToAct))
    {
        printf("D - Defence\n");
    }
    printf("Select an option to proceed: ");
    char op;
    char opCollection[4] = {'q', 'w', 'd', 'u'};

    while (1)
    {   
        if (mode == 'm')
        {
            scanf("\n%c", &op);
        }
        else
        {
            op = opCollection[getRandNum(0, 3)];
        }

        if (toupper(op) != 'Q' && toupper(op) != 'W' && toupper(op) != 'D')
        {
            printf("Invalid input, the option can only be 'Q', 'W', or 'D'.\n");
            sleep(1);
            continue;
        }
        if (toupper(op) == 'Q') 
        {
            printf("Purchase given up, quit now!\n");
            break;
        }
        if (toupper(op) == 'W')
        {
            if (gold[playerToAct] < minWeaponPurchaseGold(playerToAct))
            {
                printf("Not enough gold for weapon purchase, select again!\n");
                sleep(1);
                continue;
            }
            break;
        }
        if (toupper(op) == 'D')
        {
            if (gold[playerToAct] < minDefencePurchaseGold(playerToAct))
            {
                printf("Not enough gold for defence purchase, select again!\n");
                sleep(1);
                continue;
            }
            break;
        }
    }

    return op;
}

int selectWeaponForPurchase(int playerToAct)
{
    printf("Available weapons:\n");
    for (int wIdx = 0; wIdx < 10; wIdx++)
    {
        if (pwc[playerToAct][wIdx].isPurchased == -1 && gold[playerToAct] >= weaponCollection[wIdx].cost)
        {
            printf("%d - Type: %s, Name: %s, Damage: %d, Cool Down (turns): %d, Hit Chance (? turns / ? turns): %d/%d, Cost: %d\n", 
                                wIdx, weaponCollection[wIdx].type, weaponCollection[wIdx].name, 
                                weaponCollection[wIdx].damage, weaponCollection[wIdx].cooldown, 
                                3, weaponCollection[wIdx].hitchance,
                                weaponCollection[wIdx].cost);
        }
    }
    printf("Select a weapon to proceed: ");
    int op;
    while (1)
    {
        if (mode == 'm')
        {
            scanf("%d", &op);
        }
        else
        {
            op = getRandNum(0, 9);
        }

        // check whether the weapon is purchased and "cool"
        if (pwc[playerToAct][op].isPurchased == 1)
        {
            printf("Invalid input, weapon already purchased. Select again!\n");
            sleep(1);
            continue;
        }

        if (gold[playerToAct] < weaponCollection[op].cost)
        {
            printf("Invalid input, not enough gold to purchase this weapon. Select again!\n");
            sleep(1);
            continue;
        }

        printf("Weapon %d purchased.\n", op);
        printf("Type: %s, Name: %s, Damage: %d, Cool Down (turns): %d, Hit Chance (? turns / ? turns): %d/%d, Cost: %d\n", 
                weaponCollection[op].type, weaponCollection[op].name, 
                weaponCollection[op].damage, weaponCollection[op].cooldown, 
                3, weaponCollection[op].hitchance,
                weaponCollection[op].cost);
        return op;
    }
}

int selectDefenceForPurchase(int playerToAct)
{
    // print info of defence
    printf("Available defences:\n");
    for (int dIdx = 0; dIdx < 10; dIdx++)
    {
        if (pdc[playerToAct][dIdx].isPurchased == -1 && gold[playerToAct] >= defenceCollection[dIdx].cost)
        {
            printf("%d - Type: %s, Name: %s, Hit Points: %d, Damage Reduction: %d\n", dIdx, defenceCollection[dIdx].type, defenceCollection[dIdx].name, defenceCollection[dIdx].hitpoints, defenceCollection[dIdx].DamageDeduction);
        }
    }
    printf("Select a defence to proceed: ");
    int op;
    while (1)
    {
        if (mode == 'm')
        {
            scanf("%d", &op);
        }
        else
        {
            op = getRandNum(0, 9);
        }

        // check whether the defence is purchased
        if (pdc[playerToAct][op].isPurchased == 1)
        {
            printf("Invalid input, defence already purchased. Select again!\n");
            sleep(1);
            continue;
        }

        if (gold[playerToAct] < defenceCollection[op].cost)
        {
            printf("Invalid input, not enough gold to purchase this defence. Select again!\n");
            sleep(1);
            continue;
        }

        printf("Defence %d purchased.\n", op);
        printf("Type: %s, Name: %s, Hit Points: %d, Damage Reduction: %d\n", 
                defenceCollection[op].type, defenceCollection[op].name, defenceCollection[op].hitpoints, defenceCollection[op].DamageDeduction);
        return op;
    }
}

// purchase
void doPurchase(int playerToAct)
{   
    char op = selectOptionForPurchase(playerToAct);

    if (toupper(op) == 'Q')
    {
        return;
    }
    else if (toupper(op) == 'W')
    {
        int wIdx = selectWeaponForPurchase(playerToAct);
        pwc[playerToAct][wIdx].isPurchased = 1;
        gold[playerToAct] -= weaponCollection[wIdx].cost;
    }
    else if (toupper(op) == 'D')
    {
        int dIdx = selectDefenceForPurchase(playerToAct);
        pdc[playerToAct][dIdx].isPurchased = 1;
        gold[playerToAct] -= defenceCollection[dIdx].cost;
    }
    else
    {   
        printf("Invalid option, quit now!\n");
        return;
    }
}

// rest
void doRest(playerToAct)
{
    gold[playerToAct] += 150 + consecutiveRestCnt[playerToAct] * 50;
    printf("Player %d has rested continually for %d times! Health points increased by %d.\n", playerToAct + 1, consecutiveRestCnt[playerToAct], 150 + consecutiveRestCnt[playerToAct] * 50);
}

// do action by player
void doAction(char action, int playerToAct, int opponentPlayer)
{   
    if (action == 'a') 
    {
        doAttack(playerToAct, opponentPlayer);
        consecutiveRestCnt[playerToAct] = 0;
    }
    if (action == 'd') 
    {
        doDefend(playerToAct, opponentPlayer);
        consecutiveRestCnt[playerToAct] = 0;
    }
    if (action == 'p')
    {
        doPurchase(playerToAct);
        consecutiveRestCnt[playerToAct] = 0;
    }
    if (action == 'r')
    {
        doRest(playerToAct);
        consecutiveRestCnt[playerToAct]++;
    }
}

// get a random integer in [min, max]
int getRandNum (int min, int max){
  return rand() % (max - min + 1) + min;
}

void printInfo(int player1, int player2)
{   
    printf("Current Info:\n");
    printf("- Player %d", player1 + 1);
    printf(", HP = %d", healthPoint[player1]);
    printf(", Gold = %d", gold[player1]);
    printf(", Weapon = { ");
    for(int i = 0; i < 10; i++)
    {
        if (pwc[player1][i].isPurchased == 1)
        {
            printf("%d ", i);
        }
    }
    printf("}");
    printf(", Defence = { ");
    for(int i = 0; i < 10; i++)
    {
        if (pdc[player1][i].isPurchased == 1)
        {
            printf("%d ", i);
        }
    }
    printf("}\n");
    printf("- Player %d", player2 + 1);
    printf(", HP = %d", healthPoint[player2]);
    printf(", Gold = %d", gold[player2]);
    printf(", Weapon = { ");
    for(int i = 0; i < 10; i++)
    {
        if (pwc[player2][i].isPurchased == 1)
        {
            printf("%d ", i);
        }
    }
    printf("}");
    printf(", Defence = { ");
    for(int i = 0; i < 10; i++)
    {
        if (pdc[player2][i].isPurchased == 1)
        {
            printf("%d ", i);
        }
    }
    printf("}\n\n");
}






