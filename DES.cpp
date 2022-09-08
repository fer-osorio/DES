/*  Implementation of DES algorithm.*/

#include<iostream>
#include"CipherFunctions.hpp"
#include"DES.hpp"
#include"Error.hpp"
#include"SDef&Consts.hpp"
#include"KeySchedule.hpp"

/*Appends enought null characters to meet the 
    (msg.length() +1)%8 == 0 equality.*/
/*The last character appended is the number of
    character that were appended by this function.*/
std::string paddedMessage(std::string msg)
{
    while (msg.length()%8 != 0)
        msg += static_cast<char>(0x00);

    return msg;
}

/*Takes 8 character and forms an unsigned long from them.*/
/*If we see each character just as a word of bits, we can say 
    that we are just concatenating them.*/
/*The length of 'word' must be 8.*/
unsigned long long EightChartoLong(std::string word)
{
	if (word.length() != 8)
		error("Error in function 'EightCharInt'; length of word must be 8 bytes.");

	unsigned long long block = 0x00;

	/*Since char type is one byte long, for each word[i],
		i = 1, ... , 8, the character bits are 
		located in the rightmost placed byte; so
		we have to move them to the right place.*/
	for (int i = 0; i < 8; i++)
		block = block << 8 | static_cast<unsigned long long>(word[i])&0xFF;

	return block;
}

/*Converts a string to a sequence (array) of unsigned long integers.*/
/*Each integer is create from eight characters.*/
/*We are assuming str.length() is divisible by 8.*/
/*WARNING: Since we are using the 'new' operator, each time we call this
    function, we'll have (at some point) to deallocate the memory used.*/
unsigned long long* StringToLongIntStream(std::string str)
{
	/*In case of a wrong assumption.*/
    if (str.length() % 8 != 0)
        error("Error in function 'StringToIntegerStream'; str.length() must be divisible by 8.");
	
	/*Integer Stream Length.*/
	int ISLength = str.length() / 8;

	std::string word; int k;
    unsigned long long* intStream = new unsigned long long[ISLength];

	/*Taking blocks of 8 characters and converting them
	to a unsigned integers.*/
	for (int i = 0; i < ISLength; i ++) {
		word = ""; k = i * 8;
        for (int j = 0; j < 8; j++)
			word += str[k + j];
        intStream[i] = EightChartoLong(word);
	}

	return intStream;
}

/*Setting a 'random' 64-bits key.*/
std::string setKey(void)
{
	std::string key = "";

	/*Setting the seed.*/
	srand(time(NULL));
    for(int i = 0; i < 8; i++)
		/*Inserting 'random' characters.*/
		key += static_cast<unsigned char>(rand()%128);

	return key;
}

/*Implementation of DES function on a 64-bits word.*/
unsigned long long DESLongInt(unsigned long long word, unsigned long long* const keySchedule)
{
	word = Permutation64(word, IP);
    /*WARNING: DivLongToInt uses de 'new' operator, deallocation of memory
    will be necesary.*/
	unsigned int * divWord = DivLongToInt(word);
	for (int i = 0; i < 15 ; i++){
        /*WARNING: Since the function 'Phi' returns a pointer created
        with the 'new' operator, the next line may cause a memory 
        leak.*/
		divWord = Phi(keySchedule[i], divWord[0], divWord[1]); 
		Interchange(divWord);
	}
	divWord = Phi(keySchedule[15], divWord[0], divWord[1]);
	word = MergeIntToLong(divWord[0], divWord[1]);

    delete[] divWord;
	word = Permutation64(word, IPinv);

	return word;
}

/*Implementation of the DES inverse function on a 64-bits word.*/
unsigned long long DESInvLongInt(unsigned long long word, unsigned long long* const keySchedule)
{
	word = Permutation64(word, IP);
    /*WARNING: DivLongToInt uses de 'new' operator.
    Deallocation of memory will be necesary.*/
	unsigned int* divWord = DivLongToInt(word);
	for (int i = 15; i > 0 ; i--){
        /*WARNING: Since the function 'Phi' returns a pointer created
            with the 'new' operator, the next line may cause a memory 
            leak.*/
		divWord = Phi(keySchedule[i], divWord[0], divWord[1]); 
		Interchange(divWord);
	}
	divWord = Phi(keySchedule[0], divWord[0], divWord[1]);
	word = MergeIntToLong(divWord[0], divWord[1]);

    delete[] divWord;
	word = Permutation64(word, IPinv);

	return word;
}

/*Converts a 64-bit unsigned integer in a string.*/
/*The 64 bits are grouped in bytes (8 bits) and then the 
    bytes are casted to characters.*/
std::string LongToString(unsigned long long word)
{
    std::string str = "";
                             /*Eight ones followed by 54 zeros.*/
    const unsigned long long ones8 = 0xFF00000000000000;
    unsigned char ch;

    for (int i = 0; i < 8; i++){
        /*Grouping and casting.*/
        ch = static_cast<unsigned char>((word & ones8 >> i*8) >> (7 - i)*8);
        str += ch;
    }

    return str;
}

/*Converts a 64-bits unsigned integer array into a string.*/
/*WARNING: If the length of a message is in bytes, the we have to divide it by 8.*/
std::string LongIntStreamToString(unsigned long long* const intStream, unsigned int length)
{
	std::string str = "";
	for (int i = 0; i < length; i++)
		str += LongToString(intStream[i]);

	return str;
}

/*Implementation of DES encryption algorithm on a String.*/
/*WARNING: This function allocates memory with the 'new' operator. Each time we call this
    function we'll have to deallocate that memory.*/
unsigned long long * const DESEncryption(std::string message, unsigned long long * const keySchedule)
{
    /*Making sure that message.length() is a eight multiple.*/
	message = paddedMessage(message);
	if (message.length() % 8 != 0)
		error("Error on function 'DESEncryption; 'paddedMessage' didn't work.'");

	/*Integers Stream.*/
    unsigned int ISlength = message.length() / 8;
	unsigned long long * const IS = new unsigned long long[ISlength],
                              *aux =  StringToLongIntStream(message);

	/*Encrypting each 64-bits block.*/
	for (int i = 0; i < ISlength; i++){
		aux[i] = DESLongInt(aux[i], keySchedule);
        IS[i] = aux[i];
        }

	return IS;
}

/*Implementation of the DES decryption algorithm on a integers stream.*/
std::string DESDecryption(unsigned long long * IS, unsigned int ISlength, unsigned long long* const keySchedule)
{
	/*Decrypting each 64-bits block.*/
	for (int i = 0; i < ISlength; i++)
		IS[i] = DESInvLongInt(IS[i], keySchedule);

    std::string message;
	message = LongIntStreamToString(IS, ISlength);

	return message;
}