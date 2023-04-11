/**
 @file
 @author Frederick Patterson <fjp39@drexel.edu>
 @date 2021-2022
 @section Description
 
 Implementation of sort unit testing library.
 */


#include <stdio.h> //For printf, malloc, and free
#include <stdlib.h> //For rand
#include <iostream>
//Functions to help test sorts

//--You may not change this file--

#include <cmath>
#include "mergesort.h"

void mergesort(int* A, int size)
{
    //Call the starting function
	msortSec(A,0, size - 1);
 }


void msortSec(int* A, int start, int stop){
    //If only 1 item in the array then return
	if (start >= stop){
		return;
    }
    else{
    //Get the middle value of the array
	int middle = start + floor((stop-start)/2);
    //Recursivly split the array
	msortSec(A,start,middle);
	msortSec(A,middle+1,stop);
    //Merge based on sub array
	merge(A,start,middle,stop);
    }
}


void merge(int* A, int start, int middle, int stop){
	//Create a new Array Aux with stop-start+1 spaces
	//Copy elements from A to Aux 
    int* Aux;
    Aux =  new int[stop - start + 1];
    for (int x = 0; x < (stop - start + 1); x++){
	    Aux[x] = A[start + x];
    }
    //Create the middle end and start values of Aux
	int auxMiddle =(middle-start);
    int auxStop=(stop-start);
	int i=0;
    int j = auxMiddle+1;
    //Do the merge
	for(int k=start; k < stop + 1; k++){
		if( i > auxMiddle){
			A[k] = Aux[j]; j++;
            }
		else if(j > auxStop){
			A[k]=Aux[i]; i++;
            }
		else if(Aux[j] > Aux[i]){
			A[k] = Aux[i]; i++;
        }
		else{
			A[k] = Aux[j]; j++;
        }
    }
}


