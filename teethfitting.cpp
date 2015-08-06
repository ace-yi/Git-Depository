#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <iostream>

using namespace std;

void Rotate_clock(double *x, double *y, double angle);
void Rotate_anticlock(double *x, double *y, double angle);


int main()
{
	double x1 = 1.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;
	double angle = asin(0.5);
	if (((y2 - y1) / (x2 - x1)) > 0){
		Rotate_clock(&x1, &y1, angle);
	}
	else{
		Rotate_anticlock(&x1, &y1, angle);
	}
		cout << x1 << "   "<<y1 << endl;


}

void Rotate_clock(double *x, double *y, double angle)
{
	double tempx, tempy;
	tempx = *x;
	tempy = *y;
	*x = sin(angle) * tempy + cos(angle) * tempx;
	*y = cos(angle) * tempy - sin(angle) * tempx;

}

void Rotate_anticlock(double *x, double *y, double angle)
{
	double tempx, tempy;
	tempx = *x;
	tempy = *y;
	*x = cos(angle) * tempx - sin(angle) * tempy;
	*y = cos(angle) * tempy + sin(angle) * tempx;

}