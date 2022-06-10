
## R functions that generate FRep / HyperFun code (when printed)

hfName <- (function() {
    i <- 0
    function(name, n) {
        if (is.null(name)) {
            i <<- i + 1
            name <- i
        }
        result <- paste0("HF_", name, "_", 1:n)
        ## In case 'name' is longer than 'n'
        result[1:n]
    }
})()

getName <- function(x) {
    x$name
}

hfOp <- function(op) {
    switch(op,
           "+"="|")
}

################################################################################
## FRep primitives

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

################################################################################
## Models (combinations of objects)

hfModel <- function(..., op = "+", name = NULL) {
    objects <- list(...)
    if (!all(sapply(objects, inherits, "HyperFunObject") |
             sapply(objects, inherits, "HyperFunModel"))) {
        stop("A model can only contain HyperFun objects and/or models")
    }
    if (!(op %in% "+")) {
        stop(paste0("Operator ", op, " not supported (yet)"))
    }
    name <- hfName(name, 1)
    model <- list(objects = objects, op = op, name = name)
    class(model) <- c("HyperFunModel", "HyperFun")
    model
}

## NOTE that we MUST call the final model "my_model"
as.character.HyperFunModel <- function(x, ..., top=TRUE) {
    modelChar <- paste0(x$name,
                        "(x[3], a[1])\n",
                        "{\n",
                        "  ", x$name, " = ",
                        paste(paste0(unlist(lapply(x$objects, getName)),
                                     "(x, a)"),
                              collapse = paste0(" ", hfOp(x$op), " ")), ";\n",
                        "}\n")
    if (top) {
        modelChar <- paste0(modelChar,
                            "my_model(x[3], a[1])\n",
                            "{\n",
                            paste0("  my_model = ", x$name, "(x, a);\n"),
                            "}\n")
    }
    c(unlist(lapply(x$objects, as.character, top=FALSE)), 
      modelChar)
}

################################################################################
## General methods

print.HyperFun <- function(x, ...) {
    cat(as.character(x), sep="")
}

Ops.HyperFunObject <- function(e1, e2) {
    if (nargs() < 2) {
        stop("Unary operations not valid on HyperFun objects")
    }
    if (!(.Generic %in% "+")) {
        stop("Arithmetic operation not supported on HyperFun objects (yet)")
    }
    if (inherits(e1, "HyperFunObject") && inherits(e2, "HyperFunObject")) {
        hfModel(e1, e2, .Generic)
    } else {
        stop("Arithmetic operations only supported between HyperFun objects")
    }
}

