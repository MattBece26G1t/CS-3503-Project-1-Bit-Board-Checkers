// [saveload.h] header file
// function declarations for "saveload.c"
// implemented in "main.c"

// { Phase 2 - Checkers Game Implementations } //
// "2.11 Implementation Flexibility" - Extra Features: Save/Load game

/*
    Utilizes a text file to save and load the current game state

    The SaveGame function writes 5 seperate lines to a text file depicting
    the current game state using "GameState" structure features from "game.c"
    Each line represents the

        // 64-bit bit board for pieces
        1: player1_men
        2: player1_kings
        3: player2_men
        4: player2_kings

        // integer returning 1 for Red and 2 for Black
        5: current_turn 

        lines 1-4 are bitboards, one bit per square (0-63)
        if a bit is 1 at index i, that piece type occupies square i
        formats used %llu for bitboard pieces and %d for "current_turn"

    The LoadGame function reads those same 5 lines back in order
    each line is validated, if any of the lines are invalid or empty, 
    the file does not load and game status remains the same.
    On success, sets the current game status (bitboards and current turns)
*/

#ifndef SAVELOAD_H
#define SAVELOAD_H

#include "game.h" // for GameState (bitboard pieces and current_turn)

// save the current game state to a text file named "filename"
// writes 5 lines to "filename": p1_men, p1_kings, p2_men, p2_kings, current_turn
// returns 1 if saved successfully, 
// return 0 if save failed (open or write error)
int SaveGame(const char* filename, const GameState* game);

// load a game state from a text file named "filename", with the same 5-line format
// validates each line and on success sets the bitboards and "current_turn"
// returns 1 if loaded successfully, 
// 0 if file missing or invalid
int LoadGame(const char* filename, GameState* game);

#endif