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

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

// signum function
static int sign(double x)
{
  return (x == 0 ? 0 : (x < 0 ? -1 : 1));
}


// returns a / b (0 / 0 = 0, overflow = LARGE_VAL with correct sign)
static double fdiv(double a, double b)
{
  if (b == 0)
    {
      if (a == 0) return 0;
      else return LARGE_VAL * sign(a);
    }
  else
    {
      if (a == 0) return 0;
      else
	{
	  if ((a + b) == a) return LARGE_VAL * sign(a) * sign(b);
	  else return a / b;
	}
    }
}


/* gnu libm already has cbrt, not VC */
#if defined _MSC_VER
static double cbrt (double a)
{
  return pow(a,1.0/3.0);
}
#endif


// root of linear equation c1 * x + c0 = 0
// if successful, returns ret.value[0] = x
static void calc_root_of_linear(struct retval  *ret, double c1, double c0)
{
  if (c1 == 0)
    ret->num_val = 0;
  else
    {
      ret->num_val = 1;
      ret->value[0] = fdiv(-c0, c1);
    }
}

// distinct real roots x1, x2 of quadratic equation:
//   c2 * x^2 + c1 * x + c0 = 0
// if successful, returns ret.value[0] = x1 & ret.value[1] = x2
// if roots are equal, ret.value[0] = root
static void calc_roots_of_quadratic(struct retval *ret, double c2, 
									double c1, double c0)
{
  double d;

  if (c2 == 0)
    calc_root_of_linear(ret, c1, c0);
  else
    {
      d = c1 * c1 - 4 * c2 * c0;

      if (d == 0)
	{
	  ret->num_val = 1;
	  ret->value[0] = fdiv(-c1, 2 * c2);
	}
      else
	{
	  if (d > 0)
	    {
	      ret->num_val = 2;
		  ret->value[0] = fdiv(-c1 + sqrt(d), 2 * c2);
	      ret->value[1] = fdiv(-c1 - sqrt(d), 2 * c2);
	    }
	  else
	    ret->num_val = 0;
	}
    }
}


// real roots of cubic equation:
//   c3 * x^3 + c2 * x^2 + c1 + c0 = 0
// if successful, returns real roots in ret.value[]
static void calc_roots_of_cubic(struct retval  *ret, double c3, 
								double c2, double c1, double c0)
{
  static struct retval quad_rts;
  double k, p, q, w, cos3a, t, p_d3_e3, a, RAD_120, RAD_240;
  int i;

  if (c3 == 0)
    calc_roots_of_quadratic(ret, c2, c1, c0);
  else
    {
      if (c2 == 0 && c1 == 0)
	{
	  ret->num_val = 1;
	  ret->value[0] = cbrt(fdiv(-c0, c3));
	}
      else
	{
	  if (c0 == 0)
	    {
	      ret->num_val = 1;
	      ret->value[0] = 0;
	      calc_roots_of_quadratic(&quad_rts, c3, c2, c1);
	      for (i = 0; i < quad_rts.num_val; i++)
		if (quad_rts.value[i] != 0)
		  ret->value[ret->num_val++] = quad_rts.value[i];
	    }
	  else
	    {
	      if (c2 != 0)
		{
		  k = fdiv(-c2, 3 * c3);
		  p = fdiv(3 * c3 * c1 - c2 * c2, -3 * c3 * c3);
		  q = fdiv(2 * c2 * c2 * c2 - 9 * c3 * c2 * c1 + 27 * c3 * c3 * c0, -27 * c3 * c3 * c3);
		}
	      else
		{
		  k = 0;
		  p = fdiv(-c1, c3);
		  q = fdiv(-c0, c3);
		}

	      p_d3_e3 = p * p / 27 * p;
	      w = q / 4 * q - p_d3_e3;

	      if (w < 0)
		{
		  cos3a = fdiv(q, 2 * sqrt(p_d3_e3));
		  a = acos(cos3a) / 3;
		  t = sqrt(p * 1.33333333333333);
		  ret->num_val = 3;
		  ret->value[0] = t * cos(a) + k;

		  RAD_120 = 2.0*PI;
		  RAD_120 = RAD_120/3.0;
		  RAD_240 = 4.0*PI;
		  RAD_240 = RAD_240/3.0;
		  ret->value[1] = t * cos(a + RAD_120) + k;
		  ret->value[2] = t * cos(a + RAD_240) + k;
		}
	      else
		{
		  if (w == 0)
		    {
		      ret->num_val = 1;
		      ret->value[0] = 2 * cbrt(q / 2) + k;
		    }
		  else
		    {
		      ret->num_val = 1;
		      ret->value[0] = cbrt(q / 2 + sqrt(w)) + cbrt(q / 2 - sqrt(w)) + k;
		    }
		}
	    }
	}
    }
}

// real roots of quartic equation:
//   c4 * x^4 + c3 * x^3 + c2 * x^2 + c1 + c0 = 0
// if successful, returns real roots in ret.value[]
void calc_roots_of_quartic(struct retval  *ret, double c4, 
						   double c3, double c2, 
						   double c1, double c0)
{
  static struct retval subpoly_rts[2];
  double m, b, a3, a2, a1, a0;
  int i, j;

  
  if (c0 == 0)
    {
      ret->num_val = 1;
      ret->value[0] = 0;
      calc_roots_of_cubic(&subpoly_rts[0], c4, c3, c2, c1);
      for (i = 0; i < subpoly_rts[0].num_val; i++)
	if (subpoly_rts[0].value[i] != 0)
	  ret->value[ret->num_val++] = subpoly_rts[0].value[i];
    }
  else
    {
      struct retval cb_rts;
	  double hi_root;
	  
	  a3 = fdiv(c3, c4);
      a2 = fdiv(c2, c4);
      a1 = fdiv(c1, c4);
      a0 = fdiv(c0, c4);
	  	  
      calc_roots_of_cubic(&cb_rts, -8, 4 * a2, 8 * a0 - 2 * a1 * a3, a1 * a1 - a0 * (4 * a2 - a3 * a3));
	  
      hi_root = cb_rts.value[0];
	  
      if (cb_rts.num_val == 2)
	{
	  if (cb_rts.value[1] > hi_root)
	    hi_root = cb_rts.value[1];
	}
      else if (cb_rts.num_val == 3)
	{
	  if (cb_rts.value[1] > cb_rts.value[2])
	    {
	      if (cb_rts.value[1] > hi_root)
		hi_root = cb_rts.value[1];
	    }
	  else
	    {
	      if (cb_rts.value[2] > hi_root)
		hi_root = cb_rts.value[2];
	    }
	}
	  
      ret->num_val = 0;
	  
      if ((a2 = hi_root * hi_root) > a0)
	{
	  b = sqrt(a2 - a0);
	  m = fdiv(a3 * hi_root - a1, 2 * b);
		  
	  calc_roots_of_quadratic(&subpoly_rts[0], 1, a3 / 2 + m, hi_root + b);
	  calc_roots_of_quadratic(&subpoly_rts[1], 1, a3 / 2 - m, hi_root - b);
		  
	  for (i = 0; i < 2; i++)
	    for (j = 0; j < subpoly_rts[i].num_val; j++)
	      ret->value[ret->num_val++] = subpoly_rts[i].value[j];
	}
    }
}

