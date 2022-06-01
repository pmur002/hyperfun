#include "Rcpp.h"

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

    int argc = args.size();
    // MAXIMUM of 21 arguments 
    if (argc > 21)
        Rcpp::stop("Too many arguments.");
    char *argv[21];
    char array[21][100];
    int i;
    for (i = 0; i < argc; i++) {
        if (args[i].size() > 99)
            Rcpp::stop("Argument %i too long", i + 1);
        strcpy(array[i], args[i]);
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

