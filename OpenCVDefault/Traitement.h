#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#pragma once

class CTraitement
{
public:
	CTraitement(void);
	void DrawLine(Mat image,Point p1,Point p2,Scalar color);
	void FindMiddleCircle(Mat image);
	void DrawHistogramme(Mat image, Mat imageSeuil,int SeuilX, int SeuilY);
	Mat Sobel(Mat Image);
	void FindRectangleAngle(Mat Image);
	~CTraitement(void);
};

