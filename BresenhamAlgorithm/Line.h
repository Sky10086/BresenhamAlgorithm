#ifndef LINE_H
#define LINE_H

#include "Point.h"

class Line
{
public:
	Line(myPoint start, myPoint end);
	Line(int sW, int sH, int eW, int eH);
	~Line();
	void drawLine(CV_IN_OUT Mat& img);//最普通的算法，包含浮点运算
	void drawLinePro(CV_IN_OUT Mat& img, Scalar lineColor = Scalar(255, 255, 255));//使用Bresenham算法优化，去除浮点运算
private:
	float lineK;//斜率
	float lineB;//截距
	float findHbyLine(int w);
	myPoint startP;
	myPoint endP;
};

Line::Line(myPoint start, myPoint end)
{
	//assert(start.getW() != end.getW());
	this->lineK = (end.getH() - start.getH())*1.0f / (end.getW() - start.getW()+0.000001f)*1.0f;
	this->lineB = end.getH()*1.0f - this->lineK*end.getW();
	this->startP = start.getW() < end.getW() ? start : end;
	this->endP = start.getW() < end.getW() ? end : start;
}

Line::Line(int sW, int sH, int eW, int eH)
{
	assert(sW != eW);
	this->lineK = (eH - sH)*1.0f / (eW - sW)*1.0f;
	this->lineB = eH*1.0f - this->lineK*eW;
	/*this->startP = myPoint(sW,sH);
	this->endP = myPoint(eW,eH);*/
	this->startP = sW < eW ? myPoint(sW, sH) : myPoint(eW, eH);
	this->endP = sW < eW ? myPoint(eW, eH) : myPoint(sW, sH);
}

Line::~Line()
{
}

void Line::drawLine(CV_IN_OUT Mat& img)
{
	this->startP.drawPoint(img);
	myPoint pointPath(startP.getW(),startP.getH());
	
	for (int w = this->startP.getW() + 1; w < this->endP.getW();w++)
	{
		float h = this->findHbyLine(w);
		float lastH = pointPath.getH();
		if (h < lastH+0.5f)
		{
			pointPath.setWH(w, lastH);
			
		}
		else
		{
			pointPath.setWH(w, lastH + 1);
		}
		pointPath.drawPoint(img);
	}
}
float Line::findHbyLine(int w)
{
	return this->lineK*w + this->lineB;
}

void Line::drawLinePro(CV_IN_OUT Mat& img, Scalar lineColor /*= Scalar(255, 255, 255)*/)
{
	int dh = abs(endP.getH() - startP.getH());
	int dw = abs(endP.getW() - startP.getW());
	int eee = 0;
	
	myPoint pointPath(startP.getW(), startP.getH());

	if (dw >= dh)
	{
		int draw_h = startP.getH();
		int step = startP.getH() < endP.getH() ? 1 : -1;
		for (int w = startP.getW(); w <= endP.getW(); w++)
		{
			eee += dh;
			if (2 * eee >= dw)
			{
				draw_h += step;
				pointPath.setWH(w, draw_h);//h+1
				eee = eee - dw;
			}
			else
			{
				pointPath.setWH(w, draw_h);
			}
			pointPath.drawPoint(img, lineColor);
		}
	}

	else
	{
		int draw_w = startP.getW();
		/*int start = min(startP.getH(), endP.getH());
		int end = max(startP.getH(), endP.getH());*/
		int step = startP.getH() < endP.getH() ? 1 : -1;
		for (int h = startP.getH(); h != endP.getH(); h += step)
		{
			eee += dw;
			if (2 * eee >= dh)
			{
				pointPath.setWH(draw_w++, h);//w+1
				eee = eee - dh;

			}
			else
			{
				pointPath.setWH(draw_w, h);
			}
			pointPath.drawPoint(img, lineColor);

		}
	}

	
	return;
}

#endif