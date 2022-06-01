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
#    Primitive:  Cauchy Line with Convolution Surface
#
#    Definition:  1 / (1 + S^2*R^2)^2
#                 R is the distance between primitive and x
#
#    Parameters:  R - width of primitive
#                 S - control value for width of the kernel
#                 end - ending points coordinate array
#                 begin - beginning points coordinate array
#                 x - points coordinate array
# 
#    Call      :  hfConvLine(x,begin,end,R);
#
#    Test file :  myline2.hf
#
#    C-Parameters :  f_a[0] corresponds to R
#                    f_a[1] corresponds to array end
#                    f_a[2] corresponds to array begin
#                    f_a[3] corresponds to array x
#
#    Coded by :  Ken Yoshikawa
#
#    Last revised :  Nov.27.1999
###########################################################################
*/

#include <math.h>
#include "general.h"

#define SQ(x) ((x)*(x))

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

double hfConvLineR(double* f_a, String_Array_T* str_param) {
  int n;
  double *x;
  double *begin;   /* beginning Point */
  double *end;     /* ending Point */
  double R;
  double T;        /* threshold value */
  double ax, ay, az;   /* normalized vector from beginnig to ending Point */
  double dx, dy, dz;   /* d = r - b */
  double pT,qT2;
  double l, p, q2, xx, /*S=1.0*/ S=1.0, f_tmp, f=0.0; 
  int array_index,dim;
  int N;  /* the number of primitive */

  R = f_a[0];

  if(R<1) S= 1/R;

  array_index = (int)f_a[1];
  HF_Get_Runtime_Array(array_index, &end, &N);
  N /= 3;

  array_index = (int)f_a[2];
  HF_Get_Runtime_Array(array_index, &begin, &dim);

  array_index = (int)f_a[3];
  HF_Get_Runtime_Array(array_index, &x, &dim);
  if(dim != 3)
    return -1111111111111.0;

  for(n=0;n<N;n++) {
    l = sqrt(SQ(end[3*n] - begin[3*n]) + SQ(end[3*n+1] - begin[3*n+1]) + SQ(end[3*n+2] - begin[3*n+2]));

    ax = (end[3*n] - begin[3*n]) / l;
    ay = (end[3*n+1] - begin[3*n+1]) / l;
    az = (end[3*n+2] - begin[3*n+2]) / l;
    
    dx = x[0] - begin[3*n];
    dy = x[1] - begin[3*n+1];
    dz = x[2] - begin[3*n+2];
    
    pT = sqrt(1 + SQ(S)*SQ(R));
    qT2 = 1 + SQ(S)*(SQ(R) + SQ(l));

    T = l / (2.0*SQ(pT)*qT2)
      + (atan(S*l/pT)) / (2.0*S*SQ(pT)*pT);

    xx = dx*ax + dy*ay + dz*az;
    p = sqrt(1 + SQ(S)*(SQ(dx) + SQ(dy) + SQ(dz) - SQ(xx)));
    q2 = 1 + SQ(S)*(SQ(dx) + SQ(dy) + SQ(dz) + SQ(l) - 2.0*l*xx );

    f_tmp = xx / (2.0*p*p*(p*p + S*S*xx*xx)) + (l - xx) / (2.0*p*p*q2)
            + (atan(S*xx/p) + atan(S*(l - xx)/p)) / (2.0*S*p*p*p);

    if(f < 1.0) {
      f = f + f_tmp;
      if(f > 1.0)
        f = 1.0;
    }
  }

  return f - T;
}






