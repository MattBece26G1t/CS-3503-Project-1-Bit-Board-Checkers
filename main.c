// [main.c] file
// run program here!

/*
Bash inserted into the terminal:

This program was built in Windows 11 using the IDE Visual Studio.

Initially, the way I was able to get this program to run was by opening "MSYS2 MINGW64",
and navigating to the project directory using the command 
(This is just an example, but you would paste this trajectory into terminal):

   $ cd /c/Users/name/path/path/BitBoardCheckersFinal

Then pasted this into the terminal to get the program to build and run

    make clean
    make
    ./bitboardcheckers.exe

The program output appears in the "MSYS2 MINGW64" terminal, but additionally
It can also be run directly from the Visual Studio terminal 
by entering in this, anytime you want to run the program:

    make clean
    make    
    ./bitboardcheckers.exe

After the initial setup, there is no further need to access or configure MSYS2 MINGW64 separately.
*/

#include <stdio.h> // for printing and reading files
#include <string.h> // string functions for input handling
#include <stdint.h> // for uint32_t (bitboard pieces)

#include "bitoperations.h" // bit manipulation functions (Phase 1 - Test Functions)
#include "game.h" // GameState structure and game functions
#include "consoleUI.h" // console UI functions
#include "saveload.h" // save/load functions

// method for switching turns between players using "current_turn" flagger
static void SwitchTurn(GameState* game) 
{
    // qualifier: if current turn is Player 1 (Red) then switch to Player 2 (Black)
    if (game->current_turn == 1) 
    {
        game->current_turn = 2;
    }

    // otherwise, switch to Player 1 (Red), if current turn is Player 2 (Black)
    else 
    {
        game->current_turn = 1;
    }
}

// Bit Operations Demo (Phase 1 - Test Functions) menu and options
// allows user to test each of the bit manipulation functions
static void BitOpsDemoPhase1(void) 
{
    unsigned int value = 0u; // initial value to manipulate
    int bitopRunning = 0; // flag to control demo loop

    printf("\n[Bit Operations Demo (Phase 1 - Test Functions)]\n");
    printf("Starting value = 0\n");

    // demo loop, continues until user chooses to sendinel to exit
    while (!bitopRunning) 
    {
        int option = -1; // user menu option
        printf("\n1 - SetBit\n");
        printf("2 - ClearBit\n");
        printf("3 - ToggleBit\n");
        printf("4 - GetBit\n");
        printf("5 - CountBits\n");
        printf("6 - ShiftLeft\n");
        printf("7 - ShiftRight\n");
        printf("8 - PrintBinary\n");
        printf("9 - PrintHex\n");
        printf("0 - Exit demo\n");
        printf("\n");
        printf("Choice: ");

        // qualifier: if UserInt fails (invalid input), print error and re-prompt
        // using & to get the address of option variable and certain positions for bit functions
        if (!UserInt(&option)) 
        {
            printf("\n");
            printf("Invalid menu option. Try again (0-9).\n");
            continue;
        }

        // process the user menu option
        switch (option) 
        {
            // sentinel, exit demo
            case 0:
                bitopRunning = 1;
                break;

            // 1 - SetBit    
            case 1: 
            {
                int position = 0; // initialize bit position to set
                printf("\n");
                // print current value and binary representation before setting bit
                printf("Value before SetBit: %u\n", value);
                PrintBinary(value); 

                printf("\n");
                printf("Enter bit position (0-31): ");

                // qualifier: if UserInt fails (invalid input) or out of range, print error and break
                if (!UserInt(&position) || position < 0 || position > 31) 
                { 
                    printf("Invalid position.\n"); 
                    break; 
                }

                // set the bit at the specified position
                value = SetBit(value, position);
                // print new value and binary representation after setting bit
                printf("Value after SetBit:  %u\n", value);
                PrintBinary(value);
                break;
            }

            // 2 - ClearBit
            case 2: 
            {
                int position = 0; // initialize bit position to clear
                printf("\n");
                // print current value and binary representation before clearing bit
                printf("Value before ClearBit: %u\n", value);
                PrintBinary(value); 

                printf("\n");
                printf("Enter bit position (0-31): ");

                // qualifier: if UserInt fails (invalid input) or out of range, print error and break
                if (!UserInt(&position) || position < 0 || position > 31) 
                { 
                    printf("Invalid position.\n"); 
                    break;                 
                }

                // clear the bit at the specified position
                value = ClearBit(value, position);
                // print new value and binary representation after clearing bit
                printf("Value after ClearBit:  %u\n", value);
                PrintBinary(value);
                break;
            }

            // 3 - ToggleBit
            case 3: 
            {
                printf("\n");
                int position = 0; // initialize bit position to toggle
                // print current value and binary representation before toggling bit
                printf("Value before ToggleBit: %u\n", value);
                PrintBinary(value); 

                printf("\n");
                printf("Enter bit position (0-31): ");
                
                // qualifier: if UserInt fails (invalid input) or out of range, print error and break
                if (!UserInt(&position) || position < 0 || position > 31) 
                { 
                    printf("Invalid position.\n"); 
                    break; 
                }

                // toggle the bit at the specified position
                value = ToggleBit(value, position);
                // print new value and binary representation after toggling bit
                printf("Value after ToggleBit:  %u\n", value);
                PrintBinary(value); 
                break;
            }

            // 4 - GetBit
            case 4: 
            {
                printf("\n");
                int position = 0; // initialize bit position to get
                // print current value and binary representation before getting bit
                printf("Retrieving Bit...\n");
                // print current value and binary representation
                printf("Current value: %u\n", value);
                PrintBinary(value); 

                printf("\n");
                printf("Enter bit position (0-31): ");

                // qualifier: if UserInt fails (invalid input) or out of range, print error and break
                if (!UserInt(&position) || position < 0 || position > 31) 
                { 
                    printf("Invalid position.\n"); 
                    break; 
                }

                // retrieve the bit at the specified position and calling GetBit 
                // with given value and position
                printf("Bit %d = %d\n", position, GetBit(value, position));
                break;
            }

            // 5 - CountBits
            case 5: 
            {
                int count = CountBits(value); // count number of bits set in value

                // print the result
                printf("\n");
                printf("CountBits of current value (%u) = %d\n", value, count);
                break;
            }

            // 6 - ShiftLeft
            case 6: 
            {
                int userShift = 0; // initialize number of positions to shift
                printf("\n");
                printf("Shifting Left\n");

                // print current value and binary representation before shifting
                printf("Current value: %u\n", value); 
                PrintBinary(value); // print binary representation

                printf("\n");
                printf("Enter number of positions to shift left: ");

                // qualifier: if UserInt fails (invalid input) or negative, print error and break
                if (!UserInt(&userShift) || userShift < 0) 
                { 
                    printf("Invalid number.\n"); 
                    break; 
                }

                // value calls ShiftLeft with given value and userShift
                value = ShiftLeft(value, userShift);

                // print the result
                printf("Result: %u\n", value);
                PrintBinary(value); // print binary representation after shifting
                break;
            }

            // 7 - ShiftRight    
            case 7:
            {
                int userShift = 0; // initialize number of positions to shift
                printf("\n");
                printf("Shifting Right\n");

                // print current value and binary representation before shifting
                printf("Current value: %u\n", value);
                PrintBinary(value); // print binary representation

                printf("\n");
                printf("Enter number of positions to shift right: ");

                // qualifier: if UserInt fails (invalid input) or negative, print error and break
                if (!UserInt(&userShift) || userShift < 0) 
                { 
                    printf("Invalid number.\n"); 
                    break; 
                }

                // value calls ShiftRight with given value and userShift
                value = ShiftRight(value, userShift);
                // print the result
                printf("Result: %u\n", value);
                PrintBinary(value); // print binary representation after shifting
                break;
            }

            // 8 - PrintBinary
            case 8:
                printf("\n");
                PrintBinary(value); // print binary representation of current value
                break;

            // 9 - PrintHex
            case 9:
                printf("\n");
                PrintHex(value); // print hexadecimal representation of current value
                break;
               
            // unknown option, print error message
            default:
                printf("\n");
                printf("Unknown option. Please choose 0-9.\n");
                break;
        }
    }
    printf("\n");
    printf("Returning to main menu...\n");
}

// method to validate the "fromPosition" entered by user for moving a piece
static int ValidateFromMovement(const GameState* game, int fromPosition) 
{
    // qualifier: if fromPosition is not a valid dark square, print error and return 0
    if (!IsValidDarkSquare(fromPosition)) 
    {
        printf("That is not a playable \"#\" dark square! Try another spot.\n");
        return 0;
    }

    // qualifier: FROM must have a piece belonging to the current player
    // if not, print error and return 0
    // otherwise, return 1 for valid FROM position
    if (!PieceBelongToPlayer(game, fromPosition)) 
    {
        printf("Invalid move. Please try again.\n");
        return 0; // not current player piece
    }
    return 1; // piece belongs to current player
}

// method for running the entire program (entry point), including everything together
int main(void) 
{
    GameState game; // holds all game state information
    int mainRunning = 1; // flag to control main game loop

    SetBoard(&game); // initialize/refresh the board for a new game
    PrintTitle(); // print game title
    PrintBoardPretty(&game); // print the intial board

    // main menu and game loop, continues until user chooses to exit
    while (mainRunning) 
    {
        int choice = 0; // initialize user menu choice
        DisplayMenu(); // display the main menu

        // qualifier: if UserInt invalid menu choice, print error and re-prompt
        if (!UserInt(&choice)) 
        {
            printf("\n");
            printf("Invalid menu choice. Please enter an integer.\n");
            continue;
        }
        printf("\n");

        // process the valid menu choice
        switch (choice) 
        {
            // 1 - Print Board
            case 1:
                PrintBoardPretty(&game); // print the current game board
                break;

            // 2 - Make A Move
            case 2: 
            {
                int fromPosition = -1; // initialize fromPosition, chosen square index
                int toPosition = -1; // initialize toPosition, chosen destination index

                // qualifier: print whose turn it is based on "current_turn" flagger
                if (game.current_turn == 1) 
                {
                    printf("[Player 1 (Red) turn]\n");
                }
                else                        
                {
                    printf("[Player 2 (Black) turn]\n");
                }

                PrintBoardPretty(&game); // print the current game board during each turn
                PrintReferenceBoard(); // print the reference board for index reference

                printf("Tip: If stuck, enter -1 to cancel and return to the main menu.\n\n");

                // loop until a valid FROM position is entered or user cancels with -1
                while (1) 
                {
                    // prompt user for FROM position
                    if (!PromptPosition("Enter FROM position (0-63, \"#\" dark square): ", &fromPosition)) 
                    {
                        continue; // reprompt on invalid input
                    }
                    // qualifier: if user cancels with -1, break from the loop
                    if (fromPosition == -1) 
                    {
                        break; // user cancelled on FROM prompt
                    }
                    // qualifier: check if FROM position belongs to current player and is valid # square
                    if (!ValidateFromMovement(&game, fromPosition)) 
                    {
                        continue; // reprompt on invalid FROM position
                    }
                    break; // move on to TO position prompt
                }

                // if user cancelled, display message and return to main menu
                if (fromPosition == -1) 
                {
                    printf("Move cancelled. Returning to main menu.\n");
                    break; // break the loop to return to main menu
                }

                // loop until a valid TO position is entered or user cancels with -1
                while (1) 
                {
                    // prompt user for TO position
                    if (!PromptPosition("Enter TO position (0-63, \"#\" dark square): ", &toPosition)) 
                    {
                        continue; // reprompt on invalid input
                    }
                    // qualifier: if user cancels with -1, break from the loop
                    if (toPosition == -1) 
                    {
                        printf("Move cancelled. Returning to main menu.\n");
                        break; // user cancelled on TO prompt
                    }

                    // qualifier: check if TO position is valid and empty
                    if (!ValidateToMovement(&game, fromPosition, toPosition)) 
                    {
                        continue; // reprompt on invalid TO position
                    }

                    // attempt to make the move, once both FROM and TO are valid
                    if (TryMove(&game, fromPosition, toPosition)) 
                    {
                        // print which player moved and from/to positions
                        PrintMoveText(game.current_turn, fromPosition, toPosition);

                        // print the updated board after the move
                        PrintBoardPretty(&game);

                        // check for a winner based on captured pieces
                        // if a winner is found, announce and prompt for new game or exit
                        {
                            int win = CheckWinner(&game); // win assigns the result of CheckWinner

                            // check if a player has won
                            if (win == 1 || win == 2) 
                            {
                                PrintPlayerText(win); // print the winning player
                                printf(" wins!\n\n");

                                // prompt for new game or exit
                                {
                                    int playAgain = 0; // initialize play again choice

                                    printf("Would you like to play again (New Game - Reset Board) [Enter 1]?\n");
                                    printf("Or Exit [Enter 2]?\n\n");
                                    printf("Enter Option: ");

                                    // get user input for play again option
                                    if (!UserInt(&playAgain)) { playAgain = 2; }
                                    printf("\n");

                                    // handle play again or exit
                                    if (playAgain == 1) 
                                    {
                                        SetBoard(&game); // reset the board and game state
                                        PrintBoardPretty(&game); // print the new board
                                    }
                                    // if user chooses to exit
                                    else 
                                    {
                                        printf("Goodbye!\n");
                                        mainRunning = 0; // stop the main loop
                                    }
                                }
                                break;
                            }
                        }

                        // check if the next player has any legal moves available
                        // if blocked, announce the winner and prompt for new game or exit
                        {
                            GameState nextState = game; // copy current game state
                            SwitchTurn(&nextState); // simulate other player's turn

                            // check if the next player has any legal moves available 
                            if (!CheckLegalMoves(&nextState)) 
                            {

                                // identify loser, the side to move in nextState
                                int loser = nextState.current_turn;
                                int winner = 0; 

                                // if the loser is player 1, then the winner is player 2
                                if (loser == 1) { winner = 2; }
                        
                                // otherwise, the loser is player 2, so the winner is player 1
                                else { winner = 1; }
                            
                                PrintPlayerText(loser); // print the losing player
                                printf(" has no legal moves. ");
                                PrintPlayerText(winner); // print the winning player
                                printf(" wins!\n\n");

                                // prompt for new game or exit
                                {
                                    int playAgain = 0; // initialize play again choice
                                    printf("Would you like to play again (New Game - Reset Board) [Enter 1]?\n");
                                    printf("Or Exit [Enter 2]?\n\n");

                                    printf("Enter Option: ");

                                    // get user input for play again option
                                    if (!UserInt(&playAgain)) { playAgain = 2; }
                                    printf("\n");

                                    // handle play again or exit
                                    if (playAgain == 1) 
                                    {
                                        SetBoard(&game); // reset the board and game state
                                        PrintBoardPretty(&game); // print the new board
                                    } 
                                    // program terminates
                                    else 
                                    {
                                        printf("Goodbye!\n");
                                        mainRunning = 0; // stop the main loop
                                    }
                                }
                                break;
                            }
                        }

                        SwitchTurn(&game); // switch to the other player's turn
                        break;
                    } 
                    
                    // if TryMove failed, print error and reprompt for TO position
                    else 
                    {
                        printf("Invalid move. Please try again.\n");
                    }
                }
                break;
            }

            // 3 - Save Game
            case 3:
            {
                char filename[256]; // initialize filename buffer
                printf("Enter a save file name (example: game1): ");
                
                // retrieve the filename from user input, the size (max buffer) and null terminator
                if (fgets(filename, sizeof(filename), stdin) == NULL) 
                { 
                    printf("Read Error!\n"); 
                    break; 
                }
                // trim newline character from fgets input if present
                {
                    size_t length = strlen(filename); // get the current length of the input string  

                    // qualifier: if last character is newline, replace with null terminator
                    if (length > 0 && filename[length - 1] == '\n') 
                    {
                        filename[length - 1] = '\0';
                    }
                }

                // qualifier: if filename is empty after trimming, print error and break
                if (filename[0] == '\0') 
                { 
                    printf("Empty file name not allowed!\n"); 
                    break; 
                }

                SaveGame(filename, &game); // save the game state to the specified file
                break;
            }

            // 4 - Load Game
            case 4: 
            {
                char filename[256]; // initialize filename buffer
                printf("Enter save file name to load (enter exactly as typed!): ");

                // retrieve the filename from user input, the size (max buffer) and null terminator
                if (fgets(filename, sizeof(filename), stdin) == NULL) 
                { 
                    printf("Read Error!\n"); 
                    break;
                }
                // trim newline character from fgets input if present
                {
                    size_t length = strlen(filename); // get the current length of the input string

                    // qualifier: if last character is newline, replace with null terminator
                    if (length > 0 && filename[length - 1] == '\n') 
                    {
                        filename[length - 1] = '\0';
                    }
                }

                // qualifier: try to load the game, if it fails print error
                if (!LoadGame(filename, &game)) 
                {
                    printf("Load failed. Enter valid file name exactly.\n");
                }
                // otherwise, print the loaded board
                else 
                {
                    PrintBoardPretty(&game);
                }
                break;
            }

            // 5 - Bit Operations Demo (Phase 1 - Test Functions)
            case 5:
                BitOpsDemoPhase1(); // opens the Bit Operations Demo
                break;

            // 6 - Print Instructions
            case 6:
                PrintInstructions(); // print how to play instructions
                break;

            // 7 - New Game (Reset Board)   
            case 7:
                SetBoard(&game); // reset the game board
                PrintBoardPretty(&game); // print the new game board
                break;

            // 8 - Quit Game
            case 8:
                mainRunning = 0; // stop the main loop
                printf("Goodbye!\n");
                break;

            // unknown option, print error message
            default:
                printf("Invalid option. Please enter a number from the menu (1-8).\n");
                break;
        }
    }
    return 0; // normal program termination
}