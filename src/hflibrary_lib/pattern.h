


#include <math.h>

#ifndef PATTERN_H
#define PATTERN_H


#ifndef M_PI
#define M_PI   3.1415926535897932384626
#endif

/*********************************************/
/**************HyperFun Noise******************/
static double Noise(double xt,double yt,double zt, double phase,double a2/*is freq*/){

  double a2x, a2y, a2z, sx, sy, sz, sx2, sy2, sz2, a1d;
  double Serx, Sery, Serz;
  double a1, SS; 
  double c;
  double c_min,c_max;

a1 = 1.0 ;


        a2x=a2*xt;
        a2y=a2*yt;
        a2z=a2*zt;
        sx=sin(a2x);
        sy=sin(a2y);
        sz=sin(a2z);
        a1d=a1/1.17;
        sx2=a1d*sin(a2x/1.35+phase*sz);
        sy2=a1d*sin(a2y/1.35+phase*sx);
        sz2=a1d*sin(a2z/1.35+phase*sy);
        Serx=a1*sx+sx2;
        Sery=a1*sy+sy2;
        Serz=a1*sz+sz2;
        SS=Serx*Sery*Serz;

c=2.17/1.17; 
c_min = pow(-a1*c,3);
c_max = -c_min;
		
SS=(SS-c_min)/(c_max-c_min);

return SS;

}

static double ConcentriqueCircle(double xt,double yt,double zt){
double val;

	
	xt *= xt;
	yt *= yt;
	zt *= zt;
	val = (fmod(sqrt(xt+yt+zt), 1.0));
  
  return val;
}

static double cycle(double value)
{
  
  int i;
    i = (int)((value - floor(value)) * 1000.0);
    value = (1.0+sin((double)i*2.0*M_PI/1000.0))/2.0;
	return value;
 
}

static double Crackles(double xt,double yt,double zt,double phase,double freq){

double noise_x = -1+(Noise(xt,xt,zt,phase,freq)*2.0);
double noise_y = -1+(Noise(yt,yt,xt,phase,freq)*2.0);
double noise_z = -1+(Noise(zt,zt,yt,phase,freq)*2.0);

double  noise = (cycle(noise_x+noise_y+noise_z));

return noise;
}


/********************************************************************/
/*******************NOISE FROM PERLIN********************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


static double Noise3(double *);

static double clamp(double x, double a, double b)
{
    return (x < a ? a : (x > b ? b : x));
}


static double turbulence(double *v, double freq)
{
	double t, vec[3];

	for (t = 0. ; freq >= 1. ; freq /= 2) {
		vec[0] = freq * v[0];
		vec[1] = freq * v[1];
		vec[2] = freq * v[2];
		t += fabs(Noise3(vec)) / freq;
	}
	return t;
}


#define B 0x100
#define BM 0xff

#define N 0x1000
#define NP 12   
#define NM 0xfff

static int p[B + B + 2];
static double g3[B + B + 2][3];
static int start = 1;



static double s_curve(double t) {
	return ( t * t * (3. - 2. * t) );
}

static double lerp(t, a, b) {
	return 	( a + t * (b - a) );
}

static void setup_pattern(int i, int b0, int b1, double r0, double r1, double *vec) 
{
	double t = vec[i] + N;
	b0 = ((int)t) & BM;
	b1 = (b0+1) & BM;
	r0 = t - (int)t;
	r1 = r0 - 1.;
}


static double Noise3(double *vec)
{
	int bx0, bx1, by0, by1, bz0, bz1, b00, b10, b01, b11;
	double rx0, rx1, ry0, ry1, rz0, rz1, *q, sy, sz, a, b, c, d, t, u, v;
	register i, j;

	

	setup_pattern(0, bx0,bx1, rx0,rx1,vec);
	setup_pattern(1, by0,by1, ry0,ry1,vec);
	setup_pattern(2, bz0,bz1, rz0,rz1,vec);

	i = p[ bx0 ];
	j = p[ bx1 ];

	b00 = p[ i + by0 ];
	b10 = p[ j + by0 ];
	b01 = p[ i + by1 ];
	b11 = p[ j + by1 ];

	t  = s_curve(rx0);
	sy = s_curve(ry0);
	sz = s_curve(rz0);

#define at3(rx,ry,rz) ( rx * q[0] + ry * q[1] + rz * q[2] )

	q = g3[ b00 + bz0 ] ; u = at3(rx0,ry0,rz0);
	q = g3[ b10 + bz0 ] ; v = at3(rx1,ry0,rz0);
	a = lerp(t, u, v);

	q = g3[ b01 + bz0 ] ; u = at3(rx0,ry1,rz0);
	q = g3[ b11 + bz0 ] ; v = at3(rx1,ry1,rz0);
	b = lerp(t, u, v);

	c = lerp(sy, a, b);

	q = g3[ b00 + bz1 ] ; u = at3(rx0,ry0,rz1);
	q = g3[ b10 + bz1 ] ; v = at3(rx1,ry0,rz1);
	a = lerp(t, u, v);

	q = g3[ b01 + bz1 ] ; u = at3(rx0,ry1,rz1);
	q = g3[ b11 + bz1 ] ; v = at3(rx1,ry1,rz1);
	b = lerp(t, u, v);

	d = lerp(sy, a, b);

	return lerp(sz, c, d);
}



static void normalize3(double v[3])
{
	double s;

	s = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] = v[0] / s;
	v[1] = v[1] / s;
	v[2] = v[2] / s;
}



static void InitPerlin(int seed)
{
	int i, j, k;

	srand(seed);
	
	for (i = 0 ; i < B ; i++) {
		p[i] = i;

		for (j = 0 ; j < 3 ; j++)
			g3[i][j] = (double)((rand() % (B + B)) - B) / B;
		normalize3(g3[i]);
	}

	while (--i) {
		k = p[i];
		p[i] = p[j = rand() % B];
		p[j] = k;
	}

	for (i = 0 ; i < B + 2 ; i++) {
		p[B + i] = p[i];
		for (j = 0 ; j < 3 ; j++)
			g3[B + i][j] = g3[i][j];
	}
}


/********************************************************************************/
/**********************************MUSGRAVE NOISE***********************************/
static int first=1;
static double     *exponent_array;

static double fBm( double* vect, double H, double lacunarity, double octaves )
{

      double            value, frequency, remainder;
      int               i;
     
      /* precompute and store spectral weights */
      if ( first==1 ) {
            /* seize required memory for exponent_array */
            exponent_array = (double *)malloc( (size_t)(octaves+1) * sizeof(double) );
            frequency = 1.0;
            for (i=0; i<=octaves; i++) {
                  /* compute weight for each frequency */
                  exponent_array[i] = pow( frequency, -H );
                  frequency *= lacunarity;
            }
            first = 0;
      }

      value = 0.0;            /* initialize vars to proper values */
      frequency = 1.0;

      /* inner loop of spectral construction */
      for (i=0; i<octaves; i++) {
            value += Noise3( vect ) * exponent_array[i];
            vect[0] *= lacunarity;
            vect[1] *= lacunarity;
            vect[2] *= lacunarity;
      } /* for */

      remainder = octaves - (int)octaves;
      if ( remainder )      /* add in ``octaves''  remainder */
            /* ``i''  and spatial freq. are preset in loop above */
            value += remainder * Noise3( vect ) * exponent_array[i];

      return( value );

} /* fBm() */



#endif /* PATTERN_H */
