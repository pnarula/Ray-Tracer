#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include "matrix.h"
using namespace std;


struct Color
{
	float r, g, b;
};

struct Point
{
	float x, y, z;
};

struct Ray
{
	Point o;
	Point d;
	int reflected;
};

struct Triangle
{
	Point v0;
	Point v1;
	Point v2;
	int materialId;
};

struct Material 
{
	float reflection;
	float r, g, b;
};

struct Sphere 
{
	Point c;
	float r;
	int transformed; //flag that says whether transformed or not
	Matrix mat;//scale
	Point tlate;
	int materialId;
};

struct Light
{
	Point pos;
	Color col;
};

struct Scene
{
  std::vector<Sphere> spheres;
  std::vector<Light> lights;
  std::vector<Material> materials;
  std::vector<Triangle> triangles;
  
  int width, height, spherecount, trianglecount, lightcount, materialcount; //width y, height x
};

Color addClr(Color &c1, Color &c2)
{	  Color c;
          c.r = c1.r + c2.r;
          c.g = c1.g + c2.g;
          c.b = c1.b + c2.b;
          return c;
}

Color subClr(Color &c1, Color &c2)
{	  Color c;
          c.r = c1.r - c2.r;
          c.g = c1.g - c2.g;
          c.b = c1.b - c2.b;
          return c;
}

Color multClr(Color &c1, float f)
{	  Color c;
          c.r = c1.r * f;
          c.g = c1.g * f;
          c.b = c1.b * f;
          return c;
}

Point subPoint(Point &p1, Point &p2)
{
	Point p;// = (Point*)malloc(sizeof(Point*));
	p.x = p1.x - p2.x;
	p.y = p1.y - p2.y;
	p.z = p1.z - p2.z;
	return p;
}

Point addPoint(Point &p1, Point &p2)
{
	Point p;// = (Point*)malloc(sizeof(Point*));
	p.x = p1.x + p2.x;
	p.y = p1.y + p2.y;
	p.z = p1.z + p2.z;
	return p;
}

float multPoint(Point &p1, Point &p2)
{
	float ret = p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
	return ret;
}

Point multPoint(float a, Point &p2)
{
	Point p1;// = (Point*)malloc(sizeof(Point*));
	p1.x = a * p2.x;
	p1.y = a * p2.y;
	p1.z = a * p2.z;
	return p1;
}

Point crossProduct(Point &p1, Point &p2)
{
	Point p;
	p.x = p1.y * p2.z - p1.z * p2.y;
	p.y = p1.z * p2.x - p1.x * p2.z;
	p.z = p1.x * p2.y - p1.y * p2.x;
	return p;
}