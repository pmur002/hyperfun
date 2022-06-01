/*==============================================================================

Copyright 1999 Eric Fausett
Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt 
Copyright 2007-2008 Oleg Fryazinov

This Work or file is part of the greater total Work, software or group of
files named HyperFun Polygonizer.

The implemented polygonization algorithm is described in

Pasko A.A., Pilyugin V.V., Pokrovskiy V.N. 
"Geometric modeling in the analysis of trivariate functions", 
Communications of Joint Insititute of Nuclear Research, P10-86-310, 
Dubna, Russia, 1986 (in Russian).

Pasko A.A., Pilyugin V.V., Pokrovskiy V.N. 
"Geometric modeling in the analysis of trivariate functions",
Computers and Graphics, vol.12, Nos.3/4, 1988, pp.457-465. 

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


#include "hfpolymesh.h"
//* Refinement

/*
Problem statement: Some vertices in the polygonal mesh may have negative function value (due to the polygonal approximation. It means that attributes corresponding to this vertex
may not be defined.

Proposed solution: very simple:
1-Get the cell corresponding to this vertex (at least one)
2-Initialise the distance min dmin to arbitrary large value 
3-For each vertex of the cell, 

a-if the function value is positive (itsBoolGrid), calculate the distance of the polygon vertex and the current cell vertex
b--If the dist is lower than the distance min, get the attributes corresponding to this cell vertex (not stored, need to call itsIntrepreter
4-Store in itsData the attributes value previously stored


Remarks:
1-Very simple solution. Works fine. What happen for cells on the boundary? Fine because the grid is extended, the cast double to int takes the lower integer part
2-Will change (and removed) when the search option will be added to the polygonaliser (each vertex of the polygonal model will have a positive value).
*/

using namespace std;

void HFPolyMesh::Refinement()
{
	//int N=0;
	int V=itsData.getNumV();
	int i=0, j=0;

	double *tmpSAttributes =new double[itsSSize];

	double f=0;

	bool found;

	double dst, dstMin=99999.1;

	vector<double> vertex(3,0.0);
	vector<double> gridvertex(3,0.0);
	int CellIndex[3];

	for(i = 0; i < V; i++){
		f = itsData.getFunctVal(i);
		if(f < 0.0) {
			//Get the cell corresponding to this vertex. (ie one of the cell)
			vertex = itsData.getVertex(i);

			CellIndex[0] = (vertex[0]-itsBBMin[0])/itsDelta[0];
			CellIndex[1] = (vertex[1]-itsBBMin[1])/itsDelta[1];
			CellIndex[2] = (vertex[2]-itsBBMin[2])/itsDelta[2];

			gridvertex[0] = itsBBMin[0]+CellIndex[0]*itsDelta[0];
			gridvertex[1] = itsBBMin[1]+CellIndex[1]*itsDelta[1];
			gridvertex[2] = itsBBMin[2]+CellIndex[2]*itsDelta[2];

			dstMin=99999.1;

			found = false;
			for(int ii = 0; ii < 2; ii++){
				if(CellIndex[0]+ii <= itsGridSize[0]){
					gridvertex[0] = itsBBMin[0]+(CellIndex[0]+ii)*itsDelta[0];
					for(int jj = 0; jj < 2; jj++){
						if(CellIndex[1]+jj <= itsGridSize[1]){
							gridvertex[1] = itsBBMin[1]+(CellIndex[1]+jj)*itsDelta[1];
							for(int kk = 0; kk < 2; kk++){
								if(CellIndex[2]+kk <= itsGridSize[2]){
									gridvertex[2] = itsBBMin[2]+(CellIndex[2]+kk)*itsDelta[2];

									if(itsBoolGrid[CellIndex[0]+ii][CellIndex[1]+jj][CellIndex[2]+kk]){
										dst = (vertex[0]-gridvertex[0])*(vertex[0]-gridvertex[0])
											+(vertex[1]-gridvertex[1])*(vertex[1]-gridvertex[1])
											+(vertex[2]-gridvertex[2])*(vertex[2]-gridvertex[2]);

										if(dst < dstMin){
											found = true;
											dstMin = dst;
											f = itsInterpreter.calc(gridvertex,tmpSAttributes);
											for(int h=0;h<itsSSize;h++)
												itsSAttributesM[h] =tmpSAttributes[h];
										}
									}
								}
							}
						}
					}
				}
			}

			if(found) {
				//*Set the set of attributes, corresponding to the closest cell vertex with positive value.
				itsData.setAttributes(i,itsSAttributesM);
			}
		}
	}


	delete[] tmpSAttributes;

}

void HFPolyMesh::GeneralRefinement(void)
{
	//int N=0;
	int V=itsData.getNumV();
	int i=0, j=0;

	double *tmpSAttributes =new double[itsSSize];

	double f=0;

	bool found;

	double dst, dstMin=99999.1;

	vector<double> vertex(3,0.0);
	vector<double> gridvertex(3,0.0);
	int CellIndex[3];

	for (i = 0; i<3; i++)
		itsDelta[i]=fabs((itsBBMax[i]-itsBBMin[i])/itsGridSize[i]);

	for (i = 0; i < V; i++){
		f = itsData.getFunctVal(i);
		if (f >= 0.0)
			continue;
		//Get the cell corresponding to this vertex. (ie one of the cell)
		vertex = itsData.getVertex(i);

		CellIndex[0] = (vertex[0]-itsBBMin[0])/itsDelta[0];
		CellIndex[1] = (vertex[1]-itsBBMin[1])/itsDelta[1];
		CellIndex[2] = (vertex[2]-itsBBMin[2])/itsDelta[2];

		gridvertex[0] = itsBBMin[0]+CellIndex[0]*itsDelta[0];
		gridvertex[1] = itsBBMin[1]+CellIndex[1]*itsDelta[1];
		gridvertex[2] = itsBBMin[2]+CellIndex[2]*itsDelta[2];

		dstMin=99999.1;

		found = false;
		for (int ii = 0; ii < 2; ii++){
			if (CellIndex[0]+ii <= itsGridSize[0]){
				gridvertex[0] = itsBBMin[0]+(CellIndex[0]+ii)*itsDelta[0];
				for (int jj = 0; jj < 2; jj++){
					if (CellIndex[1]+jj <= itsGridSize[1]){
						gridvertex[1] = itsBBMin[1]+(CellIndex[1]+jj)*itsDelta[1];
						for (int kk = 0; kk < 2; kk++){
							if (CellIndex[2]+kk <= itsGridSize[2]){
								gridvertex[2] = itsBBMin[2]+(CellIndex[2]+kk)*itsDelta[2];

								if (itsInterpreter.calc(gridvertex, tmpSAttributes)>itsS0){
									dst = (vertex[0]-gridvertex[0])*(vertex[0]-gridvertex[0])
										+(vertex[1]-gridvertex[1])*(vertex[1]-gridvertex[1])
										+(vertex[2]-gridvertex[2])*(vertex[2]-gridvertex[2]);

									if (dst < dstMin){
										found = true;
										dstMin = dst;
										f = itsInterpreter.calc(gridvertex,tmpSAttributes);
										for (int h = 0; h < itsSSize; h++)
											itsSAttributesM[h] =tmpSAttributes[h];
									}
								}
							}
						}
					}
				}
			}
		}

		if(found) 
		{
			//*Set the set of attributes, corresponding to the closest cell vertex with positive value.
			itsData.setAttributes(i,itsSAttributesM);
		}
	}


	delete[] tmpSAttributes;
}