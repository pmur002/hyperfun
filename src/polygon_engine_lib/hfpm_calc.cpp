/*==============================================================================

 Copyright 1999 Eric Fausett
 Copyright 2003-2004 Benjamin Schmitt 

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
using namespace std;

void HFPolyMesh::Calc(){
	if(itsTimer) itsT.start("Time for Initialization of Data Structures");

	//****** IMPORTANT: int itsGridSize[] are number of cells, so number sample points is itsGridSize[]+1 ******
	//*** Temp Vectors Used For Initilization ***
	vector<double> tmpd2(itsGridSize[2]+1, 0.0);
	vector< vector<double> > tmpd1(itsGridSize[1]+1, tmpd2);
	//*** Initialize itsValGrid to right size itsGridSize[]+1 ***
	itsValGrid.resize(itsGridSize[0]+1, tmpd1);
	//*** Clean up Temp Vectors ***
	tmpd1.clear();
	tmpd2.clear();

	//*** Temp Vectors Used For Initilization ***
	vector<bool> tmpb2(itsGridSize[2]+1, false);
	vector< vector<bool> > tmpb1(itsGridSize[1]+1, tmpb2);
	//*** Initialize itsBoolGrid to right size itsGridSize[]+1 ***
	itsBoolGrid.resize(itsGridSize[0]+1, tmpb1);
	//*** Clean up Temp Vectors ***
	tmpb1.clear();
	tmpb2.clear();

	//*** Temp Vectors Used For Initilization ***
	vector<int> tmpi3(itsGridSize[2]+1, -1);
	vector< vector<int> > tmpi2(itsGridSize[1]+1, tmpi3);
	vector< vector< vector<int> > > tmpi1(itsGridSize[0]+1, tmpi2);
	//*** Initialize its(X,Y,Z)VertGrids to right size itsGridSize[]+1 ***
	itsVertGrid.resize(3, tmpi1);
	//*** Clean up Temp Vectors ***
	tmpi1.clear();
	tmpi2.clear();
	tmpi3.clear();

	//*** Temp Vectors Used For Initilization ***
	vector<int> tmpii2(itsGridSize[2]+2, 0);
	vector< vector<int> > tmpii1(itsGridSize[1]+2, tmpii2);
	//*** Initialize itsCellGrid to right size itsGridSize[]+1 ***
	itsCellGrid.resize(itsGridSize[0]+2, tmpii1);
	//*** Clean up Temp Vectors ***
	tmpii1.clear();
	tmpii2.clear();

	//*** Initialize itsConnect
	vector<int> tmpcv(2, -1);
	itsConnect.resize(12, tmpcv);
	tmpcv.clear();
	
	//*** Calculate x, y, and z Deltas ***
	itsNormDelt.resize(3, 0.0);
	for(int i=0; i<3; i++){
		itsDelta[i]=fabs((itsBBMax[i]-itsBBMin[i])/itsGridSize[i]);
		itsNormDelt[i]=NORMDELTMULT*itsDelta[i];
		}

	//*** Initialize DupVert ***
	vector<int> tvi(itsGridSize[2]+2, -1);
	vector<vector<int> > tvvi(itsGridSize[1]+2, tvi);
	DupVert.resize(itsGridSize[0]+2, tvvi);
	tvi.clear();
	tvvi.clear();

	//*** Calculate MinEdgeLen
	itsMinEdgeLen=0.5*itsDelta[0];
	double tmpMEL=0.5*itsDelta[1];
	if(tmpMEL<itsMinEdgeLen) itsMinEdgeLen=tmpMEL;
	tmpMEL=0.5*itsDelta[2];
	if(tmpMEL<itsMinEdgeLen) itsMinEdgeLen=tmpMEL;

	//*** Calculate guess for num of triangles and vertices
	int averageGridSize=(int)((itsGridSize[0]+itsGridSize[1]+itsGridSize[2])/3);
	int tguess=(int)8*averageGridSize*averageGridSize;
	int vguess=(int)tguess/2;
	itsData.guessSize(tguess, vguess,itsSSize);

	if(itsTimer) itsT.stop();
	
	//*** Fill itsValGrid With Function Values and itsBoolGrid ***
	if(itsTimer) itsT.start("Time to Fill function matrix");
	FillMatrix();
	if(itsTimer) itsT.stop();

	//*** Calculate Vertices, Fill VertGrids, Fill CellGrid ***
	if(itsTimer) itsT.start("Time to Calculate Vertices");
	FillVertices();
	if(itsTimer) itsT.stop();

	//*** Create Triangles ***
	if(itsTimer) itsT.start("Time to Create Triangles from Vertices");
	CreateTriangles();
	if(itsTimer) itsT.stop();

	//*** Calculate Normals ***
	if(itsNormals){
		if(itsTimer) itsT.start("Time to Calculate Normals for Vertices");
		CreateVNormals();
		CreateTNormals();
		if(itsTimer) itsT.stop();
		}

	

	if(itsRefinement){
		if(!itsNormals){
			std::cout<<"Normals calculation required for Attributes..."<<endl;
			if(itsTimer) itsT.start("Time to Calculate Normals for Vertices");
				CreateVNormals();
				CreateTNormals();
			if(itsTimer) itsT.stop();
		}
		if(itsTimer) itsT.start("Time for Refinement for Attributes :");
			Refinement();
		if(itsTimer) itsT.stop();
	}
	
}	










