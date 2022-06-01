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

Primitive: Voxel to Function converion

Definition: 

Call: 

Parameters:
 x - point coordinates
 

Test file: 

C-parameters:
      

Code by: Pasko
Last revised: 10.09.01 (by Schmitt)
*/
#include "hfvoxel.h"




//typedef unsigned char byte;
//byte tmpData[NZ*NY*NX];
 unsigned char volumeData[NZ][NY][NX];

/*-------------------------------------------------------------------------

voxel density for given voxel indeces */
double LEVEL;

int findVoxelDensity(int idx, int idxx, int idxxx)
{
 if(idx>=NX || idxx>=NY || idxxx>=NZ)
 {
/*printf("NOT EXIST= %lu %lu %lu %lu\n", idx,idxx,idxxx,NOT_EXIST);*/
  return (int)(0xFFFF);
 }

//if(volumeData[idxxx][idxx][idx]>=LEVEL) return 1;
//else return -1;
return volumeData[idxxx][idxx][idx];

}

/*-------------------------------------------------------------------------

real function from voxel data with trilinear interpolation*/
static FILE *fin;
static int readflag = 0;


/*--------------------------------------------------------------*/
double hfVoxel(double* f_a, String_Array_T* str_param)
{
	double px,py,pz;
   int array_index;
   int      dim;
   double* x;

double FAILED;
long ED; 
//int NX;
//int NY; 
//int NZ; 
double X_MAX; 
double Y_MAX; 
double Z_MAX; 

double X_STEP; 
double Y_STEP; 
double Z_STEP; 
int NOT_FOUND;
int NOT_EXIST;

 int fg1,fg2,fg3,fg4,fg5,fg6,fg7,fg8;
 long idx, idy, idz, idx1,idy1,idz1,num;
 double fglob,u,v,w,u1,v1,w1,xt,yt,zt,xx,yy,zz;
 double a = 0.48*0.48, b = 0.48*0.48 , c = 0.48*0.48;





NOT_FOUND= (-1);
NOT_EXIST= (int)(0xFFFF);

//#define LEVEL ((double) 32.1) /* for siemens.vol */


 FAILED= -9.9e5;

ED = 1;
//NX=150;
//NY=200; 
//NZ=192; 
X_MAX=(double)1.0;
Y_MAX=(double)1.0;
Z_MAX=(double)1.0;

X_STEP = (double)(X_MAX/(NX-1));
Y_STEP = (double)(Y_MAX/(NY-1));
Z_STEP = (double)(Z_MAX/(NZ-1));


if(readflag == 0){
/*------------------------------------------------------------------*/
 if (!(fin=fopen("siemens.vol","rb")))
 {
  printf("Cannot open data file \n");
  ////exit(-1);
 }
// num=read(fileno(fin),volumeData,sizeof( unsigned char)*NX*NY*NZ)/sizeof( unsigned char);
  num=fread(volumeData,sizeof( unsigned char),NX*NY*NZ,fin);
 //fread( list, sizeof( char ), 25, stream );
      
  
  printf("%d bytes read \n", num);
 fclose(fin);
 printf("Data file read \n");
 readflag = 1;
/*------------------------------------------------------------------*/
}

LEVEL = (double)f_a[0];

array_index = (int)f_a[1];
HF_Get_Runtime_Array(array_index, &x, &dim);

px=x[0];
py=x[1];
pz=x[2];


xx = px;
yy = py;
zz = pz;

 xt = xx +1.; yt = yy +1.; zt = zz +1.;
 xt = xt/2.; yt = yt/2.; zt = zt/2.;

 idx = (long)((xt)/(X_STEP));
 idy = (long)((yt)/(Y_STEP));
 idz = (long)((zt)/(Z_STEP));
 u=(xt-X_STEP*(double)idx)/X_STEP;
 v=(yt-Y_STEP*(double)idy)/Y_STEP;
 w=(zt-Z_STEP*(double)idz)/Z_STEP;
 u1=1.-u;
 v1=1.-v;
 w1=1.-w;
idx1=idx+ED;
idy1=idy+ED;
idz1=idz+ED;

/*if((fg1=findVoxelDensity(idx, idy,idz))==NOT_EXIST)
  return FAILED;*/
fglob=FAILED;
if((idx >=0) && (idx1<NX) && (idy >= 0) && (idy1<NY) && (idz >=0) &&
(idz1 < NZ))
{
if((fg1=findVoxelDensity(idx, idy,idz))==NOT_EXIST)
  return FAILED;
if((fg2=findVoxelDensity(idx, idy1,idz))==NOT_EXIST)
  return FAILED;
if((fg3=findVoxelDensity(idx1, idy1,idz))==NOT_EXIST)
  return FAILED;
if((fg4=findVoxelDensity(idx1, idy,idz))==NOT_EXIST)
  return FAILED;
if((fg5=findVoxelDensity(idx, idy,idz1))==NOT_EXIST)
  return FAILED;
if((fg6=findVoxelDensity(idx, idy1,idz1))==NOT_EXIST)
  return FAILED;
if((fg7=findVoxelDensity(idx1, idy1,idz1))==NOT_EXIST)
  return FAILED;
if((fg8=findVoxelDensity(idx1, idy,idz1))==NOT_EXIST)
  return FAILED;
fglob = w1*(u1*v1*(double)fg1+v*u1*(double)fg2+
            u*v*(double)fg3+u*v1*(double)fg4) +
        w *(u1*v1*(double)fg5+v*u1*(double)fg6+
            u*v*(double)fg7+u*v1*(double)fg8);
}

fglob = fglob -LEVEL;

return fglob;

 
}

