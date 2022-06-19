
library(hyperfun.combined)
library(tools)

################################################################################
## HyperFun models

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

## Combination of models
hfModel(hfSphere(1:2), hfModel(hfSphere(3:4)))

## Non-default operators

## Unary operator (!)

########################
## Different operators

## | (union)
hfSphere(1) | hfSphere(2)

## & (intersection)
hfSphere(1) & hfSphere(2)

## - (subtraction => \)
hfSphere(1) - hfSphere(2)

## ! (not => ~)
!hfSphere()
hfSphere(1) - !hfSphere(2)
## Can only use ! with single object or single model
assertError(!hfSphere(1:2))

## * (cartesian product => @)
hfSphere(1) * hfSphere(2)

## multiple operations

## operations with arguments that are multiple objects

## operations with arguments that are models

## operations with arguments that are mixture of objects and models
