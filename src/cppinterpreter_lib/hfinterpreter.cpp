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


// HFInterpreter.cpp: implementation of the HFInterpreter class.
//
//////////////////////////////////////////////////////////////////////


#include "hfinterpreter.h"
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HFInterpreter::HFInterpreter()
{
	itsModel = NULL;
	itsObject = NULL;
	itsStack = NULL;
	itsStackUsage = NULL;
	itsPNList.first = NULL;
}



HFInterpreter::~HFInterpreter(){
  //Clean up Char* 's
	if (itsModel != NULL)
		delete[] itsModel;
	itsModel = NULL;
	if (itsObject != NULL)
		delete[] itsObject;
	itsObject = NULL;
	//Clean up Stack
	if(itsStack!=NULL)
		free(itsStack);
	itsStack = NULL;
	if(itsStackUsage!=NULL)
		free(itsStackUsage);
	itsStackUsage = NULL;
	
	// we need to free the memory used in itsPNList with HF_Empty_PN_List
	HF_Empty_PN_List(&itsPNList);
	itsPNList.first = NULL;
}



void HFInterpreter::parse(const string& model, const string& object){
  //Copy model into local C-style string itsModel
  itsModel = new char[model.length()+1];
  model.copy(itsModel,string::npos);
  itsModel[model.length()]='\0';

  itsObject = new char[object.length()+1];
  object.copy(itsObject,string::npos);
  itsObject[object.length()]='\0';
  
	//Note: It's Library List is now Empty
  init();
}



void HFInterpreter::init(){
  int memory_status=HF_parse(itsModel, &itsPNList, &itsErrorList, &itsLibraryList);
  if(!memory_status) //Memory Error
    throw MemoryError("Parse Init"); 
  if(itsErrorList.first!=NULL) //Parse Error
    throw ParseError(itsErrorList.first->er_info.error_text, itsErrorList.first->er_info.line, itsErrorList.first->er_info.position);
  /*
    We compute the size of the stack
  */
  size_t stack_size = 0;
  PN_NODE_T* lpn_node=itsPNList.first;

  while( lpn_node != itsPNList.last)
    {
      stack_size = stack_size + lpn_node->pn_info->stack_size;
      lpn_node = lpn_node->next;
    }
  stack_size = stack_size + lpn_node->pn_info->stack_size;

  // The size of the stack is known now.  
  
  //Set PN_Node, BodyCode, and itsObject to first in list
  //itsPNNode=itsPNList.last;//first;
  
  itsPNNode = itsPNList.first;
  
  while (itsPNNode != itsPNList.last)
    {
      if(!strcmp(itsObject,itsPNNode->pn_info->body_name))
	break;
      itsPNNode = itsPNNode->next;
    }
    
  if (strcmp(itsObject, itsPNNode->pn_info->body_name))
    itsPNNode = itsPNList.first;
    
    //printf("%s\n",itsPNNode->pn_info->body_name );

  itsBodyCode=itsPNNode->pn_info;
  //Set XSize and ASize for current Object
  itsXSize=itsBodyCode->X_dim;
  itsASize=itsBodyCode->A_dim;
  itsSSize=itsBodyCode->S_dim;

  //printf("itssize: %d\n", itsSSize);

  //Allocate Memory For Stacks
  itsStack = (double *) calloc (stack_size,sizeof(double));
  itsStackUsage = (int *) calloc (stack_size,sizeof(int));
  //Ready for calc
}



double HFInterpreter::calc(const vector<double>& X, const vector<double>& A, double *S){
  int i, j;
  //Fill Stack with available values  NOTE: Leaves the rest the same
  for(i=0; (i<(int)X.size()) && (i < getXSize()); i++)
    itsStack[i]=X[i];
  for(j=0; (i<(int)A.size()) && (i<getXSize() + getASize()); i++, j++)
    itsStack[i]=A[j];
  //Perform calculation
  itsCalcStatus=HF_calc(itsBodyCode, itsStack, itsStackUsage, &itsValue, S, &itsER);
  //Return the Value
  return itsValue;
}



double HFInterpreter::calc(const vector<double>& X, double *S){
  //Fill Stack with available values  NOTE: Leaves the rest the same
  for(int i=0; (i < (int)X.size()) && (i < getXSize()); i++)
    itsStack[i]=X[i];
  //Perform calculation
  itsCalcStatus=HF_calc(itsBodyCode, itsStack, itsStackUsage, &itsValue, S,&itsER);
  //Return the Value
  return itsValue;
}



void HFInterpreter::parameters(vector<double> A){
  //Fill Stack with available values  NOTE: Leaves the rest the same
	if (!itsStack)
		return;
  for(int i = 0; (i < getASize()) && (i < A.size()); i++)
    itsStack[i + getXSize()]=A[i];
}



vector<double> HFInterpreter::parameters(){
  //Returns Current Parameters A[]
  vector<double> ret(getASize());
  for(int i=0; i < getASize(); i++)
    ret[i] = itsStack[i + getXSize()];
  return ret;	
}



short HFInterpreter::getXSize(){
  return itsXSize;
}



short HFInterpreter::getASize(){
  return itsASize;
}



short HFInterpreter::getSSize(){
	return itsSSize;
}



void HFInterpreter::object(const string& object){
  // hfp -o name_object did not work
  // problem was here. Fixed now.
  
  //Set Local Test node to first node
  //PN_NODE_T* lpn_node=itsPNList.first;
  itsPNNode = itsPNList.first;
  
  while (itsPNNode != itsPNList.last)
    {
      if(!strcmp(object.c_str(),itsPNNode->pn_info->body_name))
	break;
      itsPNNode = itsPNNode->next;
    }
    
  if (strcmp(object.c_str(), itsPNNode->pn_info->body_name))
    itsPNNode = itsPNList.first;
    
  itsBodyCode = itsPNNode->pn_info;
 }



string HFInterpreter::object(){
  return itsBodyCode->body_name;
}
