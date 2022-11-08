
library(hyperfun)

## Output can be checked against FRep.Rout.save

## Examples can also be used for regression testing against, e.g.,
## model VRML output

################################################################################
## FRep library

## Defaults
hfSphere()
hfBlock()
hfEllipsoid()
hfCylinder()
hfEllCylinder()
hfCone()
hfEllCone()
hfTorus()
hfSuperell()
hfBlobby()

## Vectorisation
hfSphere(1:2, 1:2, 1:2)
hfBlock(1:2, 1:2, 1:2)
hfEllipsoid(c(0, 3))
hfCylinder(cy=c(0, 3))
hfEllCylinder(cy=c(0, 3))
hfCone(cy=c(0, 5))
hfEllCone(cy=c(0, 3))
hfTorus(cy=c(0, 3))
hfSuperell(c(0, 3))
hfBlobby(1:4)
