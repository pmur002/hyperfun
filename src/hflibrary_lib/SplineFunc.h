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


#ifndef SPLINEFUNC_H
#define SPLINEFUNC_H


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

/**************************************************/
/**************************************************/

double Knot_u(int i);

double RecursSpline_u(int i,int n,double t);

double Knot_v(int i);

double RecursSpline_v(int i,int n,double t);

double Knot_w(int i);

double RecursSpline_w(int i,int n,double t);


#endif /* SPLINEFUNC_H */
