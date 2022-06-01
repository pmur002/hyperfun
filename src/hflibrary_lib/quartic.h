#ifndef QUARTIC
#define QUARTIC

#define LARGE_VAL 1e10

/* structure that stores upto 5 floating point numbers */
struct retval
{
  int num_val;
  double value[5];
};


/* real roots of quartic equation:
   c4 * x^4 + c3 * x^3 + c2 * x^2 + c1 + c0 = 0
 if successful, returns real roots in ret.value[] */
void calc_roots_of_quartic(struct retval  *ret, double c4, 
						   double c3, double c2, 
						   double c1, double c0);

#endif /*QUARTIC*/
