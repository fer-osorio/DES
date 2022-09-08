/*  Required functions for the ruction of the cipher function "f".*/

#include<iostream>
#include<bitset>
#include"Error.hpp"
#include"SDef&Consts.hpp"
#include"CipherFunctions.hpp"

/*Extension function.*/
/*Here we take a 32 bits block and 
	we extend it to a 48 bits block.*/
unsigned long long E(unsigned int blk)
{
	/*We change from a 32 bits (4bytes) word to a 64 bits (8bytes) word.*/
	unsigned long long block = static_cast<unsigned long long>(blk);
	unsigned long long eblock;          /*Extended block.*/
	unsigned long long x0 = 0x01, 
					   x1 = 0xF8000000, /* 11111000000000000000000000000000 */
					   x2 = 0x1F800000, /* 00011111100000000000000000000000 */
					   x3 = 0x1F80000,  /* 00000001111110000000000000000000 */
					   x4 = 0x1F8000,   /* 00000000000111111000000000000000 */
					   x5 = 0x1F800,    /* 00000000000000011111100000000000 */
		               x6 = 0x1F80,     /* 00000000000000000001111110000000 */
					   x7 = 0x1F8,      /* 00000000000000000000000111111000 */
					   x8 = 0x1F,       /* 00000000000000000000000000011111 */
					   x9 = 0x80000000; /* 10000000000000000000000000000000 */

	eblock = (block & x0) << 47 | (block & x1) << 15 | (block & x2) << 13 |
			 (block & x3) << 11 | (block & x4) << 9  | (block & x5) << 7  |
		     (block & x6) << 5  | (block & x7) << 3  | (block & x8) << 1  |
		     (block & x9) >> 31;

	return eblock;
}

/*Permutation of the bits of a 32-bits word.*/
/*We are assuming that 'permutation' is an integer array that represents a true 
	permutation of the set { 1, ... , 32}.*/
unsigned int Permutation32(unsigned int word, const unsigned int permutation[32])
{
	const unsigned int frontBit = 0x80000000; /* 1(31 zeros)*/
	/*Permutated word declaration.*/
	unsigned int permWord = 0x00;
	for (int i = 0; i < 32; i++)
		/*Computing the permuted word.*/
		permWord |= (((frontBit >> PTable[i] - 1) & word) << PTable[i] - 1) >> i;

	return permWord;
}


/*Substitution functions.*/
/*Starting from the most rigth bit and moving to the left,
	we take the bits from the positions leftShift*6 to 
	(leftShift+1)*6-1 and return a four bit number shifted
	leftShift*4 positions to the left.*/
unsigned int SFunction(unsigned long long blk, const unsigned int SDefinition[][16], unsigned int leftShift)
{
	if (leftShift > 7)
		error("Error in function 'SFunction'; the inequality leftShift < 8 must be meet.");

	const unsigned long long consts[4] = {0x3F,  /*(58 zeros)111111 */
										  0x20,	 /*(58 zeros)100000 */
										  0x01,  /*(58 zeros)000001 */
										  0x1E}; /*(58 zeros)011110 */
	/*Extracting the 6 bit block.*/
	unsigned long long extract = (blk & consts[0] << 6 * leftShift) >> 6*leftShift;
	/*Finding the row and column.*/
	unsigned int row = (extract & consts[1]) >> 4 | extract & consts[2],
				 column = (extract & consts[3]) >> 1;

	return SDefinition[row][column] << leftShift*4;
}

/*Definition of the Cipher fucntion.*/
unsigned int CipherFunction(unsigned long long key, unsigned int blk)
{
	if (blk > THRESHOLD48)
		error("Error in function 'CipherFunction; the inequality blk <= 0xFFFFFFFFFFFF must hold.");

    if (key > THRESHOLD48)
		error("Error in function 'CipherFunction; the inequality key <= 0xFFFFFFFFFFFF must hold.");

	unsigned long long B = key ^ E(blk);

    static int counterCipherFunction = 0;
    counterCipherFunction ++;

	unsigned int SB = SFunction(B, S1, 7) | SFunction(B, S2, 6) |
					  SFunction(B, S3, 5) | SFunction(B, S4, 4) |
					  SFunction(B, S5, 3) | SFunction(B, S6, 2) |
					  SFunction(B, S7, 1) | SFunction(B, S8, 0) ;

    if (SB > THRESHOLD32)
		error("Error in function 'CipherFunction; the inequality SB <= 0xFFFFFFFF must hold.");

	return Permutation32(SB, PTable);
}

/*Permutation of the bits of a 64-bits word.*/
/*We are assuming that 'permutation' is an integer array that represents a true 
	permutation of the set { 1, ... , 64}.*/
unsigned long long Permutation64(unsigned long long word, const unsigned int permutation[64])
{
	const unsigned long long frontBit = 0x8000000000000000;//static_cast<unsigned long long>(0x01) << 63;
	
	/*Permuted word initialization.*/
	unsigned long long permWord = 0x00;
	for (int i = 0; i < 64; i++)
		/*Computing the permuted word.*/
		permWord |= (((frontBit >> permutation[i] - 1) & word) << permutation[i] - 1) >> i;
 
	return permWord;
}

/*Computing the 'round' function phi.*/
unsigned int* const Phi(unsigned long long key,unsigned int x, unsigned int y)
{
	unsigned int* vec = new unsigned int[2];
    vec[0] = x ^ CipherFunction(key,y);
    vec[1] = y;
	return vec;
}

/*Interchangeing the values in a 2-array.*/
void Interchange(unsigned int* const v)
{
	unsigned int aux = v[0];
	v[0] = v[1];
	v[1] = aux;
}

/*Takes a 64-bits word and divides it in two 32-bits words.*/
unsigned int* const DivLongToInt(unsigned long long n)
{
	unsigned long long constant = THRESHOLD32;
	/*Coping the thirty two rightmost bits.*/
	unsigned long long y = n & constant, 
					   /*Coping the thirty two leftmost bits.*/
					   x = (n & (constant << 32)) >> 32;

    if (x > THRESHOLD32)
		error("Error in function 'DivLongToInt'; the inequality x <= 0xFFFFFFFF must hold.");

    if (y > THRESHOLD32)
		error("Error in function 'DivLongToInt'; the inequality y <= 0xFFFFFFFF must hold.");

	unsigned int* pair = new unsigned int[2];
    pair[0] = static_cast<unsigned int>(x);
    pair[1] = static_cast<unsigned int>(y);
    
	return pair;
}

/*Takes two 32- bits word and creates a 64-bits word just
	by concatenate them.*/
unsigned long long MergeIntToLong(unsigned int n1, unsigned int n2)
{
	unsigned long long n = static_cast<unsigned long long>(n1) << 32 |
						   static_cast<unsigned long long>(n2);
	return n;
}