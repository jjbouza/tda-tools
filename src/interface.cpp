#include <Rcpp.h>
#include "pl.h"
#include "rip.h"


using namespace Rcpp;
RCPP_EXPOSED_CLASS(PersistenceLandscapeInterface)
RCPP_MODULE(Landscape) {

    class_<PersistenceLandscapeInterface>("PersistenceLandscape")
    .constructor<NumericMatrix,bool,double,double>()

    .method("getExact", &PersistenceLandscapeInterface::getPersistenceLandscapeExact, "Returns the PL in the exact representation.")
    .method("getDiscrete", &PersistenceLandscapeInterface::getPersistenceLandscapeDiscrete, "Returns the PL in the discrete representation.")
    .method("add", &PersistenceLandscapeInterface::sum, "Adds this PL to another.")
    .method("scale", &PersistenceLandscapeInterface::scale, "Scales this PL by a scaler.")
    .method("inner", &PersistenceLandscapeInterface::inner, "Take inner product of this PL with another.")
    ;

    Rcpp::function("PLsum", &PLsum);
    Rcpp::function("PLscale", &PLscale);
    Rcpp::function("PLinner", &PLinner);

}

RCPP_MODULE(Diagram) {
	Rcpp::function("rip_raw", &rip_raw);
}
