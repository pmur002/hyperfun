/*==============================================================================

 Copyright 1999 Eric Fausett
 Copyright 2003-2004 Pierre-Alain Fayolle, Shogo Onozuka, Benjamin Schmitt

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


// HFInterpreter.h: interface for the HFInterpreter class.
//
//////////////////////////////////////////////////////////////////////



#if !defined(HFINTERPRETER_H)
#define HFINTERPRETER_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <string>
#include <vector>
#include <cstring>

extern "C"{
#include "general.h"
}

struct MemoryError{	
	std::string error;
  MemoryError(std::string e){ error=e; }
};

struct ParseError{ 
	std::string error; 
  int line; 
  int pos;
  ParseError(std::string e, int l, int p){ error=e; line=l; pos=p; }
};

class HFInterpreter{
 public:
  HFInterpreter();
  virtual ~HFInterpreter();
		
  //parse can throw MemoryError or ParseError
  virtual void parse(const std::string& model, const std::string &object);
		
  virtual double calc(const std::vector<double>& X, const std::vector<double>& A, double *S);
  virtual double calc(const std::vector<double>& X, double *S );
		
  void parameters(std::vector<double> A);
  std::vector<double> parameters();
  
  virtual short getXSize();
  virtual short getASize();
  virtual short getSSize();



  void object(const std::string& object);
  std::string object();
			
		
		
 private:
  void init();
  
  char* itsModel; char *itsObject;

  int itsXSize;
  int itsASize;
  int itsSSize;
  
  PN_List_T itsPNList;
  ER_List_T itsErrorList;
  PN_List_T itsLibraryList;
  
  PN_NODE_T* itsPNNode;
  PN_T* itsBodyCode;
  
  double* itsStack;
  int* itsStackUsage;
  
  double itsValue;
  Error_Info_T itsER;
  int itsCalcStatus;
};

#endif // !defined(HFINTERPRETER_H)
