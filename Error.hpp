/*Error handling.*/

#include<string>

/*Displays an error message and terminates the 
	program.*/
void error(std::string msg0, std::string msg1 = "");
void DebugBin8Print(char obj,int counter, std::string msj0, std::string msj1);
void DebugBin32Print(unsigned int obj,int counter, std::string msj0, std::string msj1);
void DebugBin64Print(unsigned long long obj , int counter, std::string msj0, std::string msj1);