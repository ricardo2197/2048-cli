#ifndef GAME_LOGIC_H_INCLUDED
#define GAME_LOGIC_H_INCLUDED

#include <curses.h>
#include <menu.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/select.h>

#define H 3
#define L 7
#define TIME_LEFT 60

int gameIsOn;           //0 - ended 1 - playing 2 - paused
unsigned int **BOARD;
unsigned int **UNDO_BOARD;
int score;
int isMoveValid;
int timeLeft;
int undoScore;
int N;
int ok;

int generateRandomNumber();
void printBoard();
int fillEmptyField(int a, int getOnlySpaces);
void moveVertically();
void rotateBoard();
void initColors();
int getColorCode(int n);
void isGameOver();
void newGame();
void pauseGame();
void resumeGame();
void copyBoard(int reverse);
void getEfficientMove();
void undoMove();
void allocBoard(int a);
void freeBoard();


#endif // GAME_LOGIC_H_INCLUDED
