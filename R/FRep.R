
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

## hfBlock
hfBlock <- function(x = 0, y = 0, z = 0,
                    dx = 1, dy = 1, dz = 1,
                    jx = .5, jy = jx, jz = jx,
                    name = NULL) {
    if (!(is.numeric(x) &&
          is.numeric(y) &&
          is.numeric(z) &&
          is.numeric(dx) &&
          is.numeric(dy) &&
          is.numeric(dz) &&
          is.numeric(jx) &&
          is.numeric(jy) &&
          is.numeric(jz)))
        stop("All arguments must be numeric")
    params <- cbind(x, y, z, dx, dy, dz, jx, jy, jz)
    colnames(params) <- c("x", "y", "z", "dx", "dy", "dz", "jx", "jy", "jz")
    name <- hfName(name, nrow(params))
    block <- list(params = params, name = name)
    class(block) <- c("HyperFunBlock", "HyperFunObject", "HyperFun")
    block
}

as.character.HyperFunBlock <- function(x, ...) {
    paste0(x$name,
           "(x[3], a[1])\n",
           "{\n",
           "  array vertex[3];\n",
           "  vertex = [",
           x$params[,"x"] - x$params[,"jx"]*x$params[,"dx"], ", ", 
           x$params[,"y"] - x$params[,"jy"]*x$params[,"dy"], ", ", 
           x$params[,"z"] - x$params[,"jz"]*x$params[,"dz"], "];\n",
           "  block = hfBlock(x, vertex, ",
           x$params[,"dx"], ", ", x$params[,"dy"], ", ", x$params[,"dz"],
           ");\n",
           "  ", x$name, " = block;\n",
           "}\n")
}

length.HyperFunBlock <- function(x, ...) {
    nrow(x$params)
}
