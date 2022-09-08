/*  Main function. Implementation of the DES algorithm.*/

#include<iostream>
#include<string>
#include<fstream>
#include"DES.hpp"
#include"KeySchedule.hpp"
#include"Error.hpp"

int main(int argc, char *argv[])
{
    std::string message; 
    std::ofstream output;

    std::cout << "Write the message you want to cipher:\n";
    std::getline(std::cin, message);

    //std::cout << "message: " << message << '\n';
    unsigned int msjlen = message.length();
    if (msjlen%8 != 0)
        msjlen += 8 - message.length()%8;

    std::string key = setKey();
    unsigned long long KEY = EightChartoLong(key);
    //std::cout << "key: " << std::hex << KEY << "\n\n";

    unsigned long long * const keyschedule = DESKeyGenerator(KEY),
                              *encryptedMessage = DESEncryption(message, keyschedule);
    std::string encryptedmessage = LongIntStreamToString(encryptedMessage, msjlen/8);

    output.open("Cipher_message.txt");
    if (!output)
        error("Can't create output file for the cipher message.\n");
    output << encryptedmessage;
    output.close();

    output.open("key.txt");
    if (!output)
        error("Can't create output file for the key.\n");
    output << key;
    output.close();

    //std::cout << "Encrypted message:" << encryptedmessage << '\n' << '\n';

    //message = DESDecryption(encryptedMessage, msjlen/8, keyschedule);
    delete[] keyschedule;
    //std::cout << "Decrypted message:" << message <<'\n';
    
    return 0;
}