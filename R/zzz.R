
## For evaluation of non-exported R function hfRcall() in C code
.hyperfunEvalEnv <- new.env()

.onLoad <- function(libname, pkgname)
{
    environment(.hyperfunEvalEnv) <- asNamespace("hyperfun")
    hfRinit(.hyperfunEvalEnv)
}

