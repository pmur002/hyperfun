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



// HFPCommandLine.cpp: implementation of the HFPCommandLine class.
//
//////////////////////////////////////////////////////////////////////

#include "hfpcommandline.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HFPCommandLine::HFPCommandLine(){
}


HFPCommandLine::~HFPCommandLine(){
}


HFPCommandLine::HFPCommandLine(int& argc, char** argv){
  Init(argc, argv);	
}


void HFPCommandLine::Init(int& argc, char** argv){
  for(int i=1; i<argc; i++){
    itsArgs.push_back(argv[i]);
  }
  defaults();
  parse();
}


void HFPCommandLine::defaults(){
  itsObject=DEF_OBJECT;
  
  itsXYZMapping.push_back(DEF_XMAPPING);
  itsXYZMapping.push_back(DEF_YMAPPING);
  itsXYZMapping.push_back(DEF_ZMAPPING);
  itsXConstants.resize(DEF_XCONSTANT_SIZE, DEF_XCONSTANT);
  
  itsAConstants.resize(DEF_ACONSTANTS_SIZE, DEF_ACONSTANTS);
  
  itsTimeReport=DEF_TIMEREPORT;
  
  itsGridSize.resize(DEF_GRIDSIZE_SIZE, DEF_GRIDSIZE);

  itsSlicingGridStep.resize(DEF_SGSTEP_SIZE, DEF_SGSTEP);
  
  itsBBMinMax.push_back(DEF_BBMIN);
  itsBBMinMax.push_back(DEF_BBMIN);
  itsBBMinMax.push_back(DEF_BBMIN);
  itsBBMinMax.push_back(DEF_BBMAX);
  itsBBMinMax.push_back(DEF_BBMAX);
  itsBBMinMax.push_back(DEF_BBMAX);
  
  itsLineColor.push_back(DEF_LINECOLORR);
  itsLineColor.push_back(DEF_LINECOLORG);
  itsLineColor.push_back(DEF_LINECOLORB);
  
  itsFaceColor.push_back(DEF_FACECOLORR);
  itsFaceColor.push_back(DEF_FACECOLORG);
  itsFaceColor.push_back(DEF_FACECOLORB);
  
  itsIsoValue=DEF_ISOVALUE;
  
  itsWindowSize.push_back(DEF_WINDOWSIZEX);
  itsWindowSize.push_back(DEF_WINDOWSIZEY);
  
  itsSearch=DEF_SEARCH;
  itsSearchPer=DEF_SEARCHPER;
  
  itsReduce=DEF_REDUCE;
  

  itsWRL=DEF_WRL;
  itsWRLName=DEF_WRLNAME;

  itsSTL=DEF_STL;
  itsSTLB=DEF_STL;
  itsSTLName=DEF_STLNAME;

  itsPOV=DEF_POV;
  itsPOVName=DEF_POVNAME;

  itsCLI=DEF_CLI;
  itsCLIName=DEF_CLINAME;

  itsWireframe=DEF_WIREFRAME;
  itsNormals=DEF_NORMALS;
  itsNormalsDisplay=DEF_NORMALSDISPLAY;
  itsNormalsVertex=DEF_NORMALVERTEX;
  itsDiffuse=DEF_DIFFUSE;
  itsSpecular=DEF_SPECULAR;
  itsFlat=DEF_FLAT;
  itsDisplayMode = DEF_DISPLAY_MODE;
  itsWireAndSurface = false;
  
  itsShowBoundingBox = DEF_SHOWBOUNDINGBOX;
  itsShow3DAxes = DEF_SHOW3DAXES;
  itsUseDC = DEF_USEDC;

  /*ADDED FOR ATTRIBUTES*/
  itsAttributes = DEF_ATTRIBUTES;
}


void HFPCommandLine::parse(){
  itsCurrentArg=itsArgs.begin();
  itsValidArgArray=fillSV(VALIDARGS);
  
  //Check for no arguments
  if(itsCurrentArg==itsArgs.end()) 
    throw CommandLineError("No arguments specified.\nCheck Usage!\n\n", helpOut());
  
  //Check for help query
  if(itsArgs[0]=="-h") 
    throw CommandLineError("Usage:\n", helpOut());
  
  //Check for no Model as first argument
  if(validArg(*itsCurrentArg)) 
    throw CommandLineError("No Model specified.\nCheck Usage!\n\n", helpOut());
  
  //Set Model
  itsModel=*itsCurrentArg;
  itsCurrentArg++;
  
  //Parse Loop
  for(;itsCurrentArg!=itsArgs.end();){
    ARG argSwitch=getArg(*itsCurrentArg);
    itsCurrentArg++;
    switch(argSwitch){
      /* Object */
    case Object:
      if(hasArg("-o")){
	itsObject=*itsCurrentArg;
	itsCurrentArg++;
      }
      break;
      
      /* Mapping */
    case Mapping:
      if(hasArg("-x")){
	parseMapping(*itsCurrentArg);
	if(!checkMapping()){
	  itsXYZMapping[0]=DEF_XMAPPING;
	  itsXYZMapping[1]=DEF_YMAPPING;
	  itsXYZMapping[2]=DEF_ZMAPPING;
	  itsXConstants.clear();
	  itsXConstants.resize(DEF_XCONSTANT_SIZE, DEF_XCONSTANT);
	  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-x>.\n";
	  cout << "Using default value.\n";
	}
	itsCurrentArg++;
      }
      break;
      
      /* Parameters */
    case Parameters:
      if(hasArg("-a")){
	itsAConstants=fillDV(*itsCurrentArg);
	itsCurrentArg++;
      }				
      break;
      
      /* TimeReport */
    case TimeReport:
      itsTimeReport=true;				
      break;
      
      /* GridSize */
    case GridSize:
      if(hasArg("-g")){
		itsGridSize=fillIV(*itsCurrentArg);
		switch(itsGridSize.size()){
			case 1:
			  itsGridSize[0]=clampI(itsGridSize[0], DEF_GRIDSIZE_MIN, DEF_GRIDSIZE_MAX, "-g");
			  itsGridSize.resize(DEF_GRIDSIZE_SIZE, itsGridSize[0]);
			  break;
			case 3:
			  itsGridSize[0]=clampI(itsGridSize[0], DEF_GRIDSIZE_MIN, DEF_GRIDSIZE_MAX, "-g");
			  itsGridSize[1]=clampI(itsGridSize[1], DEF_GRIDSIZE_MIN, DEF_GRIDSIZE_MAX, "-g");
			  itsGridSize[2]=clampI(itsGridSize[2], DEF_GRIDSIZE_MIN, DEF_GRIDSIZE_MAX, "-g");
			  break;
			default:
			  itsGridSize.clear();
			  itsGridSize.resize(DEF_GRIDSIZE_SIZE, DEF_GRIDSIZE);
			  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-g>.\n";
			  cout << "Using default values.\n";
			  break;
		}
		itsCurrentArg++;
      }
      break;

	  /* GridSize */
	case SlicingGridStep:
		if (hasArg("-gs")) {
			itsSlicingGridStep = fillDV(*itsCurrentArg);
			switch (itsSlicingGridStep.size()) {
			case 1:
				itsSlicingGridStep[0] = clampD(itsSlicingGridStep[0], DEF_SGSTEP_MIN, DEF_SGSTEP_MAX, "-gs");
				itsSlicingGridStep.resize(DEF_SGSTEP_SIZE, itsSlicingGridStep[0]);
				break;
			case 2:
				itsSlicingGridStep[0] = clampD(itsSlicingGridStep[0], DEF_SGSTEP_MIN, DEF_SGSTEP_MAX, "-gs");
				itsSlicingGridStep.push_back(clampD(itsSlicingGridStep[1], DEF_SGSTEP_MIN, DEF_SGSTEP_MAX, "-gs"));
				itsSlicingGridStep[1] = clampD(itsSlicingGridStep[0], DEF_SGSTEP_MIN, DEF_SGSTEP_MAX, "-gs");
				break;
			case 3:
				itsSlicingGridStep[0] = clampD(itsSlicingGridStep[0], DEF_SGSTEP_MIN, DEF_SGSTEP_MAX, "-gs");
				itsSlicingGridStep[1] = clampD(itsSlicingGridStep[1], DEF_SGSTEP_MIN, DEF_SGSTEP_MAX, "-gs");
				itsSlicingGridStep[2] = clampD(itsSlicingGridStep[2], DEF_SGSTEP_MIN, DEF_SGSTEP_MAX, "-gs");
				break;
			default:
				itsSlicingGridStep.clear();
				itsSlicingGridStep.resize(DEF_SGSTEP_SIZE, DEF_SGSTEP);
				cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-gs>.\n";
				cout << "Using default values.\n";
				break;
			}
			itsCurrentArg++;
		}
		break;
      
      /* BoundingBox */
    case BoundingBox:
      if(hasArg("-b")){
	itsBBMinMax=fillDV(*itsCurrentArg);
	switch(itsBBMinMax.size()){
	case 1:
	  if(itsBBMinMax[0]>0.0) 
	    itsBBMinMax[0]=-itsBBMinMax[0];
	  itsBBMinMax.resize(3, itsBBMinMax[0]);
	  itsBBMinMax.resize(6, -itsBBMinMax[0]);
	  break;
	case 2:
	  if(itsBBMinMax[0]>itsBBMinMax[1]){
	    double dc=itsBBMinMax[0];
	    itsBBMinMax[0]=itsBBMinMax[1];
	    itsBBMinMax[1]=dc;
	  }
	  itsBBMinMax.resize(6);
	  itsBBMinMax[3]=itsBBMinMax[4]=itsBBMinMax[5]=itsBBMinMax[1];
	  itsBBMinMax[1]=itsBBMinMax[2]=itsBBMinMax[0];							
	  break;
	case 3:
	  if(itsBBMinMax[0]>0.0) itsBBMinMax[0]=-itsBBMinMax[0];
	  if(itsBBMinMax[1]>0.0) itsBBMinMax[1]=-itsBBMinMax[1];
	  if(itsBBMinMax[2]>0.0) itsBBMinMax[2]=-itsBBMinMax[2];
	  itsBBMinMax.resize(6);
	  itsBBMinMax[5]=-itsBBMinMax[2];
	  itsBBMinMax[4]=-itsBBMinMax[1];
	  itsBBMinMax[3]=-itsBBMinMax[0];
	  break;
	case 6:							
	  break;
	default:
	  itsBBMinMax.clear();
	  itsBBMinMax.push_back(DEF_BBMIN);
	  itsBBMinMax.push_back(DEF_BBMIN);
	  itsBBMinMax.push_back(DEF_BBMIN);
	  itsBBMinMax.push_back(DEF_BBMAX);
	  itsBBMinMax.push_back(DEF_BBMAX);
	  itsBBMinMax.push_back(DEF_BBMAX);
	  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-b>.\n";
	  cout << "Using default values.\n";
	  break;
	}
	if(!checkBB()){
	  itsBBMinMax.clear();
	  itsBBMinMax.push_back(DEF_BBMIN);
	  itsBBMinMax.push_back(DEF_BBMIN);
	  itsBBMinMax.push_back(DEF_BBMIN);
	  itsBBMinMax.push_back(DEF_BBMAX);
	  itsBBMinMax.push_back(DEF_BBMAX);
	  itsBBMinMax.push_back(DEF_BBMAX);
	  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-b>.\n";
	  cout << "Using default values.\n";
	}					
	itsCurrentArg++;
      }
      
      
      break;
      
      /* LineColor */
    case LineColor:
      if(hasArg("-cl")){
	int divLC;
	divLC=DEF_COLOR_MAX-DEF_COLOR_MIN;
	vector<int> tmpIV1=fillIV(*itsCurrentArg);
	switch(tmpIV1.size()){
	case 1:
	  tmpIV1[0]=clampI(tmpIV1[0], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cl");
	  itsLineColor[0]=itsLineColor[1]=itsLineColor[2]=(double)tmpIV1[0]/divLC;
	  break;
	case 3:
	  tmpIV1[0]=clampI(tmpIV1[0], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cl");
	  tmpIV1[1]=clampI(tmpIV1[1], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cl");
	  tmpIV1[2]=clampI(tmpIV1[2], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cl");
	  itsLineColor[0]=(double)tmpIV1[0]/divLC;
	  itsLineColor[1]=(double)tmpIV1[1]/divLC;
	  itsLineColor[2]=(double)tmpIV1[2]/divLC;							
	  break;
	default:
	  itsLineColor.clear();
	  itsLineColor.push_back(DEF_LINECOLORR);
	  itsLineColor.push_back(DEF_LINECOLORG);
	  itsLineColor.push_back(DEF_LINECOLORB);
	  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-cl>.\n";
	  cout << "Using default values.\n";
	  break;
	}
	itsCurrentArg++;
      }				
      break;
      
      /* FaceColor */
    case FaceColor:
      if(hasArg("-cf")){
	int divFC;
	divFC=DEF_COLOR_MAX-DEF_COLOR_MIN;
	vector<int> tmpIV2=fillIV(*itsCurrentArg);
	switch(tmpIV2.size()){
	case 1:
	  tmpIV2[0]=clampI(tmpIV2[0], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cf");
	  itsFaceColor[0]=itsFaceColor[1]=itsFaceColor[2]=(double)tmpIV2[0]/divFC;
	  break;
	case 3:
	  tmpIV2[0]=clampI(tmpIV2[0], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cf");
	  tmpIV2[1]=clampI(tmpIV2[1], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cf");
	  tmpIV2[2]=clampI(tmpIV2[2], DEF_COLOR_MIN, DEF_COLOR_MAX, "-cf");
	  itsFaceColor[0]=(double)tmpIV2[0]/divFC;
	  itsFaceColor[1]=(double)tmpIV2[1]/divFC;
	  itsFaceColor[2]=(double)tmpIV2[2]/divFC;							
	  break;
	default:
	  itsFaceColor.clear();
	  itsFaceColor.push_back(DEF_FACECOLORR);
	  itsFaceColor.push_back(DEF_FACECOLORG);
	  itsFaceColor.push_back(DEF_FACECOLORB);
	  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-cf>.\n";
	  cout << "Using default values.\n";
	  break;
	}
	itsCurrentArg++;
      }				
      break;
      
      /* IsoValue */
    case IsoValue:
      if(hasArg("-i")){
	itsIsoValue=fillD(*itsCurrentArg);
	itsCurrentArg++;
      }
      break;
      
      /* Help */
    case Help:
      cout << helpOut();				
      break;
      
      /* WindowSize */
    case WindowSize:
      if(hasArg("-w")){
	itsWindowSize=fillIV(*itsCurrentArg);
	switch(itsWindowSize.size()){
	case 1:
	  itsWindowSize.resize(2, itsWindowSize[0]);
	  itsWindowSize[0]=clampI(itsWindowSize[0], DEF_WINDOWSIZEX_MIN, DEF_WINDOWSIZEX_MAX, "-w");
	  itsWindowSize[1]=clampI(itsWindowSize[1], DEF_WINDOWSIZEY_MIN, DEF_WINDOWSIZEY_MAX, "-w");
	  break;
	case 2:
	  itsWindowSize[0]=clampI(itsWindowSize[0], DEF_WINDOWSIZEX_MIN, DEF_WINDOWSIZEX_MAX, "-w");
	  itsWindowSize[1]=clampI(itsWindowSize[1], DEF_WINDOWSIZEY_MIN, DEF_WINDOWSIZEY_MAX, "-w");
	  break;
	  
	default:
	  itsWindowSize.clear();
	  itsWindowSize.push_back(DEF_WINDOWSIZEX);
	  itsWindowSize.push_back(DEF_WINDOWSIZEY);
	  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-w>.\n";
	  cout << "Using default values.\n";
	  break;
	}
	itsCurrentArg++;
      }				
      break;
      
      /* Search */
    case Search:
      itsSearch=true;
      if(hasArg()){
	itsSearchPer=fillD(*itsCurrentArg);
	itsSearchPer=clampD(itsSearchPer, DEF_SEARCHPER_MIN, DEF_SEARCHPER_MAX, "-s");
	if(itsSearchPer==1.0) itsSearch=false; //If 1 then no point in doing search
	itsCurrentArg++;
      }
      break;
      
      /* Reduce */
    case Reduce:
      itsReduce=true;
      break;
      
      /* Object */
    case VRMLOut:
      itsWRL=true;
      if(hasArg("-wrl")){
	itsWRLName=*itsCurrentArg;
	itsCurrentArg++;
      }
      break;

    case STLOut:
      itsSTL=true;
      if(hasArg("-stl")){
	itsSTLName=*itsCurrentArg;
	itsCurrentArg++;
      }
      break;

	case STLBOut:
      itsSTLB=true;
      if(hasArg("-stlb")){
	itsSTLName=*itsCurrentArg;
	itsCurrentArg++;
      }
      break;

     case POVOut:
      itsPOV=true;
      if(hasArg("-pov")){
	itsPOVName=*itsCurrentArg;
	itsCurrentArg++;
      }
      break;

	 case CLIOut:
		itsCLI = true;
		if (hasArg("-cli")) {
			itsCLIName = *itsCurrentArg;
			itsCurrentArg++;
		}
		break;

      /* Display */
    case Display:
      if(hasArg("-d")){
		vector<int> tmpIV3=fillIV(*itsCurrentArg);
		switch(tmpIV3.size()){
		case 1:
		  itsDisplayMode = tmpIV3[0];
		  switch(tmpIV3[0]){
			case W:
				itsWireframe=true;
				itsNormals=false;
				itsNormalsDisplay=false;
				itsDiffuse=false;
				itsSpecular=false;
				itsFlat=false;
				itsWireAndSurface = false;
			break;
			case WF:
				itsWireframe=true;
				itsNormals=true;
				itsNormalsDisplay=false;
				itsDiffuse=false;
				itsSpecular=false;
				itsFlat=true;
				itsWireAndSurface = true;
			break;
			case ND:
				itsWireframe=false;
				itsNormals=true;
				itsNormalsDisplay=false;
				itsDiffuse=true;
				itsSpecular=false;
				itsFlat=false;
				itsWireAndSurface = false;
			break;
			case WND:
				itsWireframe=true;
				itsNormals=true;
				itsNormalsDisplay=false;
				itsDiffuse=true;
				itsSpecular=false;
				itsFlat=false;
				itsWireAndSurface = true;
			break;
			case NDS:
				itsWireframe=false;
				itsNormals=true;
				itsNormalsDisplay=false;
				itsDiffuse=true;
				itsSpecular=true;
				itsFlat=false;
				itsWireAndSurface = false;
			break;
			case WNDS:
				itsWireframe=true;
				itsNormals=true;
				itsNormalsDisplay=false;
				itsDiffuse=true;
				itsSpecular=true;
				itsFlat=false;
				itsWireAndSurface = true;
			break;
			case NIDS:
				itsWireframe=false;
				itsNormals=true;
				itsNormalsDisplay=true;
				itsDiffuse=true;
				itsSpecular=true;
				itsFlat=false;
				itsWireAndSurface = false;
			break;
			default:
			cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-d>.\n";
			cout << "Using default values.\n";						
			break;
	  }
	  break;
	default:
	  itsDisplayMode = DEF_DISPLAY_MODE;
	  cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-d>.\n";
	  cout << "Using default values.\n";
	  break;
	}
	itsCurrentArg++;
      }				
      break;
      
    case ShowBoundingBox:
      itsShowBoundingBox = true;
      break;
      
    case Show3DAxes:
      itsShow3DAxes = true;
      break;

	case UseDC:
		{
		vector<int> tmpIV4=fillIV(*itsCurrentArg);
		if (tmpIV4.size() != 1)
		{
			itsUseDC = DEF_USEDC;
			cout << "\"" << *itsCurrentArg << "\" is not a valid argument for <-usedc>.\n";
			cout << "Using default values.\n";
		}
		if (tmpIV4[0] == 0)
			itsUseDC = false;
		else
			itsUseDC = true;
		}
		itsCurrentArg++;
		break;
		      
      /* ADDED FOR ATTRIBUTES */
      /* Attributes */	
    case Attributes:		
      itsAttributes = true;		
      break;
      
      /* Default */
    default:
      break;
    }
  }
}


HFPCommandLine::ARG HFPCommandLine::getArg(string s){
  int i=0;
  for(vector<string>::iterator it=itsValidArgArray.begin(); it!=itsValidArgArray.end(); it++, i++){
    if(s==*it) return static_cast<HFPCommandLine::ARG>(i);
  }
  cout << "\"" << *itsCurrentArg << "\" is not a valid argument.\nIgnoring\n";
  return ERR;
}


bool HFPCommandLine::validArg(string s){
  int i=0;
  for(vector<string>::iterator it=itsValidArgArray.begin(); it!=itsValidArgArray.end(); it++, i++){
    if(s==*it) return true;
  }
  return false;	
}


string HFPCommandLine::helpOut(){
  string ret;
  
  ret+=HELP0;
  ret+=HELP1;
  ret+=HELPFileName;
  ret+=HELPa;
  ret+=HELPb;
  ret+=HELPcf;
  ret+=HELPcl;
  ret+=HELPd;
  ret+=HELPg;
  ret+=HELPgs;
  ret+=HELPh;
  ret+=HELPi;
  ret+=HELPo;
  ret+=HELPs;
  ret+=HELPt;
  ret+=HELPw;
  ret+=HELPwrl;
  ret+=HELPstl;
  ret+=HELPpov;
  ret+=HELPcli;
  ret+=HELPct;
  ret+=HELPx;
  
  return ret;
}


int HFPCommandLine::clampI (int value, int min, int max, string sarg){
  if (value<min){
    cout << "The value " << value << " in the <" << sarg << "> argument is not within the specified range.\n";
    cout << "Clamping to " << min << ".\n";
    return min;
  }
  else if (value>max){
    cout << "The value " << value << " in the <" << sarg << "> argument is not within the specified range.\n";
    cout << "Clamping to " << max << ".\n";
    return max;
  }
  else
    return value;
}


double HFPCommandLine::clampD (double value, double min, double max, string sarg){
  if (value<min){
    cout << "The value " << value << " in the <" << sarg << "> argument is not within the specified range.\n";
    cout << "Clamping to " << min << ".\n";
    return min;
  }
  else if (value>max){
    cout << "The value " << value << " in the <" << sarg << "> argument is not within the specified range.\n";
    cout << "Clamping to " << max << ".\n";
    return max;
  }
  else
    return value;
}


int HFPCommandLine::fillI(string s){
  int ret=atoi(s.c_str());
  //Warn of Possible Bad Conversion
  if(ret==0){
    if(s!="0"){
      cout << "\"" << s << "\" is not a proper integer value.\nUsing the value 0 instead.\n";
    }
  }
  return ret;
}


double HFPCommandLine::fillD(string s){
  double ret=atof(s.c_str());
  //Warn of Possible Bad Conversion
  if(ret==0){
    if((s!="0") && (s!="0.0")){
      cout << "\"" << s << "\" is not a proper floating point value.\nUsing the value 0.0 instead.\n";
    }
  }
  return ret;
}


vector<string> HFPCommandLine::fillSV(string s){
  string stmp;
  vector<string> ret;
  int j = s.size();
  for(int i = 0; i < j; i++){
    if(s[i] != ','){
      stmp += s[i];
    }
    else{
      ret.push_back(stmp);
      stmp.erase(stmp.begin(), stmp.end());
    }
  }
  ret.push_back(stmp);
  return ret;
}


vector<int> HFPCommandLine::fillIV(string s){
  vector<string> v = fillSV(s);
  vector<int> ret(v.size(), 0);
  int j = ret.size();
  for(int i = 0; i < j; i++){
    ret[i] = fillI(v[i]);
  }
  return ret;	
}


vector<double> HFPCommandLine::fillDV(string s){
  vector<string> v = fillSV(s);
  vector<double> ret(v.size(), 0);
  int j = ret.size();
  for(int i = 0; i < j; i++){
    ret[i] = fillD(v[i]);
  }
  return ret;	
}


void HFPCommandLine::parseMapping(string s){
  vector<string> sv = fillSV(s);
  itsXConstants.resize(sv.size(), 0.0);
  int j = sv.size();
  for(int i = 0; i < j; i++){
    if(sv[i] == "X"){
      itsXYZMapping[0] = i;
    }
    else if(sv[i] == "Y"){
      itsXYZMapping[1] = i;
    }
    else if(sv[i] == "Z"){
      itsXYZMapping[2] = i;
    }
    else{
      itsXConstants[i] = fillD(sv[i]);
    }
  }
}


bool HFPCommandLine::checkMapping(){
  int size = itsXConstants.size();
  for(int i=0; i<3; i++){
    if((itsXYZMapping[i]<0) || (itsXYZMapping[i] >= size))
      return false;
  }
  if((itsXYZMapping[0]==itsXYZMapping[1]) || (itsXYZMapping[0]==itsXYZMapping[2]))
    return false;
  if(itsXYZMapping[1]==itsXYZMapping[2])
    return false;
  return true;	
}


bool HFPCommandLine::checkBB(){
  if(itsBBMinMax[0]>=itsBBMinMax[3]) return false;
  if(itsBBMinMax[1]>=itsBBMinMax[4]) return false;
  if(itsBBMinMax[2]>=itsBBMinMax[5]) return false;
  return true;
}


bool HFPCommandLine::hasArg(string key){
  if((itsCurrentArg == itsArgs.end()) || (validArg(*itsCurrentArg))){
    cout << key << " requires an argument.\n";
    cout << "Using default value.\n";
    return false;
  }
  else{
    return true;
  }				
}


bool HFPCommandLine::hasArg(){
  if((itsCurrentArg == itsArgs.end()) || (validArg(*itsCurrentArg))) return false;
  else return true;				
}
