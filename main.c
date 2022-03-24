#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <stdlib.h>
#include <winuser.h>
#define U 1
#define D 2
#define L 3
#define R 4

const int TAILLE = 50;
int status = 4;
int score = 0;
FILE *fp;
int foodX;
int foodY;

typedef struct SNAKE{
 int x;
 int y;
 struct SNAKE *suiv;
}snake;

void Pos(int x, int y){
 COORD pos;
 HANDLE hOutput;
 pos.X = x;
 pos.Y = y;
 hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleCursorPosition(hOutput, pos);
}

void red () {
  printf("\033[1;31m");
}
void yellow() {
  printf("\033[1;33m");
}
void blue(){
  printf("\x1b[34m");
}
void reset () {
  printf("\033[0m");
}

void drawMap(){{
 int i;
 for (i = 0; i<58; i += 2){
  Pos(i, 0);
  printf(" - ");
  Pos(i, 26);
  printf(" - ");
 }
 for (i = 1; i<26; i++){
  Pos(0, i);
  printf(" | ");
  Pos(56, i);
  printf(" | ");
 }
}
}

void afficheSnake(snake * tete){
    snake * temp = tete;
    Pos(temp->x,temp->y);
    red();
    printf("x");
    reset();
    yellow();
    temp = temp->suiv;
    while(temp!=NULL){
        Pos(temp->x,temp->y);
        printf("o");
        temp = temp->suiv;
    }
    reset();
}

snake * createSnake(){
    snake * tete = (snake*)malloc(sizeof(snake));
    tete->x = 23;
    tete->y = 5;
    snake * temp = tete;
    for(int i = 0 ; i<3 ;i++){
        temp->suiv = (snake*)malloc(sizeof(snake));
        temp->suiv->x = temp->x - 1;
        temp->suiv->y = temp->y;
        temp = temp->suiv;
    }
    temp->suiv = NULL;
    return tete;
}
int snakeStatus(){
    if(GetAsyncKeyState(VK_UP) && status != D){
        status = U;
    }
    else{
    if(GetAsyncKeyState(VK_DOWN) && status != U){
        status = D;
    }
    else{
    if(GetAsyncKeyState(VK_RIGHT) && status != L){
        status = R;
    }
    else{
     if(GetAsyncKeyState(VK_LEFT) && status != R){
        status = L;
    }
    }
    }
    }
}

void drawScore(){
    Pos(60,4);
    blue();
    printf("SCORE : %d", score);
    reset();
}

void moveSnake(snake *tete, int status){
    snakeStatus();
    system("cls");
    drawScore();
    snake * temp = tete;
    int myX = tete->x;
    int myY = tete->y;
    switch(status){
        case U : tete->y = tete->y - 1;
        break;
        case D : tete->y = tete->y + 1;
        break;
        case R : tete->x = tete->x + 1;
        break;
        case L : tete->x  = tete->x - 1;
    }

    temp = temp->suiv;
    snake * newElement = (snake *)malloc(sizeof(snake));
    newElement->x = myX;
    newElement->y = myY;
    newElement->suiv = temp;
    tete->suiv = newElement;
    while(temp->suiv->suiv != NULL){
        temp = temp->suiv;
    }
    snake * toFree = temp->suiv;
    temp->suiv = NULL;
    free(toFree);
    drawMap();
    drawFood();
    Pos(tete->x,tete->y);
    afficheSnake(tete);

}

int colission(snake * tete){
    snake * temp = tete->suiv;
    int i = 1;
    while(temp!=NULL){
        if(temp->x == tete->x && temp->y == tete->y){
                printf("TETE POS : x = %d, y = %d\n",tete->x,tete->y);
                printf("temp[%d] POS : x = %d, y = %d\n",i,temp->x,temp->y);
            return 1;
        }
        temp = temp->suiv;
        i++;
    }
    return 0;
}

int alive(snake * tete){
    if(tete->x >= 58 || tete->x <= 2 || tete->y >= 27 || tete->y <= 2){
        return 0;
    }
    else{
    if(colission(tete)){
        return 0;
    }
    else{
    return 1;
    }
    }
}
void saveGame(){
    char name[50];
    printf("Donner votre nom : ");
    scanf(" %s",name);
    fp = fopen("scoreboard","a");
    fprintf(fp, "%s : %d\n", name, score);
    fclose(fp);
    printf("Game saved successfully.");
}
void LeaderBoard(){
    char ch;
    system("cls");
    red();
    printf("LISTE DES SCORES\n");
    fp = fopen("scoreboard","r");
    if(fp==NULL){
      printf("Il n'ya pas de liste des scores.");
      return;
    }
    while((ch = fgetc(fp)) != EOF)
      printf("%c", ch);
}

void gameOver(){
    int c;
    system("cls");
    Pos(25,5);
    red();
    printf("Game ");
    yellow();
    printf("over!\n ");
    reset();
    printf("SCORE : %d\n",score);

    printf("Wanna try again ? press 1 or press anything else to continue.\n");
    scanf("%d",&c);
    if(c==1){
        startGame();
    }
    saveGame();
    LeaderBoard();
}

void generateFood(){
    foodX = (rand()+2) %56 ;
    foodY = (rand()+2) %26 ;
}

void drawFood(){
    Pos(foodX,foodY);
    printf("*");
}
int SnakeAte(snake * tete){
    if(tete->x == foodX && tete->y == foodY){
        return 1;
    }
    else{
        return 0;
    }
}
void addTail(snake * tete){
    snake * temp = tete;
    while(temp->suiv!=NULL){
        temp=temp->suiv;
    }
    snake * newElement = (snake *)malloc(sizeof(snake));
    newElement->suiv = NULL;
    switch(status){
        case U : newElement->y = temp->y - 1; newElement->x = temp->x;
        break;
        case D : newElement->y = temp->y + 1; newElement->x = temp->x;
        break;
        case R : newElement->x = temp->x + 1; newElement->y = temp->y;
        break;
        case L : newElement->x  = temp->x - 1; newElement->y = temp->y;
    }
    temp->suiv = newElement;
}

void startGame(){
    int c;
    status = 4;
    generateFood();
    drawFood();
   snake * initSnake = createSnake();
   afficheSnake(initSnake);

   while(alive(initSnake)){
        moveSnake(initSnake, status);
        if(SnakeAte(initSnake)){
            Beep(750, 10);
            score++;
            generateFood();
            addTail(initSnake);

        }
    }
      gameOver();
   }


int main(){
    srand(time(0));
    drawMap();
    startGame();
    return 0;
}
