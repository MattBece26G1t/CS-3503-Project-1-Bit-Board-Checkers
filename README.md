# CS-3503-Project-1-Bit-Board-Checkers

## Author 
Matthew Becerra

## Project Description
Bit Board Checkers [Two-Player Edition]
Program developed in the IDE Visual Studio Code (Language: C) and system used: Windows 11
My adaptation of the "Owl Tech Industries Software Engineering Internship Program - BitBoard Game Engine" 
 
The project implements a simplified version of checkers using bitboard representation of the game board. Each 64-bit unsigned integer encodes piece positions and movement for both players. 

The program covers the two phases from the project guidelines:

[Phase 1 - Bit Manipulation Operations] 

Users can access the "Bit Operations Demonstration" through the main menu (option 5), which is a separate interactive demo (menu and options separate from the main program) that allows users to test core bit manipulation capabilities, using a 32-bit integer value.

Operations such as:

	- Set / Clear / Toggle / and Get individual bits
  
	- Count total bits set to 1
  
	- Shift bits Left or Right
  
	- Display the current value in binary and hexadecimal formats
  
Additionally, the value is printed before and after methods that manipulate the bit.

This phase captures foundation level ideas and logic that will be used for the actual bit board checkers game. This miniature program separates itself from the actual game.  

[Phase 2 - Checkers Game Implementation] 

The main gameplay portion of the program, which implements a playable two player checkers game built using bitboard representation, with 64-bit unsigned integers to depict the game state.

Included in this section of the program:

	- Diagonal step movement (man-one direction; king-both directions), singular capture (this version does not support more than one capture), and king promotion conditions
  
	- Win conditions based on complete captured pieces of opposing player or if a player has any legal moves left
  
	- Player turn switches between Player 1 (Red) and Player 2 (Black)
  
	- Save/Load functionality for restoring game states
  
	- UI display, including menu, ASCII board, and descriptive text

## Build and Run
This program was built and tested on Windows 11, using the Visual Studio Code IDE for development/editing, and initially compiled through the MSYS2 MINGW64 terminal environment. This was just my method of building the program to get it to run.

[Step 1 – Open MSYS2 MINGW64]

Locate and open “MSYS2 MINGW64” (this application was already included with my system). A terminal should appear providing a Unix-styled environment that supports the 'make' building system.

Example when opening: 
```
user@LAPTOP-XXXXXXX MINGW64 ~

$
```

[Step 2 – Navigate to the Project Directory]

Use the 'cd' command and paste your project's directory path to move into the folder that contains your all of your project files.

Example:
```
$ cd /c/Users/name/path/path/BitBoardCheckersFinal
```

(Replace “name/path/path/BitBoardCheckersFinal” with your actual user and folder names.)

[Step 3 – Build and Run the Program]

Once inside your project directory, enter the following:
```
make clean
make
./bitboardcheckers.exe
```


"make clean" - will refresh from any previous runs, deleting any previously compiled object files or executable builds

"make" - uses the instructions in the Makefile to automatically compile and link all .c source files into object files and implement them into "bitboardcheckers.exe".

"./bitboardcheckers.exe" - runs the compiled game in the terminal after being built.

The bitboard checkers game should appear in the terminal, but alternatively, can be run in the IDE application


[Afterwards – Visual Studio Terminal]

The program can also be run directly from the Visual Studio Code terminal after initial setup in MSYS2 MINGW64. Simply insert this into your terminal and the game should run:
```
make clean
make
./bitboardcheckers.exe
```

## Game Instructions - Adapted From InGame Menu
Close to playing like regular checkers!

[Board Setup]

The board is 8 × 8 and displayed using ASCII symbols:

"_" Light squares (non-playable)

"#" Empty dark squares (playable)

"r" / "b" Men pieces for Player 1 (Red) and Player 2 (Black)

"R" / "B" King pieces for Player 1 (Red) and Player 2 (Black)

Each player begins with 12 pieces.

[Movement]

Pieces are only allowed to move along the dark “#” squares diagonally.

Players enter valid board positions (0–63).
They will specify which piece to move:
FROM -> TO
(Example: move “b” on index 40 -> TO index 33)

A numbered reference board (0–63) is displayed for easy visualization and interpretation of all playable squares.

[General Movement Direction]
Player 1 (Red) moves first. Red men (r) move downward on the board (toward larger indexes).

Player 2 (Black) moves second. Black men (b) move upward on the board (toward smaller indexes).

[Simple Movement]

One step diagonally onto an empty dark square.
"r" and "b" move toward their opponent’s side.

[Capturing]

Occurs by jumping diagonally over an adjacent opponent piece onto an empty dark square.

Input the destination number you are jumping to, not the piece you are jumping over.
(Example: “b” on (FROM) index 40 jumps TO index 26, capturing the “r” piece on index 33)

*Normally, checkers allows multiple captures, but in this version,
only single captures are allowed - for both men and kings.

[King Promotion]

When a piece reaches the far end of the board:

"r" reaching the bottom edge becomes "R".

"b" reaching the top edge become "B".

Kings can move both directions diagonally.

[Win Conditions]

A player wins when all opponent pieces are captured,
or when the opponent has no legal moves remaining.

[Saving and Loading]

Save and Load features use a simple text based system that records the game state in 5 lines.

When saving, enter a file name string (Example: game1).

When loading, type the file name exactly as it appears to restore the game.
(Example: Enter save file name to load: game1)

## Test File Examples
Provided are two save files with the 5 line game states: "BlackWinTest1" and "gameOneMidGame" 

"BlackWinTest1": is set up to where there is a single "r" piece positioned to be captured by a "b" piece. The user can let Player 2 be the winner by moving the "b" piece on index 55 to 37, which captures the last Player 1 "r" piece on index 46.

"gameOneMidGame": is set up to where there is a current game.

When running the program, make sure to have these two files inside your project folder. Select option 4 "Load Game" and type the file name exactly as type to load the most recent game state.

Additionally, there are provided "screen shots" (screen examples) of the two save files, which is simply the output history from the terminal copy and pasted into a text file.
 
*Note: When you run the program again in a new terminal, it will ONLY show the latest game state (whose last turn it was) when loading save files. The rest of the output history will not be shown. The "BlackWinTest1" (screen example) shows a better example of loading a save file into a new terminal, showing where the game last left off. "gameOneMidGame" was the most recent game played, so the output history in the terminal was copied.

[Text File Appearance Of The Save Files]

The actual files depict the game state only and wont actually show what's inside the running terminal (game presentation).

"BlackWinTest1"
```
70368744177664
0
1549801256397045760
1073741824
2

```

"gameOneMidGame"
```
270685602
0
6161558854489341952
0
1

```
