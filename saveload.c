// [saveload.c] file

#include <stdio.h> // for printing and reading files

#include "saveload.h" // declare "saveload" and "game" variables/methods

// save the current game state to a text file
int SaveGame(const char* filename, const GameState* game) 
{
    // open file for writing, overwrite if exists
    FILE* file = fopen(filename, "w");
    
    // qualifier: could not open file due to file pointer/path errors or protected file 
    if (file == NULL) 
    {
        printf("Could not open file for writing: %s\n", filename);
        return 0; // unsuccessful save
    }

    // write the 5 lines in text file, depicting the game state
    // 1: player1_men       2: player1_kings        3: player2_men
    // 4: player2_kings     5: current_turn

    // "game" is a pointer to GameState, so "game->" to any of the fields from GameState
    // structure is accessed. The variables are casted to (unsigned long long) to match 
    // the %llu format for the 64-bit bitboards
    fprintf(file, "%llu\n", (unsigned long long)game->player1_men);
    fprintf(file, "%llu\n", (unsigned long long)game->player1_kings);
    fprintf(file, "%llu\n", (unsigned long long)game->player2_men);
    fprintf(file, "%llu\n", (unsigned long long)game->player2_kings);
    
    // "current_turn" is an int, so we use %d
    fprintf(file, "%d\n", game->current_turn);

    fclose(file); // close file after writing

    // confirma successful save to the user
    printf("Game saved to \"%s\".\n", filename);
    return 1; // successful save
}

// load a game state from a text file
int LoadGame(const char* filename, GameState* game) 
{
    // open file for reading
    FILE* file = fopen(filename, "r");

    // qualifier: could not open file, either does not exist or mis-type
    if (file == NULL) 
    { 
        printf("Could not open save file: %s\n", filename); 
        return 0; // unsuccessful load
    }

    // local holders representing the 4 bitboard pieces, storage for fscanf
    // assigned to 0ull (unsigned long long zero) as default for safety
    unsigned long long p1MBits=0ull; 
    unsigned long long p1KBits=0ull; 
    unsigned long long p2MBits=0ull; 
    unsigned long long p2KBits=0ull;

    // turn flagger and items read counter
    int savePlrTurn = 0; // holds which player's turn was saved (1 for Red, 2 for Black)
    int itemsRead = 0; // tracks how many items were successfully read by fscanf

    //  read 5 lines from the save file //
    // each line is validated, reading exactly 1 item per line
    // if any line is invalid the file does not load
    // fscanf takes the file pointer, format, and address of local variable to store in

    // line 1 - read player 1 mens
    itemsRead = fscanf(file, "%llu", &p1MBits); 
    // qualifier: if not exactly 1 item read, invalid file
    if (itemsRead != 1) 
    { 
        fclose(file); // close file for safety
        printf("Invalid save file (line 1).\n"); 
        return 0; // unsuccessful load
    }

    // line 2 - read player 1 kings
    // qualifier: if not exactly 1 item read, invalid file
    itemsRead = fscanf(file, "%llu", &p1KBits); 
    if (itemsRead != 1) 
    { 
        fclose(file); // close file for saftey
        printf("Invalid save file (line 2).\n"); 
        return 0; // unsuccessful load
    }

    // line 3 - read player 2 mens
    // qualifier: if not exactly 1 item read, invalid file
    itemsRead = fscanf(file, "%llu", &p2MBits); 
    if (itemsRead != 1) 
    {
        fclose(file); // close file for safety
        printf("Invalid save file (line 3).\n"); 
        return 0; // unsuccessful load
    }

    // line 4 - read player 2 kings
    // qualifier: if not exactly 1 item read, invalid file
    itemsRead = fscanf(file, "%llu", &p2KBits); 
    if (itemsRead != 1) 
    { 
        fclose(file); // close file for safety
        printf("Invalid save file (line 4).\n"); 
        return 0; // unsuccessful load
    }

    // line 5 - read current turn
    // qualifier: if not exactly 1 item read, invalid file
    itemsRead = fscanf(file, "%d", &savePlrTurn); 
    if (itemsRead != 1) 
    { 
        fclose(file); // close file for safety
        printf("Invalid save file (line 5).\n"); 
        return 0; // unsuccessful load
    }

    fclose(file); // close file after reading

    // all 5 lines read successfully and validated //
    // set the game state to the loaded values
    game->player1_men = p1MBits;
    game->player1_kings = p1KBits;
    game->player2_men = p2MBits;
    game->player2_kings = p2KBits;

    // set the current turn, only if valid (1 or 2), otherwise default to 1
    if (savePlrTurn == 1 || savePlrTurn == 2) 
    { 
        // pointer "game" access to "current_turn" field, assign loaded value
        game->current_turn = savePlrTurn; 
    }
    // invalid value, default to Player 1 (Red)
    else 
    { 
        game->current_turn = 1; 
    }

    // confirma successful load to the user
    printf("Game loaded from \"%s\".\n", filename);
    return 1; // successful load
}