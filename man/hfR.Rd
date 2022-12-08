\name{hfR}
\alias{hfR}
\title{
  Describe a HyperFun Object via Function Representation
}
\description{
  Describe a 3D shape using function representation via an R function.
  The surface of the shape corresponds to the function returning zero.
  Negative values are inside the shape and positive values are
  outside the shape.
}
\usage{
hfR(f, name = NULL)
}
\arguments{
  \item{f}{
    A function that takes a single argument (a length 3 numeric vector)
    and returns a single numeric value.
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
  \link{FRep}, \code{\link{hfModel}}, \code{\link{hfp}}.
}
\examples{
## A 3D plane
f <- function(xyz) xyz[2]
hfR(f)
}
\keyword{ dplot }
