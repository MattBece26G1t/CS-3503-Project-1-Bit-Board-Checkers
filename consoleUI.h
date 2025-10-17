// [consoleUI.h] header file
// function declerations for "consoleUI.c"
// implement in "main.c"

#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "game.h" // implement GameState strucutre

// { Phase 2 -  Checkers Game Implementation } //
// Implement additional UI to support the required game functions

/*
    The following are for user interface related functionalities.
    These functionalities help to keep a organized console display.
    Prints the title, menu, boards, instructions, and player action results. 
    Also helps to read user input (piece movement) by pritning a prompt
*/

// Print Title and Menu //

// print the program banner the first time program runs
void PrintTitle(void);

// display the main menu options and menu choice prompt
// loops every time after an option, until exit sentinel
void DisplayMenu(void);

// Read User Input //

// reads an integer choice from the user, 
// returns 1 for valid input and 0 for invalid input
// used for menu choices or player movement
int UserInt(int* out);

// prompt the user for a specific action on the board
// interpret the user action and respond accordingly to their action
// valid responses (0-63) and -1 to cancel out of move (prevent "softlock" of game)
int PromptPosition(const char* prompt, int* outPosition);

// Print Additional Displays //

// prints the instructions on how to play,
// when user selects option (case) 6 on the main menu
void PrintInstructions(void);

// prints which player turn it is based on
// the "current_turn" flagger in GameState structure
void PrintWhoseTurn(const GameState* game);

// show a numbered 8x8 reference board, printing indexes 0-63
// is displayed along side the ASCII board interpretation for each player turn
// helps to reference what index you're piece is on and where to move
void PrintReferenceBoard(void);

// print “Player 1 (Red)” or “Player 2 (Black)”
// if player == 1 (player 1) (otherwise, player 2 would be if player == 2)
void PrintPlayerText(int player);

// print which player "moved FROM __ TO __"
// displays after user enters their valid stationary piece (FROM)
// and enters a unoccupied and fair dark square (#) (TO)
void PrintMoveText(int player, int fromPosition, int toPosition);

#endif