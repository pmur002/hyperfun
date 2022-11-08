
hfp <- function(x, ...) {
    UseMethod("hfp")
}

hfp.HyperFunObject <- function(x, ...) {
    hfp(hfModel(x), ...)
}

fixDensity <- function(density) {
    density <- as.numeric(density)
    n <- length(density)
    if (n < 1 || n == 2 || n > 3)
        stop("Invalid density")
    rep(density, length.out=3)
}

fixBBox <- function(bbox) {
    bbox <- as.numeric(bbox)
    n <- length(bbox)
    if (n < 1 || (n > 3 && n < 6) || n > 6) {
        stop("Invalid bounding box")
    }
    if (n == 1) {
            rep(c(-bbox, bbox), each=3)
    } else if (n == 2) {
        rep(bbox, each=3)
    } else if (n == 3) {
        as.numeric(cbind(-bbox, bbox))
    } else {
        bbox
    }
}

hfp.HyperFunModel <- function(x, format = "rgl",
                              bbox = 10, density = 30, ...) {
    hfp(as.character(x),
        format=format, bbox=bbox, density=density,
        ...)
}

## Single character values is taken to be the name of a .hf file
## Multiple character values are taken to be lines of HyperFun code
hfp.character <- function(x, format = "rgl",
                          bbox = 10, density = 30, ...) {
    if (length(x) == 1) {
        hfcode <- readLines(x)
        if (length(hfcode) < 2)
            stop("File does not appear to contain HyperFun code")
        return(hfp(hfcode))
    } 
    ## Assemble arguments for 'hyperfun'
    if (format == "rgl") {
        plot <- TRUE
        format <- "stlb"
    } else {
        plot <- FALSE
    }
    density <- fixDensity(density)
    bbox <- fixBBox(bbox)
    hfFile <- tempfile(fileext = ".hf")
    outFile <- gsub("hf$", format, hfFile)
    params <- c(paste0("-", format), outFile,
                "-b", paste0(bbox, collapse=","),
                "-g", paste0(density, collapse=","))
    ## Write .hf file
    writeLines(x, hfFile)
    ## Call 'hfp' 
    hyperfun(c(hfFile, params))
    ## Render result (optionally)
    if (plot) {
        rgl::readSTL(outFile, ...)
    }
    ## Return output file
    invisible(outFile)
}

################################################################################
## HyperFun models (combinations of objects)

hfModel <- function(..., op = "|", name = NULL) {
    objects <- list(...)
    if (length(objects) < 1)
        stop("Model must contain at least one object or model")
    if (!all(sapply(objects, inherits, "HyperFunObject") |
             sapply(objects, inherits, "HyperFunModel"))) {
        stop("A model can only contain HyperFun objects and/or models")
    }
    if (length(op) > 1) {
        op <- op[1]
        warning("Ignoring operators after first operator")
    }
    if (op == "!" &&
        (length(objects) > 1 || length(objects[[1]]) > 1))
        stop("Logical not is only valid on single object or model")
    if (!(op %in% c("|", "&", "-", "!", "*"))) {
        stop(paste0("Operator ", op, " not supported"))
    }
    ## IF making a model based on more than 1 object (i.e., combining objects)
    ## THEN "reduce" any object that represents more than one shape to
    ## a single model (using "|" for 'op')
    if (length(objects) > 1) {
        objects <- lapply(objects,
                          function(x) {
                              if (length(x) > 1)
                                  hfModel(x)
                              else
                                  x
                          })
    }
    name <- hfName(name, 1)
    model <- list(objects = objects, op = op, name = name)
    class(model) <- c("HyperFunModel", "HyperFun")
    model
}

## NOTE that we MUST call the final model "my_model"
as.character.HyperFunModel <- function(x, ..., top=TRUE) {
    ## Special case unary ! (not)
    if (x$op == "!") {
        modelChar <- paste0(x$name,
                            "(x[3], a[1])\n",
                            "{\n",
                            "  ", x$name, " = ",
                            ## Constructor ensure 'objects' is length 1
                            hfOp(x$op), getName(x$objects[[1]]), "(x, a);\n",
                            "}\n")
    } else {
        modelChar <- paste0(x$name,
                            "(x[3], a[1])\n",
                            "{\n",
                            "  ", x$name, " = ",
                            paste(paste0(unlist(lapply(x$objects, getName)),
                                         "(x, a)"),
                                  collapse = paste0(" ", hfOp(x$op), " ")),
                            ";\n",
                            "}\n")
    }
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

length.HyperFunModel <- function(x, ...) {
    1
}


################################################################################
## Plot methods

plot.HyperFunObject <- function(x, ...) {
    plot(hfModel(x), ...)
}

plot.HyperFunModel <- function(x, ...) {
    hfp(x, format="rgl", ...)
}


################################################################################
## General methods

Ops.HyperFun <- function(e1, e2) {
    if (nargs() < 2) {
        if (!(.Generic == "!"))
            stop("Unary operations not valid on HyperFun objects")
        hfModel(e1, op = .Generic)
    } else {
        if (!(.Generic %in% c("|", "&", "-", "*"))) {
            stop("Operation not supported on HyperFun objects")
        }
        if (inherits(e1, "HyperFun") && inherits(e2, "HyperFun")) {
            hfModel(e1, e2, op = .Generic)
        } else {
            stop("Operations only supported between HyperFun objects")
        }
    }
}

