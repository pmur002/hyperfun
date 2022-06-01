/*==============================================================================

Copyright 2003-2004 Pierre-Alain Fayolle, Benjamin Schmitt
Copyright 2007-2008 Oleg Fryazinov, Denis Kravtsov
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



/////////////////////////////////////
// INCLUDE 
/////////////////////////////////////
#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "main.h"
#include "hf_error.h"

////////////////////////////////////
// Global variables
////////////////////////////////////

int width;
int height;


double itsCPos[3];
double itsCLookAt[3];

int HF_FACE_LINE_FILL;  // 0: WIREFRAME; 1: SOLID; 2: WIRE & SOLID
int HF_FLAT_SMOOTH;     // 1: FLAT; 0: SMOOTH
int HF_NORMAL;          // 0: NO NORMALS COMPUTED; 1: NORMALS COMPUTED
int HF_SHOW_NORMAL;     // 0: DO NOT SHOW  NORMALS; 1: SHOW NORMALS
int HF_VERTEX_NORMAL;     // 0: FACE NORMALS; 1: VERTEX NORMALS
int first_normal_show;
int HF_SHOW_AXES;
int HF_LIGHT_TYPE;

GLfloat HF_FACE_COLOR[3];
GLfloat HF_LINE_COLOR[3];

GLfloat HF_MIN[3],HF_MAX[3]; // boundng box

int window_main, sub_render;
int sub_help;

bool HAS_ATTRIBUTES = 1;

extern int solidAndWire;




//////////////////////
// Methods
//////////////////////

// should be called after a CMainApp object has been instanciated
// get value of command line thanks to the CMainApp object
void
init_var (CMainApp *mainapp)
{
    width = mainapp->getWindowSizeX();
    height = mainapp->getWindowSizeY();
    
    // gives to HF_FACE_LINE_FILL a 0 or 1 value
    // meaning wireframe or solid 
    HF_FACE_LINE_FILL = mainapp->getFaceLineFill();
    
    // if it has beeen required WireAndSurface from the command line
    // then we should give a value of 2 
    if (mainapp->getWireAndSurface() == 1)
        HF_FACE_LINE_FILL = 2; 
    
    HF_NORMAL = mainapp->getNormal();
    if (HF_NORMAL)
	{   
            HF_SHOW_NORMAL = mainapp->getNormalDisplay ();
            HF_FLAT_SMOOTH = mainapp->getFlat();
	}
    else // we did not compute the normals so we can not show them
	{
            HF_SHOW_NORMAL = 0;
            HF_FLAT_SMOOTH = 1; // defaut is flat
	}

    first_normal_show = 0;
    HF_SHOW_AXES = 1;

    HF_VERTEX_NORMAL = 1;

    HF_LIGHT_TYPE = mainapp->getLightType (); // 1 diffuse; 2 specular; 
    // 3: diffuse & specular.

    mainapp->getFaceColor(HF_FACE_COLOR);
    mainapp->getLineColor(HF_LINE_COLOR);
    mainapp->getBoundingBox(HF_MIN, HF_MAX);
    mainapp->getViewPos(itsCPos, itsCLookAt);

    //BEN
    HAS_ATTRIBUTES = edi->getAttribute();
}



