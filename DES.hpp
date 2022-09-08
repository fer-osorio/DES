/*  Implementation of DES algorithm.*/
#include<string>

/*  If necessary, paddedMessage appends enough single space characters
    (this is, ' ' characters) in orden to met the msj.length() % 4 == 0
    equality.*/
std::string paddedMessage(std::string msj);

/*Forms an integer with four characters.*/
unsigned long long EightChartoLong(std::string word);

/*Converts a string to a sequence (array) of integers.*/
/*Each integer is create from four characters.*/
/*WARNING: Since we are using the 'new' operator, each time we call this
    function, we'll have (at some point) to deallocate the memory used.*/
unsigned long long * StringToLongIntStream(std::string str);

/*Setting a 'random' 64-bits key.*/
std::string setKey(void);

/*Implementation of DES function on a 64-bits word.*/
unsigned long long DESLongInt(unsigned long long, unsigned long long * const keySchedule);

/*Implementation of the DES inverse function on a 64-bits word.*/
unsigned long long DESInvLongInt(unsigned long long word, unsigned long long * const keySchedule);

/*Converts a 64-bit unsigned integer in a string.*/
std::string LongToString(unsigned long long word);

/*Converts a 64-bits unsigned integer array into a string.*/
std::string LongIntStreamToString(unsigned long long* intStream, unsigned int len);

/*Implementation of DES encryption algorithm on a String.*/
/*WARNING: This function allocates memory with the 'new' operator. Each time we call this
    function we'll have to deallocate that memory.*/
unsigned long long * const DESEncryption(std::string message, unsigned long long * const keySchedule);

/*Implementation of the DES decryption algorithm on a string.*/
std::string DESDecryption(unsigned long long * IS, unsigned int ISlength, unsigned long long* const keySchedule);