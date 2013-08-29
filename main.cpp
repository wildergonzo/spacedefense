#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
using namespace std;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

void gotoxy(int x, int y){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hCon, dwPos);
}
void hideCursor(){
    HANDLE hCon;
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize = 50;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hCon, &cci);
}

void draw_limits(){
    for(int i=2;i<78;i++){
        gotoxy(i,3);printf("%c", 205);
        gotoxy(i,33);printf("%c", 205);
    }
    for(int i=4;i<33;i++){
        gotoxy(2,i);printf("%c", 186);
        gotoxy(77,i);printf("%c", 186);
    }
    gotoxy(2,3); printf("%c", 201);
    gotoxy(2,33); printf("%c", 200);
    gotoxy(77,3); printf("%c", 187);
    gotoxy(77,33); printf("%c", 188);
}

class SHIP{
    int x,y;
    int health_points;
    int lives;
public:
    SHIP(int _x, int _y, int _health_points, int _lives):x(_x), y(_y), health_points(_health_points), lives(_lives){}
    int X(){return x;}
    int Y(){return y;}
    int returnLives(){return lives;}
    void decreaseHealthPoints(){health_points--;}
    void drawShip();
    void clearShip();
    void moveShip();
    void drawHealth();
    void explode();
};

void SHIP::drawShip(){
    gotoxy(x,y);printf("  %c", 30);
    gotoxy(x,y+1);printf(" %c%c%c", 40, 207, 41);
    gotoxy(x,y+2);printf("%c%c %c%c", 30, 190, 190, 30);
}
void SHIP::clearShip(){
    gotoxy(x,y);printf("         ");
    gotoxy(x,y+1);printf("         ");
    gotoxy(x,y+2);printf("         ");
}
void SHIP::moveShip(){
    if(kbhit()){
        char key = getch();
        clearShip();
        if(key == LEFT && x>3) x--;
        if(key == RIGHT && x+6<77) x++;
        if(key == UP && y>4) y--;
        if(key == DOWN && y+3<33) y++;
        if(key == 'e') health_points--;
        drawShip();
        drawHealth();
    }
}
void SHIP::drawHealth(){
    gotoxy(50,2); printf("LIVES %d", lives);
    gotoxy(67,2); printf("Health:");
    gotoxy(74,2); printf("        "); //dont know why??
    for(int i=0; i<health_points; i++){
        gotoxy(75+i,2);printf("%c",3);
    }
}
void SHIP::explode(){
    if(health_points == 0){
        clearShip();
        gotoxy(x,y);printf("   **   ");
        gotoxy(x,y+1);printf("  ***  ");
        gotoxy(x,y+2);printf("   **   ");
        Sleep(300);
        clearShip();
        gotoxy(x,y);printf("* ** *");
        gotoxy(x,y+1);printf(" **** ");
        gotoxy(x,y+2);printf("* ** *");
        Sleep(300);
        clearShip();
        lives--;
        health_points=3;
        drawHealth();
        drawShip();
    }
}
class STEROID{
    int x,y;
public:
    STEROID(int _x, int _y):x(_x), y(_y){}
    int X(){return x;}
    int Y(){return y;}
    void drawSteroid();
    void moveSteroid();
    void impact(class SHIP &ship);
};
void STEROID::drawSteroid(){
    gotoxy(x,y);printf("%c", 216);
}
void STEROID::moveSteroid(){
    gotoxy(x,y);printf(" ");
    y++;
    if(y>32){
        x=rand()%71 + 4;
        y=4;
    }
    drawSteroid();
}
void STEROID::impact(class SHIP &ship){
    if(x>=ship.X() && x<ship.X()+6 && y>=ship.Y() && y<=ship.Y()+2){
        ship.decreaseHealthPoints();
        ship.clearShip();
        ship.drawShip();
        ship.drawHealth();
        x=rand()%71 + 4;
        y=4;
    }
}
class SHOT{
    int x,y;
public:
    SHOT(int _x, int _y):x(_x), y(_y){}
    int X(){return x;}
    int Y(){return y;}
    void moveBullet();
    bool bulletLimit();
};
void SHOT::moveBullet(){
    gotoxy(x,y);printf(" ");
    y--;
    gotoxy(x,y);printf("*");
}
bool SHOT::bulletLimit(){
    if(y==4) return true;
    return false;
}

int main()
{
    //gotoxy(15,10)
    hideCursor();
    draw_limits();
    SHIP battlecruser(37,30,3,3);
    battlecruser.drawShip();
    battlecruser.drawHealth();

    //STEROID steroid1(10,4), steroid2(4,8), steroid3(15,10);

    list<STEROID*> steroids;
    list<STEROID*>::iterator itS;
    for(int i=0; i<5; i++){
        steroids.push_back(new STEROID(rand()%75 + 3, rand()%5 + 4));
    }
    list<SHOT*> bullets;
    list<SHOT*>::iterator itB; //pointer index

    bool game_over=false;
    int points =0;

    while(!game_over){

        gotoxy(4,2);printf("Points: %d", points);
        if(kbhit()){
            char key = getch();
            if(key=='a')
                bullets.push_back(new SHOT(battlecruser.X()+2, battlecruser.Y()-1));
        }
        for(itB = bullets.begin();itB != bullets.end(); itB++){
            (*itB)->moveBullet();
            if((*itB)->bulletLimit()){
                gotoxy((*itB)->X(),(*itB)->Y());printf(" ");
                delete(*itB);
                itB = bullets.erase(itB); //next bullet
            }
        }
        /*steroid1.moveSteroid(); steroid1.impact(battlecruser);
        steroid2.moveSteroid(); steroid2.impact(battlecruser);
        steroid3.moveSteroid(); steroid3.impact(battlecruser);*/

        for(itS = steroids.begin();itS != steroids.end();itS++){
            (*itS)->moveSteroid();
            (*itS)->impact(battlecruser);
        }

        for(itS = steroids.begin();itS != steroids.end();itS++){
            for(itB = bullets.begin();itB != bullets.end(); itB++){
                if((*itS)->X() == (*itB)->X() && ((*itS)->Y()+1 == (*itS)->Y() || (*itS)->Y() == (*itS)->Y())){
                    gotoxy((*itB)->X(), (*itB)->Y());printf(" ");
                    delete(*itB);
                    itB = bullets.erase(itB);

                    steroids.push_back(new STEROID(rand()%75+3,4));
                    gotoxy((*itS)->X(), (*itS)->Y());printf(" ");
                    delete(*itS);
                    itS = steroids.erase(itS);

                    points+=5;
                }
            }
        }

        if(battlecruser.returnLives()==0) game_over = true; //gameover

        battlecruser.explode();
        battlecruser.moveShip();
        Sleep(30);
    }
    return 0;
}
