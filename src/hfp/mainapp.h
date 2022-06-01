/*==============================================================================

Copyright 1999 Eric Fausett
Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt
Copyright 2018-2019 Dmitry Popov

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



// MainApp.h: interface for the CMainApp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(MAINAPP_H)
#define MAINAPP_H

#pragma warning( disable : 4786 )


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <iostream>
#include <fstream>
#ifdef WIN32
#include <windows.h>
#endif

#include "hftimer.h"
#include "hfpcommandline.h"
#include "hfpolymesh.h"
#include "hfslicer.h"

//DUAL_CONTOURING
#include "hfdc_dcgenerator.h"

#define MAX_FILE_SIZE 100000
#define PI 3.14159265

class CMainApp 
{
public:
	CMainApp(int & argc, char ** argv);
	virtual ~CMainApp();
	int init ();
	void createObject(double **** triangle,double **** normal);

	void createGLTriangle(bool bNormal, bool bVertexNormal, bool bAttributes);

	double GetVertexFromTriangle(int,int,int);
	double GetNormalFromTriangle(int,int,int);
	double GetAttrFromTriangle(int,int,int);


	bool getAttribute();

	void getViewPos(double pos[],double  look[]);
	int getAttributes(double **** color);
	int getNormalSegments(double ***);
	int getFaceLineFill();
	int getNormal();
	int getNormalDisplay();
	int getLightType();
	int getFlat();
	int getWindowSizeX();
	int getWindowSizeY();
	int getWireAndSurface();

	void getFaceColor(float *);
	void getLineColor(float *);
	void getBoundingBox(float *,float *);

private:
	void readFile();
	void parseString();
	void generateTriangles();
	void generateSlicingGrid();
	void vrmlOut();
	void stlOut();
	void stlBinaryOut();
	void povrayMeshOut();
	int cliOut();

	HFPCommandLine itsCL;
	HFTimer itsTimer;
	std::string itsString;
	HFInterpreter itsInterpreter;
	HFPolyMesh* itsPolyMesh_;
	HFSlicer* itsSliser_;
	



	double Pos_[3];


	double Look_[3];
	float Min_[3];
	float Max_[3];

};


#endif // !defined(MAINAPP_H)
