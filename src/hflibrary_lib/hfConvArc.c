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
#    Primitive:  Cauchy Arc with Convolution Surface
#
#    Definition:  1 / (1 + S^2*R^2)^2
#                 R is the distance between primitive and x
#
#    Parameters:  T - threshold value
#                 S - control value for width of the kernel
#                 angle - rotate angle
#                 axis - rotate axis
#                 theta - arc angle
#                 radius - arc radius
#                 center - center of arc
#                 x - points coordinate array
# 
#    Call      :  hfConvArc(x,center,radius,theta,axis,angle,S,T);
#
#    Test file :  myarc.hf
#
#    C-Parameters :  f_a[0] corresponds to T
#                    f_a[1] corresponds to S
#                    f_a[2] corresponds to array angle
#                    f_a[3] corresponds to array axis
#                    f_a[4] corresponds to array theta
#                    f_a[5] corresponds to array radius
#                    f_a[6] corresponds to array center
#                    f_a[7] corresponds to array x
#
#    Coded by :  Ken Yoshikawa
#
#    Last revised :  Feb.24.2000
###########################################################################
*/

#include <math.h>
#include "general.h"
#include "archyp.h"

#define PI    3.141592

#define SQ(x) ((x)*(x))
#define QU(x) (SQ(x)*SQ(x))

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern const double EPS;

double hfConvArc(double* f_a, String_Array_T* str_param) {
  int n;
  double *x;
  double *center, *radius, *theta;
  double *axis, *angle;
  double *S, T;
  double r, th, rd=PI/180.0;
  double f1, f2, d2, b, a, p1, p2, p3, f=0.0;
  double i,j,k,c,s,ii,jj,kk,ij,jk,ki,is,js,ks,one_c,length;
  double cx,cy,cz,new_x,new_y,new_z;
  double tempx,tempy,tempz;
  double over_i=0.0,over_j=0.0,over_k=1.0,over_th,over_c,over_s,
         over_one_c,over_ii,over_jj,over_kk,over_ij,over_jk,over_ki,
         over_is,over_js,over_ks,over_x,over_y,over_z;
  int array_index,dim;
  int N;  /* the number of primitive */

  T = f_a[0];

  array_index = (int)f_a[1];
  HF_Get_Runtime_Array(array_index, &S, &N);

  array_index = (int)f_a[2];
  HF_Get_Runtime_Array(array_index, &angle, &dim);

  array_index = (int)f_a[3];
  HF_Get_Runtime_Array(array_index, &axis, &dim);

  array_index = (int)f_a[4];
  HF_Get_Runtime_Array(array_index, &theta, &dim);

  array_index = (int)f_a[5];
  HF_Get_Runtime_Array(array_index, &radius, &dim);

  array_index = (int)f_a[6];
  HF_Get_Runtime_Array(array_index, &center, &dim);

  array_index = (int)f_a[7];
  HF_Get_Runtime_Array(array_index, &x, &dim);
  if(dim != 3)
    return -1111111111111.0;

  for(n=0;n<N;n++) {
    cx = center[3*n];    /* Center of Arc */
    cy = center[3*n+1];
    cz = center[3*n+2];
 
    r = radius[n];
    angle[n] += EPS;  /* avoid error */

    i = axis[3*n] + EPS; /* avoid error */
    j = axis[3*n+1] + EPS; /* avoid error */
    k = axis[3*n+2] + EPS; /* avoid error */

    length = sqrt(i*i + j*j + k*k);
    if( length < EPS ) {
      length = EPS;
    }

    i /= length;   /* Calculate normal vector around which Arc rotates */
    j /= length;
    k /= length;

    c = cos(rd * (-angle[n]));
    s = sin(rd * (-angle[n]));

    one_c = 1.0 - c;

    ii = i*i;  jj = j*j;  kk = k*k;
    ij = i*j;  jk = j*k;  ki = k*i;
    is = i*s;  js = j*s;  ks = k*s;

    if(theta[n] > 360.0)
      theta[n] = 360.0;

    /********** [Begin] over PI operation ***************************/
    if(theta[n] > 180.0) {
      over_th = (theta[n] - 180.0)*rd;
      theta[n] = 180.0;
      
      /* rotate by -angle */
      tempx = (c + ii * one_c)*(x[0]-cx) + (-ks + ij * one_c)*(x[1]-cy) + (js + ki * one_c)*(x[2]-cz);
      tempy = (ks + ij * one_c)*(x[0]-cx) +  (c + jj * one_c)*(x[1]-cy) + (-is + jk * one_c)*(x[2]-cz);
      tempz = (-js + ki * one_c)*(x[0]-cx) + (is + jk * one_c)*(x[1]-cy) + (c + kk * one_c)*(x[2]-cz);

      /************* [Begin] rotate -PI operation **********************/
      over_c = cos(rd * (-180.0));
      over_s = sin(rd * (-180.0));
      over_one_c = 1.0 - over_c;

      over_ii = SQ(over_i); over_jj = SQ(over_j); over_kk = SQ(over_k);
      over_ij = over_i*over_j; over_jk = over_j*over_k; over_ki = over_k*over_i;
      over_is = over_i*over_s; over_js = over_j*over_s; over_ks = over_k*over_s;

      over_x = (over_c + over_ii * over_one_c)*(tempx) + (-over_ks + over_ij * over_one_c)*(tempy) + (over_js + over_ki * over_one_c)*(tempz);
      over_y = (over_ks + over_ij * over_one_c)*(tempx) + (over_c + over_jj * over_one_c)*(tempy) + (-over_is + over_jk * over_one_c)*(tempz);
      over_z = (-over_js + over_ki * over_one_c)*(tempx) + (over_is + over_jk * over_one_c)*(tempy) + (over_c + over_kk * over_one_c)*(tempz);
      /************* [End] rotate -PI operation **********************/

      a = 2.0*r*S[n]*S[n];
      d2 = SQ(over_x) + SQ(over_y) + SQ(over_z);
      b = 1.0 + SQ(r)*SQ(S[n]) + SQ(S[n])*d2;
      p2 = -QU(r)*QU(S[n]) + 2.0*SQ(r)*SQ(S[n])*(SQ(S[n])*(d2 - 2.0*SQ(over_z)) - 1.0) - SQ(1.0 + SQ(S[n])*d2);
      p1 = (p2 < 0.0) ? sqrt(-p2) : sqrt(p2);
      p3 = p1*p2;
      
      f1 = (b*over_y) / (over_x*p2*(a*over_x-b))
	+ (a*(SQ(over_x) + SQ(over_y))*sin(over_th) - b*over_y) / (over_x*p2*(a*(over_x*cos(over_th) + over_y*sin(over_th)) - b));
      
      if(p2 < 0.0)
	f2 = 2.0*b*(atan(-a*over_y/p1) + atan((a*over_y - (a*over_x + b)*tan(over_th/2.0)) / p1)) / p3;
      else
	f2 = 2.0*b*(atanh(a*over_y/p1) + atanh(((a*over_x + b)*tan(over_th/2.0) - a*over_y) / p1)) / p3;

      f += f1 + f2;
    }
    /********** [End] over PI operation ***************************/

    th = theta[n]*rd;
    new_x = (c + ii * one_c)*(x[0]-cx) + (-ks + ij * one_c)*(x[1]-cy) + (js + ki * one_c)*(x[2]-cz);
    new_y = (ks + ij * one_c)*(x[0]-cx) +  (c + jj * one_c)*(x[1]-cy) + (-is + jk * one_c)*(x[2]-cz);
    new_z = (-js + ki * one_c)*(x[0]-cx) + (is + jk * one_c)*(x[1]-cy) + (c + kk * one_c)*(x[2]-cz);

    a = 2.0*r*S[n]*S[n];
    d2 = SQ(new_x) + SQ(new_y) + SQ(new_z);
    b = 1.0 + SQ(r)*SQ(S[n]) + SQ(S[n])*d2;
    p2 = -QU(r)*QU(S[n]) + 2.0*SQ(r)*SQ(S[n])*(SQ(S[n])*(d2 - 2.0*SQ(new_z)) - 1.0) - SQ(1.0 + SQ(S[n])*d2);
    p1 = (p2 < 0.0) ? sqrt(-p2) : sqrt(p2);
    p3 = p1*p2;

    f1 = (b*new_y) / (new_x*p2*(a*new_x-b))
      + (a*(SQ(new_x) + SQ(new_y))*sin(th) - b*new_y) / (new_x*p2*(a*(new_x*cos(th) + new_y*sin(th)) - b));

    if(p2 < 0.0)
      f2 = 2.0*b*(atan(-a*new_y/p1) + atan((a*new_y - (a*new_x + b)*tan(th/2.0)) / p1)) / p3;
    else
      f2 = 2.0*b*(atanh(a*new_y/p1) + atanh(((a*new_x + b)*tan(th/2.0) - a*new_y) / p1)) / p3;

    f += f1 + f2;
  }

  return f - T;
}
