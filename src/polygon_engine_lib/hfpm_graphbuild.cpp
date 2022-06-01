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

void HFPolyMesh::GraphBuildCase2(const int* Edge, int* I[12]){
	int E[2];
	int C=0;
	//*** Cycle Through Face Edges ***
	for(int A=0; A<4; A++){
		//*** If Edge is not Empty Make it a connection Edge ***
		if(*I[Edge[A]]>=0) E[C++]=Edge[A];
		}
	//*** Connect the Found Edges ***

	if(itsConnect[E[0]][0]<0) itsConnect[E[0]][0]=E[1];
	else itsConnect[E[0]][1]=E[1];

	if(itsConnect[E[1]][0]<0) itsConnect[E[1]][0]=E[0];
	else itsConnect[E[1]][1]=E[0];
	}

void HFPolyMesh::GraphBuildCase4(double V, double A1, double A2, const int* Edge){
	if(V<(A1/A2)){
		if(itsConnect[Edge[0]][0]<0) itsConnect[Edge[0]][0]=Edge[1];
		else itsConnect[Edge[0]][1]=Edge[1];

		if(itsConnect[Edge[1]][0]<0) itsConnect[Edge[1]][0]=Edge[0];
		else itsConnect[Edge[1]][1]=Edge[0];

		if(itsConnect[Edge[2]][0]<0) itsConnect[Edge[2]][0]=Edge[3];
		else itsConnect[Edge[2]][1]=Edge[3];

		if(itsConnect[Edge[3]][0]<0) itsConnect[Edge[3]][0]=Edge[2];
		else itsConnect[Edge[3]][1]=Edge[2];
		}
	else{
		if(itsConnect[Edge[0]][0]<0) itsConnect[Edge[0]][0]=Edge[3];
		else itsConnect[Edge[0]][1]=Edge[3];

		if(itsConnect[Edge[3]][0]<0) itsConnect[Edge[3]][0]=Edge[0];
		else itsConnect[Edge[3]][1]=Edge[0];

		if(itsConnect[Edge[2]][0]<0) itsConnect[Edge[2]][0]=Edge[1];
		else itsConnect[Edge[2]][1]=Edge[1];

		if(itsConnect[Edge[1]][0]<0) itsConnect[Edge[1]][0]=Edge[2];
		else itsConnect[Edge[1]][1]=Edge[2];
		}
	}

