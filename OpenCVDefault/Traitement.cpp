#include "stdafx.h"
#include "device_launch_parameters.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "Traitement.h"
#include <math.h>  
using namespace std;
using namespace cv;

CTraitement::CTraitement(void)
{

}

void CTraitement::DrawLine(Mat image,Point p1,Point p2,Scalar color)
{
	line(image, p1, p2,color, 1, 8, 0);
}

void CTraitement::FindMiddleCircle(Mat image)
{
	int xDroite = 0;
	int xGauche = 1000;
	int yHaut = 1000;
	int yBas = 0;

	for( int y = 0; y < image.rows; y++ ) 
	{
		for( int x = 0; x < image.cols; x++ ) 
		{
			if ( image.at<Vec3b>(y,x) == cv::Vec3b(0,0,0) ) 
			{
				if(x > xDroite)
					xDroite = x;

				if(x < xGauche)
					xGauche = x;

				if(y > yBas)
					yBas = y;

				if(y < yHaut)
					yHaut = y;
			} 
		}
	}

	line(image,Point((xDroite + xGauche)/2 - 5,(yBas + yHaut)/2),Point((xDroite + xGauche) /2 + 5,(yBas + yHaut)/2),Scalar(255,255,255),1,8,0);
	line(image,Point((xDroite + xGauche)/2 ,(yBas + yHaut)/2 - 5),Point((xDroite + xGauche) /2,(yBas + yHaut)/2 + 5),Scalar(255,255,255),1,8,0);
}

void CTraitement::DrawHistogramme(Mat image, Mat imageSeuil,int SeuilX, int SeuilY)
{
	DrawLine(imageSeuil,Point(0,imageSeuil.rows - SeuilX),Point(imageSeuil.cols,imageSeuil.rows - SeuilX),Scalar(0,0,0));
	string x;

	ostringstream posX1s;

	posX1s << SeuilX;

	x = posX1s.str();
	
	putText(imageSeuil,x,Point(0,imageSeuil.rows - SeuilX),FONT_HERSHEY_COMPLEX_SMALL,0.8, Scalar(0,255,0), 1, 8, false );
	string y2;


	int lastx = 0;
	int lasty = 255;

	Mat grayHisto;
	cvtColor(image,grayHisto,CV_BGR2GRAY);
	for(int x = 0; x < grayHisto.cols;x++)
	{
		uchar Color = grayHisto.at<uchar>(Point(lastx,SeuilY));
		imageSeuil.rows;
		uchar PosY = imageSeuil.rows - Color;




		DrawLine(imageSeuil,Point(lastx,lasty),Point(x,PosY),Scalar(0,0,0));
		lastx = x;
		lasty = PosY;
	}
}

Mat CTraitement::Sobel(Mat Image)
{
	int Edge = 8;

	int Color;
	int TopColor;
	int LeftColor;

	Mat Result;
	
	cvtColor(Image,Result,CV_BGR2GRAY);
	cvtColor(Image,Image,CV_BGR2GRAY);
	for(int y = 1; y< Image.rows -1 ;y++)
	{
		for(int x = 1; x< Image.cols - 1 ; x++)
		{
			Color = Image.at<uchar>(Point(x, y));
			TopColor = Image.at<uchar>(Point(x, y - 1));
			LeftColor = Image.at<uchar>(Point(x - 1, y));

			if(abs(Color - TopColor) > Edge || abs(Color - LeftColor) > Edge)
			{
				// pixel blanc
				Result.at<uchar>(Point(x, y)) = 255;
			}
			else
			{
				// pixel noir
				Result.at<uchar>(Point(x, y)) = 1;
			}
		}
	}
	return Result;
}

void CTraitement::FindRectangleAngle(Mat Image)
{
	int xDroite = 0;
	int xGauche = 1000;
	int yHaut = 1000;
	int yBas = 0;

	for( int y = 0; y < Image.rows; y++ ) 
	{
		for( int x = 0; x < Image.cols; x++ ) 
		{
			if ( Image.at<Vec3b>(y,x) != cv::Vec3b(255,255,255) ) 
			{
				if(x > xDroite)
					xDroite = x;

				if(x < xGauche)
					xGauche = x;

				if(y > yBas)
					yBas = y;

				if(y < yHaut)
					yHaut = y;
			} 
		}
	}

	Point Haut;
	Point Gauche;



	//Trouver le coin a droite et a gauche
	for(int i = 0; i< Image.rows;i++)
	{
		if ( Image.at<Vec3b>(Point(xDroite,i)) != cv::Vec3b(255,255,255) )
		{
			line(Image,Point(xDroite - 10,i),Point(xDroite + 10,i),Scalar(255,255,255),1,8,0);
			line(Image,Point(xDroite,i - 10),Point(xDroite,i + 10),Scalar(255,255,255),1,8,0);
		}

		if ( Image.at<Vec3b>(Point(xGauche,i)) != cv::Vec3b(255,255,255) )
		{
			line(Image,Point(xGauche - 10,i),Point(xGauche + 10,i),Scalar(255,255,255),1,8,0);
			line(Image,Point(xGauche,i - 10),Point(xGauche,i + 10),Scalar(255,255,255),1,8,0);

			Gauche.x = xGauche;
			Gauche.y = i;
		}
	}

	//Trouver le coin en bas et en haut
	for(int i = 0; i< Image.cols;i++)
	{
		if ( Image.at<Vec3b>(Point(i,yHaut)) != cv::Vec3b(255,255,255) )
		{
			line(Image,Point(i - 10,yHaut),Point(i + 10,yHaut),Scalar(255,255,255),1,8,0);
			line(Image,Point(i,yHaut - 10),Point(i,yHaut + 10),Scalar(255,255,255),1,8,0);
			Haut.x = yHaut;
			Haut.y = i;
		}

		if ( Image.at<Vec3b>(Point(i,yBas)) != cv::Vec3b(255,255,255) )
		{
			line(Image,Point(i - 10,yBas),Point(i + 10,yBas),Scalar(255,255,255),1,8,0);
			line(Image,Point(i,yBas - 10),Point(i,yBas + 10),Scalar(255,255,255),1,8,0);

		}
	}


	float angle = cv::fastAtan2(abs(Haut.y - Gauche.y),abs(Haut.x - Gauche.x));


	string s;

	ostringstream osAngle;

	osAngle << angle;

	s = osAngle.str();

	putText(Image,s,Point(0,50),FONT_HERSHEY_COMPLEX_SMALL,0.8, Scalar(0,255,0), 1, 8, false );
	

	line(Image,Point((xDroite + xGauche)/2 - 5,(yBas + yHaut)/2),Point((xDroite + xGauche) /2 + 5,(yBas + yHaut)/2),Scalar(255,255,255),1,8,0);
	line(Image,Point((xDroite + xGauche)/2 ,(yBas + yHaut)/2 - 5),Point((xDroite + xGauche) /2,(yBas + yHaut)/2 + 5),Scalar(255,255,255),1,8,0);
}

CTraitement::~CTraitement(void)
{
}
