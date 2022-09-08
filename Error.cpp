/*Error handling.*/

#include<iostream>
#include<cstdlib>
#include<bitset>
#include"Error.hpp"

/*When it is called, this function displays 
	an error message and terminates the program.*/
/*msg0 and msg1 conform the error message. msg1
	is by default an empty string.*/
void error(std::string msg0, std::string msg1)
{
	std::cout << msg0 << ' ' << msg1 << '\n';
	exit(EXIT_FAILURE);
}

void DebugBin32Print(unsigned int obj , int counter, std::string msj0, std::string msj1)
{
    if (counter < 2){
        std::bitset<32> x(obj);
        std::cout << msj0 << '\n' << msj1 << x << "\n\n";
    }
}

void DebugBin64Print(unsigned long long obj , int counter, std::string msj0, std::string msj1)
{
    if (counter < 2){
        std::bitset<64> x(obj);
        std::cout << msj0 << '\n' << msj1 << x << "\n\n";
    }
}

void DebugBin8Print(char obj,int counter, std::string msj0, std::string msj1)
{
    if (counter < 2){
        std::bitset<8> x(obj);
        std::cout << msj0 << '\n' << msj1 << x << "\n\n";
    }
}