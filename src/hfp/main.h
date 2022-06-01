/*==============================================================================

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



// INCLUDES
//BEN
#pragma warning( disable : 4786 )


#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "mainapp.h"

#ifndef MAIN_H__
#define MAIN_H__

// GLOB VAR
extern int width;
extern int height;

/******************************************************************/


extern CMainApp *edi;

extern double itsCPos[3];
extern double itsCLookAt[3];

extern GLuint hfObject;
extern GLuint hfNormals;
extern GLuint hfAxes;

/****************************************************************/

extern int HF_FACE_LINE_FILL;           //1 : WireFrame; 0:Solid;   2:both
extern int HF_FLAT_SMOOTH;              //0 : Flat;  1 : Smooth
extern int HF_NORMAL;                   // Compute Normal or not ?

extern int HF_SHOW_NORMAL;              //0 : shows normal    1:does not show;
extern int HF_VERTEX_NORMAL;
extern int first_normal_show;
extern int HF_SHOW_AXES;

extern int HF_LIGHT_TYPE;               //0 : ambient;  1: ambient+diffuse;  2:ambient+diffuse+specular. 
extern GLfloat HF_FACE_COLOR[3];			// Face Color; 
extern GLfloat HF_LINE_COLOR[3];

extern float HF_MIN[3],HF_MAX[3];			// Bounding Box

extern int HELP;

extern bool HAS_ATTRIBUTES;

#endif //MAIN_H__
