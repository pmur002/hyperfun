/*==============================================================================

Copyright 1999 Eric Fausett
Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt
Copyright 2007-2008 Oleg Fryazinov

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



// HFPCommandLine.h: interface for the HFPCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER >= 1000
#pragma warning( disable : 4786 )
#endif /* _MSC_VER >= 1000 */

#include <vector>
#include <string>
#include <iostream>

#if defined(__sgi) || defined(__unix) || defined(unix) 
#include <ctype.h>
#else /* neither sgi or unix */
#include <cctype>
#endif /* if defined sgi or unix */

#include "hfpclhelp.h"


#ifndef HFPCOMMANDLINE_H
#define HFPCOMMANDLINE_H

#if _MSC_VER >= 1000
#pragma once
#endif /* _MSC_VER >= 1000 */

struct CommandLineError{ 
	std::string error; 
	std::string help;
	CommandLineError(std::string e, std::string h){ 
		error=e; 
		help=h; 
	}
};

class HFPCommandLine{
public:
	HFPCommandLine();
	virtual ~HFPCommandLine();

	//init can throw CommandLineError
	HFPCommandLine(int& argc, char** argv);
	void Init(int& argc, char** argv); 

	// *********** MODEL **********
	// ARGUMENT FORMAT: string
	// NOTES:	No KEY.  Must be first Argument.(REQUIRED) Specifies file to be read.
	std::string itsModel;

	// *********** OBJECT **********
	// ARGUMENT FORMAT: string
	// NOTES:	Specifies Object in file to polygonize.
	// ARGUMENT KEY: OBJECT "-o"
#define DEF_OBJECT "my_model"
	std::string itsObject;

	// *********** MAPPING **********
	// ARGUMENT FORMAT: Comma delimited double values or the charecters X, Y, or Z. Must have at least 3 arguments.
	// NOTES:	Specifies variable and constant X array values for Object. =0.0 where variable.
	// ARGUMENT KEY: MAPPING "-x"
#define DEF_XMAPPING 0
#define DEF_YMAPPING 1
#define DEF_ZMAPPING 2
#define DEF_XCONSTANT_SIZE 3
#define DEF_XCONSTANT 0.0
	std::vector<int> itsXYZMapping;
	std::vector<double> itsXConstants;

	// *********** PARAMETERS **********
	// ARGUMENT FORMAT: Comma delimited double values. Must have at least 1 argument.
	// NOTES:	Specifies constant A array values for Object.
	// ARGUMENT KEY: ACONSTANTS "-a"
#define DEF_ACONSTANTS_SIZE 1
#define DEF_ACONSTANTS 0.0
	std::vector<double> itsAConstants;

	// *********** TIME REPORT **********
	// ARGUMENT FORMAT: None
	// NOTES:	Turns on the Time Report.
	// ARGUMENT KEY: TIMEREPORT "-t"
#define DEF_TIMEREPORT false
	bool itsTimeReport;

	// *********** GRID SIZE **********
	// ARGUMENT FORMAT: Comma delimited integer values >2 and <250.  Can have 1 or 3 arguments.
	// NOTES:	Specifies size of grid array for polygonization.
	// ARGUMENT KEY: GRIDSIZE "-g"
#define DEF_GRIDSIZE_SIZE 3
#define DEF_GRIDSIZE 30
#define DEF_GRIDSIZE_MIN 2
#define DEF_GRIDSIZE_MAX 250
	std::vector<int> itsGridSize;

	// ***********SLICING GRID SIZE **********
	// ARGUMENT FORMAT: Comma delimited double values >0.001 and <10. Can have 1 or 3 arguments.
	// NOTES:	Specifies step size of grid array for sclicing.
	// ARGUMENT KEY: SGSTEP "-gs"
#define DEF_SGSTEP_SIZE 3
#define DEF_SGSTEP 1
#define DEF_SGSTEP_MIN 0.001
#define DEF_SGSTEP_MAX 10
	std::vector<double> itsSlicingGridStep;

	// *********** BOUNDING BOX **********
	// ARGUMENT FORMAT: Comma delimited double values.  Can have 1, 2, 3, or 6 arguments.
	// NOTES:	Specifies bounding box for polygonization
	// ARGUMENT KEY: BBMINMAX "-b"
#define DEF_BBMIN -10.0
#define DEF_BBMAX 10.0
	std::vector<double> itsBBMinMax;

	// *********** LINE COLOR **********
	// ARGUMENT FORMAT: Comma delimited integer values between 0 and 255.  Can have 1 or 3 arguments.
	// NOTES:	Specifies RGB Color of lines.
	// ARGUMENT KEY: LINECOLOR "-cl"
#define DEF_LINECOLORR 0.0
#define DEF_LINECOLORG 0.0
#define DEF_LINECOLORB 0.0
#define DEF_COLOR_MIN 0
#define DEF_COLOR_MAX 255
	std::vector<double> itsLineColor;

	// *********** FACE COLOR **********
	// ARGUMENT FORMAT: Comma delimited integer values between 0 and 255.  Can have 1 or 3 arguments.
	// NOTES:	Specifies RGB Color of faces.
	// ARGUMENT KEY: FACECOLOR "-cf"
#define DEF_FACECOLORR 0.196078
#define DEF_FACECOLORG 0.784314
#define DEF_FACECOLORB 0.313725
	//Uses DEF_COLOR_MIN from above
	//Uses DEF_COLOR_MAX from above
	std::vector<double> itsFaceColor;

	// *********** ISO VALUE **********
	// ARGUMENT FORMAT: Double value.  Can have 1 argument.
	// NOTES:	Specifies the ISO Value of the polygonized surface.
	// ARGUMENT KEY: ISOVALUE "-i"
#define DEF_ISOVALUE 0.0
	double itsIsoValue;

	// *********** HELP **********
	// ARGUMENT FORMAT: None.
	// NOTES:	Asks for Help Info.
	// ARGUMENT KEY: HELP "-h"

	// *********** WINDOW SIZE **********
	// ARGUMENT FORMAT: Comma delimited integer values.  Can have 1 or 2 arguments.
	// NOTES:	Specifies size of display window in pixels.
	// ARGUMENT KEY: WINDOWSIZE "-w"
#define DEF_WINDOWSIZEX 480
#define DEF_WINDOWSIZEY 320
#define DEF_WINDOWSIZEX_MIN 160
#define DEF_WINDOWSIZEY_MIN 120
#define DEF_WINDOWSIZEX_MAX 1600
#define DEF_WINDOWSIZEY_MAX 1200
	std::vector<int> itsWindowSize;

	// *********** SEARCH **********
	// ARGUMENT FORMAT: Double value. Must have 1 argument.
	// NOTES:	Specifies the Search accuracy in Percentage of cell lengths.  Must be a value between 0.01 and 100.
	// ARGUMENT KEY: SEARCH "-s"
#define DEF_SEARCH false
#define DEF_SEARCHPER 0.1
#define DEF_SEARCHPER_MIN 0.0001
#define DEF_SEARCHPER_MAX 1.0
	bool itsSearch;
	double itsSearchPer;

	// *********** REDUCE POLYGONS **********
	// ARGUMENT FORMAT: None.
	// NOTES:	
	// ARGUMENT KEY: REDUCE "-r"
#define DEF_REDUCE false
	bool itsReduce;

	// *********** VRML OUT **********
	// ARGUMENT FORMAT: String.
	// NOTES:	Specifies vrml output file name.  .wrl extension will be added if not exist.
	// ARGUMENT KEY:  "-wrl"
#define DEF_WRL false
#define DEF_WRLNAME "out.wrl"
	bool itsWRL;
	std::string itsWRLName;


	// *********** STL OUT ***********
	// ARGUMENT FORMAT: STring.
	// NOTES: Specifies stl output file name. .stl extension will be added if it not exist.
	// ARGUMENT KEY: "-stl"
#define DEF_STL false
#define DEF_STLNAME "out.stl"
	bool itsSTL;
	bool itsSTLB;
	std::string itsSTLName;

	// POV  OUT
	// ARGUMENT FORMAT string.
	// NOTES: Specifies the name of the POV file.
#define DEF_POV false
#define DEF_POVNAME "out.pov"
	bool itsPOV;
	std::string itsPOVName;

	// *********** CLI  OUT **********
	// ARGUMENT FORMAT string.
	// NOTES: Specifies the name of the CLI file.
	// ARGUMENT KEY: "-cli"
#define DEF_CLI false
#define DEF_CLINAME "out.cli"
	bool itsCLI;
	std::string itsCLIName;

	// *********** DISPLAY **********
	// ARGUMENT FORMAT: None.
	// NOTES:	
	// ARGUMENT KEY:  "-d"
#define DEF_WIREFRAME false
#define DEF_NORMALS true
#define DEF_NORMALSDISPLAY false
#define DEF_NORMALVERTEX true
#define DEF_DIFFUSE true
#define DEF_SPECULAR true
#define DEF_FLAT false
#define DEF_DISPLAY_MODE 5

	enum DISP { W=1, WF=2, ND=3, WND=4, NDS=5, WNDS=6, NIDS=7 };
	bool itsWireframe;
	bool itsNormals;
	bool itsNormalsDisplay;
	bool itsNormalsVertex;
	bool itsDiffuse;
	bool itsSpecular;
	bool itsFlat;
	bool itsWireAndSurface;
	int itsDisplayMode;

	// *********** SHOW BOUNDING BOX **********
	// ARGUMENT FORMAT: None.
	// NOTES: ***** This option is not official. *****
	// ARGUMENT KEY:  "-sb"

#define DEF_SHOWBOUNDINGBOX false

	bool itsShowBoundingBox;

	// *********** SHOW 3D AXES **********
	// ARGUMENT FORMAT: None.
	// NOTES: ***** This option is not official. *****
	// ARGUMENT KEY:  "-sa"

#define DEF_SHOW3DAXES false

	bool itsShow3DAxes;

	// *********** Use Dual Contouring **********
	// ARGUMENT FORMAT: None.
	// NOTES: ***** This option is not official. *****
	// ARGUMENT KEY:  "-usedc"

#define DEF_USEDC true

	bool itsUseDC;

	/****ADDED FOR ATTRIBUTES*/
	// *********** ATTRIBUTES **********
	// ARGUMENT FORMAT: None.
	// NOTES: Set colors to the isosurface using the constructive texturing (ct) method.  	
	// ARGUMENT KEY:  "-ct"

#define DEF_ATTRIBUTES false
	bool itsAttributes;


private:
	//NOTE: enum ARG order and VALIDARGS must match!!
	enum ARG {Object=0, Mapping, Parameters, TimeReport, GridSize, SlicingGridStep,
		BoundingBox, LineColor, FaceColor, IsoValue, Help, WindowSize, Search, Reduce, STLOut, STLBOut, VRMLOut,POVOut, CLIOut, Display, ShowBoundingBox, Show3DAxes,  Attributes, UseDC, ERR};
	// *********** Comprehensive List of Current Valid Arguments **********
#define VALIDARGS "-o,-x,-a,-t,-g,-gs,-b,-cl,-cf,-i,-h,-w,-s,-r,-stl,-stlb,-wrl,-pov,-cli,-d,-sb,-sa,-ct,-usedc"


	void defaults();
	void parse();

	ARG getArg(std::string s);

	bool validArg(std::string s);

	std::string helpOut();

	int clampI (int value, int min, int max, std::string sarg);
	double clampD (double value, double min, double max, std::string sarg);

	int fillI(std::string s);
	double fillD(std::string s);

	std::vector<std::string> fillSV(std::string s);		
	std::vector<int> fillIV(std::string s);
	std::vector<double> fillDV(std::string s);

	void parseMapping(std::string s);
	bool checkMapping();
	bool checkBB();
	bool hasArg(std::string key);
	bool hasArg();

	std::vector<std::string> itsArgs;
	std::vector<std::string> itsValidArgArray;
	std::vector<std::string>::iterator itsCurrentArg;
};

#endif // !defined(HFPCOMMANDLINE_H)
