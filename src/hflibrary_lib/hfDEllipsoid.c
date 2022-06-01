/*
  Core HyperFun Library

  Primitive: distance to the ellipsoid surface

  Definition: 

  Call: hfDEllipsoid(x,center,a,b,c);

  Parameters:
  x - point coordinates array
  center - sphere center array
  a,b,c - ellipsoid half-axes along x,y,z

  Test file: 

  C-parameters:
  f_a[0]   corresponds to c
  f_a[1]   corresponds to b
  f_a[2]   corresponds to a
  f_a[3]   corresponds to array center
  f_a[4]   corresponds to array x

  Code by: Pierre-Alain Fayolle
  Last revised: 10.25.2004
*/

#include "general.h"
#include <math.h>

extern int HF_Get_Runtime_Array(int index, double** array, int* size);
void HF_Fill_Error(PN_T* p_pn, int er_index, Error_Info_T* EI, int ic);

extern double EPS;

double hfDEllipsoid(double* f_a, String_Array_T* str_param)
{
	double a,b,c;
  double xx,yy,zz,a2,b2,c2;
  double u2,v2,w2,a2u2,b2v2;
  double c2w2,uratio,vratio,wratio;
  double length,t;
  double signed_distance, distance;

  double p,q,r;
  double EPSILON;
  double fabs_fs;

  double closestx, closesty;
  double closestz,diffx,diffy;
  double diffz;

  double* x;
  double* center;

  int maxIteration;
  int i;
  int array_index, dim;

  short isInside;

  c = f_a[0];
  b = f_a[1];
  a = f_a[2];

  array_index = (int)f_a[3];
  HF_Get_Runtime_Array(array_index, &center, &dim);

  array_index = (int)f_a[4];
  HF_Get_Runtime_Array(array_index, &x, &dim);

  if (dim != 3 || (fabs(a) < EPS) || (fabs(b) < EPS) || (fabs(c) < EPS))
    return -1111111111111.0;

  xx = x[0] - center[0];
  yy = x[1] -center[1];
  zz = x[2] - center[2];

  a2 = a*a;
  b2 = b*b;
  c2 = c*c;
	
  u2 = xx*xx;
  v2 = yy*yy;
  w2 = zz*zz;
	
  a2u2 = a2*u2;
  b2v2 = b2*v2;
  c2w2 = c2*w2;
	
  uratio = xx/a;
  vratio = yy/b;
  wratio = zz/c;
	
  length = sqrt(xx*xx + yy*yy + zz*zz);
	
  if (uratio*uratio + vratio*vratio + wratio*wratio < 1.0){
    t = 0.0;
    isInside = 1;
  }
  else {
    double max = a;
    if (b>max) max = b;
    if (c>max) max = c;
    t = max*length;
    isInside = 0;
  }
	
  maxIteration = 128;
  EPSILON = 1e-6;
  	
  for (i=0;i<maxIteration;++i){
    double p2,q2,r2;
    double s,pq,pr,qr,pqr,ds;

    p=t+a2;
    q = t+b2;
    r=t+c2;
    p2=p*p;
    q2=q*q;
    r2=r*r;

    s = p2*q2*r2 - a2u2*q2*r2 - b2v2*p2*r2 -c2w2*p2*q2;
    if ((fabs_fs = fabs(s)) < EPSILON)
      break;

    pq = p*q, pr=p*r, qr=q*r, pqr=p*q*r;
    ds = 2.0*(pqr*(qr+pr+pq)-a2u2*qr*(q+r)-b2v2*pr*(p+r)-c2w2*pq*(p+q));
    t -= s/ds;
  }
	
  closestx = (a2*x[0] + t*center[0])/(t+a2);
  closesty = (b2*x[1] + t*center[1])/(t+b2);
  closestz = (c2*x[2] + t*center[2])/(t+c2);
	
  diffx=closestx - x[0];
  diffy=closesty - x[1];
  diffz=closestz - x[2];
	
  distance = sqrt(diffx*diffx + diffy*diffy + diffz*diffz);
	
  if (isInside == 0)
    signed_distance = -distance;
  else
    signed_distance = distance;
 
  return signed_distance;
}
