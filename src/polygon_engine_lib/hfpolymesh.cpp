/*==============================================================================

 Copyright 1999 Eric Fausett
 Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt 

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

// HFPolyMesh.cpp: implementation of the HFPolyMesh class.

//

//////////////////////////////////////////////////////////////////////



#include "hfpolymesh.h"

using namespace std;

HFPolyMesh::~HFPolyMesh(){

  delete[] itsSAttributesM;

	}



HFPolyMesh::HFPolyMesh(HFInterpreter& interp):itsInterpreter(interp){

	short i;



	itsS0=0.0;

	itsTimer=false;



	itsSearch=false;

	itsSearchPer=0.01;



	itsNormals=false;



	if(itsInterpreter.getXSize()<3) throw HFPError("Dimension is less than 3");

	

	//Reset it's XYZ Mapping to defaults

		itsXMap=0;

		itsYMap=1;

		itsZMap=2;

		

	//Reset Bounding Box to defaults

	for(i=0; i<3; i++){

		itsBBMin[i]=-10;

		itsBBMax[i]=10;

		}

		

	//Reset its Grid Sizes to defaults

	for(i=0; i<3; i++)

		itsGridSize[i]=30;



	//Set up its SendingX with defaults

	itsSendingX.resize(itsInterpreter.getXSize(), 0.0);

	

	//Parameters are defaulted already

	

	//Set it's epsilon value

	itsEPS=fabs(itsS0*EPS);

	if(itsS0==0.0) itsEPS=EPS;



	/******ADDED FOR S ATTRIBUTES******/

	itsSSize = itsInterpreter.getSSize();

	itsSAttributesM =new double[itsSSize];
	
	

	itsRefinement = 0;



	}



int HFPolyMesh::VertexNum(){

	return itsData.getNumV();

	}



int HFPolyMesh::TriangleNum(){ 

	return itsData.getNumT();

	}



void HFPolyMesh::Timer(bool t){

	itsTimer=t;

	}



bool HFPolyMesh::Timer(){

	return itsTimer;

	}



void HFPolyMesh::Search(bool s){

	itsSearch=s;

	}



bool HFPolyMesh::Search(){

	return itsSearch;

	}



void HFPolyMesh::SearchPercent(double p){

	itsSearchPer=p;

	}



double HFPolyMesh::SearchPercent(){

	return itsSearchPer;

	}



void HFPolyMesh::Normals(bool s){

	itsNormals=s;

	}



bool HFPolyMesh::Normals(){

	return itsNormals;

	}



void HFPolyMesh::Reduce(bool r){

	itsReduce=r;

	}



bool HFPolyMesh::Reduce(){

	return itsReduce;

	}



void HFPolyMesh::IsoValue(double isov){ 

	itsS0=isov;

	}



double HFPolyMesh::IsoValue(){ 

	return itsS0;

	}



void HFPolyMesh::Refine(bool r){



	itsRefinement = r;

}

	

bool HFPolyMesh::Refine(){

	return itsRefinement;

}





void HFPolyMesh::MinMax(vector<double> mm){ 

	short i;

	//Set up its Bounding Box

	if(mm.size()<6){//Reset to defaults

		for(i=0; i<3; i++){

			itsBBMin[i]=-10;

			itsBBMax[i]=10;

			}

		}

	else{

		for(i=0; i<3; i++) 

			itsBBMin[i]=mm[i];

		for(; i<6; i++)

			itsBBMax[i-3]=mm[i];

		if((itsBBMin[0]>=itsBBMax[0]) || (itsBBMin[1]>=itsBBMax[1]) || (itsBBMin[2]>=itsBBMax[2])){//Reset to defaults

			for(i=0; i<3; i++){

				itsBBMin[i]=-10;

				itsBBMax[i]=10;

				}

			}

		}

	}



vector<double> HFPolyMesh::MinMax(){

	vector<double> mmret(6, 0.0);

	short i;

	for(i=0; i<3; i++) 

		mmret[i]=itsBBMin[i];

	for(; i<6; i++)

		mmret[i]=itsBBMax[i-3];

	return mmret;

	}

vector<double> HFPolyMesh::Delta() {

	vector<double> dret(3, 0.0);

	for (short i = 0; i<3; i++)

		dret[i] = itsDelta[i];

	return dret;
}



void HFPolyMesh::Grid(vector<int> g){

	short i;

	//Set up its Grid Sizes

	if(g.size()<3){//Reset to defaults

		for(i=0; i<3; i++)

			itsGridSize[i]=30;

		}

	else{

		for(i=0; i<3; i++) 

			itsGridSize[i]=g[i];

		if((itsGridSize[0]<2) || (itsGridSize[1]<2) || (itsGridSize[2]<2)){//Reset to defaults

			for(i=0; i<3; i++)

				itsGridSize[i]=30;

			}

		}

	}



vector<int> HFPolyMesh::Grid(){

	short i;

	vector<int> gret(3, 0);

	for(i=0; i<3; i++)

		gret[i]=itsGridSize[i];

	return gret;

	}



void HFPolyMesh::DimMap(vector<int> dm){

	//Set up it's XYZ Mapping

	itsXMap=dm[0];

	itsYMap=dm[1];

	itsZMap=dm[2];

	if((itsXMap>=itsInterpreter.getXSize()) || (itsYMap>=itsInterpreter.getXSize()) 

		|| (itsZMap>=itsInterpreter.getXSize())){//Reset to defaults

		itsXMap=0;

		itsYMap=1;

		itsZMap=2;

		}

	if((itsXMap==-1) || (itsYMap==-1) || (itsXMap==-1)){//Reset to defaults

		itsXMap=0;

		itsYMap=1;

		itsZMap=2;

		}

	}



vector<int> HFPolyMesh::DimMap(){

	vector<int> ret;

	ret.push_back(itsXMap);

	ret.push_back(itsYMap);

	ret.push_back(itsZMap);

	return ret;

	}



void HFPolyMesh::Constants(vector<double> con){

	for(int i=0; i<itsInterpreter.getXSize() && i<con.size(); i++){

		itsSendingX[i]=con[i];

		}

	}



vector<double> HFPolyMesh::Constants(){

	return itsSendingX;

	}



void HFPolyMesh::Parameters(vector<double> par){
	itsInterpreter.parameters(par);
	}



vector<double> HFPolyMesh::Parameters(){

	return itsInterpreter.parameters();

	}







