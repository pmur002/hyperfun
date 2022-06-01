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

// HFPolyMesh.h: interface for the HFPolyMesh class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(HFPolyMesh_H)
#define HFPolyMesh_H

#pragma warning( disable : 4786 )

#include <vector>
#include <iostream>
#include <map>
#include <string>

#include <cmath>  

#include "hfinterpreter.h"
#include "hftimer.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define F1_0 0			// 0000000000000000
#define F1_1 1			// 0000000000000001
#define F1_2 2			// 0000000000000010
#define F1_4 3			// 0000000000000011

#define F2_0 0			// 0000000000000000
#define F2_1 4			// 0000000000000100
#define F2_2 8			// 0000000000001000
#define F2_4 12			// 0000000000001100

#define F3_0 0			// 0000000000000000
#define F3_1 16			// 0000000000010000
#define F3_2 32			// 0000000000100000
#define F3_4 48			// 0000000000110000

#define F4_0 0			// 0000000000000000
#define F4_1 64			// 0000000001000000
#define F4_2 128		// 0000000010000000
#define F4_4 192		// 0000000011000000

#define F5_0 0			// 0000000000000000
#define F5_1 256		// 0000000100000000
#define F5_2 512		// 0000001000000000
#define F5_4 768		// 0000001100000000

#define F6_0 0			// 0000000000000000
#define F6_1 1024		// 0000010000000000
#define F6_2 2048		// 0000100000000000
#define F6_4 3072		// 0000110000000000

#define NONE 0			// 0000000000000000
#define One	1			// 0000000000000001
#define TWO 2			// 0000000000000010
#define FOUR 3			// 0000000000000011

const int Face2Vert[6][4]={ {0,1,2,3}, {0,9,4,8}, {3,11,7,8}, {4,5,6,7}, {2,10,6,11}, {1,10,5,9} };
//{{0,1,2,3}, {3,11,7,8}, {4,5,6,7}, {1,10,5,9}, {0,9,4,8}, {2,10,6,11}};

const std::vector<double> EMPTYVD(0);
const std::vector<unsigned int> EMPTYVI(0);

#define EPS (double)0.00001
#define NORMDELTMULT (double)0.001
//#define pol_eps	0.001

struct HFPError{ std::string error;
HFPError(std::string e){ error=e; }
};

class HFPolyMesh{
public:
	//**********   PUBLIC METHODS   **********
	//HFPolyMesh can throw HFPError
	HFPolyMesh(HFInterpreter& interp);
	virtual ~HFPolyMesh();

	void Calc();

	int VertexNum();
	int TriangleNum();

	void Timer(bool t);
	bool Timer();

	void Normals(bool s);
	bool Normals();

	void SearchPercent(double p);
	double SearchPercent();
	void Search(bool s);
	bool Search();

	void Reduce(bool r);
	bool Reduce();

	void IsoValue(double isov);
	double IsoValue();

	void MinMax(std::vector<double> mm);
	std::vector<double> MinMax();

	std::vector<double> Delta();

	void Grid(std::vector<int> g);
	std::vector<int> Grid();

	void DimMap(std::vector<int> dm);
	std::vector<int> DimMap();

	void Constants(std::vector<double> con);
	std::vector<double> Constants();

	void Parameters(std::vector<double> par);
	std::vector<double> Parameters();

	void Refine(bool);
	bool Refine();

	void GeneralRefinement();
	void CalcVNormal(double x, double y, double z, double& nx, double& ny, double& nz);

	class TriData{
	public:
		TriData();
		~TriData();
		//Initilization 

		/*********ADDED FOR ATTRIBUTES*/
		/*----Added for refinement---*/
		void ReplaceVertex(int, std::vector<double>,double,double *);
		void setAttributes(int, double *);

		//void guessSize(int tNum, int );
		void guessSize(int tNum, int VNum,int );
		//Data Creation
		int pushBackVertex(double* x, double fvalue,double *S);
		int pushBackTriangle(int v[3]);
		//Data Modification
		void initVNormal();
		void initTNormal();

		/**Modified by schmitt :03 May 2001
		/**Call inside the hfpolymesh class. 
		/**No need to use a double *, and copy it into a vector data type*/
		void setVNormal(int , std::vector<double>);
		void setTNormal(int , std::vector<double>);
		std::vector<double> calcTNormal(int);

		void setFunctVal(int v, double fvalue);
		void deleteEdge(int e);
		void refresh();
		void ccwTriangles(); //Requires Normals
		//Data Retrival
		int getNumV();
		int getNumT();
		std::vector<double> getVertex(int v);
		double getVertex(int v, int x);
		std::vector<double> getVNormal(int v);
		double getVNormal(int v, int x);
		double getTNormal(int v, int x);
		double getFunctVal(int v);
		std::vector<int> getTriangle(int t);
		int getTriangle(int t, int vnum);//vnum is 0, 1, or 2 only

		/*****ADDED FOR ATTRIBUTES***********/
		std::vector<double> getAttributes(int v);
		double getAttributes(int v, int x);
		double getVal(int);

	private:
		//Vertex Data
		std::vector<std::vector<double> > itsVCoordinates;//Size 3
		std::vector<std::vector<int> > itsVEdges;//Size Variable
		std::vector<bool> itsVBad;
		std::vector<double> itsVFVal;
		std::vector<std::vector<double> > itsVNormal;//Size 3
		//Edge Data
		std::vector<std::vector<int> > itsEVertices;//Size 2
		std::vector<std::vector<int> > itsETriangles;//Size 2
		std::vector<bool> itsEBad;
		std::vector<double> itsELength;
		//Triangle Data
		std::vector<std::vector<int> > itsTEdges;//Size 3
		std::vector<std::vector<double> > itsTNormal; //Size 3
		std::vector<bool> itsTBad;
		std::vector<bool> itsTRev;

		/*****ADDED FOR ATTRIBUTES */
		//Attributes Data
		std::vector<std::vector<double> > itsSAttributes;//Size itsSSize
		int itsSSize;

		int itsVBadCount;
		int itsEBadCount;
		int itsTBadCount;

		std::vector<int> itsOpenEdges;

		template <typename T>
		inline void fillVector(const T* d, int size, std::vector<T>& ret) const;
		double edgeLength(int v1, int v2);
		void refreshTriangles();
		void refreshEdges();
		void refreshVerticies();
	};

	TriData itsData;



private:
	//**********   PRIVATE METHODS   **********
	void TimerStart(std::string tString);
	void TimerStop();
	void FillMatrix();
	void FillVertices();
	void CreateTriangles();

	void CreateVNormals();
	void CreateTNormals();

	/* ADDED FOR ATTRIBUTES REFINEMENT*/		
	void Refinement();
	int FindPositiveValue1(std::vector<double> *, std::vector<double>, std::vector<double>,std::vector<double> );
	int FindPositiveValue2(std::vector<double> *vertex,std::vector<double> step,std::vector< std::vector<double> > cube,std::vector<double>);
	void CalcVNormal(int i,std::vector<double> *norm);

	/***************************************/

	double VertEst(const double& left, const double& right, const double& delta);
	double EdgeSearch(int leftI, int leftJ, int leftK, const int& select, const double lp[3], double* centerPos);

	double CalcVal(const double pos[3]);

	void VertCalc(int i, int j, int k, int select, const double lNodePos[3]);

	void StoreCell(int i, int j, int k, int select);

	void GraphBuildCase2(const int* Edge, int* I[12]);
	void GraphBuildCase4(double V,double A1, double A2, const int* Edge);

	void TraceGraph(int* I[12]);

	void FillIndex(int i, int j, int k, int* I[12]);

	void AddFace1ToCell(int li, int lj, int lk);
	void AddFace2ToCell(int li, int lj, int lk);
	void AddFace3ToCell(int li, int lj, int lk);
	void AddFace4ToCell(int li, int lj, int lk);
	void AddFace5ToCell(int li, int lj, int lk);
	void AddFace6ToCell(int li, int lj, int lk);

	//**********   PRIVATE MEMBERS   **********
	bool itsSearch;			
	double itsSearchPer;

	bool itsReduce;

	bool itsNormals;
	bool itsNormalDisplay;

	bool itsTimer;
	HFInterpreter& itsInterpreter;
	double itsS0;					//*** Iso Value for Surface ***
	short itsXMap, itsYMap, itsZMap;//*** Its X,Y,Z Mapping
	double itsBBMax[3], itsBBMin[3];//*** Bounding Box Definition ***
	int itsGridSize[3];				//*** Grid Size Definition ***
	std::vector<double> itsSendingX;
	HFTimer itsT;
	double itsEPS;					//*** Epsilon Value ***
	double itsDelta[3];				//*** Delta Values ***

	std::vector<double> itsNormDelt;

	double itsMinEdgeLen;
	int itsFirstTriangleInSlice;
	std::vector < std::vector < std::vector < int > > > DupVert;

	std::vector< std::vector < std::vector <double> > > itsValGrid;		//*** 3D Grid Holding Function Value Points ***
	std::vector< std::vector < std::vector <bool> > > itsBoolGrid;			//*** 3D Grid Holding Boolean for Inside/Outside Description of Grid Points ***
	std::vector< std::vector< std::vector < std::vector <int> > > > itsVertGrid;//*** 3D Grids Holding Integer Vertex indeces ***
	std::vector< std::vector < std::vector <int> > > itsCellGrid;			//*** 3D Grid Holding bitmap Style Info about Cell Faces ***
	std::vector< std::vector <int> > itsConnect;						//*** Conectivity Graph ***

	/*****ADDED FOR ATTRIBUTES */
	double * itsSAttributesM;
	int itsSSize;
	bool itsRefinement;


};

#endif // !defined(HFPolyMesh_H)
