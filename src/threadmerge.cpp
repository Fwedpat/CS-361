/**
 @file
 @author Frederick Patterson <fjp39@drexel.edu>
 @date 2021-2022
 @section Description
 
 Implementation of sort unit testing library.
 */

#include <iostream>
#include <stdio.h> //For printf, malloc, and free
#include <stdlib.h> //For rand
#include <thread>
#include <mutex>
//Functions to help test sorts

//--You may not change this file--

#include <cmath>
#include "mergesort.h"
//Declare variable and lock
int tnum = 0;
std::mutex m;


void tmsortSec(int* A, int numThreads, int size){
    //Lock the thread value so that it is know to be the correct thread
    m.lock();
    int threadnum = tnum++;
    m.unlock();
    //Calculate the start, middle and stop values
    int low = threadnum * (size/numThreads);
    int high = (threadnum + 1) * (size /numThreads) - 1;
    int mid = low + (high - low) / 2;
    if (low < high) {
        //Recursivly split the array
        msortSec(A, low, mid);
        msortSec(A, mid + 1, high);
        //Merge based on sub array
        merge(A, low, mid, high);
    }
    return;
}

void tmergeArray(int arr[], int num, int agg, int size, int ThreadPerNum ) {
    //Merge by the amount of thread spits, and then call again with smaller groups
    //Setout variables
    for(int i = 0; i < num; i = i + 2) {
        int low = i * (ThreadPerNum * agg);
        int high = ((i + 2) * ThreadPerNum * agg) - 1;
        int middle = low + ( ThreadPerNum * agg) - 1;
        if (high >= size) {
            high = size - 1;
        }
        //Merge on variables
        merge(arr, low, middle, high);
    }
    if (num / 2 >= 1) {
        tmergeArray(arr, num / 2, agg * 2, size, ThreadPerNum);
    }
}

void tmergesort(int* A, int size, int numThreads){
    tnum = 0;
    //If only 1 thread then run regular sort
	if(numThreads == 1 or size < numThreads){
		mergesort(A,size);
		return;
    }
    else{
    //Create array of threads based on the numThreads
    std::thread* myThreads = new std::thread[numThreads];
    //Have each thread start the threaded merge sort
    for(int i = 0; i < numThreads; i++){
        myThreads[i] = std::thread(tmsortSec, A, numThreads, size); 
    }
    //Have the threads join the runtime
    for(int x=0; x < numThreads; x++){
		myThreads[x].join();
	}
    if(numThreads < 16){
    //If threads 4 - 1 then merge only on the quarters then the full
    merge(A,0, (size / 2 - 1) / 2, size / 2 - 1);
    merge(A,size / 2, size/2 + (size-1-size/2)/2, size - 1);
    merge(A,0, (size - 1)/2, size - 1);
    }
    else{
    //Merge by threads size to rebuild array
    tmergeArray(A, numThreads, 1, size, size/numThreads);
    }


}
}
