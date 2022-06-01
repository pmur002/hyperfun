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

void HFPolyMesh::FillIndex(int i, int j, int k, int* I[12]){
	I[0] = &itsVertGrid[1][i-1]	[j-1]	[k-1];
	I[1] = &itsVertGrid[0][i-1]	[j]		[k-1];
	I[2] = &itsVertGrid[1][i]	[j-1]	[k-1];
	I[3] = &itsVertGrid[0][i-1]	[j-1]	[k-1];
	I[4] = &itsVertGrid[1][i-1]	[j-1]	[k];
	I[5] = &itsVertGrid[0][i-1]	[j]		[k];
	I[6] = &itsVertGrid[1][i]	[j-1]	[k];
	I[7] = &itsVertGrid[0][i-1]	[j-1]	[k];
	I[8] = &itsVertGrid[2][i-1]	[j-1]	[k-1];
	I[9] = &itsVertGrid[2][i-1]	[j]		[k-1];
	I[10]= &itsVertGrid[2][i]	[j]		[k-1];
	I[11]= &itsVertGrid[2][i]	[j-1]	[k-1];	
	}

