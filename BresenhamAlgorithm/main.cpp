#include <iostream>
#include <time.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include<vector>;
#include<math.h>
#include<stack>
#include <string>

#include "Line.h"
#include "circle.h"
#include "Ellipse.h"
#include "Polygon.h"
#include "ShadowLine.h"
#include "scanLine.h"
using namespace std;
using namespace cv;

#define PI 3.1415926
void help()
{
	printf("\nThis program show some drawing.\n"
		"Usage:\n"
		" press a : draw a random line!\n"
		" press b : draw a random circle!\n"
		" press c : draw a random ellipse!\n"
		" press d : draw a ShadowLines whth random color!\n"
		" press e : draw a ColorByScanLine whth random color!\n"
		" press enter : exit!\n"
		" allCopyRight by 刘潇\n");
}
static Scalar randomColor(RNG &rng)
{
	
	int color = (unsigned)rng;
	return Scalar(color & 255, (color >> 8) & 255, (color >> 16) & 255);
}


/*根据直线方程，输入w找对应的h，这里是图片坐标系，左上角是(0,0)*/
float findHbyLine(int w);

int main()
{
	help();
	char wndname[] = "show";
	int i, width = 700, height = 600;
	RNG rng(0xFFFFFFFF);
	Mat image = Mat::zeros(height, width, CV_8UC3);
	imshow(wndname, image);

	while (true)
	{
		char key = waitKey(10);
		switch (key)
		{
		case '\r':
			return 0;
			break;
		case 'a':
		{
			image = Mat::zeros(height, width, CV_8UC3);
			Line line(rng.uniform(100, 300), rng.uniform(200, 500), rng.uniform(140, 700), rng.uniform(100, 500));
			line.drawLinePro(image, randomColor(rng));
			imshow(wndname, image);
			break;
		}
			
		case 'b':
		{
			image = Mat::zeros(height, width, CV_8UC3);
			myCircle testCircel(rng.uniform(200, 400), rng.uniform(300, 400), rng.uniform(100, 300));
			testCircel.drawCircle(image, randomColor(rng));
			imshow(wndname, image);
		}
			break;
		case 'c':
		{
			image = Mat::zeros(height, width, CV_8UC3);
			myElipse testEli(rng.uniform(200, 400), rng.uniform(100, 190), rng.uniform(300, 400), rng.uniform(300, 400));
			testEli.drawEllipse(image, randomColor(rng));
			imshow(wndname, image);
		}
			break;
		case 'd':
		{
			image = Mat::zeros(height, width, CV_8UC3);
			myShadowLine testShadowLine(PI / 6, 10);
			testShadowLine.drawShadowLines(image, randomColor(rng));
			imshow(wndname, image);
		}
			break;
		case 'e':
		{
			image = Mat::zeros(height, width, CV_8UC3);
			myScanLine testScanLine;
			testScanLine.drawColorByScanLine(image, randomColor(rng));
			imshow(wndname, image);
		}
			break;
		default:
			break;
		}
	}
	
	
	

	

	//myPolygon testPolygon;
	//testPolygon.drawPolygon(image);
	//imshow(wndname, image);

	

	


	waitKey();
	return 0;
}