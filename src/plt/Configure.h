//    Copyright 2013-2014 University of Pennsylvania
//    Created by Pawel Dlotko
//
//    This file is part of Persistence Landscape Toolbox (PLT).
//
//    PLT is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or (at
//    your option) any later version.
//
//    PLT is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with PLT.  If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include <fstream>
#include <iostream>
#include <sstream>

// global variables and their default values.
bool areThereInfiniteIntervals = false;
double infty = -1;
bool shallInfiniteBarcodesBeIgnored = true;
double valueOfInfinity = INT_MAX;
bool useGridInComputations = false;
double gridDiameter = 0.01;
double epsi = 0.000005;

using namespace std;

void configure() {
  bool dbg = false;

  ifstream in;
  in.open((char *)"configure");

  if (!in.good()) {
    cerr << "The configure file is not present in the folder. Program will now "
            "terminate. Please put the original configure file to this folder "
            "and try again \n";
    throw("The configure file is not present in the folder. Program will now "
          "terminate. Please put the original configure file to this folder "
          "and try again \n");
  }

  std::string line;
  getline(in, line);

  bool isThisAFirsLine = true;

  double vaiableNumber = 0;
  while (!in.eof()) {
    getline(in, line);
    if (!(line.length() == 0 || line[0] == '#')) {
      std::stringstream lineSS;
      lineSS << line;

      if (vaiableNumber == 0) {
        int i;
        lineSS >> i;
        if (i == 1) {
          areThereInfiniteIntervals = true;
        } else {
          areThereInfiniteIntervals = false;
        }
        if (dbg) {
          cerr << "areThereInfiniteIntervals : " << areThereInfiniteIntervals
               << endl;
        }
      }
      if (vaiableNumber == 1) {
        lineSS >> infty;
        if (dbg) {
          cerr << "infty : " << infty << endl;
        }
      }
      if (vaiableNumber == 2) {
        lineSS >> shallInfiniteBarcodesBeIgnored;
        if (dbg) {
          cerr << "shallInfiniteBarcodesBeIgnored : "
               << shallInfiniteBarcodesBeIgnored << endl;
        }
      }
      if (vaiableNumber == 3) {
        lineSS >> valueOfInfinity;
        if (dbg) {
          cerr << "valueOfInfinity : " << valueOfInfinity << endl;
        }
      }
      if (vaiableNumber == 4) {
        lineSS >> useGridInComputations;
        if (dbg) {
          cerr << "useGridInComputations : " << useGridInComputations << endl;
        }
      }
      if (vaiableNumber == 5) {
        lineSS >> gridDiameter;
        if (dbg) {
          cerr << "gridDiameter : " << gridDiameter << endl;
        }
      }
      if (vaiableNumber == 6) {
        lineSS >> epsi;
        if (dbg) {
          cerr << "epsi : " << epsi << endl;
        }
      }
      ++vaiableNumber;
    } else {
      if (dbg) {
        std::cout << "IGNORE THIS LINE : " << line << endl;
      }
      isThisAFirsLine = false;
    }
  }
  in.close();
}

// this is an old version of a configuration which do not use data readed from
// file. It require re-compilation each time the program is used.
/*
//This is a configuration file for a Persistence Landscape Toolbox (PLT). NOTE
THAT THE
//PLT LIBRARY HAVE TO BE RE-COMPILED WHEN ANY OF THIS PARAMETERS IS CHANGED.

//If in your files with persistence intervals there are infinite ones, please
set the
//variable areThereInfiniteIntervals to true. Otherwise, please set it up to
false. bool areThereInfiniteIntervals = false;
//If there are infinite persistence intervals, please provide here a values
which
//should be read as plus infinity:
double infty= -1;
//If there are infinite barcodes possible, they either can be ignored or not by
the PLT.
//If you do not wnat them to be ignored, please set
shallInfiniteBarcodesBeIgnored to false. bool shallInfiniteBarcodesBeIgnored =
true;
//If the infinite barcodes are not to be ignored and the variable
shallInfiniteBarcodesBeIgnored
//is set to false, then please provide below the value which should be used by
PLT in place of infinity: double valueOfInfinity = INT_MAX;

//The Persistence Landscape Toolbox (PLT) can compute distances using two
different
//representations of landscapes. The first, and default one, is the
representation that
//uses pairs (critical point, critical value). Alternative representation, which
may be
//faster in some cases, is a grid-base one. In case of grid-base representation
we assume
//that the begin and end points of the intervals are sampled from a finite,
uniformly
//distributed grid. If this is the case in your computations, please set the
variable
//useGridInComputations to true.
bool useGridInComputations = false;
//in case grid is used in a computations, you need to provide a grid diameter
here. double gridDiameter = 0.01;



//A small number, used for a debugging purposes. Ignore when not in the
debugging mode. double epsi = 0.000005;
*/
