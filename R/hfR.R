
hfRmethods <- (function() {

    funList <- list(NULL)
    funListCounter <- 0
    funListSize <- 1

    ## Store functions in a list
    addFun <- function(f) {
        if (funListCounter == funListSize) {
            newSize <- 2*funListSize
            length(funList) <<- newSize
            funListSize <- newSize
        }
        funListCounter <<- funListCounter + 1
        funList[[funListCounter]] <<- f
    }

    ## Check function accepts a numeric vector of length 3 as the first argument
    ## (at least) and returns at least one numeric value.
    ## Additional arguments ignored and additional returned values ignored.
    checkFun <- function(f) {
        if (!is.function(f))
            stop("First argument to hfR() must be a function")
        value <- f(rep(0, 3))
        if (!(is.numeric(value) && length(value) >= 1)) {
            stop("Invalid hfR() function")
        }
    }
    
    registerFun <- function(f, name = NULL) {
        checkFun(f)
        addFun(f)
        name <- hfName(name, 1)
        object <- list(index = funListCounter, name = name)
        class(object) <- c("HyperFunR", "HyperFunObject", "HyperFun")
        object
    }

    callFun <- function(xyz, index) {
        if (index > funListCounter)
            stop("Undefined hfR() function")
        (funList[[index]])(xyz)
    }

    list(register = registerFun, call = callFun)
})()

hfR <- hfRmethods$register

hfRcall <- hfRmethods$call

as.character.HyperFunR <- function(x, ...) {
    paste0(x$name,
           "(x[3], a[1])\n",
           "{\n",
           "  object = hfR(x, ", x$index, ");\n",
           "  ", x$name, " = object;\n",
           "}\n")    
}
