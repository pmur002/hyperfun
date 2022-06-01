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
#    Primitive:  Cauchy Mesh (connected triangle) with Convolution Surface
#
#    Definition:  1 / (1 + S^2*R^2)^2
#                 R is the distance between primitive and x
#
#    Parameters:  T - threshold value
#                 S - control value for width of the kernel
#                 tri - index of each triangle coordinate 
#                 vect - triangle coordinate array
#                 x - points coordinate array
# 
#    Call      :  hfConvMesh(x,vect,tri,S,T);
#
#    Test file :  mymesh.hf
#
#    C-Parameters :  f_a[0] corresponds to T
#                    f_a[1] corresponds to S
#                    f_a[2] corresponds to array tri
#                    f_a[3] corresponds to array vect
#                    f_a[4] corresponds to array x
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

double hfConvMesh(double* f_a, String_Array_T* str_param) {
  int n;
  double *x;
  double *vect, *tri;
  double *S, T;
  double a1x, a1y, a1z; /* coordinate 1 */
  double a2x, a2y, a2z; /* coordinate 2 */
  double a3x, a3y, a3z; /* coordinate 3 */
  double len[3];  /* len[0] means distance of coordinate 1 and 2 */
                  /* len[1] means distance of coordinate 2 and 3 */
                  /* len[2] means distance of coordinate 3 and 1 */
  double t, bx, by, bz;
  double ux, uy, uz, ul, u;  /* (ux,uy,uz) is the normal vector from b to a2 */
  double vx, vy, vz, vl, v;  /* (vx,vy,vz) is the normal vector from b to h */
  double dx, dy, dz, d2;     /* vector d = (x,y,z) - (bx,by,bz) */
  double a21x, a21y, a21z;   /* vector a21 = (a2x,a2y,a2z) - (a1x,a1y,a1z) */
  double a13x, a13y, a13z;   /* vector a1h = (a1x,a1y,a1z) - (a3x,a3y,a3z) */
  double a1, a2, h;
  double tempx, tempy, tempz;
  double A2, A, B2, B, C2, C, g, q, w, m, k, f=0.0, arc1, arc2, arc3;
  double n1, n2, n3, n4, n5, n6; 
  int array_index,dim;
  int N;  /* the number of primitive */

  T = f_a[0];

  array_index = (int)f_a[1];
  HF_Get_Runtime_Array(array_index, &S, &N);

  array_index = (int)f_a[2];
  HF_Get_Runtime_Array(array_index, &tri, &dim);

  array_index = (int)f_a[3];
  HF_Get_Runtime_Array(array_index, &vect, &dim);

  array_index = (int)f_a[4];
  HF_Get_Runtime_Array(array_index, &x, &dim);
  if(dim != 3)
    return -1111111111111.0;

  for(n=0;n<N;n++) {
    /* triangle coodinates */
    a1x = vect[3*((int)tri[3*n]-1)];
    a1y = vect[3*((int)tri[3*n]-1)+1];
    a1z = vect[3*((int)tri[3*n]-1)+2];
    a2x = vect[3*((int)tri[3*n+1]-1)];
    a2y = vect[3*((int)tri[3*n+1]-1)+1];
    a2z = vect[3*((int)tri[3*n+1]-1)+2];
    a3x = vect[3*((int)tri[3*n+2]-1)];
    a3y = vect[3*((int)tri[3*n+2]-1)+1];
    a3z = vect[3*((int)tri[3*n+2]-1)+2];

    len[0] = sqrt(SQ(a2x - a1x) + SQ(a2y - a1y) + SQ(a2z - a1z));
    len[1] = sqrt(SQ(a3x - a2x) + SQ(a3y - a2y) + SQ(a3z - a2z));
    len[2] = sqrt(SQ(a1x - a3x) + SQ(a1y - a3y) + SQ(a1z - a3z));

    if ((len[1] >= len[2]) && (len[1] > len[0])){
      tempx = a1x;    tempy = a1y;    tempz = a1z;
      a1x = a2x;      a1y = a2y;      a1z = a2z;
      a2x = a3x;       a2y = a3y;       a2z = a3z;
      a3x = tempx;     a3y = tempy;     a3z = tempz;
    }else if ((len[2] >= len[1]) && (len[2] > len[0])){
      tempx = a1x;    tempy = a1y;    tempz = a1z;
      a1x = a3x;       a1y = a3y;       a1z = a3z;
      a3x = a2x;       a3y = a2y;       a3z = a2z;
      a2x = tempx;    a2y = tempy;    a2z = tempz;
    }
    len[0] = sqrt(SQ(a2x - a1x) + SQ(a2y - a1y) + SQ(a2z - a1z));
    len[1] = sqrt(SQ(a3x - a2x) + SQ(a3y - a2y) + SQ(a3z - a2z));
    len[2] = sqrt(SQ(a1x - a3x) + SQ(a1y - a3y) + SQ(a1z - a3z));

    a21x = a2x - a1x;
    a21y = a2y - a1y;
    a21z = a2z - a1z;
    a13x = a1x - a3x;
    a13y = a1y - a3y;
    a13z = a1z - a3z;

    t = -(a21x*a13x + a21y*a13y + a21z*a13z) / SQ(len[0]);
    bx = a1x + t * a21x;
    by = a1y + t * a21y;
    bz = a1z + t * a21z;

    dx = x[0] - bx;
    dy = x[1] - by;
    dz = x[2] - bz;

    ux = a2x - bx;
    uy = a2y - by;
    uz = a2z - bz;
    ul = sqrt(SQ(ux) + SQ(uy) + SQ(uz));
    ux = ux / ul;
    uy = uy / ul;
    uz = uz / ul;

    vx = a3x - bx;
    vy = a3y - by;
    vz = a3z - bz;
    vl = sqrt(SQ(vx) + SQ(vy) + SQ(vz));
    vx = vx / vl;
    vy = vy / vl;
    vz = vz / vl;

    d2 = SQ(dx) + SQ(dy) + SQ(dz);
    u = dx*ux + dy*uy + dz*uz;
    v = dx*vx + dy*vy + dz*vz;
    h = sqrt( SQ(a3x - bx) + SQ(a3y - by) + SQ(a3z - bz) );
    a1 = sqrt( SQ(a1x - bx) + SQ(a1y - by) + SQ(a1z - bz) );
    a2 = sqrt( SQ(a2x - bx) + SQ(a2y - by) + SQ(a2z - bz) );

    g = v - h;
    m = a2*g + u*h;
    k = u*h - a1*g;
    C2 = 1.0/(SQ(S[n])) + d2 - SQ(u);
    C = sqrt(C2);
    q = C2 - SQ(v);
    w = C2 - 2.0*v*h + SQ(h);
    A2 = SQ(a1)*w + SQ(h)*(q + SQ(u)) - 2.0*a1*h*u*g;
    A = sqrt(A2);
    B2 = SQ(a2)*w + SQ(h)*(q + SQ(u)) + 2.0*a2*h*u*g;
    B = sqrt(B2);

    n1 = a1 + u;
    n2 = a2 - u;
    n3 = a1*n1 + v*h;
    n4 = -a1*u - g*h;
    n5 = -a2*n2 - v*h;
    n6 = -a2*u + g*h;

    arc1 = k * (atan(n3/A) + atan(n4/A))/A;
    arc2 = m * (atan(n5/B) + atan(n6/B))/B;
    arc3 = v * (atan(n1/C) + atan(n2/C))/C;
    f += (arc1 + arc2 + arc3)/(2.0*q*S[n]);
  }

  return f - T;
}
