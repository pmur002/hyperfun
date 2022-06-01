/*==============================================================================

Copyright 2018-2019 Dmitry Popov

This Work or file is part of the greater total Work, software or group of
files named HyperFun Polygonizer.

HyperFun Polygonizer can be redistributed and/or modified under the terms
of the CGPL, The Common Good Public License as published by and at CGPL.org
(http://CGPL.org).  It is released under version 1.0 Beta of the License
until the 1.0 version is released after which either version 1.0 of the
License, or (at your option) any later version can be applied.

THIS WORK, OR SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED (See the
CGPL, The Common Good Public License for more information.)

You should have received a copy of the CGPL along with HyperFun Polygonizer;
if not, see -  http://CGPL.org to get a copy of the License.

==============================================================================*/

#pragma once

#include "hfinterpreter.h"
#include <iomanip>

using namespace std;

struct link {
	double x;
	double y;
	struct link * next;
};

typedef struct link link;

struct chain {
	link* head;
	link* tail;
	int orientation;
	int number;
	struct chain** isLeftForFirst;
	struct chain** isLeftForSecond;
};

typedef struct chain chain;

class HFSlicer
{
public:
	HFSlicer(HFInterpreter& interp, vector<double> boundaries, vector<double> slicingStep);
	~HFSlicer();
	double GetZMin();
	double GetZMax();
	double GetZDelta();
	int WriteCLILayer(ostream& toFile, double z);

private:
	HFInterpreter* itsInterpreter;
	int numberOfArrtibutes;
	double* currentAttributesValues;
	double currentXValue[3];
	double calcF();
	//Looking for f(X)=0 on x range from negative x to positive
	double fx0(double negativex, double positivex);
	//Looking for f(X)=0 on y range from negative y to positive
	double fy0(double negativey, double positivey);

	//presicion for x and y calulations
	double eps;

	//multiplier for presicion. It's 1% now
	double eps_multiplier = 0.01;

	void processCell(int cell_counter);
	void processFirstColumn();
	void processLowerCell();
	int sliceZ(double z);
	//print "$$POLYLINE" line into the CLI file
	void printchain(ostream& toFile, const chain c);

	//xMin, yMin, zMin, xMax, yMax, zMax
	vector<double> boundaries;
	
	vector<double> delta;

	//Y size
	int ylength;
	//max number of points in chains
	int maxPoints;
	//max number of chains
	int maxChains;
	//empty links to use
	link * stack;
	//Number of segments have been took from stack
	int segment_counter;
	//Number of chains
	int number_of_chains;
	//array of chains
	chain * chains;
	//chains from left
	chain ** leftrow;
	//chains below current cell
	chain ** column;
	//vertices of considering cell
	double x1;
	double x2;
	double y1;
	double y2;
	//Values of f(X) in vertices
	double f11;
	double f12;
	double f21;
	double f22;
	//Type of cell
	int cell_code;
	//coordinates of a current line segment
	double xstart;
	double ystart;
	//pointer to hold cells for connection
	chain * tmpbuffer = NULL;
};
