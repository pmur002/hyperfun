\name{FRep}
\alias{FRep}
\alias{hfSphere}
\alias{hfBlock}
\alias{hfCone}
\alias{hfEllipsoid}
\alias{hfSuperEll}
\alias{hfTorus}
\alias{hfBlobby}
\alias{hfCylinder}
\alias{hfEllCylinder}
\alias{hfEllCone}
\title{
  Describe a HyperFun Object
}
\description{
  Describe one of the predefined objects from the HyperFun
  FRep library.
}
\usage{
hfBlock(x = 0, y = 0, z = 0, dx = 1, dy = 1, dz = 1,
        jx = .5, jy = jx, jz = jx, name = NULL)
hfSphere(cx = 0, cy = 0, cz = 0, r = 1, name = NULL)
hfEllipsoid(cx = 0, cy = 0, cz = 0, a = 1, b = 2, c = 3, name = NULL)
hfCylinder(cx = 0, cy = 0, cz = 0, r = 1, axis = "X", name = NULL)
hfEllCylinder(cx = 0, cy = 0, cz = 0, a = 2, b = 4, axis = "X", name = NULL)
hfCone(cx = 0, cy = 0, cz = 0, r = 0.5, axis = "X", name = NULL)
hfEllCone(cx = 0, cy = 0, cz = 0, a = 0.5, b = 1, axis = "X", name = NULL)
hfTorus(cx = 0, cy = 0, cz = 0, R = 2, r0 = 1, axis = "X", name = NULL)
hfSuperEll(cx = 0, cy = 0, cz = 0, a = 1, b = 2, c = 3,
           s1 = 0.5, s2 = 2, name = NULL)
hfBlobby(x0 = c(1, -1), y0 = 0, z0 = 0, a0 = 2, b0 = 3, T = 0.5, name = NULL)
}
\arguments{
  \item{x, y, z}{
    Numeric values specifying the location of a rectanguloid.
  }
  \item{dx, dy, dz}{
    Numeric values specifying the dimensions of a rectanguloid.
  }
  \item{jx, jy, jz}{
    Numeric values specifying the justifications (0 to 1) of a rectanguloid
    relative to its location.
  }
  \item{cx, cy, cz}{
    Numeric values specifying the centre of an object.
  }
  \item{r}{
    Numeric value specifying the radius of a sphere or a cylinder or a cone.
    For a cone, this is the radius of the cone at height 1.
  }
  \item{a, b, c}{
    Numeric values specifying the lengths of the semi-axes of an
    ellipsoid or elliptic cylinder or elliptic cone.
    For an elliptic cone, this is the length at height 1.
  }
  \item{axis}{
    Character value specifying the axis, \code{"X"},
    \code{"Y"}, or \code{"Z"}, for, e.g., a cone or cylinder.
  }
  \item{R}{
    Numeric value specifying the radius of rotation for a torus.
  }
  \item{r0}{
    Numeric value specifying the disc radius for a torus.
  }
  \item{s1, s2}{
    Numeric values controlling the sharpness (in z- and xy-directions
    respectively) for a super ellipsoid.  Smaller values produce
    more blocky results and larger values produce more pointy results.
  }
  \item{x0, y0, z0}{
    Numeric values specifying the locations of blobs in a blobby object.
  }
  \item{a0, b0, T}{
    Numeric values controlling the connectedness of blobs in a blobby
    object.  Larger \code{a0} and \code{b0} and lower \code{T}
    will make blobs more connected.
    
  }
  \item{name}{
    A character value used as a label for the object.
  }
}
\value{
  A HyperFun object.
}
\author{
  Paul Murrell
}
\seealso{
  \link{hfp}, \code{\link{hfModel}}, \code{\link{hfR}}.
}
\examples{
hfSphere()
}
\keyword{ dplot }
