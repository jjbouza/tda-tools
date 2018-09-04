#include <Rcpp.h>
#include <limits>
#include "plt/PersistenceLandscape.h"
#include "plt/PersistenceBarcode.h"

using namespace Rcpp;

int TDIndex(int X, int Y, int Z, int x, int y, int z){
	return x+X*(y+Y*z);
}

NumericVector persistenceLandscapeToR(std::vector<std::vector<std::vector<double>>> input){
	Dimension d(input.size(), input[0].size(), 2);
	NumericVector out(d[0]*d[1]*d[2]);
	NumericVector out_d(d);

	for(int j = 0; j < input.size(); j++){
		for(int i = 0; i < input[0].size(); i++){
			out[TDIndex(input.size(), input[0].size(), 2, j, i, 0)] = input[j][i][0];
			out[TDIndex(input.size(), input[0].size(), 2, j, i, 1)] = input[j][i][1];

			if(input[j][i][0] == INT_MAX)
				out[TDIndex(input.size(), input[0].size(), 2, j, i, 0)] = R_PosInf;

		}
	}

	std::copy(out.begin(), out.end(), out_d.begin());
	return out_d;
}


//need to figure out a better way of doing what the next two functions are doing.
//Neccesary since Rcpp can not handle std::pair.
NumericVector persistenceDataProcess(std::vector<std::vector<std::pair<double, double>>> pl){
	std::vector<std::vector<std::vector<double>>> out;
	for(auto order : pl){
		std::vector<std::vector<double>> order_vec;
		for(auto point : order){
			std::vector<double> point_n{point.first, point.second};
			order_vec.push_back(point_n);
		}
		out.push_back(order_vec);
	}

	return persistenceLandscapeToR(out);
}

std::vector<std::pair<double, double>> rDataProcess(NumericMatrix pd, double max){
	std::vector<std::pair<double,double>> output;
	for(int i = 0; i < pd.nrow(); i++)
		output.push_back(std::make_pair(pd(i,0), std::min(pd(i,1), max)));

	return output;
}

std::vector<std::vector<std::pair<double, double>>> addDiscreteLandscapes(const PersistenceLandscape& l1, const PersistenceLandscape& l2){

	int min_level = std::min(l1.land.size(), l2.land.size());
	std::vector<std::vector<std::pair<double, double>>> out;
	for(int i = 0; i < min_level; i++){
		int min_index = std::min(l1.land[i].size(), l2.land[i].size());
		std::vector<std::pair<double, double>> level_out;
		for(int j = 0; j < min_index; j++)
			level_out.push_back(std::make_pair(l1.land[i][j].first, l1.land[i][j].second+l2.land[i][j].second));

		for(; min_index < l1.land[i].size(); min_index++)
			level_out.push_back(l1.land[i][min_index]);

		for(; min_index < l2.land[i].size(); min_index++)
			level_out.push_back(l2.land[i][min_index]);

		out.push_back(level_out);
	}

	for(; min_level < l1.land.size(); min_level++)
		out.push_back(l1.land[min_level]);

	for(; min_level < l2.land.size(); min_level++)
		out.push_back(l2.land[min_level]);

	return out;
}

std::vector<std::vector<std::pair<double, double>>> scaleDiscreteLandscapes(double scale, PersistenceLandscape l){
	std::vector<std::vector<std::pair<double, double>>> out;
	for(std::vector<std::pair<double,double>> level : l.land){
		std::vector<std::pair<double, double>> level_out;
		for (std::pair<double,double> pair : level)
			level_out.push_back(std::make_pair(pair.first, scale*pair.second));
		out.push_back(level_out);
	}

	return out;
}

std::vector<std::vector<std::pair<double,double>>> exactLandscapeToDiscrete(PersistenceLandscape l, double dx, double max_x){
	std::vector<std::vector<std::pair<double,double>>> out;
	for(std::vector<std::pair<double,double>> level : l.land){
		std::vector<std::pair<double,double>> level_out;

		std::pair<double,double> previous_point = std::make_pair(0,0);
		double x_buffer = previous_point.first;
		double y_buffer = previous_point.second;

		for(int i = 0; i < level.size(); i++){
			std::pair<double, double> point = level[i];
			//Make sure slope is well defined:
			if(point.first != previous_point.first){
				double delta_x = level[i].first - previous_point.first;
				double delta_y = level[i].second - previous_point.second;
				double slope = delta_y/delta_x;

				while(x_buffer < point.first && x_buffer < max_x){
					y_buffer += dx*slope;
					x_buffer += dx;
					level_out.push_back(std::make_pair(x_buffer, y_buffer));
				}
			}

			previous_point = point;
		}

		out.push_back(level_out);
	}

	return out;
}

double innerProductDiscreteLandscapes(PersistenceLandscape l1, PersistenceLandscape l2, double dx){
	int min_level = std::min(l1.land.size(), l2.land.size());
	double integral_buffer = 0;
	for(int i = 0; i < min_level; i++){
		int min_index = std::min(l1.land[i].size(), l2.land[i].size());
		for(int j = 0; j < min_index; j++)
			integral_buffer += l1.land[i][j].second*l2.land[i][j].second;
	}

	return integral_buffer*dx;
}

std::vector<std::pair<double,double>> generateGrid(double start, double end, double dx){
	std::vector<std::pair<double,double>> grid;
	for(double current = start; current < end; current+=dx)
		grid.push_back(std::make_pair(current, 0));
	
	return grid;
}

class PersistenceLandscapeInterface{
public:
	//Creates PL from PD
	//TODO: Better defaults.
	PersistenceLandscapeInterface(NumericMatrix persistence_diagram, bool exact=false, double min_pl=0, double max_pl=10, double dx=0.01, double max_y=10) : exact(exact), min_pl(min_pl), max_pl(max_pl), dx(dx){
		//Initalize a PersistenceLandscape object.
		auto pd = PersistenceBarcodes(rDataProcess(persistence_diagram, max_y));
		PersistenceLandscapeInterface::pl_raw = PersistenceLandscape(pd, exact, 2*dx, min_pl, max_pl);
	}

	PersistenceLandscapeInterface(PersistenceLandscape pl, bool exact, double min_pl, double max_pl, double dx) : pl_raw(pl), exact(exact), min_pl(min_pl), max_pl(max_pl), dx(dx){}


	NumericVector getPersistenceLandscapeExact(){
		if (!exact){
			stop("Error: Can not convert a discrete PL to an exact PL.");
		}

		else{
			return persistenceDataProcess(pl_raw.land);
		}
	}
	
	NumericVector getPersistenceLandscapeDiscrete(){
		if(exact){
			return persistenceDataProcess(exactLandscapeToDiscrete(pl_raw.land, dx, max_pl));
		}
		else{
			return persistenceDataProcess(pl_raw.land);
		}
	}

	NumericVector getInternal(){
		return persistenceDataProcess(pl_raw.land);
	}

	//Adds this to another PL
	PersistenceLandscapeInterface sum(const PersistenceLandscapeInterface& other){
		PersistenceLandscape pl_out;
		if(exact)
			pl_out = pl_raw+other.pl_raw;
		else{
			if (!checkPairOfLandscapes(*this, other)) {
				stop("Persistence Landscape Properties Do Not Match.");
			}
			pl_out = PersistenceLandscape(addDiscreteLandscapes(pl_raw, other.pl_raw));
		}
		
		return PersistenceLandscapeInterface(pl_out, exact, min_pl, max_pl, dx);
	}


	double inner(PersistenceLandscapeInterface& other){
		double scaler_out;
		if (exact)
			scaler_out = computeInnerProduct(pl_raw, other.pl_raw);

		else
			scaler_out = innerProductDiscreteLandscapes(pl_raw, other.pl_raw, dx);
		
		return scaler_out;
	}

	PersistenceLandscapeInterface scale(double scale){
		PersistenceLandscape pl_out;
		if (exact)
			pl_out = scale*pl_raw;

		else
			pl_out = PersistenceLandscape(scaleDiscreteLandscapes(scale, pl_raw));

		return PersistenceLandscapeInterface(pl_out, exact, min_pl,max_pl, dx);
	}

	
	
		friend bool checkPairOfLandscapes(PersistenceLandscapeInterface& l1, const PersistenceLandscapeInterface& l2);
private:
	bool exact;
	PersistenceLandscape pl_raw;
	double max_pl = 2;
	double min_pl = 0;
	double dx = 0.001;
};


PersistenceLandscapeInterface PLsum(PersistenceLandscapeInterface p1, PersistenceLandscapeInterface p2){
	return p1.sum(p2);
}

PersistenceLandscapeInterface PLscale(double scale, PersistenceLandscapeInterface p){
	return p.scale(scale);
}


double PLinner(PersistenceLandscapeInterface p1, PersistenceLandscapeInterface p2){
	return p1.inner(p2);
}


bool checkPairOfLandscapes(PersistenceLandscapeInterface& l1, const PersistenceLandscapeInterface& l2){
	if(l1.min_pl != l2.min_pl || l1.max_pl != l2.max_pl || l1.dx != l2.dx)
		return false;
	return true;
}

