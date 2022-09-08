/*This file contains the functions necesaries to compute the keys schedule*/

#include<stdlib.h>
#include<iostream>
#include"SDef&Consts.hpp"
#include"KeySchedule.hpp"
#include"CipherFunctions.hpp"
#include"Error.hpp"

/*Takes a 32-bits word and rotates its bits n places to the left.*/
unsigned int RotL32(unsigned int w, unsigned int n){
	/*Making sure that n is a number in the set {0, ... , 31}.*/
	n %= 32;
	return w << n | w >> 32 - n;
}

/*Given a 28-bits word written on the 28 rithmost bits of a
	32-bits integer, RotL28in32 performs the cyclic left shift
	on that word as if the four leftmost bits of the integer 
	not exist.*/
/*Look for the constants in the SDef&Const.hpp file.*/
unsigned int RotL28in32(unsigned int word, unsigned int n)
{
	if (word > THRESHOLD28)
		error("Something went wrong in function 'RotL28in32'; the inequality word <= FFFFFFF must be hold.");
	
	/*Making sure that n is a number in the set {0, ... , 27}.*/
	n %= 28;
	unsigned int ones32 = THRESHOLD32, ones28 = THRESHOLD28;
	unsigned int aux = word << 4;

	word = RotL32(word, n);
	aux = RotL32(aux, n) & ones32 >> 32-n;
    word = (word | aux) & ones28;

	return word;
}

/*Definition of the PC1 function.*/
unsigned int* const PC1(unsigned long long word)
{
	/*All the bits in 'bitsOut are one's but the bits
		8, 16, 24, 32, 40, 48, 56, 64.*/
	const unsigned long long bitsOut = 0xFEFEFEFEFEFEFEFE;

	/*Converting in zeros the bits 8, 16, 24, 32, 40, 48,
		56, 64.*/
	word &= bitsOut;

	/*Obtaining a 54-bits word.*/
	word = Permutation64(word, PC1Table);

	if (word > 0xFFFFFFFFFFFFFF)
		error("Error on function PC1; the first 8 bits are not all zeros.");

	const unsigned long long half = static_cast<unsigned long long>(THRESHOLD28); /*28 ones*/
	/*Dividing 'word' in two halfs.*/
	unsigned long long C0 = (word & half << 28) >> 28, D0 = word & half;

	if( C0 > half )
		error("Something went wrong on function PC1; the first thirty six bits of C0 \
			   (from left to right) must be zeros.");

	if( D0 > half )
		error("Something went wrong on function PC1; the first thirty six bits of D0 \
			   (from left to right) must be zeros.");

	unsigned int* C0D0 = new unsigned int[2];
    C0D0[0] = static_cast<unsigned int>(C0);
    C0D0[1] = static_cast<unsigned int>(D0);

	return C0D0;
}

/*Definition of the PC2 function.*/
/*The first four  bits of 'C' and 'D' must be zeros.*/
unsigned long long PC2(unsigned int C, unsigned int D)
{
	/*In case of not having zeros in the first four bits of C.*/
	if (C > THRESHOLD28)
		error("Error on function PC2; the first 4 bits of 'C' are not all zeros.");

	/*In case of not having zeros in the first four bits of D.*/
	if (D > THRESHOLD28)
		error("Error on function PC2; the first 4 bits of 'D' are not all zeros.");

	/*Creating a 56-bits word with C and D.*/
	unsigned long long word = static_cast<unsigned long long>(C) << 28 |
							  static_cast<unsigned long long>(D);

	/*1 followed by 55 zeros.*/
	const unsigned long long frotBit = 0x80000000000000;
	unsigned long long pc2 = 0x00;
	
	/*Computing PC2.*/
	for(int i = 0; i < 48; i++)
		pc2 |= ((frotBit >> PC2Table[i] - 1 & word) << PC2Table[i] - 1) >> i;

	/*We started to 'put bits' in the fifty six bit of pc2, the eight last bits are 
		untouched, so we need to adjust.*/
	pc2 >>= 8;

	if (pc2 > THRESHOLD48)
		error("Error on function PC2; the first 16 bits of 'pc2' are not all zeros.");

	return pc2;
}

/*This fucntion will return us the key schedule.*/
/*WARNING: Since we are using the 'new' operator, each time we call this
    function, we'll have (at some point) to deallocate the memory used.*/
unsigned long long* const DESKeyGenerator(unsigned long long k)
{
	unsigned int* const CD = PC1(k);
	unsigned long long* keySchedule = new unsigned long long[16];

	for (int i = 0; i < 16; i++)
	{
		if ( i==0 || i==1 || i==8 || i==15 )
		{
			CD[0] = RotL28in32(CD[0], 1);
			CD[1] = RotL28in32(CD[1], 1);
		}	else
			{
				CD[0] = RotL28in32(CD[0], 2);
				CD[1] = RotL28in32(CD[1], 2);
			}
		keySchedule[i] = PC2(CD[0], CD[1]);
	}

    delete[] CD;
	
	return keySchedule;
}