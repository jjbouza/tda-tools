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
#include "PersistenceLandscape.h"
#include "VectorSpaceOfPersistenceLandscapes.h"
#include <cmath>
#include <vector>

using namespace std;

template <typename DescriptorOfTopology> class anova {
public:
  anova(std::vector<std::vector<DescriptorOfTopology>>
            populationOfTopologicalInvariants,
        double (*functionToUse)(const DescriptorOfTopology &));

  double FCharacteristic() {
    std::cout << "The number of DOF of numerator is : "
              << this->populationOfTopologicalInvariants.size() - 1
              << std::endl;
    std::cout << "The number of DOF of denominator is : "
              << (this->totalNumberOfTopologicalInvariants -
                  this->populationOfTopologicalInvariants.size())
              << std::endl;
    std::cout << "The value of F statistic is : " << this->MST / this->MSE
              << ". For the p-values please consult the appropriate tables \n";
    return this->MST / this->MSE;
  }

  void printStatistics();

private:
  void computeBasicStatistics();
  std::vector<std::vector<DescriptorOfTopology>>
      populationOfTopologicalInvariants;
  double (*characteristic)(const DescriptorOfTopology &);
  std::vector<double> averagesOfPopulations;
  std::vector<double> standardDeviationOfPopulations;
  std::vector<size_t> deegreesOfFreedomOfPopulation;
  double overalMean;
  size_t totalNumberOfTopologicalInvariants;

  double SSTotal;
  double SST;
  double SSE;

  double MST;
  double MSE;
};

template <typename DescriptorOfTopology>
void anova<DescriptorOfTopology>::computeBasicStatistics() {
  // http://cba.ualr.edu/smartstat/topics/anova/example.pdf
  bool dbg = false;
  // here we compute the values of a characteristic functions of the topological
  // invariants. This is the first and the last point in which the topological
  // invariants are used in this program.
  std::vector<std::vector<double>> characteristicOfAllPopulations;
  for (size_t populationNo = 0;
       populationNo != this->populationOfTopologicalInvariants.size();
       ++populationNo) {
    // computing averages
    std::vector<double> valuesOfCharacteristicForThisPopulation;
    for (size_t insidePopulation = 0;
         insidePopulation !=
         this->populationOfTopologicalInvariants[populationNo].size();
         ++insidePopulation) {
      double valueForThisElementOfPopulation = characteristic(
          this->populationOfTopologicalInvariants[populationNo]
                                                 [insidePopulation]);
      valuesOfCharacteristicForThisPopulation.push_back(
          valueForThisElementOfPopulation);
    }
    characteristicOfAllPopulations.push_back(
        valuesOfCharacteristicForThisPopulation);
  }
  // no persistnece landscapes are used beyond this point

  if (dbg) {
    cerr << "We are in the procedure computeBasicStatistics, starting "
            "computing basic statistics\n";
  }

  this->overalMean = 0;
  this->totalNumberOfTopologicalInvariants = 0;
  for (size_t populationNo = 0;
       populationNo != characteristicOfAllPopulations.size(); ++populationNo) {
    if (dbg) {
      cerr << "populationNo : " << populationNo << endl;
    }
    // computing averages
    double averageOfThisPopulation = 0;
    for (size_t insidePopulation = 0;
         insidePopulation !=
         characteristicOfAllPopulations[populationNo].size();
         ++insidePopulation) {
      averageOfThisPopulation +=
          characteristicOfAllPopulations[populationNo][insidePopulation];
      this->overalMean +=
          characteristicOfAllPopulations[populationNo][insidePopulation];
    }
    this->deegreesOfFreedomOfPopulation.push_back(
        characteristicOfAllPopulations[populationNo].size());
    averageOfThisPopulation /=
        (double)characteristicOfAllPopulations[populationNo].size();
    this->averagesOfPopulations.push_back(averageOfThisPopulation);
    this->totalNumberOfTopologicalInvariants +=
        characteristicOfAllPopulations[populationNo].size();

    // computing standard deviation:
    double standardDeviation = 0;
    for (size_t insidePopulation = 0;
         insidePopulation !=
         characteristicOfAllPopulations[populationNo].size();
         ++insidePopulation) {
      standardDeviation +=
          pow((characteristicOfAllPopulations[populationNo][insidePopulation] -
               averageOfThisPopulation),
              2);
    }
    standardDeviation /= characteristicOfAllPopulations[populationNo].size();
    standardDeviation = sqrt(standardDeviation);

    this->standardDeviationOfPopulations.push_back(standardDeviation);
  }
  if (dbg) {
    cerr << "Exiting loop in which we compute basic average and standard "
            "deviations of populations \n";
  }
  this->overalMean /= (double)this->totalNumberOfTopologicalInvariants;

  this->SSTotal = 0;
  for (size_t populationNo = 0;
       populationNo != characteristicOfAllPopulations.size(); ++populationNo) {
    for (size_t insidePopulation = 0;
         insidePopulation !=
         characteristicOfAllPopulations[populationNo].size();
         ++insidePopulation) {
      this->SSTotal +=
          pow((characteristicOfAllPopulations[populationNo][insidePopulation] -
               this->overalMean),
              2);
    }
  }
  if (dbg) {
    cerr << "SSTotal has been computed \n";
  }

  this->SST = 0;
  this->SSE = 0;
  for (size_t populationNo = 0; populationNo != averagesOfPopulations.size();
       ++populationNo) {
    this->SST +=
        pow((this->averagesOfPopulations[populationNo] - this->overalMean), 2) *
        this->deegreesOfFreedomOfPopulation[populationNo];

    this->SSE += (this->deegreesOfFreedomOfPopulation[populationNo]) *
                 pow((this->standardDeviationOfPopulations[populationNo]), 2);
  }
  if (dbg) {
    cerr << "SST and SSE has been computed \n";
    std::cout << "this->SSTotal : " << this->SSTotal << std::endl;
    std::cout << "this->SST + this->SSE : " << this->SST + this->SSE
              << std::endl;
    std::cout << "this->SST : " << this->SST << std::endl;
    std::cout << "this->SSE : " << this->SSE << std::endl;
  }

  this->MST = this->SST / (this->populationOfTopologicalInvariants.size() - 1);
  this->MSE = this->SSE / (this->totalNumberOfTopologicalInvariants -
                           this->populationOfTopologicalInvariants.size());

} // computeBasicStatistics

template <typename DescriptorOfTopology>
anova<DescriptorOfTopology>::anova(
    std::vector<std::vector<DescriptorOfTopology>>
        populationOfTopologicalInvariants,
    double (*functionToUse)(const DescriptorOfTopology &)) {
  bool dbg = true;
  this->characteristic = functionToUse;
  for (size_t i = 0; i != populationOfTopologicalInvariants.size(); ++i) {
    this->populationOfTopologicalInvariants.push_back(
        populationOfTopologicalInvariants[i]);
  }
  if (dbg) {
    cerr << "Data read, now computing basic statistics \n";
  }
  this->computeBasicStatistics();
}

template <typename DescriptorOfTopology>
void anova<DescriptorOfTopology>::printStatistics() {
  cerr << "overalMean : " << this->overalMean << endl;
  cerr << "totalNumberOfTopologicalInvariants : "
       << this->totalNumberOfTopologicalInvariants << endl;
  cerr << "SSTotal : " << this->SSTotal << endl;
  cerr << "SST : " << this->SST << endl;
  cerr << "SSE : " << this->SSE << endl;
  cerr << "MST : " << this->MST << endl;
  cerr << "MSE : " << this->MSE << endl;

  cerr << "averagesOfPopulations : " << endl;
  for (size_t i = 0; i != averagesOfPopulations.size(); ++i) {
    cerr << averagesOfPopulations[i] << endl;
  }
  cerr << "standardDeviationOfPopulations : " << endl;
  for (size_t i = 0; i != standardDeviationOfPopulations.size(); ++i) {
    cerr << standardDeviationOfPopulations[i] << endl;
  }
  cerr << "deegreesOfFreedomOfPopulation : " << endl;
  for (size_t i = 0; i != deegreesOfFreedomOfPopulation.size(); ++i) {
    cerr << deegreesOfFreedomOfPopulation[i] << endl;
  }
}
