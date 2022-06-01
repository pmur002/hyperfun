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

void HFPolyMesh::StoreCell(int i, int j, int k, int select){
	int t[3]={i,j,k};
	t[select]+=1;
	switch(select){
		case 0:
			//*** Cell i,j,k Face 4, 6 ***
			AddFace4ToCell(t[0], t[1], t[2]);
			AddFace6ToCell(t[0], t[1], t[2]);
			//*** Cell i,j+1,k Face 3, 4 ***
			AddFace3ToCell(t[0], t[1]+1, t[2]);
			AddFace4ToCell(t[0], t[1]+1, t[2]);
			//*** Cell i,j+1,k+1 Face 1, 3 ***
			AddFace1ToCell(t[0], t[1]+1, t[2]+1);
			AddFace3ToCell(t[0], t[1]+1, t[2]+1);
			//*** Cell i,j,k+1 Face 1, 6 ***
			AddFace1ToCell(t[0], t[1], t[2]+1);
			AddFace6ToCell(t[0], t[1], t[2]+1);
			break;
		case 1:
			//*** Cell i,j,k Face 4, 5 ***
			AddFace4ToCell(t[0], t[1], t[2]);
			AddFace5ToCell(t[0], t[1], t[2]);
			//*** Cell i+1,j,k Face 2, 4 ***
			AddFace2ToCell(t[0]+1, t[1], t[2]);
			AddFace4ToCell(t[0]+1, t[1], t[2]);
			//*** Cell i+1,j,k+1 Face 1, 2 ***
			AddFace1ToCell(t[0]+1, t[1], t[2]+1);
			AddFace2ToCell(t[0]+1, t[1], t[2]+1);
			//*** Cell i,j,k+1 Face 1, 5 ***
			AddFace1ToCell(t[0], t[1], t[2]+1);
			AddFace5ToCell(t[0], t[1], t[2]+1);
			break;
		case 2:
			//*** Cell i,j,k Face 5, 6 ***
			AddFace5ToCell(t[0], t[1], t[2]);
			AddFace6ToCell(t[0], t[1], t[2]);
			//*** Cell i,j+1,k Face 3, 5 ***
			AddFace3ToCell(t[0], t[1]+1, t[2]);
			AddFace5ToCell(t[0], t[1]+1, t[2]);
			//*** Cell i+1,j+1,k Face 2, 3 ***
			AddFace2ToCell(t[0]+1, t[1]+1, t[2]);
			AddFace3ToCell(t[0]+1, t[1]+1, t[2]);
			//*** Cell i+1,j,k Face 2, 6 ***
			AddFace2ToCell(t[0]+1, t[1], t[2]);
			AddFace6ToCell(t[0]+1, t[1], t[2]);
			break;
		default:
			break;
		}
	}

