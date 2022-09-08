/*This file contains the functions necesaries to compute the keys needed in each
	round.*/

/*Takes a 64-bits word and rotates its bits n places to the left.*/
unsigned int RotL32(unsigned int w, unsigned int n);

/*Given a 28-bits word written on the 28 rithmost bits of a
	32-bits integer, RotL28in32 performs the cyclic left shift
	on that word as if the four leftmost bits of the integer 
	not exist.*/
unsigned int RotL28in32(unsigned int word, unsigned int n);

/*Declaration of the PC1 function.*/
unsigned int* const PC1(unsigned long long word);

/*Declaration of the PC2 function.*/
unsigned long long PC2(unsigned int C, unsigned int D);

/*This fucntion will return us the key schedule.*/
/*WARNING: Since we are using the 'new' operator, each time we call this
    function, we'll have (at some point) to deallocate the memory used.*/
unsigned long long* const DESKeyGenerator(unsigned long long k);