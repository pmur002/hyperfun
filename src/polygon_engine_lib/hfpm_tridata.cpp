/*==============================================================================

Copyright 1999 Eric Fausett
Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt 
Copyright 2007-2008 Oleg Fryazinov, Denis Kravtsov

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

HFPolyMesh::TriData::TriData()
{
	itsVBadCount=itsEBadCount=itsTBadCount=0;
}

HFPolyMesh::TriData::~TriData()
{
}

/****ADDED FOR ATTRIBUTES : int s coresponds to itsSSize*/
/*void HFPolyMesh::TriData::guessSize(int tNum,int s){
int vNum=(int)tNum/2;
guessSize(tNum, vNum,s);		
}*/


void HFPolyMesh::TriData::guessSize(int tNum, int vNum, int s)
{
	int eNum=(int)(2*tNum)/3;

	itsVCoordinates.reserve(vNum);
	itsVEdges.reserve(vNum);
	itsVBad.reserve(vNum);
	itsVFVal.reserve(vNum);
	//itsVNormal.reserve(vNum);  //Not needed due to init Normal

	itsEVertices.reserve(eNum);
	itsETriangles.reserve(eNum);
	itsEBad.reserve(eNum);
	itsELength.reserve(eNum);

	itsTEdges.reserve(tNum);
	itsTBad.reserve(tNum);
	itsTRev.reserve(tNum);

	itsOpenEdges.reserve(eNum);

	itsSSize = s;
	itsSAttributes.reserve(vNum);
}


int HFPolyMesh::TriData::pushBackVertex(double* x, double fvalue,double *s)
{
	vector<double> vx(3, 0.0);
	fillVector(x, 3, vx);
	itsVCoordinates.push_back(vx);
	//vector<int> tvi(0);
	itsVEdges.push_back(vector<int>(0));
	itsVBad.push_back(false);//Good New Vertex
	itsVFVal.push_back(fvalue);
	//itsVNormal Doesn't Exist Yet

	vector<double> vs(itsSSize,0.0);
	fillVector(s, itsSSize, vs);
	itsSAttributes.push_back(vs);

	return itsVCoordinates.size()-1;
}

void HFPolyMesh::TriData::initVNormal()
{
	itsVNormal.resize(getNumV());
	std::fill(itsVNormal.begin(), itsVNormal.end(), vector<double>(3, 0.0));
}

void HFPolyMesh::TriData::initTNormal()
{
	itsTNormal.resize(getNumT());
	std::fill(itsTNormal.begin(), itsTNormal.end(), vector<double>(3, 0.0));
}

int HFPolyMesh::TriData::pushBackTriangle(int v[3])
{
	int E[3]= {-1, -1, -1}; //0: v0_v1  1: v1_v2  2: v2_v0
	//Cycle Through Open Edges and close if the edge is part of the new triangle
	bool Break=false;
	for(unsigned int i=0; (i<itsOpenEdges.size()) && (!Break); i++){
		if(itsEVertices[itsOpenEdges[i]][0]==v[0]){
			if(itsEVertices[itsOpenEdges[i]][1]==v[1]){//Found Edge for v0_v1
				E[0]=itsOpenEdges[i];
				itsETriangles[itsOpenEdges[i]][1]=itsTEdges.size();//Save second Edge Triangle
				itsOpenEdges.erase(itsOpenEdges.begin()+i);//This Edge No Longer Open
				i--;//Adjust for Delete Above
			}
			else{
				if(itsEVertices[itsOpenEdges[i]][1]==v[2]){//Found Edge for v0_v2
					E[2]=itsOpenEdges[i];
					itsETriangles[itsOpenEdges[i]][1]=itsTEdges.size();//Save second Edge Triangle
					itsOpenEdges.erase(itsOpenEdges.begin()+i);//This Edge No Longer Open
					i--;//Adjust for Delete Above
				}
			}
		}
		else{
			if(itsEVertices[itsOpenEdges[i]][0]==v[1]){
				if(itsEVertices[itsOpenEdges[i]][1]==v[0]){//Found Edge for v1_v0
					E[0]=itsOpenEdges[i];
					itsETriangles[itsOpenEdges[i]][1]=itsTEdges.size();//Save second Edge Triangle
					itsOpenEdges.erase(itsOpenEdges.begin()+i);//This Edge No Longer Open
					i--;//Adjust for Delete Above
				}
				else{
					if(itsEVertices[itsOpenEdges[i]][1]==v[2]){//Found Edge for v1_v2
						E[1]=itsOpenEdges[i];
						itsETriangles[itsOpenEdges[i]][1]=itsTEdges.size();//Save second Edge Triangle
						itsOpenEdges.erase(itsOpenEdges.begin()+i);//This Edge No Longer Open
						i--;//Adjust for Delete Above
					}
				}
			}
			else{
				if(itsEVertices[itsOpenEdges[i]][0]==v[2]){
					if(itsEVertices[itsOpenEdges[i]][1]==v[0]){//Found Edge for v2_v0
						E[2]=itsOpenEdges[i];
						itsETriangles[itsOpenEdges[i]][1]=itsTEdges.size();//Save second Edge Triangle
						itsOpenEdges.erase(itsOpenEdges.begin()+i);//This Edge No Longer Open
						i--;//Adjust for Delete Above
					}
					else{
						if(itsEVertices[itsOpenEdges[i]][1]==v[1]){//Found Edge for v2_v1
							E[1]=itsOpenEdges[i];
							itsETriangles[itsOpenEdges[i]][1]=itsTEdges.size();//Save second Edge Triangle
							itsOpenEdges.erase(itsOpenEdges.begin()+i);//This Edge No Longer Open
							i--;//Adjust for Delete Above
						}
					}
				}
			}
		}
		//If all edges were found and closed then break;
		if((E[0]!=-1) && (E[1]!=-1) && (E[2]!=-1)) Break=true;
	}
	//Create new open edges
	if(!Break){//Cycle through Edges
		if(E[0]==-1){//We Need to Create a New Edge
			vector<int> ev(2, -1);
			ev[0]=v[0];
			ev[1]=v[1];
			itsEVertices.push_back(ev);//Create Edge
			itsOpenEdges.push_back(itsEVertices.size()-1);//Edge is Open so Push onto Open Edge vector
			vector<int> et(2, -1);//For Edge Triangle PushBack
			et[0]=itsTEdges.size();//Index of the new triangle that will be pushed back
			itsETriangles.push_back(et);//Save first Edge Triangle
			itsEBad.push_back(false);//Good New Edge
			itsELength.push_back(edgeLength(v[0],v[1]));//Calculate Edge Length
			//Deal With the Vertices VEdges
			itsVEdges[v[0]].push_back(itsEVertices.size()-1);
			itsVEdges[v[1]].push_back(itsEVertices.size()-1);
			E[0]=itsEVertices.size()-1;
		}
		if(E[1]==-1){//We Need to Create a New Edge
			vector<int> ev(2, -1);
			ev[0]=v[1];
			ev[1]=v[2];
			itsEVertices.push_back(ev);//Create Edge
			itsOpenEdges.push_back(itsEVertices.size()-1);//Edge is Open so Push onto Open Edge vector
			vector<int> et(2, -1);//For Edge Triangle PushBack
			et[0]=itsTEdges.size();//Index of the new triangle that will be pushed back
			itsETriangles.push_back(et);//Save first Edge Triangle
			itsEBad.push_back(false);//Good New Edge
			itsELength.push_back(edgeLength(v[1],v[2]));//Calculate Edge Length
			//Deal With the Vertices VEdges
			itsVEdges[v[1]].push_back(itsEVertices.size()-1);
			itsVEdges[v[2]].push_back(itsEVertices.size()-1);
			E[1]=itsEVertices.size()-1;
		}
		if(E[2]==-1){//We Need to Create a New Edge
			vector<int> ev(2, -1);
			ev[0]=v[2];
			ev[1]=v[0];
			itsEVertices.push_back(ev);//Create Edge
			itsOpenEdges.push_back(itsEVertices.size()-1);//Edge is Open so Push onto Open Edge vector
			vector<int> et(2, -1);//For Edge Triangle PushBack
			et[0]=itsTEdges.size();//Index of the new triangle that will be pushed back
			itsETriangles.push_back(et);//Save first Edge Triangle
			itsEBad.push_back(false);//Good New Edge
			itsELength.push_back(edgeLength(v[2],v[0]));//Calculate Edge Length
			//Deal With the Vertices VEdges
			itsVEdges[v[2]].push_back(itsEVertices.size()-1);
			itsVEdges[v[0]].push_back(itsEVertices.size()-1);
			E[2]=itsEVertices.size()-1;
		}
	}
	//PushBack Triangle
	vector<int> pbt(3, 0);
	fillVector(E, 3, pbt);
	itsTEdges.push_back(pbt);
	itsTBad.push_back(false);
	itsTRev.push_back(false);
	return itsTEdges.size()-1;
}

void HFPolyMesh::TriData::setVNormal(int v, vector<double> vnt)
{
	itsVNormal[v]=vnt;
}

void HFPolyMesh::TriData::setTNormal(int t, vector<double> tn)
{
	itsTNormal[t]=tn;
}

std::vector<double> HFPolyMesh::TriData::calcTNormal(int t)
{
	vector<int> ttri(3, 0);
	vector<double> tA(3, 0.0);//First vector in Triangle
	vector<double> tB(3, 0.0);//Second vector in Triangle
	vector<double> tC(3, 0.0);//tC=tA(cross)tB

	ttri[0]=itsEVertices[itsTEdges[t][0]][0];
	ttri[1]=itsEVertices[itsTEdges[t][0]][1];

	int test=itsEVertices[itsTEdges[t][1]][0];
	if ((test!=ttri[0]) && (test!=ttri[1])) 
		ttri[2]=test;
	else 
		ttri[2]=itsEVertices[itsTEdges[t][1]][1];
	tA[0] = getVertex(ttri[0], 0) - getVertex(ttri[1], 0);
	tA[1] = getVertex(ttri[0], 1) - getVertex(ttri[1], 1);
	tA[2] = getVertex(ttri[0], 2) - getVertex(ttri[1], 2);

	tB[0] = getVertex(ttri[2], 0) - getVertex(ttri[1], 0);
	tB[1] = getVertex(ttri[2], 1) - getVertex(ttri[1], 1);
	tB[2] = getVertex(ttri[2], 2) - getVertex(ttri[1], 2);

	tC[0] = (tB[1] * tA[2])-(tB[2] * tA[1]);
	tC[1] = (tB[2] * tA[0])-(tB[0] * tA[2]);
	tC[2] = (tB[0] * tA[1])-(tB[1] * tA[0]);

	if (itsTRev[t])
	{
		tC[0] = -tC[0];
		tC[1] = -tC[1];
		tC[2] = -tC[2];
	}
	double tclen = sqrt(tC[0]*tC[0] + tC[1]*tC[1] + tC[1]*tC[1]);
	if (tclen != 0.0){
		tC[0] /= tclen;
		tC[1] /= tclen;
		tC[2] /= tclen;
	}
	return tC;
}

void HFPolyMesh::TriData::setFunctVal(int v, double fvalue)
{
	//No v index Checking!! Be careful!!
	itsVFVal[v]=fvalue;
}

/*******ADDED FOR ATTRIBUTES */
void HFPolyMesh::TriData::setAttributes(int i, double *s)
{
	for(int j=0;j<itsSSize;j++)
		itsSAttributes[i][j]=s[j];
}

void HFPolyMesh::TriData::deleteEdge(int e){
	//No e index Checking!! Be careful!!
	//Finde v1 to colapse to v2
	unsigned int i;
	double avLen1=0;
	int ve1=itsEVertices[e][0];
	for(i=0; i<itsVEdges[ve1].size(); i++)
		avLen1+=itsELength[itsVEdges[e][i]];
	avLen1 /= i;
	double avLen2=0;
	int ve2=itsEVertices[e][1];
	for(i=0; i<itsVEdges[ve2].size(); i++)
		avLen2+=itsELength[itsVEdges[e][i]];
	avLen2 /= i;

	itsEBad[e]=true;//Delete Edge
	itsEBadCount++;
	itsTBad[itsETriangles[e][0]]=true;//Delete First Triangle
	itsTBadCount++;
	itsTBad[itsETriangles[e][1]]=true;//Delete Second Triangle
	itsTBadCount++;

	if(avLen1>avLen2){//Then Colapse v2 to v1
		itsVBad[ve2]=true;//Delete Vertex 2
		itsVBadCount++;
		for(i=0; i<itsVEdges[ve2].size(); i++){//Move all v2 edges to v1
			int e=itsVEdges[ve2][i];
			if(itsEVertices[e][0]==ve2){
				itsEVertices[e][0]=ve1;//Change Vertex to ve1
				itsELength[e]=edgeLength(itsEVertices[e][0], itsEVertices[e][1]);
				itsVEdges[ve1].push_back(e);//Add new edge to Vertex Edges of v1
			}
			else{
				if(itsEVertices[e][1]==ve2){
					itsEVertices[e][1]=ve1;//Change Vertex to ve1
					itsELength[e]=edgeLength(itsEVertices[e][0], itsEVertices[e][1]);
					itsVEdges[ve1].push_back(e);//Add new edge to Vertex Edges of v1
				}
			}
		}
	}
	else{//avLen2>=avLen1 so Colapse v1 to v2
		itsVBad[ve1]=true;//Delete Vertex 1
		itsVBadCount++;
		for(i=0; i<itsVEdges[ve1].size(); i++){//Move all v1 edges to v2
			int e=itsVEdges[ve1][i];
			if(itsEVertices[e][0]==ve1){
				itsEVertices[e][0]=ve2;//Change Vertex to ve2
				itsELength[e]=edgeLength(itsEVertices[e][0], itsEVertices[e][1]);
				itsVEdges[ve2].push_back(e);//Add new edge to Vertex Edges of v2
			}
			else{
				if(itsEVertices[e][1]==ve1){
					itsEVertices[e][1]=ve2;//Change Vertex to ve2
					itsELength[e]=edgeLength(itsEVertices[e][0], itsEVertices[e][1]);
					itsVEdges[ve2].push_back(e);//Add new edge to Vertex Edges of v2
				}
			}
		}
	}
}

void HFPolyMesh::TriData::refresh(){
	refreshTriangles();
	refreshEdges();
	refreshVerticies();
	ccwTriangles();
}

void HFPolyMesh::TriData::ccwTriangles(){
	vector<int> ttri(3, 0);
	vector<double> tA(3, 0.0);//First vector in Triangle
	vector<double> tB(3, 0.0);//Second vector in Triangle
	vector<double> tC(3, 0.0);//tC=tA(cross)tB
	vector<double> tN(3, 0.0);//tN is normal vector
	double direction;
	int t, reverse;
	for(t=0; t<getNumT(); t++){
		ttri[0]=itsEVertices[itsTEdges[t][0]][0];
		ttri[1]=itsEVertices[itsTEdges[t][0]][1];
		int test=itsEVertices[itsTEdges[t][1]][0];
		if((test!=ttri[0]) && (test!=ttri[1])) ttri[2]=test;
		else ttri[2]=itsEVertices[itsTEdges[t][1]][1];
		tA[0] = getVertex(ttri[0], 0) - getVertex(ttri[1], 0);
		tA[1] = getVertex(ttri[0], 1) - getVertex(ttri[1], 1);
		tA[2] = getVertex(ttri[0], 2) - getVertex(ttri[1], 2);

		tB[0] = getVertex(ttri[2], 0) - getVertex(ttri[1], 0);
		tB[1] = getVertex(ttri[2], 1) - getVertex(ttri[1], 1);
		tB[2] = getVertex(ttri[2], 2) - getVertex(ttri[1], 2);

		//tC[0] = (tA[1] * tB[2])-(tA[2] * tB[1]);
		//tC[1] = (tA[2] * tB[0])-(tA[0] * tB[2]);
		//tC[2] = (tA[0] * tB[1])-(tA[1] * tB[0]);
		tC[0] = (tB[1] * tA[2])-(tB[2] * tA[1]);
		tC[1] = (tB[2] * tA[0])-(tB[0] * tA[2]);
		tC[2] = (tB[0] * tA[1])-(tB[1] * tA[0]);


		//tN = getNormal(ttri[1]);
		vector<double> N0=getVNormal(ttri[0]);
		vector<double> N1=getVNormal(ttri[1]);
		vector<double> N2=getVNormal(ttri[2]);
/*
		tN[0]=(N0[0]+N1[0]+N2[0])/3;
		tN[1]=(N0[1]+N1[1]+N2[1])/3;
		tN[2]=(N0[2]+N1[2]+N2[2])/3;
		direction = (tC[0] * tN[0])+(tC[1] * tN[1])+(tC[2] * tN[2]);
*/
		reverse = 0;
		direction = (tC[0] * N0[0])+(tC[1] * N0[1])+(tC[2] * N0[2]);
		if (direction < 0)
			reverse++;
		direction = (tC[0] * N1[0])+(tC[1] * N1[1])+(tC[2] * N1[2]);
		if (direction < 0)
			reverse++;
		direction = (tC[0] * N2[0])+(tC[1] * N2[1])+(tC[2] * N2[2]);
		if (direction < 0)
			reverse++;

		if (reverse > 1)
			itsTRev[t]=true;
		else
			itsTRev[t]=false;
/*
		//		if(fabs(direction)<0.001) cout << "direction: " << direction << "\n";
		if(direction>0) 
			itsTRev[t]=false;
		else 
			itsTRev[t]=true;
*/
	}
}

void HFPolyMesh::TriData::refreshTriangles(){
	unsigned int i;
	vector<int> TMap(itsTEdges.size(), 0);
	vector<vector<int> > newTEdges;
	vector<bool> newTRev;
	int tsize=itsTEdges.size()-itsTBadCount;
	newTEdges.reserve(tsize);
	newTRev.reserve(tsize);
	int counter=0;
	//Create newTEdges, newTRev, and TMap for Mapping
	for(i=0; i<itsTEdges.size(); i++){
		if(itsTBad[i]) counter++;
		else{
			newTEdges.push_back(itsTEdges[i]);//Good Triangle
			newTRev.push_back(itsTRev[i]);
		}
		TMap[i]=i-counter;
	}
	itsTEdges=newTEdges;
	itsTRev=newTRev;
	//Map all T Indeces
	for(i=0; i<itsETriangles.size(); i++){
		if(!itsEBad[i]){
			itsETriangles[i][0]=TMap[itsETriangles[i][0]];
			itsETriangles[i][1]=TMap[itsETriangles[i][1]];
		}
	}
	itsTBad.clear();
	itsTBad.resize(itsTEdges.size(), false);
}

void HFPolyMesh::TriData::refreshEdges(){
	unsigned int i;
	vector<int> EMap(itsEVertices.size(), 0);
	vector<vector<int> > newEVertices;
	vector<vector<int> > newETriangles;
	vector<double> newELength;
	int esize=itsEVertices.size()-itsEBadCount;
	newEVertices.reserve(esize);
	newETriangles.reserve(esize);
	newELength.reserve(esize);
	int counter=0;
	//Create newEVertices, new ETriangles, and newELength and EMap for Mapping
	for(i=0; i<itsEVertices.size(); i++){
		if(itsEBad[i]) counter++;
		else{//Good Edge
			newEVertices.push_back(itsEVertices[i]);
			newETriangles.push_back(itsETriangles[i]);
			newELength.push_back(itsELength[i]);
		}
		EMap[i]=i-counter;
	}
	itsEVertices=newEVertices;
	itsETriangles=newETriangles;
	itsELength=newELength;
	//Map all E Indeces
	for(i=0; i<itsTEdges.size(); i++){
		//No Bad Triangles should be left
		itsTEdges[i][0]=EMap[itsTEdges[i][0]];
		itsTEdges[i][1]=EMap[itsTEdges[i][1]];
		itsTEdges[i][2]=EMap[itsTEdges[i][2]];
	}
	for(i=0; i<itsVEdges.size(); i++){
		if(!itsVBad[i]){
			for(unsigned int j=0; j<itsVEdges[i].size(); j++)
				itsVEdges[i][j]=EMap[itsVEdges[i][j]];
		}
	}
	itsEBad.clear();
	itsEBad.resize(itsEVertices.size(), false);
}

void HFPolyMesh::TriData::refreshVerticies(){
	unsigned int i;
	vector<int> VMap(itsVCoordinates.size(), 0);
	vector<vector<double> > newVCoordinates;
	vector<vector<int> > newVEdges;
	vector<double> newVFVal;
	vector<vector<double> > newVNormal;
	int vsize=itsVCoordinates.size()-itsVBadCount;
	newVCoordinates.reserve(vsize);
	newVEdges.reserve(vsize);
	newVFVal.reserve(vsize);
	if(itsVNormal.size()>0)
		newVNormal.reserve(vsize);
	int counter=0;
	//Create newVCoordinates, newVEdges, newVFVal, and newVNormal and VMap for Mapping
	for(i=0; i<itsVCoordinates.size(); i++){
		if(itsVBad[i]) counter++;
		else{//Good Vertex
			newVCoordinates.push_back(itsVCoordinates[i]);
			newVEdges.push_back(itsVEdges[i]);
			newVFVal.push_back(itsVFVal[i]);
			if(itsVNormal.size()>0)
				newVNormal.push_back(itsVNormal[i]);
		}
		VMap[i]=i-counter;
	}
	itsVCoordinates=newVCoordinates;
	itsVEdges=newVEdges;
	itsVFVal=newVFVal;
	if(itsVNormal.size()>0)
		itsVNormal=newVNormal;
	//Map all V Indeces
	for(i=0; i<itsEVertices.size(); i++){
		//No Bad Edges should be left
		itsEVertices[i][0]=VMap[itsEVertices[i][0]];
		itsEVertices[i][1]=VMap[itsEVertices[i][1]];
		itsEVertices[i][2]=VMap[itsEVertices[i][2]];
	}
	itsVBad.clear();
	itsVBad.resize(itsVCoordinates.size(), false);
}

int HFPolyMesh::TriData::getNumV(){
	return itsVCoordinates.size();
}

int HFPolyMesh::TriData::getNumT(){
	return itsTEdges.size();
}

vector<double> HFPolyMesh::TriData::getVertex(int v){
	//No v index Checking!! Be careful!!
	return itsVCoordinates[v];
}

double HFPolyMesh::TriData::getVertex(int v, int x){
	//No v or x index Checking!! Be careful!!
	return itsVCoordinates[v][x];
}

vector<double> HFPolyMesh::TriData::getVNormal(int v){
	//No v index Checking!! Be careful!!
	return itsVNormal[v];
}

double HFPolyMesh::TriData::getVNormal(int v, int x){
	//No v or x index Checking!! Be careful!!
	return itsVNormal[v][x];
}

double HFPolyMesh::TriData::getTNormal(int v, int x){
	//No v or x index Checking!! Be careful!!
	return itsTNormal[v][x];
}

double HFPolyMesh::TriData::getFunctVal(int v){
	//No v index Checking!! Be careful!!
	return itsVFVal[v];
}	

vector<int> HFPolyMesh::TriData::getTriangle(int t){
	//No t index Checking!! Be careful!!
	vector<int> ret;
	ret.push_back(itsEVertices[itsTEdges[t][0]][0]);
	ret.push_back(itsEVertices[itsTEdges[t][0]][1]);
	int test=itsEVertices[itsTEdges[t][1]][0];
	if((test!=ret[0]) && (test!=ret[1])) ret.push_back(test);
	else ret.push_back(itsEVertices[itsTEdges[t][1]][1]);
	if(itsTRev[t]){
		int tmpr=ret[2];
		ret[2]=ret[1];
		ret[1]=tmpr;
	}
	return ret;	
}

int HFPolyMesh::TriData::getTriangle(int t, int vnum){
	//No t or vnum index Checking!! Be careful!!
	switch(vnum){
		int test;
		case 0:
			return itsEVertices[itsTEdges[t][0]][0];
			break;
		case 1:
			if(itsTRev[t]){
				test=itsEVertices[itsTEdges[t][1]][0];
				if((test!=itsEVertices[itsTEdges[t][0]][0]) && (test!=itsEVertices[itsTEdges[t][0]][1])) ;
				else test=itsEVertices[itsTEdges[t][1]][1];
				return test;
			}
			else
				return itsEVertices[itsTEdges[t][0]][1];			
			break;
		case 2:
			if(itsTRev[t])
				return itsEVertices[itsTEdges[t][0]][1];
			else{
				test=itsEVertices[itsTEdges[t][1]][0];
				if((test!=itsEVertices[itsTEdges[t][0]][0]) && (test!=itsEVertices[itsTEdges[t][0]][1])) ;
				else test=itsEVertices[itsTEdges[t][1]][1];
				return test;
			}			
			break;
		default:
			return 0;
			break;
	}
}


/*******ADDED FOR ATTRIBUTES *****/
vector<double> HFPolyMesh::TriData::getAttributes(int v){
	//No v index Checking!! Be careful!!
	return itsSAttributes[v];
}

double HFPolyMesh::TriData::getAttributes(int v, int x){
	//No v or x index Checking!! Be careful!!
	return itsSAttributes[v][x];
}

/********************************/

/*
void HFPolyMesh::TriData::fillDoubleVector(double* d, int size, vector<double>& ret){
for(int i=0; i<size; i++){
ret[i]=d[i];
}
}

void HFPolyMesh::TriData::fillIntVector(int* d, int size, vector<int>& ret){
for(int i=0; i<size; i++){
ret[i]=d[i];
}
}
*/
template <typename T>
inline void HFPolyMesh::TriData::fillVector(const T* d, int size, vector<T>& ret) const {
	for(int i=0; i < size; i++){
		ret[i] = (T)d[i];
	}
}

double HFPolyMesh::TriData::edgeLength(int v1, int v2){
	double x0 = itsVCoordinates[v1][0]-itsVCoordinates[v2][0];
	double x1 = itsVCoordinates[v1][1]-itsVCoordinates[v2][1];
	double x2 = itsVCoordinates[v1][2]-itsVCoordinates[v2][2];
	x0 *= x0;
	x1 *= x1;
	x2 *= x2;
	double ret = x0+x1+x2;
	return sqrt(ret);	
}


/*****************************/
/***ATTRIBUTE : Function used for refinement
/***i.e. if the function value is negative
/***One has to find the closest vertex along the
/***normal where it is positive*/
//double HFPolyMesh::TriData::getVal(int i){ 
//	return itsVFVal[i];
//}

void HFPolyMesh::TriData::ReplaceVertex(int i,vector<double> x,double val,double *attr){
	itsVCoordinates[i][0]=x[0];
	itsVCoordinates[i][1]=x[1];
	itsVCoordinates[i][2]=x[2];
	itsVFVal[i]=val;

	for(int j=0;j<itsSSize;j++) 
		itsSAttributes[i][j] = attr[j];



}
