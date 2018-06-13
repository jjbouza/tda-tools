#include <Rcpp.h>
#include "plt/PersistenceLandscape.h"
#include "plt/PersistenceBarcode.h"


//need to figure out a better way of doing what the next two functions are doing.
//Neccesary since Rcpp can not handle std::pair.
std::vector<std::vector<std::vector<double>>> persistenceDataProcess(std::vector<std::vector<std::pair<double, double>>> pl){
	std::vector<std::vector<std::vector<double>>> out;
	for(auto order : pl){
		std::vector<std::vector<double>> order_vec;
		for(auto point : order){
			std::vector<double> point_n{point.first, point.second};
			order_vec.push_back(point_n);
		}
		out.push_back(order_vec);
	}

	return out;
}

std::vector<std::pair<double, double>> rDataProcess(std::vector<std::vector<double>> pd){
	std::vector<std::pair<double,double>> output;

	for(auto point : pd)
		output.push_back(std::make_pair(point[0], point[1]));
	return output;
}

//Note: We can only allow getPersistenceLandscapeExact if the data is already represented exactly.
//To add: Average
class PersistenceLandscapeInterface{
public:
	//Creates PL from PD
	PersistenceLandscapeInterface(std::vector<std::vector<double>> persistence_diagram, bool exact){
		//Initalize a PersistenceLandscape object.
		PersistenceLandscapeInterface::exact = exact;
		auto pd = PersistenceBarcodes(rDataProcess(persistence_diagram));
		PersistenceLandscapeInterface::pl_raw = PersistenceLandscape(pd, exact);
	}
	//TEST
	std::vector<std::vector<std::vector<double>>> getPersistenceLandscapeExact(){
		if (!exact){
			std::cout << "Error: Can not convert a discrete PL to an exact PL.";
		}

		else{
			return persistenceDataProcess(pl_raw.land);
		}
	}

	//TEST
	//NOTE: This will permentantly change the representation of this to discrete.
	std::vector<std::vector<std::vector<double>>> getPersistenceLandscapeDiscrete(){
		if(exact){
 			//pl_raw.computeLandscapeOnDiscreteSetOfPoints(0.000001);
			//return persistenceDataProcess(pl_raw.land);
		}
		
		else{
			return persistenceDataProcess(pl_raw.land);
		}
	}
	
	//Adds this to another PL
	//We need to implement the appropriate functions for doing this with discrete representations.
	PersistenceLandscapeInterface sum(const PersistenceLandscapeInterface& other);
	PersistenceLandscapeInterface scale(double);
	double inner(PersistenceLandscapeInterface& other);

private:
	bool exact;
	PersistenceLandscape pl_raw;
};

using namespace Rcpp;
RCPP_EXPOSED_CLASS(PersistenceLandscapeInterface)
RCPP_MODULE(pl_interface) {

    class_<PersistenceLandscapeInterface>("PL")

    .constructor<std::vector<std::vector<double>>, bool>()

    //.method("getExact", &PersistenceLandscapeInterface::getPersistenceLandscapeExact, "Returns the PL in the exact representation.")
    //.method("getDiscrete", &PersistenceLandscapeInterface::getPersistenceLandscapeDiscrete, "Returns the PL in the discrete representation.")
    //.method("add", &PersistenceLandscapeInterface::sum, "Adds this PL to another.")
    //.method("scale", &PersistenceLandscapeInterface::scale, "Scales this PL by a scaler.")
    //.method("inner", &PersistenceLandscapeInterface::inner, "Take inner product of this PL with another.")
    ;
}

