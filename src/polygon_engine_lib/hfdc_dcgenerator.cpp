/*==============================================================================

 Copyright 2003 Yutaka Ohtake
 Copyright 2008 Oleg Fryazinov


 This Work or file is part of the greater total Work, software or group of
 files named HyperFun Polygonizer.

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

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>

#include "hfdc_dcgenerator.h"

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

using namespace std;

DcGenerator::DcGenerator()
{
	value = NULL;
	quadMesh = NULL;
	hf_mesh = NULL;
	gX = gY = gZ = 0;
	p.resize(3, 0);
	IMAX = 10;
	pTmpS = NULL;
}

DcGenerator::~DcGenerator()
{
	if (pTmpS)
		delete[] pTmpS;
}

HFPolyMesh* DcGenerator::generate()
{
	pTmpS = new double[func->getSSize()];

	cout << endl << "Sampling in 3d-grids" << endl;
	sampleValue();

	cout << "Generating Quad Mesh" << endl;
	generateQuadMesh();
//	cout << quadMesh->face_N << "quad" << endl;
//	cout << quadMesh->vertex_N << "vertex" << endl;

	cout << "Converting HFPolyMesh" << endl;
	createHfMesh();
	cout << "Polygonization is done" << endl;
	hf_mesh->GeneralRefinement();
	return hf_mesh;
}

void DcGenerator::setGridN(int x, int y, int z)
{
	gX = x + 2;
	gY = y + 2;
	gZ = z + 2;
}

void DcGenerator::setBox(double _minX, double _maxX, double _minY, double _maxY, double _minZ, double _maxZ){
	minX = _minX;
	minY = _minY;
	minZ = _minZ;
	maxX = _maxX;
	maxY = _maxY;
	maxZ = _maxZ;

	double sx = maxX - minX;
	double sy = maxY - minY;
	double sz = maxZ - minZ;
	double dia = sqrt(sx*sx + sy*sy + sz*sz);
	eps = EPS*dia;
	h = eps;
}

void DcGenerator::setFunc(HFInterpreter* _func, double _iso)
{
	func = _func;
	iso = _iso;
}

void DcGenerator::setParameters(std::vector<double>& params)
{
	func->parameters(params);
}

void DcGenerator::sampleValue()
{
	int i;
	double iX = (maxX - minX)/(gX-1);
	double iY = (maxY - minY)/(gY-1);
	double iZ = (maxZ - minZ)/(gZ-1);
	value = (double ***)new double*[gZ];
	for(i=0; i<gZ; i++){
		cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << i << "/" << gZ;
		double z = minZ + i*iZ;
		value[i] = new double*[gY];
		for(int j=0; j<gY; j++){
			double y = minY + j*iY;
			value[i][j] = new double[gX];
			for(int k=0; k<gX; k++){
				double x = minX + k*iX;
				value[i][j][k] = evalFunc(x, y, z, pTmpS);
			}
		}
	}
	cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b" << i << "/" << gZ << endl;
}

void DcGenerator::generateQuadMesh()
{
	int i, j, k;

	//Vertex ID table
	int ***ID = (int ***)new int*[gZ-1];
	for(i=0; i<gZ-1; i++){
		ID[i] = new int*[gY-1];
		for(j=0; j<gY-1; j++){
			ID[i][j] = new int[gX-1];
		}
	}

	//count the number of vertices
	//and assign vertex ID to each cube
	//that contains at least one sign-changed edge.
	int vN = 0;
	for(i=0; i<gZ-1; i++){
		for(j=0; j<gY-1; j++){
			for(k=0; k<gX-1; k++){
				bool flag = (value[i][j][k] > 0);
				if(flag != (value[i][j][k+1] > 0) ||
					flag != (value[i][j+1][k] > 0) ||
					flag != (value[i][j+1][k+1] > 0) ||
					flag != (value[i+1][j][k] > 0) ||
					flag != (value[i+1][j][k+1] > 0) ||
					flag != (value[i+1][j+1][k] > 0) ||
					flag != (value[i+1][j+1][k+1] > 0)){
						ID[i][j][k] = vN++;
				}
				else{
					ID[i][j][k] = -1;
				}
			}
		}
	}

	//init vertex positions and error-quadrics
	quadMesh = new QuadMesh();
	quadMesh->setVertexN(vN);
	double (*Q)[9] = new double[vN][9];
	for(i=0; i<vN; i++){
		Q[i][0] = Q[i][1] = Q[i][2] = Q[i][3] = Q[i][4] =
			Q[i][5] = Q[i][6] = Q[i][7] = Q[i][8] = 0;
	}

	//create connectivity and compute error-quadrics
	double iX = (maxX-minX)/(gX-1);
	double iY = (maxY-minY)/(gY-1);
	double iZ = (maxZ-minZ)/(gZ-1);
	//Edges in x direction
	for(i=1; i<gZ-1; i++){
		double z = minZ + i*iZ;
		for(j=1; j<gY-1; j++){
			double y = minY + j*iY;
			for(k=1; k<gX-2; k++){
				double v1 = value[i][j][k];
				double v2 = value[i][j][k+1];
				if(v1*v2 < 0){
					double x1 = minX + k*iX;
					double x2 = minX + (k+1)*iX;

					//find Hermite data along the edge
					std::vector<double> p, n;
					convergeToZero(p, x1, y, z, v1, x2, y, z, v2);
					nablaFuncF(n, p[0], p[1], p[2]);
					double len = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
					if(len == 0)
						continue;
					n[0] = n[0]/len;
					n[1] = n[1]/len;
					n[2] = n[2]/len;

					//Quadric to be added
					double Q_tmp[9];
					Q_tmp[0] = n[0]*n[0];
					Q_tmp[1] = n[0]*n[1];
					Q_tmp[2] = n[0]*n[2];
					Q_tmp[3] = n[1]*n[1];
					Q_tmp[4] = n[1]*n[2];
					Q_tmp[5] = n[2]*n[2];
					double dot = p[0]*n[0] + p[1]*n[1] + p[2]*n[2];
					Q_tmp[6] = dot*n[0];
					Q_tmp[7] = dot*n[1];
					Q_tmp[8] = dot*n[2];

					//vertex ID
					int i1 = ID[i-1][j-1][k];
					int i2 = ID[i-1][j][k];
					int i3 = ID[i][j-1][k];
					int i4 = ID[i][j][k];

					//error check (never satisfied, I hope)
					if(i1 < 0 || i2 < 0 || i3 < 0 || i4 < 0){
						cout << "Error: found a strange edge?" << endl;
						continue;
					}

					//add quadrics
					for(int l=0; l<9; l++){
						Q[i1][l] += Q_tmp[l];
						Q[i2][l] += Q_tmp[l];
						Q[i3][l] += Q_tmp[l];
						Q[i4][l] += Q_tmp[l];
					}
					//add face to the mesh
					if(v1 > v2)
						quadMesh->addFace(i1, i2, i4, i3, n);
					else
						quadMesh->addFace(i2, i1, i3, i4, n);

					quadMesh->degree[i1]++;
					quadMesh->degree[i2]++;
					quadMesh->degree[i3]++;
					quadMesh->degree[i4]++;

					quadMesh->vertex[i1][0] += p[0];
					quadMesh->vertex[i1][1] += p[1];
					quadMesh->vertex[i1][2] += p[2];

					quadMesh->vertex[i2][0] += p[0];
					quadMesh->vertex[i2][1] += p[1];
					quadMesh->vertex[i2][2] += p[2];

					quadMesh->vertex[i3][0] += p[0];
					quadMesh->vertex[i3][1] += p[1];
					quadMesh->vertex[i3][2] += p[2];

					quadMesh->vertex[i4][0] += p[0];
					quadMesh->vertex[i4][1] += p[1];
					quadMesh->vertex[i4][2] += p[2];
				}
			}
		}
	}

	//Edges in y direction
	for(i=1; i<gX-1; i++){
		double x = minX + i*iX;
		for(j=1; j<gZ-1; j++){
			double z = minZ + j*iZ;
			for(k=1; k<gY-2; k++){
				double v1 = value[j][k][i];
				double v2 = value[j][k+1][i];
				if(v1*v2 < 0){
					double y1 = minY + k*iY;
					double y2 = minY + (k+1)*iY;

					//find Hermite data along the edge
					std::vector<double> p, n;
					convergeToZero(p, x, y1, z, v1, x, y2, z, v2);
					nablaFuncF(n, p[0], p[1], p[2]);
					double len = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
					if(len == 0)
						continue;
					n[0] = n[0]/len;
					n[1] = n[1]/len;
					n[2] = n[2]/len;

					//Quadric to be added
					double Q_tmp[9];
					Q_tmp[0] = n[0]*n[0];
					Q_tmp[1] = n[0]*n[1];
					Q_tmp[2] = n[0]*n[2];
					Q_tmp[3] = n[1]*n[1];
					Q_tmp[4] = n[1]*n[2];
					Q_tmp[5] = n[2]*n[2];
					double dot = p[0]*n[0] + p[1]*n[1] + p[2]*n[2];
					Q_tmp[6] = dot*n[0];
					Q_tmp[7] = dot*n[1];
					Q_tmp[8] = dot*n[2];

					//vertex ID
					int i1 = ID[j-1][k][i-1];
					int i2 = ID[j][k][i-1];
					int i3 = ID[j-1][k][i];
					int i4 = ID[j][k][i];

					//error check (never satisfied, I hope)
					if(i1 < 0 || i2 < 0 || i3 < 0 || i4 < 0){
						cout << "Error: found a strange edge?" << endl;
						continue;
					}

					//add quadrics
					for(int l=0; l<9; l++){
						Q[i1][l] += Q_tmp[l];
						Q[i2][l] += Q_tmp[l];
						Q[i3][l] += Q_tmp[l];
						Q[i4][l] += Q_tmp[l];
					}
					//add face to the mesh
					if(v1 > v2)
						quadMesh->addFace(i1, i2, i4, i3, n);
					else
						quadMesh->addFace(i2, i1, i3, i4, n);

					quadMesh->degree[i1]++;
					quadMesh->degree[i2]++;
					quadMesh->degree[i3]++;
					quadMesh->degree[i4]++;

					quadMesh->vertex[i1][0] += p[0];
					quadMesh->vertex[i1][1] += p[1];
					quadMesh->vertex[i1][2] += p[2];

					quadMesh->vertex[i2][0] += p[0];
					quadMesh->vertex[i2][1] += p[1];
					quadMesh->vertex[i2][2] += p[2];

					quadMesh->vertex[i3][0] += p[0];
					quadMesh->vertex[i3][1] += p[1];
					quadMesh->vertex[i3][2] += p[2];

					quadMesh->vertex[i4][0] += p[0];
					quadMesh->vertex[i4][1] += p[1];
					quadMesh->vertex[i4][2] += p[2];
				}
			}
		}
	}

	//Edges in z direction
	for(i=1; i<gY-1; i++){
		double y = minY + i*iY;
		for(j=1; j<gX-1; j++){
			double x = minX + j*iX;
			for(k=1; k<gZ-2; k++){
				double v1 = value[k][i][j];
				double v2 = value[k+1][i][j];
				if(v1*v2 < 0){
					double z1 = minZ + k*iZ;
					double z2 = minZ + (k+1)*iZ;

					//find Hermite data along the edge
					std::vector<double> p, n;
					convergeToZero(p, x, y, z1, v1, x, y, z2, v2);
					nablaFuncF(n, p[0], p[1], p[2]);
					double len = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
					if(len == 0)
						continue;
					n[0] = n[0]/len;
					n[1] = n[1]/len;
					n[2] = n[2]/len;

					//Quadric to be added
					double Q_tmp[9];
					Q_tmp[0] = n[0]*n[0];
					Q_tmp[1] = n[0]*n[1];
					Q_tmp[2] = n[0]*n[2];
					Q_tmp[3] = n[1]*n[1];
					Q_tmp[4] = n[1]*n[2];
					Q_tmp[5] = n[2]*n[2];
					double dot = p[0]*n[0] + p[1]*n[1] + p[2]*n[2];
					Q_tmp[6] = dot*n[0];
					Q_tmp[7] = dot*n[1];
					Q_tmp[8] = dot*n[2];

					//vertex ID
					int i1 = ID[k][i-1][j-1];
					int i2 = ID[k][i-1][j];
					int i3 = ID[k][i][j-1];
					int i4 = ID[k][i][j];

					//error check (never satisfied, I hope)
					if(i1 < 0 || i2 < 0 || i3 < 0 || i4 < 0){
						cout << "Error: found a strange edge?" << endl;
						continue;
					}

					//add quadrics
					for(int l=0; l<9; l++){
						Q[i1][l] += Q_tmp[l];
						Q[i2][l] += Q_tmp[l];
						Q[i3][l] += Q_tmp[l];
						Q[i4][l] += Q_tmp[l];
					}
					//add face to the mesh
					if(v1 > v2)
						quadMesh->addFace(i1, i2, i4, i3, n);
					else
						quadMesh->addFace(i2, i1, i3, i4, n);

					quadMesh->degree[i1]++;
					quadMesh->degree[i2]++;
					quadMesh->degree[i3]++;
					quadMesh->degree[i4]++;

					quadMesh->vertex[i1][0] += p[0];
					quadMesh->vertex[i1][1] += p[1];
					quadMesh->vertex[i1][2] += p[2];

					quadMesh->vertex[i2][0] += p[0];
					quadMesh->vertex[i2][1] += p[1];
					quadMesh->vertex[i2][2] += p[2];

					quadMesh->vertex[i3][0] += p[0];
					quadMesh->vertex[i3][1] += p[1];
					quadMesh->vertex[i3][2] += p[2];

					quadMesh->vertex[i4][0] += p[0];
					quadMesh->vertex[i4][1] += p[1];
					quadMesh->vertex[i4][2] += p[2];
				}
			}
		}
	}

	//free 
	for(i=0; i<gZ; i++){
		for(int j=0; j<gY; j++){
			delete[] value[i][j];
		}
		delete[] value[i];
	}
	delete[] value;

	for(i=0; i<gZ-1; i++){
		for(int j=0; j<gY-1; j++){
			delete[] ID[i][j];
		}
		delete[] ID[i];
	}
	delete[] ID;

	//decide vertex postion
	//For SVD
	float **A = new float*[4];
	float **v = new float*[4];
	for(i=1; i<4; i++){
		A[i] = new float[4];
		v[i] = new float[4];
	}
	float x[4], b[4], w[4];
	for(i=0; i<vN; i++){
		if(quadMesh->degree[i] == 0)
			continue;
		double* Qi = Q[i];
		double ideg = 1.0f/quadMesh->degree[i];

		//centroid of edge points
		double px = quadMesh->vertex[i][0] *= ideg;
		double py = quadMesh->vertex[i][1] *= ideg;
		double pz = quadMesh->vertex[i][2] *= ideg;
		//continue;
		ideg = 1;
		A[1][1] = (float)(Qi[0]*ideg);
		A[1][2] = A[2][1] = (float)(Qi[1]*ideg);
		A[1][3] = A[3][1] = (float)(Qi[2]*ideg);
		A[2][2] = (float)(Qi[3]*ideg);
		A[2][3] = A[3][2] = (float)(Qi[4]*ideg);
		A[3][3] = (float)(Qi[5]*ideg);

		//origin is shifted for numercal stability
		b[1] = (float)((Qi[6] - Qi[0]*px - Qi[1]*py - Qi[2]*pz)*ideg);
		b[2] = (float)((Qi[7] - Qi[1]*px - Qi[3]*py - Qi[4]*pz)*ideg);
		b[3] = (float)((Qi[8] - Qi[2]*px - Qi[4]*py - Qi[5]*pz)*ideg);

		//solve Ax=b by SVD
		COMP::svdcmp(A, 3, 3, w, v);
		for(int k=1;k<=3;k++)
			if(fabs(w[k]) < 0.1) w[k] = 0.0;
		COMP::svbksb(A, w, v, 3, 3, b, x);

#ifdef WIN32
		if(_isnan(x[1]) || _isnan(x[2]) || _isnan(x[3]))
			continue;
#else
		if(__isnan(x[1]) || __isnan(x[2]) || __isnan(x[3]))
			continue;
#endif
		quadMesh->vertex[i][0] += x[1];
		quadMesh->vertex[i][1] += x[2];
		quadMesh->vertex[i][2] += x[3];
	}

	//free
	for(i=1; i<4; i++){
		delete[] A[i];
		delete[] v[i];
	}
	delete[] Q;
	delete[] A;
	delete[] v;
}    

void DcGenerator::createHfMesh()
{
	//cut quads to triangles
	int i;
	int fN = quadMesh->face_N;
	int vN = quadMesh->vertex_N;

	//compute vertex normals
	for(i=0; i<vN; i++)
		quadMesh->normal_v[i][0] = quadMesh->normal_v[i][1] = quadMesh->normal_v[i][2] = 0;
	for(i=0; i<fN; i++){
		for(int j=0; j<4; j++){
			quadMesh->normal_v[quadMesh->face[i][j]][0] -= quadMesh->normal_f[i][0];
			quadMesh->normal_v[quadMesh->face[i][j]][1] -= quadMesh->normal_f[i][1];
			quadMesh->normal_v[quadMesh->face[i][j]][2] -= quadMesh->normal_f[i][2];
		}
	}

	//Construct output mesh
	hf_mesh = new HFPolyMesh(*func);
	hf_mesh->itsData.guessSize(fN*2, vN, func->getSSize());
	
	std::vector<int> vecGrid;
	vecGrid.push_back(gX); vecGrid.push_back(gY); vecGrid.push_back(gZ);
	hf_mesh->Grid(vecGrid);

	std::vector<double> vecBox;
	vecBox.push_back(minX);	vecBox.push_back(minY); vecBox.push_back(minZ);
	vecBox.push_back(maxX);	vecBox.push_back(maxY); vecBox.push_back(maxZ);
	hf_mesh->MinMax(vecBox);
	hf_mesh->Normals(true);

	for (i = 0; i < vN; i++)
	{
		double p[3], val;
		p[0] = quadMesh->vertex[i][0]; p[1] = quadMesh->vertex[i][1]; p[2] = quadMesh->vertex[i][2];
		memset(pTmpS, 0, func->getSSize()*sizeof(double));
		val = evalFunc(quadMesh->vertex[i][0], quadMesh->vertex[i][1], quadMesh->vertex[i][2], pTmpS);
		hf_mesh->itsData.pushBackVertex(p, val, pTmpS);
	}

	hf_mesh->itsData.initVNormal();
	for (i = 0; i < vN; i++)
	{
		std::vector<double> n;
		n.push_back(quadMesh->normal_v[i][0]);
		n.push_back(quadMesh->normal_v[i][1]);
		n.push_back(quadMesh->normal_v[i][2]);
		hf_mesh->itsData.setVNormal(i, n);
	}

	for(i=0; i<fN; i++){
		int i1 = quadMesh->face[i][0];
		int i2 = quadMesh->face[i][1];
		int i3 = quadMesh->face[i][2];
		int i4 = quadMesh->face[i][3];
		int f[3];
		int t1, t2;

		double a1, a2, min1, min2;
		//cut along (i1,i3)
		a1 = angle(quadMesh->vertex[i1], quadMesh->vertex[i2], quadMesh->vertex[i3]);
		a2 = angle(quadMesh->vertex[i3], quadMesh->vertex[i4], quadMesh->vertex[i1]);
		if(a1 < a2)
			min1 = a1;
		else
			min1 = a2;
		//cut along (i2,i3)
		a1 = angle(quadMesh->vertex[i1], quadMesh->vertex[i2], quadMesh->vertex[i4]);
		a2 = angle(quadMesh->vertex[i3], quadMesh->vertex[i4], quadMesh->vertex[i2]);
		if(a1 < a2)
			min2 = a1;
		else
			min2 = a2;

		if(min1 > min2){
			f[0] = i1; f[1] = i2; f[2] = i3;
			t1 = hf_mesh->itsData.pushBackTriangle(f);
			f[0] = i3; f[1] = i4; f[2] = i1;
			t2 = hf_mesh->itsData.pushBackTriangle(f);
		}
		else{
			f[0] = i1; f[1] = i2; f[2] = i4;
			t1 = hf_mesh->itsData.pushBackTriangle(f);
			f[0] = i3; f[1] = i4; f[2] = i2;
			t2 = hf_mesh->itsData.pushBackTriangle(f);
		}
	}
	hf_mesh->itsData.initTNormal();
	for(i=0; i<fN; i++)
	{
		std::vector<double> n;
		n.push_back(-quadMesh->normal_f[i][0]);
		n.push_back(-quadMesh->normal_f[i][1]);
		n.push_back(-quadMesh->normal_f[i][2]);
		hf_mesh->itsData.setTNormal(i*2, n);
		hf_mesh->itsData.setTNormal(i*2+1, n);
	}
	delete quadMesh;
}

double DcGenerator::angle(const std::vector<double>& p0, const std::vector<double>& p1, const std::vector<double>& p2)
{
	double v1[3] = {p1[0]-p0[0], p1[1]-p0[1], p1[2]-p0[2]};
	double v2[3] = {p2[0]-p0[0], p2[1]-p0[1], p2[2]-p0[2]};
	double v3[3] = {p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]};
	double dot1 = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
	double dot2 = -(v1[0]*v3[0] + v1[1]*v3[1] + v1[2]*v3[2]);
	double dot3 = v3[0]*v2[0] + v3[1]*v2[1] + v3[2]*v2[2];
	double dot = dot1;
	if (dot < dot2)
		dot = dot2;
	if (dot < dot3)
		dot = dot3;
	return acos(dot);
}

double DcGenerator::evalFunc(double x, double y, double z, double* s)
{
	double ret;
	p[0] = x;
	p[1] = y;
	p[2] = z;
	ret = func->calc(p, s) - iso;
	return ret;
}

//Central diffrence
void DcGenerator::nablaFuncC(std::vector<double>& g, double x, double y, double z)
{
	g.clear();
	g.push_back(0.5f*(evalFunc(x+h, y, z, pTmpS) - evalFunc(x-h, y, z, pTmpS))/h);
	g.push_back(0.5f*(evalFunc(x, y+h, z, pTmpS) - evalFunc(x, y-h, z, pTmpS))/h);
	g.push_back(0.5f*(evalFunc(x, y, z+h, pTmpS) - evalFunc(x, y, z-h, pTmpS))/h);
}

//Forward diffrence (returns also f(x,y,z))
double DcGenerator::nablaFuncF(std::vector<double>& g, double x, double y, double z){
	double f = evalFunc(x, y, z, pTmpS);
	g.push_back((evalFunc(x+h, y, z, pTmpS) - f)/h);
	g.push_back((evalFunc(x, y+h, z, pTmpS) - f)/h);
	g.push_back((evalFunc(x, y, z+h, pTmpS) - f)/h);
	return f;
}

//Find root between (x1, y1, z1) and (x2, y2, z2)
double DcGenerator::convergeToZero(std::vector<double>& p, double x1, double y1, double z1, double v1, 
								   double x2, double y2, double z2, double v2)
{
	double x, y, z, v;
	int i;
	for (i = 0; i < IMAX; i++){
		double w1 = fabs(v2);
		double w2 = fabs(v1);
		double w = w1 + w2;
		if (w == 0){
			x = x1;
			y = y1;
			z = z1;
			v = 0;
			break;
		}
		w1 /= w;
		w2 /= w;

		x = w1*x1 + w2*x2;
		y = w1*y1 + w2*y2;
		z = w1*z1 + w2*z2;

		v = evalFunc(x, y, z, pTmpS);

		if (fabs(v) < eps)
			break;
		if(v1*v > 0){
			x1 = x;
			y1 = y;
			z1 = z;
			v1 = v;
		}
		else{
			x2 = x;
			y2 = y;
			z2 = z;
			v2 = v;
		}
	}
	//Change into Bisection
	if (i == IMAX){
		for(int i=0; i<2*IMAX; i++){
			x = 0.5f*(x1 + x2);
			y = 0.5f*(y1 + y2);
			z = 0.5f*(z1 + z2);

			v = evalFunc(x, y, z, pTmpS);

			if(fabs(v) < eps)
				break;
			if(v1*v > 0){
				x1 = x;
				y1 = y;
				z1 = z;
				v1 = v;
			}
			else{
				x2 = x;
				y2 = y;
				z2 = z;
				v2 = v;
			}
		}
	}
	p.clear();
	p.push_back(x);
	p.push_back(y);
	p.push_back(z);

	return v;
}
