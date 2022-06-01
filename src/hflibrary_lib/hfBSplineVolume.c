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


#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SplineFunc.h"
#include "Bezier.h"
#include "general.h"


/***************************************************/
/*
HyperFun Library

Primitive : BSpline Volume with functionnal clipping

Definition :
	    l   m  n
S(x,y,z) = Sum(Sum(Sum(N(x) * N(y) * N(z) * P(w)
	   i=0 j=0 k=0     [i,l]   [j,m]  [k,n]  [i,j,k]

Where :
-- x,y,z : current point

--N(x) is the spline blossoming function
   [k,n]

--P(w) is the function value of the i-j-k-th control point.
[i,j,k]


Call: hfBSplineVolume(x,l,m,n,order_l,order_m,order_n,bbox,w)

Parameters:

x - points coordinate array

l - number of control points on u axis
m - number of control points on v axis
n - number of control points on w axis

order_l - interpolation degree on u axis
order_m - interpolation degree on v axis
order_n - interpolation degree on w axis

bbox : Array of the bounding box:
		xmin,ymin,zmin, xmax,ymax,zmax.

w - array of the function value of each control points.
     The order is:
		P(0,0,0),P(0,0,1),...,P(0,0,n)
		P(0,1,0),P(0,1,1),...,P(0,1,n)
		...
		P(0,m,0),P(0,m,0),...,P(0,m,n)
		P(1,m,0),P(1,m,1),...,P(0,m,n)
		...
		P(l-1,m,0),P(l-1,m,0),...,P(l-1,m,n)
		P(l,m,0),P(l,m,1),...,P(l,m,n)

Test file : fish.hpf

C-Parameters:
	f_a[0]	corresponds to array w (size l*m*n)
	f_a[1]	corresponds to the array of the bounding box
	f_a[2]	corresponds to order_n
	f_a[3]	corresponds to order_m
	f_a[4]	corresponds to order_l
	f_a[5]	corresponds to n
	f_a[6]	corresponds to m
	f_a[7]	corresponds to l
	f_a[8]	corresponds to array x o

Code by : Toon
Last revised : 20.11.99
*/



double hfBSplineVolume(double* f_a, String_Array_T* str_param)
{
  double *ctr_points;
  double *x;
  double *bbox;
  int u,v,w;
  int i,j,k;
  
  int array_index,dim;
  double cube;
  double B_value;
  double xt;
  double yt;
  double zt;
  double factor_u;
  double factor_v;
  double factor_w;
  
  array_index=(int)f_a[0];
  HF_Get_Runtime_Array(array_index, &ctr_points, &dim);
  
  array_index=(int)f_a[1];
  HF_Get_Runtime_Array(array_index, &bbox, &dim);
  
  KnotOrder_u=(int)f_a[2];
  KnotOrder_v=(int)f_a[3];
  KnotOrder_w=(int)f_a[4];
  /* printf("oru=%d  orv=%d  orw=%d\n",KnotOrder_u,KnotOrder_v,KnotOrder_w); */
  
  w=(int)f_a[5];
  v=(int)f_a[6];
  u=(int)f_a[7];
  
  array_index=(int)f_a[8];
  HF_Get_Runtime_Array(array_index, &x, &dim);
  
  if (dim != 3) return -1111111111111.0;
  
  
  xt = (x[0]-bbox[0])/(bbox[3]-bbox[0]);
  yt = (x[1]-bbox[1])/(bbox[4]-bbox[1]);
  zt = (x[2]-bbox[2])/(bbox[5]-bbox[2]);
  
  
  factor_u = 1.0+(double)(u-KnotOrder_u);
  factor_v = 1.0+(double)(v-KnotOrder_v);
  factor_w = 1.0+(double)(w-KnotOrder_w);
  
  
  KnotNbSom_u = u-1;
  KnotNbSom_v = v-1;
  KnotNbSom_w = w-1;
  
  B_value=0.0;
  /*printf("fu=%f  fv=%f  fv=%f\n",factor_u,factor_v,factor_w); */
  /* printf("x=%f  y=%f  z=%f\n",xt,yt,zt); */
  xt =xt * factor_u;
  yt =yt * factor_v;
  zt =zt * factor_w;
  
  /* printf("x=%f  y=%f  z=%f\n",xt,yt,zt); */
  
  /* getchar(); */
  
  for( k=0;k<w;k++){
    for( i=0;i<u;i++)
      for( j=0;j<v;j++){
	B_value=B_value+(
			 RecursSpline_v(j,KnotOrder_v,yt)
			 *RecursSpline_u(i,KnotOrder_u,xt)
			 *RecursSpline_w(k,KnotOrder_w,zt)
			 *ctr_points[k*u*v+i*v+j]
			 );
	
      }
    ;
  }
  
  
  cube=R_int(R_int((bbox[3]-0.01-x[0])*(x[0]-bbox[0]+0.01),
		   (bbox[4]-0.01-x[1])*(x[1]-bbox[2]+0.01)),(bbox[5]-x[2]-0.01)*(x[2]-bbox[2]+0.01));
  
  /* if(B_value!=0.0) printf("%f\n",B_value); */
  /* getchar(); */
  return R_int(cube,B_value);
  /* return 1.0; */
  /* return B_value; */
  
}
/*--------------------------------------------------------------*/
int R_hfBSplineVolume(double* f_a,
		      String_Array_T* str_param,
		      Error_Info_T* error_info,
		      PN_T* p_pn,
		      int code_index)
{
  /*
    if((fabs(f_a[0]) < EPS) || (fabs(f_a[1]) < EPS) || (fabs(f_a[2]) < EPS))
    {
    HF_Fill_Error(p_pn, Blend_2_3_Error, error_info, code_index);
    return 0;
    }
    return 1;
  */
  
  return 1;
}
/*--------------------------------------------------------------*/
