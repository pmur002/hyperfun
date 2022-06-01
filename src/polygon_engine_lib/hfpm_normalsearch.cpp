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


/*double HFPolyMesh::NormalLISearch(const vector<double>& normDir, 
								  vector<double>& leftPos, double leftVal, 
								  vector<double> rightPos, double rightVal, double dist){
	double moveRight=VertEst(leftVal, rightVal, dist);
	vector<double> centerPos(3,0.0);
	centerPos[0]=leftPos[0]+(moveRight*normDir[0]);
	centerPos[1]=leftPos[1]+(moveRight*normDir[1]);
	centerPos[2]=leftPos[2]+(moveRight*normDir[2]);
	double centerVal=CalcVal(centerPos);
	double centerCheck=centerVal-itsS0;
	bool b1, b2;
	int il=0;

	while(fabs(centerCheck)>itsSearchDataVal){
		il++;
		b1=(centerCheck>0);
		b2=((leftVal-itsS0)>0);
		if(il>12){ cout << "Loop > 12 \n"; leftPos=centerPos; return centerVal; }
		if( (b1 && !b2) || (!b1 && b2) ){
			rightPos=centerPos;
			rightVal=centerVal;
			dist=moveRight;
			}
		else{
			leftPos=centerPos;
			leftVal=centerVal;
			dist-=moveRight;
			}
		moveRight=VertEst(leftVal, rightVal, dist);
		centerPos[0]=leftPos[0]+(dist*normDir[0]);
		centerPos[1]=leftPos[1]+(dist*normDir[1]);
		centerPos[2]=leftPos[2]+(dist*normDir[2]);
		centerVal=CalcVal(centerPos);
		centerCheck=centerVal-itsS0;
		}
	leftPos=centerPos;
	return centerVal;
	}*/

/*
void HFPolyMesh::NormalVertSearch(){
	cout << "MSD (Diagnal):" << MSD << "\n";
	int VBCount=0;
	int c1,c2,c3,c4,c5,c6,c7,c8;
	c1=c2=c3=c4=c5=c6=c7=c8=0;
	vector<double> leftPos(3,0.0);
	double leftVal;
	double leftCheck;
	vector<double> rightPos(3,0.0);
	double rightVal;
	double rightCheck;
	vector<double> normal(3,0.0);
	double normLength;
	double QLipLength;
	for(int a=0; a<itsSVA.size(); a++){
		bool check=true;
		leftPos=itsData.itsVertices[itsSVA[a]];//Get Start Position
		leftVal=itsData.itsFunctVal[itsSVA[a]];//Get Start Value
		normal=NormalCalc(leftPos, leftVal, normLength);//Calculate normal and get normLength
		//cout << "NormLength: " << normLength;
		QLipLength=leftVal/normLength;//Calculate Quasi-Lipschitz Bound
		//cout << "         QLipLength: " << QLipLength << "\n";

		if( (QLipLength<MSD) ){//Good Quasi-Lipschitz Bound
			//cout << "HERE!\n";
			rightPos[0]=leftPos[0]+(QLipLength*normal[0]);
			rightPos[1]=leftPos[1]+(QLipLength*normal[1]);
			rightPos[2]=leftPos[2]+(QLipLength*normal[2]);
			rightVal=CalcVal(rightPos);//Calc Right Value
			rightCheck=rightVal-itsS0;
			if(fabs(rightCheck)<itsSearchDataVal){//Right is new vertex
				itsData.itsVertices[itsSVA[a]]=rightPos;
				itsData.itsFunctVal[itsSVA[a]]=rightVal;
				//cout << "LV1: " << leftVal << "       RV1: " << rightVal<< "\n";
				c1++;
				check=false;
				}
			else{//Right is not new vertex
				leftCheck=leftVal-itsS0;
				if( (leftCheck>0 && rightCheck<0) || (leftCheck<0 && rightCheck>0) ){//Different Signs
					itsData.itsFunctVal[itsSVA[a]]=NormalLISearch(normal, leftPos, leftVal, rightPos, rightVal, QLipLength);
					itsData.itsVertices[itsSVA[a]]=leftPos;
					c2++;
					check=false;
					}
				else{//QLip didn't work, try QLip*2
					rightPos[0]=rightPos[0]+(QLipLength*normal[0]);//Double QLipLength
					rightPos[1]=rightPos[1]+(QLipLength*normal[1]);
					rightPos[2]=rightPos[2]+(QLipLength*normal[2]);
					rightVal=CalcVal(rightPos);//Calc Right Value
					rightCheck=rightVal-itsS0;
					if(fabs(rightCheck)<itsSearchDataVal){//Right is new vertex
						itsData.itsVertices[itsSVA[a]]=rightPos;
						itsData.itsFunctVal[itsSVA[a]]=rightVal;
						c3++;
						check=false;
						}
					else{//Right is not new vertex
						leftCheck=leftVal-itsS0;
						if( (leftCheck>0 && rightCheck<0) || (leftCheck<0 && rightCheck>0) ){//Different Signs
							itsData.itsFunctVal[itsSVA[a]]=NormalLISearch(normal, leftPos, leftVal, rightPos, rightVal, QLipLength);
							itsData.itsVertices[itsSVA[a]]=leftPos;
							c4++;
							check=false;
							}
						else{//QLip*2 go try Diag
							;//Do nothing
							}
						}
					}
				}
			}//End: Good Quasi-Lipschitz Bound
		if(check){//If nothing found so far
			double Na=normal[0]/MSL[0]; Na*=Na;
			double Nb=normal[1]/MSL[1]; Nb*=Nb;
			double Nc=normal[2]/MSL[2]; Nc*=Nc;
			double len=sqrt(1/(Na+Nb+Nc));
			if((leftVal-itsS0)<0) len = -len;//Reverse direction if needed
			len*=1;//1+itsSearchData;//Use NormDiag*2
			rightPos[0]=leftPos[0]+(len*normal[0]);
			rightPos[1]=leftPos[1]+(len*normal[1]);
			rightPos[2]=leftPos[2]+(len*normal[2]);
			rightVal=CalcVal(rightPos);//Calc Right Value
			rightCheck=rightVal-itsS0;
			if(fabs(rightCheck)<itsSearchDataVal){//Right is new vertex
				itsData.itsVertices[itsSVA[a]]=rightPos;
				itsData.itsFunctVal[itsSVA[a]]=rightVal;
				cout << "LV5: " << leftVal << "\n";
				c5++;
				}
			else{//Right is not new vertex
				leftCheck=leftVal-itsS0;
				if( (leftCheck>0 && rightCheck<0) || (leftCheck<0 && rightCheck>0) ){//Different Signs
					itsData.itsFunctVal[itsSVA[a]]=NormalLISearch(normal, leftPos, leftVal, rightPos, rightVal, QLipLength);
					itsData.itsVertices[itsSVA[a]]=leftPos;
					c6++;
					}
				else{//Diag*2 didn't work, try Diag/2
					len *= 3;//Use Diag/2
					rightPos[0]=rightPos[0]+(len*normal[0]);
					rightPos[1]=rightPos[1]+(len*normal[1]);
					rightPos[2]=rightPos[2]+(len*normal[2]);
					rightVal=CalcVal(rightPos);//Calc Right Value
					rightCheck=rightVal-itsS0;
					if(fabs(rightCheck)<itsSearchDataVal){//Right is new vertex
						itsData.itsVertices[itsSVA[a]]=rightPos;
						itsData.itsFunctVal[itsSVA[a]]=rightVal;
						c7++;
						}
					else{//Right is not new vertex
						leftCheck=leftVal-itsS0;
						if( (leftCheck>0 && rightCheck<0) || (leftCheck<0 && rightCheck>0) ){//Different Signs
							itsData.itsFunctVal[itsSVA[a]]=NormalLISearch(normal, leftPos, leftVal, rightPos, rightVal, QLipLength);
							itsData.itsVertices[itsSVA[a]]=leftPos;
							c8++;
							}
						else{//QLip*2 go try Diag
							//cout << "LVB: " << leftVal << "\n";
							VBCount++;//Do nothing
							}
						}
					}
				}
			}//End: nothing found so far
		}//End: For loop
	cout << "QLip Find:          " << c1 << "\n";
	cout << "QLip Search Find:   " << c2 << "\n";
	cout << "2*QLip Find:        " << c3 << "\n";
	cout << "2*QLip Search Find: " << c4 << "\n";
	cout << "Diag Find:          " << c5 << "\n";
	cout << "Diag Search Find:   " << c6 << "\n";
	cout << "Diag/6 Find:        " << c7 << "\n";
	cout << "Diag/6 Search Find: " << c8 << "\n";
	cout << "Nothing Found:      " << VBCount << "\n";
	cout << "Sum of all:         " << c1+c2+c3+c4+c5+c6+c7+c8+VBCount << "\n";
	cout << "Size:               " << itsSVA.size() << "\n\n";
	}
	
*/

/*
double HFPolyMesh::NormalLISearch(const vector<double>& normDir, 
								  vector<double>& leftPos, double leftVal, 
								  vector<double> rightPos, double rightVal, double dist){
	return leftVal;
	}*/




	
	

/*void HFPolyMesh::NormalVertSearch(){//lower case: function value -- Upper Case: Position
	int N=itsCurrentIndex-1;
	vector<double> searchDir;
	vector<double> Left(3, 0.0);
	vector<double> Right(3, 0.0);
	vector<double> Center(3, 0.0);
	for(int i=0; i<N; i++){
		//cout << N-i << "\n";//TMP
		Left=itsData.itsVertices[i];
		double left, right;
		searchDir=NormalCalc(Left, left);
		if(left<0){
			// Search along Gradient instead of normal
			searchDir[0]=-searchDir[0];
			searchDir[1]=-searchDir[1];
			searchDir[2]=-searchDir[2];
			}
		//Find right position
		double searchDist=sqrt(((MSL[0]*searchDir[0])*(MSL[0]*searchDir[0]))+((MSL[1]*searchDir[1])*(MSL[1]*searchDir[1]))+((MSL[2]*searchDir[2])*(MSL[2]*searchDir[2])));
		Right[0]=Left[0]+(searchDist*searchDir[0]);
		Right[1]=Left[1]+(searchDist*searchDir[1]);
		Right[2]=Left[2]+(searchDist*searchDir[2]);
		right=CalcVal(Right);

		if((right>0 && left>0) || (right<0 && left<0)){
			cout << "Bad News!!!   " << i << "\n";
			}
		else{
			double target=itsSearchPer;
			double center=itsS0+itsSearchPer+1;
			if(left>itsS0){
				//cout << "Entered #1\n";//TMP
				for(int c1=0;;c1++){
					//cout << c1 << " ";
					double offsetDist=VertEst(left, right, searchDist);
					Center[0]=Left[0]+(offsetDist*searchDir[0]);
					Center[1]=Left[1]+(offsetDist*searchDir[1]);
					Center[2]=Left[2]+(offsetDist*searchDir[2]);
					if(fabs(center-itsS0)<target) break; //Target Reached
					center=CalcVal(Center);
					if(center<=itsS0){ 
						right=center; Right=Center; searchDist=offsetDist;}
					else{ 
						left=center; Left=Center; searchDist-=offsetDist;}
					}
				}
			else{
				//cout << "Entered #2\n";//TMP
				for(int c2=0;;c2++){
					//cout << c2 << " ";
					double offsetDist=VertEst(left, right, searchDist);
					Center[0]=Left[0]+(offsetDist*searchDir[0]);
					Center[1]=Left[1]+(offsetDist*searchDir[1]);
					Center[2]=Left[2]+(offsetDist*searchDir[2]);
					if(fabs(center-itsS0)<target) break; //Target Reached
					center=CalcVal(Center);
					if(center<=itsS0){ 
						left=center; Left=Center; searchDist-=offsetDist;}
					else{ 
						right=center; Right=Center; searchDist=offsetDist;}
					}
				}
			itsData.itsVertices[i]=Center;
			}
		}
	}

double HFPolyMesh::CalcVal(const vector<double>& pos){
	itsSendingX[itsXMap]=pos[0];
	itsSendingX[itsYMap]=pos[1];
	itsSendingX[itsZMap]=pos[2];
	return itsInterpreter.calc(itsSendingX);
	}*/


/*
  double HFPolyMesh::VertSearchCalc(const double& left, const double& right, const double& delta, const double& offset, const int& mapping){
	double L=0.0;
	double R=1.0;
	double l=left;
	double r=right;
	double PREC=0.0;
	double C=0.0;  //?? What is correct starting Value ??
	double TargetVal=(itsSearchPer*fabs(l-r));
	double c=TargetVal+1.0;

	if(l>itsS0){
		for(;;){
			PREC=C;
			C=L+(((itsS0-l)/(r-l))*(R-L));
			if((fabs(C-PREC)<=itsSearchPer) || (fabs(c-itsS0)<=TargetVal)) break;
			itsSendingX[mapping]=offset+(C*delta);
			c=itsInterpreter.calc(itsSendingX);
			if(c<=itsS0){ r=c; R=C;	}
			else{ l=c; L=C; }
			}
		}
	else{
		for(;;){
			PREC=C;
			C=L+(((itsS0-l)/(r-l))*(R-L));
			if((fabs(C-PREC)<=itsSearchPer) || (fabs(c-itsS0)<=TargetVal)) break;
			itsSendingX[mapping]=offset+(C*delta);
			c=itsInterpreter.calc(itsSendingX);
			if(c<=itsS0){ l=c; L=C; }
			else{ r=c; R=C; }
			}
		}
	return (C*delta);
	}*/

/*vector<double> HFPolyMesh::NormalCalc(const vector<double>& pos, double& val){
	vector<double> ret(3, 0.0);
	//Start Point
	val=CalcVal(pos);
	//XPoint
	itsSendingX[itsXMap]+=itsNormDelt[0];
	ret[0]=-((itsInterpreter.calc(itsSendingX)-val)/itsNormDelt[0]);
	//YPoint
	itsSendingX[itsYMap]+=itsNormDelt[1];
	itsSendingX[itsXMap]=pos[0];
	ret[1]=-((itsInterpreter.calc(itsSendingX)-val)/itsNormDelt[1]);
	//ZPoint
	itsSendingX[itsZMap]+=itsNormDelt[2];
	itsSendingX[itsYMap]=pos[1];
	ret[2]=-((itsInterpreter.calc(itsSendingX)-val)/itsNormDelt[2]);

	double length=sqrt(ret[0]*ret[0] + ret[1]*ret[1] + ret[2]*ret[2]);
	ret[0]/=length;
	ret[1]/=length;
	ret[2]/=length;
	return ret;
	}*/

/*
void HFPolyMesh::NormalVertSearch(){//lower case: function value -- Upper Case: Position
	int C0=0;
	int C1=0;
	int C2[6]={0,0,0,0,0,0};
	int C3[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	int FCT=0;
	int EBAD=0;

	int conv, div;
	conv=div=0;
	int N=itsCurrentIndex-1;
	vector<double> searchDir;
	vector<double> Left(3, 0.0);
	vector<double> Right(3, 0.0);
	vector<double> Center(3, 0.0);
	double target=itsSearchData;
	for(int i=0; i<N; i++){
		cout << "V:" << i;
		int FC=0;
		Left=itsData.itsVertices[i];
		double left=CalcVal(Left);
		FC++;
		if(fabs(left-itsS0)>target){//If left Value is not within target range
			double right, length;
			searchDir=NormalCalc(Left, left, length);
			FC+=3;
			for(int LZ=0;; LZ++, left=CalcVal(Left), FC++){	
				cout << " l:" << left/length;
				if(LZ>=5){
					cout << "\nBAD -- len: " << left/length;
					C2[5]++;
					break;
					}
				if(fabs(left-itsS0)>target){//If left Value is not within target range
					//Find right position
					double searchDist=left/length;
					if(fabs(searchDist)>5){
						EBAD++;
						searchDist=sqrt(((MSL[0]*searchDir[0])*(MSL[0]*searchDir[0]))+((MSL[1]*searchDir[1])*(MSL[1]*searchDir[1]))+((MSL[2]*searchDir[2])*(MSL[2]*searchDir[2])));
						}

					Right[0]=Left[0]+(searchDist*searchDir[0]);
					Right[1]=Left[1]+(searchDist*searchDir[1]);
					Right[2]=Left[2]+(searchDist*searchDir[2]);
					right=CalcVal(Right);
					FC++;
					if(fabs(right-itsS0)>target){//If right value is not within target range
						if((right>itsS0 && left>itsS0) || (right<itsS0 && left<itsS0)){//If right and left do not stradle the Iso Value
							Left=Right;
							}
						else{//If right and left do stradle the Iso Value
							double center=itsS0+itsSearchData+1;
							int RC=0;
							if(left>itsS0){
								for(;RC<12;RC++){
									double offsetDist=VertEst(left, right, searchDist);
									Center[0]=Left[0]+(offsetDist*searchDir[0]);
									Center[1]=Left[1]+(offsetDist*searchDir[1]);
									Center[2]=Left[2]+(offsetDist*searchDir[2]);
									if(fabs(center-itsS0)<target) break; //Target Reached
									center=CalcVal(Center);
									FC++;
									if(center<=itsS0){ 
										right=center; Right=Center; searchDist=offsetDist;}
									else{ 
										left=center; Left=Center; searchDist-=offsetDist;}
									}
								}
							else{
								for(;RC<12;RC++){
									double offsetDist=VertEst(left, right, searchDist);
									Center[0]=Left[0]+(offsetDist*searchDir[0]);
									Center[1]=Left[1]+(offsetDist*searchDir[1]);
									Center[2]=Left[2]+(offsetDist*searchDir[2]);
									if(fabs(center-itsS0)<target) break; //Target Reached
									center=CalcVal(Center);
									FC++;
									if(center<=itsS0){ 
										left=center; Left=Center; searchDist-=offsetDist;}
									else{ 
										right=center; Right=Center; searchDist=offsetDist;}
									}
								}
							itsData.itsVertices[i]=Center;
							C3[RC]++;
							FCT+=FC;
							//cout << "Case 3:   ---   L Search= " << LZ << "  Recursion= " << RC << "  Funct Calls= " << FC << "\n";
							break;
							}
						}
					else{//If right value is within target range
						itsData.itsVertices[i]=Right;
						C2[LZ]++;
						FCT+=FC;
						//cout << "Case 2:   ---   L Search= " << LZ << "                Funct Calls= " << FC << "\n";
						break;
						}
					}
				else{//If left Value is within target range
					itsData.itsVertices[i]=Left;
					C1++;
					FCT+=FC;
					//cout << "Case 1:   ---   L Search= " << LZ << "                Funct Calls= " << FC << "\n";
					break;
					}
				}
			}
		else{
			C0++;
			FCT+=FC;
			//cout << "Case 0:   ---                              Funct Calls= " << FC << "\n";
			}
		cout << " -- FINAL: " << CalcVal(itsData.itsVertices[i]) << "\n";
		}
	cout << "\n\nFinal Results:\n";
	
	cout << "Case 0: " << C0 << "\n";
	
	cout << "Case 1: " << C1 << "\n";
	
	cout << "Case 2: ";
	for(int j=0; j<5; j++){ cout << "[" << j << "]:" << C2[j] << "  "; }
	cout << "BAD:" << C2[5] << "\n";
	cout << "\n";

	cout << "Case 3: ";
	for(int k=0; k<12; k++){ cout << "[" << k << "]:" << C3[k] << "  "; }
	cout << "\n";
	
	cout << "Function Calls for Normal Search: " << FCT << "\n";
	cout << "Function Calls for Grid Calculation: " << itsGridSize[0]*itsGridSize[1]*itsGridSize[2] << "\n";

	cout << "EBAD: " << EBAD << "\n";

	}*/











