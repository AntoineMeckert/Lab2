#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "Traitement.h"
using namespace std;
using namespace cv;

int m_MouseX1 = 0;
int m_MouseY1 = 0;
int m_MouseX2 = 0;
int m_MouseY2 = 0;
int m_SeuilX = 0;
bool m_MSet1 = false;
bool m_MSet2 = true;
Mat m_image;
bool DrawHisto = false;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		if(!m_MSet1)
		{
			m_MouseX1 = x; m_MouseY1 = y;
			m_MSet1 = true; m_MSet2 = false;
			DrawHisto = false;
		}
	}
	else if  ( event == EVENT_LBUTTONUP )
	{
		if(!m_MSet2)
		{
			m_MouseX2 = x; m_MouseY2 = y; 
			m_MSet2 = true; m_MSet1 = false;
			m_SeuilX = abs(m_MouseX2 - m_MouseX1);
			if(m_SeuilX > 255) m_SeuilX = 255;
			DrawHisto = true;
		}

	}

}

int main(int argc, char** argv)
{
	Mat imageSeuil;
	string ImageLocation;
    bool ConvertInGray = false;
	VideoCapture cam(0);
	CTraitement traitement;
	bool IsSobel = false;
	bool IsCircle = false;
	bool IsRectangle = false;
	bool IsCam = false;
	string s;
	cout << "Emplacement de l'image (CAM si camera): \r\n";
	cin >> ImageLocation;

	if (ImageLocation == "CAM" || ImageLocation == "cam")
	{
		IsCam = true;
		if(!cam.isOpened())  // check if we succeeded
			return -1;
	}
	if(ImageLocation == "cercle.png") IsCircle = true;
	if(ImageLocation == "rectangle.png") IsRectangle = true;

	cout << "Charger l'image en gris ? (Y/N)\r\n";
	cin >> s;
	if (s == "Y" || s == "y")
		ConvertInGray = true;
	cout << "Dessiner les contours en blanc? (Y/N)\r\n";
	cin >> s;
	if (s == "Y" || s == "y")
		IsSobel = true;


	
	while(waitKey(1))
	{
		string x1;
		string y1;
		string x2;
		string y2;

		ostringstream posX1s;
		ostringstream posY1s;
		ostringstream posX2s;
		ostringstream posY2s;

		posX1s << m_MouseX1;
		posY1s << m_MouseY1;
		posX2s << m_MouseX2;
		posY2s << m_MouseY2;

		x1 = posX1s.str();
		y1 = posY1s.str();
		x2 = posX2s.str();
		y2 = posY2s.str();

		//Convert To Gray
		if(IsCam)
		{
			cam >> m_image;
			if(ConvertInGray) 
				cvtColor(m_image,m_image,CV_BGR2GRAY);
		}
		else
		{
			if(ConvertInGray) 
				m_image = imread(ImageLocation, CV_BGR2GRAY);
			else
				m_image = imread(ImageLocation, CV_LOAD_IMAGE_COLOR);
		}

		if (m_image.cols != 0 && m_image.rows != 0)
		{
			if(IsCircle) traitement.DrawCross(m_image,50);
			if(IsRectangle) traitement.FindRectangleAngle(m_image, 50);
			putText(m_image,x1 + "-" + y1 + "  " + x2 + "-" + y2,Point(m_image.cols - 150, m_image.rows - 10),FONT_HERSHEY_COMPLEX_SMALL,0.8, Scalar(0,255,0), 1, 8, false );
			imageSeuil = imread("HISTOGRAMME.png",CV_BGR2GRAY);

			if(DrawHisto)
			{ 
				traitement.DrawHistogramme(m_image,imageSeuil,m_SeuilX,m_MouseY1);
				namedWindow("Histogramme",WINDOW_AUTOSIZE);
				imshow("Histogramme", imageSeuil);
			}
			Mat Out;
		
			namedWindow("Image",WINDOW_AUTOSIZE);
			setMouseCallback("Image", CallBackFunc, NULL);
			if(IsSobel)
				imshow("Image", traitement.Sobel(m_image));
			else
				imshow("Image", m_image);
		}
		
		
	}

	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}




