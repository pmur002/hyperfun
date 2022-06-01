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

  Operation: Space Mapping

  Definition: inverse mapping based on cubic potential function


  Call: hfSpaceMapCubic(xt,source,target,b);

  Parameters:
  xt - point coordinates array to be changed
  source : source point coordinate
  target: target point coordinate
  b : parameter control the area of influence along each axis
  Test files: hfspacemapcubic.hf

  C-parameters:
  f_a[0]   corresponds to array b
  f_a[1]   corresponds to array delta vector
  f_a[2]   corresponds to array source point
  f_a[3]   corresponds to xt

  Code by: Schmitt Benjamin
  Last revised: 09.06.05
*/

#include "general.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);


double hfSpaceMapCubic(double* f_a, String_Array_T* str_param)
{

  double* xt;
  double* from;
  double* to;
  double *fratio;
  int array_index, num;
  double dt[3];
  double tmp=0.0;
  int N;
  int i;
  double x,y,z;


  array_index = (int)f_a[0];
  HF_Get_Runtime_Array(array_index, &fratio, &num);

  array_index = (int)f_a[1];
  HF_Get_Runtime_Array(array_index, &to, &num);

  array_index = (int)f_a[2];
  HF_Get_Runtime_Array(array_index, &from, &num);
  N=num;

  array_index = (int)f_a[3];
  HF_Get_Runtime_Array(array_index, &xt, &num);


  for(i=0;i<N;i+=3){

	
    dt[0]=(to[i+0]-from[i+0]);
    dt[1]=(to[i+1]-from[i+1]);
    dt[2]=(to[i+2]-from[i+2]);

    x=	(xt[0]-to[i+0])/(fratio[i+0]*(1.0+sqrt(dt[0]*dt[0])));
    y=	(xt[1]-to[i+1])/(fratio[i+1]*(1.0+sqrt(dt[1]*dt[1])));
    z=	(xt[2]-to[i+2])/(fratio[i+2]*(1.0+sqrt(dt[2]*dt[2])));

    x=x*x;
    y=y*y;
    z=z*z;

    tmp=sqrt(x+y+z);
    if(tmp<=1.0){
      tmp = ((1-tmp)*(1-tmp))*(1-tmp)/(1+tmp);

      xt[0]=xt[0]-tmp*(dt[0]);
      xt[1]=xt[1]-tmp*(dt[1]);
      xt[2]=xt[2]-tmp*(dt[2]);
    }
  }


  return 1.;
}

