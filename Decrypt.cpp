#include<iostream>
#include<string>
#include<fstream>
#include"DES.hpp"
#include"KeySchedule.hpp"
#include"Error.hpp"

int main(int argc, char* argv[])
{
	if (argc != 3)
		error("Wrong number of arguments.\n");

	char ch;
	std::string cipher = "", key = "", message;

	std::ifstream input(argv[1]);
	if (!input)
		error("Can't open input file.", argv[1]);
	while (input.get(ch))
		cipher += ch;
	input.close();

	if (cipher.length()%8 != 0)
		error("Cipher message has not been padden. Its length must be a eight multiple.\n");

	input.open(argv[2]);
	if (!input)
		error("Can't open input file.", argv[2]);
	while (input.get(ch))
		key += ch;
	input.close();

	if (key.length() != 8)
		error("Something went wrong; key.lenght() must be equal to eight.");

	unsigned long long KEY = EightChartoLong(key);
	unsigned long long * const keyschedule = DESKeyGenerator(KEY),
	                          *Cipher = StringToLongIntStream(cipher);

	message = DESDecryption(Cipher, cipher.length() / 8, keyschedule);

	while (message[message.length() - 1] == '\0')
		message.pop_back();

	std::cout << message << '\n';
	
	return 0;
}