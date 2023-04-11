/**
	@file
	@author Mark Boady <mwb33@drexel.edu>
	@date 2021-2022
	@section Description

Demo of how to compute a point on the Mandelbrot set.
	
*/
#include <iostream>
#include <complex>

/**
 Mandelbrot Z Function.
 Returns the number of iterations needed to reach 4 or threshold if bounded.
 We use long double for maximum accuracy
 @param x is the x coordinate
 @param y is the y coordinate
 @param t is the threshold
 @return Threshold if point in set otherwise number of iterations to determine not in set
 */
int z(long double x, long double y, int t);

/**
 Test a point to see if it is in the Mandelbrot set
 @param argc is not used
 @param argv is not used
 @return is always 0
 */
int main(int argc, char** argv)
{
	std::cout << "Test a Mandelbrot Point" << std::endl;
	long double x;
	long double y;
	
	std::cout << "Enter x point: " << std::endl;
	std::cin >> x;
	std::cout << "Enter y point: " << std::endl;
	std::cin >> y;
	
	//Do the computation
	int threshold = 100;
	int value = z(x,y,threshold);
	
	std::cout << "Return Value: " << value << std::endl;
	if(value < threshold){
		std::cout << "This point is not in the set." << std::endl;
	}else{
		std::cout << "This point is in the set." << std::endl;
	}
	return 0;
}

//Compute the z function
int z(long double x, long double y, int t){
	//Create Values
	std::complex<long double> c(x,y);
	std::complex<long double> current(0,0);
	//Run Iteratively
	for(int i=1; i < t; i++){
		current = current*current+c;
		if(std::abs(current) > 4){
			return i;
		}
	}
	//Never got above 4
	return t;
}
