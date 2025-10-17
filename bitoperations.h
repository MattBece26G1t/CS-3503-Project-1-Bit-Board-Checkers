// [bitoperations.h] header file
// function declarations for "bitoperations.c"
// implement in "main.c"

#ifndef BITOPERATIONS_H 
#define BITOPERATIONS_H 

#include <stdio.h> // for printing and reading files
#include <stdint.h> // for uint32_t (functions here use unsigned int)

// { Phase 1 - Core Bit Manipulation Capabilities } //
// adapts the skeleton ideas provided in 
// "2.3 Core Bit Manipulation Capabilities"
// "2.6 Example Implementations and Helper Functions"

/*
    These functions focus on manipulating bits inside a 32-bit int "value"
    "value" is a 32-bit unsigned integer that is being modified/interpreted
    "position" focuses on which bit out of 0-31 to work on
    Any "position" read from the user outside of range 0-31 is invalid

    Includes basic operations for bit interaction: 
        set / clear / toggle(flip) / shift left+right bit
    and for reference: get and count bit / print binary + hex
*/

// Basic bit operations //

// set bit at "position" (0-31) to 1 in "value"
// returns the new 32-bit "value" after setting bit
unsigned int SetBit(unsigned int value, int position);

// clear the bit at "position" (0–31) to 0 in "value"
// returns the new 32-bit value after clearing that bit
unsigned int ClearBit(unsigned int value, int position);

// toggle (flip) the bit at "position" (0–31) in "value" (0 <-> 1)
// returns the new 32-bit value after toggling that bit
unsigned int ToggleBit(unsigned int value, int position);

// read the bit at "position" (0–31) from "value," returns either 0 or 1
// checks "value" at that bit, function returns the bit state
// if "position" is outside the range, returns 0 by default
int GetBit(unsigned int value, int position);

// Counting and finding //

// count number of 1s in "value"
// loops through each bit (0-31) and adds all bits set
int CountBits(unsigned int value);

// Shift operations //

// shift "value" left by "positions" (logical)
// positions <= 0 (no change) OR positions >= 32 (result is 0)
// returns the new 32-bit value
unsigned int ShiftLeft(unsigned int value, int positions);

// shift "value" right by "positions" (logical)
// positions <= 0 (no change) OR positions >= 32 (result is 0)
// returns the new 32-bit value
unsigned int ShiftRight(unsigned int value, int positions);

// Display functions //

// print "value" in 32-bit binary format
// displays as groups of 4 bits
void PrintBinary(unsigned int value);

// print "value" in hexadecimal format (0xXXXXXXXX)
void PrintHex(unsigned int value);

// Additional helper function approach //

// helper function to create a bit mask at given "position" (0–31)
// returns a mask with a single 1 at that bit spot
// if "position" is outside range, returns 0 by default
unsigned int CreateMask(int position);

#endif