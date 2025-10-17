// [bitoperations.c] file

// note: used unsigned literals (1u or 0u) so shifts are unsigned,
// avoiding sign related issues when creating masks

#include "bitoperations.h" // declare "bitoperations" variables/methods

// Additional helper function approach //
// helper function to create a bit mask at given "position" (0–31)
// returns a mask with a single 1 at that bit spot
unsigned int CreateMask(int position)
{
    // qualifier: if position is outside range, return 0
    if (position < 0 || position >= 32)
    {
        // return 0 using unsigned int literal for safety to avoid sign issues
        return 0u;
    }

    // create the mask by shifting 1 to the left by "position" bits
    return (1u << position);
}

// Basic bit operations //

// set bit at "position" (0-31) to 1 in "value"
// returns the new 32-bit "value" after setting bit
unsigned int SetBit(unsigned int value, int position)
{
    // create a mask for that position to set
    unsigned int mask = CreateMask(position);

    // qualifier: if mask is 0 (invalid position), return original value
    if (mask == 0u) { return value; }

    // set bit using bitwise OR and return new value
    return value | mask;
}

// clear the bit at "position" (0–31) to 0 in "value"
// returns the new 32-bit value after clearing that bit
unsigned int ClearBit(unsigned int value, int position)
{
    // create a mask for that position to clear
    unsigned int mask = CreateMask(position);

    // qualifier: if mask is 0 (invalid position), return original value
    if (mask == 0u) { return value; }

    // clear bit using bitwise AND with NOT mask and return new value
    return value & (~mask);
}

// toggle (flip) the bit at "position" (0–31) in "value" (0 <-> 1)
// returns the new 32-bit value after toggling that bit
unsigned int ToggleBit(unsigned int value, int position)
{
    // create a mask for that position to toggle
    unsigned int mask = CreateMask(position);

    // qualifier: if mask is 0 (invalid position), return original value
    if (mask == 0u) { return value; }

    // toggle (flip) bit using bitwise XOR and return new value 0 <-> 1
    return value ^ mask;
}

// read the bit at "position" (0–31) from "value," returns either 0 or 1
// checks "value" at that bit, function returns the bit state
// if "position" is outside the range, returns 0 by default
int GetBit(unsigned int value, int position)
{
    // create a mask for that position to read
    unsigned int mask = CreateMask(position);

    // qualifier: if mask is 0 (invalid position), return 0 by default
    if (mask == 0u) { return 0; }

    // qualifier: check if that bit is set in value and return 1 if so
    if ((value & mask) != 0u) { return 1; }
    
    // otherwise, return 0 if that bit is not set
    return 0;
}

// Counting and finding //

// count number of 1s in "value"
// loops through each bit (0-31) and adds all bits set
int CountBits(unsigned int value)
{
    int count = 0; // initialize count of set bits
    int i = 0; // loop index for each bit position (0-31)

    // traverse each bit position
    for (i = 0; i < 32; i++) 
    {
        // create a mask for the current bit position,
        // shifting 1 to the left by i bits
        // this isolates the bit at position i
        unsigned int mask = (1u << i);

        // qualifier: if value AND mask is not zero, that bit is set
        if ((value & mask) != 0u) 
        {
            count = count + 1; // increment count for each bit set
        }
    }

    // return the total count of set bits
    return count;
}

// Shift operations //

// shift "value" left by "positions" (logical)
// positions <= 0 (no change) OR positions >= 32 (result is 0)
// returns the new 32-bit value
unsigned int ShiftLeft(unsigned int value, int positions)
{
    // qualifier: if positions is less than or equal to 0, return original value
    if (positions <= 0) { return value; }

    // qualifier: if positions is greater than or equal to 32, return 0
    if (positions >= 32) return 0u;

    // otherwise, shift left by positions
    return (value << positions);
}

// shift "value" right by "positions" (logical)
// positions <= 0 (no change) OR positions >= 32 (result is 0)
// returns the new 32-bit value
unsigned int ShiftRight(unsigned int value, int positions)
{
    // qualifier: if positions is less than or equal to 0, return original value
    if (positions <= 0) { return value; }

    // qualifier: if positions is greater than or equal to 32, return 0
    if (positions >= 32) return 0u;

    // otherwise, shift right by positions
    return (value >> positions);
}

// Display functions //

// print "value" in 32-bit binary format
// displays as groups of 4 bits
void PrintBinary(unsigned int value)
{
    int i = 31; // initialize loop index for each bit position, going 31 down to 0
    int group = 0; // initialize group counter for formatting spaces
    printf("Printed Binary: ");

    // traverse each bit position from 31 down to 0
    for (i = 31; i >= 0; i--) 
    {
        // qualifier: if value AND 1 shifted left by i 
        // is not zero, that bit is 1. Use putchar to print '1'
        if ((value & (1u << i)) != 0u) { putchar('1'); }
        
        // otherwise, that bit is 0. Use putchar to print '0'
        else { putchar('0'); }
        group = group + 1; // increment group counter for formatting spaces

        // qualifier: if group is 4 and not at the last bit, print a space
        if (group == 4 && i != 0) 
        { 
            putchar(' '); // use putchar to print space
            group = 0; // reset group counter after printing space
        }
    }
    putchar('\n'); // print newline after binary representation
}

// print "value" in hexadecimal format (0xXXXXXXXX)
void PrintHex(unsigned int value)
{
    // use printf to format and print value in hexadecimal
    // with "0x" and zero-padded to 8 digits
    printf("Printed Hex: 0x%08X\n", value);
}