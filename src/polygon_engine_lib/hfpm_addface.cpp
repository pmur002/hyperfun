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

void HFPolyMesh::AddFace1ToCell(int li, int lj, int lk){
	switch(itsCellGrid[li][lj][lk]&F1_4){
		case F1_0:
			itsCellGrid[li][lj][lk]|=F1_1;
			break;
		case F1_1:
			itsCellGrid[li][lj][lk]&= ~F1_1;
			itsCellGrid[li][lj][lk]|=F1_2;
			break;
		case F1_2:
			itsCellGrid[li][lj][lk]|=F1_4;
			break;
		default:
			break;
		}
	}

void HFPolyMesh::AddFace2ToCell(int li, int lj, int lk){
	switch(itsCellGrid[li][lj][lk]&F2_4){
		case F2_0:
			itsCellGrid[li][lj][lk]|=F2_1;
			break;
		case F2_1:
			itsCellGrid[li][lj][lk]&= ~F2_1;
			itsCellGrid[li][lj][lk]|=F2_2;
			break;
		case F2_2:
			itsCellGrid[li][lj][lk]|=F2_4;
			break;
		default:
			break;
		}
	}

void HFPolyMesh::AddFace3ToCell(int li, int lj, int lk){
	switch(itsCellGrid[li][lj][lk]&F3_4){
		case F3_0:
			itsCellGrid[li][lj][lk]|=F3_1;
			break;
		case F3_1:
			itsCellGrid[li][lj][lk]&= ~F3_1;
			itsCellGrid[li][lj][lk]|=F3_2;
			break;
		case F3_2:
			itsCellGrid[li][lj][lk]|=F3_4;
			break;
		default:
			break;
		}
	}

void HFPolyMesh::AddFace4ToCell(int li, int lj, int lk){
	switch(itsCellGrid[li][lj][lk]&F4_4){
		case F4_0:
			itsCellGrid[li][lj][lk]|=F4_1;
			break;
		case F4_1:
			itsCellGrid[li][lj][lk]&= ~F4_1;
			itsCellGrid[li][lj][lk]|=F4_2;
			break;
		case F4_2:
			itsCellGrid[li][lj][lk]|=F4_4;
			break;
		default:
			break;
		}
	}

void HFPolyMesh::AddFace5ToCell(int li, int lj, int lk){
	switch(itsCellGrid[li][lj][lk]&F5_4){
		case F5_0:
			itsCellGrid[li][lj][lk]|=F5_1;
			break;
		case F5_1:
			itsCellGrid[li][lj][lk]&= ~F5_1;
			itsCellGrid[li][lj][lk]|=F5_2;
			break;
		case F5_2:
			itsCellGrid[li][lj][lk]|=F5_4;
			break;
		default:
			break;
		}
	}

void HFPolyMesh::AddFace6ToCell(int li, int lj, int lk){
	switch(itsCellGrid[li][lj][lk]&F6_4){
		case F6_0:
			itsCellGrid[li][lj][lk]|=F6_1;
			break;
		case F6_1:
			itsCellGrid[li][lj][lk]&= ~F6_1;
			itsCellGrid[li][lj][lk]|=F6_2;
			break;
		case F6_2:
			itsCellGrid[li][lj][lk]|=F6_4;
			break;
		default:
			break;
		}
	}
