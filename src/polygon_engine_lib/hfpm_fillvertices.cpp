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

void HFPolyMesh::FillVertices(){

	int i,j,k;
	double pos[3]={0.0, 0.0, 0.0};

	cout << "Search: ";
	if(itsSearch) cout << "TRUE\n";
	else cout << "FALSE\n";

	//*** Cycle Through Cells Calculating 3 Edge Vertices ***
	for(i=0, pos[0]=itsBBMin[0]; i<itsGridSize[0]; i++, pos[0]+=itsDelta[0]){					
		for(j=0, pos[1]=itsBBMin[1]; j<itsGridSize[1]; j++, pos[1]+=itsDelta[1]){
			for(k=0, pos[2]=itsBBMin[2]; k<itsGridSize[2]; k++, pos[2]+=itsDelta[2]){
				if(itsBoolGrid[i][j][k]){
					if(!itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
					if(!itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
					if(!itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
					}
				else{
					if(itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
					if(itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
					if(itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
					}
				}
			}
		}
//*** At this point the Bounding Box Front Face, Right Face, and Top Face Vertices still need to be calculated ***
	//*** Calculate Right Face Vertices ***
	i=itsGridSize[0];
	pos[0]=itsBBMax[0];	
	for(j=0, pos[1]=itsBBMin[1]; j<itsGridSize[1]; j++, pos[1]+=itsDelta[1]){
		for(k=0, pos[2]=itsBBMin[2]; k<itsGridSize[2]; k++, pos[2]+=itsDelta[2]){
			if(itsBoolGrid[i][j][k]){
				if(!itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
				if(!itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
				}
			else{
				if(itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
				if(itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
				}
			}
		}
	//*** Calculate Top Face Vertices ***
	j=itsGridSize[1]; 
	pos[1]=itsBBMax[1];	
	for(i=0, pos[0]=itsBBMin[0]; i<itsGridSize[0]; i++, pos[0]+=itsDelta[0]){
		for(k=0, pos[2]=itsBBMin[2]; k<itsGridSize[2]; k++, pos[2]+=itsDelta[2]){
			if(itsBoolGrid[i][j][k]){
				if(!itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
				if(!itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
				}
			else{
				if(itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
				if(itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
				}
			}
		}
	//*** Calculate Front Face Vertices ***
	k=itsGridSize[2]; 
	pos[2]=itsBBMax[2];	
	for(i=0, pos[0]=itsBBMin[0]; i<itsGridSize[0]; i++, pos[0]+=itsDelta[0]){
		for(j=0, pos[1]=itsBBMin[1]; j<itsGridSize[1]; j++, pos[1]+=itsDelta[1]){
			if(itsBoolGrid[i][j][k]){
				if(!itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
				if(!itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
				}
			else{
				if(itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
				if(itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
				}
			}
		}
//*** At this point Vertices Lying Along the Bounding Box Axes need to be calculated ***
	//*** Calculate X-axis Vertices ***
	j=itsGridSize[1];
	k=itsGridSize[2]; 
	pos[1]=itsBBMax[1];
	pos[2]=itsBBMax[2];	
	for(i=0, pos[0]=itsBBMin[0]; i<itsGridSize[0]; i++, pos[0]+=itsDelta[0]){
		if(itsBoolGrid[i][j][k]){
			if(!itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
			}
		else{
			if(itsBoolGrid[i+1][j][k]) VertCalc(i,j,k,0,pos);
			}
		}
	//*** Calculate Y-axis Vertices ***
	i=itsGridSize[0];
	k=itsGridSize[2]; 
	pos[0]=itsBBMax[0];
	pos[2]=itsBBMax[2];	
	for(j=0, pos[1]=itsBBMin[1]; j<itsGridSize[1]; j++, pos[1]+=itsDelta[1]){
		if(itsBoolGrid[i][j][k]){
			if(!itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
			}
		else{
			if(itsBoolGrid[i][j+1][k]) VertCalc(i,j,k,1,pos);
			}
		}
	//*** Calculate Z-axis Vertices ***
	i=itsGridSize[0];
	j=itsGridSize[1];
	pos[0]=itsBBMax[0];
	pos[1]=itsBBMax[1];	
	for(k=0, pos[2]=itsBBMin[2]; k<itsGridSize[2]; k++, pos[2]+=itsDelta[2]){
		if(itsBoolGrid[i][j][k]){
			if(!itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
			}
		else{
			if(itsBoolGrid[i][j][k+1]) VertCalc(i,j,k,2,pos);
			}
		}
	}

void HFPolyMesh::VertCalc(int i, int j, int k, int select, const double lNodePos[3]){
	int i0,j0,k0,i1,j1,k1,i2,j2,k2;

	switch(select){
		case 0:	
			if(i-1<0) i0=i; else i0=i-1; 
			i1=i+1; 
			if(i+2>itsGridSize[0]) i2=i1; else i2=i+2;	
			j0=j1=j2=j;
			k0=k1=k2=k;
			break;
		case 1: 
			i0=i1=i2=i;
			if(j-1<0) j0=j;	else j0=j-1;
			j1=j+1; 
			if(j+2>itsGridSize[1]) j2=j1; else j2=j+2;	
			k0=k1=k2=k;
			break;
		case 2: 
			i0=i1=i2=i;
			j0=j1=j2=j;	
			if(k-1<0) k0=k;	else k0=k-1; 
			k1=k+1; 
			if(k+2>itsGridSize[2]) k2=k1; else k2=k+2;	
			break;
		default: 
			break;
		}

	double vDist=VertEst(itsValGrid[i][j][k], itsValGrid[i1][j1][k1], itsDelta[select]);
	double vPos[3]={lNodePos[0], lNodePos[1], lNodePos[2]};
	vPos[select]+=vDist;

	/**ATTRIBUTES COMMENTS : the call to the following function 
	also set the itsSAttributes array. "S" is a public member of the present class*/

	double vVal=CalcVal(vPos);

	if(itsSearch){//If Search Should be Performed
		double tempPos[3]={lNodePos[0], lNodePos[1], lNodePos[2]};
		vVal=EdgeSearch(i, j, k, select, tempPos, vPos);//Search
		}
	/**ATTRIBUTES COMMENTS : also pushBack The "S" array*/
	itsVertGrid[select][i][j][k]=itsData.pushBackVertex(vPos, vVal,itsSAttributesM);
	StoreCell(i,j,k,select);//*** Store Cell Face Information to 4 appropriate Cells ***
	}

double HFPolyMesh::VertEst(const double& leftVal, const double& rightVal, const double& delta){
	return (((itsS0-leftVal)/(rightVal-leftVal))*delta);
	}

double HFPolyMesh::EdgeSearch(int leftI, int leftJ, int leftK, const int& select, const double lp[3], double* centerPos){
	//Search Increment Length
	double SearchIncLen=itsDelta[select]*itsSearchPer;
	//******************
	//*****  LEFT  *****
	double leftPos[3]={lp[0], lp[1], lp[2]};
	//leftPos already done
	double leftVal=itsValGrid[leftI][leftJ][leftK];//leftVal
	//******************
	//*****  RIGHT  *****
	double rightPos[3]={leftPos[0], leftPos[1], leftPos[2]};
	rightPos[select]+=itsDelta[select];//rightPos
	double rightVal;
	switch(select){
		case 0:	rightVal=itsValGrid[leftI+1]	[leftJ]		[leftK];	break;
		case 1:	rightVal=itsValGrid[leftI]		[leftJ+1]	[leftK];	break;
		case 2:	rightVal=itsValGrid[leftI]		[leftJ]		[leftK+1];	break;
		default: break;
		}//rightVal
	//*********************
	//*****  DISTANCE *****
	double lrDist=itsDelta[select];
	double lcDist=VertEst(leftVal, rightVal, lrDist);
	//*******************
	//***** CENTER  *****
	centerPos[0]=leftPos[0];
	centerPos[1]=leftPos[1];
	centerPos[2]=leftPos[2];
	centerPos[select]+=lcDist;//centerPos
	double centerVal=CalcVal(centerPos);//centerVal
	//*******************
	//*****  CHECK  *****
	if(lcDist<SearchIncLen) return centerVal;//PosCheck on first step
	//******************
	//*****  LOOP  *****
	do{
		double deltaDist;
		bool b1=(centerVal-itsS0)>0;//centerVal Positive
		bool b2=(leftVal-itsS0)>0;//leftVal Positive
		if(b1 ^ b2){//If Diferent signs Move right;
			rightPos[0]=centerPos[0];
			rightPos[1]=centerPos[1];
			rightPos[2]=centerPos[2];
			rightVal=centerVal;
			deltaDist = lrDist;
			lrDist = lcDist;
			deltaDist -= lrDist;
			}
		else{
			leftPos[0]=centerPos[0];
			leftPos[1]=centerPos[1];
			leftPos[2]=centerPos[2];
			leftVal=centerVal;
			deltaDist = lrDist;
			lrDist -= lcDist;
			deltaDist -= lrDist;
			}
		//**********************
		//*****  DISTANCE  *****
		lcDist=VertEst(leftVal, rightVal, lrDist);
		//********************
		//*****  CENTER  *****
		centerPos[select]=leftPos[select]+lcDist;
		centerVal=CalcVal(centerPos);
		//*******************
		//*****  CHECK  *****
		if(deltaDist<SearchIncLen) return centerVal;//PosCheck on first step
		} while(true);
	}
