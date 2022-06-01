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
###########################################################################
#  HyperFun Library
#
#    Primitive:  Cauchy Point with Convolution Surface
#
#    Definition:  1 / (1 + S^2*R^2)^2
#                 R is the distance between primitive and x
#
#    Parameters:  T - threshold value
#                 S - control value for width of the kernel
#                 vect - center of primitive
#                 x - points coordinate array
# 
#    Call      :  hfConvPoint(x,vect,S,T);
#
#    Test file :  mypoint.hf
#
#    C-Parameters :  f_a[0] corresponds to T
#                    f_a[1] corresponds to S
#                    f_a[2] corresponds to array vect
#                    f_a[3] corresponds to array x
#
#    Coded by :  Ken Yoshikawa
#
#    Last revised :  Feb.24.2000
###########################################################################
*/

#include <math.h>
#include "general.h"

#define SQ(x) ((x)*(x))

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

double hfConvPoint(double* f_a, String_Array_T* str_param) {
  int n;
  double *x;
  double *vect;
  double *S, T;
  double r2, f=0.0;
  int array_index,dim;
  int N;   /* the number of primitive */

  T = f_a[0];

  array_index = (int)f_a[1];
  HF_Get_Runtime_Array(array_index, &S, &N);

  array_index = (int)f_a[2];
  HF_Get_Runtime_Array(array_index, &vect, &dim);

  array_index = (int)f_a[3];
  HF_Get_Runtime_Array(array_index, &x, &dim);
  if(dim != 3)
    return -1111111111111.0;

  for(n=0;n<N;n++) {
    r2 = SQ(vect[3*n] - x[0]) + SQ(vect[3*n+1] - x[1]) + SQ(vect[3*n+2] - x[2]);
    f += 1 / SQ(1 + SQ(S[n]) * r2);
  }

  return f - T;
}
