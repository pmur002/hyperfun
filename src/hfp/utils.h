
#ifndef _UTILS_H_
#define _UTILS_H_

#if defined(WIN32)
#include <float.h>
#pragma warning (disable:4244)          /* disable bogus conversion warnings */
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#endif
#include <math.h>
//#include <iostream.h>

#ifndef NULL
#define NULL 0
#endif

// Some <math.h> files do not define M_PI...
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef FLT_EPSILON
#define    FLT_EPSILON     1.19209290E-07F
#endif

#ifndef FLT_MAX
#define	FLT_MAX		3.40282347E+38F // max decimal value of a "float"
#endif

#ifndef UINT_MAX
#define UINT_MAX 4294967295U
#endif

class vect3f
{
public:
  float v[3];
  // constructor
  vect3f(); 
  vect3f(const vect3f& n); 
  vect3f(const float x, const float y, const float z);
  // copy
  void operator=(const vect3f& n);
  void set(const float x, const float y, const float z);
  // member operations
  float operator[](int i) const { return v[i]; }
  float& operator[](int i) { return v[i]; }

  void operator+=(const vect3f& n);
  void operator-=(const vect3f& n);
  void operator*=(float); // scale
  void operator/=(float); // scale
  friend float length2(const vect3f& a);
  friend float length(const vect3f& a);
  friend float dot(const vect3f& a, const vect3f& b);
  friend void cross(const vect3f& a, const vect3f& b, 
		    vect3f& res);
  friend void add(const vect3f& a, const vect3f& b, 
		  vect3f& res);
  friend void sub(const vect3f& a, const vect3f& b, 
		  vect3f& res);
  void normalize();
  // friend operations
  friend void normalize(float* a);
  friend float length2(const float* a);
  friend float length(const float* a);

  
  // Les mmes en FLOAT-STAR !!!
  vect3f(const float* n); // init to zero if pointer is void
  // cast
  operator const float*() const { return v;}
  operator float*() { return v; }
  // standard operators
  void operator=(const float* n); // give NULL to zeroe
  void operator+=(const float* n); // n!=NULL  !!
  void operator-=(const float* n);
  friend float dot(const float* a, const float* b);
  friend void cross(const float* a, const float* b, 
		    float* res);
  // res = a^b
  friend void add(const float* a, const float* b, 
		  float* res);
  // res = a+b
  friend void sub(const float* a, const float* b, 
		  float* res);
  // res = a-b
};

class quaternion
{
  //
  //  Rappel des conventions utilisees ici: 
  //
  //   Soient:  v[3]   ->   vecteur directeur unitaire
  //            phi    ->   angle
  //
  //   Quaternion correspondant:
  //       {q[0],q[1],q[2]}  =    1/sin(phi/2) * {v[0],v[1],v[2]}
  //    et            q[3]   =    cos(phi/2)
public:
  float q[4];
  // constructor
  quaternion();
  quaternion(const quaternion& n);
  quaternion(const vect3f& v, const float angle);
  quaternion(const vect3f& v, const float sina, const float cosa);
  quaternion(const float m[4][4]);
  // cast
  operator float*() { return q;}
  // operations
  void fromAxis(const vect3f& v, 
		const float angle);
  void fromAxis(const vect3f& v, 
		const float sina,
		const float cosa); 
  void fromMatrix(const float m[4][4]);
  // sina / cosa -> avec a=angle/2.0
  void rotmatrix(float m[4][4]) const; // m should be m[4][4]
  void operator=(const quaternion& n);
  void operator+=(const quaternion& n);
  void operator-=(const quaternion& n);
  void normalize();
  void nullRot() { q[0]=q[1]=q[2]=0.0; q[3]=1.0; }
  // friend operations
  friend float length(const quaternion& a);
  friend float length2(const quaternion& a);
  friend void add(const quaternion& a, 
		  const quaternion& b,
		  quaternion& res);
  friend void sub(const quaternion& a, 
		  const quaternion& b,
		  quaternion& res);
  // res=rotation equivalent to rotations a and b
  // res may be a or b
  // -- vect3f utilities
  void image(const vect3f& v, vect3f& res) const;
  // res <- image par la rotation q de v
  void source(const vect3f& v, vect3f& res) const;
  // res <- vecteur t.q. v est image de res par la rotation q
  void image(float* v, float* res) const;
  void source(float* v, float* res) const;
};

class Box {
public:
  vect3f min, max;
  Box(const float xmin = 0.0f, 
      const float ymin = 0.0f, 
      const float zmin = 0.0f,
      const float xmax = 0.0f, 
      const float ymax = 0.0f, 
      const float zmax = 0.0f);
  Box(const vect3f& init_min, const vect3f& init_max);
  
  int contains(const vect3f& p) const;
  // point p contained in box
  int contains(const vect3f& pmin, const vect3f& pmax) const;
  // Is Box(pmin,pmax) contained in box ? (=entirely included inside)
  int contains(const Box& b) const;
  // Is Box b contained in box ? (=entirely included inside)
  int intersect(const vect3f& pmin, const vect3f& pmax) const;
  // Does Box(pmin, pmax) intersect box?
  int intersect(const Box& b) const;
  // Does Box b intersect box?
};

////////////////////////////////////////////////////////////////////////////
//
//          vect3f inline implementation
//
////////////////////////////////////////////////////////////////////////////

inline vect3f::vect3f() 
{
  v[0]=v[1]=v[2]=0.0;
}
inline vect3f::vect3f(const vect3f& n)
{
  v[0]=n.v[0];v[1]=n.v[1];v[2]=n.v[2];
}
inline vect3f::vect3f(const float* n) 
{
  if (n!=NULL) {
    v[0]=n[0];v[1]=n[1];v[2]=n[2];
  } else {
    v[0] = v[1] = v[2] = 0.;
  }
}
inline vect3f::vect3f(const float x, const float y, const float z)
{ 
  v[0]=x;v[1]=y;v[2]=z;
}
// copy
inline void
vect3f::operator=(const vect3f& n)
{
  v[0]=n.v[0];v[1]=n.v[1];v[2]=n.v[2]; 
}
inline void
vect3f::operator=(const float* n) 
{
  if (n) {
    v[0]=n[0];v[1]=n[1];v[2]=n[2]; 
  } else {
    v[0]=v[1]=v[2]=0.0;
  }
}
inline void
vect3f::set(const float x, const float y, const float z)
{ 
  v[0]=x;v[1]=y;v[2]=z;
}
// member operations
inline void 
vect3f::normalize()
{
  float l=length2(*this);
  if (l>0.0f) {
    l=sqrt(l);
    v[0]/=l; v[1]/=l; v[2]/=l;
  } else {
    v[0]=v[1]=v[2]=0.0f;
  }
}
inline void
vect3f::operator+=(const vect3f& n)
{
  v[0]+=n.v[0]; v[1]+=n.v[1]; v[2]+=n.v[2];
}
inline void
vect3f::operator-=(const vect3f& n)
{
  v[0]-=n.v[0]; v[1]-=n.v[1]; v[2]-=n.v[2];
}
inline void
vect3f::operator*=(float s)
{
  v[0]*=s;v[1]*=s;v[2]*=s; 
}
inline void
vect3f::operator/=(float s)
{
  v[0]/=s;v[1]/=s;v[2]/=s; 
}
inline float 
dot(const vect3f& a, const vect3f& b)
{
  return (a.v[0]*b.v[0] + a.v[1]*b.v[1] + a.v[2]*b.v[2]);
}
inline float 
length(const vect3f& a)
{
  return sqrtf(dot(a,a));
}
inline float 
length2(const vect3f& a)
{
  return dot(a,a);
}
inline void 
cross(const vect3f& a, const vect3f& b, vect3f& res)
 // res = a^b  ++ prevent side effects (if res==a or b !!!)
{
  float x=a.v[1]*b.v[2] - a.v[2]*b.v[1];
  float y=a.v[2]*b.v[0] - a.v[0]*b.v[2];
  float z=a.v[0]*b.v[1] - a.v[1]*b.v[0];
  res.v[0] = x; res.v[1] = y; res.v[2] = z;
}
inline void 
add(const vect3f& a, const vect3f& b, vect3f& res)
  // res = a+b
{
  res.v[0]=a.v[0]+b.v[0];
  res.v[1]=a.v[1]+b.v[1];
  res.v[2]=a.v[2]+b.v[2];
}
inline void 
sub(const vect3f& a, const vect3f& b, vect3f& res)
    // res = a-b
{
  res.v[0]=a.v[0]-b.v[0];
  res.v[1]=a.v[1]-b.v[1];
  res.v[2]=a.v[2]-b.v[2];
}

// FLOAT-STAR VERSIONS
inline void
vect3f::operator+=(const float* n)
{
  v[0]+=n[0]; v[1]+=n[1]; v[2]+=n[2];
}
inline void
vect3f::operator-=(const float* n)
{
  v[0]-=n[0]; v[1]-=n[1]; v[2]-=n[2];
}
inline float 
dot(const float* a, const float* b)
{
  return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}
inline float 
length(const float* a)
{
  return sqrtf(dot(a,a));
}
inline float 
length2(const float* a)
{
  return dot(a,a);
}
inline void
normalize(float* a)
{
  float v=length2(a);
  if (v>0.0f) {
    v=sqrtf(v);
    a[0]/=v; a[1]/=v; a[2]/=v;
  } else {
    a[0]=a[1]=a[2]=0.0;
  }
}
inline void 
cross(const float* a, const float* b, float* res)
 // res = a^b
{
  float x=a[1]*b[2] - a[2]*b[1];
  float y=a[2]*b[0] - a[0]*b[2];
  float z=a[0]*b[1] - a[1]*b[0];
  res[0] = x; res[1] = y; res[2] = z;
}
inline void 
add(const float* a, const float* b, float* res)
  // res = a+b
{
  res[0]=a[0]+b[0];
  res[1]=a[1]+b[1];
  res[2]=a[2]+b[2];
}
inline void 
sub(const float* a, const float* b, float* res)
    // res = a-b
{
  res[0]=a[0]-b[0];
  res[1]=a[1]-b[1];
  res[2]=a[2]-b[2];
}

////////////////////////////////////////////////////////////////////////////
//
//          quaternion inline implementation
//
////////////////////////////////////////////////////////////////////////////

inline quaternion::quaternion()
{
  q[0]=q[1]=q[2]=0.0; q[3]=1.0;
}

inline quaternion::quaternion(const quaternion& n)
{
  q[0]=n.q[0]; q[1]=n.q[1]; q[2]=n.q[2]; q[3]=n.q[3];
}
// operations
inline void
quaternion::fromAxis(const vect3f& v, 
		     const float sina,
		     const float cosa)
{
  q[0]=v.v[0]; q[1]=v.v[1]; q[2]=v.v[2];
  float l = length2(v);
  if (l>0.0f) {
    l = sina/sqrtf(l);
    q[0]*=l; q[1]*=l; q[2]*=l; q[3] = cosa;
  } else {
    q[0]=q[1]=q[2]=0.0f; q[3]=1.0f;
  }
}
inline quaternion::quaternion(const vect3f& v, 
			      const float sina,
			      const float cosa)
{
  fromAxis(v, sina, cosa);
}
inline void
quaternion::fromAxis(const vect3f& v, const float angle)
{
  float da = angle/2.0;
  fromAxis(v, sinf(da), cosf(da));
}
inline quaternion::quaternion(const vect3f& v, const float angle) 
{
  fromAxis(v, angle);
}

inline void
quaternion::fromMatrix(const float m[4][4])
{
  // *** quote from SbRotation.c++ ***
  // direct method:
  // q[0] = sqrtf(m[0][0]-m[1][1]-m[2][2]+m[3][3])/2.0;
  // q[1] = sqrtf(m[1][1]-m[2][2]-m[0][0]+m[3][3])/2.0;
  // q[2] = sqrtf(m[2][2]-m[0][0]-m[1][1]+m[3][3])/2.0;
  // q[3] = sqrtf(m[0][0]+m[1][1]+m[2][2]+m[3][3])/2.0;

  // First, find largest diagonal in matrix:
  int i = 2;
  if (m[0][0] > m[1][1]) {
    if (m[0][0] > m[2][2]) {
     i = 0;
    } 
  } else {
    if (m[1][1] > m[2][2]) {
      i = 1;
    }
  }
  if (m[0][0]+m[1][1]+m[2][2] > m[i][i]) {
    // Compute w first:
    q[3] = sqrtf(m[0][0]+m[1][1]+m[2][2]+m[3][3])/2.0;
    // And compute other values:
    q[0] = (m[1][2]-m[2][1])/(4*q[3]);
    q[1] = (m[2][0]-m[0][2])/(4*q[3]);
    q[2] = (m[0][1]-m[1][0])/(4*q[3]);
  } else {
    // Compute x, y, or z first:
    int j = (i+1)%3; 
    int k = (i+2)%3;

    // Compute first value:
    q[i] = sqrtf(m[i][i]-m[j][j]-m[k][k]+m[3][3])/2.0;

    // And the others:
    q[j] = (m[i][j]+m[j][i])/(4*q[i]);
    q[k] = (m[i][k]+m[k][i])/(4*q[i]);

    q[3] = (m[j][k]-m[k][j])/(4*q[i]);
  }
  q[3]=-q[3];
}
inline
quaternion::quaternion(const float m[4][4])
{
  fromMatrix(m);
}
inline void  
quaternion::rotmatrix(float m[4][4]) const
// m should be m[4][4]
{
  m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
  m[0][1] =       2.0 * (q[0] * q[1] - q[2] * q[3]);
  m[0][2] =       2.0 * (q[2] * q[0] + q[1] * q[3]);
  m[0][3] = 0.0;

  m[1][0] =       2.0 * (q[0] * q[1] + q[2] * q[3]);
  m[1][1] = 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
  m[1][2] =       2.0 * (q[1] * q[2] - q[0] * q[3]);
  m[1][3] = 0.0;

  m[2][0] =       2.0 * (q[2] * q[0] - q[1] * q[3]);
  m[2][1] =       2.0 * (q[1] * q[2] + q[0] * q[3]);
  m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
  m[2][3] = 0.0;

  m[3][0] = 0.0;
  m[3][1] = 0.0;
  m[3][2] = 0.0;
  m[3][3] = 1.0;
}
inline float 
length2(const quaternion& a)
{
  return a.q[0]*a.q[0] + a.q[1]*a.q[1] + a.q[2]*a.q[2] + a.q[3]*a.q[3];
}
inline float 
length(const quaternion& a)
{
  return sqrtf(length2(a));
}
inline void  
quaternion::normalize()
{
  float mag = length2(*this);
  if (mag>0) {
    mag=sqrtf(mag);
    q[0]/=mag;q[1]/=mag;q[2]/=mag;q[3]/=mag;
  } else {
    q[0]=q[1]=q[2]=0.0; q[3]=1.0;
  }
}
inline void 
add(const quaternion& a, const quaternion& b,
    quaternion& res)
// res=rotation equivalent to rotations a and b
{
  float p0 = a.q[3]*b.q[0] + b.q[3]*a.q[0] + a.q[1]*b.q[2] - a.q[2]*b.q[1];
  float p1 = a.q[3]*b.q[1] + b.q[3]*a.q[1] + a.q[2]*b.q[0] - a.q[0]*b.q[2];
  float p2 = a.q[3]*b.q[2] + b.q[3]*a.q[2] + a.q[0]*b.q[1] - a.q[1]*b.q[0];
  float p3 = a.q[3]*b.q[3] - a.q[0]*b.q[0] - a.q[1]*b.q[1] - a.q[2]*b.q[2];
  res.q[0]=p0; res.q[1]=p1; res.q[2]=p2; res.q[3]=p3;
  res.normalize();
}
inline void 
sub(const quaternion& a, const quaternion& b,
    quaternion& res)
// res=rotation equivalent to rotations a and b
{
  float p0 =-a.q[3]*b.q[0]+b.q[3]*a.q[0]-a.q[1]*b.q[2]+a.q[2]*b.q[1];
  float p1 =-a.q[3]*b.q[1]+b.q[3]*a.q[1]-a.q[2]*b.q[0]+a.q[0]*b.q[2];
  float p2 =-a.q[3]*b.q[2]+b.q[3]*a.q[2]-a.q[0]*b.q[1]+a.q[1]*b.q[0];
  float p3 = a.q[3]*b.q[3]+a.q[0]*b.q[0]+a.q[1]*b.q[1]+a.q[2]*b.q[2];
  res.q[0]=p0; res.q[1]=p1; res.q[2]=p2; res.q[3]=p3;
  res.normalize();
}
inline void
quaternion::operator=(const quaternion& n)
{
  q[0]=n.q[0];q[1]=n.q[1];q[2]=n.q[2];q[3]=n.q[3];
}
inline void
quaternion::operator+=(const quaternion& n)
{
  add(*this, n, *this);
}
inline void
quaternion::operator-=(const quaternion& n)
{
  sub(*this, n, *this);
}
// -- vect3f utilities
inline void 
quaternion::image(const vect3f& v, vect3f& res) const
  // res <- image par la rotation q de v
{
  // utilise la matrice plutot que des calculs vectoriels
  float x = 
    (1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]))*v.v[0] +
    (      2.0 * (q[0] * q[1] - q[2] * q[3]))*v.v[1] +
    (      2.0 * (q[2] * q[0] + q[1] * q[3]))*v.v[2];
  float y = 
    (      2.0 * (q[0] * q[1] + q[2] * q[3]))*v.v[0] +
    (1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]))*v.v[1] +
    (      2.0 * (q[1] * q[2] - q[0] * q[3]))*v.v[2];
  float z = 
    (      2.0 * (q[2] * q[0] - q[1] * q[3]))*v.v[0] +
    (      2.0 * (q[1] * q[2] + q[0] * q[3]))*v.v[1] +
    (1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]))*v.v[2];
  res.v[0]=x; res.v[1]=y; res.v[2]=z;
}
inline void 
quaternion::source(const vect3f& v, vect3f& res) const
  // res <- vecteur t.q. v est image de res par la rotation q
{  
  // utilise la matrice plutot que des calculs vectoriels
  float x = 
    (1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]))*v.v[0] +
    (      2.0 * (q[0] * q[1] + q[2] * q[3]))*v.v[1] +
    (      2.0 * (q[2] * q[0] - q[1] * q[3]))*v.v[2];
  float y = 
    (      2.0 * (q[0] * q[1] - q[2] * q[3]))*v.v[0] +
    (1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]))*v.v[1] +
    (      2.0 * (q[1] * q[2] + q[0] * q[3]))*v.v[2];
  float z = 
    (      2.0 * (q[2] * q[0] + q[1] * q[3]))*v.v[0] +
    (      2.0 * (q[1] * q[2] - q[0] * q[3]))*v.v[1] +
    (1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]))*v.v[2];
  res.v[0]=x; res.v[1]=y; res.v[2]=z;
}

inline void 
quaternion::image(float* v, float* res) const
  // res <- image par la rotation q de v
{
  // utilise la matrice plutot que des calculs vectoriels
  float x = 
    (1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]))*v[0] +
    (      2.0 * (q[0] * q[1] - q[2] * q[3]))*v[1] +
    (      2.0 * (q[2] * q[0] + q[1] * q[3]))*v[2];
  float y = 
    (      2.0 * (q[0] * q[1] + q[2] * q[3]))*v[0] +
    (1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]))*v[1] +
    (      2.0 * (q[1] * q[2] - q[0] * q[3]))*v[2];
  float z = 
    (      2.0 * (q[2] * q[0] - q[1] * q[3]))*v[0] +
    (      2.0 * (q[1] * q[2] + q[0] * q[3]))*v[1] +
    (1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]))*v[2];
  res[0]=x; res[1]=y; res[2]=z;
}
inline void 
quaternion::source(float* v, float* res) const
  // res <- vecteur t.q. v est image de res par la rotation q
{  
  // utilise la matrice plutot que des calculs vectoriels
  float x = 
    (1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]))*v[0] +
    (      2.0 * (q[0] * q[1] + q[2] * q[3]))*v[1] +
    (      2.0 * (q[2] * q[0] - q[1] * q[3]))*v[2];
  float y = 
    (      2.0 * (q[0] * q[1] - q[2] * q[3]))*v[0] +
    (1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]))*v[1] +
    (      2.0 * (q[1] * q[2] + q[0] * q[3]))*v[2];
  float z = 
    (      2.0 * (q[2] * q[0] + q[1] * q[3]))*v[0] +
    (      2.0 * (q[1] * q[2] - q[0] * q[3]))*v[1] +
    (1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]))*v[2];
  res[0]=x; res[1]=y; res[2]=z;
}

//============= Box impl. ================

inline
Box::Box(const float xmin, const float ymin, const float zmin,
	 const float xmax, const float ymax, const float zmax)
  : min(xmin, ymin, zmin), max(xmax, ymax, zmax)
{}

inline
Box::Box(const vect3f& init_min, const vect3f& init_max)
  : min(init_min), max(init_max)
{}

inline int 
Box::contains(const vect3f& p) const
{
  return (p[0]>=min[0]) && (p[1]>=min[1]) && (p[2]>=min[2])
    &&   (p[0]<=max[0]) && (p[1]<=max[1]) && (p[2]<=max[2]);
}

inline int 
Box::contains(const vect3f& pmin, const vect3f& pmax) const
{
  return (pmin[0]>=min[0]) && (pmin[1]>=min[1]) && (pmin[2]>=min[2])
    &&   (pmax[0]<=max[0]) && (pmax[1]<=max[1]) && (pmax[2]<=max[2]);
}

inline int 
Box::contains(const Box& b) const
{
  return contains(b.min, b.max);
}

inline int 
Box::intersect(const vect3f& pmin, const vect3f& pmax) const
{
  return (min[0]<pmax[0]) && (min[1]<pmax[1]) && (min[2]<pmax[2])
    &&   (max[0]>pmin[0]) && (max[1]>pmin[1]) && (max[2]>pmin[2]);
}
inline int
Box::intersect(const Box& b) const
{
  return intersect(b.min, b.max);
}

//============= streams facilities ================
/*
inline ostream& 
operator<<(ostream& out, const vect3f& v) {
  out << v.v[0] << " " << v.v[1] << " " << v.v[2] ;
  return out;
}

inline istream& 
operator>>(istream& in, vect3f& v) {
  in >> v.v[0];
  if (!in.fail()) {
    in >> v.v[1];
    if (!in.fail()) {
      in >> v.v[2];
    }
  }
  return in;
}

inline ostream& 
operator<<(ostream& out, const quaternion& q) {
  out << q.q[0] << " " << q.q[1] << " " << q.q[2] << " " << q.q[3] ;
  return out;
}

inline istream& 
operator>>(istream& in, quaternion& q) {
  in >> q.q[0];
  if (!in.fail()) {
    in >> q.q[1];
    if (!in.fail()) {
      in >> q.q[2];
      if (!in.fail()) {
	in >> q.q[3];
      }
    }
  }
  return in;
}

inline ostream& 
operator<<(ostream& out, const Box& b) {
  out << b.min << " " << b.max;
  return out;
}
*/
#endif
