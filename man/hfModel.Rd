\name{hfModel}
\alias{hfModel}
\title{
  Describe a HyperFun Model
}
\description{
  Generate a HyperFun model by combining one or more
  HyperFun objects.
}
\usage{
hfModel(..., op = "|", name = NULL)
}
\arguments{
  \item{\dots}{
    One or more HyperFun objects.
  }
  \item{op}{
    An operator to use to combine the objects. See the Details Section.
  }
  \item{name}{
    A character value used as a label for the shape.
  }
}
\details{
  Possible operators are:
  \describe{
    \item{\code{"|"}}{Generate the union of the objects.}
    \item{\code{"&"}}{Generate the intersection of the objects.}
    \item{\code{"-"}}{Subtract subsequent objects from previous objects.}
    \item{\code{"!"}}{Invert the object.}
  }
}
\value{
  A HyperFun model.
}
\author{
  Paul Murrell
}
\seealso{
  \link{FRep}, \code{\link{hfR}}, \code{\link{hfp}}.
}
\examples{
hfModel(hfSphere(), hfBlock())
}
\keyword{ dplot }
