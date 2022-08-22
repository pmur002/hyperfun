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


#include <math.h>
#include "SplineFunc.h"
#include "Bezier.h"
#include "hflib.h"


/***************/

int KnotNbSom_u;
int KnotNbSom_v;
int KnotNbSom_w;

/**************/

int KnotOrder_u;
int KnotOrder_v;
int KnotOrder_w;

/**************/

double factor_u;
double factor_v;
double factor_w;


/*******************************************************************************/

double Knot_u(int i)
{
  if(i<KnotOrder_u) return 0.0;
  else if(i>KnotNbSom_u) return (double)(KnotNbSom_u-KnotOrder_u+2);
  else return (double)(i-KnotOrder_u +1);
}


/*******************************************************************************/

double RecursSpline_u(int i,int n,double t){
  double B,T;
  if(n==1) {
    if( ((Knot_u(i)<=t) && (t<Knot_u(i+1))) 
	|| ((t==Knot_u(i+1)) &&  (i==KnotNbSom_u))) B=1.0;
    else B=0.0;
  }
  else {
    B=0.0;
    T=Knot_u(i+n-1)-Knot_u(i);
    if(T!=0.0) B=(t-Knot_u(i))*RecursSpline_u(i,n-1,t)/T;
    
    T=Knot_u(i+n)-Knot_u(i+1);
    if(T!=0.0) B=B+(Knot_u(i+n)-t)*RecursSpline_u(i+1,n-1,t)/T;
    /* return B; */
  }
  return B;
}


/*******************************************************************************/

double Knot_v(int i)
{
  if(i<KnotOrder_v) return 0.0;
  else if(i>KnotNbSom_v) return (double)(KnotNbSom_v-KnotOrder_v+2);
  else return (double)(i-KnotOrder_v +1);
}


/*******************************************************************************/

double RecursSpline_v(int i,int n,double t){
  double B,T;
  if(n==1) {
    if( ((Knot_v(i)<=t) && (t<Knot_v(i+1))) 
	|| ((t==Knot_v(i+1)) &&  (i==KnotNbSom_v))) B=1.0;
    else B=0.0;
  }
  else {
    B=0.0;
    T=Knot_v(i+n-1)-Knot_v(i);
    if(T!=0.0) B=(t-Knot_v(i))*RecursSpline_v(i,n-1,t)/T;
    
    T=Knot_v(i+n)-Knot_v(i+1);
    if(T!=0.0) B=B+(Knot_v(i+n)-t)*RecursSpline_v(i+1,n-1,t)/T;
    /* return B; */
  }
  return B;
}


/*******************************************************************************/

double Knot_w(int i)
{
  if(i<KnotOrder_w) return 0.0;
  else if(i>KnotNbSom_w) return (double)(KnotNbSom_w-KnotOrder_w+2);
  else return (double)(i-KnotOrder_w +1);
}


/*******************************************************************************/

double RecursSpline_w(int i,int n,double t){
  double B,T;
  if(n==1) {
    if( ((Knot_w(i)<=t) && (t<Knot_w(i+1))) 
	|| ((t==Knot_w(i+1)) &&  (i==KnotNbSom_w))) B=1.0;
    else B=0.0;
  }
  else {
    B=0.0;
    T=Knot_w(i+n-1)-Knot_w(i);
    if(T!=0.0) B=(t-Knot_w(i))*RecursSpline_w(i,n-1,t)/T;
    
    T=Knot_w(i+n)-Knot_w(i+1);
    if(T!=0.0) B=B+(Knot_w(i+n)-t)*RecursSpline_w(i+1,n-1,t)/T;
    /* return B; */
    
  }
  return B;
  
}
