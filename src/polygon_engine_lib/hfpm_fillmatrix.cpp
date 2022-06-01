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

void HFPolyMesh::FillMatrix(){
	int VRCTMP=0;
	//*** Fill itsValGrid with function values ***
	std::cout << "Filling Function Grid \n";
	double Per=0.0;
	double Add=(double)(100.0/(double)itsGridSize[0]);
	int PerOut=0;

	itsSendingX[itsXMap]=itsBBMin[0];
	for(int i=0;i<=itsGridSize[0];i++){

		PerOut=(int)Per;
		std::cout << "\b\b\b\b\b\b\b" << PerOut << "%   ";
		Per+=Add;

		itsSendingX[itsYMap]=itsBBMin[1];
		for(int j=0;j<=itsGridSize[1];j++){
			itsSendingX[itsZMap]=itsBBMin[2];
			for(int k=0;k<=itsGridSize[2];k++){

				/***ADDED FOR S ATTRIBUTES*/
				//reset of the S array. But is it needed??
				for(int p=0;p<itsSSize;p++) itsSAttributesM[p]=0.0;
				itsValGrid[i][j][k]=itsInterpreter.calc(itsSendingX,itsSAttributesM);
				
				//*** Change Any Points That Lie on the IsoSurface ***
				if(fabs(itsValGrid[i][j][k]-itsS0)<itsEPS){ 
					itsValGrid[i][j][k]+=(10.0*itsEPS);
					VRCTMP++;
					}
				//*** Fill itsBoolGrid ***
				itsBoolGrid[i][j][k]=itsValGrid[i][j][k]>itsS0;

				itsSendingX[itsZMap]+=itsDelta[2];
				}
			itsSendingX[itsYMap]+=itsDelta[1];
			}
		itsSendingX[itsXMap]+=itsDelta[0];
		}

	std::cout << "\b\b\b\b\b\b\b" << 100 << "%   ";
	std::cout << "\n\n";
	std::cout << "RARE CASE: " << VRCTMP << "\n\n";
	}

