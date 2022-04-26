#ifndef V_POINT.h
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <gsl/gsl_fit.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;

Mat vanishing_point(Mat frame);
Mat canny(Mat frame);

#endif