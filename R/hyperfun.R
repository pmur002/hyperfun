
hfp <- function(x, ...) {
    UseMethod("hfp")
}

hfp.HyperFunObject <- function(x, ...) {
    hfp(hfModel(x), ...)
}

hfp.HyperFunModel <- function(x, format = "stlb", ...) {
    hfFile <- tempfile(fileext = ".hf")
    outFile <- gsub("hf$", format, hfFile)
    writeLines(as.character(x), hfFile)
    hyperfun(c(hfFile, paste0("-", format), outFile))
    outFile
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

