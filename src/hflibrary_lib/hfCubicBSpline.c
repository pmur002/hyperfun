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
#include "general.h"
#include "cubicspline.h"


extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI,int ic);

static double R_int(double x, double y,int c)
{	
//    int i;
	double ct;
	double xx,yy;
//	double result;
	double r_int;
	xx=x*x;
	yy=y*y;

	ct = (xx+yy);
	ct=pow(ct,c/2.0);
	
	r_int =  ((x)+(y)-sqrt((xx)+(yy))) * ct;


return r_int;
}

double hfCubicBSpline(double* f_a, String_Array_T* str_param)
{

int indice_u=0;
int indice_v=0;
int indice_w=0;
double result=0.0;
double xt ;
double yt ;
double zt ;
int i,j,k;
double bu[4],bv[4],bw[4];
//double tmp_v,tmp_w;
double *ctr_points;
double *bbox;
double old_bbox[6];
int array_index,dim;
double *x;
double cube;
//double xtt,ytt,ztt;
//double xmin,ymin,zmin;
//double xmax,ymax,zmax;
double tmp1,tmp2;
double my_eps;

array_index=(int)f_a[0];
HF_Get_Runtime_Array(array_index, &ctr_points, &dim);

array_index=(int)f_a[1];
HF_Get_Runtime_Array(array_index, &bbox, &dim);

for(i=0;i<6;i++)
old_bbox[i] = bbox[i];
//*****Please change also the bounding box !!!!
if(init_spline==1) {

	W=(int)f_a[2]+3+3;
	V=(int)f_a[3]+3+3;
	U=(int)f_a[4]+3+3;
	
	InitSpline(U,V,W,ctr_points);
    InitArray(U,V,W,ctr_points);
}

 



array_index=(int)f_a[5];
HF_Get_Runtime_Array(array_index, &x, &dim);

my_eps = 3.0*(bbox[3]-bbox[0])/(U-6.0-1.0);
bbox[0] = bbox[0] - my_eps;
bbox[3] = bbox[3] + my_eps;

my_eps = 3.0*(bbox[4]-bbox[1])/(V-6.0-1.0);
bbox[1] = bbox[1] - my_eps;
bbox[4] = bbox[4] + my_eps;

my_eps = 3.0*(bbox[5]-bbox[2])/(W-6.0-1.0);
bbox[2] = bbox[2] - my_eps;
bbox[5] = bbox[5] + my_eps;


			/*Scaling.....*/
			xt = (x[0]-bbox[0])/(bbox[3]-bbox[0]);
			yt = (x[1]-bbox[1])/(bbox[4]-bbox[1]);
			zt = (x[2]-bbox[2])/(bbox[5]-bbox[2]);

			
			/*Mapping to the parameter space :        */

			result=0.0;

/****X****/	if(((xt>0.0)&&(xt<1.0))
			&&((yt>0.0)&&(yt<1.0))
			&&((zt>0.0)&&(zt<1.0)) ){
				xt = xt * noeuds_u[U];
				indice_u=(int)floor(xt)+3;
				if(indice_u<3) indice_u=3;
				for(k=0;k<4;k++)
						bu[k]=bspline_u(xt,indice_u-k,3);
			
/****Y****/	
				yt = yt * noeuds_v[V];
				indice_v=(int)floor(yt)+3;
				if(indice_v<3) indice_v=3;
				for(k=0;k<4;k++)
						bv[k]=bspline_v(yt,indice_v-k,3);
		

/****Z****/	
				zt = zt * noeuds_w[W];
				indice_w=(int)floor(zt)+3;
				if(indice_w<3) indice_w=3;
				for(k=0;k<4;k++)
						bw[k]=bspline_w(zt,indice_w-k,3);
			



			for( k=0;k<4;k++){
				for( i=0;i<4;i++){
						for( j=0;j<4;j++){
						result+=bv[i]*bu[j]*bw[k]*
									(ext_ctr_pts[(indice_w-k)][(indice_u-j)][(indice_v-i)]);
									//(ctr_points[(indice_w-k)*U*V+(indice_u-j)*U+(indice_v-i)]);
						
						}
					}
				}
			}


my_eps =0.0;// 4.0*(bbox[3]-bbox[0])/(U-6.0-1.0);

/*xt = (old_bbox[3]-my_eps-x[0])*(x[0]-old_bbox[0]-my_eps);
yt = (old_bbox[4]-my_eps-x[1])*(x[1]-old_bbox[1]-my_eps);
zt = (old_bbox[5]-my_eps-x[2])*(x[2]-old_bbox[2]-my_eps);
*/

xt = (bbox[3]-my_eps-x[0])*(x[0]-bbox[0]-my_eps);
yt = (bbox[4]-my_eps-x[1])*(x[1]-bbox[1]-my_eps);
zt = (bbox[5]-my_eps-x[2])*(x[2]-bbox[2]-my_eps);

 
tmp1 = R_int(xt,yt,0);			
cube = R_int(tmp1,zt,0);
tmp2 = R_int(cube,result,0);	


//	return tmp2;
	return tmp2;


}

/*--------------------------------------------------------------*/ 
int R_hfCubicBSpline(double* f_a,
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
