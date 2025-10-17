// [game.c] file

#include <stdio.h> // for printing and reading files

#include "game.h" // declare game variables and functions

// method for building a bitboard mask of 
// all playable dark squares ("#") on an 8x8 board
static unsigned long long MaskDarkSq(void) 
{
    // initialize accumulator mask to 0
    // each encountered dark square will set one bit to 1
    unsigned long long mask = 0ull;
    int row = 0; // initialize row iterator (0-7)
    int col = 0; // intiialize column iterator (0-7)

    // traverse through the entire 8 x 8 grid
    for (row = 0; row < 8; row++) 
    {
        // inner loop traverses through columns
        for (col = 0; col < 8; col++) 
        {
            // qualifier: # sqaure if the result of row + col, is odd
            if (((row + col) % 2) == 1) 
            {
                // compute the index for that dark square
                int index = row * 8 + col;
                
                // set the bit at that index in the mask to 1
                mask = mask | (1ull << index);
            }
        }
    }

    // return the completed dark-square mask 
    // 1s for dark squares, 
    // 0s for light squares
    return mask;
}

// used for runtime acess to validate playable dark squares, set in SetBoard()
static unsigned long long validSquares = 0ull;

// method for promoting a piece to KING if the player piece reaches the far opposite side
// if it's player 1's move, promotes upon landing on row 7
// if it's player 2's move, promotes upon landing on row 0
static void Promote(GameState* game, int position) 
{
    int row = position / 8; // determine the row of the position

    // qualifier: check which player's turn it is, to decide which far row to promote 
    if (IsRedPlayer1Turn(game)) 
    {
        // qualifier: if player 1 piece reaches row 7, promote to KING
        if (row == 7) 
        {
            // create a mask for the position
            unsigned long long mask = (1ull << position);

            // qualifier: promote if the piece at position is player 1
            if ((game->player1_men & mask) != 0ull) 
            {
                game->player1_men &= ~mask; // remove the man from position
                game->player1_kings |=  mask; // promote to king
                printf("Player 1 (Red) piece promoted to KING at position %d.\n", position);
            }
        }
    } 
    
    // otherwise, player 2's turn
    else 
    {
        // qualifier: if player 2 piece reaches row 0, promote to KING
        if (row == 0) 
        {
            // create a mask for the position
            unsigned long long mask = (1ull << position);

            // qualifier: promote if the piece at position is player 2
            if ((game->player2_men & mask) != 0ull) 
            {
                game->player2_men &= ~mask; // remove the man from position
                game->player2_kings |=  mask; // promote to king
                printf("Player 2 (Black) piece promoted to KING at position %d.\n", position);
            }
        }
    }
}

// method to check if a diagonal move (non-capture) is legal
// takes in the game state, FROM and TO positions, and player information for direction
static int IsLegalMove(const GameState* game, int fromPosition, int toPosition, int isKing, int isPlayer) 
{
    (void)game;
    {
        // compute the row and column differences between FROM and TO positions
        int rowDifference = (toPosition / 8) - (fromPosition / 8);
        int colDifference = (toPosition % 8) - (fromPosition % 8);

        // qualifier for foward step: 
        // destination is one row ahead and one column diagonally
        // for player 1 (Red), moving "down" the board (increasing row index)
        // for player 2 (Black), moving "up" the board (decreasing row index)
        if (rowDifference == 1 && (colDifference == 1 || colDifference == -1)) 
        {
            // qualifier: if the piece is a KING, the move is legal
            if (isKing) { return 1; }
            // qualifier: if it's the player's turn, the move is legal
            if (isPlayer) { return 1; }
            return 0; // otherwise, illegal move for player 2
        }

        // qualifier for backward step:
        // destination is one row behind and one column diagonally
        // only KING pieces can move backward
        // for player 1 (Red), moving "up" the board (decreasing row index)
        // for player 2 (Black), moving "down" the board (increasing row index)
        if (rowDifference == -1 && (colDifference == 1 || colDifference == -1)) 
        {
            // qualifier: if the piece is a KING, the move is legal
            if (isKing) { return 1; }
            // qualifier: player 1 cannot move "up"
            if (isPlayer) { return 0; }
            return 1; // otherwise, player 2 men move toward decreasing row index
        }
        return 0; // any other move is illegal that isn't a diagonal step
    }
}

// method to check if a diagonal capture move is legal
// takes in the game state, FROM and TO positions, and player information for direction
static int IsLegalCapture(const GameState* game, int fromPosition, int toPosition, int isKing, int isPlayer, int* captured) 
{
    // compute the row and column differences between FROM and TO positions
    int rowDifference = (toPosition / 8) - (fromPosition / 8);
    int colDifference = (toPosition % 8) - (fromPosition % 8);

    // qualifier: capture move must be two squares away diagonally
    if (!((rowDifference == 2 && (colDifference == 2 || colDifference == -2)) || (rowDifference == -2 && (colDifference == 2 || colDifference == -2)))) 
    {
        return 0; // if not, illegal capture move
    }

    // direction qualifier for non-KING pieces
    // for Player 1 (Red), moving "down" the board (increasing row index)
    // for Player 2 (Black), moving "up" the board (decreasing row index)
    if (!isKing) 
    {
        // player 1 (Red)
        if (isPlayer) 
        { 
            // qualifier: wrong direction for player 1
            if (rowDifference != 2) { return 0; } 
        }
        // player 2 (Black)
        else 
        { 
            // qualifier: wrong direction for player 2
            if (rowDifference != -2) { return 0; } 
        }
    }

    // compute the position of the piece being jumped over and validate if occupied or opponent
    {
        // compute the middle row position
        int midRow = (fromPosition / 8) + (rowDifference / 2);
        // compute the middle column position
        int midCol = (fromPosition % 8) + (colDifference / 2);
        // compute the middle position
        int midPos = midRow * 8 + midCol;

        // there must be a piece to jump over in the middle, and the landing square must be empty
        if (!IsOccupiedSpace(game, midPos)) { return 0; } // no piece to jump over
        if (IsOccupiedSpace(game, toPosition)) { return 0; } // landing square not empty

        // qualifier: the piece being jumped over must belong to the opponent
        // if it's Player 1's turn, the jumped piece must be Player 2's
        if (IsRedPlayer1Turn(game)) 
        {
            // m = mask for the middle position, used to check occupancy
            unsigned long long m = (1ull << midPos);
            // qualifier: the piece being jumped over must belong to the opponent
         if (((game->player2_men | game->player2_kings) & m) == 0ull) { return 0; }
        } 
        // otherwise, if it's Player 2's turn, the jumped piece must be Player 1's
        else 
        {
            // m = mask for the middle position, used to check occupancy
            unsigned long long m = (1ull << midPos);
            // qualifier: the piece being jumped over must belong to the opponent
            if (((game->player1_men | game->player1_kings) & m) == 0ull) { return 0; }
        }

        *captured = midPos; // store the square that is being captured
        return 1; // legal capture move
    }
}

// Initialize Board and Display //

// initialize the board when new game, pieces assume starting positions,
// "current_turn" set to 1 (player 1 Red) at the start
void SetBoard(GameState* game)
{
    int row = 0; // initialize row iterator (0-7)
    int col = 0; // initialize column iterator (0-7)
    
    // player 1 (Red) is assigned to the top three rows (0, 1, 2)
    unsigned long long player1 = 0ull;

    // player 2 (Black) is assigned to the bottom three rows (5, 6, 7)
    unsigned long long player2 = 0ull;

    // precompute the bitmask of all valid dark squares (used by other checks)
    validSquares = MaskDarkSq();

    // populate player pieces on their starting dark squares
    // for player 1 (Red) men across rows 0, 1, 2 on dark squares
    for (row = 0; row <= 2; row++) 
    {
        // inner loop traverses through columns
        for (col = 0; col < 8; col++) 
        {
            // qualifier: only place pieces on dark squares where (row + col) is odd
            if (((row + col) % 2) == 1) 
            {
                // compute the index for that dark square
                int index = row * 8 + col;
                // set the corresponding bit in player1 bitboard to place a man at this index
                player1 = player1 | (1ull << index);
            }
        }
    }

    // populate player pieces on their starting dark squares
    // for player 2 (Black) men across rows 5, 6, 7 on dark squares
    for (row = 5; row <= 7; row++) 
    {
        // inner loop traverses through columns
        for (col = 0; col < 8; col++) 
        {
            // qualifier: only place pieces on dark squares where (row + col) is odd
            if (((row + col) % 2) == 1) 
            {
                // compute the index for that dark square
                int index = row * 8 + col;
                // set the corresponding bit in player2 bitboard to place a man at this index
                player2 = player2 | (1ull << index);
            }
        }
    }

    // Initialize game bitboards and starting turn:
    // player1_men / player2_men for 64-bit bitboards, where set bits mark pieces on # squares
    // player1_kings / player2_kings initialized as empty at the start.
    // current_turn: 1 indicates it's player 1's (Red's) turn first.
    game->player1_men = player1;
    game->player1_kings = 0ull;
    game->player2_men = player2;
    game->player2_kings = 0ull;
    game->current_turn = 1;

}

// print an ASCII representation of the board
// 8 x 8 size, "#" dark squares & "_" white squares
// Player 1 Red Pieces: "r" men & "R" kings
// Player 2 Black Pieces: "b" men & "B" kings
void PrintBoardPretty(const GameState* game)
{
    // row/col iterators for the 8x8 board; rows and columns range 0..7
    int row = 0;
    int col = 0;

    // print column header for board coordinates
    printf("\n    0 1 2 3 4 5 6 7 \n");

    // iterate rows from top (0) to bottom (7)
    for (row = 0; row < 8; row++)
    {
        // print row index at the start of each line
        printf("%d | ", row);

        // iterate columns left (0) to right (7)
        for (col = 0; col < 8; col++)
        {
            // convert board coordinates (row, col) into single 0–63 index
            int index = row * 8 + col;

            // bitmask for this square used to check piece occupancy
            unsigned long long mask = (1ull << index);

            // determine if square is dark (#) — dark when (row + col) is odd
            int dark = ((row + col) % 2);

            // print piece characters in priority order 
            // kings, men, then empty square
            if ((game->player1_kings & mask) != 0ull) { putchar('R'); }
            else if ((game->player1_men & mask) != 0ull) { putchar('r'); }
            else if ((game->player2_kings & mask) != 0ull) { putchar('B'); }
            else if ((game->player2_men & mask) != 0ull) { putchar('b'); }
            else
            {
                // empty: show '#' for dark (playable) squares and '_' for light squares
                if (dark == 1) { putchar('#'); }
                else { putchar('_'); }
            }

            // spacing between columns
            putchar(' ');
        }

        // end of row
        printf("|\n");
    }

    // blank line after the board for readability
    printf("\n");
}

// Inspired Helper Functions //

// convert board coordinates (row, col) to a bit position (0-63)
// return -1 if coordinates are out of bounds
int ConvertRowColToIndex(int row, int col)
{
    // qualifier: ensure row value is within board boundaries (0–7)
    // if not, return -1 to indicate invalid coordinate
    if (row < 0 || row > 7) { return -1; }
    
    // qualifier: ensure column value is within board boundaries (0–7)
    // if not, return -1 to indicate invalid coordinate
    if (col < 0 || col > 7) { return -1; }
    
    // translate valid (row, col) into a board index (0–63)
    // used to identify specific bit positions on the 8 x 8 bitboard
    return row * 8 + col;
}

// check if given board position is a valid playable dark square ("#")
// return 1 when valid, otherwise 0 when invalid
int IsValidDarkSquare(int position)
{
    // qualifier: ensure position index is within valid board range (0–63)
    // if position is outside the board, return 0 (invalid)
    if (position < 0 || position > 63) { return 0; }

    // calculate which row the position belongs to (0–7)
    int row = position / 8;

    // calculate which column the position belongs to (0–7)
    int col = position % 8;

    // determine if square is dark by summing row and column
    // “#” squares occur where sum is odd
    int sum = row + col;

    // qualifier: when sum of row and col is odd, 
    // the square is playable, return 1 for valid dark square
    if ((sum % 2) == 1) { return 1; }

    // otherwise, sqaure is "_" light square and return 0 for invalid dark square
    return 0;
}

// check if any player piece is currently occupying a board position
// return 1 if occupied, otherwise 0 if unoccupied
int IsOccupiedSpace(const GameState* game, int position)
{
    // qualifier: ensure given position is within valid board range (0–63)
    // if position is outside board boundaries, return 0 (unoccupied)
    if (position < 0 || position > 63) { return 0; }
    
    // create a bitmask representing the specific board position
    unsigned long long mask = (1ull << position);
    
    // combine all pieces for both players into a single mask
    // represents every square currently occupied by players
    unsigned long long allPieces = game->player1_men | game->player1_kings | game->player2_men | game->player2_kings;
    
    // qualifier: perform a bitwise AND to check if the given position’s bit is set in allPieces
    // if true, the position is occupied and return 1
    if ((allPieces & mask) != 0ull) { return 1; }
    
    // otherwise, the bit is not set and no piece occupies this position
    // return 0 indicating unoccupied square
    else { return 0; }
}

// check if the piece at "position" belongs to a current player
// used to ensure that the player will be moving THEIR piece
int PieceBelongToPlayer(const GameState* game, int position)
{
    // qualifier: ensure given position is within the valid board range (0–63)
    // if outside bounds, return 0 (invalid position, no ownership)
    if (position < 0 || position > 63) { return 0; }

    // create a bitmask corresponding to the given position
    unsigned long long mask = (1ull << position);

    // qualifier: determine which player’s turn it currently is
    // if Player 1 (Red), check if that bit exists in player 1’s bitboard pieces
    if (IsRedPlayer1Turn(game))
    {
        // perform bitwise AND between combined player 1 pieces and position mask
        // if nonzero, a player 1 piece occupies that square
        // return 1, belonging to player 1
        if (((game->player1_men | game->player1_kings) & mask) != 0ull) { return 1; }
    }
    // otherwise, if it is Player 2’s (Black) turn
    else
    {
        if (((game->player2_men | game->player2_kings) & mask) != 0ull) { return 1; }
    }

    // no piece belonging to the current player at this position
    return 0;
}

// Player Move and Turn Functions //

// attempts to read a move or capture, for "FROM" to "TO" position
// return 1 if valid move and proceed with the action, otherwise 0
int TryMove(GameState* game, int fromPosition, int toPosition)
{
    // qualifiers for both FROM and TO
    // ensure within indexes of 0-63
    if (fromPosition < 0 || fromPosition > 63) { return 0; }
    if (toPosition < 0 || toPosition > 63) { return 0; }

    // qualifier: both squares FROM and TWO must be valid dark squares “#” 
    // light squares are illegal
    if (!IsValidDarkSquare(fromPosition)) { return 0; }
    if (!IsValidDarkSquare(toPosition)) { return 0; }

    // qualifier: FROM must contain a piece owned by the turn player to move,
    // and TO must be unoccupied to transition to
    if (!PieceBelongToPlayer(game, fromPosition)) { return 0; }
    if (IsOccupiedSpace(game, toPosition)) { return 0; }

    // create a bitmask representing the FROM square using bit shifting
    unsigned long long fromMask = (1ull << fromPosition);
    // determine which player’s turn it is, return 1 for Red or 0 for Black
    int isPlayer = IsRedPlayer1Turn(game); 
    // initialize flag for king status. 1 for king and 0 for man
    int isKing = 0;

    // if Red to move, check Red’s king bitboard at FROM and set isKing if present
    if (isPlayer) 
    { 
        // when FROM bit is set in player1_kings, the piece is a king
        if ((game->player1_kings & fromMask) != 0ull) { isKing = 1; } 
    }

    // otherwise Black to move, check Black’s king bitboard
    else      
    { 
        // when FROM bit is set in player2_kings, the piece is a king
        if ((game->player2_kings & fromMask) != 0ull) { isKing = 1; } 
    }

    // attempt a capture (jump) move before a normal step
    // this version supports single-capture moves only (no multiple jumps)
    {
        int jumped = -1; // hold the index of the captured piece when valid

        // validate capture path and confirm opponent piece lies between FROM and TO
        // if capture is legal, the middle square index is saved in "jumped"
        if (IsLegalCapture(game, fromPosition, toPosition, isKing, isPlayer, &jumped)) 
        {
            // precompute masks for destination and the captured position
            unsigned long long toMask = (1ull << toPosition);
            unsigned long long jumpedMask = (1ull << jumped);

            // perform capture logic for the player currently taking their turn
            if (isPlayer) 
            {
                // player 1 (Red): perform capture by moving piece from FROM to TO using bitwise operations
                if ((game->player1_kings & fromMask) != 0ull) 
                {
                    // clear the original FROM bit in player1_kings, then set the TO bit
                    game->player1_kings = (game->player1_kings & ~fromMask) | toMask; 
                }
                else                                  
                { 
                    // clear the FROM bit and set the TO bit in player1_men
                    game->player1_men = (game->player1_men & ~fromMask) | toMask; 
                }

                // remove the opposing Black piece from the jumped square, king or man
                if ((game->player2_kings & jumpedMask) != 0ull) 
                {
                    // clear from kings
                    game->player2_kings &= ~jumpedMask;
                }
                else                                    
                {
                    // clear from men
                    game->player2_men &= ~jumpedMask;
                }

                printf("Player 1 (Red) captured Player 2 (Black)! Jumping over position %d.\n", jumped);
            } 

            // perform capture logic for player 2 black
            else 
            {
                //  perform capture by moving piece from FROM to TO using bitwise operations
                if ((game->player2_kings & fromMask) != 0ull) 
                { 
                    // update player2_kings bitboard
                    game->player2_kings = (game->player2_kings & ~fromMask) | toMask; 
                }
                else
                { 
                    // update player2_men bitboard
                    game->player2_men = (game->player2_men & ~fromMask) | toMask; 
                }

                // remove the opposing Red piece from the jumped square, either king or man
                if ((game->player1_kings & jumpedMask) != 0ull) 
                {
                    // clear from kings
                    game->player1_kings &= ~jumpedMask;
                }
                else
                {
                    // clear from men
                    game->player1_men &= ~jumpedMask;
                }

                printf("Player 2 (Black) captured Player 1 (Red)! Jumping over position %d.\n", jumped);
            }

            // after capture, check if the piece qualifies for promotion
            Promote(game, toPosition);
            return 1; // capture successful
        }
    }

    // check if the intended move (non-capture) follows valid diagonal direction rules
    // returns 0 if the move invalid
    if (!IsLegalMove(game, fromPosition, toPosition, isKing, isPlayer)) 
    { 
        return 0; 
    }

    // otherwise, apply the standard move with no capture
    {
        // create bitmask for TO square
        unsigned long long toMask = (1ull << toPosition);

        // if it's player 1 (Red) turn
        if (isPlayer) 
        {
            // check if the piece being moved is a KING
            if ((game->player1_kings & fromMask) != 0ull) 
            { 
                // clear the FROM bit in kings and set the TO bit
                game->player1_kings = (game->player1_kings & ~fromMask) | toMask; 
            }
            // otherwise, move a normal man piece, clearing FROM and setting TO
            else 
            { 
                game->player1_men = (game->player1_men & ~fromMask) | toMask; 
            }
        }
        // when player 2 (Black) turn
        else 
        {
            // check if the piece being moved is a KING
            if ((game->player2_kings & fromMask) != 0ull) 
            { 
                // clear the FROM bit in kings and set the TO bit
                game->player2_kings = (game->player2_kings & ~fromMask) | toMask; 
            }
            // otherwise, move a normal man piece, clearing FROM and setting TO
            else 
            { 
                game->player2_men = (game->player2_men & ~fromMask) | toMask; 
            }
        }
    }

    // after completing the move, check for promotion to KING if the piece reached the far end
    Promote(game, toPosition);
    return 1; // move successful
}

// check which player’s turn it currently is
// returns 1 if Player 1’s turn
int IsRedPlayer1Turn(const GameState* game)
{
    // qualifier: compare the "current_turn" flag in the GameState struct
    return (game->current_turn == 1);
}

// check which player’s turn it currently is
// returns 1 if Player 2’s turn
int IsBlackPlayer2Turn(const GameState* game)
{
    // qualifier: compare the "current_turn" flag in the GameState struct
    return (game->current_turn == 2);
}

// Win Conditions //

// checks if current player has any legal moves available
// returns 1 if at least one move exists, 
// return 0 if blocked
int CheckLegalMoves(const GameState* game)
{
    int position = 0; // initialize position to iterate for every board index sqaure

    // traverse through each index and determine if movable piece for the current player exists
    for (position = 0; position < 64; position++) 
    {
        // create a bitmask for this index to examine occupancy
        unsigned long long mask = (1ull << position);
        int M = 0; // flagger if square contains a piece for the current turn player
        int K = 0; // flagger for either man (0) or king (1)

        // examine which player turn it is 
        // examine player 1 turn
        if (IsRedPlayer1Turn(game)) 
        {
            // check if player 1 man occupies the sqaure
            if ((game->player1_men & mask) != 0ull) 
            { 
                M = 1; // mark found piece
                K = 0; // located man
            }
            // check if player 1 king occupies the sqaure
            if ((game->player1_kings & mask) != 0ull) 
            { 
                M = 1; // mark found piece
                K = 1; // located king
            }
        } 

        // examine player 2 turn
        else 
        {
            // check if player 2 man occupies the sqaure
            if ((game->player2_men & mask) != 0ull) 
            { 
                M = 1; // mark found piece
                K = 0; // located man
            }
            // check if player 2 king occupies the sqaure
            if ((game->player2_kings & mask) != 0ull) 
            { 
                M = 1; // mark found piece
                K = 1; // located king
            }
        }

        // if no current player piece occupies the square, skip to next square
        if (!M) continue;

        // otherwise, check every possible diagonal direction for legal moves or captures
        {
            // define directional offsets for diagonal movement (4 directions total)
            // representing Down-Right, Down-Left, Up-Right, and Up-Left
            int rowDifference[4] = { 1, 1, -1, -1 };
            int colDifference[4] = { 1, -1, 1, -1 };
            int i = 0; // loop iterator for direction checking

            // traverse through all diagnol directions for possible moves
            for (i = 0; i < 4; i++) 
            {
                // compute single step destination coordinates for this diagonal
                int row2 = (position / 8) + rowDifference[i];
                int col2 = (position % 8) + colDifference[i];
                // translate (row2,col2) into a single 0-63 index
                int to  = row2 * 8 + col2;

                // verify the square is inside board boundaries and is a valid dark square
                if (row2 >= 0 && row2 < 8 && col2 >= 0 && col2 < 8 && IsValidDarkSquare(to))
                {
                    // if the square is empty, verify if the step move is legal
                    if (!IsOccupiedSpace(game, to))
                    {
                        // validate diagonal step according to piece type and player
                        if (IsLegalMove(game, position, to, K, IsRedPlayer1Turn(game))) 
                        { return 1; } // valid non caputre move
                    }
                }

                // otherwise when no simple step, check for possible captures, which is 2 step jumps
                {
                    // calculate potential jump destination (two squares away)
                    int row3 = (position / 8) + (2 * rowDifference[i]);
                    int col3 = (position % 8) + (2 * colDifference[i]);
                    // translate (row3,col3) into a single 0-63 index
                    int to2 = row3 * 8 + col3;
                    int jumped = -1; // placeholder to store middle piece index if capture valid

                    // confirm jump destination is within board and playable on dark square
                    if (row3 >= 0 && row3 < 8 && col3 >= 0 && col3 < 8 && IsValidDarkSquare(to2))
                    {
                        // check if jumping over an opponent piece results in a valid capture
                        if (IsLegalCapture(game, position, to2, K, IsRedPlayer1Turn(game), &jumped)) 
                        { return 1; } // valid capture
                    }
                }
            }
        }
    }
    return 0; // no legal steps or captures found for any piece in any direction
}

// checks for a winner based on captured pieces
// returns 1 if player 1 (Red) wins, 
// return 2 if player 2 (Black) wins, 
// return 0 if no winner yet
int CheckWinner(const GameState* game)
{
    // build a combined bitboard of all player 1 pieces (men OR kings)
    // if this becomes 0, player 1 has no pieces remaining on the board
    unsigned long long player1_allPieces = game->player1_men | game->player1_kings;
    
    // build a combined bitboard of all player 2 pieces (men OR kings)
    // if this becomes 0, player 2 has no pieces remaining on the board
    unsigned long long player2_allPieces = game->player2_men | game->player2_kings;
    
    // qualifier: if player 1 has zero pieces, declare player 2 the winner
    if (player1_allPieces == 0ull) { return 2; }
    
    // qualifier: if player 2 has zero pieces, declare player 1 the winner
    if (player2_allPieces == 0ull) { return 1; }
    
    return 0; // otherwise, both players still have at least one piece and no winner yet
}