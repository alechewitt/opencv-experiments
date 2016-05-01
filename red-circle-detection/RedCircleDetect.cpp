#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "Usage: ./RedCircleDetect image_circles_path" << std::endl;
		std::exit(-1);
	}

	// Load image
	string path_image = argv[1];
	cv::Mat bgr_image = cv::imread(path_image);

	cv::Mat orig_image = bgr_image.clone();

	cv::medianBlur(bgr_image, bgr_image, 3);

	// Convert image to HSV
	cv::Mat hsv_image;
	cv::cvtColor(bgr_image, hsv_image, cv::COLOR_BGR2HSV);

	// Threshold the HSV image in order to keep only the red pixels
	cv::Mat lower_red_hue_range;
	cv::Mat upper_red_hue_range;
	cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
	cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), upper_red_hue_range);

	// Lets combine the two images
	cv::Mat red_hue_image;
	cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

	//Add a Gaussian Blur
	cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

	std::vector<cv::Vec3f> circles;
	cv::HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if(circles.size() == 0) std::exit(-1);
	for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) {
		cv::Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		int radius = round(circles[current_circle][2]);

		cv::circle(orig_image, center, radius, cv::Scalar(0, 255, 0), 5);
	}

	// Show images
	cv::namedWindow("Lower Red Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Lower Red Image", lower_red_hue_range);

	cv::namedWindow("Upper Red Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Upper Red Image", upper_red_hue_range);

	cv::namedWindow("Original Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Original Image", orig_image);

	cv::namedWindow("Merged", cv::WINDOW_AUTOSIZE);
	cv::imshow("Merged", red_hue_image);

	cv::namedWindow("With Blur", cv::WINDOW_AUTOSIZE);
	cv::imshow("With Blur", bgr_image);



	cv::waitKey(0);
}
