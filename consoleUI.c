// [consoleUI.c] file

#include <stdio.h> // for printing and reading files
#include <string.h> // string functions
#include <stdlib.h> // for parsing integers from user input

#include "consoleUI.h" // declare "consoleUI" and "game" variables/methods

// print the program banner the first time program runs
void PrintTitle(void) 
{
    printf("Bit Board Checkers [Two-Player Edition]\n\n");
}

// display the main menu options and menu choice prompt
void DisplayMenu(void) 
{
    printf("\n------------ MAIN MENU ------------\n");
    printf("1 - Display Current Board\n");
    printf("2 - Make A Move (enter FROM/TO positions)\n");
    printf("3 - Save Game\n");
    printf("4 - Load Game\n");
    printf("5 - Bit Operations Demo (Phase 1 - Test Functions)\n");
    printf("6 - How To Play\n");
    printf("7 - New Game (Reset Board)\n");
    printf("8 - Exit\n");
    printf("-----------------------------------\n");
    printf("Enter option number: ");
}

// read an integer choice from user, from stdin to pointer out
// returns 1 for valid input and 0 for invalid input
int UserInt(int* out) 
{
    char line[128]; // buffer to hold user input line
    char *endPointer = NULL; // pointer for strtol to indicate where parsing stopped
    long value = 0; // temporary holder for parsed long integer

    // read a line of input from the user, store in "line", 
    // if fgets fails (NULL), return 0 for invalid input
    if (fgets(line, sizeof(line), stdin) == NULL) 
    {
        return 0;
    }

    // parse the integer from the line
    // strtol will skip leading whitespace, parse until non-digit
    // if no digits found, endPointer will be same as line start
    {
        int i = 0; // index for skipping whitespace

        // skip leading whitespace
        while (line[i] == ' ' || line[i] == '\t') { i++; }
        
        // qualifier: if line is empty or only whitespace, invalid input
        if (line[i] == '\n' || line[i] == '\0') { return 0; }

        // convert substring starting at line[i] to long
        // base 10 conversion
        value = strtol(&line[i], &endPointer, 10);

        // qualifier: if no digits were found, invalid input
        if (endPointer == &line[i]) { return 0; }
    }
    *out = (int)value; // store the parsed integer in the output pointer
    return 1; // indicate successful parsing
}

// prompt the user for a specific action on the board
// interpret the user action and respond accordingly to their action
// valid responses (0-63) and -1 to cancel out of move (prevent "softlock" of game)
int PromptPosition(const char* prompt, int* outPosition) 
{
    int position = -1; // local holder for user input

    // loop until a valid position is entered or user cancels with -1
    while (1) 
    {
        printf("%s", prompt); // print the prompt message

        // qualifier: read user input, if invalid data type, reprompt
        if (!UserInt(&position)) 
        {
            printf("Invalid data type. Please enter an integer 0-63.\n");
            continue;
        }

        // qualifier: if user enters -1, cancel the action
        if (position == -1) 
        {
            *outPosition = -1; // store the cancel to output pointer
            return 1; // indicate user cancelled
        }
        // qualifier: if position is out of range, reprompt
        if (position < 0 || position > 63) 
        {
            printf("Out of range. Please enter 0-63.\n");
            continue;
        }

        // qualifier: if position is not a valid dark square, reprompt
        if (!IsValidDarkSquare(position)) 
        {
            printf("That is not a playable \"#\" dark square! Try another spot.\n");
            continue;
        }
        *outPosition = position; // store valid position to output pointer
        return 1; // indicate successful input
    }
}

// prints the instructions on how to play
void PrintInstructions(void) 
{
    printf("\n=== How to Play Bit Board Checkers Edition (Simplified Ver.) ===\n");
    printf("- Close to playing like regular checkers!\n\n");
    printf("- The Board is 8 x 8 and is displayed using ASCII implementations.\n");
    printf("    - \"_\" for light squares \n");
    printf("    - \"#\" for empty dark squares\n");
    printf("    - \"r/b\" men representations for Red and Black player pieces\n");
    printf("    - \"R/B\" king representations for Red and Black player pieces\n");
    printf("    * 12 pieces for each player!\n\n");
    printf("- Movement:\n");
    printf("    - Pieces are only allowed to move along the \"#\" dark squares. Diagonally\n");
    printf("    - Players enter a valid positions (0-63) on the board. Players will state which\n");
    printf("      piece they would like to move FROM -> TO (Ex: MOVE \"b\" on index 40 -> TO index 33)\n");
    printf("    * A (numbered) board displaying the exact indexes (0-63) of the ASCII board is provided\n");
    printf("      for convenience/reference to interpret where to move!\n\n");
    printf("    - Player 1 (Red) moves first! \"r\" men move DOWN the board (to larger indexes)\n");
    printf("    - Player 2 (Black) moves next! \"b\" men move UP the board (to smaller indexes).\n\n");
    printf("    - Simple movement: One step diagonally onto an empty \"#\" dark square.\n");
    printf("      \"r\" and \"b\" move towards their opposite direction\n");
    printf("    - Capture: Jump diagonally over an adjacent opponent piece onto an empty dark square.\n");
    printf("      Make sure to input the number you're jumping to, which is over the opposing player piece.\n");
    printf("      (Ex: \"b\" initially on (FROM) index 40, will jump TO index 26, capturing the \"r\" piece on index 33)\n");
    printf("      * Normally, men and kings can perform multiple captures, but in this version,\n");
    printf("        capture only works for 1 piece (single-capture only), for men and kings.\n\n");
    printf("    - King Promotion: A player that reaches the far edge is promoted to KING (\"r\" -> \"R\" or \"b\" -> \"B\")\n");
    printf("      and can move both ways.\n\n");
    printf("- Win Condition: When all opposing player pieces are captured OR a player has no legal moves left.\n");
    printf("- Save and Load functions use a simple text file containing the game state. Enter the string name of\n");
    printf("  your save file when you save. When loading, type the save file exactly as typed.\n");
    printf("======================================================\n");
}


// prints which player turn it is based on
// the "current_turn" flagger in GameState structure
void PrintWhoseTurn(const GameState* game) 
{
    // qualifier: if current_turn is 1, Player 1 (Red) turn
    if (game->current_turn == 1) 
    {
        printf("It is Player 1 (Red) turn.\n");
    } 
    
    // otherwise, Player 2 (Black) turn
    else 
    {
        printf("It is Player 2 (Black) turn.\n");
    }
}

// show a numbered 8x8 reference board, printing indexes 0-63
// is displayed along side the ASCII board interpretation for each player turn
// helps to reference what index you're piece is on and where to move
void PrintReferenceBoard(void) 
{
    int row = 0; // row index for printing

    printf("===============================\n");
    printf("{Bit Board Checkers Reference}\n");
    printf("Use this board to refer to your positions\n\n");
    printf("\tColumns 0 - 7 \n");

    // print each row with its corresponding index
    for (row = 0; row < 8; row++) 
    {
        int col = 0; // column index for printing
        int base = row * 8; // base index for the row, used to calculate each column index
        printf("Row %d: ", row);

        // print each column index
        for (col = 0; col < 8; col++) 
        {
            int index = base + col; // calculate the index for this column
            
            // qualifier: column 0 prints without leading space
            if (col == 0) 
            {
                // index formatted to width 2 for alignment
                printf("%2d", index);
            }

            // qualifier: column 1-7 prints with leading space
            else
            {
                // index formatted to width 2 for alignment
                printf(" %2d", index);
            }
        }
        printf("\n");
    }
    printf("===============================\n\n");
}

// print “Player 1 (Red)” or “Player 2 (Black)”
void PrintPlayerText(int player) 
{
    // qualifier: if player is 1, print Player 1 (Red)
    if (player == 1) 
    { 
        printf("Player 1 (Red)"); 
    }

    // otherwise, print Player 2 (Black)
    else 
    { 
        printf("Player 2 (Black)"); 
    }
}

// print which player "moved FROM __ TO __"
// displays after user enters their valid stationary piece (FROM)
// and enters a unoccupied and fair dark square (#) (TO)
void PrintMoveText(int player, int fromPosition, int toPosition) 
{
    PrintPlayerText(player); // print the current player
    // indicate player movement using FROM and TO positions indexes
    printf(" Moved FROM %d TO %d.\n", fromPosition, toPosition);
}