/*==============================================================================

 Copyright 2004 Pierre-Alain Fayolle

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
#include "quartic.h"

//--------------------------------------------------

// union1: uses unbounded approach in quadrant 1
// and bounded approach in quadrant 3
// arguments:
// xt, IN, value of first function at current point
// yt, IN, value of second function at current point
// R, IN, maximum radius of the bounded approach (quadrant 3)
// alpha, IN, angle for the unbounded approach (quadrant 1)

double
uni_sardef_1(const double xt, const double yt, const double R,
	     const double alpha){	
  double d=0;
  double lim2;
  double g,g2,gt;
  double aa,b,c,Ds;
  double d1,t;

  if (fabs(xt)<1.0e-7 && fabs(yt)<1.0e-7) return 0.0;

  // case when xt=0 and yt!=0
  // or when xt!=0 and yt=0
  if (fabs(xt)<1.0e-7 && yt>0) {d=yt; return d;}
  if (fabs(xt)<1.0e-7 && yt<0) {d=0.0; return d;}
  if (fabs(yt)<1.0e-7 && xt>0) {d=xt; return d;}
  if (fabs(yt)<1.0e-7 && xt<0) {d=0.0; return d;}
	
	
  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1.0e-7 ) && (fabs(xt)>1.0e-7 )) { 	
		
    // II quarter
    if(yt > 0 && xt<0) {d=yt; return d;} 
		
    // IV quarter
    if(xt > 0 && yt < 0) {d=xt; return d;} 
		
    // I quarter
    if((xt > 0) && (yt > 0)) {
      g = 1.0 / (tan(alpha));
      g2 = tan(alpha);
      gt = xt/yt;
			
      if ((gt < g) && (gt>g2)){
	t=tan(alpha);
	aa = t*t + 2.0 * t - 1.0;
	b = -2.0 * t * (xt+yt);
	c = xt*xt + yt*yt;
	d1 = -b - sqrt(b*b - 4.0*aa*c); // +->-
	d1 = d1 / (2.0*aa);
	return d1;
      }
      else if (gt>=g) {
	return xt;
      }
      else if (gt<=g2) {
	return yt;
      }
			
    }
    // end for I quarter
		
		
    // definition of lim2
    lim2 = ((xt + 2*R)*(xt+2*R)) + ((yt + 2*R)*(yt+2*R));
		
    // III quarter
    if((xt < 0 ) && (yt < 0)) {
			
      // angle approach: inside the boundary
      if ( (xt >= -R) || (yt >= -R) || 
	   ((xt >= -2*R) && (yt >= -2*R) && (lim2 >= R*R))) {

	double xx = xt*xt;
	double yy=yt*yt;
	xx = -xx/(4.0*R);
	yy = -yy/(4.0*R);

	if((xt <= yy) && (yt<=xx)){

	  double c0 = -(xt*xt + yt*yt);
	  double c1 = -4.0 * sqrt(R) * (xt+yt);
	  double c2 = -4.0 * R;
	  double c3 = -4.0*sqrt(R);
	  double c4 = 1.0;

	  struct retval sol;

	  double m;

	  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);


	  m = sol.value[1];
	  m = -m*m;
	  return m;

	}// else means outside given angle
	else{
	  if(yt>xx)  {d = yt; return d;} 
	  if(yy<xt) {d = xt; return d;} 
	  // end for inside given angle check
	}//end of if inside angle
				
      }
      else
	{
	  // inside the given stripe	
	  if((yt <= xt+R) && (yt >= xt-R)) { 
					
	    // coefficients of quadratic equation for distance d
	    aa = 2;
	    b = -2*xt - 4*R -2*yt;
	    c= xt*xt + yt*yt + R*R +2*xt*R +2*yt*R;
					
	    //- solution of quadratic equation
	    Ds=b*b-4*aa*c;
	    if(Ds>=0) {
	      d=(-b-sqrt(Ds))/(2*aa);
	      return d;
	    }
	  }
	  // else means outside given stripe
	  else{
	    if(yt < xt-R) {d = xt;return d;} 
	    if(yt > xt+R) {d = yt;return d;} 
	    // end for inside given angle check
	  }
				
	}// end of if angle approach or strip approach;
			
      // end for III quarter
    }
		
    // for y/=0 and x/=0
  } 
  
  return d; 
}

//------------------------------------------------

// union1: uses bounded approach in quadrant 1
// and bounded approach in quadrant 3
// arguments:
// xt, IN, first function value at current point,
// yt, IN, second function value at current point, 
// R1, IN, maximum radius of the bounded approach (quadrant 1)
// R2, IN, maximum radius of the bounded approach (quadrant 3)

double
uni_sardef_2(double xt, double yt, double R1,
	     double R2){	
  double d=0;
  double lim, lim2;
  double aa,b,c,Ds;

  if (fabs(xt)<1.0e-7 && fabs(yt)<1.0e-7) return 0.0;

  // case when xt=0 and yt!=0
  // or when xt!=0 and yt=0
  if (fabs(xt)<1.0e-7 && yt>0) {d=yt; return d;}
  if (fabs(xt)<1.0e-7 && yt<0) {d=0.0; return d;}
  if (fabs(yt)<1.0e-7 && xt>0) {d=xt; return d;}
  if (fabs(yt)<1.0e-7 && xt<0) {d=0.0; return d;}
	
	
  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1.0e-7 ) && (fabs(xt)>1.0e-7 )) { 	
		
    // II quarter
    if(yt > 0 && xt<0) {d=yt; return d;} 
		
    // IV quarter
    if(xt > 0 && yt < 0) {d=xt; return d;} 
		
    // I quarter
    if((xt > 0) && (yt > 0)) {
      lim=((xt-R1)*(xt-R1)) + ((yt-R1)*(yt-R1));
      // angle approach: outside the boundary
      if ( (xt <= R1) || (yt<=R1) || 
	   ((xt<=2*R1) && (yt<=2*R1) && ( lim <= R1*R1 ) ) )  {
				
	double xx = xt*xt;
	double yy = yt*yt;
	xx = xx/(4.0*R1);
	yy =yy/(4.0*R1);
				
	if((xx <= yt) && (yy <= xt)) { 
					
	  double c0 = xt*xt + yt*yt;
	  double c1 = 0.0;
	  double c2 = (-1/(2.0*R1))*(xt+yt+2.0*R1);
	  double c3 = (1/(2.0*R1));
	  double c4 = 1/(16.0*R1*R1);
					
	  struct retval sol;
	  double m;
		
	  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);
	  m = sol.value[3];
	  return m;
	}
	// else means outside given angle
	else{
	  if(xx>yt) {d = xt; return d;} 
	  if(yy>xt) {d = yt; return d;} 
	  // end for inside given angle check
	}
      }
      else
	{
	  // inside the given stripe	
	  if((yt <= xt+R1) && (yt >= xt-R1)) { 
					
	    // coefficients of quadratic equation for distance d
	    aa = 2;
	    b = -2*xt - 4*R1 -2*yt;
	    c= xt*xt + yt*yt + R1*R1 +2*xt*R1 +2*yt*R1;
					
	    //- solution of quadratic equation
	    Ds=b*b-4*aa*c;
	    if(Ds>=0) {
	      d=(-b-sqrt(Ds))/(2*aa);
	      return d;
	    }
	  }
	  // else means outside given stripe
	  else{
	    if(yt < xt-R1) {d = xt; return d;} 
	    if(yt > xt+R1) {d = yt; return d;} 
	    // end for inside given angle check
	  }
	}
    }
    // end for I quarter
		
    // definition of lim2
    lim2 = ((xt + 2*R2)*(xt+2*R2)) + ((yt + 2*R2)*(yt+2*R2));
		
    // III quarter
    if((xt < 0 ) && (yt < 0)) {
      // angle approach: inside the boundary
      if ( (xt >= -R2) || (yt >= -R2) || 
	   ((xt >= -2*R2) && (yt >= -2*R2) && (lim2 >= R2*R2))) {

	double xx = xt*xt;
	double yy = yt*yt;
	xx = -xx/(4.0*R2);	
	yy = -yy/(4.0*R2);

	if((xt <= yy) && (yt<=xx)){

	  double c0 = -(xt*xt + yt*yt);
	  double c1 = -4.0 * sqrt(R2) * (xt+yt);
	  double c2 = -4.0 * R2;
	  double c3 = -4.0*sqrt(R2);
	  double c4 = 1.0;

	  struct retval sol;
	  double m;

	  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);


	  m = sol.value[1];
	  m = -m*m;
	  return m;

	}// else means outside given angle
	else{
	  if(yt>xx)  {d = yt; return d;} 
	  if(yy<xt) {d = xt; return d;} 
	  // end for inside given angle check
	}//end of if inside angle
				
      }
      else
	{
	  // inside the given stripe	
	  if((yt <= xt+R2) && (yt >= xt-R2)) { 
					
	    // coefficients of quadratic equation for distance d
	    aa = 2;
	    b = -2*xt - 4*R2 -2*yt;
	    c= xt*xt + yt*yt + R2*R2 +2*xt*R2 +2*yt*R2;
					
	    //- solution of quadratic equation
	    Ds=b*b-4*aa*c;
	    if(Ds>=0) {
	      d=(-b-sqrt(Ds))/(2*aa);
	      return d;
	    }
	  }
	  // else means outside given stripe
	  else{
	    if(yt < xt-R2) {d = xt;return d;} 
	    if(yt > xt+R2) {d = yt;return d;} 
	    // end for inside given angle check
	  }
				
	}// end of if angle approach or strip approach;
			
      // end for III quarter
    }
		
    // for y/=0 and x/=0
  } 
  
  return d; 
}

//------------------------------------------------------

// union1: uses unbounded approach in quadrant 1
// and bounded approach in quadrant 3
// arguments:
// xt, IN, value of first function at current point
// yt, IN, value of second function at current point
// R, IN, maximum radius of the bounded approach (quadrant 3)
// alpha, IN, angle for the unbounded approach (quadrant 1)
// gxt, IN, gradient of first function at current point
// gyt, IN, gradient of second function at current point
// g, OUT, gradient of xt U yt

double
uni_sardef_and_gradient_1(const double xt, const double yt,
			  const double R, const double alpha,
			  const double gxt[3], const double gyt[3], 
			  double grad[3]){	
  double d=0;
  double lim2;
  double g,g2,gt;
  double aa,b,c,Ds;
  double d1,t;

  if (fabs(xt)<1.0e-7 && fabs(yt)<1.0e-7) {
    // no gradient defined, takes the average of both
    grad[0]=0.5*gxt[0] + 0.5*gyt[0];
    grad[1]=0.5*gxt[1] + 0.5*gyt[1];
    grad[2]=0.5*gxt[2] + 0.5*gyt[2];
    return 0.0;
  }

  // case when xt=0 and yt!=0
  // or when xt!=0 and yt=0
  if (fabs(xt)<1.0e-7 && yt>0) {
    d=yt; 
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];
    return d;
  }
  if (fabs(xt)<1.0e-7 && yt<0) {
    d=0.0;
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];
    return d;
  }
  if (fabs(yt)<1.0e-7 && xt>0) {
    d=xt;
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return d;
  }
  if (fabs(yt)<1.0e-7 && xt<0) {
    d=0.0;
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return d;
  }

  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1.0e-7 ) && (fabs(xt)>1.0e-7 )) { 	
	
    // II quarter
    if(yt > 0 && xt<0) {
      d=yt;
      grad[0]=gyt[0];
      grad[1]=gyt[1];
      grad[2]=gyt[2];
      return d;
    } 
		
    // IV quarter
    if(xt > 0 && yt < 0) {
      d=xt;
      grad[0]=gxt[0];
      grad[1]=gxt[1];
      grad[2]=gxt[2];
      return d;} 
		
    // I quarter
    if((xt > 0) && (yt > 0)) {
      g = 1.0 / (tan(alpha));
      g2 = tan(alpha);
      gt = xt/yt;
			
      if ((gt < g) && (gt>g2)){
		  double dxt, dyt;
	t=tan(alpha);
	aa = t*t + 2.0 * t - 1.0;
	b = -2.0 * t * (xt+yt);
	c = xt*xt + yt*yt;
	d1 = -b - sqrt(b*b - 4.0*aa*c); // +->-
	d1 = d1 / (2.0*aa);

	// gradient
	dxt = -2.0*xt + 2.0*t*d1;
	dxt = dxt / (2.0*d1*aa + b);
	dyt = -2.0*yt + 2.0*t*d1;
	dyt = dyt / (2.0*d1*aa + b);

	grad[0] = dxt*gxt[0] + dyt*gyt[0];
	grad[1] = dxt*gxt[1] + dyt*gyt[1];
	grad[2] = dxt*gxt[2] + dyt*gyt[2];

	return d1;
      }
      else if (gt>=g) {
	grad[0]=gxt[0];
	grad[1]=gxt[1];
	grad[2]=gxt[2];
	return xt;
      }
      else if (gt<=g2) {
	grad[0]=gyt[0];
	grad[1]=gyt[1];
	grad[2]=gyt[2];
	return yt;
      }
			
    }
    // end for I quarter
		
		
    // definition of lim2
    lim2 = ((xt + 2*R)*(xt+2*R)) + ((yt + 2*R)*(yt+2*R));
		
    // III quarter
    if((xt < 0 ) && (yt < 0)) {
			
      // angle approach: inside the boundary
      if ( (xt >= -R) || (yt >= -R) || 
	   ((xt >= -2*R) && (yt >= -2*R) && (lim2 >= R*R))) {

	double xx = xt*xt;
	double yy = yt*yt;
	xx = -xx/(4.0*R);
	yy = -yy/(4.0*R);

	if((xt <= yy) && (yt<=xx)){

	  double c0 = -(xt*xt + yt*yt);
	  double c1 = -4.0 * sqrt(R) * (xt+yt);
	  double c2 = -4.0 * R;
	  double c3 = -4.0*sqrt(R);
	  double c4 = 1.0;

	  struct retval sol;

	  double m;
	  double dxt,dyt;

	  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);


	  m = sol.value[1];

	  dxt = -2*m*(2*xt + 4*sqrt(R)*m);
	  dxt = dxt / (4*m*m*m - 12*sqrt(R)*m*m 
		       - 8*R*m - 4*sqrt(R)*(xt+yt));
	  
	  dyt = -2*m*(2*yt + 4*sqrt(R)*m);
	  dyt = dyt / (4*m*m*m - 12*sqrt(R)*m*m 
		       - 8*R*m - 4*sqrt(R)*(xt+yt));

	  grad[0]=dxt*gxt[0]+dyt*gyt[0];
	  grad[1]=dxt*gxt[1]+dyt*gyt[1];
	  grad[2]=dxt*gxt[2]+dyt*gyt[2];

	  m = -m*m;
	  return m;

	}// else means outside given angle
	else{
	  if(yt>xx) {
	    d = yt;
	    grad[0]=gyt[0];
	    grad[1]=gyt[1];
	    grad[2]=gyt[2];
	    return d;
	  } 
	  if(yy<xt) {
	    d = xt;
	    grad[0]=gxt[0];
	    grad[1]=gxt[1];
	    grad[2]=gxt[2];
	    return d;
	  } 
	  // end for inside given angle check
	}//end of if inside angle
				
      }
      else
	{
	  // inside the given stripe	
	  if((yt <= xt+R) && (yt >= xt-R)) { 
					
	    // coefficients of quadratic equation for distance d
	    aa = 2;
	    b = -2*xt - 4*R -2*yt;
	    c= xt*xt + yt*yt + R*R +2*xt*R +2*yt*R;
					
	    //- solution of quadratic equation
	    Ds=b*b-4*aa*c;
	    if(Ds>=0) {
			double dxt,dyt;
	      d=(-b-sqrt(Ds))/(2*aa);


	      // compute the gradient
	      dxt = 2*d - 2*xt - 2*R;
	      dxt = dxt / (4*d + (-2*xt - 4*R - 2*yt));
	      
	      dyt = 2*d - 2*yt - 2*R;
	      dyt = dyt / (4*d + (-2*xt - 4*R - 2*yt));

	      grad[0]=gxt[0]*dxt + gyt[0]*dyt;
	      grad[1]=gxt[1]*dxt + gyt[1]*dyt;
	      grad[2]=gxt[2]*dxt + gyt[2]*dyt;
	      return d;
	    }
	  }
	  // else means outside given stripe
	  else{
	    if(yt < xt-R) {
	      d = xt;
	      grad[0]=gxt[0];
	      grad[1]=gxt[1];
	      grad[2]=gxt[2];
	      return d;
	    } 
	    if(yt > xt+R) {
	      d = yt;
	      grad[0]=gyt[0];
	      grad[1]=gyt[1];
	      grad[2]=gyt[2];
	      return d;
	    } 
	    // end for inside given angle check
	  }
				
	}// end of if angle approach or strip approach;
			
      // end for III quarter
    }
		
    // for y/=0 and x/=0
  } 
  
  return d; 
}

//-----------------------------------------------

// union: uses bounded approach in quadrant 1
// and bounded approach in quadrant 3
// arguments:
// xt, IN, first function value at current point,
// yt, IN, second function value at current point, 
// R1, IN, maximum radius of the bounded approach (quadrant 1)
// R2, IN, maximum radius of the bounded approach (quadrant 3)
// gxt, IN, gradient of first function at current point
// gyt, IN, gradient of second function at current point
// g, OUT, gradient of xt U yt

double
uni_sardef_and_gradient_2(const double xt, const double yt,
			  const double R1, const double R2,
			  const double gxt[3], const double gyt[3],
			  double grad[3]){	
  double d=0;
  double lim, lim2;
  double aa,b,c,Ds;

  if (fabs(xt)<1.0e-7 && fabs(yt)<1.0e-7){
    // no gradient defined, takes the average of both
    grad[0]=0.5*gxt[0] + 0.5*gyt[0];
    grad[1]=0.5*gxt[1] + 0.5*gyt[1];
    grad[2]=0.5*gxt[2] + 0.5*gyt[2];
    return 0.0;
  }

  // case when xt=0 and yt!=0
  // or when xt!=0 and yt=0
  if (fabs(xt)<1.0e-7 && yt>0) {
    d=yt;
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];
    return d;
  }
  if (fabs(xt)<1.0e-7 && yt<0) {
    d=0.0;
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];
    return d;
  }
  if (fabs(yt)<1.0e-7 && xt>0) {
    d=xt;
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return d;
  }
  if (fabs(yt)<1.0e-7 && xt<0) {
    d=0.0;
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return d;
  }
	
	
  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1.0e-7 ) && (fabs(xt)>1.0e-7 )) { 	
		
    // II quarter
    if(yt > 0 && xt<0) {
      d=yt;
      grad[0]=gyt[0];
      grad[1]=gyt[1];
      grad[2]=gyt[2];
      return d;
    } 
		
    // IV quarter
    if(xt > 0 && yt < 0) {
      d=xt;
      grad[0]=gxt[0];
      grad[1]=gxt[1];
      grad[2]=gxt[2];
      return d;
    } 
		
    // I quarter
    if((xt > 0) && (yt > 0)) {
      lim=((xt-R1)*(xt-R1)) + ((yt-R1)*(yt-R1));
      // angle approach: outside the boundary
      if ( (xt <= R1) || (yt<=R1) || 
	   ((xt<=2*R1) && (yt<=2*R1) && ( lim <= R1*R1 ) ) )  {
				
	double xx = xt*xt;
	double yy=yt*yt;
	xx = xx/(4.0*R1);
	yy =yy/(4.0*R1);
				
	if((xx <= yt) && (yy <= xt)) { 
					
	  double c0 = xt*xt + yt*yt;
	  double c1 = 0.0;
	  double c2 = (-1/(2.0*R1))*(xt+yt+2.0*R1);
	  double c3 = (1/(2.0*R1));
	  double c4 = 1/(16.0*R1*R1);
					
	  struct retval sol;

	  double m;
	  double dxt, dyt;
					
	  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);
	  m = sol.value[3];

	  // compute the gradient
	  dxt = 0.5*(m*m);
	  dxt = dxt - 2*xt*R1;
	  dxt = dxt / ( (1/(4*R1))*m*m*m + 1.5*m*m - (xt+yt+2*R1)*m );

	  dyt = 0.5*(m*m);
	  dyt = dyt - 2*yt*R1;
	  dyt = dyt / ( (1/(4*R1))*m*m*m + 1.5*m*m - (xt+yt+2*R1)*m );

	  grad[0]=gxt[0]*dxt + gyt[0]*dyt;
	  grad[1]=gxt[1]*dxt + gyt[1]*dyt;
	  grad[2]=gxt[2]*dxt + gyt[2]*dyt;

	  return m;
	}
	// else means outside given angle
	else{
	  if(xx>yt) {
	    d = xt;
	    grad[0]=gxt[0];
	    grad[1]=gxt[1];
	    grad[2]=gxt[2];
	    return d;
	  } 
	  if(yy>xt) {
	    d = yt;
	    grad[0]=gyt[0];
	    grad[1]=gyt[1];
	    grad[2]=gyt[2];
	    return d;
	  } 
	  // end for inside given angle check
	}
      }
      else
	{
	  // inside the given stripe	
	  if((yt <= xt+R1) && (yt >= xt-R1)) { 
					
	    // coefficients of quadratic equation for distance d
	    aa = 2;
	    b = -2*xt - 4*R1 -2*yt;
	    c= xt*xt + yt*yt + R1*R1 +2*xt*R1 +2*yt*R1;
					
	    //- solution of quadratic equation
	    Ds=b*b-4*aa*c;
	    if(Ds>=0) {
			double dxt,dyt;
	      d=(-b-sqrt(Ds))/(2*aa);

	      // compute the gradient
	      dxt = 2*d - 2*xt - 2*R1;
	      dxt = dxt / (4*d + (-2*xt - 4*R1 - 2*yt));

	      dyt = 2*d - 2*yt - 2*R1;
	      dyt = dyt / (4*d + (-2*xt - 4*R1 - 2*yt));

	      grad[0]=gxt[0]*dxt + gyt[0]*dyt;
	      grad[1]=gxt[1]*dxt + gyt[1]*dyt;
	      grad[2]=gxt[2]*dxt + gyt[2]*dyt;

	      return d;
	    }
	  }
	  // else means outside given stripe
	  else{
	    if(yt < xt-R1) {
	      d = xt;
	      grad[0]=gxt[0];
	      grad[1]=gxt[1];
	      grad[2]=gxt[2];
	      return d;
	    } 
	    if(yt > xt+R1) {
	      d = yt;
	      grad[0]=gyt[0];
	      grad[1]=gyt[1];
	      grad[2]=gyt[2];
	      return d;
	    } 
	    // end for inside given angle check
	  }
	}
    }
    // end for I quarter
		
    // definition of lim2
    lim2 = ((xt + 2*R2)*(xt+2*R2)) + ((yt + 2*R2)*(yt+2*R2));
		
    // III quarter
    if((xt < 0 ) && (yt < 0)) {
      // angle approach: inside the boundary
      if ( (xt >= -R2) || (yt >= -R2) || 
	   ((xt >= -2*R2) && (yt >= -2*R2) && (lim2 >= R2*R2))) {

	double xx = xt*xt;
	double yy=yt*yt;

	xx = -xx/(4.0*R2);
	yy = -yy/(4.0*R2);

	if((xt <= yy) && (yt<=xx)){

	  double c0 = -(xt*xt + yt*yt);
	  double c1 = -4.0 * sqrt(R2) * (xt+yt);
	  double c2 = -4.0 * R2;
	  double c3 = -4.0*sqrt(R2);
	  double c4 = 1.0;

	  struct retval sol;

	  double m;

	  double dxt,dyt;

	  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);


	  m = sol.value[1];

	  dxt = -2*m*(2*xt + 4*sqrt(R2)*m);
	  dxt = dxt / (4*m*m*m - 12*sqrt(R2)*m*m 
		       - 8*R2*m - 4*sqrt(R2)*(xt+yt));
	  
	  dyt = -2*m*(2*yt + 4*sqrt(R2)*m);
	  dyt = dyt / (4*m*m*m - 12*sqrt(R2)*m*m 
		       - 8*R2*m - 4*sqrt(R2)*(xt+yt));

	  grad[0]=dxt*gxt[0]+dyt*gyt[0];
	  grad[1]=dxt*gxt[1]+dyt*gyt[1];
	  grad[2]=dxt*gxt[2]+dyt*gyt[2];

	  m = -m*m;
	  return m;

	}// else means outside given angle
	else{
	  if(yt>xx) {
	    d = yt;
	    grad[0]=gyt[0];
	    grad[1]=gyt[1];
	    grad[2]=gyt[2];
	    return d;
	  } 
	  if(yy<xt) {
	    d = xt;
	    grad[0]=gxt[0];
	    grad[1]=gxt[1];
	    grad[2]=gxt[2];
	    return d;
	  } 
	  // end for inside given angle check
	}//end of if inside angle
				
      }
      else
	{
	  // inside the given stripe	
	  if((yt <= xt+R2) && (yt >= xt-R2)) { 
					
	    // coefficients of quadratic equation for distance d
	    aa = 2;
	    b = -2*xt - 4*R2 -2*yt;
	    c= xt*xt + yt*yt + R2*R2 +2*xt*R2 +2*yt*R2;
					
	    //- solution of quadratic equation
	    Ds=b*b-4*aa*c;
	    if(Ds>=0) {
			double dxt,dyt;
	      d=(-b-sqrt(Ds))/(2*aa);

	      // compute the gradient
	      dxt = 2*d - 2*xt - 2*R2;
	      dxt = dxt / (4*d + (-2*xt - 4*R2 - 2*yt));
	      
	      dyt = 2*d - 2*yt - 2*R2;
	      dyt = dyt / (4*d + (-2*xt - 4*R2 - 2*yt));

	      grad[0]=gxt[0]*dxt + gyt[0]*dyt;
	      grad[1]=gxt[1]*dxt + gyt[1]*dyt;
	      grad[2]=gxt[2]*dxt + gyt[2]*dyt;

	      return d;
	    }
	  }
	  // else means outside given stripe
	  else{
	    if(yt < xt-R2) {
	      d = xt;
	      grad[0]=gxt[0];
	      grad[1]=gxt[1];
	      grad[2]=gxt[2];
	      return d;
	    } 
	    if(yt > xt+R2) {
	      d = yt;
	      grad[0]=gyt[0];
	      grad[1]=gyt[1];
	      grad[2]=gyt[2];
	      return d;} 
	    // end for inside given angle check
	  }
				
	}// end of if angle approach or strip approach;
			
      // end for III quarter
    }
		
    // for y/=0 and x/=0
  } 
  
  return d; 
}

//-------------------------------------------------------------

// intersection1: uses bounded approach in quadrant 1
// and unbounded approach in quadrant 3
// arguments:
// xt, IN, value of first function at current point,
// yt, IN, value of first function at current point,
// R, IN, maximum radius of the bounded approach (quadrant 1),
// alpha, IN, angle for the unbounded approach (quadrant 3)

double 
int_sardef_1 (double xt, double yt,
			  double R, double alpha)
{
  double d=0;
  double lim, lim2, g, g2, gt, aa, b, c;
  double Ds;

  double d2;
  double t;

  if(fabs(xt)<1e-07 &&
     fabs(yt)<1e-07){
    return 0.0;
  }
	
  // case where xt=0 and yt!=0
  // 		   or xt!=0 and yt=0
  if(fabs(yt)<1e-7 && xt>0){
    return 0.0;
  }
  if(fabs(yt)<1e-7 && xt<0){
    return xt;
  }
  if(fabs(xt)<1e-7 && yt>0){
    return 0.0;
  }
  if(fabs(xt)<1e-7 && yt<0){
    return yt;
  }
	
  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1e-7) && (fabs(xt)>1e-7))
    {  
      // II quarter
      if(xt < 0 && yt > 0) {
	return xt;
      }
		
      // IV quarter
      if(yt < 0 && xt > 0) {
	return yt;
      }
		
      // I quarter
      if((xt > 0) && (yt > 0)) 
	{
	  // equation of the circle centered in 2R,2R
	  // used for the boundary condition to know whether to apply
	  // the fixed radius approach or the angle approach
	  lim=((xt-2*R)*(xt-2*R)) + ((yt-2*R)*(yt-2*R)); 
	  // angle approach: outside the boundary
	  if ( (xt <= R) || (yt<=R) 
	       || ((xt<=2*R) && (yt<=2*R) && ( lim >= R*R ) ) ) 
	    { 
	      double xx=xt*xt;
		  double yy=yt*yt;
	      xx=xx/(4.0*R);
	      yy=yy/(4.0*R);		

	      // inside the given angle
	      if((xx <= yt) && (yy <= xt))
		{ 
		  // coefficients of quadratic equation for distance d
		  double c0= -(xt*xt + yt*yt);
		  double c1= 4.0*(xt+yt)*sqrt(R);
		  double c2= -4.0*R;
		  double c3=-4.0*sqrt(R);
		  double c4= 1.0;
					
		  struct retval sol;

		  double m;
		  //- solution of quadratic equation
		  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);

		  m = sol.value[1];
		  m=m*m;
		  return m;
		}
	      // else means outside given angle
	      else {
		if(xx>yt) {
		  return yt;
		} 
		if(yy>xt) {
		  return xt;
		}
		// end for inside given angle check
	      }
	    }
	  // inside the boundary: apply fixed radius circle
	  else {
	    // inside the given stripe	
	    if((yt <= xt+R) && (yt >= xt-R)) { 
					
	      // coefficients of quadratic equation for distance d
	      aa = 2;
	      b = -2*xt + 4*R -2*yt;
	      c= xt*xt + yt*yt + R*R -2*xt*R -2*yt*R;
					
	      //- solution of quadratic equation
	      Ds=b*b-4*aa*c;
	      if(Ds>=0) {
		d=(-b+sqrt(Ds))/(2*aa);
		return d;
	      }
	    }
	    // else means outside given stripe
	    else {
	      if(yt < xt-R) {
		return yt;
	      }
	      if(yt > xt+R) {
		return xt;
	      }
	    }// end for inside given angle check
	  }
	} // end for I quarter
      // definition of lim2
      lim2 = ((xt + R)*(xt+R)) + ((yt + R)*(yt+R));
	
      // III quarter
      if((xt < 0 ) && (yt < 0)) {
	// if alpha == 0 then g is NaN and g2 is 0
	// the first if() is always true
	g = 1.0 / (tan(alpha));
	g2 = tan(alpha);
	gt = xt/yt;

	if ((gt<g) && (gt>g2)){
	  t = tan(alpha);
	  aa = 1.0 - t*t - 2.0*t;
	  b = 2.0*t*(xt+yt);
	  c = -xt*xt - yt*yt;

	  Ds = b*b - 4.0*aa*c;
	  //d1 = (-b + sqrt(Ds)) / (2.0*aa);
	  d2 = (-b - sqrt(Ds)) / (2.0*aa);
	  d = d2;

	  return d;
	}
	else if (gt>=g) {
	  return xt;
	}
	else if (gt<=g2) {
	  return yt;
	}
      }// end for III quarter
      // for y/=0 and x/=0
    } 
  return d; 
}

//---------------------------------------------------------

// intersection2: uses bounded approach in quadrant 1
// and bounded approach in quadrant 3
// arguments:
// xt, IN, value of first function at current point,
// yt, IN, value of first function at current point,
// R1, IN, maximum radius of the bounded approach (quadrant 1),
// R2, IN, maximum radius of the bounded approach (quadrant 3)

double 
int_sardef_2 (double xt, double yt,
	      double R1, double R2)
{
  double d=0;
  double lim, lim2, aa, b, c;
  double Ds;

  if(fabs(xt)<1e-07 &&
     fabs(yt)<1e-07){
    return 0.0;
  }
	
  // case where xt=0 and yt!=0
  // 		   or xt!=0 and yt=0
  if(fabs(yt)<1e-7 && xt>0){
    return 0.0;
  }
  if(fabs(yt)<1e-7 && xt<0){
    return xt;
  }
  if(fabs(xt)<1e-7 && yt>0){
    return 0.0;
  }
  if(fabs(xt)<1e-7 && yt<0){
    return yt;
  }
	
  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1e-7) && (fabs(xt)>1e-7))
    {  
      // II quarter
      if(xt < 0 && yt > 0) {
	return xt;
      }
		
      // IV quarter
      if(yt < 0 && xt > 0) {
	return yt;
      }
		
      // I quarter
      if((xt > 0) && (yt > 0)) 
	{
	  // equation of the circle centered in 2R,2R
	  // used for the boundary condition to know whether to apply
	  // the fixed radius approach or the angle approach
	  lim=((xt-2*R1)*(xt-2*R1)) + ((yt-2*R1)*(yt-2*R1)); 
	  // angle approach: outside the boundary
	  if ( (xt <= R1) || (yt<=R1) 
	       || ((xt<=2*R1) && (yt<=2*R1) && ( lim >= R1*R1 ) ) ) 
	    { 
	      double xx=xt*xt;
		  double yy=yt*yt;
	      xx=xx/(4.0*R1);
	      yy=yy/(4.0*R1);		

	      // inside the given angle
	      if((xx <= yt) && (yy <= xt))
		{ 
		  // coefficients of quadratic equation for distance d
		  double c0= -(xt*xt + yt*yt);
		  double c1= 4.0*(xt+yt)*sqrt(R1);
		  double c2= -4.0*R1;
		  double c3=-4.0*sqrt(R1);
		  double c4= 1.0;
					
		  struct retval sol;
		  double m;
		  //- solution of quadratic equation
		  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);

		  m = sol.value[1];
		  m=m*m;
		  return m;
		}
	      // else means outside given angle
	      else {
		if(xx>yt) {
		  return yt;
		} 
		if(yy>xt) {
		  return xt;
		}
		// end for inside given angle check
	      }
	    }
	  // inside the boundary: apply fixed radius circle
	  else {
	    // inside the given stripe	
	    if((yt <= xt+R1) && (yt >= xt-R1)) { 
					
	      // coefficients of quadratic equation for distance d
	      aa = 2;
	      b = -2*xt + 4*R1 -2*yt;
	      c= xt*xt + yt*yt + R1*R1 -2*xt*R1 -2*yt*R1;
					
	      //- solution of quadratic equation
	      Ds=b*b-4*aa*c;
	      if(Ds>=0) {
		d=(-b+sqrt(Ds))/(2*aa);
		return d; // avoid unecessary later checks
	      }
	    }
	    // else means outside given stripe
	    else {
	      if(yt < xt-R1) {
		return yt;
	      }
	      if(yt > xt+R1) {
		return xt;
	      }
	    }// end for inside given angle check
	  }
	} // end for I quarter
      // definition of lim2
      lim2 = ((xt + R2)*(xt+R2)) + ((yt + R2)*(yt+R2));
	
      // III quarter
      if((xt < 0 ) && (yt < 0)) {
	// angle approach: inside the boundary
	if ( (xt >= -R2) || (yt >= -R2) 
	     || ((xt >= -2*R2) && (yt >= -2*R2) && (lim2 <= R2*R2 ))) {

	  double xx = xt*xt;
	  double yy=yt*yt;
	  xx = -xx/(4.0*R2);
	  yy = -yy/(4.0*R2);

	  // inside the given angle in III quarter
	  if((yt <= xx) && (xt<=yy)) { 
		
	    // coefficients for quartic eqution in d
	    double c0 = (xt*xt + yt*yt);
	    double c2 = (1.0/(2.0*R2))*((xt+yt-2.0*R2));
	    double c1 = 0.0;
	    double c3 = -1.0/(2.0*R2);
	    double c4 = 1.0/(16.0*R2*R2);

	    struct retval sol;
		double m;
	    calc_roots_of_quartic (&sol,c4,c3,c2,c1,c0);

	    m = sol.value[2];
	    //m = -m;

	    return m; // avoid later unecessary checks
	  }
	  // else means outside given angle
	  else {
	    if(yt>xx) {
	      d = xt;
	      return d;
	    }
	    if(xt>yy) {
	      d = yt;
	      return d;
	    } 
	  }// end for inside given angle check
	  //end of if inside angle
	}
		
	else
	  {
	    // inside the given stripe	
	    if((yt <= xt+R2) && (yt >= xt-R2)) { 
					
	      // coefficients of quadratic equation for distance d
	      aa = 2;
	      b = -2*xt + 4*R2 -2*yt;
	      c= xt*xt + yt*yt + R2*R2 -2*xt*R2 -2*yt*R2;
					
	      //- solution of quadratic equation
	      Ds=b*b-4*aa*c;
	      if(Ds>=0){ 
		d=(-b+sqrt(Ds))/(2*aa);
		return d;
	      }
	    }
	    // else means outside given stripe
	    else{
	      if(yt < xt-R2) {
		d = yt; 
		return d;
	      } 
	      if(yt > xt+R2) {
		d = xt;
		return d;
	      } 
	    }
	  }// end for inside given angle check
			
	// end of if angle approach or strip approach;
	
	//}
      }// end for III quarter
  
      // for y/=0 and x/=0
    } 

  return d; 
}

//-------------------------------------------------------------

// intersection and gradient: uses bounded approach in quadrant 1
// and unbounded approach in quadrant 3
// arguments:
// xt, IN, value of first function at current point,
// yt, IN, value of second function at current point,
// R, IN, maximum radius of the bounded approach (quadrant 1),
// alpha, IN, angle for the unbounded approach (quadrant 3)
// gxt[3], IN, gradient of the first function at current point, 
// gyt[3], IN, gradient of the second function at current point, 
// g[3], OUT, gradient of the intersection at the current point, 

double 
int_sardef_and_gradient_1 (const double xt, const double yt,
			   const double R, const double alpha,
			   const double gxt[3], const double gyt[3],
			   double grad[3])
{
  double d=0;
  double lim, lim2, g, g2, gt, aa, b, c;
  double Ds;

  double d2;
  double t;

  if(fabs(xt)<1e-07 &&
     fabs(yt)<1e-07){
    grad[0]=0.5*gxt[0]+0.5*gyt[0];
    grad[1]=0.5*gxt[1]+0.5*gyt[1];
    grad[2]=0.5*gxt[2]+0.5*gyt[2];
    return 0.0;
  }
	
  // case where xt=0 and yt!=0
  // 		   or xt!=0 and yt=0
  if(fabs(yt)<1e-7 && xt>0){
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];
    return 0.0;
  }
  if(fabs(yt)<1e-7 && xt<0){
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return xt;
  }
  if(fabs(xt)<1e-7 && yt>0){
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return 0.0;
  }
  if(fabs(xt)<1e-7 && yt<0){
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];
    return yt;
  }
	
  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1e-7) && (fabs(xt)>1e-7))
    {  
      // II quarter
      if(xt < 0 && yt > 0) {
	grad[0]=gxt[0];
	grad[1]=gxt[1];
	grad[2]=gxt[2];

	return xt;
      }
		
      // IV quarter
      if(yt < 0 && xt > 0) {
	grad[0]=gyt[0];
	grad[1]=gyt[1];
	grad[2]=gyt[2];

	return yt;
      }
		
      // I quarter
      if((xt > 0) && (yt > 0)) 
	{
	  // equation of the circle centered in 2R,2R
	  // used for the boundary condition to know whether to apply
	  // the fixed radius approach or the angle approach
	  lim=((xt-2*R)*(xt-2*R)) + ((yt-2*R)*(yt-2*R)); 
	  // angle approach: outside the boundary
	  if ( (xt <= R) || (yt<=R) 
	       || ((xt<=2*R) && (yt<=2*R) && ( lim >= R*R ) ) ) 
	    { 
	      double xx=xt*xt;
		  double yy=yt*yt;
	      xx=xx/(4.0*R);
	      yy=yy/(4.0*R);		

	      // inside the given angle
	      if((xx <= yt) && (yy <= xt))
		{ 
		  // coefficients of quadratic equation for distance d
		  double c0= -(xt*xt + yt*yt);
		  double c1= 4.0*(xt+yt)*sqrt(R);
		  double c2= -4.0*R;
		  double c3=-4.0*sqrt(R);
		  double c4= 1.0;
					
		  struct retval sol;

		  double m;
		  double dxt, dyt;
		  //- solution of quadratic equation
		  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);

		  m = sol.value[1];

		  dxt = 2 * m * (2 * xt - 4 * sqrt(R) * m);
		  dxt = dxt / (4 * m*m*m - 12 * sqrt(R) * m*m - 8 * R * m
				 + 4 * sqrt(R)*(xt+yt));
		  dyt = 2*m*(2*yt - 4*sqrt(R)*m);
		  dyt = dyt / (4 * m*m*m - 12 * sqrt(R) * m*m - 8 * R * m
				 + 4 * sqrt(R)*(xt+yt));

		  grad[0] = dxt * gxt[0] + dyt * gyt[0];
		  grad[1] = dxt * gxt[1] + dyt * gyt[1];
		  grad[2] = dxt * gxt[2] + dyt * gyt[2];

		  m=m*m;
		  return m;
		}
	      // else means outside given angle
	      else {
		if(xx>yt) {
		  grad[0] = gyt[0];
		  grad[1] = gyt[1];
		  grad[2] = gyt[2];

		  return yt;
		} 
		if(yy>xt) {
		  grad[0] = gxt[0];
		  grad[1] = gxt[1];
		  grad[2] = gxt[2];
						
		  return xt;
		}
		// end for inside given angle check
	      }
	    }
	  // inside the boundary: apply fixed radius circle
	  else {
	    // inside the given stripe	
	    if((yt <= xt+R) && (yt >= xt-R)) { 
					
	      // coefficients of quadratic equation for distance d
	      aa = 2;
	      b = -2*xt + 4*R -2*yt;
	      c= xt*xt + yt*yt + R*R -2*xt*R -2*yt*R;
					
	      //- solution of quadratic equation
	      Ds=b*b-4*aa*c;
	      if(Ds>=0) {
			  double dxt,dyt;
		d=(-b+sqrt(Ds))/(2*aa);

		dxt = 2 *d - 2 *(xt - R);
		dxt = dxt / (4*d + (4*R - 2*xt - 2*yt));

		dyt = 2 * d - 2 * (yt - R);
		dyt = dyt / (4*d + (4*R - 2*xt - 2*yt));

		grad[0] = dxt * gxt[0] + dyt * gyt[0];
		grad[1] = dxt * gxt[1] + dyt * gyt[1];
		grad[2] = dxt * gxt[2] + dyt * gyt[2];

		return d; // avoid unecessary later checks
	      }
	    }
	    // else means outside given stripe
	    else {
	      if(yt < xt-R) {
		grad[0] = gyt[0];
		grad[1] = gyt[1];
		grad[2] = gyt[2];
						
		return yt;
	      }
	      if(yt > xt+R) {
		grad[0] = gxt[0];
		grad[1] = gxt[1];
		grad[2] = gxt[2];

		return xt;
	      }
	    }// end for inside given angle check
	  }
	} // end for I quarter
      // definition of lim2
      lim2 = ((xt + R)*(xt+R)) + ((yt + R)*(yt+R));
	
      // III quarter
      if((xt < 0 ) && (yt < 0)) {
	g = 1.0 / (tan(alpha));
	g2 = tan(alpha);
	gt = xt/yt;

	if ((gt<g) && (gt>g2)){
		double dxt,dyt;
	  t = tan(alpha);
	  aa = 1.0 - t*t - 2.0*t;
	  b = 2.0*t*(xt+yt);
	  c = -xt*xt - yt*yt;

	  Ds = b*b - 4.0*aa*c;
	  //d1 = (-b + sqrt(Ds)) / (2.0*aa);
	  d2 = (-b - sqrt(Ds)) / (2.0*aa);
	  d = d2;

	  dxt = 2*d*t - 2*xt;
	  dxt = dxt / (2*d*(t*t + 2*t - 1) - 2 *(xt+yt) * t);
	  dyt = 2*d*t - 2*yt;
	  dyt = dyt / (2*d*(t*t + 2*t - 1) - 2*(xt+yt)*t);

	  grad[0] = dxt * gxt[0] + dyt * gyt[0];
	  grad[1] = dxt * gxt[1] + dyt * gyt[1];
	  grad[2] = dxt * gxt[2] + dyt * gyt[2];

	  return d;
	}
	else if (gt>=g) {

	  grad[0] = gxt[0];
	  grad[1] = gxt[1];
	  grad[2] = gxt[2];

	  return xt;
	}
	else if (gt<=g2) {

	  grad[0] = gyt[0];
	  grad[1] = gyt[1];
	  grad[2] = gyt[2];

	  return yt;
	}
      }// end for III quarter
		
      // for y/=0 and x/=0
    } 
	
  return d; 
}

//-------------------------------------------------------------

// intersection and gradient: uses bounded approach in quadrant 1
// and bounded approach in quadrant 3
// arguments:
// xt, IN, value of first function at current point,
// yt, IN, value of second function at current point,
// R1, IN, maximum radius of the bounded approach (quadrant 1),
// R2, IN, angle for the unbounded approach (quadrant 3)
// gxt[3], IN, gradient of the first function
// gyt[3], IN, gradient of the second function
// g[3], OUT, gradient of the intersection

double 
int_sardef_and_gradient_2 (const double xt, const double yt,
	    const double R1, const double R2,
	    const double gxt[3], const double gyt[3], double grad[3])
{
  double d=0;
  double lim, lim2, aa, b, c;
  double Ds;

  if(fabs(xt)<1e-07 &&
     fabs(yt)<1e-07){
    grad[0]=0.5*gxt[0]+0.5*gyt[0];
    grad[1]=0.5*gxt[1]+0.5*gyt[1];
    grad[2]=0.5*gxt[2]+0.5*gyt[2];
    return 0.0;
  }
	
  // case where xt=0 and yt!=0
  // 		   or xt!=0 and yt=0
  if(fabs(yt)<1e-7 && xt>0){
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];
    return 0.0;
  }
  if(fabs(yt)<1e-7 && xt<0){
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return xt;
  }
  if(fabs(xt)<1e-7 && yt>0){
    grad[0]=gxt[0];
    grad[1]=gxt[1];
    grad[2]=gxt[2];
    return 0.0;
  }
  if(fabs(xt)<1e-7 && yt<0){
    grad[0]=gyt[0];
    grad[1]=gyt[1];
    grad[2]=gyt[2];

    return yt;
  }
	
  // theta analysis for (x,y) point, gt=ctg(theta)
  if((fabs(yt)>1e-7) && (fabs(xt)>1e-7))
    {  
      // II quarter
      if(xt < 0 && yt > 0) {
	grad[0]=gxt[0];
	grad[1]=gxt[1];
	grad[2]=gxt[2];

	return xt;
      }
		
      // IV quarter
      if(yt < 0 && xt > 0) {
	grad[0]=gyt[0];
	grad[1]=gyt[1];
	grad[2]=gyt[2];

	return yt;
      }
		
      // I quarter
      if((xt > 0) && (yt > 0)) 
	{
	  // equation of the circle centered in 2R,2R
	  // used for the boundary condition to know whether to apply
	  // the fixed radius approach or the angle approach
	  lim=((xt-2*R1)*(xt-2*R1)) + ((yt-2*R1)*(yt-2*R1)); 
	  // angle approach: outside the boundary
	  if ( (xt <= R1) || (yt<=R1) 
	       || ((xt<=2*R1) && (yt<=2*R1) && ( lim >= R1*R1 ) ) ) 
	    { 
	      double xx=xt*xt;
		  double yy=yt*yt;
	      xx=xx/(4.0*R1);
	      yy=yy/(4.0*R1);		

	      // inside the given angle
	      if((xx <= yt) && (yy <= xt))
		{ 
		  // coefficients of quadratic equation for distance d
		  double c0= -(xt*xt + yt*yt);
		  double c1= 4.0*(xt+yt)*sqrt(R1);
		  double c2= -4.0*R1;
		  double c3=-4.0*sqrt(R1);
		  double c4= 1.0;
					
		  struct retval sol;

		  double m;
		  double dxt,dyt;
		  //- solution of quadratic equation
		  calc_roots_of_quartic(&sol,c4,c3,c2,c1,c0);

		  m = sol.value[1];

		  dxt = 2 * m * (2 * xt - 4 * sqrt(R1) * m);
		  dxt = dxt / (4 * m*m*m - 12 * sqrt(R1) * m*m - 8 * R1 * m
				 + 4 * sqrt(R1)*(xt+yt));
		  dyt = 2*m*(2*yt - 4*sqrt(R1)*m);
		  dyt = dyt / (4 * m*m*m - 12 * sqrt(R1) * m*m - 8 * R1 * m
				 + 4 * sqrt(R1)*(xt+yt));

		  grad[0] = dxt * gxt[0] + dyt * gyt[0];
		  grad[1] = dxt * gxt[1] + dyt * gyt[1];
		  grad[2] = dxt * gxt[2] + dyt * gyt[2];

		  m=m*m;
		  return m;
		}
	      // else means outside given angle
	      else {
		if(xx>yt) {
		  grad[0] = gyt[0];
		  grad[1] = gyt[1];
		  grad[2] = gyt[2];

		  return yt; // avoid later unecessary checks
		} 
		if(yy>xt) {
		  grad[0] = gxt[0];
		  grad[1] = gxt[1];
		  grad[2] = gxt[2];
						
		  return xt; //avoid later unecessary checks 
		}
		// end for inside given angle check
	      }
	    }
	  // inside the boundary: apply fixed radius circle
	  else {
	    // inside the given stripe	
	    if((yt <= xt+R1) && (yt >= xt-R1)) { 
					
	      // coefficients of quadratic equation for distance d
	      aa = 2;
	      b = -2*xt + 4*R1 -2*yt;
	      c= xt*xt + yt*yt + R1*R1 -2*xt*R1 -2*yt*R1;
					
	      //- solution of quadratic equation
	      Ds=b*b-4*aa*c;
	      if(Ds>=0) {
			  double dxt,dyt;
		d=(-b+sqrt(Ds))/(2*aa);

		dxt = 2 *d - 2 *(xt - R1);
		dxt = dxt / (4*d + (4*R1 - 2*xt - 2*yt));

		dyt = 2 * d - 2 * (yt - R1);
		dyt = dyt / (4*d + (4*R1 - 2*xt - 2*yt));

		grad[0] = dxt * gxt[0] + dyt * gyt[0];
		grad[1] = dxt * gxt[1] + dyt * gyt[1];
		grad[2] = dxt * gxt[2] + dyt * gyt[2];

		return d; // avoid unecessary later checks
	      }
	    }
	    // else means outside given stripe
	    else {
	      if(yt < xt-R1) {
		grad[0] = gyt[0];
		grad[1] = gyt[1];
		grad[2] = gyt[2];
						
		return yt;
	      }
	      if(yt > xt+R1) {
		grad[0] = gxt[0];
		grad[1] = gxt[1];
		grad[2] = gxt[2];

		return xt;
	      }
	    }// end for inside given angle check
	  }
	} // end for I quarter
      // definition of lim2
      lim2 = ((xt + R2)*(xt+R2)) + ((yt + R2)*(yt+R2));
	
      // III quarter
      if((xt < 0 ) && (yt < 0)) {
	// angle approach: inside the boundary
	if ( (xt >= -R2) || (yt >= -R2) 
	     || ((xt >= -2*R2) && (yt >= -2*R2) && (lim2 <= R2*R2 ))) {
	  double xx = xt*xt;
	  double yy = yt*yt;
	  xx = -xx/(4.0*R2);
	  yy = -yy/(4.0*R2);
	  
	  // inside the given angle in III quarter
	  if((yt <= xx) && (xt<=yy)) { 
	    
	    // coefficients for quartic eqution in d
	    double c0 = (xt*xt + yt*yt);
	    double c2 = (1.0/(2.0*R2))*((xt+yt-2.0*R2));
	    double c1 = 0.0;
	    double c3 = -1.0/(2.0*R2);
	    double c4 = 1.0/(16.0*R2*R2);
	    
	    struct retval sol;
	    
		double m;
		double dxt, dyt;

	    calc_roots_of_quartic (&sol,c4,c3,c2,c1,c0);
	    
	    m = sol.value[2];
	    //m = -m;

	    dxt = 2.0*xt*R2 - 0.5*m*m;
	    dxt = dxt / (1/R2 * 0.25 * m*m*m - 1.5*m*m + (xt+yt-2*R2)*m);
	    dyt = 2.0*yt*R2 - 0.5*m*m;
	    dyt = dyt / (1/R2 * 0.25 * m*m*m - 1.5*m*m + (xt+yt-2*R2)*m);
	    
	    grad[0]=dxt*gxt[0]+dyt*gyt[0];
	    grad[1]=dxt*gxt[1]+dyt*gyt[1];
	    grad[2]=dxt*gxt[2]+dyt*gyt[2];

	    return m; // avoid later unecessary checks
	  }
	  // else means outside given angle
	  else {
	    if(yt>xx) {
	      grad[0]=gxt[0];
	      grad[1]=gxt[1];
	      grad[2]=gxt[2];
	      return xt;
	    }
	    if(xt>yy) {
	      grad[0]=gyt[0];
	      grad[1]=gyt[1];
	      grad[2]=gyt[2];
	      return yt;
	    } 
	  }// end for inside given angle check
	  //end of if inside angle
	}
			
	else
	  {
	    // inside the given stripe	
	    if((yt <= xt+R2) && (yt >= xt-R2)) { 
					
	      // coefficients of quadratic equation for distance d
	      aa = 2;
	      b = -2*xt + 4*R2 -2*yt;
	      c= xt*xt + yt*yt + R2*R2 -2*xt*R2 -2*yt*R2;
	      
	      //- solution of quadratic equation
	      Ds=b*b-4*aa*c;
	      if(Ds>=0){ 
			  double dxt,dyt;
		d=(-b+sqrt(Ds))/(2*aa);

		dxt = 2*(d - (xt-R2));
		dxt = dxt / (4*d + (-2*xt + 4*R2 - 2*yt));
		dyt = 2*(d - (yt-R2));
		dyt = dyt / (4*d + (-2*xt + 4*R2 - 2*yt));

		grad[0]=dxt*gxt[0]+dyt*gyt[0];
		grad[1]=dxt*gxt[1]+dyt*gyt[1];
		grad[2]=dxt*gxt[2]+dyt*gyt[2];

		return d;
	      }
	    }
	    // else means outside given stripe
	    else{
	      if(yt < xt-R2) {
		grad[0]=gyt[0];
		grad[1]=gyt[1];
		grad[2]=gyt[2];
		return yt;
	      } 
	      if(yt > xt+R2) {
		grad[0]=gxt[0];
		grad[1]=gxt[1];
		grad[2]=gxt[2];
		return xt;
	      } 
	    }
	  }// end for inside given angle check
			
	// end of if angle approach or strip approach;

      }// end for III quarter
      
      // for y/=0 and x/=0
    } 
  
  return d; 
}

