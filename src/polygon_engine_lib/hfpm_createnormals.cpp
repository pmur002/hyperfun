/*==============================================================================

Copyright 1999 Eric Fausett
Copyright 2003-2004 Benjamin Schmitt 
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
using namespace std;

/*********Modified by Schmitt 03-05-2001*/
void HFPolyMesh::CreateVNormals(){
	/**ATTRIBUTES : IS IT NEEDED? I DO NOT THINK SO... CHECK LATER!!!*/

	itsData.initVNormal();
	vector<double> norm(3,0.0);
	int count=itsData.getNumV();
	for(int i=0; i<count; i++){
		//Calculate a normal
		CalcVNormal(i,&norm);
		//Save Normal
		itsData.setVNormal(i, norm);
	}
	itsData.ccwTriangles();
}

void HFPolyMesh::CreateTNormals()
{
	vector<double> norm(3,0.0);
	itsData.initTNormal();

	for(int i = 0; i < itsData.getNumT(); i++){
		//Calculate a normal
		norm = itsData.calcTNormal(i);
		//Save Normal
		itsData.setTNormal(i, norm);
	}
}

/***FOR ATTRIBUTES : to be more clear and for debuging,
I did not add a parameter to this function (the S array).
It is not needed, because S blelongs to the PolyMesh class.
*/ 
double HFPolyMesh::CalcVal(const double pos[3]){
	itsSendingX[itsXMap]=pos[0];
	itsSendingX[itsYMap]=pos[1];
	itsSendingX[itsZMap]=pos[2];
	/*******ADDED FOR S ATTRIBUTES*/
	//Reset of the array S
	//for(int i=0;i<itsSSize;i++) itsSAttributes[i]=0.0;
	return itsInterpreter.calc(itsSendingX,itsSAttributesM);
}

/******For Attibutes : During the refinement process
/******vertices where the function value is negative
/******due to the polygonalisation, are replaced 
/******with another close vertices where the function 
/****** value becomes positive. Thus, a new normal is need.
*/

void HFPolyMesh::CalcVNormal(int i,vector<double> *norm)
{

	double val=itsData.getFunctVal(i);
	double pos[3];
	pos[0]=itsSendingX[itsXMap]=itsData.getVertex(i,0);
	pos[1]=itsSendingX[itsYMap]=itsData.getVertex(i,1);
	pos[2]=itsSendingX[itsZMap]=itsData.getVertex(i,2);
	//XPoint
	itsSendingX[itsXMap]+=itsNormDelt[0];
	(*norm)[0]=-((itsInterpreter.calc(itsSendingX,itsSAttributesM)-val)/itsNormDelt[0]);
	//YPoint
	itsSendingX[itsYMap]+=itsNormDelt[1];
	itsSendingX[itsXMap]=pos[0];
	(*norm)[1]=-((itsInterpreter.calc(itsSendingX,itsSAttributesM)-val)/itsNormDelt[1]);
	//ZPoint
	itsSendingX[itsZMap]+=itsNormDelt[2];
	itsSendingX[itsYMap]=pos[1];
	(*norm)[2]=-((itsInterpreter.calc(itsSendingX,itsSAttributesM)-val)/itsNormDelt[2]);
	//Normalize
	double length=sqrt((*norm)[0]*(*norm)[0] + (*norm)[1]*(*norm)[1] + (*norm)[2]*(*norm)[2]);
	if(length==0.0) cout << "Normal Length of Zero Error!\n";
	else{
		(*norm)[0]/=length;
		(*norm)[1]/=length;
		(*norm)[2]/=length;
	}
}

void HFPolyMesh::CalcVNormal(double x, double y, double z, double& nx, double& ny, double& nz)
{
	double pos[3];
	pos[0]=x;
	pos[1]=y;
	pos[2]=z;
	double val = CalcVal(pos);
	//XPoint
	itsSendingX[itsXMap]+=itsNormDelt[0];
	nx=-((itsInterpreter.calc(itsSendingX,itsSAttributesM)-val)/itsNormDelt[0]);
	//YPoint
	itsSendingX[itsYMap]+=itsNormDelt[1];
	itsSendingX[itsXMap]=pos[0];
	ny=-((itsInterpreter.calc(itsSendingX,itsSAttributesM)-val)/itsNormDelt[1]);
	//ZPoint
	itsSendingX[itsZMap]+=itsNormDelt[2];
	itsSendingX[itsYMap]=pos[1];
	nz=-((itsInterpreter.calc(itsSendingX,itsSAttributesM)-val)/itsNormDelt[2]);
	//Normalize
	double length=sqrt(nx*nx + ny*ny + nz*nz);
	if(length==0.0) cout << "Normal Length of Zero Error!\n";
	else{
		nx/=length;
		ny/=length;
		nz/=length;
	}
}
