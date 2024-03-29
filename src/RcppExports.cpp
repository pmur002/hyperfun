// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// hyperfun
void hyperfun(Rcpp::StringVector args);
RcppExport SEXP _hyperfun_hyperfun(SEXP argsSEXP) {
BEGIN_RCPP
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< Rcpp::StringVector >::type args(argsSEXP);
    hyperfun(args);
    return R_NilValue;
END_RCPP
}
// hfRinit
SEXP hfRinit(SEXP evalEnv);
RcppExport SEXP _hyperfun_hfRinit(SEXP evalEnvSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type evalEnv(evalEnvSEXP);
    rcpp_result_gen = Rcpp::wrap(hfRinit(evalEnv));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_hyperfun_hyperfun", (DL_FUNC) &_hyperfun_hyperfun, 1},
    {"_hyperfun_hfRinit", (DL_FUNC) &_hyperfun_hfRinit, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_hyperfun(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
