
## R functions that generate FRep code (when printed)

################################################################################
## FRep library

hfSphere <- function(cx = 0, cy = 0, cz = 0, r = 1, name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(r)))
        stop("All arguments must be numeric")
    params <- cbind(cx, cy, cz, r)
    colnames(params) <- c("x", "y", "z", "r")
    name <- hfName(name, nrow(params))
    sphere <- list(params = params, name = name)
    class(sphere) <- c("HyperFunSphere", "HyperFunObject", "HyperFun")
    sphere
}

as.character.HyperFunSphere <- function(x, ...) {
    paste0(x$name,
           "(x[3], a[1])\n",
           "{\n",
           "  array center[3];\n",
           "  center = [",
           x$params[,"x"], ", ", 
           x$params[,"y"], ", ", 
           x$params[,"z"], "];\n",
           "  sphere = hfSphere(x, center, ", x$params[,"r"], ");\n",
           "  ", x$name, " = sphere;\n",
           "}\n")
}

length.HyperFunSphere <- function(x, ...) {
    nrow(x$params)
}
