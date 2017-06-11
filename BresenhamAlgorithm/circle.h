#ifndef CIRCLE_H
#define CIRCLE_H

#include "Point.h"

class myCircle
{
public:
	myCircle(int centerW,int centerH,int R);
	~myCircle();
	void drawCircle(Mat &img, Scalar pointColor = Scalar(255, 255, 255));

private:
	int centerH;
	int centerW;
	int R;
};

myCircle::myCircle(int centerW, int centerH, int R)
{
	this->centerW = centerW;
	this->centerH = centerH;
	this->R = R;
}

myCircle::~myCircle()
{
}

void myCircle::drawCircle(Mat &img, Scalar pointColor /*= Scalar(255, 255, 255)*/)
{
	//d = 1.25 - R;//��ʼֵ��Ӱ��Ч��
	//�������㶼����4
	int d = 5 - 4 * R;
	
	int w = 0;
	int h = R;
	while (w <= h)
	{
		myPoint(w + centerW, h + centerH).drawPoint(img, pointColor);
		myPoint(w + centerW, -h + centerH).drawPoint(img, pointColor);
		myPoint(-w + centerW, h + centerH).drawPoint(img, pointColor);
		myPoint(-w + centerW, -h + centerH).drawPoint(img, pointColor);
		myPoint(h + centerW, w + centerH).drawPoint(img, pointColor);
		myPoint(h + centerW, -w + centerH).drawPoint(img, pointColor);
		myPoint(-h + centerW, w + centerH).drawPoint(img, pointColor);
		myPoint(-h + centerW, -w + centerH).drawPoint(img, pointColor);
		
		if (d >= 0)
		{
			d = d + 8 * (w - h) + 20;
			h--;
		}
		else
		{
			d = d + 8 * w + 12;
		}
		w++;
	}
	return;
}
#endif