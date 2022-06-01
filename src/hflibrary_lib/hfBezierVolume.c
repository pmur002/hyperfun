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
#include "Bezier.h"
#include "general.h"

/*
HyperFun Library

Primitive : Bezier Volume with functionnal clipping

Definition :
	    l   m  n 	
S(x,y,z) = Sum(Sum(Sum(Br(x) * Br(y) * Br(z) * P(w)
	   i=0 j=0 k=0 [i,l]   [j,m]   [k,n]   [i,j,k]

Where : 
-- x,y,z : current point

--Br(x) is the Bernstein basis functions
  [k,n]

--P(w) is the fourth coordinate of the i-j-k-th control point.
[i,j,k]


Call: hfBezierVolume(x,l,m,n,bbox,w)

Parameters:

x - points coordinate array

l - degree on u axis
m - degree on v axis
n - degree on w axis
 
bbox : Array of the bounding box:
		xmin,ymin,zmin, xmax,ymax,zmax.

w - array of the fourth coordinate of each control points.
    The order is:
		P(0,0,0),P(0,0,1),...,P(0,0,n) 
		P(0,1,0),P(0,1,1),...,P(0,1,n)
		...
		P(0,m,0),P(0,m,0),...,P(0,m,n)
		P(1,m,0),P(1,m,1),...,P(0,m,n)
		...
		P(l-1,m,0),P(l-1,m,0),...,P(l-1,m,n)
		P(l,m,0),P(l,m,1),...,P(l,m,n)

Test file : hffish.hpf -- hfVase.hpf

C-Parameters:
	f_a[0]	corresponds to array w
	f_a[1]	corresponds to the array of the bounding box
	f_a[2]	corresponds to n
	f_a[3]	corresponds to m
	f_a[4]	corresponds to l
	f_a[5]	corresponds to array x o

Code by : Schmitt 
Last revised : 6.28.99
*/

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI,int ic);


double hfBezierVolume(double* f_a, String_Array_T* str_param)
{
double *ctr_points;
double *x;
double *bbox;
int l,m,n;
int array_index,dim;
double cube;
int i,j,k;
double B_value;
double xt;
double yt;
double zt;


array_index=(int)f_a[0];
HF_Get_Runtime_Array(array_index, &ctr_points, &dim); 

array_index=(int)f_a[1];
HF_Get_Runtime_Array(array_index, &bbox, &dim); 

n=(int)f_a[2];
m=(int)f_a[3];
l=(int)f_a[4];

if(init_bezier==1) InitBezier(l,m,n);
array_index=(int)f_a[5];
HF_Get_Runtime_Array(array_index, &x, &dim); 

if (dim != 3) return -1111111111111.0;


xt = (x[0]-bbox[0])/(bbox[3]-bbox[0]);
yt = (x[1]-bbox[1])/(bbox[4]-bbox[1]);
zt = (x[2]-bbox[2])/(bbox[5]-bbox[2]);



for(i=0;i<l;i++)
		bu[i]=Bernstein(i,l-1,xt);
	

for(j=0;j<m;j++)
		bv[j]=Bernstein(j,m-1,zt);
			

for(k=0;k<n;k++)
		bw[k]=Bernstein(k,n-1,yt);

/*********/
B_value=0.0;
for(i=0;i<l;i++)
	for(j=0;j<m;j++)
		for(k=0;k<n;k++){
		B_value=B_value+(
			 bu[i]*bv[j]*bw[k]
			*ctr_points[i*m*n+j*n+k]
		   	);

	}


xt = (bbox[3]-x[0])*(x[0]-bbox[0]);
yt = (bbox[4]-x[1])*(x[1]-bbox[1]);
zt = (bbox[5]-x[2])*(x[2]-bbox[2]); 
cube = R_int(R_int(xt,yt),zt);
 
//return B_value;

return R_int(cube,B_value);

}
/*--------------------------------------------------------------*/ 
int R_hfBezierVolume(double* f_a,
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
