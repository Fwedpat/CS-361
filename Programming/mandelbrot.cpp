/**
 @mainpage CS361 - Mandelbrot Program
 @section Description
 
 This is a CPP program that performs a calculation of the mandelbrot set and creates a 1500x1500 bitmap of the results.
 
 To build the code you can use make. You have the following targets available:
 
 Build the program
	
	 make
 
 
 Clean up all the compiled code to start from scratch	
 
	 make clean
 
 
 To run the program
 
	 ./bin/mandelbrot [x and y values along with name of file]
 
 @section Notes
 This is not designed for production use, it is an educational example.
 */

/**
	@file
	@author Frederick Patterson <fj39@drexel.edu>
	@date October 10, 2022
	@section Description
 
 	This is a CPP program that performs a calculation of the mandelbrot set and creates a 1500x1500 bitmap of the results.
 */

#include <stdlib.h>
//Header for useful methods
#include <iostream>
//For input and output
#include <cstdlib>
//For useful methods
#include <cmath>
//For math functions like absolute
#include <complex>
//For complex numebers
#include <fstream>
//For easy file names
#include <string>
//Add Threads
#include <thread>

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
 Write Header of a Bitmap Image
 @param image is a reference to the file containing our image
 @param height is number of pixels tall the image is
 @param width is the number of pixels wide the image is
*/
void writeHeader(std::ofstream &image, int height, int width);
/**
 Write DIB header of a Bitmap Image. The DIB is metadata for the image.
 @param image is a reference to the file we are writing into
 @param height is number of pixels tall the image is
 @param width is the number of pixels wide the image is
 @param dpi is the dots per inch of the image when printed
*/
void writeDIB(std::ofstream &image, int height, int width, int dpi);
/**
 Write Pixels from a collection of arrays. All three arrays must be the same size.
 
 @param image is a reference to the file we are writting into
 @param red is an array storing the red component of each pixel
 @param green is an array storing the green component of each pixel
 @param blue is an array storing the blue component of each pixel
 @param pixels is the number of elements in the array
*/
void writePixels(std::ofstream &image,
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int pixels);

/**
 Take an integer (32 bit) and break it up into bytes (8bit).
 @param value is the integer to break up
 @param byte is which byte you want (0 to 4)
 @return the byte requested as a char
 */
char getByte(int value, int byte);
/**
 Convert Dots Per Inch to Pixels Per Meter. Both are useful formats, but dpi is more commom. Bitmaps use PPM.
 @param dpi is the dots per inch
 @return equivelant measurement in pixels per meter
 */int dpiToPPM(int dpi);


/**
 Given three arrays (one per color), color in a section with a specific shade of grey.
 @param red is the array of red pixels
 @param green is the array of green pixels
 @param blue is the array of blue pixels
 @param start is the first pixel to color
 @param stop is the last pixel to color
 @param grey is the shade this thread is in charge of
 */
void colorThread(
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int start,
	int stop,
	unsigned char grey,
	long double x1,
	long double x2,
	long double y1,
	long double y2,
	long double ydif
	);

/**
 Generate the bitmap using threads to fill in the array
 @param argc is not used
 @param argv is not used
 @return Is always 0
 */



int main(int argc, char** argv){
	//Flush cout just incase
	std::cout.flush();
	//Set values for the image output
	int height = 1500;//pixels
	int width = 1500;//pixels
	int dpi = 150;//pixel per inch
	std::string filename= std::string(argv[5]) + ".bmp"; //Filename
	
	//Open File
	std::ofstream image(filename,std::ios::binary);
	//Make the Bitmap Header
	writeHeader(image, height, width);
	writeDIB(image,height,width,dpi);
	
	//Arrays to Store the Three Colors of a Pixel
	int pixels = height*width;
	unsigned char* red = new unsigned char[pixels];
	unsigned char* green = new unsigned char[pixels];
	unsigned char* blue = new unsigned char[pixels];
	
	//Load in the x and y values of the mandelbrot
	long double x1 = atof(argv[1]);
	long double x2 = atof(argv[2]);
	long double y1 = atof(argv[3]);
	long double y2 = atof(argv[4]);
	//Break into 4 for each thread to know where to stop
	long double quater = abs(y2 - y1)/4;

	//Color the Image with 4 threads
	int stepSize = pixels/4;
	std::thread* myThreads = new std::thread[4];
	//First Section gets first quarter
	myThreads[0] = std::thread(colorThread,red,green,blue,0,stepSize,0,x1,x2,y1,y2,abs(y2 - y1)/1500);
	//Second Section gets second quater
	myThreads[1] = std::thread(colorThread,red,green,blue,stepSize+1,2*stepSize,150,x1,x2,y1 + quater + abs(y1 - y2)/1500 ,y2,abs(y2 - y1)/1500);
	//Third Section gets third quater
	myThreads[2] = std::thread(colorThread,red,green,blue,2*stepSize+1,3*stepSize,200,x1,x2, y1 + 2*quater + abs(y1 - y2)/1500 ,y2,abs(y2 - y1)/1500) ;
	//Fourth Section gets last quater
	myThreads[3] = std::thread(colorThread,red,green,blue,3*stepSize+1,pixels,255,x1,x2,y1+quater*3 + abs(y1 - y2)/1500,y2,abs(y2 - y1)/1500);
	
	for(int i=0; i < 4; i++){
		myThreads[i].join();
	}
	
	//Write the pixels to file
	writePixels(image, red, green, blue, pixels);
	//Close the File
	image.close();
	
	//Exit the Program
	std::cout <<"Made Image " << filename << std::endl;
	return 0;
}


int z(long double x, long double y, int t){
	//Create Values
	int threshold = 100;
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

char getByte(int value, int byte)
{
	int newValue = value;
	
	unsigned char rem;
	for(int i=0; i <= byte; i++)
	{
		rem = static_cast<unsigned char>( newValue%256 );
		newValue = newValue/256;
	}
	return rem;
}

//Convesion
//x pixels/inches * C inches/meter = y pixels/meter
int dpiToPPM(int dpi)
{
	float inchesPerMeter = 39.3701/1;
	float convert = dpi*inchesPerMeter;
	return static_cast<int>(convert);
}

void writeHeader(std::ofstream &image, int height, int width)
{
	//How many pixel does the image have
	int pixels = height*width;
	//Make the header. It is always 14 bytes
	int headerSize = 14;
	//Array to store the header
	char* header = new char[headerSize];
	//The header is 14 Bytes
	//The DIB is 40 bytes
	int offset = headerSize + 40;
	//Each Pixel is another 3 bytes
	int totalBits = pixels*3+offset;
	//Make the Header
	//First 2 Bytes are BM for bitmap
	header[0] = 'B';
	header[1] = 'M';
	//Next 4 bytes are the total size of the file
	header[2] = getByte(totalBits,0);
	header[3] = getByte(totalBits,1);
	header[4] = getByte(totalBits,2);
	header[5] = getByte(totalBits,3);
	//Next for bits are 0 (reserved for other uses)
	header[6] = 0;
	header[7] = 0;
	header[8] = 0;
	header[9] = 0;
	//Last 4 bytes are offset
	//Where do the pixels start
	header[10] = getByte(offset,0);
	header[11] = getByte(offset,2);
	header[12] = getByte(offset,2);
	header[13] = getByte(offset,3);
	//Write the Header to the file in binary
	image.write(header, headerSize);
	//Exit the Function
	return;
}

//Using the BITMAPINFOHEADER standard
void writeDIB(std::ofstream &image, int height, int width, int dpi)
{
	//Convert DPI to Pixels Per Meter
	int resolution = dpiToPPM(dpi);
	//Fixed Size of 40 Bytes
	int sizeDIB = 40;
	//Make array of bytes
	char* DIB = new char[sizeDIB];
	//Set Values
	//First 4 bytes are header size of this header (40)
	DIB[0] = getByte(40,0);
	DIB[1] = getByte(40,1);
	DIB[2] = getByte(40,2);
	DIB[3] = getByte(40,3);
	//Bitmap Width (4 bytes)
	DIB[4] = getByte(width,0);
	DIB[5] = getByte(width,1);
	DIB[6] = getByte(width,2);
	DIB[7] = getByte(width,3);
	//Height (4 bytes)
	DIB[8] = getByte(height,0);
	DIB[9] = getByte(height,1);
	DIB[10] = getByte(height,2);
	DIB[11] = getByte(height,3);
	//Color Plane (2 bytes) is always 1
	DIB[12] = 1;
	DIB[13] = 0;
	//Color Depth (2 bytes) we are using 24 (three 8 bit colors)
	DIB[14] = getByte(24,0);
	DIB[15] = getByte(24,1);
	//Compression (4 bytes) 0 means none
	DIB[16] = 0;
	DIB[17] = 0;
	DIB[18] = 0;
	DIB[19] = 0;
	//Uncompressed Size (4 bytes)
	//0 because we aren't using compression
	DIB[20] = 0;
	DIB[21] = 0;
	DIB[22] = 0;
	DIB[23] = 0;
	//Horizontal Resolution (4 bytes)
	//Pixel per meter
	DIB[24] = getByte(resolution, 0);
	DIB[25] = getByte(resolution, 1);
	DIB[26] = getByte(resolution, 2);
	DIB[27] = getByte(resolution, 3);
	//Vertical Resolution (4 bytes)
	//Pixel per meter
	DIB[28] = getByte(resolution, 0);
	DIB[29] = getByte(resolution, 1);
	DIB[30] = getByte(resolution, 2);
	DIB[31] = getByte(resolution, 3);
	//Color Pallet (4 bytes)
	//0 means all
	DIB[32] = 0;
	DIB[33] = 0;
	DIB[34] = 0;
	DIB[35] = 0;
	//Number of important colors
	//0 mean all equal
	DIB[36] = 0;
	DIB[37] = 0;
	DIB[38] = 0;
	DIB[39] = 0;
	//Write the Header to the file in binary
	image.write(DIB, sizeDIB);
	//Exit the Function
	return;
}

void writePixels(std::ofstream &image,
	unsigned char* red,
	unsigned char* green,
	unsigned char* blue,
	int pixels)
{
	//For each pixel set the color of the different color values
	char* pixel = new char[3];
	for(int i=0; i < pixels; i++)
	{
		pixel[2] = red[i];
		pixel[1] = green[i];
		pixel[0] = blue[i];
		image.write(pixel, 3);
	}
	return;
}

int getColour(int value){
//If value is in the set, make it black
if (value == 100) {
	int color = 0;
	return color;
}
//All values progressing downwards are given a level of grey
else if (value < 100 && value > 40 ){
	int color = 80;
	return color;
}
else if (value < 40 && value > 20 ){
	int color =170;
	return color;
}
else if (value < 40 && value > 20 ){
	int color =180;
	return color;
}
else if (value < 20 && value > 3 ){
	int color =246;
	return color;}
else if (value < 3 && value > 0 ){
	int color =248;
	return color;}
//If close to the set at all give value of exactly white
else{
	int color = 255;
	return color;
}
}


void colorThread( unsigned char* red,unsigned char* green, unsigned char* blue, int start,
    int stop, unsigned char grey, long double x1,long double x2, long double y1,long double y2, long double ydif
	) 
{
	//Scales x incrament by 1500
	long double xdif = abs(x1 - x2)/1500;
	//Keep track of current line
	int lines = 0;

	for(int i=start; i < stop; i++)
	{
		int color;
		//Calculate current point in the mandelbrot set
		int value = z((x1+i%1500*xdif),y1+lines*ydif,100);
		//Set color of pixel based on value returned from the mandelbrot set
		color = getColour(value);
		red[i] = color;
		green[i] = color;
		blue[i] = color;
		//If the current pixel is a mod of 1500 then move onto the next line
		if ((i % 1500) == 0){
			lines = lines + 1;
		}
	}
}
