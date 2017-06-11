#ifndef SCANLINE_H
#define SCANLINE_H

#include "Polygon.h"

typedef struct edgeNode
{
	float hmin, hmax, w;//在opencv的图像坐标系h表示y，w表示x
	float dw;// = 1/k
	edgeNode *next;
}edgeNode;

typedef struct ETnode
{
	int h;
	edgeNode *ptr;
}ETnode;

class myScanLine
{
public:
	myScanLine();
	~myScanLine();
	void drawColorByScanLine(Mat &img, Scalar pointColor = Scalar(255, 255, 255));

private:
	myPolygon polygon;
	vector<myPoint> outPoints;//外围的边集合
	int Hmax;
	int Hmin;
	int Hrange;
	void findHRange();
	edgeNode **node;
	void edgeNodeInitialize();
	ETnode *NETnode;
	int processSingularity(int p0_y, int p1_y, int p2_y, int p3_y);//预处理极值点
	void NETnodeInitialize();
	void NETAddEdgeNode();
	void insertNodeToNET(edgeNode *temp);
	edgeNode *AELHeader;
	void AELAddEdgeNode(edgeNode *&k);
	void insertNodeToAEL(edgeNode *temp);
	void AELDeleteNode(int h);
	void AELChangeX();

};

myScanLine::myScanLine()
{
	polygon = myPolygon();
	outPoints = polygon.outPoints;
	AELHeader = NULL;
	node = new edgeNode *[outPoints.size()];
}

myScanLine::~myScanLine()
{
}

void myScanLine::findHRange()
{
	Hmax = 0;
	Hmin = 9999999;
	for (int i = 0; i < polygon.outPoints.size();i++)
	{
		if (polygon.outPoints[i].getH() > Hmax)
		{
			Hmax = polygon.outPoints[i].getH();
		}
		if (polygon.outPoints[i].getH() < Hmin)
		{
			Hmin = polygon.outPoints[i].getH();
		}
	}
	Hrange = Hmax - Hmin + 1;
}

int myScanLine::processSingularity(int p0_y, int p1_y, int p2_y, int p3_y)
{
	int smaller_y;
	smaller_y = min(p1_y, p2_y);
	if (smaller_y == p1_y)
	{
		if ((p1_y - p0_y)*(p1_y - p2_y) >= 0)
		{
			return p1_y;//这样计算的时候就会拿这个点算两次吗
		}
		else
		{
			return p1_y + 1;
		}
	}
	else //即smaller_y == p2_y
	{
		if ((p2_y - p1_y)*(p2_y - p3_y) >= 0)
		{
			return p2_y;
		}
		else
		{
			return p2_y + 1;
		}
	}
}

void myScanLine::edgeNodeInitialize()
{
	for (int i = 0; i < polygon.outPoints.size();i++)
	{
		node[i] = NULL;
	}

	for (int i = 0; i < polygon.outPoints.size();i++)
	{
		int j = (i + 1) % outPoints.size();
		if (outPoints[i].getW() != outPoints[j].getW())
		{
			node[i] = new edgeNode;
			node[i]->hmax = outPoints[i].getH() > outPoints[j].getH() ? outPoints[i].getH() : outPoints[j].getH();
			node[i]->hmin = processSingularity(outPoints[(i + outPoints.size() - 1) % outPoints.size()].getH(), outPoints[i].getH(),
				outPoints[(i + 1) % outPoints.size()].getH(), outPoints[(i + 2) % outPoints.size()].getH());
			node[i]->w = outPoints[i].getH() > outPoints[j].getH() ? outPoints[j].getW() : outPoints[i].getW();
			float k = (outPoints[j].getH() - outPoints[i].getH())*1.0f / (outPoints[j].getW() - outPoints[i].getW())*1.0f;
			node[i]->dw = 1 / k;
			node[i]->next = NULL;
		}
	}
}

void myScanLine::NETnodeInitialize()
{
	NETnode = new ETnode[Hrange];
	for (int i = 0; i < Hrange;i++)
	{
		NETnode[i].h = Hmin + i;
		NETnode[i].ptr = NULL;
	}
}

void myScanLine::NETAddEdgeNode()
{
	edgeNode *temp;
	for (int i = 0; i < outPoints.size();i++)
	{
		if (node[i] != NULL)
		{
			temp = node[i];
			insertNodeToNET(temp);
		}
	}
}

void myScanLine::insertNodeToNET(edgeNode *temp)
{
	int i = 0;
	edgeNode *cur, *pre = NULL;

	while (NETnode[i].h < temp->hmin)
	{
		i++;
	}

	if (NETnode[i].ptr == NULL)
	{
		NETnode[i].ptr = temp;
	}
	else
	{
		cur = NETnode[i].ptr;
		while (cur != NULL && (cur->w < temp->w || (cur->w == temp->w && cur->dw < temp->dw)))
		{
			pre = cur;
			cur = cur->next;
		}
		if (cur == NULL)
		{
			pre->next = temp;
		}
		else
		{
			if (pre != NULL)
			{
				//插入pre和cur之间
				temp->next = cur;
				pre->next = temp;
			}
			else
			{
				temp->next = cur;
				NETnode[i].ptr = temp;
				cur = NETnode[i].ptr;
			}
			
		}
	}
}

void myScanLine::AELAddEdgeNode(edgeNode *&k)
{
	edgeNode *t, *h = k;
	k = NULL;
	while (h != NULL)
	{
		t = h;
		h = h->next;
		t->next = NULL;
		insertNodeToAEL(t);
	}
}

void myScanLine::insertNodeToAEL(edgeNode *temp)
{
	edgeNode *cur, *pre = NULL;
	if (AELHeader == NULL)
	{
		AELHeader = temp;
	}
	else
	{
		cur = AELHeader;
		while (cur != NULL && (cur->w < temp->w || (cur->w == temp->w && cur->dw < temp->dw)))
		{
			pre = cur;
			cur = cur->next;
		}
		if (cur == NULL)
		{
			pre->next = temp;
		}
		else
		{
			if (pre != NULL)
			{
				//插入pre和cur之间
				temp->next = cur;
				pre->next = temp;
			}
			else
			{
				temp->next = cur;
				AELHeader = temp;
				cur = temp;
			}
		}
	}
}

void myScanLine::AELDeleteNode(int h)
{
	edgeNode *p = AELHeader, *q = NULL;
	while (p != NULL)
	{
		if (p->hmax == h)
		{
			if (q == NULL)
			{
				AELHeader = p->next;
				delete p;
				p = AELHeader;
			}
			else
			{
				q->next = p->next;
				delete p;
				p = q->next;
			}
		}
		else
		{
			q = p;
			p = p->next;
		}
	}
}

void myScanLine::AELChangeX()
{
	edgeNode *p = AELHeader;
	while (p != NULL)
	{
		p->w += p->dw;
		p = p->next;
	}
}

void myScanLine::drawColorByScanLine(Mat &img, Scalar pointColor /*= Scalar(255, 255, 255)*/)
{
	polygon.drawPolygonOut(img);//画出外围多边形

	findHRange();//赋值 Hmax、Hmin、和Hrange

	edgeNodeInitialize();//初始化边
	NETnodeInitialize();//初始化NET表
	NETAddEdgeNode();//将边加入NET表中

	edgeNode *dp1, *dp2;
	for (int i = 0; i < Hrange;i++)
	{
		if (NETnode[i].ptr != NULL)
		{
			AELAddEdgeNode(NETnode[i].ptr);
		}
		if (AELHeader != NULL)
		{
			dp1 = AELHeader;
			dp2 = dp1->next;
			if (dp2 != NULL && (int)dp1->w != (int)dp2->w)
			{
				Line(dp1->w, NETnode[i].h, dp2->w, NETnode[i].h).drawLinePro(img, pointColor);
			}
			while (dp2 != NULL &&dp2->next != NULL && dp2->next->next != NULL)
			{
				dp1 = dp2->next;
				dp2 = dp2->next->next;
				if ((int)dp1->w != (int)dp2->w)
				{
					Line(dp1->w, NETnode[i].h, dp2->w, NETnode[i].h).drawLinePro(img, pointColor);
				}
			}
		}
		AELDeleteNode(NETnode[i].h);
		AELChangeX();
	}
}
#endif