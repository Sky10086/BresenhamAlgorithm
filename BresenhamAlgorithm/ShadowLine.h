#ifndef SHADOWLINE_H
#define SHADOWLINE_H

#include "Polygon.h"


class myShadowLine
{
public:
	myShadowLine(float alpha,int dieH);
	~myShadowLine();
	float getAlpha();
	int getDisH();
	vector<float> minIntercept;//较小的截距数组
	vector<float> maxIntercept;//较大的截距数组

	vector<myPoint>  intersectionPoints;//阴影线与棱边的交点数组
	float Bmax;//多边形结合阴影线斜率计算得到的最大截距
	float Bmin;//多边形结合阴影线斜率计算得到的最小截距
	void drawShadowLines(Mat &img, Scalar pointColor = Scalar(255, 255, 255));
private:
	float alpha;//阴影线斜率
	int disH;//相邻阴影线的垂直间隔h
	myPolygon polygon;
	void getIntercept();//获取多边形每个边两个顶点加上阴影线斜率构成直线的截距
	void getIntersectionPoints(Mat &img, Scalar pointColor);//获取阴影线与棱边的交点

};

myShadowLine::myShadowLine(float alpha, int disH)
{
	this->alpha = alpha;
	this->disH = disH;
	this->polygon = myPolygon();
}

myShadowLine::~myShadowLine()
{
}

float myShadowLine::getAlpha()
{
	return alpha;
}
int myShadowLine::getDisH()
{
	return disH;
}

void myShadowLine::getIntercept()
{
	float tanA = tan(alpha);
	float b1 = 0.0f;
	float b2 = 0.0f;
	for (int i = 0; i < polygon.outPoints.size(); i++)
	{
		b1 = polygon.outPoints[i].getH() - polygon.outPoints[i].getW()*tanA;
		b2 = polygon.outPoints[(i + 1)%polygon.outPoints.size()].getH() - 
			polygon.outPoints[(i + 1) % polygon.outPoints.size()].getW()*tanA;
		minIntercept.push_back(b1 < b2 ? b1 : b2);
		maxIntercept.push_back(b1 < b2 ? b2 : b1);

	}
	for (int i = 0; i < polygon.inerPoints.size(); i++)
	{
		b1 = polygon.inerPoints[i].getH() - polygon.inerPoints[i].getW()*tanA;
		b2 = polygon.inerPoints[(i + 1) % polygon.inerPoints.size()].getH() -
			polygon.inerPoints[(i + 1) % polygon.inerPoints.size()].getW()*tanA;
		minIntercept.push_back(b1 < b2 ? b1 : b2);
		maxIntercept.push_back(b1 < b2 ? b2 : b1);
	}
	Bmax = maxIntercept[0];
	Bmin = minIntercept[0];
	for (int i = 0; i < maxIntercept.size();i++)
	{
		if (maxIntercept[i] >Bmax)
		{
			Bmax = maxIntercept[i];
		}
	}
	for (int i = 0; i < minIntercept.size(); i++)
	{
		if (minIntercept[i] < Bmin)
		{
			Bmin = minIntercept[i];
		}
	}
	//sort(minIntercept.begin(), minIntercept.end());
}


bool compPoints(myPoint p1, myPoint p2)
{
	return p1.getW() < p2.getW();
}

void myShadowLine::getIntersectionPoints(Mat &img, Scalar pointColor)
{
	float disB = disH / cos(alpha);
	float k = tan(alpha);
	for (float B = Bmin; B < Bmax;B += disB)
	{
		for (int i = 0; i < polygon.outPoints.size(); i++)
		{
			int dH = polygon.outPoints[(i) % polygon.outPoints.size()].getH() - polygon.outPoints[(i + 1) % polygon.outPoints.size()].getH();
			int dW = polygon.outPoints[(i) % polygon.outPoints.size()].getW() - polygon.outPoints[(i + 1) % polygon.outPoints.size()].getW();
			if (B > minIntercept[i] && B < maxIntercept[i] && 
				 (dH - tan(alpha)*dW != 0))
			{
				int w1 = polygon.outPoints[(i) % polygon.outPoints.size()].getW();
				int h1 = polygon.outPoints[(i) % polygon.outPoints.size()].getH();
				int w2 = polygon.outPoints[(i + 1) % polygon.outPoints.size()].getW();
				int h2 = polygon.outPoints[(i + 1) % polygon.outPoints.size()].getH();
				int jiaoW = (w1*h2 - w2*h1 + B*(w2 - w1)) / ((h2 - h1) - tan(alpha)*(w2 - w1));
				int jiaoH = k*jiaoW + B;
				intersectionPoints.push_back(myPoint(jiaoW, jiaoH));
			}
		}

		for (int i = 0; i < polygon.inerPoints.size(); i++)
		{
			int dH = polygon.inerPoints[(i) % polygon.inerPoints.size()].getH() - polygon.inerPoints[(i + 1) % polygon.inerPoints.size()].getH();
			int dW = polygon.inerPoints[(i) % polygon.inerPoints.size()].getW() - polygon.inerPoints[(i + 1) % polygon.inerPoints.size()].getW();
			if (B > minIntercept[i + polygon.outPoints.size()] && B < maxIntercept[i+polygon.outPoints.size()] &&
				(dH - tan(alpha)*dW != 0))
			{
				int w1 = polygon.inerPoints[(i) % polygon.inerPoints.size()].getW();
				int h1 = polygon.inerPoints[(i) % polygon.inerPoints.size()].getH();
				int w2 = polygon.inerPoints[(i + 1) % polygon.inerPoints.size()].getW();
				int h2 = polygon.inerPoints[(i + 1) % polygon.inerPoints.size()].getH();
				int jiaoW = (w2*h1 - w1*h2 + B*(w1 - w2)) / ((h1 - h2) - tan(alpha)*(w1 - w2));
				int jiaoH = k*jiaoW + B;
				intersectionPoints.push_back(myPoint(jiaoW, jiaoH));
			}
		}

		sort(intersectionPoints.begin(), intersectionPoints.end(), compPoints);
		for (int i = 0; i < intersectionPoints.size(); i += 2)
		{
			Line(intersectionPoints[i], intersectionPoints[i + 1]).drawLinePro(img, pointColor);
		}
		intersectionPoints.clear();
	}
}



void myShadowLine::drawShadowLines(Mat &img, Scalar pointColor /*= Scalar(255, 255, 255)*/)
{
	polygon.drawPolygon(img);
	getIntercept();
	getIntersectionPoints(img, pointColor);
	//sort(intersectionPoints.begin(), intersectionPoints.end(), compPoints);

	/*for (int i = 0; i < intersectionPoints.size()-1; i += 2)
	{
		Line(intersectionPoints[i], intersectionPoints[i + 1]).drawLinePro(img);
		cv::line(img, Point(intersectionPoints[i].getW(), intersectionPoints[i].getH()),
			Point(intersectionPoints[i+1].getW(), intersectionPoints[i+1].getH()), Scalar(0, 0, 255));
		imshow("debug", img);
		waitKey();
	}*/
	return;
}
#endif