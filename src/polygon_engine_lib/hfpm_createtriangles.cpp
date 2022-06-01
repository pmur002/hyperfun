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

void HFPolyMesh::CreateTriangles(){
	int CurrentFaceState;
	int* I[12];
	//vector< vector<bool> > Connect(12, vector<bool>(12, false));
	double A1,A2;
	double S[8];
	double tV;
	bool empty=false;
	int Edge[4];
	int cp;
	//*** Cycle Through all Cells (Remember to Ignore Outer Garbage Shell) ***
	for(int i=1;i<=itsGridSize[0];i++){
		for(int j=1;j<=itsGridSize[1];j++){
			for(int k=1;k<=itsGridSize[2];k++){
				FillIndex(i, j, k, I);
				//*** Check for Empty Cell ***
				if(itsCellGrid[i][j][k]==0) empty=true;
				//*** Cycle Through Faces ***
				if(!empty){
					for(int F=1;F<=6;F++){
						int E=itsCellGrid[i][j][k];
						CurrentFaceState=itsCellGrid[i][j][k]&3;
						switch (CurrentFaceState){
							case NONE:
								break;
							case TWO:
								for(cp=0; cp<4; cp++){
									Edge[cp]=Face2Vert[F-1][cp];
									}
								GraphBuildCase2(Edge, I); 
								break;
							case FOUR:
								S[0]=itsValGrid [i-1]	[j-1]	[k-1];
								S[1]=itsValGrid [i-1]	[j]		[k-1];
								S[2]=itsValGrid [i]		[j]		[k-1];
								S[3]=itsValGrid [i]		[j-1]	[k-1];
								S[4]=itsValGrid [i-1]	[j-1]	[k];
								S[5]=itsValGrid [i-1]	[j]		[k];
								S[6]=itsValGrid [i]		[j]		[k];
								S[7]=itsValGrid [i]		[j-1]	[k];
								
								switch (F){
									//	1			2			3			4			5			6
									//{ {0,1,2,3}, {0,9,4,8}, {3,11,7,8}, {4,5,6,7}, {2,10,6,11}, {1,10,5,9} }
									
									//	1			3			4			6			2			5
									//{1,2,3,4}, {4,12,8,9}, {5,6,7,8}, {2,11,6,10}, {1,10,5,9}, {3,11,7,12}
									case 1://1
										A1=S[0]-S[1];
										A2=S[0]+S[2]-S[1]-S[3]; //((S[0]+S[2])-S[1])-S[3]
										tV=(itsS0-S[1])/(S[2]-S[1]);
										for(cp=0; cp<4; cp++){
											Edge[cp]=Face2Vert[F-1][cp];
											}
										GraphBuildCase4(tV, A1, A2, Edge);
										break;
									case 3://3
										A1=S[0]-S[3];
										A2=S[0]+S[7]-S[3]-S[4]; //((S[0]+S[7])-S[3])-S[4]
										tV=(itsS0-S[3])/(S[7]-S[3]);
										for(cp=0; cp<4; cp++){
											Edge[cp]=Face2Vert[F-1][cp];
											}
										GraphBuildCase4(tV, A1, A2, Edge);				
										break;
									case 4://4
										A1=S[4]-S[5]; //(S[0]-S[1])-(((S[0]+S[5])-S[1])-S[4])
										A2=S[4]+S[6]-S[5]-S[7]; //(((((S[1]+((S[3]+S[4])+S[6]))-S[0])-S[2])-S[5])-S[7])-(((S[0]+S[2])-S[1])-S[3])
										tV=(itsS0-S[5])/(S[6]-S[5]);
										for(cp=0; cp<4; cp++){
											Edge[cp]=Face2Vert[F-1][cp];
											}
										GraphBuildCase4(tV, A1, A2, Edge);
										break;
									case 6://6
										A1=S[1]-S[2]; //-((((S[0]+S[2])-S[1])-S[3])+(S[3]-S[0]))
										A2=S[1]-S[2]+S[6]-S[5]; //(((((S[1]+((S[3]+S[4])+S[6]))-S[0])-S[2])-S[5])-S[7])+(((S[0]+S[7])-S[3])-S[4])
										tV=(itsS0-S[2])/(S[6]-S[2]);
										for(cp=0; cp<4; cp++){
											Edge[cp]=Face2Vert[F-1][cp];
											}
										GraphBuildCase4(tV, A1, A2, Edge);
										break;
									case 2://2
										A1=S[0]-S[1];
										A2=((S[0]+S[5])-S[1])-S[4];
										tV=(itsS0-S[1])/(S[5]-S[1]);
										for(cp=0; cp<4; cp++){
											Edge[cp]=Face2Vert[F-1][cp];
											}
										GraphBuildCase4(tV, A1, A2, Edge);
										break;
									case 5://5
										A1=S[3]-S[2]; //-((((S[0]+S[2])-S[1])-S[3])+(S[1]-S[0]))
										A2=S[3]-S[2]+S[6]-S[7]; //(((((S[1]+((S[3]+S[4])+S[6]))-S[0])-S[2])-S[5])-S[7])+(((S[0]+S[5])-S[1])-S[4])
										tV=(itsS0-S[2])/(S[6]-S[2]);
										for(cp=0; cp<4; cp++){
											Edge[cp]=Face2Vert[F-1][cp];
											}
										GraphBuildCase4(tV, A1, A2, Edge);
										break;
									}
								break;
							}
						//*** Shift to next Face ***
						itsCellGrid[i][j][k]>>=2;
						}
					///*** Trace Graph and Create Triangles ***
					TraceGraph(I);
					}
				empty=false;
				}
			}
		}
	}
