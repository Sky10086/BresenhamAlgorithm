#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <time.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include<vector>;
#include<math.h>
#include<stack>
#include <string>
#include <assert.h> 
#include <algorithm>
using namespace std;
using namespace cv;

class myPoint
{
public:
	//使用的是opencv的图片坐标系，这里使用w和h更容易理解
	myPoint(int w,int h);
	myPoint(){  };
	~myPoint();
	void drawPoint(CV_IN_OUT Mat& img, Scalar pointColor = Scalar(255, 255, 255));
	int getW();
	int getH();
	void setWH(int w,int h);
private:
	int w;
	int h;
};

myPoint::myPoint(int w,int h)
{
	this->w = w;
	this->h = h;
}

myPoint::~myPoint()
{
}

void myPoint::drawPoint(CV_IN_OUT Mat& img, Scalar pointColor /*= Scalar(255, 255, 255)*/)
{
	circle(img, Point(this->w, this->h), 0, pointColor);
}

int myPoint::getW()
{
	return this->w;
}

int myPoint::getH()
{
	return this->h;
}

void myPoint::setWH(int w, int h)
{
	this->w = w;
	this->h = h;
}
#endif