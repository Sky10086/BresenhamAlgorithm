#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Point.h"

/*
椭圆方程为：

w*w / (longAxis*longAxis) + h*h / (shortAxis*shortAxis)  = 1;

*/

class myElipse
{
public:
	myElipse(int longAxis,int shortAxis,int centerW,int centerH);
	~myElipse();
	void drawEllipse(Mat &img, Scalar pointColor = Scalar(255, 255, 255));

private:
	int longAxis;
	int shortAxis; 
	int centerW;
	int centerH;
};

myElipse::myElipse(int longAxis, int shortAxis, int centerW, int centerH)
{
	this->longAxis = longAxis;
	this->shortAxis = shortAxis;
	this->centerW = centerW;
	this->centerH = centerH;
}

myElipse::~myElipse()
{
}

void myElipse::drawEllipse(Mat &img, Scalar pointColor /*= Scalar(255, 255, 255)*/)
{
	//初始的d
	int shortAxis2 = shortAxis*shortAxis;
	int shortAxis2_2 = shortAxis*shortAxis*2;
	int longAxis2 = longAxis*longAxis;
	int longAxis2_2 = longAxis*longAxis * 2;
	int d = shortAxis2_2 + longAxis2 - shortAxis*longAxis2_2 ;
	int w = 0;
	int h = shortAxis;
	int upEnd = int(float(longAxis2) / (float)std::sqrt(longAxis2 + shortAxis2));
	while (w < upEnd)
	{
		myPoint(w + centerW, h + centerH).drawPoint(img, pointColor);
		myPoint(w + centerW, -h + centerH).drawPoint(img, pointColor);
		myPoint(-w + centerW, h + centerH).drawPoint(img, pointColor);
		myPoint(-w + centerW, -h + centerH).drawPoint(img, pointColor);
		if (d >= 0)
		{
			d = d + shortAxis2_2*(2 * w + 3) + longAxis2_2*(2 - 2 * h);
			h--;
		}
		else
		{
			d = d + shortAxis2_2*(2 * w + 3);
		}
		w++;

	}

	d = shortAxis2 * (w * w + w) + longAxis2 * (h * h - h) - longAxis2 * shortAxis2;

	while (h >= 0)
	{
		myPoint(w + centerW, h + centerH).drawPoint(img, pointColor);
		myPoint(w + centerW, -h + centerH).drawPoint(img, pointColor);
		myPoint(-w + centerW, h + centerH).drawPoint(img, pointColor);
		myPoint(-w + centerW, -h + centerH).drawPoint(img, pointColor);
		h--;
		if (d >= 0)
		{
			d = d - longAxis2_2 * h - longAxis2;
		}
		else
		{
			w++;
			d = d - longAxis2_2 * h - longAxis2 + shortAxis2_2*w + shortAxis2_2;
		}
	}
}
#endif