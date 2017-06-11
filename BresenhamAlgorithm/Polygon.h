#ifndef POLYGON_H
#define POLYGON_H

#include "Line.h"
#define isDebugPolygon 0

class myPolygon
{
public:
	myPolygon();
	~myPolygon();
	void drawPolygon(Mat &img);
	void drawPolygonOut(Mat &img);

	//�μ���д�Ķ�ά���飬����û��Ҫ��������һά����һ�����������һ������ڶ����
	vector<myPoint> outPoints;
	vector<myPoint> inerPoints;

private:
	
};

myPolygon::myPolygon()
{
	outPoints.push_back(myPoint(100, 200));
	outPoints.push_back(myPoint(150, 400));
	outPoints.push_back(myPoint(400, 450));
	outPoints.push_back(myPoint(500, 300));
	outPoints.push_back(myPoint(300, 50));
	inerPoints.push_back(myPoint(302, 210));
	inerPoints.push_back(myPoint(160, 320));
	inerPoints.push_back(myPoint(360, 340));
}

myPolygon::~myPolygon()
{
}

void myPolygon::drawPolygon(Mat &img)
{
	for (int i = 0; i < outPoints.size();i++)
	{
		Line(outPoints[i], outPoints[(i + 1)%outPoints.size()]).drawLinePro(img);
#if isDebugPolygon
		imshow("debug", img);
		waitKey();
#endif
	}
	for (int i = 0; i < inerPoints.size(); i++)
	{
		Line(inerPoints[i], inerPoints[(i + 1) % inerPoints.size()]).drawLinePro(img);
#if isDebugPolygon
		imshow("debug", img);
		waitKey();
#endif
	}
	return;
}

void myPolygon::drawPolygonOut(Mat &img)
{
	for (int i = 0; i < outPoints.size(); i++)
	{
		Line(outPoints[i], outPoints[(i + 1) % outPoints.size()]).drawLinePro(img);
	}
}
#endif