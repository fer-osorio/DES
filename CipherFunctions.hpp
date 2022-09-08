/*  Construction of the cipher function.  */

#include<string>
#include<bitset>

/*Extension function.*/
unsigned long long E(unsigned int block);	

/*Permutation of the bits of a 32-bits word.*/
/*We are assuming that 'permutation' is an integer array that represents a true 
	permutation of the set { 1, ... , 32}.*/
unsigned int Permutation32(unsigned int word, const unsigned int permutation[32]);

/*Permutation of the bits of a 64-bits word.*/
/*We are assuming that 'permutation' is an integer array that represents a true 
	permutation of the set { 1, ... , 64}.*/
unsigned long long Permutation64(unsigned long long word, const unsigned int permutation[64]);

/*Substitution functions.*/
unsigned int SFunction(unsigned long long blk, const unsigned int SDefinition[][16], unsigned int leftShift);

/*Definition of the Cipher fucntion.*/
unsigned int CipherFunction(unsigned long long key, unsigned int blk);

/*Computing the 'round' function phi.*/
unsigned int* const Phi(unsigned long long key,unsigned int x, unsigned int y);

/*Interchangeing the values in a 2-array (\mu function).*/
void Interchange(unsigned int* const v);

/*Takes a 64-bits word and divides it in two 32-bits words.*/
unsigned int* const DivLongToInt(unsigned long long n);

/*Takes two 32- bits word and creates a 64-bits wordjust
	by concatenate them.*/
unsigned long long MergeIntToLong(unsigned int n1, unsigned int n2);