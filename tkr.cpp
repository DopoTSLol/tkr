/*
    Name: Dopo Keane
    Date: 3/18/2022 - 5/25/2022
    File Name: tkr.cpp
    Description: 2D Adventure Game Thing (Final Project for Mr. Hatzai)
*/
//getchar(); = cin.ignore(); (little bit of dev commentary here, but a lot of the comments on the libraries were from a file I had that
//acts as a template for me (I made it myself)

#include <iostream> //simmilar to <Stdio.h>
#include <cstdlib> //stdlib
#include <iomanip> //for setpercision, setw, fixed
#include <ctime> //time.h
#include <cmath> //math.h
#include <windows.h>
#include <cctype>
#include <cstring>
#include <fstream>
#define SIZE 500//declaration of constant

using namespace std;

ifstream load;
ofstream save;//creates key words for input/out functionality
ifstream loadC;
ofstream saveC;

struct inventory
{
    string itemName[100] = {"Grass","Seed","Food","Shell","Sand","Salt","Stone","Flint","Snow","Wool","Stick","Wood","Axe","Pickaxe","Furnace","Campfire","Glass","Iron Ore","Iron Bar","Copper Ore","Copper Bar","Zinc Ore","Zinc Bar","Water Bottle","Shirt","Pants","Hat","Raft","Twine","Brass Bar","Steel Bar","Iron Sword","Reinforced Sword","Chestplate","Armored Pants","Helmet","Fishing Rod","Gauntlet of Balance","Teleporter"};
    //woohoo, really long string declaration! I love having a lot of items!
    int itemAmount[100] = {0};
    int craftingRecip[8][100];
};//end of declaration of structure inventory
struct enemies
{
    string name[7]={"Snake","Tarantula","Giant Squid","Mountain Lion","Lynx","Bear","God"};
    int mHealth[7]={20,15,50,45,45,70,300};
    int mAttk[7]={3,2,12,8,5,5,100};
    int mDef[7]={0,0,5,2,1,3,100};
    int mItem[7]={0,0,0,9,9,9,1};//not used yet
};//end of declaration of structure enemies
struct weather
{
    int turn=0;
    int temp;
};//end of declaration of structure pTime
struct pStat
{
    enemies mob;
    inventory inv;
    weather w;
    int health=100;
    int hunger=100;
    int attk=3;
    int def=0;
    int color=15;
    int teleUses=3;
};//end of declaration of structure pStat

void genWorld(int[][SIZE], int, int, int[][SIZE]);//declaration of functions. The reason why they are so long is because my teacher didnt
//want me to use globals. Trust me, if I could use globals, I would.
void print(int[][SIZE], int, int, int, int, int, int, int[][SIZE]);
void movement(int[][SIZE], int, int, char[], int[][SIZE], int, int, pStat);
void locPrint(int[][SIZE], int, int, int, char[], pStat);
void worldConfig(char[], int&, int&, int [][SIZE], int [][SIZE], int&, int&, int&, pStat&);
char actions(char, int[][SIZE], int[][SIZE], int, int, int&, int&, pStat&, int);
void saveWorld(int[][SIZE], int[][SIZE], int, int, int, int, pStat);
void loadWorld(int[][SIZE], int[][SIZE], int&, int&, int&, int&, pStat&);
void inventory(pStat);
void pickup(pStat&, int);
void craftingLoad(pStat&);
void crafting(pStat&);
void attkDef(pStat&);
void enemies(pStat&, int);
void resetStat(int&, int&, pStat&);
void fishing(int[][SIZE], pStat&, int, int);
void delItem(pStat&);
void teleport(int[][SIZE], int&, int&, pStat&, int, int);

int main()
{
    int arr[SIZE][SIZE];//declaration of arrays and variables
    int foliage[SIZE][SIZE];
    char name[SIZE];
    int worldX, worldY, genCheck, x=0, y=0;
    char loop;
    pStat player;

    srand(time(NULL));//randomizes seed

    loadC.open("craft.txt");
    craftingLoad(player);

    load.open("sav.txt");
    do{
        worldConfig(name, worldX, worldY, arr, foliage, genCheck, x, y, player);

        switch(genCheck)
        {
        case 1:
            do{
                genWorld(arr, worldX, worldY, foliage);//calls functions, because im lazy
            }while(arr[4][8]==2);//end of do-while. Generates world where player doesnt get stuck in water right off the bat
            break;

        default:
            break;
        }//end of switch statement
        movement(arr, worldX, worldY, name, foliage, x, y, player);

        cout<<"Do you want to replay the game? (Y/N)\n";
        cin>>loop;
        cin.ignore();//clears the buffer

        if((loop=='y')||(loop=='Y'))
        {
            resetStat(x, y, player);
        }//end of if-statement
    }while((loop=='y')||(loop=='Y'));//end of do-while

    return 0;//successfully ends the function
}//end of main function
/*===
//  Input: int x, int y, pStat pInv
//  Output: void (pass by)
//  Description: resets the player's stats
===*/
void resetStat(int& x, int& y, pStat& pInv)
{
    x=0;
    y=0;
    pInv.attk=3;
    pInv.def=0;
    pInv.health=100;
    pInv.hunger=100;
    pInv.w.turn=0;

    for(int i=0;i<100;i++)
    {
        pInv.inv.itemAmount[i]=0;
    }//end of for-loop
}//end of function reset
/*===
//  Input: char name[], int& xL, int& yL, int a[][SIZE], int f[][SIZE], int g, int x, int y, pStat pInv
//  Output: void (pass by)
//  Description: Configures the world size, and the player's name
===*/
void worldConfig(char name[], int& xL, int& yL, int a[][SIZE], int f[][SIZE], int& g, int& x, int& y, pStat& pInv)
{
    char fileLoadCheck;

    cout<<"What is your name, adventurer?\n";//scans for character array
    cin.getline(name, SIZE);

    if((strstr(name,"Hatzai"))||(strstr(name,"hatzai")))
    {
        cout<<"McDonald's Mode Activated\n";
        pInv.color = 78;
    }//end of if-statement

    cout<<"Would you like to load a file?(Y/N)\n";
    cin>>fileLoadCheck;

    if((load)&&((fileLoadCheck=='y')||(fileLoadCheck=='Y')))
    {
        loadWorld(a, f, xL, yL, x, y, pInv);
        g=0;
    }
    else
    {
        if(load)
        {

        }
        else
        {
            cout<<"Coudn't load save file, file not found.\n";
        }//end of if-else statement

        cout<<"What is the length of the world?\n";
        do{
            cin>>xL;

            if(xL>SIZE)
                cout<<"Invalid input. Length of world has to be at most "<<SIZE<<endl;
            else if(xL<10)
                cout<<"Invalid input. Length of world has to be at least 10\n";
        }while((xL>SIZE)||(xL<10));//end of do-while

        cout<<"What is the width of the world?\n";
        do{
            cin>>yL;

            if(yL>SIZE)
                cout<<"Invalid input. Width of world has to be at most "<<SIZE<<endl;
            else if(yL<10)
                cout<<"Invalid input. Width of world has to be at least 10\n";
        }while((yL>SIZE)||(yL<10));//end of do-while
        g=1;
    }//end of if-else statement

}//end of function worldConfig
/*===
//  Input: int arr[][], int x, int y, int f[][SIZE]
//  Output: void
//  Description: generates the world
===*/
void genWorld(int a[][SIZE], int x, int y, int f[][SIZE])
{
    int dice8, dice2;

    for(int i=0; i<y; i++)
    {
        for(int j=0; j<x; j++)
        {
            dice2=rand()%5;
            f[i][j]=dice2;
            if((i!=0)&&(j!=0))
            {
                dice8=rand()%13+1;
                if(dice8<7)
                {
                    a[i][j]=a[i][j-1];
                }
                else if((dice8>6)&&(dice8<13))
                {
                    a[i][j]=a[i-1][j];
                }
                else
                {
                    a[i][j]=rand()%8;
                    if(a[i][j]>=6)
                    {
                        a[i][j]=2;
                    }//end of if statement
                }//end of if-else ladder
            }
            else
            {
                a[i][j]=rand()%8;
                if(a[i][j]>=6)
                {
                    a[i][j]=2;
                }//end of if statement
            }
        }//end of for loop
    }//end of for loop
}//end of function genWorld
/*===
//  Input: pStat pInv
//  Output: void (pass by)
//  Description: updates player's attack and defense stats over time
===*/
void attkDef(pStat& pInv)
{
    pInv.attk = 3;//sets attk and def to their default value after each use, to insure the values dont add up over time
    pInv.def = 0;

    if(pInv.inv.itemAmount[32]>0)
    {
        pInv.attk = pInv.attk+5;
    }
    else if(pInv.inv.itemAmount[31]>0)
    {
        pInv.attk = pInv.attk+2;
    }//end of if-else ladder

    if(pInv.inv.itemAmount[33]>0)
    {
        pInv.def = pInv.def + 7;
    }
    else if(pInv.inv.itemAmount[24]>0)
    {
        pInv.def = pInv.def + 3;
    }//end of if-else ladder

    if(pInv.inv.itemAmount[34]>0)
    {
        pInv.def = pInv.def + 4;
    }
    else if(pInv.inv.itemAmount[25]>0)
    {
        pInv.def = pInv.def + 2;
    }//end of if-else ladder

    if(pInv.inv.itemAmount[35]>0)
    {
        pInv.def = pInv.def + 2;
    }
    else if(pInv.inv.itemAmount[26]>0)
    {
        pInv.def = pInv.def + 1;
    }//end of if-else ladder

}//end of function attkDef
/*===
//  Input: int arr[][], int wX, int wY, char name[], int f[][SIZE], int X, int Y, pStat pInv
//  Output: void
//  Description: main movement and action function
===*/
void movement(int arr[][SIZE], int wX, int wY, char name[], int f[][SIZE], int X, int Y, pStat pInv)
{
    int prevPos[2];//declaration of local arrays and variables
    int currentPos=6, movement=0, loopVar=0, dice;
    char input;

    do{
        system("CLS");//clears screen
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 243);//sets the color of the text
        cout<<"=+=Text Kool Run=+=By Dopo Keane=+=\n";

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), pInv.color);//sets the color of the text
        print(arr, 18, 9, Y, X, wX, wY, f);//prints current map
        arr[Y+4][X+8]=currentPos;
        do{
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), pInv.color);//sets the color of the text
            locPrint(arr, currentPos, X, Y, name, pInv);

            prevPos[0]=Y;
            prevPos[1]=X;

            attkDef(pInv);

            enemies(pInv, currentPos);

            input = actions(input, arr, f, wX, wY, X, Y, pInv, currentPos);

            switch(input)
            {
            case 'W'://uppercase W
                Y--;
                loopVar=1;
                break;
            case 'w'://lowercase W
                Y--;
                loopVar=1;
                break;

            case 'A'://uppercase A
                X--;
                loopVar=1;
                break;
            case 'a'://lowercase A
                X--;
                loopVar=1;
                break;

            case 'S'://uppercase S
                Y++;
                loopVar=1;
                break;
            case 's'://lowercase S
                Y++;
                loopVar=1;
                break;

            case 'D'://uppercase D
                X++;
                loopVar=1;
                break;
            case 'd'://lowercase D
                X++;
                loopVar=1;
                break;

            case 'x':
                loopVar=1;
                break;

            case 'p':
                loopVar=1;
                break;

            case 'f':
                loopVar=1;
                break;

            default:
                loopVar=0;
                cout<<"Invalid input\n";
                break;
            }//end of switch. Determines where the player goes dependant on input

            if(((arr[Y+4][X+8]==2)||(Y<-4)||(Y>wY-5)||(X>wX-9)||(X<-8))&&(pInv.inv.itemAmount[27]==0))
            {
                Y=prevPos[0];
                X=prevPos[1];
            }//end of if-statement

            currentPos=arr[Y+4][X+8];
            arr[Y+4][X+8]=6;
            pInv.w.turn++;
        }while(loopVar==0);//end of do-while. Input validation/main movement section of function
        cout<<endl;
        if((pInv.hunger == 0)||(pInv.health == 0))
        {
            movement=1;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);//sets the color of the text
            system("CLS");
            cout<<"Game over.\n"<<"Turns survived: "<<pInv.w.turn<<endl;
        }//end of if statement
    }while(movement==0);//end of do-while
}//end of function movement
/*===
//  Input: char input, int a[][SIZE], int f[][SIZE, int wX, int wY, pStat pInv, int
//  Output: char
//  Description: action function
===*/
char actions(char input, int a[][SIZE], int f[][SIZE], int wX, int wY, int& x, int& y, pStat& pInv, int cPos)
{
    char inputs[SIZE]="WwAaSsDdHhPpFfIiCcEeRrTt";//declaration of local arrays and variables
    char inputReturn[SIZE]="WwAaSsDd";
    char inputThing[SIZE];
    int loop=0, dice, counter=0;

        do{
            switch(loop)
            {
            case 0:
                cout<<"Please input an action (H for Help)\n";
                break;

            case 1:
                if(strstr(inputs, inputThing))
                {

                }
                else
                    cout<<"Invalid input. Please type 'H' for a list of all the commands\n";//end of if-else
                break;

            default:
                break;
            }//end of switch statement
            cin.getline(inputThing, SIZE);

            if(iscntrl(inputThing[0]))
            {
                loop=1;
            }
            else
            {
                if(strstr(inputs, inputThing))
                {
                    if(strstr(inputReturn, inputThing))
                    {
                        cout<<inputThing<<endl;
                        dice = rand()%5;
                        switch(dice)
                        {
                        case 0:
                            counter=0;
                            pInv.hunger--;
                            if(pInv.health<100)
                            {
                                do{
                                    pInv.health++;
                                    counter++;
                                }while((pInv.health<=100)&&(counter<5));//end of do-while loop
                            }//end of if statement
                            break;
                        }//end of switch statement
                        return inputThing[0];
                    }
                    else
                    {

                        input=inputThing[0];
                        switch(input)
                        {
                        case 'H':
                            cout<<"Hello, and welcome to TKR: A Text Adventure game!\n\n";
                            cout<<"Basics:\nIn order to get items, you need to go to a tile with an text on it, then press P.\nYou cannot go into water times, until you get the raft\n";
                            cout<<"You are the purple player.\nYou slowly lose hunger as time goes on.\n To regenerate hunger press ''E''\nThis will also regenerate health.\n";
                            cout<<"If you run out of hunger or health, you die.\n When you lose 1 hunger, you regain 1 health\n\n";
                            cout<<"Commands:\nWASD: Movement\nH: Help\nP: Pickup\nF: File Save\nI: Inventory\nC: Craft\nE: Eat\nQ: Drop Item\nR: Fish\nT: Teleport\n";
                            loop=1;
                            break;

                        case 'h':
                            cout<<"Hello, and welcome to TKR: A Text Adventure game!\n\n";
                            cout<<"Basics:\nIn order to get items, you need to go to a tile with an text on it, then press P.\nYou cannot go into water times, until you get the raft\n";
                            cout<<"You are the purple player.\nYou slowly lose hunger as time goes on.\nTo regenerate hunger press ''E''\nThis will also regenerate health.\n";
                            cout<<"If you run out of hunger or health, you die.\n When you lose 1 hunger, you regain 1 health\n\n";
                            cout<<"Commands:\nWASD: Movement\nH: Help\nP: Pickup\nF: File Save\nI: Inventory\nC: Craft\nE: Eat\nQ: Drop Item\nR: Fish\nT: Teleport\n";
                            loop=1;
                            break;

                        case 'R':
                            fishing(a,pInv, x, y);
                            loop=1;
                            break;

                        case 'r':
                            fishing(a,pInv, x, y);
                            loop=1;
                            break;

                        case 'E':
                            if(pInv.inv.itemAmount[2]!=0)
                            {
                                if(pInv.hunger>=100)
                                {
                                    cout<<"Too full to eat.\n";
                                }
                                else
                                {
                                    pInv.hunger = pInv.hunger+10;
                                    pInv.inv.itemAmount[2]--;
                                    cout<<"Ate Food!\n";
                                    if(pInv.health<100)
                                    {
                                        pInv.health = pInv.health + 5;

                                        if(pInv.health>100)
                                        {
                                            pInv.health=100;
                                        }//end of if-statement
                                    }//end of if statement

                                        if(pInv.hunger>100)
                                        {
                                            pInv.hunger=100;
                                        }//end of if-statement
                                }//end of if-else ladder
                            }
                            else
                            {
                                cout<<"Cant eat, no food!\n";
                            }//end of if-else statement
                            loop=1;
                            break;

                        case 'e':
                            if(pInv.inv.itemAmount[2]!=0)
                            {
                                if(pInv.hunger>=100)
                                {
                                    cout<<"Too full to eat.\n";
                                }
                                else
                                {
                                    pInv.hunger = pInv.hunger+10;
                                    pInv.inv.itemAmount[2]--;
                                    cout<<"Ate Food!\n";
                                    if(pInv.health<100)
                                    {
                                        pInv.health = pInv.health + 5;

                                        if(pInv.health>100)
                                        {
                                            pInv.health=100;
                                        }//end of if-statement
                                    }//end of if statement
                                        if(pInv.hunger>100)
                                        {
                                            pInv.hunger=100;
                                        }//end of if-statement
                                }//end of if-else ladder
                            }
                            else
                            {
                                cout<<"Cant eat, no food!\n";
                            }//end of if-else statement
                            loop=1;
                            break;

                        case 'T':
                            teleport(a, x, y, pInv, wX, wY);
                            loop=1;
                            break;

                        case 't':
                            teleport(a, x, y, pInv, wX, wY);
                            loop=1;
                            break;

                        case 'F':
                            saveWorld(a, f, wX, wY, x, y, pInv);
                            loop=1;
                            break;

                        case 'f':
                            saveWorld(a, f, wX, wY, x, y, pInv);
                            loop=1;
                            break;

                        case 'P':
                            switch(f[y+4][x+8])
                            {
                            case 0:
                                pickup(pInv, cPos);
                                f[y+4][x+8]=1;
                                break;

                            default:
                                cout<<"There is no item here.\n";
                                break;
                            }//end of switch
                            loop=1;
                            break;

                        case 'p':
                            switch(f[y+4][x+8])
                            {
                            case 0:
                                pickup(pInv, cPos);
                                f[y+4][x+8]=1;
                                break;

                            default:
                                cout<<"There is no item here.\n";
                                break;
                            }//end of switch
                            loop=1;
                            break;

                        case 'I':
                            inventory(pInv);
                            loop=1;
                            break;

                        case 'i':
                            inventory(pInv);
                            loop=1;
                            break;

                        case 'C':
                            crafting(pInv);
                            loop=1;
                            break;

                        case 'c':
                            crafting(pInv);
                            loop=1;
                            break;

                        default:
                            loop=0;
                            return 'x';
                            break;
                        }//end of switch
                    }//end of if-else
                }
                else
                {
                    loop=1;
                }//end of if-else
            }
        }while(loop==1);//end of do-while
}//end of function actions
/*===
//  Input: int a[][SIZE], int x, int y, pStat pInv, int wX, int wY
//  Output: void (pass by)
//  Description: main teleport function
===*/
void teleport(int a[][SIZE], int& x, int& y, pStat& pInv, int wX, int wY)
{
    if(pInv.teleUses==0)
    {
        cout<<"Can't teleport, no teleports left!\n";
    }
    else if(pInv.inv.itemAmount[38]==0)
    {
        cout<<"Can't teleport, no teleporter!\n";
    }
    else
    {
        do{
            y=rand()%wY;
            x=rand()%wX;
        }while(a[y+4][y+8]==2);//end of do-while loop

        cout<<"Teleported to "<<x+8<<" "<<y+4<<".\n";
        pInv.teleUses--;
    }//end of if-else ladder
}//end of function teleport
/*===
//  Input: int a[][SIZE], pStat pInv, int x, int y
//  Output: void (pass by)
//  Description: main fishing function
===*/
void fishing(int a[][SIZE], pStat& pInv, int x, int y)
{
    int dice, waterCheck=0;

    if(pInv.inv.itemAmount[36]>0)
    {
        for(int i=0; i<=8; i++)
        {
            switch(i)
            {
            case 0:
                if(a[y+5][x+9]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 1:
                if(a[y+5][x+8]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 2:
                if(a[y+5][x+7]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 3:
                if(a[y+4][x+7]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 4:
                if(a[y+3][x+7]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 5:
                if(a[y+3][x+8]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 6:
                if(a[y+3][x+9]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 7:
                if(a[y+4][x+9]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;

            case 8:
                if(a[y+4][x+8]==2)
                {
                    waterCheck++;
                }//end of if statement
                break;
            }//end of switch statement
        }//end of for-loop

        if(waterCheck!=0)
        {
            dice=rand()%100;
            if(dice<30)
            {
                cout<<"Sorry bucko, but you got nothing.\n";
            }
            else if((dice>30)&&(dice<98))
            {
                cout<<"Caught a fish!\n";
                pInv.inv.itemAmount[2]++;
            }
            else if(pInv.inv.itemAmount[37] == 0)
            {
                cout<<"Caught the Gauntlet of Balance! Monster spawn rate reduced!\n";
                pInv.inv.itemAmount[37]++;
            }
            else
            {
                cout<<"Sorry bucko, but you got nothing.\n";
            }//end if if-else ladder
        }
        else
        {
            cout<<"You are unable to fish, no water nearby!\n";
        }//end of if-else statement
    }
    else
    {
        cout<<"You are unable to fish, you got no rod!\n";
    }//end of if-else statement
}//end of declaration of function fishing
/*===
//  Input: pStat pInv int b
//  Output: void(pass by)
//  Description: Main command for enemies
===*/
void enemies(pStat& pinv, int b)
{
    int dice, health,doCombat=0, damage, monDamage;//declaration of local arrays and variables
    char attkCommand;

    health=pinv.mob.mHealth[b];
    if(pinv.inv.itemAmount[37]!=0)
    {
        dice=rand()%80;
    }
    else
    {
        dice=rand()%40;
    }

    if(b!=6)
    {
        switch(dice)
        {
        case 0:
            cout<<"You are under attack by a "<<pinv.mob.name[b]<<"!\nEngaging in Battle...\n";
            do{
                cout<<pinv.mob.name[b]<<": "<<health<<"\nYou: "<<pinv.health<<endl;
                do{
                    cin.ignore();//clears the buffer
                    cout<<"Attack or run away?(A or R)\n";
                    cin>>attkCommand;
                    if((attkCommand!='A')&&(attkCommand!='a')&&(attkCommand!='R')&&(attkCommand!='r'))
                    {
                        cout<<"Invalid Input. You must input either A for attack or R for run.\n";
                    }
                }while((attkCommand!='A')&&(attkCommand!='a')&&(attkCommand!='R')&&(attkCommand!='r'));//end of do-while

                switch(attkCommand)
                {
                case 'R':
                    dice=rand()%15;
                    switch(dice)
                    {
                    case 0:
                        cout<<"You successfully ran away from the "<<pinv.mob.name[b]<<"!\n";
                        doCombat=1;
                        break;

                    default:
                        cout<<"You couldnt run away!\n";

                        monDamage = (pinv.mob.mAttk[b]-pinv.def)+(rand()%3-1);

                        if(monDamage<=0)
                        {
                            monDamage=1;
                        }//end of if statement

                        for(int i=0;i<=monDamage;i++)
                        {
                            if(pinv.health==1)
                            {
                                pinv.health--;
                                doCombat=1;
                                break;
                            }
                            else
                            {
                                pinv.health--;
                            }//end of if-else statement
                        }//end of for loop
                        break;
                    }//end of switch statement
                    break;

                case 'r':
                    dice=rand()%15;
                    switch(dice)
                    {
                    case 0:
                        cout<<"You successfully ran away from the "<<pinv.mob.name[b]<<"!\n";
                        doCombat=1;
                        break;

                    default:
                        cout<<"You couldnt run away!\n";
                        monDamage = (pinv.mob.mAttk[b]-pinv.def)+(rand()%3-1);

                        if(monDamage<=0)
                        {
                            monDamage=1;
                        }//end of if statement
                        for(int i=0;i<=monDamage;i++)
                        {
                            if(pinv.health==1)
                            {
                                pinv.health--;
                                doCombat=1;
                                break;
                            }
                            else
                            {
                                pinv.health--;
                            }//end of if-else statement
                        }//end of for loop
                        break;
                    }//end of switch statement
                    break;

                case 'a':
                    damage = (pinv.attk-pinv.mob.mDef[b])+(rand()%3-1);
                    monDamage = (pinv.mob.mAttk[b]-pinv.def)+(rand()%3-1);

                    if(monDamage<=0)
                    {
                        monDamage=1;
                    }//end of if statement

                    if(damage<=0)
                    {
                       damage=1;
                    }//end of if-statement
                    for(int i=0;i<=damage;i++)
                    {
                        if(health==1)
                        {
                            health--;
                            doCombat=1;
                            dice=rand()%3+1;
                            pinv.inv.itemAmount[2]=pinv.inv.itemAmount[2]+dice;
                            cout<<"Successfully killed the "<<pinv.mob.name[b]<<endl;
                            cout<<"Gained "<<dice<<" "<<pinv.inv.itemName[2]<<endl;
                            break;
                        }
                        else
                        {
                            health--;
                        }//end of if-else statement
                    }//end of for loop

                    for(int i=0;i<=monDamage;i++)
                    {
                        if(pinv.health==1)
                        {
                            pinv.health--;
                            doCombat=1;
                            break;
                        }
                        else
                        {
                            pinv.health--;
                        }//end of if-else statement
                    }//end of for loop
                    break;

                case 'A':
                    damage = (pinv.attk-pinv.mob.mDef[b])+(rand()%3-1);

                    monDamage = (pinv.mob.mAttk[b]-pinv.def)+(rand()%3-1);

                    if(monDamage<=0)
                    {
                        monDamage=1;
                    }//end of if statement

                    if(damage<=0)
                    {
                       damage=1;
                    }//end of if-statement
                    for(int i=0;i<=damage;i++)
                    {
                        if(health==1)
                        {
                            health--;
                            doCombat=1;
                            dice=rand()%3+1;
                            pinv.inv.itemAmount[2]=pinv.inv.itemAmount[2]+dice;
                            cout<<"Successfully killed the "<<pinv.mob.name[b]<<endl;
                            cout<<"Gained "<<dice<<" "<<pinv.inv.itemName[2]<<endl;
                            break;
                        }
                        else
                        {
                            health--;
                        }//end of if-else statement
                    }//end of for loop

                    for(int i=0;i<=monDamage;i++)
                    {
                        if(pinv.health==1)
                        {
                            pinv.health--;
                            doCombat=1;
                            break;
                        }
                        else
                        {
                            pinv.health--;
                        }//end of if-else statement
                    }//end of for loop
                    break;
                }//end of switch statement
            }while(doCombat==0);//end of do-while
            break;

        default:

            break;
        }//end of switch statement
    }//end of if statement
}//end of function enemies
/*===
//  Input: pStat pInv
//  Output: void (pass by)
//  Description: main crafting function
===*/
void crafting(pStat& pInv)
{
    int loopVar, cRecipe;//declaration of local variables
    char contCraft;

        loopVar=0;
        cout<<"What would you like to craft?\n";
        for(int i=0; i<20; i++)
        {
            cout<<i<<": "<<pInv.inv.craftingRecip[6][i]<<" "<<pInv.inv.itemName[pInv.inv.craftingRecip[7][i]]<<" = ";
            for(int j=0; j<6; j++)
            {
                switch(pInv.inv.craftingRecip[j][i])
                {
                case -1:
                    break;

                default:
                    if(j%2==1)
                    {
                        switch(j)
                        {
                        case 5:
                            cout<<pInv.inv.itemName[pInv.inv.craftingRecip[j][i]]<<".";
                            break;

                        default:
                            cout<<pInv.inv.itemName[pInv.inv.craftingRecip[j][i]]<<", ";
                            break;
                        }//end of switch statement
                    }
                    else
                    {
                        cout<<pInv.inv.craftingRecip[j][i]<<" ";
                    }//end of if-else statement. Mainly just formatting stuff
                    break;
                }//end of switch-statement
            }//end of for-loop
            cout<<endl;
        }//end of for-loop
    do{
        loopVar=0;
        do{
            cin>>cRecipe;

            if(cRecipe>19)
            {
                cout<<"Invalid input. Number is not a valid option.\n";
            }//end of if statement
            else
            {
                for(int i=0; i<8;i=i+2)
                {
                    switch(i)
                    {
                    case 6:
                        if(loopVar==0)
                        {
                            for(int j=0;j<6;j=j+2)
                            {
                                switch(pInv.inv.craftingRecip[j][cRecipe])
                                {
                                case -1:
                                    break;
                                default:
                                    if(((cRecipe>=10)&&(cRecipe<=13))&&(pInv.inv.craftingRecip[j+1][cRecipe]==14))//for furnace recipes
                                    {
                                        pInv.inv.itemAmount[14]++;
                                    }//end of if statement
                                    pInv.inv.itemAmount[pInv.inv.craftingRecip[j+1][cRecipe]]=pInv.inv.itemAmount[pInv.inv.craftingRecip[j+1][cRecipe]]-pInv.inv.craftingRecip[j][cRecipe];
                                }//end of switch statement
                            }//end of for loop
                            pInv.inv.itemAmount[pInv.inv.craftingRecip[7][cRecipe]]=pInv.inv.itemAmount[pInv.inv.craftingRecip[7][cRecipe]]+pInv.inv.craftingRecip[6][cRecipe];
                            cout<<"Successfully crafted item.\n";
                        }//end of if statement
                        break;

                    default:
                        switch(pInv.inv.craftingRecip[i][cRecipe])
                        {
                        case -1:
                            break;

                        default:
                            if(pInv.inv.craftingRecip[i][cRecipe]>pInv.inv.itemAmount[pInv.inv.craftingRecip[i+1][cRecipe]])
                            {
                                cout<<"Not enough "<<pInv.inv.itemName[pInv.inv.craftingRecip[i+1][cRecipe]]<<" to craft item\n";
                                loopVar=1;
                            }
                            else
                            {

                            }//end of if-else statement
                            break;
                        }//end of switch statement
                        break;
                    }//end of switch statement
                }//end of for-loop
            }//end of if-else statement
        }while((cRecipe>18)||(cRecipe<0));

        do{
            if(loopVar==1)
            {
                cout<<"Unable to craft item. Would you like to try and craft something else?(Y/N)\n";//asks user if they would like to craft another item
            }
            else
            {
                cout<<"Would you like to try and craft something else?(Y/N)\n";
            }//end of if-else statement
                cin>>contCraft;
                switch(contCraft)
                {
                case 'N':
                    loopVar=0;
                    break;

                case 'n':
                    loopVar=0;
                    break;

                case 'Y':
                    loopVar=1;
                    break;

                case 'y':
                    loopVar=1;
                    break;

                default:
                    loopVar=2;
                    cout<<"Invalid input. Please input either Y or N\n";
                    break;
                }//end of switch statement
        }while(loopVar==2);//end of do-while loop
    }while(loopVar==1);//end of do-while loop
}//end of function pInv
/*===
//  Input: pStat pInv
//  Output: void (pass by)
//  Description: loads all the crafting recipes to the game (via text file)
===*/
void craftingLoad(pStat& pInv)
{
    for(int i=0; i<100; i++)
    {
        for(int j=0; j<8; j++)
        {
            loadC>>pInv.inv.craftingRecip[j][i];
        }//end of for loop
    }//end of for loop
}//end of function craftingLoad
/*===
//  Input: pStat pInv
//  Output: void
//  Description: prints out player's inventory
===*/
void inventory(pStat pInv)
{
    int counter=0;//declaration of local variables

    for(int i=0; i<100;i++)
    {
        if(pInv.inv.itemAmount[i]!=0)
        {
            cout<<pInv.inv.itemName[i]<<": "<<pInv.inv.itemAmount[i]<<endl;
            counter++;
        }//end of if statement
    }//end of for loop

    if(counter==0)
    {
        cout<<"No Items in Inventory\n";
    }//end of if-statement
}//end of function inventory
/*===
//  Input: pStat pInv, int cPos
//  Output: void
//  Description: picks up item off of ground
===*/
void pickup(pStat& pInv, int cPos)
{
    int dice, itemDice;//declaration of local variables

    switch(cPos)
    {
    case 0:
        dice=rand()%10;
        if(dice >= 3)
        {
            itemDice=rand()%5+1;
            pInv.inv.itemAmount[0]= pInv.inv.itemAmount[0]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[0]<<endl;
        }
        else
        {
            itemDice=rand()%5+1;
            pInv.inv.itemAmount[1] = pInv.inv.itemAmount[1]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[1]<<endl;
        }//end of if-else statement
        break;

    case 1:
        dice=rand()%5;
        if(dice<2)
        {
            itemDice=rand()%3+1;
            pInv.inv.itemAmount[2] = pInv.inv.itemAmount[2]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[2]<<endl;
        }
        else if((dice>=2)&&(dice<5))
        {
            itemDice=rand()%5+1;
            pInv.inv.itemAmount[4] = pInv.inv.itemAmount[4]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[4]<<endl;
        }
        else
        {
            itemDice=rand()%5+1;
            pInv.inv.itemAmount[3] = pInv.inv.itemAmount[3]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[3]<<endl;
        }//end of if-else ladder
        break;

    case 2:
        dice=rand()%2;
        switch(dice)
        {
        case 0:
            itemDice=rand()%5+1;
            pInv.inv.itemAmount[3] = pInv.inv.itemAmount[3]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[3]<<endl;
            break;

        case 1:
            itemDice=rand()%5+1;
            pInv.inv.itemAmount[5] = pInv.inv.itemAmount[5]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[5]<<endl;
            break;
        }//end of switch statement
        break;

    case 3:
        dice=rand()%6;
        switch(pInv.inv.itemAmount[13])
        {
        case 0:
            if(dice<2)
            {
                itemDice=rand()%5+1;
                pInv.inv.itemAmount[6] = pInv.inv.itemAmount[6]+itemDice;
                cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[6]<<endl;
            }
            else
            {
                itemDice=rand()%5+1;
                pInv.inv.itemAmount[7] = pInv.inv.itemAmount[7]+itemDice;
                cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[7]<<endl;
            }//end of if-else statement
            break;

        default:
            switch(dice)
            {
            case 0:
                itemDice=rand()%5+1;
                pInv.inv.itemAmount[7] = pInv.inv.itemAmount[7]+itemDice;
                cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[7]<<endl;
                break;

            case 1:
                itemDice=rand()%5+1;
                pInv.inv.itemAmount[17] = pInv.inv.itemAmount[17]+itemDice;
                cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[17]<<endl;
                break;

            case 2:
                itemDice=rand()%5+1;
                pInv.inv.itemAmount[19] = pInv.inv.itemAmount[19]+itemDice;
                cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[19]<<endl;
                break;

            case 3:
                itemDice=rand()%5+1;
                pInv.inv.itemAmount[21] = pInv.inv.itemAmount[21]+itemDice;
                cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[21]<<endl;
                break;

            default:
                itemDice=rand()%5+1;
                pInv.inv.itemAmount[6] = pInv.inv.itemAmount[6]+itemDice;
                cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[6]<<endl;
                break;
            }//end of switch statement
            break;
        }//end of switch statement
        break;

    case 4:
        dice=rand()%10;
        if(dice>2)
        {
            itemDice=rand()%3+1;
            pInv.inv.itemAmount[8] = pInv.inv.itemAmount[8]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[8]<<endl;
        }
        else
        {
            itemDice=rand()%2+1;
            pInv.inv.itemAmount[9] = pInv.inv.itemAmount[9]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[9]<<endl;
        }//end of if-else statement
        break;

    case 5:
        itemDice=rand()%3+1;
        pInv.inv.itemAmount[10]=pInv.inv.itemAmount[10]+itemDice;
        cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[10]<<endl;
        switch(pInv.inv.itemAmount[12])
        {
        default:
            itemDice=rand()%4+1;
            pInv.inv.itemAmount[11]=pInv.inv.itemAmount[11]+itemDice;
            cout<<"Picked up "<<itemDice<<" "<<pInv.inv.itemName[11]<<endl;
            break;

        case 0:
            break;
        }//end of switch statement
        break;

    default:
        cout<<"Nothing to pickup\n";
        break;
    }//end of switch statement. Determines the biome, then determines what item should be picked up via random chance
}//end of function pickup
/*===
//  Input: int arr[][], int loc, int x, int y, char name[], pStat pInv
//  Output: void
//  Description: prints statistics to help the player
===*/
void locPrint(int arr[][SIZE], int loc, int x, int y, char name[], pStat pInv)
{
    string biome[7] = {"Plains", "Desert", "Ocean", "Mountain", "Tundra", "Forest", "N/A"};//declaration of local arrays/variables

    switch(loc)
    {
    case 0:
        pInv.w.temp=rand()%23+60;
        break;

    case 1:
        pInv.w.temp=rand()%13+96;
        break;

    case 2:
        pInv.w.temp=rand()%4+36;
        break;

    case 3:
        pInv.w.temp=rand()%26-15;
        break;

    case 4:
        pInv.w.temp=rand()%10-5;
        break;

    case 5:
        pInv.w.temp=rand()%17+46;
        break;

    default:
        pInv.w.temp=50;
        break;
    }//end of switch statement. determines temperature

    cout<<"Location: ("<<x+8<<","<<y+4<<")"<<setw(15)<<"Biome: "<<biome[loc]<<endl<<"Player:   "<<name<<endl;
    cout<<"Temperature: "<<pInv.w.temp<<"F"<<setw(10)<<"Hunger: "<<pInv.hunger<<" Health: "<<pInv.health<<endl;
    cout<<"Attack: "<<pInv.attk<<setw(10)<<"Defense: "<<pInv.def<<setw(10)<<"Turn: "<<pInv.w.turn<<endl;
}//end of function locPrint
/*===
//  Input: int arr[][], int x, int y, int posY, int posX, int wX, int wY, int f[][SIZE]
//  Output: void
//  Description: prints the layout of the area/map
===*/
void print(int a[][SIZE], int x, int y, int posY, int posX, int wX, int wY, int f[][SIZE])
{
    for(int i=0; i<y; i++)//woohoo, uber long nested for loop
    {
        for(int j=0; j<x; j++)
        {
            if((posY+i>=0)&&(posY+i<wY)&&(posX+j>=0)&&(posX+j<wX))
            {
                switch(a[i+posY][j+posX])
                {
                case 0:
                    switch(f[i+posY][j+posX])
                    {
                    case 0:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 162);//grass
                        break;

                    default:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 170);//grass
                        break;
                    }
                    cout<<"^";
                    break;

                case 1:
                    switch(f[i+posY][j+posX])
                    {
                    case 0:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 228);//sand
                        break;

                    default:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 238);//sand
                        break;
                    }
                    cout<<"0";
                    break;

                case 2:
                    switch(f[i+posY][j+posX])
                    {
                    case 0:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 179);//water
                        break;

                    default:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 187);//water
                        break;
                    }
                    cout<<"X";
                    break;

                case 3:
                    switch(f[i+posY][j+posX])
                    {
                    case 0:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 120);//stone
                        break;

                    default:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 119);//stone
                        break;
                    }
                    cout<<"O";
                    break;

                case 4:
                    switch(f[i+posY][j+posX])
                    {
                    case 0:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 247);//snow
                        break;

                    default:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 255);//snow
                        break;
                    }
                    cout<<"X";
                    break;

                case 5:
                    switch(f[i+posY][j+posX])
                    {
                    case 0:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);//grass
                        break;

                    default:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 34);//grass
                        break;
                    }
                    cout<<"|";
                    break;

                case 6:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 221);//player
                    cout<<"O";
                    break;
                }//end of switch statement. Determines what color the tile of a selected piece is
            }
            else
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);//void
                cout<<"X";
            }//end of if-else ladder. Prints out the biomes and items when specified
        }//end of for loop
        cout<<endl;
    }//end of for loop
}//end of function print
/*===
//  Input: a[][SIZE], int f[][SIZE], int wX, int wY, int x, int y, pStat pInv
//  Output: void (wX/wY pass-by)
//  Description: loads the save file
===*/
void loadWorld(int a[][SIZE], int f[][SIZE], int& wX, int& wY, int& x, int& y, pStat& pInv)
{
    load>>wX;
    load>>wY;
    load>>x;
    load>>y;
    load>>pInv.health;
    load>>pInv.hunger;

    for(int i=0; i<100;i++)
    {
        load>>pInv.inv.itemAmount[i];
    }//end of for loop. Loads inventory

    for(int i=0; i<wY; i++)
    {
        for(int j=0; j<wX; j++)
        {
            load>>a[i][j];
        }//end of for loop
    }//end of for loop. Loads world map

    for(int i=0; i<wY; i++)
    {
        for(int j=0; j<wX; j++)
        {
            load>>f[i][j];
        }//end of for loop
    }//end of for loop. Loads pickup items
}//end of function loadWorld
/*===
//  Input: a[][SIZE], int f[][SIZE], int wX, int wY, int x, int y, pStat pInv
//  Output: void
//  Description: writes the save file
===*/
void saveWorld(int a[][SIZE], int f[][SIZE], int wX, int wY, int x, int y, pStat pInv)
{
    save.open("sav.txt");
    cout<<"Loading file...\n";

    save<<wX<<endl;
    save<<wY<<endl;
    save<<x<<endl;
    save<<y<<endl;
    save<<pInv.health<<endl;
    save<<pInv.hunger<<endl;//saves player stats

    for(int i=0; i<100;i++)
    {
        save<<pInv.inv.itemAmount[i]<<" ";
    }//end of for loop. Saves inventory
    save<<endl;

    for(int i=0; i<wY;i++)
    {
        for(int j=0; j<wX; j++)
        {
            save<<a[i][j]<<" ";
        }//end of for loop
        save<<endl;
    }//end of for loop. Saves main map

    for(int i=0; i<wY;i++)
    {
        for(int j=0; j<wX; j++)
        {
            save<<f[i][j]<<" ";
        }//end of for loop
        save<<endl;
    }//end of for loop. Saves pickup items
    cout<<"Save complete!\n";
}//end of function saveWorld
/*
    Dev Log:

    Pre-Alpha(3/18/2022-3/23/2022)
        -Development of the "game" began as a test with 2D arrays. EXTREMELY Buggy.
    Alpha(3/23/2022-4/16/2022)
        Alpha 1
            -Added colors, actual movement
            -Added Borders
            -Fixed some bugs, like how the game just spazzed out whenever you walked out of bounds, stuff like that
            -Made walking on water impossible
            -Fixed bug, where you could walk on water (kinda)
        Alpha 2
            -Added name, and world configuration
            -Added foliage/items
            -Actually added actions besides movement
                -Theres a bug when you hit enter, you imprint yourself(like the purple tile that is you)
                -Tried fixing it multiple times
                -(Just fixed the bug)
            -Made code slightly more efficient
        Alpha 3
            -Added saving/loading functionality
                -Added saving of the X&Y Position
            -Added basic item pickup code & inventory system
            -Fixed inventory/item pickup
            -Added over world item regeneration
            -Started working on crafting
            -Added crafting
            -Added crafting failsafe in case someone tries to craft without enough items
            -Added ways to get the ores and wood, as well as functionality to raft
    Beta(4/17/2022-5/24/2022)
        Beta 1(4/17/2022-5/4/2022)
            - Moved most of the player stats to a structure (like temperature)
            - Added health and hunger
            - Added Attack and Defense Stats
            - Added more crafting recipes, specifically smelting recipes
        Beta 2(5/5/2022-5/24/2022)
            - Added enemy stats
            - Added enemy encounters
            - Optimized enemy encounters
            - Slightly updated World generation to make water more common
            - Added regeneration script
            - Optimized Eating Command
            - Added option to throw away item
            - Added ability to continue game after death
            - Added stat reset
    Official Release (5/25/2022 - ?/?/2022)
        Official Release 1 (5/25/2022 - 5/26/2022)
            - Added McDonald's Mode
            - Better Help Command
        Official Release 2 (5/27/2022)
            - Better crafting command
            - Fixed purple glitch
            - Inventory command checks if there is nothing in inventory now
            - Mobs now drop food
            - Mobs now do a random amount of damage
            - Changed cactus to food
        Official Release 3 (5/27/2022 - 6/1/2022)
            - Added Gauntlet, Teleporter, Fishing Rod
            - Added functionality to the teleporter (WIP)
            - Added Drop command (WIP)
        Official Release 4 (???)
            -
        Official Release 5 (???)
            -
*/
