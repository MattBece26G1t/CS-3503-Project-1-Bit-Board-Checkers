// [game.h] header file
// function declarations for "game.c"
// implemented in "consoleUI.h" / "saveload.h" / "main.c"

#ifndef GAME_H
#define GAME_H

#include <stdint.h> // for 64-bit unsigned integers (bitboard pieces)

// { Phase 2 - Checkers Game Implementation } //
// adapts the skeleton ideas provided in 
// "2.12 Example Structures and Approaches" & Helper Function hints
// "2.13 Using Bitboard Creatively"

// additionally, "2.8 Required Game Features" and "2.11 Implementation Flexibility"
// are adapted in this file

/*
    This header file lays out mostly all of the bit board checkers
    game functionalities, with additional helpers and UI display

    Provides board set up and game's main structures, 
    move/capture logic, turn management, and win conditions 
*/

// "Approach 2: More Detailed" - structure to track the current game state //
typedef struct 
{
    // player 1 are red pieces
    unsigned long long player1_men; // "r" pieces
    unsigned long long player1_kings; // "R" pieces

    // player 2 are black pieces 
    unsigned long long player2_men; // "b" pieces
    unsigned long long player2_kings; // "B" pieces

    // present whose current turn by either 1 or 2 
    // Player 1 Red = 1 OR Player 2 Black = 2
    int current_turn; 
} GameState; // structure used for all gameplay operations

// Initialize Board and Display //

// initialize the board when new game, pieces assume starting positions,
// "current_turn" set to 1 (Player 1 Red) at the start
void SetBoard(GameState* game);

// print an ASCII representation of the board
// 8 x 8 size, "#" dark squares & "_" white squares
// Player 1 Red Pieces: "r" men & "R" kings
// Player 2 Black Pieces: "b" men & "B" kings
void PrintBoardPretty(const GameState* game);

// Inspired Helper Functions //

// convert board coordinates (row, col) to a bit position (0-63)
// return -1 if coordinates are out of bounds
int ConvertRowColToIndex(int row, int col);

// check if given board position is a valid playable dark square ("#")
// return 1 when valid, otherwise 0 when invalid
int IsValidDarkSquare(int position);

// check if any player piece is currently occupying a board position
// return 1 if occupied, otherwise 0 if unoccupied
int IsOccupiedSpace(const GameState* game, int position);

// check if the piece at "position" belongs to a current player
// used to ensure that the player will be moving THEIR piece
// return 1 if piece belongs to player for their turn
// return 0 if piece belongs to opponent or empty square
int PieceBelongToPlayer(const GameState* game, int position);

// Player Move and Turn Functions //

// attempts to read a move or capture, for "FROM" to "TO" position
// return 1 if valid move and proceed with the action, otherwise 0
int TryMove(GameState* game, int fromPosition, int toPosition);

// check which player’s turn it currently is
// returns 1 if Player 1’s turn
int IsRedPlayer1Turn(const GameState* game);

// check which player’s turn it currently is
// returns 1 if Player 2’s turn
int IsBlackPlayer2Turn(const GameState* game);

// Win Conditions //

// checks if current player has any legal moves available
// returns 1 if at least one move exists, 
// return 0 if blocked
int CheckLegalMoves(const GameState* game);

// checks for a winner based on captured pieces
// returns 1 if Player 1 (Red) wins, 
// return 2 if Player 2 (Black) wins, 
// return 0 if no winner yet
int CheckWinner(const GameState* game);

#endif