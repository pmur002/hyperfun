/*==============================================================================

Copyright 1999 Eric Fausett
Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt
Copyright 2007-2008 Oleg Fryazinov
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



// MainApp.cpp: implementation of the CMainApp class.
//
//////////////////////////////////////////////////////////////////////

#include "mainapp.h"
#ifdef DEBUG
#include "crtdbg.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMainApp::CMainApp(int & argc, char ** argv) 
{
#ifdef DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF; 
	flag |= _CRTDBG_ALLOC_MEM_DF; 
	flag |= _CRTDBG_CHECK_ALWAYS_DF; 
	flag |= _CRTDBG_CHECK_CRT_DF; 
	_CrtSetDbgFlag(flag); // Set flag to the new value
#endif

	itsPolyMesh_ = NULL;
	itsSliser_ = NULL;
	try{
		itsCL.Init(argc, argv);
	}
	catch(CommandLineError err){
		std::cout << err.error << "\n\n";



		std::cout << err.help << "\n\n";
		std::cout << "Exiting..." << "\n\n";
		exit(0);
	}
}


CMainApp::~CMainApp(){
	if(itsPolyMesh_!=NULL)
		delete itsPolyMesh_;
	if (itsSliser_!=NULL)
		delete itsSliser_;
}


int CMainApp::init() {
	std::cout << "Initialization.";
	if(itsCL.itsTimeReport) std::cout << "\n\n";
	if(itsCL.itsTimeReport) itsTimer.start("Time to Read and Parse File");
	readFile();
	if(!itsCL.itsTimeReport) std::cout << ".";
	parseString();
	if(!itsCL.itsTimeReport) std::cout << ".";
	if(itsCL.itsTimeReport) itsTimer.stop();

	if (itsCL.itsCLI) {
		generateSlicingGrid();
		int layers = cliOut();
		cout << "\n" << layers << " layers have been successfully written\n";
		return - layers;
	}
	else
	{
		generateTriangles();

		/* used for vrmlout and povraymeshout */
		getViewPos(Pos_, Look_);
		getBoundingBox(Min_, Max_);

		if (itsCL.itsWRL) {
			vrmlOut();
		}
		if (itsCL.itsSTL) {
			itsCL.itsSTLB = false;
			stlOut();
		}
		if (itsCL.itsSTLB) {
			stlBinaryOut();
		}
		if (itsCL.itsPOV) {
			povrayMeshOut();
		}

		return itsPolyMesh_->TriangleNum();
	}
}


void CMainApp::readFile(){
	// ********** READ FILE *************
	//Read file
	std::ifstream fromFile(itsCL.itsModel.c_str());
	if(!fromFile){
		std::cout << "\n File: " << itsCL.itsModel << " not found\n";
		exit(0);
	}
	//Read file into string
	char ch;
	// int i=0;
	while(fromFile.get(ch))	itsString+=ch;
	ch = '\0';
	itsString+=ch;
	//Check correct transfer
	if(!fromFile.eof()){
		std::cout << "\nFile Read Error.\n";
		exit(0);
	}
	//Close fromFile
	fromFile.close();
	// **********************************
}


void CMainApp::parseString(){

	try{
		itsInterpreter.parse(itsString, itsCL.itsObject);
	}
	//Check for memory error during Parse
	catch(MemoryError me){
		std::cout << "\nMemory Allocation Error on Parse!\nNot Enough Memory to complete Parse!\n";
		std::cout << "Occured at: " << me.error << "\n";
		std::cout << "Exiting..." << "\n";
		exit(0);
	}
	//Check for Errors in File while parsing
	catch(ParseError pe){
		std::cout << "Error: " << pe.error; 
		std::cout << "\n" << "       line = " << pe.line;
		std::cout << " , position = " << pe.pos << "\n" << "\n";
		std::cout << "Exiting..." << "\n";
		exit(0);
	}


	//Set itsObject
	//itsInterpreter.object(itsCL.itsObject);


}


void CMainApp::generateTriangles(){
	// ********** Generate Triangles *************
	if (itsCL.itsUseDC)
	{
		DcGenerator gen;
		gen.setGridN(itsCL.itsGridSize[0], itsCL.itsGridSize[1], itsCL.itsGridSize[2]);
		gen.setBox(itsCL.itsBBMinMax[0], itsCL.itsBBMinMax[3], 
			itsCL.itsBBMinMax[1], itsCL.itsBBMinMax[4], 
			itsCL.itsBBMinMax[2], itsCL.itsBBMinMax[5]);
		gen.setFunc(&itsInterpreter, itsCL.itsIsoValue);
		gen.setParameters(itsCL.itsAConstants);
		itsPolyMesh_ = gen.generate();
		return;
	}

	//not use dual contouring, use as standart
	try{
		itsPolyMesh_ = new HFPolyMesh(itsInterpreter);
	}
	catch(HFPError he){
		std::cout << "Error: " << he.error << "\n";
		std::cout << "Exiting..." << "\n";
		exit(0);
	}

	itsPolyMesh_->Timer(itsCL.itsTimeReport);
	itsPolyMesh_->IsoValue(itsCL.itsIsoValue);
	itsPolyMesh_->MinMax(itsCL.itsBBMinMax);
	itsPolyMesh_->Grid(itsCL.itsGridSize);
	itsPolyMesh_->DimMap(itsCL.itsXYZMapping);
	itsPolyMesh_->Constants(itsCL.itsXConstants);
	itsPolyMesh_->Parameters(itsCL.itsAConstants);
	itsPolyMesh_->Normals(itsCL.itsNormals);
	itsPolyMesh_->Search(itsCL.itsSearch);
	itsPolyMesh_->SearchPercent(itsCL.itsSearchPer);
	itsPolyMesh_->Reduce(itsCL.itsReduce);
	itsPolyMesh_->Refine(itsCL.itsAttributes);

	itsPolyMesh_->Calc();
}

void CMainApp::generateSlicingGrid() {
	DcGenerator gen;
	gen.setFunc(&itsInterpreter, itsCL.itsIsoValue);
	gen.setParameters(itsCL.itsAConstants);
	itsSliser_ = new HFSlicer(itsInterpreter, itsCL.itsBBMinMax, itsCL.itsSlicingGridStep);
};

void CMainApp::getViewPos(double pos[],double  look[]){
	double NMX=itsCL.itsBBMinMax[3]-itsCL.itsBBMinMax[0];
	double NMY=itsCL.itsBBMinMax[4]-itsCL.itsBBMinMax[1];
	double NMZ=itsCL.itsBBMinMax[5]-itsCL.itsBBMinMax[2];
	double NM=NMX;
	if(NMY>NM) NM=NMY;
	if(NMZ>NM) NM=NMZ;
	NM/=2;

	pos[0]=0.0;
	pos[1]=0.0;
	pos[2]=NM*5;
	look[0]=(itsCL.itsBBMinMax[3]+itsCL.itsBBMinMax[0])/2.0;
	look[1]=(itsCL.itsBBMinMax[3]+itsCL.itsBBMinMax[0])/2.0;
	look[2]=(itsCL.itsBBMinMax[3]+itsCL.itsBBMinMax[0])/2.0;

}



int CMainApp::getWindowSizeX()
{
	return itsCL.itsWindowSize[0];
}

int CMainApp::getWindowSizeY()
{
	return itsCL.itsWindowSize[1];
}


int CMainApp::getFaceLineFill(){
	/* No offset yet in the options of the command line....*/
	return itsCL.itsWireframe;
}


int CMainApp::getNormal(){
	/* No offset yet in the options of the command line....*/
	return itsCL.itsNormals;
}


int CMainApp::getNormalDisplay(){
	/* No offset yet in the options of the command line....*/
	return itsCL.itsNormalsDisplay;
}

int CMainApp::getFlat(){
	return itsCL.itsFlat;
}

int CMainApp::getLightType(){
	int type = 0;
	if(itsCL.itsDiffuse && !itsCL.itsSpecular) type = 1;
	if(itsCL.itsSpecular && !itsCL.itsDiffuse) type = 2;
	if(itsCL.itsSpecular && itsCL.itsDiffuse) type = 3;
	return type;
}


void CMainApp::getFaceColor(float *fc){
	fc[0] = (float)itsCL.itsFaceColor[0];
	fc[1] = (float)itsCL.itsFaceColor[1];
	fc[2] = (float)itsCL.itsFaceColor[2]; 
}

void CMainApp::getLineColor(float *fc){
	fc[0] = (float)itsCL.itsLineColor[0];
	fc[1] = (float)itsCL.itsLineColor[1];
	fc[2] = (float)itsCL.itsLineColor[2]; 
}



void CMainApp::getBoundingBox(float *min,float *max){
	min[0] = (float)itsCL.itsBBMinMax[0];
	min[1] = (float)itsCL.itsBBMinMax[1];
	min[2] = (float)itsCL.itsBBMinMax[2]; 

	max[0] = (float)itsCL.itsBBMinMax[3];
	max[1] = (float)itsCL.itsBBMinMax[4];
	max[2] = (float)itsCL.itsBBMinMax[5]; 
}


int CMainApp::getNormalSegments(double *** normal){
	int t = itsPolyMesh_->VertexNum();
	double NMX=itsCL.itsBBMinMax[3]-itsCL.itsBBMinMax[0];
	double NMY=itsCL.itsBBMinMax[4]-itsCL.itsBBMinMax[1];
	double NMZ=itsCL.itsBBMinMax[5]-itsCL.itsBBMinMax[2];
	double NM=NMX;
	if(NMY>NM) NM=NMY;
	if(NMZ>NM) NM=NMZ;
	NM/=20;
	(*normal)=(double **)malloc(2*t*sizeof(double*));
	for(int C=0; C<t; C++){
		(*normal)[(2*C)]=(double *)malloc(3*sizeof(double));
		(*normal)[(2*C)+1]=(double *)malloc(3*sizeof(double));
		for(int i=0;i<3;i++){
			(*normal)[(2*C)][i]=itsPolyMesh_->itsData.getVertex(C, i);
			(*normal)[(2*C)+1][i]=itsPolyMesh_->itsData.getVertex(C, i)+(itsPolyMesh_->itsData.getVNormal(C, i)*NM);
		}
	}
	return t;
}


int CMainApp::getWireAndSurface()
{
	return itsCL.itsWireAndSurface;  
}




double CMainApp::GetVertexFromTriangle(int i,int j ,int k){

	int index = itsPolyMesh_->itsData.getTriangle(i, j);
	return itsPolyMesh_->itsData.getVertex(index, k);

}

double CMainApp::GetNormalFromTriangle(int i,int j ,int k){

	int index = itsPolyMesh_->itsData.getTriangle(i, j);
	return itsPolyMesh_->itsData.getVNormal(index, k);

}

double CMainApp::GetAttrFromTriangle(int i,int j ,int k){

	int index = itsPolyMesh_->itsData.getTriangle(i, j);
	return itsPolyMesh_->itsData.getAttributes(index, k);

}

bool CMainApp::getAttribute(){
	return itsCL.itsAttributes;
}

