
## Output can be checked against FRep.Rout.save

## Examples can also be used for regression testing against, e.g.,
## model VRML output

################################################################################
## Primitives

## Defaults
hfSphere()

## Vectorisation
hfSphere(1:2)

################################################################################
## Models

## Single object
hfModel(hfSphere())

## Multiple objects
hfModel(hfSphere(), hfSphere())

## Vectorised single object
hfModel(hfSphere(1:2))

## Combination of single and vectorised objects
hfModel(hfSphere(), hfSphere(1:2))

## Combination of objects and models
hfModel(hfSphere(), hfModel(hfSphere(1:2)))
