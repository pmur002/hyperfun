\name{hfp}
\alias{hfp}
\alias{hfp.HyperFunObject}
\alias{hfp.HyperFunModel}
\alias{hfp.character}
\title{
  Render a HyperFun Model
}
\description{
  This function takes a HyperFun model and generates
  a final image, either as an external file or on screen
  (in an \pkg{rgl} window).
}
\usage{
hfp(x, \dots)
\method{hfp}{HyperFunObject}(x, \dots)
\method{hfp}{HyperFunModel}(x, format = "rgl", bbox = 10, density = 30, \dots)
\method{hfp}{character}(x, format = "rgl", bbox = 10, density = 30, \dots)
}
\arguments{
  \item{x}{
    Either a HyperFun object or a HyperFun model or a character vector.
    A character vector with a single is assumed to be the name of a file
    containing HyperFun code, otherwise the character values are
    assumed to be HyperFun code.
  }
  \item{format}{
    A character value describing the output format.  See the Details Section.
  }
  \item{bbox}{
    A numeric vector specifying the lower
    and upper limits of the bounding box for the scene.
    See the Details Section.
  }
  \item{density}{
    A numeric vector specifying the resolution of the final scene.
    See the Details Section.
    Higher values will produce a smoother result.
  }
  \item{\dots}{
    Arguments passed to other methods.
  }
}
\details{
  Possible output formats are:
  \describe{
    \item{\code{"rgl"}}{Output is to the screen (an \pkg{rgl} window).}
    \item{\code{"stl"}}{Output is an STL file.}
    \item{\code{"stlb"}}{Output is an STLB file.}
    \item{\code{"cli"}}{Output is a CLI file.}
    \item{\code{"pov"}}{Output is a POV file.}
    \item{\code{"wrl"}}{Output is a VRML file.}
  }

  The bounding box can be specified as 1, 2, 3 or 6 values giving
  the lower and upper bounds for x-, y-, and z-dimensions in the
  form \code{c(xmin, ymin, zmin, xmax, ymax, zmax)}.
  A single value, \code{d}, is interpreted as
  \code{c(-d, -d, -d, d, d, d)}.
  Two values, \code{c(d1, d2)}, are interpreted as
  \code{c(d1, d1, d1, d2, d2, d2)}.
  Three values, \code{c(d1, d2, d3)} are interpreted as
  \code{c(-d1, -d2, -d3, d1, d2, d3)}.

  The \code{density} can be specified as 1 or 3 values, giving
  the resolution of the final scene in the form
  \code{c(xdensity, ydensity, zdensity)}.
  A single value, \code{r}, is interpreted as \code{c(r, r, r)}.
}
\value{
  The name of the file to which output was written.
  For \code{format="rgl"} this is an STLB file.
}
\references{
  Cartwright, R., Adzhiev, V., Pasko, A.A., Goto, Y., Kunii,
  T.L. (2005), "Web-based shape modeling with HyperFun",
  IEEE Computer Graphics and Applications,
  IEEE Computer Society Press, 25 (2): 60-69, \doi{10.1109/MCG.2005.49}.
}
\author{
  Paul Murrell
}
\seealso{
  \link{FRep}, \code{\link{hfModel}}, \code{\link{hfR}}.
}
\examples{
if (interactive())
    hfp(hfSphere())
}
\keyword{ dplot }
