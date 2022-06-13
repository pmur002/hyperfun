#include "Rcpp.h"

#include "hyperfun.h"

#include "main.h"

CMainApp *edi;

int vn_t;

void freeRessources (void) 
{
    if(edi!=NULL){
        delete (edi);
        edi=NULL;
    }
}

// [[Rcpp::export]]
void hyperfun(Rcpp::StringVector args) 
{

    // MAXIMUM of 22 arguments 
    // First argument is program name
    // Remaining arguments come from 'args'
    int argc = args.size() + 1;
    if (argc > 22)
        Rcpp::stop("Too many arguments.");
    char array[22][100];
    char *argv[22];
    int i;
    strcpy(array[0], "hfp");
    argv[0] = array[0];
    for (i = 1; i < argc; i++) {
        if (args[i - 1].size() > 99)
            Rcpp::stop("Argument %i too long", i + 1);
        strcpy(array[i], args[i - 1]);
        argv[i] = array[i];
    }

    edi = new CMainApp (argc, argv);
    
    vn_t = edi->init();
    if (vn_t == 0) {
        if (edi != NULL) {
            delete(edi);
            edi=NULL;
        }
        Rcpp::stop("Isosurface not generated.");
    }
    
    freeRessources();
}

// [[Rcpp::export]]
SEXP hfRinit(SEXP evalEnv) {
    hyperfunEvalEnv = evalEnv;
    return R_NilValue;
}

