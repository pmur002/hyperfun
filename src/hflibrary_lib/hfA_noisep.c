/*==============================================================================

 Copyright 1998, 1999 Valery Adzhiev, Alexander Pasko, Ken Yoshikawa 
 Copyright 2003-2004 Benjamin Schmitt

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


/* 
HyperFun Attributes Library

Description: Perlin Noise

Definition: 

Call: hf

Parameters:
 x - point coordinates array 

Test file: PovRay file 

C-parameters:
        f_a[0]   corresponds to point coordinate

Code by: Schmitt
Last revised: 05 May 2003
*/ 
#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "perlin.h"
extern int HF_Get_Runtime_Array(int index, double** array, int* size);

void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);


double hfA_NoiseP(double* f_a, String_Array_T* str_param)
{
 
double* x;

int array_index, dim;
double t;

array_index = (int)f_a[0];
HF_Get_Runtime_Array(array_index, &x, &dim); 

 t = noise3(&x[0]);


return t;
}



