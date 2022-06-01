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

void HFPolyMesh::TraceGraph(int* I[12]){
	int Head, Child1, Child2;
	int tmpTri[3]={-1, -1, -1};
	
	for(Head=0; Head<12; Head++){
		
		if(itsConnect[Head][0]>=0){//Found Starting Vertex <Head>
			Child1=itsConnect[Head][0];
			Child2=itsConnect[Head][1];
			//*********************
			//*****  CHILD 1  *****
			if(itsConnect[Child1][0]==Head)//If Child1[0] is connected to Head 
				if(itsConnect[Child1][1]!=Child2) itsConnect[Child1][0]=Child2;//Replace Child1[0]-Head Link with Child1[0]-Child2 Link
				else itsConnect[Child1][0]=itsConnect[Child1][1]=-1;
			else//If Child1[1] is connected to Head 
				if(itsConnect[Child1][0]!=Child2) itsConnect[Child1][1]=Child2;//Replace Child1[1]-Head Link with Child1[1]-Child2 Link
				else itsConnect[Child1][0]=itsConnect[Child1][1]=-1;				
			
			//*********************
			//*****  CHILD 2  *****
			if(itsConnect[Child2][0]==Head)//If Child2[0] is connected to Head 
				if(itsConnect[Child2][1]!=Child1) itsConnect[Child2][0]=Child1;//Replace Child1[0]-Head Link with Child1[0]-Child2 Link
				else itsConnect[Child2][0]=itsConnect[Child2][1]=-1;	
			else//If Child2[1] is connected to Head 
				if(itsConnect[Child2][0]!=Child1) itsConnect[Child2][1]=Child1;//Replace Child2[1]-Head Link with Child2[1]-Child2 Link
				else itsConnect[Child2][0]=itsConnect[Child2][1]=-1;	
			
			//******************
			//*****  HEAD  *****
			itsConnect[Head][0]=itsConnect[Head][1]=-1;//Delete Head-Child1 and Head-Child2 Links
			
			//**********************
			//*****  TRIANGLE  *****
			tmpTri[0]=*I[Head];
			tmpTri[1]=*I[Child1];
			tmpTri[2]=*I[Child2];
			itsData.pushBackTriangle(tmpTri);
			}
		}
	}

/*

#include "hfpolymesh.h"

static const double ALPHA = 30.0;
static const double COSALPHA=cos((ALPHA*3.14159265359)/180);

void HFPolyMesh::TraceGraph(int* I[12]){
	int Head, Child1, Child2;
	vector<int> tmpTri(3,-1);

	//**********************
	//*****  TESTING  ******
	int tmpH, tmpC;
	for(tmpH=0; tmpH<12; tmpH++){
		if(itsConnect[tmpH][0]>=0){
			tmpC=itsConnect[tmpH][0];
			if((itsConnect[tmpC][0]!=tmpH) && (itsConnect[tmpC][1]!=tmpH)){
				cout << "ItsConnect Problem!!!\n";//TMP:Error Checking
				}
			}
		}
	//**********************
	

	for(Head=0; Head<12; Head++){
		if(itsConnect[Head][0]>=0){//Found Starting Vertex <Head>
			if((itsConnect[Head][1]<0)) cout << "HERE's the PROBLEM!!!! \n";//TMP: Error Checking 
			Child1=itsConnect[Head][0];
			Child2=itsConnect[Head][1];
			//*********************
			//*****  CHILD 1  *****
			if(itsConnect[Child1][0]==Head)//If Child1[0] is connected to Head 
				itsConnect[Child1][0]=Child2;//Replace Child1[0]-Head Link with Child1[0]-Child2 Link
			else//If Child1[1] is connected to Head 
				itsConnect[Child1][1]=Child2;//Replace Child1[1]-Head Link with Child1[1]-Child2 Link
			//*********************
			//*****  CHILD 2  *****
			if(itsConnect[Child2][0]==Head) //If Child2[0] is connected to Head 
				itsConnect[Child2][0]=Child1;//Replace Child1[0]-Head Link with Child1[0]-Child2 Link
			else//If Child2[1] is connected to Head 
				itsConnect[Child2][1]=Child1;//Replace Child2[1]-Head Link with Child2[1]-Child2 Link
			//******************
			//*****  HEAD  *****
			itsConnect[Head][0]=itsConnect[Head][1]=-1;//Delete Head-Child1 and Head-Child2 Links
			//**********************
			//*****  TRIANGLE  *****
			if((*I[Head]!=*I[Child1]) && (*I[Head]!=*I[Child2]) && (*I[Child1]!=*I[Child2])){//An Actual Triangle				
				double LC1H, LC2H, LCC; //Length C1-H C2-H C1-C2
				int links=0;
				//Calculate Distances  (Compare to itsMinEdgeLen)
				LC1H=VertDist(*I[Child1], *I[Head]);
				LC2H=VertDist(*I[Child2], *I[Head]);
				LCC=VertDist(*I[Child1], *I[Child2]);
				
				//
				//const double LEPS=0.01;
				short SC1H=0;
				short SC2H=0;
				short SCC=0;
				bool Zero=false;
				if(LC1H>LC2H) SC1H++;
				else SC2H++;
				if(LC1H>LCC) SC1H++;
				else SCC++;
				if(LC2H>LCC) SC2H++;
				else SCC++;

				if(LC1H<itsMinEdgeLen) links+=1;
				if(LC2H<itsMinEdgeLen) links+=2;
				if(LCC<itsMinEdgeLen)  links+=4;

				
				if(SC1H==2){
					if((LC1H/(LC2H+LCC))>COSALPHA){
						Zero=true;
						if(SC2H==1) SealTriangle(I[Child2],I[Child1]);
						else SealTriangle(I[Child2],I[Head]);
						}
					}
				if(SC2H==2){
					if((LC2H/(LC1H+LCC))>COSALPHA){
						Zero=true;
						if(SC1H==1) SealTriangle(I[Child1],I[Child2]);
						else SealTriangle(I[Child1],I[Head]);
						}
					}
				if(SCC==2){
					if((LCC/(LC1H+LC2H))>COSALPHA){
						Zero=true;
						if(SC1H==1) SealTriangle(I[Head],I[Child2]);
						else SealTriangle(I[Head],I[Child1]);
						}
					}


				if(links==0){//A Triangle with Long enough Edges and Doesn't Have Zero Area
					if(!Zero){
						tmpTri[0]=*I[Head];
						tmpTri[1]=*I[Child1];
						tmpTri[2]=*I[Child2];
						itsData.itsTempTriangles.push_back(tmpTri);
						itsData.itsBadTempTriangles.push_back(false);
						itsCurrentTriangle++;
						}
					}
				else{//A Degenerate Triangle Due to at least one short Edge
					if(!Zero){
						switch(links){
							case 1://C1-H Link Only
								if(LC2H>LCC) SealTriangle(I[Child1], I[Head]);
								else SealTriangle(I[Head], I[Child1]);
								//cout << "Case 1\n";
								break;
							case 2://C2-H Link Only
								if(LC1H>LCC) SealTriangle(I[Child2], I[Head]);
								else SealTriangle(I[Head], I[Child2]);
								//cout << "Case 2\n";
								break;
							case 3://C1-H and C2-H Links
								if(LC1H<LC2H) SealTriangle(I[Head], I[Child1]);
								else SealTriangle(I[Head], I[Child2]);
								//cout << "Case 3\n";
								break;
							case 4://C1-C2 Link Only
								if(LC1H>LC2H) SealTriangle(I[Child2], I[Child1]);
								else SealTriangle(I[Child1], I[Child2]);
								//cout << "Case 4\n";
								break;
							case 5://C1-H and C1-C2 Links
								if(LC1H<LCC) SealTriangle(I[Child1], I[Head]);
								else SealTriangle(I[Child1], I[Child2]);
								//cout << "Case 5\n";
								break;
							case 6://C2-H and C1-C2 Links
								if(LC2H<LCC) SealTriangle(I[Child2], I[Head]);
								else SealTriangle(I[Child2], I[Child1]);
								//cout << "Case 6\n";
								break;
							case 7://C1-H, C2-H, and C1-C2 Links  (Note: Use Shortest Links)
								if(SC1H==0){
									if(SC2H==1){
										SealTriangle(I[Child1], I[Head]);
										SealTriangle(I[Child2], I[Head]);
										}
									else{//SCC==1
										SealTriangle(I[Head], I[Child1]);
										SealTriangle(I[Child2], I[Child1]);
										}
									}
								else{
									if(SC2H==0){
										if(SC1H==1){
											SealTriangle(I[Child1], I[Head]);
											SealTriangle(I[Child2], I[Head]);
											}
										else{//SCC==1
											SealTriangle(I[Head], I[Child2]);
											SealTriangle(I[Child2], I[Child2]);
											}
										}
									else{
										if(SCC==0){
											if(SC1H==1){
												SealTriangle(I[Head], I[Child1]);
												SealTriangle(I[Child2], I[Child1]);
												}
											else{//SC2H==1
												SealTriangle(I[Head], I[Child2]);
												SealTriangle(I[Child1], I[Child2]);
												}
											}
										}
									}
								//cout << "Case 7\n";
								break;
							default:
								cout << "BAD!!!";
								break;
							}
						}
					//if(!BC1H){ SealTriangle(I[Child1], I[Head]);   cout << *I[Child1] << " \n";}
					//if(!BC2H){ SealTriangle(I[Child2], I[Head]);   cout << *I[Child2] << " \n";}
					//if(!BCC) { SealTriangle(I[Child1], I[Child2]); cout << *I[Child1] << " \n";}
					itsSkipCurrentTriangle++;
					}

				}
			else{//Not a Triangle (Two or More Vertices the same)
				itsSkipCurrentTriangle++;
				}
			//**********************
			//*****  TESTING  ******
			for(tmpH=0; tmpH<12; tmpH++){
				if(itsConnect[tmpH][0]>=0){
					tmpC=itsConnect[tmpH][0];
					if((itsConnect[tmpC][0]!=tmpH) && (itsConnect[tmpC][1]!=tmpH)){
						cout << "ItsConnect Problem!!!\n";//TMP:Error Checking
						}
					}
				}
			//**********************
			}
		}
	}

double HFPolyMesh::VertDist(int V1, int V2){
	double L1=itsData.itsVertices[V1][0]-itsData.itsVertices[V2][0];//X Coordinate Length
	double L2=itsData.itsVertices[V1][1]-itsData.itsVertices[V2][1];//Y Coordinate Length
	double L3=itsData.itsVertices[V1][2]-itsData.itsVertices[V2][2];//Z Coordinate Length
	return sqrt((L1*L1)+(L2*L2)+(L3*L3));//Distance
	}

void HFPolyMesh::SealTriangle(int* V1, int* V2){
	for(int i=0; i<itsData.itsTempTriangles.size(); i++){//Cycle Through Whole Temp Triangle Buffer
		if(!itsData.itsBadTempTriangles[i]){
			vector<int> T=itsData.itsTempTriangles[i];//Test Triangle
			if(T[0]==*V1){
				if((T[1]==*V2) || (T[2]==*V2)){ itsData.itsBadTempTriangles[i]=true; itsSkipCurrentTriangle++; itsCurrentTriangle--; }//cout << "H1\n";}
				else{ itsData.itsTempTriangles[i][0]=*V2; }//cout << "C1\n";}
				}
			else{
				if(T[1]==*V1){
					if((T[0]==*V2) || (T[2]==*V2)){ itsData.itsBadTempTriangles[i]=true; itsSkipCurrentTriangle++; itsCurrentTriangle--; }//cout << "H2\n";}
					else{ itsData.itsTempTriangles[i][1]=*V2; }//cout << "C2\n";}
					}
				else{
					if(T[2]==*V1){
						if((T[0]==*V2) || (T[1]==*V2)){ itsData.itsBadTempTriangles[i]=true; itsSkipCurrentTriangle++; itsCurrentTriangle--; }//cout << "H3\n";}
						else{ itsData.itsTempTriangles[i][2]=*V2; }//cout << "C3\n";}			
						}
					}
				}
			}
		}
	//Make Vertex1=Vertex2 for triangles calculated afterward.
	*V1=*V2;
	}
*/
