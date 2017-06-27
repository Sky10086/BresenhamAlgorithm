#ifndef LINE_H
#define LINE_H

#include "Point.h"

class Line
{
public:
	Line(myPoint start, myPoint end);
	Line(int sW, int sH, int eW, int eH);
	~Line();
	void drawLine(CV_IN_OUT Mat& img);//����ͨ���㷨��������������
	void drawLinePro(CV_IN_OUT Mat& img, Scalar lineColor = Scalar(255, 255, 255));//ʹ��Bresenham�㷨�Ż���ȥ����������
private:
	float lineK;//б��
	float lineB;//�ؾ�
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
	int stepH = startP.getH() < endP.getH() ? 1 : -1;
	int stepW = startP.getH() < endP.getH() ? 1 : -1;

	
	myPoint pointPath(startP.getW(), startP.getH());

	if (dw >= dh)
	{
		int draw_h = startP.getH();
		for (int w = startP.getW(); w != endP.getW(); w+=stepW)
		{
			eee += dh;
			if (2 * eee >= dw)
			{
				draw_h += stepH;
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


		for (int h = startP.getH(); h != endP.getH(); h += stepH)
		{
			eee += dw;
			if (2 * eee >= dh)
			{
				draw_w += stepW;
				pointPath.setWH(draw_w, h);
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