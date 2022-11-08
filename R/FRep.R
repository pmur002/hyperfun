
## R functions that generate FRep code (when printed)

################################################################################
## FRep library

## hfBlobby
hfBlobby <- function(x0 = c(1, -1), 
                     y0 = 0,
                     z0 = 0,
                     a = 2,
                     b = 3,
                     T = 0.5, name = NULL) {
    if (!(is.numeric(x0) &&
          is.numeric(y0) &&
          is.numeric(z0) &&
          is.numeric(a)  &&
          is.numeric(b)  &&
          is.numeric(T)))
        stop("All arguments must be numeric")
    params <- cbind(x0, y0, z0, a, b)
    colnames(params) <- c("x0", "y0", "z0", "a", "b")
    name <- hfName(name, 1)
    blobby <- list(params = params, name = name, Threshold = T)
    class(blobby) <- c("HyperFunBlobby", "HyperFunObject", "HyperFun")
    blobby
}

as.character.HyperFunBlobby <- function(x, ...) {
    arrays = paste0(c("x0", "y0", "z0", "p", "b"),
                    "[", length(x), "]", collapse = ", ")
    x0 = paste0("  x0 = [", paste(x$params[,"x0"], collapse = ", "), "];")
    y0 = paste0("  y0 = [", paste(x$params[,"y0"], collapse = ", "), "];")
    z0 = paste0("  z0 = [", paste(x$params[,"z0"], collapse = ", "), "];")
    a = paste0("  p = [", paste(x$params[,"a"], collapse = ", "), "];")
    b = paste0("  b = [", paste(x$params[,"b"], collapse = ", "), "];")
    paste0(x$name, "(x[3], a[1])\n",
           "{", "\n",
           "  array ", arrays, ";\n",
           x0, "\n",
           y0, "\n",
           z0, "\n",
           a, "\n",
           b, "\n",
           "  blobby = hfBlobby", "(x, x0, y0, z0, p, b, ", x$Threshold, ");\n",
           "  ", x$name, " = blobby;\n",
           "}\n")
}

length.HyperFunBlobby <- function(x, ...) {
    nrow(x$param)  
}

## hfSuperell
hfSuperell <- function(cx = 0, cy = 0, cz = 0, a = 1, b = 2, c = 3,
                       s1 = 0.5, s2 = 2, name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(a)  &&
          is.numeric(b)  &&
          is.numeric(c)  &&
          is.numeric(s1)  &&
          is.numeric(s2)))
        stop("All arguments must be numeric")
    params <- cbind(cx, cy, cz, a, b, c, s1, s2)
    colnames(params) <- c("x", "y", "z", "a", "b", "c", "s1", "s2")
    name <- hfName(name, nrow(params))
    superellipsoid <- list(params = params, name = name)
    class(superellipsoid) <- c("HyperFunSuperellipsoid", "HyperFunObject",
                               "HyperFun")
    superellipsoid
}

as.character.HyperFunSuperellipsoid <- function(x, ...) {
    center <- paste0("[", x$params[,"x"], ",",
                     x$params[,"y"], "," ,
                     x$params[,"z"], "]")
    paste0(x$name, "(x[3], a[1])\n",
           "{\n",
           "  array center[3];\n",
           "  center = ", center, ";\n",
           "  superell = hfSuperell", "(x, center, ",
           x$params[,"a"], ",", x$params[,"b"], ",", x$params[,"c"], ",",
           x$params[,"s1"], ",", x$params[,"s2"] ,");\n",
           "  ", x$name, " = superell;\n",
           "}\n")
}

length.HyperFunSuperellipsoid <- function(x, ...) {
    nrow(x$param)  
}

## hfTorus
hfTorus <- function(cx = 0, cy = 0, cz = 0, R = 2, r0 = 1, axis = "X",
                    name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(R)  &&
          is.numeric(r0)))
        stop("All arguments must be numeric")
    axis = toupper(axis)
    if (!all(axis %in% c("X", "Y", "Z"))) {
        stop("Axis name must be 'X' or 'Y' or 'Z'")
    }
    params <- cbind(cx, cy, cz, R, r0, axis)
    colnames(params) <- c("x", "y", "z", "R", "r0", "axis")
    name <- hfName(name, nrow(params))
    torus <- list(params = params, name = name)
    class(torus) <- c("HyperFunTorus", "HyperFunObject", "HyperFun")
    torus
}

as.character.HyperFunTorus <- function(x, ...) {
    center <- paste0("[", x$params[,"x"], ",",
                     x$params[,"y"], "," ,
                     x$params[,"z"], "]")
    paste0(x$name, "(x[3], a[1])\n",
           "{\n",
           "  array center[3];\n",
           "  center = ", center, ";\n",
           "  torus = hfTorus", x$params[,"axis"],"(x, center, ",
           x$params[,"R"], ", ", x$params[,"r0"] ,");\n",
           "  ", x$name, " = torus;\n",
           "}\n")
}

length.HyperFunTorus <- function(x, ...) {
    nrow(x$param)  
}

## hfEllipticCone
hfEllCone <- function(cx = 0, cy = 0, cz = 0, a = 0.5, b = 1, axis = "X",
                      name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(a)  &&
          is.numeric(b)))
        stop("All arguments must be numeric")
    axis = toupper(axis)
    if (!all(axis %in% c("X", "Y", "Z"))) {
        stop("Axis name must be 'X' or 'Y' or 'Z'")
    }
    params <- cbind(cx, cy, cz, a, b, axis)
    colnames(params) <- c("x", "y", "z", "a", "b", "axis")
    name <- hfName(name, nrow(params))
    ellCone <- list(params = params, name = name)
    class(ellCone) <- c("HyperFunEllipticCone", "HyperFunObject", "HyperFun")
    ellCone
}

as.character.HyperFunEllipticCone <- function(x, ...) {
    center <- paste0("[", x$params[,"x"], ",",
                     x$params[,"y"], "," ,
                     x$params[,"z"], "]")
    paste0(x$name, "(x[3], a[1])\n",
           "{\n",
           "  array center[3];\n",
           "  center = ", center, ";\n",
           "  ellcone = hfEllCone", x$params[,"axis"],"(x, center, ",
           x$params[,"a"], ",", x$params[,"b"] ,");\n",
           "  ", x$name, " = ellcone;\n",
           "}\n")
}

length.HyperFunEllipticCone <- function(x, ...) {
    nrow(x$param)  
}

## hfCone
hfCone <- function(cx = 0, cy = 0, cz = 0, r = 0.5, axis = "X", name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(r)))
        stop("All arguments must be numeric")
    axis = toupper(axis)
    if (!all(axis %in% c("X", "Y", "Z"))) {
        stop("Axis name must be 'X' or 'Y' or 'Z'")
    }
    params <- cbind(cx, cy, cz, r, axis)
    colnames(params) <- c("x", "y", "z", "r", "axis")
    name <- hfName(name, nrow(params))
    cone <- list(params = params, name = name)
    class(cone) <- c("HyperFunCone", "HyperFunObject", "HyperFun")
    cone
}

as.character.HyperFunCone <- function(x, ...) {
    center <- paste0("[", x$params[,"x"], ",",
                     x$params[,"y"], "," ,
                     x$params[,"z"], "]")
    paste0(x$name, "(x[3], a[1])\n",
           "{\n",
           "  array center[3];\n",
           "  center = ", center, ";\n",
           "  cone = hfCone", x$params[,"axis"],"(x, center, ",
           x$params[,"r"], ");\n",
           "  ", x$name, " = cone;\n",
           "}\n")
}

length.HyperFunCone <- function(x, ...) {
    nrow(x$param)  
}


## hfEllCylinder
hfEllCylinder <- function(cx = 0, cy = 0, cz = 0, a = 2, b = 4,
                          axis = "X", name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(a)  &&
          is.numeric(b)))
        stop("All arguments must be numeric")
    axis = toupper(axis)
    if (!all(axis %in% c("X", "Y", "Z"))) {
        stop("Axis name must be 'X' or 'Y' or 'Z'")
    }
    params <- cbind(cx, cy, cz, a, b, axis)
    colnames(params) <- c("x", "y", "z", "a", "b", "axis")
    name <- hfName(name, nrow(params))
    ellCylinder <- list(params = params, name = name)
    class(ellCylinder) <- c("HyperFunEllipticCylinder", "HyperFunObject",
                            "HyperFun")
    ellCylinder
}

as.character.HyperFunEllipticCylinder <- function(x, ...) {
    center <- paste0("[", x$params[,"x"], ",",
                     x$params[,"y"], "," ,
                     x$params[,"z"], "]")
    paste0(x$name, "(x[3], a[1])\n",
           "{\n",
           "  array center[3];\n",
           "  center = ", center, ";\n",
           "  ellcylinder = hfEllCyl", x$params[,"axis"],"(x, center, ",
           x$params[,"a"], ",", x$params[,"b"] ,");\n",

           "  ", x$name, " = ellcylinder;\n",
           "}\n")
}

length.HyperFunEllipticCylinder <- function(x, ...) {
    nrow(x$param)  
}

## hfCylinder
hfCylinder <- function(cx = 0, cy = 0, cz = 0, r = 1, axis = "X", name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(r)))
        stop("All arguments must be numeric")
    axis = toupper(axis)
    if (!all(axis %in% c("X", "Y", "Z"))) {
        stop("Axis name must be 'X' or 'Y' or 'Z'")
    }
    params <- cbind(cx, cy, cz, r, axis)
    colnames(params) <- c("x", "y", "z", "r", "axis")
    name <- hfName(name, nrow(params))
    cylinder <- list(params = params, name = name)
    class(cylinder) <- c("HyperFunCylinder", "HyperFunObject", "HyperFun")
    cylinder
}

as.character.HyperFunCylinder <- function(x, ...) {
    center <- paste0("[", x$params[,"x"], ",",
                     x$params[,"y"], "," ,
                     x$params[,"z"], "]")
    paste0(x$name, "(x[3], a[1])", "\n",
           "{\n",
           "  array center[3];\n",
           "  center = ", center, ";\n",
           "  cylinder = hfCylinder", x$params[,"axis"],
           "(x, center, ", x$params[,"r"], ");\n",
           "  ", x$name, " = cylinder;\n",
           "}\n")
}

length.HyperFunCylinder <- function(x, ...) {
    nrow(x$param)  
}

## hfEllipsoid
hfEllipsoid <- function(cx = 0, cy = 0, cz = 0, a = 1, b = 2, c = 3,
                        name = NULL) {
    if (!(is.numeric(cx) &&
          is.numeric(cy) &&
          is.numeric(cz) &&
          is.numeric(a)  &&
          is.numeric(b)  &&
          is.numeric(c)))
        stop("All arguments must be numeric")
    params <- cbind(cx, cy, cz, a, b, c)
    colnames(params) <- c("x", "y", "z", "a", "b", "c")
    name <- hfName(name, nrow(params))
    ellipsoid <- list(params = params, name = name)
    class(ellipsoid) <- c("HyperFunEllipsoid", "HyperFunObject", "HyperFun")
    ellipsoid
}

as.character.HyperFunEllipsoid <- function(x, ...) {
    center <- paste0("[", x$params[,"x"], ",",
                     x$params[,"y"], "," ,
                     x$params[,"z"], "]")
    halfAxes <- paste(x$params[, "a"],
                      x$params[, "b"],
                      x$params[, "c"], sep = ",")
    paste0(x$name, "(x[3], a[1])\n",
           "{\n",
           "  array center[3];\n",
           "  center = ", center, ";\n",
           "  ellipsoid = hfEllipsoid(x, center, ", halfAxes, ");\n",
           "  ", x$name, " = ellipsoid;\n",
           "}\n"
           )
}

length.HyperFunEllipsoid <- function(x, ...) {
    nrow(x$param)  
}


## hfSphere
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

