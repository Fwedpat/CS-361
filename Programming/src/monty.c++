/**
 @mainpage CS361 - Monty Program
 @section Description
 
 This is a CPP program that performs 'Lets make a deal' test.
 
 To build the code you can use make. You have the following targets available:
 
 Build the program
	
	 make
 
 
 Clean up all the compiled code to start from scratch
 
	 make clean
 
 Run a test version of the program with twelve lines
 
	 make run
 
 To run the program
 
	 ./bin/monty [number of tests for the game]
 
 @section Notes
 This is not designed for production use, it is an educational example.
 */

/**
	@file
	@author Frederick Patterson <fj39@drexel.edu>
	@date September 22, 2022
	@section Description
 
	The main program for the Monty Program. This file is designed to show me how to work with C++.
 */

#include <iostream>

#include <cstdlib>

#include <cmath>
//Set up global variables for total switches and stays
int totalSwitch = 0;
int totalStay = 0;

int Game(){
    char* doors[3]
        = { "C", "G", "G" };
	//randomize the doors locations
	for (int i = 0; i < 3; i++){
    int j = rand() % 3;
	char* temp = doors[i];
	doors[i] = doors[j];
	doors[j] = temp;
		}
	//Pick a random door and see if its right
	int randNum = rand()%(2-0 + 1) + 0;
	if(doors[randNum] == "C"){
		totalStay = totalStay + 1;
	}
	else{
		totalSwitch = totalSwitch + 1;
	}
}


int main(int argc, char** argv){
	std::cout.flush();
	if (argc < 2){
		std::cout << "Monty Hall Problem Simulator" << std::endl;
		std::cout << "Usage: monty num_tests" << std::endl;
	}
	else {
	//Set the random seed.
	srand(time(0));
	//Make sure cout is clear
	std::cout.flush();
	//convert args
	long conv = strtol(argv[1], NULL, 10);
	//Test if a valid number
	if (conv == 0){
		std::cout << "Number of tests is not a number" << std::endl;
	}
	else {
	//Run the tests and report back findings
	for (int i = 0; i < conv; i++){
	Game(); 
	}
	std::cout << "Switch would win " << ((float)totalSwitch/(float)conv) << " percent of experiments." << std::endl;
	std::cout << "Stay would win " << ((float)totalStay/(float)conv) << " percent of experiments." << std::endl;
    return 0;
	}
	}
}





