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

#include "hfslicer.h"
#include "hfpolymesh.h"
#include <iomanip>
#include <algorithm>

//creating the new chain with given values of head and tail coordinates
void createchain(chain* newchain, link* tail, link* head, int orientation, int number, double tailx, double taily, double headx, double heady) {
	newchain->tail = tail;
	newchain->head = head;
	newchain->orientation = orientation;
	newchain->number = number;
	newchain->tail->x = tailx;
	newchain->tail->y = taily;
	newchain->head->x = headx;
	newchain->head->y = heady;
	newchain->tail->next = head;
};

//linking the tolink chain with the linkwith chain: tolink---->linkwith---->
void linkchains(chain* tolink, chain* linkwith, int link_direction) {
	//if ((tolink != NULL) && (linkwith != NULL)) {
	(tolink->head)->next = linkwith->tail;
	tolink->head = linkwith->head;
	tolink->orientation += linkwith->orientation + link_direction;

	linkwith->tail = tolink->tail;
	linkwith->orientation = tolink->orientation;
	//}
};

//copying a chain
void copychain(const chain from, chain * to) {
	to->tail = from.tail;
	to->head = from.head;
	to->orientation = from.orientation;
};

//adding link to chain
void addlink(chain* tochain, link * newlink, double x, double y, bool isHead, int orientation) {
	newlink->x = x;
	newlink->y = y;
	if (isHead) {
		tochain->head->next = newlink;
		tochain->head = newlink;
	}
	else {
		newlink->next = tochain->tail;
		tochain->tail = newlink;
	}
	tochain->orientation += orientation;
};


using namespace std;

HFSlicer::HFSlicer(HFInterpreter& interp, vector<double> boundaries, vector<double> slicingStep): boundaries(boundaries), delta(slicingStep)
{
	/******ADDED FOR S ATTRIBUTES******/ 
	itsInterpreter = &interp;
	numberOfArrtibutes = itsInterpreter->getSSize();
	currentAttributesValues = new double[numberOfArrtibutes];

	currentXValue[0] = 0;
	currentXValue[1] = 0;
	currentXValue[2] = 0;

	eps = min(delta[0], delta[1]);
	eps = min(eps, delta[2]) * eps_multiplier;

	//Y size
	ylength = (boundaries[4] - boundaries[1]) / delta[1];

	//max number of points in chains
	maxPoints = 4 * ((int)((boundaries[3] - boundaries[0]) / slicingStep[0]) * (int)((boundaries[4] - boundaries[1]) / slicingStep[1]));

	//max number of chains
	maxChains = ((int)((boundaries[3] - boundaries[0]) / slicingStep[0]) * (int)((boundaries[4] - boundaries[1]) / slicingStep[1]) / 2);

	//empty links to use
	stack = (link*)calloc(maxPoints + 1, sizeof(link));

	number_of_chains = 0;

	//array of chains
	chains = (chain*)malloc(maxChains * sizeof(chain));

	//Type of cell                                                                                         
	cell_code = 0;

	//TODO: check allocation
	//chains from left
	leftrow = (chain**)calloc(ylength + 1, sizeof(chain *));

	//chains below current cell
	column = (chain**)calloc(ylength, sizeof(chain *));

	//set vertices of considering cell to min values
	x1 = boundaries[0];
	x2 = boundaries[0];
	y1 = boundaries[1];
	y2 = boundaries[1];

	//Initialize values of f(X) in vertices to 0
	f11 = 0;
	f12 = 0;
	f21 = 0;
	f22 = 0;

	//Set number of segments to 0
	segment_counter = 0;

	//Set coordinates of a current line segment to 0
	xstart = 0;
	ystart = 0;

	tmpbuffer = NULL;
};


HFSlicer::~HFSlicer()
{
	delete[] currentAttributesValues;
	free(stack);
	free(chains);
	free(leftrow);
	free(column);
};

double HFSlicer::GetZMin()
{
	return boundaries[2];
};

double HFSlicer::GetZMax()
{
	return boundaries[5];
};

double HFSlicer::GetZDelta()
{
	return delta[2];
};

double HFSlicer::calcF() 
{
	vector<double> tmpX(3);
	tmpX[0] = currentXValue[0];
	tmpX[1] = currentXValue[1];
	tmpX[2] = currentXValue[2];
	return itsInterpreter->calc(tmpX, itsInterpreter->parameters(), currentAttributesValues);
};

//Looking for f(X)=0 on x range from negative x to positive
double HFSlicer::fx0(double negativex, double positivex) 
{
	double result = negativex;
	double residual = abs(positivex - negativex);
	while (residual > eps) {
		result = negativex + (positivex - negativex) / 2;
		currentXValue[0] = result;
		if (calcF() >= 0) positivex = result;
		else negativex = result;
		residual = abs(positivex - negativex);
	}
	return result;
};

//Looking for f(X)=0 on y range from negative y to positive
double HFSlicer::fy0( double negativey, double positivey) {
	double result = negativey;
	double residual = abs(positivey - negativey);
	while (residual > eps) {
		result = negativey + (positivey - negativey) / 2;
		currentXValue[1] = result;
		if (calcF() >= 0) positivey = result;
		else negativey = result;
		residual = abs(positivey - negativey);
	}
	return result;
};

//Calculating x coordinate of an approximate hyperbola center
double hypX(double x1, double x2, double f11, double f12, double f21, double f22) {
	return (x2 * f11 + x1 * f22 - x1 * f21 - x2 * f12) / (f22 + f11 - f12 - f21);
};

//Calculating y coordinate of an approximate hyperbola center
double hypY(double y1, double y2, double f11, double f12, double f21, double f22) {
	return (y2 * f11 + y1 * f22 - y1 * f12 - y2 * f21) / (f22 + f11 - f21 - f12);
};

void HFSlicer::processCell(int cell_counter) {
 	switch (cell_code)
	{
	case 1:
		if (leftrow[cell_counter] != column[cell_counter - 1]) {
			//linking chains: the one from the bottom and the one from the left
			linkchains(column[cell_counter - 1], leftrow[cell_counter], -1);

			if (&chains[number_of_chains - 1] != leftrow[cell_counter]) {
				if (column[cell_counter - 1]->isLeftForFirst != NULL) {
					*column[cell_counter - 1]->isLeftForFirst = leftrow[cell_counter];
					if (leftrow[cell_counter]->isLeftForFirst == NULL  || leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
						leftrow[cell_counter]->isLeftForFirst = column[cell_counter - 1]->isLeftForFirst;
					else leftrow[cell_counter]->isLeftForSecond = column[cell_counter - 1]->isLeftForFirst;
				}

				if (&chains[number_of_chains - 1] != column[cell_counter - 1]) {
					copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
					if (chains[number_of_chains - 1].isLeftForFirst != NULL) {
						*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
						column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
					}
					if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
						*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
						column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
					}
				}
			}
			else
			{
				copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
				if (chains[number_of_chains - 1].isLeftForFirst != &leftrow[cell_counter]) {
					*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
					if (column[cell_counter - 1]->isLeftForFirst == NULL)
						column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
					else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForFirst;
				}
				else if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
					*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
					if (column[cell_counter - 1]->isLeftForFirst == NULL)
						column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForSecond;
					else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
				}
			}

			chains[number_of_chains - 1].isLeftForFirst = NULL;
			chains[number_of_chains - 1].isLeftForSecond = NULL;

			number_of_chains--;

		}


		//Error is possible in the first case. isLeftForSecond doesn't assign NULL
		if (leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
			leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
		else if (leftrow[cell_counter]->isLeftForSecond == &leftrow[cell_counter])
			leftrow[cell_counter]->isLeftForSecond = NULL;

		leftrow[cell_counter] = NULL;
		//column[cell_counter] = NULL;
		column[cell_counter - 1] = NULL;
		break;
	case 2:
		//setting the value of y to calculate x value
		currentXValue[1] = y2;

		//adding the new piar of coordinates to the left chain
		addlink(leftrow[cell_counter], &stack[segment_counter++], fx0(x2, x1), y2, true, -1);

		//setting the left chain as the last (bottom) for this cell
		column[cell_counter] = leftrow[cell_counter];

		//TODO: Check. The error is possible if &leftrow[cell_counter] == leftrow[cell_counter]->isLeftForSecond
		//Deleting pointer to the left chain for this cell 
		if (&leftrow[cell_counter] == leftrow[cell_counter]->isLeftForFirst) {
			leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
			leftrow[cell_counter]->isLeftForSecond = NULL;
		}
		else leftrow[cell_counter]->isLeftForSecond = NULL;

		leftrow[cell_counter] = NULL;
		//
		////Deleting pointer to the bottom chain for the previous cell
		//column[cell_counter - 1] = NULL;
		break;
	case 3:
		currentXValue[1] = y2;
		addlink(column[cell_counter - 1], &stack[segment_counter++], fx0(x2, x1), y2, true, 0);
		leftrow[cell_counter] = NULL;
		column[cell_counter] = column[cell_counter - 1];
		break;
	case 4:
		currentXValue[0] = x2;
		addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y2, y1), false, -1);

		if (column[cell_counter - 1]->isLeftForFirst == NULL)
			column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
		else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];

		leftrow[cell_counter] = column[cell_counter - 1];
		//TODO Check if I need to set column[cell_counter] to NULL
		//column[cell_counter] = NULL;
		column[cell_counter - 1] = NULL;
		break;
	case 5:
		currentXValue[0] = x2;
		addlink(leftrow[cell_counter], &stack[segment_counter++], x2, fy0(y2, y1), false, 0);
		//hint
		//leftrow[cell_counter] = leftrow[cell_counter];
		column[cell_counter - 1] = NULL;
		//column[cell_counter] = NULL;
		break;
	//TODO: FIX AS 1 and 14!!!! 
	case 6:
		currentXValue[0] = x2;
		ystart = fy0(y2, y1);
		if (ystart > hypY(y1, y2, f11, f12, f21, f22)) {
			//linking the left and the last segment
			if (leftrow[cell_counter] != column[cell_counter - 1]) {
				linkchains(leftrow[cell_counter], column[cell_counter - 1], 1);

				if (&chains[number_of_chains - 1] != leftrow[cell_counter]) {
					if (column[cell_counter - 1]->isLeftForFirst != NULL) {
						*column[cell_counter - 1]->isLeftForFirst = leftrow[cell_counter];
						if (leftrow[cell_counter]->isLeftForFirst == NULL || leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
							leftrow[cell_counter]->isLeftForFirst = column[cell_counter - 1]->isLeftForFirst;
						else leftrow[cell_counter]->isLeftForSecond = column[cell_counter - 1]->isLeftForFirst;
					}

					if (&chains[number_of_chains - 1] != column[cell_counter - 1]) {
						copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
						if (chains[number_of_chains - 1].isLeftForFirst != NULL) {
							*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
							column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
						}
						if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
							*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
							column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
						}
					}
				}
				else
				{
					copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
					if (chains[number_of_chains - 1].isLeftForFirst != &leftrow[cell_counter]) {
						*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
						if (column[cell_counter - 1]->isLeftForFirst == NULL)
							column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
						else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForFirst;
					}
					else if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
						*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
						if (column[cell_counter - 1]->isLeftForFirst == NULL)
							column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForSecond;
						else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
					}
				}

				chains[number_of_chains - 1].isLeftForFirst = NULL;
				chains[number_of_chains - 1].isLeftForSecond = NULL;

				number_of_chains--;

			}

			if (leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
				leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
			else if (leftrow[cell_counter]->isLeftForSecond == &leftrow[cell_counter])
				leftrow[cell_counter]->isLeftForSecond = NULL;

			currentXValue[1] = y2;
			//getting empty links from the stack for creation of a new chain
			createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x2, ystart, fx0(x2, x1), y2);

			column[cell_counter - 1] = &chains[number_of_chains];
			if (chains[number_of_chains].isLeftForFirst == NULL) {
				chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];
			}
			else chains[number_of_chains].isLeftForSecond = &leftrow[cell_counter];
			leftrow[cell_counter] = &chains[number_of_chains];
			column[cell_counter] = column[cell_counter - 1];
			column[cell_counter - 1] = NULL;


			segment_counter += 2;
			number_of_chains++;
		}
		else {
			currentXValue[1] = y2;
			addlink(leftrow[cell_counter], &stack[segment_counter++], fx0(x2, x1), y2, true, -1);
			currentXValue[0] = x2;
			addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y2, y1), false, -1);

			tmpbuffer = leftrow[cell_counter];

			if (&leftrow[cell_counter] == leftrow[cell_counter]->isLeftForFirst) {
				leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
				leftrow[cell_counter]->isLeftForSecond = NULL;
			}
			else leftrow[cell_counter]->isLeftForSecond = NULL;

			if (column[cell_counter - 1]->isLeftForFirst == NULL)
				column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
			else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];
			
			leftrow[cell_counter] = column[cell_counter - 1];
			column[cell_counter] = tmpbuffer;
		}
		break;
	case 7:
		currentXValue[0] = x2;
		ystart = fy0(y2, y1);
		currentXValue[1] = y2;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x2, ystart, fx0(x2, x1), y2);
		
		chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];
		
		leftrow[cell_counter] = &chains[number_of_chains];
		column[cell_counter] = &chains[number_of_chains];

		segment_counter += 2;
		number_of_chains++;
		break;
	case 8:
		currentXValue[1] = y2;
		xstart = fx0(x1, x2);
		currentXValue[0] = x2;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y2, x2, fy0(y1, y2));

		chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];
		
		leftrow[cell_counter] = &chains[number_of_chains];
		column[cell_counter] = &chains[number_of_chains];

		segment_counter += 2;
		number_of_chains++;
		break;
	case 9:
		xstart = column[cell_counter - 1]->head->x;
		if (xstart < hypX(x1, x2, f11, f12, f21, f22)) {
			//linking the last and the left segment
			if (leftrow[cell_counter] != column[cell_counter - 1]) {
				//linking chains: the one from the bottom and the one from the left
				linkchains(column[cell_counter - 1], leftrow[cell_counter], -1);

				if (&chains[number_of_chains - 1] != leftrow[cell_counter]) {
					if (column[cell_counter - 1]->isLeftForFirst != NULL) {
						*column[cell_counter - 1]->isLeftForFirst = leftrow[cell_counter];
						if (leftrow[cell_counter]->isLeftForFirst == NULL || leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
							leftrow[cell_counter]->isLeftForFirst = column[cell_counter - 1]->isLeftForFirst;
						else leftrow[cell_counter]->isLeftForSecond = column[cell_counter - 1]->isLeftForFirst;
					}

					if (&chains[number_of_chains - 1] != column[cell_counter - 1]) {
						copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
						if (chains[number_of_chains - 1].isLeftForFirst != NULL) {
							*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
							column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
						}
						if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
							*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
							column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
						}
					}
				}
				else
				{
					copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
					if (chains[number_of_chains - 1].isLeftForFirst != &leftrow[cell_counter]) {
						*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
						if (column[cell_counter - 1]->isLeftForFirst == NULL)
							column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
						else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForFirst;
					}
					else if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
						*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
						if (column[cell_counter - 1]->isLeftForFirst == NULL)
							column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForSecond;
						else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
					}
				}

				chains[number_of_chains - 1].isLeftForFirst = NULL;
				chains[number_of_chains - 1].isLeftForSecond = NULL;

				number_of_chains--;

			}

			if (leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
				leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
			else if (leftrow[cell_counter]->isLeftForSecond == &leftrow[cell_counter])
				leftrow[cell_counter]->isLeftForSecond = NULL;

			currentXValue[1] = y2;
			xstart = fx0(x1, x2);
			currentXValue[0] = x2;
			createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y2, x2, fy0(y1, y2));

			chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];

			leftrow[cell_counter] = &chains[number_of_chains];
			column[cell_counter] = &chains[number_of_chains];
			column[cell_counter - 1] = &chains[number_of_chains];

			segment_counter += 2;
			number_of_chains++;
		}
		else {
			currentXValue[0] = x2;
			addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y1, y2), true, 1);

			currentXValue[1] = y2;
			addlink(leftrow[cell_counter], &stack[segment_counter++], fx0(x1, x2), y2, false, 1);
			tmpbuffer = leftrow[cell_counter];
			
			//Deleting pointer to the left chain for this cell
			if (&leftrow[cell_counter] == leftrow[cell_counter]->isLeftForFirst) {
				leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
				leftrow[cell_counter]->isLeftForSecond = NULL;
			}
			else leftrow[cell_counter]->isLeftForSecond = NULL;

			if (column[cell_counter - 1]->isLeftForFirst == NULL)
				column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
			else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];

			leftrow[cell_counter] = column[cell_counter - 1];
			column[cell_counter] = tmpbuffer;
		}
		break;
	case 10:
		currentXValue[0] = x2;
		addlink(leftrow[cell_counter], &stack[segment_counter++], x2, fy0(y1, y2), true, 0);
		//leftrow[cell_counter] = leftrow[cell_counter];

		//column[cell_counter] = NULL;
		column[cell_counter - 1] = NULL;
		break;
	case 11:
		currentXValue[0] = x2;
		addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y1, y2), true, 1);
		if (column[cell_counter - 1]->isLeftForFirst == NULL)
			column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
		else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];
		leftrow[cell_counter] = column[cell_counter - 1];
		//column[cell_counter] = NULL;
		column[cell_counter - 1] = NULL;
		break;
	case 12:
		currentXValue[1] = y2;
		addlink(column[cell_counter - 1], &stack[segment_counter++], fx0(x1, x2), y2, false, 0);
		leftrow[cell_counter] = NULL;
		column[cell_counter] = column[cell_counter - 1];
		break;
	case 13:
		currentXValue[1] = y2;
		addlink(leftrow[cell_counter], &stack[segment_counter++], fx0(x1, x2), y2, false, 1);
		column[cell_counter] = leftrow[cell_counter];

		//Deleting pointer to the left chain for this cell
		if (&leftrow[cell_counter] == leftrow[cell_counter]->isLeftForFirst) {
			leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
			leftrow[cell_counter]->isLeftForSecond = NULL;
		}
		else leftrow[cell_counter]->isLeftForSecond = NULL;
		leftrow[cell_counter] = NULL;
		
		column[cell_counter - 1] = NULL;
		break;
	case 14:
		if (leftrow[cell_counter] != column[cell_counter - 1]) {
			linkchains(leftrow[cell_counter], column[cell_counter - 1], 1);

			if (&chains[number_of_chains - 1] != leftrow[cell_counter]) {
				if (column[cell_counter - 1]->isLeftForFirst != NULL) {
					*column[cell_counter - 1]->isLeftForFirst = leftrow[cell_counter];
					if (leftrow[cell_counter]->isLeftForFirst == NULL || leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
						leftrow[cell_counter]->isLeftForFirst = column[cell_counter - 1]->isLeftForFirst;
					else leftrow[cell_counter]->isLeftForSecond = column[cell_counter - 1]->isLeftForFirst;
				}

				if (&chains[number_of_chains - 1] != column[cell_counter - 1]) {
					copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
					if (chains[number_of_chains - 1].isLeftForFirst != NULL) {
						*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
						column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
					}
					if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
						*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
						column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
					}
				}
			}
			else
			{
				copychain(chains[number_of_chains - 1], column[cell_counter - 1]);
				if (chains[number_of_chains - 1].isLeftForFirst != &leftrow[cell_counter]) {
					*chains[number_of_chains - 1].isLeftForFirst = column[cell_counter - 1];
					if (column[cell_counter - 1]->isLeftForFirst == NULL)
						column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForFirst;
					else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForFirst;
				}
				else if (chains[number_of_chains - 1].isLeftForSecond != NULL) {
					*chains[number_of_chains - 1].isLeftForSecond = column[cell_counter - 1];
					if (column[cell_counter - 1]->isLeftForFirst == NULL)
						column[cell_counter - 1]->isLeftForFirst = chains[number_of_chains - 1].isLeftForSecond;
					else column[cell_counter - 1]->isLeftForSecond = chains[number_of_chains - 1].isLeftForSecond;
				}
			}

			chains[number_of_chains - 1].isLeftForFirst = NULL;
			chains[number_of_chains - 1].isLeftForSecond = NULL;

			number_of_chains--;
			
		}

		if (leftrow[cell_counter]->isLeftForFirst == &leftrow[cell_counter])
			leftrow[cell_counter]->isLeftForFirst = leftrow[cell_counter]->isLeftForSecond;
		else if (leftrow[cell_counter]->isLeftForSecond == &leftrow[cell_counter])
			leftrow[cell_counter]->isLeftForSecond = NULL;


		leftrow[cell_counter] = NULL;
		//column[cell_counter] = NULL;
		column[cell_counter - 1] = NULL;
		break;
	default: break;
	}}

void HFSlicer::processFirstColumn() {

	//is it the first cell
	bool first = true;

	//shift to the next column
	x2 = x1 + delta[0];

	//setting the cell number to -1
	int cell_counter = -1;

	//Initial �alculation of f12. It will be passed to the f11 from the next (1st) step
	currentXValue[0] = x1;
	currentXValue[1] = y1;
	f12 = calcF();

	//Initial �alculation of f22. It will be passed to the f21 from the next (1st) step
	currentXValue[0] = x2;
	currentXValue[1] = y1;
	f22 = calcF();
	while (y2 < boundaries[4]) {
		//counting the current number of cell in the column
		cell_counter++;

		//shift to the upper cell in the column
		y2 = y1 + delta[1];

		cell_code = 0;

		f11 = f12;
		cell_code += (f11 >= 0);

		//Calculation of f12 and making cell code
		currentXValue[0] = x1;
		currentXValue[1] = y2;
		f12 = calcF();
		cell_code += (f12 >= 0) * 2;

		f21 = f22;
		cell_code += (f21 >= 0) * 4;

		//Calculation of f22 and making cell code
		currentXValue[0] = x2;
		currentXValue[1] = y2;
		f22 = calcF();
		cell_code += (f22 >= 0) * 8;

		//Check if not inner or outer cell
		if ((cell_code > 0) && (cell_code < 15))
		{
			switch (cell_code)
			{
			case 1:
				if (first) {
					currentXValue[1] = y1;
					xstart = fx0(x2, x1);
					currentXValue[0] = x1;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y1, x1, fy0(y2, y1));//number_of_chains = 0

					segment_counter += 2;
					number_of_chains++;
				}
				else {
					currentXValue[0] = x1;
					addlink(column[cell_counter - 1], &stack[segment_counter++], x1, fy0(y2, y1), true, -1);
				}
				leftrow[cell_counter] = NULL;
				//column[cell_counter] = NULL;
				break;
			case 2:
				currentXValue[0] = x1;
				ystart = fy0(y1, y2);
				currentXValue[1] = y2;
				createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, x1, ystart, fx0(x2,x1), y2);

				leftrow[cell_counter] = NULL;
				column[cell_counter] = &chains[number_of_chains];

				segment_counter += 2;
				number_of_chains++;
				break;
			case 3:
				if (first) {
					currentXValue[1] = y1;
					xstart = fx0(x2, x1);
					currentXValue[1] = y2;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 0, number_of_chains, xstart, y1, fx0(x2, x1), y2);

					leftrow[cell_counter] = NULL;
					column[cell_counter] = &chains[number_of_chains];
					segment_counter += 2;
					number_of_chains++;
				}
				else {
					currentXValue[1] = y2;
					addlink(column[cell_counter - 1], &stack[segment_counter++], fx0(x2, x1), y2, true, 0);
					leftrow[cell_counter] = NULL;
					column[cell_counter] = column[cell_counter - 1];
				}
				break;
			case 4:
				if (first) {
					currentXValue[0] = x2;
					ystart = fy0(y2, y1);
					currentXValue[1] = y1;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, x2, ystart, fx0(x1, x2), y1);

					if (chains[number_of_chains].isLeftForFirst == NULL) {
						chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];
					}
					else chains[number_of_chains].isLeftForSecond = &leftrow[cell_counter];
					leftrow[cell_counter] = &chains[number_of_chains];
					//column[cell_counter] = NULL;
					segment_counter += 2;
					number_of_chains++;
				}
				else {
					currentXValue[0] = x2;
					addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y2, y1), false, -1);

					if (column[cell_counter - 1]->isLeftForFirst == NULL)
						column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
					else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];

					leftrow[cell_counter] = column[cell_counter - 1];
					//column[cell_counter] = NULL;
					column[cell_counter - 1] = NULL;
				}
				break;
			case 5:
				currentXValue[0] = x2;
				ystart = fy0(y2, y1);
				currentXValue[0] = x1;
				createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 0, number_of_chains, x2, ystart, x1, fy0(y2, y1));
				
				chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];

				leftrow[cell_counter] = &chains[number_of_chains];
				//column[cell_counter] = NULL;
				segment_counter += 2;
				number_of_chains++;
				break;
			case 6:
				currentXValue[0] = x2;
				ystart = fy0(y2, y1);
				if (ystart > hypY(y1, y2, f11, f12, f21, f22)) {
					if (first) {
						currentXValue[1] = y1;
						xstart = fx0(x2, x1); //it's xend in fact
						currentXValue[0] = x1;
						createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x1, fy0(y2, y1), xstart, y1);

						segment_counter += 2;
						number_of_chains++;
					}
					else {
						currentXValue[0] = x1;
						addlink(column[cell_counter - 1], &stack[segment_counter++], x1, fy0(y1, y2), false, 1);
						column[cell_counter - 1] = NULL;
					}
					segment_counter += 2;
					number_of_chains++;

					currentXValue[1] = y2;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x2, ystart, fx0(x2, x1), y2);

					chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];

					leftrow[cell_counter] = &chains[number_of_chains];
					column[cell_counter] = &chains[number_of_chains];


					segment_counter += 2;
					number_of_chains++;
				}
				else {
					if (first) {
						currentXValue[0] = x2;
						ystart = fy0(y2, y1);
						currentXValue[1] = y1;
						createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, x2, ystart, fx0(x1, x2), y1);

						leftrow[cell_counter] = &chains[number_of_chains];
						segment_counter += 2;
						number_of_chains++;
					}
					else {
						currentXValue[0] = x2;
						addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y2, y1), false, -1);

						if (column[cell_counter - 1]->isLeftForFirst == NULL)
							column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
						else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];

						leftrow[cell_counter] = column[cell_counter - 1];
						column[cell_counter - 1] = NULL;
					}

					currentXValue[0] = x1;
					ystart = fy0(y1, y2);
					currentXValue[1] = y2;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, x1, ystart, fx0(x1,x2), y2);

					segment_counter += 2;
					number_of_chains++;
					column[cell_counter] = &chains[number_of_chains];
				}
				break;
			case 7:
				currentXValue[0] = x2;
				ystart = fy0(y2, y1);
				currentXValue[1] = y2;
				createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x2, ystart, fx0(x2, x1), y2);
				
				chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];

				leftrow[cell_counter] = &chains[number_of_chains];
				column[cell_counter] = &chains[number_of_chains];

				segment_counter += 2;
				number_of_chains++;
				break;
			case 8:
				currentXValue[1] = y2;
				xstart = fx0(x1, x2);
				currentXValue[0] = x2;
				createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y2, x2, fy0(y1, y2));
				
				chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];

				leftrow[cell_counter] = &chains[number_of_chains];
				column[cell_counter] = &chains[number_of_chains];

				segment_counter += 2;
				number_of_chains++;
				break;
			case 9:
				currentXValue[1] = y1;
				xstart = fx0(x2, x1);
				if (xstart < hypX(x1, x2, f11, f12, f21, f22)) {
					if (first) {
						currentXValue[0] = x1;
						createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y1, x1, fy0(y2, y1));//number_of_chains = 0

						segment_counter += 2;
						number_of_chains++;
					}
					else {
						currentXValue[0] = x1;
						addlink(column[cell_counter - 1], &stack[segment_counter++], x1, fy0(y2, y1), true, -1);
					}

					currentXValue[1] = y2;
					xstart = fx0(x1, x2);
					currentXValue[0] = x2;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y2, x2, fy0(y1, y2));
					
					chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];
					
					leftrow[cell_counter] = &chains[number_of_chains];
					column[cell_counter] = &chains[number_of_chains];

					segment_counter += 2;
					number_of_chains++;
				}
				else {
					if (first) {
						currentXValue[0] = x2;
						createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, xstart, y1, x2, fy0(y1, y2));
						
						chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];
						
						leftrow[cell_counter] = &chains[number_of_chains];
						segment_counter += 2;
						number_of_chains++;
					}
					else {
						currentXValue[0] = x2;
						addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y1, y2), true, 1);
						
						if (column[cell_counter - 1]->isLeftForFirst == NULL)
							column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
						else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];

						leftrow[cell_counter] = column[cell_counter - 1];
						//column[cell_counter] = NULL;
						column[cell_counter - 1] = NULL;
					}

					currentXValue[0] = x1;
					ystart = fy0(y2, y1); //it's yend in fact
					currentXValue[1] = y2;

					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, fx0(x1, x2), y2, x1, ystart);

					column[cell_counter] = &chains[number_of_chains];
					segment_counter += 2;
					number_of_chains++;
				}
				break;
			case 10:
				currentXValue[0] = x1;
				ystart = fy0(y1, y2);
				currentXValue[0] = x2;
				createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 0, number_of_chains, x1, ystart, x2, fy0(y1, y2));
				
				chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];

				leftrow[cell_counter] = &chains[number_of_chains];
				//column[cell_counter] = NULL;
				segment_counter += 2;
				number_of_chains++;
				break;
			case 11:
				currentXValue[1] = y1;
				xstart = fx0(x2, x1);
				currentXValue[0] = x2;
				if (first) {
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, xstart, y1, x2, fy0(y1, y2));
					
					chains[number_of_chains].isLeftForFirst = &leftrow[cell_counter];

					leftrow[cell_counter] = &chains[number_of_chains];
					segment_counter += 2;
					number_of_chains++;
				}
				else {
					addlink(column[cell_counter - 1], &stack[segment_counter++], x2, fy0(y1, y2), true, 1);
					
					if (column[cell_counter - 1]->isLeftForFirst == NULL)
						column[cell_counter - 1]->isLeftForFirst = &leftrow[cell_counter];
					else column[cell_counter - 1]->isLeftForSecond = &leftrow[cell_counter];

					leftrow[cell_counter] = column[cell_counter - 1];
					column[cell_counter - 1] = NULL;
				}
				break;
			case 12:
				if (first) {
					currentXValue[1] = y2;
					xstart = fx0(x1, x2);
					currentXValue[1] = y1;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 0, number_of_chains, xstart, y2, fx0(x1, x2), y1);

					column[cell_counter] = &chains[number_of_chains];
					segment_counter += 2;
					number_of_chains++;
				}
				else {
					currentXValue[1] = y2;
					addlink(column[cell_counter - 1], &stack[segment_counter++], fx0(x1, x2), y2, false, 0);
					column[cell_counter] = column[cell_counter - 1];
				}
				leftrow[cell_counter] = NULL;
				break;
			case 13:
				currentXValue[0] = x1;
				ystart = fy0(y2, y1); //it's yend in fact
				currentXValue[1] = y2;
				createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, fx0(x1, x2), y2, x1, ystart);

				column[cell_counter] = &chains[number_of_chains];
				segment_counter += 2;
				number_of_chains++;
				leftrow[cell_counter] = NULL;
				break;
			case 14:
				if (first) {
					currentXValue[1] = y1;
					xstart = fx0(x2, x1); //it's xend in fact
					currentXValue[0] = x1;
					createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x1, fy0(y1, y2), xstart, y1);

					segment_counter += 2;
					number_of_chains++;
				}
				else {
					currentXValue[0] = x1;
					addlink(column[cell_counter - 1], &stack[segment_counter++], x1, fy0(y1, y2), false, 1);
					column[cell_counter - 1] = NULL;
				}
				leftrow[cell_counter] = NULL;
				//column[cell_counter] = NULL;

				segment_counter += 2;
				number_of_chains++;
				break;
			default: break;
			};
		}
		y1 = y2;
		first = false;
	}
	y1 = boundaries[1];
	y2 = boundaries[1];
	x1 = x2;
};

//Suppose that it runs after processFirstColumn
void HFSlicer::processLowerCell() {
	cell_code = 0;

	f11 = f12;
	cell_code += (f11 >= 0);

	//Calculation of f12 and making cell code
	currentXValue[0] = x1;
	currentXValue[1] = y2;
	f12 = calcF();
	cell_code += (f12 >= 0) * 2;

	f21 = f22;
	cell_code += (f21 >= 0) * 4;

	//Calculation of f22 and making cell code
	currentXValue[0] = x2;
	currentXValue[1] = y2;
	f22 = calcF();
	cell_code += (f22 >= 0) * 8;

	switch (cell_code)
	{
	case 1:
		//setting the value of y to calculate x value
		currentXValue[1] = y1;

		//adding the new piar of coordinates to the left chain
		addlink(leftrow[0], &stack[segment_counter++], fx0(x2, x1), y1, false, -1);
		column[0] = NULL;
		leftrow[0]->isLeftForFirst = NULL;
		leftrow[0]->isLeftForSecond = NULL;
		if (leftrow[0]->isLeftForFirst == &leftrow[0])
			leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
		leftrow[0]->isLeftForSecond = NULL;
		leftrow[0] = NULL;

		break;
	case 2:
		//setting the value of y to calculate x value
		currentXValue[1] = y2;

		//adding the new piar of coordinates to the left chain
		addlink(leftrow[0], &stack[segment_counter++], fx0(x2, x1), y2, true, -1);

		//setting the left chain as the last (bottom) for this cell
		column[0] = leftrow[0];

		//Deleting pointer to the left chain for this cell
		if (leftrow[0] != NULL) {
			if (&leftrow[0] == leftrow[0]->isLeftForFirst) {
				leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
				leftrow[0]->isLeftForSecond = NULL;
			}
			else leftrow[0]->isLeftForSecond = NULL;
			leftrow[0] = NULL;
		}
		break;
	case 3:
		currentXValue[1] = y1;
		xstart = fx0(x2, x1);
		currentXValue[1] = y2;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 0, number_of_chains, xstart, y1, fx0(x2, x1), y2);

		leftrow[0] = NULL;
		column[0] = &chains[number_of_chains];
		segment_counter += 2;
		number_of_chains++;
		break;
	case 4:
		currentXValue[0] = x2;
		ystart = fy0(y2, y1);
		currentXValue[1] = y1;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, x2, ystart, fx0(x1, x2), y1);

		chains[number_of_chains].isLeftForFirst = &leftrow[0];

		leftrow[0] = &chains[number_of_chains];
		column[0] = NULL;
		segment_counter += 2;
		number_of_chains++;
		break;
	case 5:
		currentXValue[0] = x2;
		addlink(leftrow[0], &stack[segment_counter++], x2, fy0(y2, y1), false, 0);
		//hint
		//leftrow[cell_counter] = leftrow[cell_counter];
		/*if (leftrow[0]->isLeftForFirst == NULL) {
			leftrow[0]->isLeftForFirst = &leftrow[0];
		}
		else leftrow[0]->isLeftForSecond = &leftrow[0];*/
		column[0] = NULL;
		break;
	case 6:
		currentXValue[0] = x2;
		ystart = fy0(y2, y1);
		if (ystart > hypY(y1, y2, f11, f12, f21, f22)) {
			currentXValue[1] = y1;
			addlink(leftrow[0], &stack[segment_counter++], fx0(x1, x2), y1, true, 1);

			if (&leftrow[0] == leftrow[0]->isLeftForFirst) {
				leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
				leftrow[0]->isLeftForSecond = NULL;
			}
			else leftrow[0]->isLeftForSecond = NULL;

			createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x2, ystart, fx0(x2, x1), y2);
			
			chains[number_of_chains].isLeftForFirst = &leftrow[0];
			
			leftrow[0] = &chains[number_of_chains];
			column[0] = &chains[number_of_chains];


			segment_counter += 2;
			number_of_chains++;
		}
		else {
			currentXValue[0] = x2;
			ystart = fy0(y2, y1);
			currentXValue[1] = y1;
			createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, x2, ystart, fx0(x1, x2), y1);
			chains[number_of_chains].isLeftForFirst = &leftrow[0];
			segment_counter += 2;
			number_of_chains++;

			currentXValue[1] = y2;
			addlink(leftrow[0], &stack[segment_counter++], fx0(x2, x1), y2, true, -1);
			if (&leftrow[0] == leftrow[0]->isLeftForFirst) {
				leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
				leftrow[0]->isLeftForSecond = NULL;
			}
			else leftrow[0]->isLeftForSecond = NULL;

			tmpbuffer = leftrow[0];
			leftrow[0] = &chains[number_of_chains];
			column[0] = tmpbuffer;
		}
		break;
	case 7:
		currentXValue[0] = x2;
		ystart = fy0(y2, y1);
		currentXValue[1] = y2;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, x2, ystart, fx0(x2, x1), y2);

		chains[number_of_chains].isLeftForFirst = &leftrow[0];
		
		leftrow[0] = &chains[number_of_chains];
		column[0] = &chains[number_of_chains];

		segment_counter += 2;
		number_of_chains++;
		break;
	case 8:
		currentXValue[1] = y2;
		xstart = fx0(x1, x2);
		currentXValue[0] = x2;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y2, x2, fy0(y1, y2));

		chains[number_of_chains].isLeftForFirst = &leftrow[0];
		
		leftrow[0] = &chains[number_of_chains];
		column[0] = &chains[number_of_chains];

		segment_counter += 2;
		number_of_chains++;
		break;
	case 9:
		currentXValue[1] = y1;
		xstart = fx0(x2, x1);
		if (xstart < hypX(x1, x2, f11, f12, f21, f22)) {
			//adding the new piar of coordinates to the left chain
			addlink(leftrow[0], &stack[segment_counter++], xstart, y1, false, -1);
			
			if (&leftrow[0] == leftrow[0]->isLeftForFirst) {
				leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
				leftrow[0]->isLeftForSecond = NULL;
			}
			else leftrow[0]->isLeftForSecond = NULL;

			currentXValue[1] = y2;
			xstart = fx0(x1, x2);
			currentXValue[0] = x2;
			createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], -1, number_of_chains, xstart, y2, x2, fy0(y1, y2));

			chains[number_of_chains].isLeftForFirst = &leftrow[0];
			
			leftrow[0] = &chains[number_of_chains];
			column[0] = &chains[number_of_chains];

			segment_counter += 2;
			number_of_chains++;
		}
		else {
			currentXValue[0] = x2;
			createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, xstart, y1, x2, fy0(y2, y1));

			currentXValue[1] = y2;
			addlink(leftrow[0], &stack[segment_counter++], fx0(x1, x2), y2, false, 1);

			if (&leftrow[0] == leftrow[0]->isLeftForFirst) {
				leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
				leftrow[0]->isLeftForSecond = NULL;
			}
			else leftrow[0]->isLeftForSecond = NULL;

			tmpbuffer = leftrow[0];
			
			chains[number_of_chains].isLeftForFirst = &leftrow[0];

			leftrow[0] = &chains[number_of_chains];

			column[0] = tmpbuffer;

			segment_counter += 2;
			number_of_chains++;
		}
		break;
	case 10:
		currentXValue[0] = x2;
		addlink(leftrow[0], &stack[segment_counter++], x2, fy0(y1, y2), true, 0);
		//leftrow[cell_counter] = leftrow[cell_counter];
		/*if (leftrow[0]->isLeftForFirst == NULL) {
			leftrow[0]->isLeftForFirst = &leftrow[0];
		}
		else leftrow[0]->isLeftForSecond = &leftrow[0];*/
		column[0] = NULL;
		break;
	case 11:
		currentXValue[1] = y1;
		xstart = fx0(x2, x1);
		currentXValue[0] = x2;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 1, number_of_chains, xstart, y1, x2, fy0(y1, y2));
		if (chains[number_of_chains].isLeftForFirst == NULL) {
			chains[number_of_chains].isLeftForFirst = &leftrow[0];
		}
		else chains[number_of_chains].isLeftForSecond = &leftrow[0];
		leftrow[0] = &chains[number_of_chains];
		segment_counter += 2;
		number_of_chains++;

		break;
	case 12:
		currentXValue[1] = y2;
		xstart = fx0(x1, x2);
		currentXValue[1] = y1;
		createchain(&chains[number_of_chains], &stack[segment_counter], &stack[segment_counter + 1], 0, number_of_chains, xstart, y2, fx0(x1, x2), y1);

		column[0] = &chains[number_of_chains];
		segment_counter += 2;
		number_of_chains++;
		break;
	case 13:
		currentXValue[1] = y2;
		addlink(leftrow[0], &stack[segment_counter++], fx0(x1, x2), y2, false, 1);
		column[0] = leftrow[0];
		if (&leftrow[0] == leftrow[0]->isLeftForFirst) {
			leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
			leftrow[0]->isLeftForSecond = NULL;
		}
		else leftrow[0]->isLeftForSecond = NULL;

		leftrow[0] = NULL;
		break;
	case 14:
		//setting the value of y to calculate x value
		currentXValue[1] = y1;

		//adding the new piar of coordinates to the left chain
		addlink(leftrow[0], &stack[segment_counter++], fx0(x1, x2), y1, true, 1);
		column[0] = NULL;
		if (&leftrow[0] == leftrow[0]->isLeftForFirst) {
			leftrow[0]->isLeftForFirst = leftrow[0]->isLeftForSecond;
			leftrow[0]->isLeftForSecond = NULL;
		}
		else leftrow[0]->isLeftForSecond = NULL;
		leftrow[0] = NULL;
		break;
	default: break;
	}
	y1 = y2;
};

int HFSlicer::sliceZ(double z) {

	currentXValue[2] = z;

	//Set number of segments to 0
	segment_counter = 0;

	//Cleaning stack array
	for (int i = 0; i < maxPoints; i++) {
		stack[i].next = NULL;
		stack[i].x = 0;
		stack[i].y = 0;
	}

	//Number of chains
	number_of_chains = 0;

	//Index in current column
	int cell_counter = 0;

	for (int i = 0; i < maxChains; i++) {
		chains[i].head = NULL;
		chains[i].tail = NULL;
		chains[i].orientation = 0;
		chains[i].isLeftForFirst = NULL;
		chains[i].isLeftForSecond = NULL;
	}

	processFirstColumn();

	while (x2 < boundaries[3]) {

		//shift to the next column
		x2 = x1 + delta[0];

		//setting the cell number to -1
		cell_counter = 0;

		//Initial �alculation of f12. It will be passed to the f11 from the next (1st) step
		currentXValue[0] = x1;
		currentXValue[1] = y1;
		f12 = calcF();

		//Initial �alculation of f22. It will be passed to the f21 from the next (1st) step
		currentXValue[0] = x2;
		currentXValue[1] = y1;
		f22 = calcF();

		y2 = y1 + delta[1];
		processLowerCell();

		while (y2 < boundaries[4]) {
			//counting the current number of cell in the column
			cell_counter++;

			//shift to the upper cell in the column
			y2 = y1 + delta[1];
			
			cell_code = 0;

			f11 = f12;
			cell_code += (f11 >= 0);

			//Calculation of f12 and making cell code
			currentXValue[0] = x1;
			currentXValue[1] = y2;
			f12 = calcF();
			cell_code += (f12 >= 0) * 2;

			f21 = f22;
			cell_code += (f21 >= 0) * 4;

			//Calculation of f22 and making cell code
			currentXValue[0] = x2;
			currentXValue[1] = y2;
			f22 = calcF();
			cell_code += (f22 >= 0) * 8;

			//Check if not inner or outer cell
			if ((cell_code > 0) && (cell_code < 15))
			{
				processCell(cell_counter);
			}
			y1 = y2;
		}
		y1 = boundaries[1];
		y2 = boundaries[1];
		x1 = x2;
	}

	//Cleaning leftrow array
	for (int i = 0; i < ylength + 1; i++) {
		leftrow[i] = NULL;
	}

	//Cleaning column array
	for (int i = 0; i < ylength; i++) {
		column[i] = NULL;
	}

	//set vertices of considering cell to min values
	x1 = boundaries[0];
	x2 = boundaries[0];
	y1 = boundaries[1];
	y2 = boundaries[1];

	//Initialize values of f(X) in vertices to 0
	f11 = 0;
	f12 = 0;
	f21 = 0;
	f22 = 0;

	tmpbuffer = NULL;

	if (number_of_chains > 0) {
		return 1;
	}
	return 0;
};

//printing "$$POLYLINE" line into the CLI file
void HFSlicer::printchain(ostream& toFile, const chain c) {

	//id
	toFile << "$$POLYLINE/" << c.number << ",";

	//dir
	if (c.orientation < 0) {
		toFile << "1,";
	}
	else if (c.orientation > 0) {
		toFile << "0,";
	}
	else toFile << "2,";

	//n
	int points_counter = 0;
	link* l = c.tail;

	while (l != c.head) {
		points_counter++;
		l = l->next;
	}
	points_counter += 2;
	
	toFile << points_counter << ",";

	//pix,piy
	l = c.tail;
	while (l != c.head) {
		toFile << l->x << "," << l->y << ",";
		l = l->next;
	}
	toFile << l->x << "," << l->y << ",";
	toFile << c.tail->x << "," << c.tail->y << "\n";
}

int HFSlicer::WriteCLILayer(ostream& toFile, double z)
{
	
	int result = sliceZ(z);
	if (result)
	{
		toFile << "$$LAYER/" << z << "\n";
		for (int i = 0; i < number_of_chains; i++) {
			if ((chains[i].tail != NULL) && (chains[i].head != NULL)) {
				printchain(toFile, chains[i]);
			}
		}
	}
	
	return result;
};