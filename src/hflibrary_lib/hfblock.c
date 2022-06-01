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
Core HyperFun Library

Primitive: Block

Definition: x:[vertex[1], vertex[1]+dx], ...

Call: hfBlock(x,vertex,dx,dy,dz);

Parameters:
 x - point coordinates array
 vertex - block vertex coordinates array
 dx,dy,dz - edge lengths along x,y,z

Test file: block.hpf

C-parameters:
        f_a[0]   corresponds to dz
        f_a[1]   corresponds to dy
        f_a[2]   corresponds to dx
        f_a[3]   corresponds to array vertex
        f_a[4]   corresponds to array x

Code by: Pasko, Adzhiev
Last revised: 5.8.98
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);
extern double ALPHA;
extern double EPS;

double hfBlock(double* f_a, String_Array_T* str_param)
{
double dx,dy,dz,xt,yt,zt,f;
double* x;
double* vertex;
int array_index, dim;

dz = f_a[0];
dy = f_a[1];
dx = f_a[2];

array_index = (int)f_a[3];
HF_Get_Runtime_Array(array_index, &vertex, &dim);

array_index = (int)f_a[4];
HF_Get_Runtime_Array(array_index, &x, &dim);

if (dim != 3 || (dx<0.) || (dy<0.) || (dz<0.))
   return -1111111111111.0;

xt=(x[0]-vertex[0])*(vertex[0]+dx-x[0]);
yt=(x[1]-vertex[1])*(vertex[1]+dy-x[1]);
zt=(x[2]-vertex[2])*(vertex[2]+dz-x[2]);

if(ALPHA==1.){
   f=(xt<yt)?xt:yt;
   f=(f<zt)?f:zt;}
else {
   f=xt+yt-sqrt(xt*xt+yt*yt-2.*ALPHA*xt*yt);
   f=f+zt-sqrt(f*f+zt*zt-2.*ALPHA*f*zt);
}

return f;
}
