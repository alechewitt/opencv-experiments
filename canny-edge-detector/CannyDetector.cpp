#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
const char* window_name = "Edge Map";


static void CannyThreshold(int, void*) {

	// Reduce noide with a kernel 3x3
	blur(src_gray, detected_edges, Size(3,3));

	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	// Fill dst image with all 0 (meaing it is black)
	dst = Scalar::all(255);

	src.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}


int main (int, char** argv) {
	// Load the image
	src = imread(argv[1]);
	if (src.empty()) {
		return -1;
	}

	// Create matrix of the same type and size as src
	dst.create(src.size(), src.type());

	// Convert the image to grayscale
	cvtColor(src, src_gray, COLOR_BGR2GRAY);

	// Create a window
	namedWindow(window_name, WINDOW_AUTOSIZE);

	// Create Trackbar for user to enter threshold
	createTrackbar("Min threshold: ", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

	// Show the image
	CannyThreshold(0, 0);

	// Show the original image
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", src);

	waitKey(0);

	return 0;
}



