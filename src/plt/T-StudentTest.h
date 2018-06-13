#pragma once
#include "PersistenceLandscape.h"
#include "VectorSpaceOfPersistenceLandscapes.h"
#include <vector>
#include <cmath>


using namespace std;

//jezeli wartosc statystyki t dla danej ilosci stopni swobody oraz danej kolumny jest mniejsza od tego co jest w danym miejscu tabeli, to z prawdopodobiuenstwem
//w naglowku kolumny mozemy przyjac, ze hipoteza zerowa jest prawdziwa.

//TABLES FOR T-STUDENT TEST
//One Sided	                   75%	   80%	   85%	   90%	   95%	 97.5%	   99%	 99.5%	 99.75%	  99.9%	 99.95%
//Two Sided             	   50%	   60%	   70%	   80%	   90%	   95%	   98%	   99%	  99.5%	  99.8%	  99.9%
static const double arr1[] = {1.000 , 1.376 , 1.963 , 3.078 , 6.314 , 12.71 , 31.82 , 63.66 , 127.3 , 318.3 , 636.6};
static const double arr2[] = {0.816 , 1.061 , 1.386 , 1.886 , 2.920 , 4.303 , 6.965 , 9.925 , 14.09 , 22.33 , 31.60};
static const double arr3[] = {0.765 , 0.978 , 1.250 , 1.638 , 2.353 , 3.182 , 4.541 , 5.841 , 7.453 , 10.21 , 12.92};
static const double arr4[] = {0.741 , 0.941 , 1.190 , 1.533 , 2.132 , 2.776 , 3.747 , 4.604 , 5.598 , 7.173 , 8.610};
static const double arr5[] = {0.727 , 0.920 , 1.156 , 1.476 , 2.015 , 2.571 , 3.365 , 4.032 , 4.773 , 5.893 , 6.869};
static const double arr6[] = {0.718 , 0.906 , 1.134 , 1.440 , 1.943 , 2.447 , 3.143 , 3.707 , 4.317 , 5.208 , 5.959};
static const double arr7[] = {0.711 , 0.896 , 1.119 , 1.415 , 1.895 , 2.365 , 2.998 , 3.499 , 4.029 , 4.785 , 5.408};
static const double arr8[] = {0.706 , 0.889 , 1.108 , 1.397 , 1.860 , 2.306 , 2.896 , 3.355 , 3.833 , 4.501 , 5.041};
static const double arr9[] = {0.703 , 0.883 , 1.100 , 1.383 , 1.833 , 2.262 , 2.821 , 3.250 , 3.690 , 4.297 , 4.781};
static const double arr10[] = {0.700 , 0.879 , 1.093 , 1.372 , 1.812 , 2.228 , 2.764 , 3.169 , 3.581 , 4.144 , 4.587};
static const double arr11[] = {0.697 , 0.876 , 1.088 , 1.363 , 1.796 , 2.201 , 2.718 , 3.106 , 3.497 , 4.025 , 4.437};
static const double arr12[] = {0.695 , 0.873 , 1.083 , 1.356 , 1.782 , 2.179 , 2.681 , 3.055 , 3.428 , 3.930 , 4.318};
static const double arr13[] = {0.694 , 0.870 , 1.079 , 1.350 , 1.771 , 2.160 , 2.650 , 3.012 , 3.372 , 3.852 , 4.221};
static const double arr14[] = {0.692 , 0.868 , 1.076 , 1.345 , 1.761 , 2.145 , 2.624 , 2.977 , 3.326 , 3.787 , 4.140};
static const double arr15[] = {0.691 , 0.866 , 1.074 , 1.341 , 1.753 , 2.131 , 2.602 , 2.947 , 3.286 , 3.733 , 4.073};
static const double arr16[] = {0.690 , 0.865 , 1.071 , 1.337 , 1.746 , 2.120 , 2.583 , 2.921 , 3.252 , 3.686 , 4.015};
static const double arr17[] = {0.689 , 0.863 , 1.069 , 1.333 , 1.740 , 2.110 , 2.567 , 2.898 , 3.222 , 3.646 , 3.965};
static const double arr18[] = {0.688 , 0.862 , 1.067 , 1.330 , 1.734 , 2.101 , 2.552 , 2.878 , 3.197 , 3.610 , 3.922};
static const double arr19[] = {0.688 , 0.861 , 1.066 , 1.328 , 1.729 , 2.093 , 2.539 , 2.861 , 3.174 , 3.579 , 3.883};
static const double arr20[] = {0.687 , 0.860 , 1.064 , 1.325 , 1.725 , 2.086 , 2.528 , 2.845 , 3.153 , 3.552 , 3.850};
static const double arr21[] = {0.686 , 0.859 , 1.063 , 1.323 , 1.721 , 2.080 , 2.518 , 2.831 , 3.135 , 3.527 , 3.819};
static const double arr22[] = {0.686 , 0.858 , 1.061 , 1.321 , 1.717 , 2.074 , 2.508 , 2.819 , 3.119 , 3.505 , 3.792};
static const double arr23[] = {0.685 , 0.858 , 1.060 , 1.319 , 1.714 , 2.069 , 2.500 , 2.807 , 3.104 , 3.485 , 3.767};
static const double arr24[] = {0.685 , 0.857 , 1.059 , 1.318 , 1.711 , 2.064 , 2.492 , 2.797 , 3.091 , 3.467 , 3.745};
static const double arr25[] = {0.684 , 0.856 , 1.058 , 1.316 , 1.708 , 2.060 , 2.485 , 2.787 , 3.078 , 3.450 , 3.725};
static const double arr26[] = {0.684 , 0.856 , 1.058 , 1.315 , 1.706 , 2.056 , 2.479 , 2.779 , 3.067 , 3.435 , 3.707};
static const double arr27[] = {0.684 , 0.855 , 1.057 , 1.314 , 1.703 , 2.052 , 2.473 , 2.771 , 3.057 , 3.421 , 3.690};
static const double arr28[] = {0.683 , 0.855 , 1.056 , 1.313 , 1.701 , 2.048 , 2.467 , 2.763 , 3.047 , 3.408 , 3.674};
static const double arr29[] = {0.683 , 0.854 , 1.055 , 1.311 , 1.699 , 2.045 , 2.462 , 2.756 , 3.038 , 3.396 , 3.659};
static const double arr30[] = {0.683 , 0.854 , 1.055 , 1.310 , 1.697 , 2.042 , 2.457 , 2.750 , 3.030 , 3.385 , 3.646};
static const double arr40[] = {0.681 , 0.851 , 1.050 , 1.303 , 1.684 , 2.021 , 2.423 , 2.704 , 2.971 , 3.307 , 3.551};
static const double arr50[] = {0.679 , 0.849 , 1.047 , 1.299 , 1.676 , 2.009 , 2.403 , 2.678 , 2.937 , 3.261 , 3.496};
static const double arr60[] = {0.679 , 0.848 , 1.045 , 1.296 , 1.671 , 2.000 , 2.390 , 2.660 , 2.915 , 3.232 , 3.460};
static const double arr80[] = {0.678 , 0.846 , 1.043 , 1.292 , 1.664 , 1.990 , 2.374 , 2.639 , 2.887 , 3.195 , 3.416};
static const double arr100[] = {0.677 , 0.845 , 1.042 , 1.290 , 1.660 , 1.984 , 2.364 , 2.626 , 2.871 , 3.174 , 3.390};
static const double arr120[] = {0.677 , 0.845 , 1.041 , 1.289 , 1.658 , 1.980 , 2.358 , 2.617 , 2.860 , 3.160 , 3.373};
static const double arrInfty[] = {0.674 , 0.842 , 1.036 , 1.282 , 1.645 , 1.960 , 2.326 , 2.576 , 2.807 , 3.090 , 3.291};

vector<double> vec1(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
vector<double> vec2(arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]) );
vector<double> vec3(arr3, arr3 + sizeof(arr3) / sizeof(arr3[0]) );
vector<double> vec4(arr4, arr4 + sizeof(arr4) / sizeof(arr4[0]) );
vector<double> vec5(arr5, arr5 + sizeof(arr5) / sizeof(arr5[0]) );
vector<double> vec6(arr6, arr6 + sizeof(arr6) / sizeof(arr6[0]) );
vector<double> vec7(arr7, arr7 + sizeof(arr7) / sizeof(arr7[0]) );
vector<double> vec8(arr8, arr8 + sizeof(arr8) / sizeof(arr8[0]) );
vector<double> vec9(arr9, arr9 + sizeof(arr9) / sizeof(arr9[0]) );
vector<double> vec10(arr10, arr10 + sizeof(arr10) / sizeof(arr10[0]) );
vector<double> vec11(arr11, arr11 + sizeof(arr11) / sizeof(arr11[0]) );
vector<double> vec12(arr12, arr12 + sizeof(arr12) / sizeof(arr12[0]) );
vector<double> vec13(arr13, arr13 + sizeof(arr13) / sizeof(arr13[0]) );
vector<double> vec14(arr14, arr14 + sizeof(arr14) / sizeof(arr14[0]) );
vector<double> vec15(arr15, arr15 + sizeof(arr15) / sizeof(arr15[0]) );
vector<double> vec16(arr16, arr16 + sizeof(arr16) / sizeof(arr16[0]) );
vector<double> vec17(arr17, arr17 + sizeof(arr17) / sizeof(arr17[0]) );
vector<double> vec18(arr18, arr18 + sizeof(arr18) / sizeof(arr18[0]) );
vector<double> vec19(arr19, arr19 + sizeof(arr19) / sizeof(arr19[0]) );
vector<double> vec20(arr20, arr20 + sizeof(arr20) / sizeof(arr20[0]) );
vector<double> vec21(arr21, arr21 + sizeof(arr21) / sizeof(arr21[0]) );
vector<double> vec22(arr22, arr22 + sizeof(arr22) / sizeof(arr22[0]) );
vector<double> vec23(arr23, arr23 + sizeof(arr23) / sizeof(arr23[0]) );
vector<double> vec24(arr24, arr24 + sizeof(arr24) / sizeof(arr24[0]) );
vector<double> vec25(arr25, arr25 + sizeof(arr25) / sizeof(arr25[0]) );
vector<double> vec26(arr26, arr26 + sizeof(arr26) / sizeof(arr26[0]) );
vector<double> vec27(arr27, arr27 + sizeof(arr27) / sizeof(arr27[0]) );
vector<double> vec28(arr28, arr28 + sizeof(arr28) / sizeof(arr28[0]) );
vector<double> vec29(arr29, arr29 + sizeof(arr29) / sizeof(arr29[0]) );
vector<double> vec30(arr30, arr30 + sizeof(arr30) / sizeof(arr30[0]) );
vector<double> vec40(arr40, arr40 + sizeof(arr40) / sizeof(arr40[0]) );
vector<double> vec50(arr50, arr50 + sizeof(arr50) / sizeof(arr50[0]) );
vector<double> vec60(arr60, arr60 + sizeof(arr60) / sizeof(arr60[0]) );
vector<double> vec80(arr80, arr80 + sizeof(arr80) / sizeof(arr80[0]) );
vector<double> vec100(arr100, arr100 + sizeof(arr100) / sizeof(arr100[0]) );
vector<double> vec120(arr120, arr120 + sizeof(arr120) / sizeof(arr120[0]) );
vector<double> vecInfty(arrInfty, arrInfty + sizeof(arrInfty) / sizeof(arrInfty[0]) );


template <typename DescriptorOfTopology>
class tStudentTest
{
public:
	tStudentTest(std::vector< DescriptorOfTopology >& firstPopulation , std::vector< DescriptorOfTopology >& secondPopulation ,double (*characteristic)(const DescriptorOfTopology&) , bool canVariancesOfPopulationsBeAssumedEqual = true );

    tStudentTest(std::vector< DescriptorOfTopology >& firstPopulation , double averageOfSecondPopulation,double (*characteristic)(const DescriptorOfTopology&));

    double test();
    void verifyNullHypotheris(double criticalT);

    void printStatistics();
private:
    void initializetTestDistribution();

    void computeBasicStatistics();

	std::vector< DescriptorOfTopology > firstPopulation;
	std::vector< DescriptorOfTopology > secondPopulation;
	double averageOfPopulation;
	double (*characteristic)(const DescriptorOfTopology&);
	bool canVariancesOfPopulationsBeAssumedEqual;

	//t-test tables:
	vector< vector<double> > tTestDistribution;
	vector<size_t> intervalsForDOF;
	vector<double> oneSidedTestPValues;
	vector<double> twoSidedTestPValues;

	//basic statistics:
	double sumOfValuesOfCharacteristicInFirstPopulation;
	double sumOfValuesOfCharacteristicInSecondPopulation;
	double averageFirstPopulation;
	double averageSecondPopulation;
	double varianceFirstPopulation;
	double varianceSecondPopulation;
	size_t numberOfDegreesOfFreedom;
};

//this function prints basic statistics. Used only in debug mode.
template <typename DescriptorOfTopology>
void tStudentTest<DescriptorOfTopology>::printStatistics()
{
    std::cout << "Values of characteristic function on the first population : " << std::endl;
    for ( size_t i = 0 ; i != this->firstPopulation.size() ; ++i )
    {
        std::cout << characteristic( this->firstPopulation[i] ) << std::endl;
    }
    std::cout << std::endl << std::endl << "Values of characteristic function on the second population : " << std::endl;
    for ( size_t i = 0 ; i != this->secondPopulation.size() ; ++i )
    {
        std::cout << characteristic( this->secondPopulation[i] ) << std::endl;
    }

    std::cout << "sumOfValuesOfCharacteristicInFirstPopulation : " << sumOfValuesOfCharacteristicInFirstPopulation << std::endl;
    std::cout << "sumOfValuesOfCharacteristicInSecondPopulation : " << sumOfValuesOfCharacteristicInSecondPopulation << std::endl;
    std::cout << "averageFirstPopulation : " << averageFirstPopulation << std::endl;
    std::cout << "averageSecondPopulation : " << averageSecondPopulation << std::endl;
    std::cout << "varianceFirstPopulation : " << varianceFirstPopulation << std::endl;
    std::cout << "varianceSecondPopulation : " << varianceSecondPopulation << std::endl;
    std::cout << "numberOfDegreesOfFreedom : " << numberOfDegreesOfFreedom << std::endl;
}//printStatistics

template <typename DescriptorOfTopology>
void tStudentTest<DescriptorOfTopology>::computeBasicStatistics()
{
    bool dbg = false;
    //summing values of characteristic for populations:
    this->sumOfValuesOfCharacteristicInFirstPopulation = 0;
    for ( size_t i = 0 ; i != this->firstPopulation.size() ; ++i )
    {
        if ( dbg )
        {
            cerr << "this->characteristic( this->firstPopulation[i] ) : " << this->characteristic( this->firstPopulation[i] ) << endl;
            getchar();
        }
        this->sumOfValuesOfCharacteristicInFirstPopulation += this->characteristic( this->firstPopulation[i] );
    }

    this->sumOfValuesOfCharacteristicInSecondPopulation = 0;
    for ( size_t i = 0 ; i != this->secondPopulation.size() ; ++i )
    {
        this->sumOfValuesOfCharacteristicInSecondPopulation += this->characteristic( this->secondPopulation[i] );
    }

    //computing averages of populations:
    if ( this->firstPopulation.size() )
    {
        this->averageFirstPopulation = this->sumOfValuesOfCharacteristicInFirstPopulation/this->firstPopulation.size();
    }
    if ( this->secondPopulation.size() )
    {
        this->averageSecondPopulation = this->sumOfValuesOfCharacteristicInSecondPopulation/this->secondPopulation.size();
    }

    //computing variance for populations:
    this->varianceFirstPopulation = 0;
    if ( this->firstPopulation.size() )
    {
        for ( size_t i = 0 ; i != this->firstPopulation.size() ; ++i )
        {
            this->varianceFirstPopulation += pow(this->characteristic( this->firstPopulation[i] ) - this->averageFirstPopulation,2);
        }
        this->varianceFirstPopulation /= (this->firstPopulation.size()-1);
    }
    this->varianceSecondPopulation = 0;
    if ( this->secondPopulation.size() )
    {
        for ( size_t i = 0 ; i != this->secondPopulation.size() ; ++i )
        {
            this->varianceSecondPopulation += pow(this->characteristic( this->secondPopulation[i] ) - this->averageSecondPopulation,2);
        }
        this->varianceSecondPopulation /= (this->secondPopulation.size()-1);
    }

}//computeBasicStatistics

template <typename DescriptorOfTopology>
void tStudentTest<DescriptorOfTopology>::initializetTestDistribution()
{
    this->tTestDistribution.push_back(vec1);
    this->tTestDistribution.push_back(vec2);
    this->tTestDistribution.push_back(vec3);
    this->tTestDistribution.push_back(vec4);
    this->tTestDistribution.push_back(vec5);
    this->tTestDistribution.push_back(vec6);
    this->tTestDistribution.push_back(vec7);
    this->tTestDistribution.push_back(vec8);
    this->tTestDistribution.push_back(vec9);
    this->tTestDistribution.push_back(vec10);
    this->tTestDistribution.push_back(vec11);
    this->tTestDistribution.push_back(vec12);
    this->tTestDistribution.push_back(vec13);
    this->tTestDistribution.push_back(vec14);
    this->tTestDistribution.push_back(vec15);
    this->tTestDistribution.push_back(vec16);
    this->tTestDistribution.push_back(vec17);
    this->tTestDistribution.push_back(vec18);
    this->tTestDistribution.push_back(vec19);
    this->tTestDistribution.push_back(vec20);
    this->tTestDistribution.push_back(vec21);
    this->tTestDistribution.push_back(vec22);
    this->tTestDistribution.push_back(vec23);
    this->tTestDistribution.push_back(vec24);
    this->tTestDistribution.push_back(vec25);
    this->tTestDistribution.push_back(vec26);
    this->tTestDistribution.push_back(vec27);
    this->tTestDistribution.push_back(vec28);
    this->tTestDistribution.push_back(vec29);
    this->tTestDistribution.push_back(vec30);
    this->tTestDistribution.push_back(vec40);
    this->tTestDistribution.push_back(vec50);
    this->tTestDistribution.push_back(vec60);
    this->tTestDistribution.push_back(vec80);
    this->tTestDistribution.push_back(vec100);
    this->tTestDistribution.push_back(vec120);
    this->tTestDistribution.push_back(vecInfty);

    this->intervalsForDOF.push_back(1);
    this->intervalsForDOF.push_back(2);
    this->intervalsForDOF.push_back(3);
    this->intervalsForDOF.push_back(4);
    this->intervalsForDOF.push_back(5);
    this->intervalsForDOF.push_back(6);
    this->intervalsForDOF.push_back(7);
    this->intervalsForDOF.push_back(8);
    this->intervalsForDOF.push_back(9);
    this->intervalsForDOF.push_back(10);
    this->intervalsForDOF.push_back(11);
    this->intervalsForDOF.push_back(12);
    this->intervalsForDOF.push_back(13);
    this->intervalsForDOF.push_back(14);
    this->intervalsForDOF.push_back(15);
    this->intervalsForDOF.push_back(16);
    this->intervalsForDOF.push_back(17);
    this->intervalsForDOF.push_back(18);
    this->intervalsForDOF.push_back(19);
    this->intervalsForDOF.push_back(20);
    this->intervalsForDOF.push_back(21);
    this->intervalsForDOF.push_back(22);
    this->intervalsForDOF.push_back(23);
    this->intervalsForDOF.push_back(24);
    this->intervalsForDOF.push_back(25);
    this->intervalsForDOF.push_back(26);
    this->intervalsForDOF.push_back(27);
    this->intervalsForDOF.push_back(28);
    this->intervalsForDOF.push_back(29);
    this->intervalsForDOF.push_back(30);
    this->intervalsForDOF.push_back(40);
    this->intervalsForDOF.push_back(50);
    this->intervalsForDOF.push_back(60);
    this->intervalsForDOF.push_back(80);
    this->intervalsForDOF.push_back(100);
    this->intervalsForDOF.push_back(120);
    this->intervalsForDOF.push_back(INT_MAX);

    //75%	80%	85%	90%	95%	97.5%	99%	99.5%	99.75%	99.9%	99.95%
    this->oneSidedTestPValues.push_back( 0.75 );
    this->oneSidedTestPValues.push_back( 0.80 );
    this->oneSidedTestPValues.push_back( 0.85 );
    this->oneSidedTestPValues.push_back( 0.90 );
    this->oneSidedTestPValues.push_back( 0.95 );
    this->oneSidedTestPValues.push_back( 0.975 );
    this->oneSidedTestPValues.push_back( 0.99 );
    this->oneSidedTestPValues.push_back( 0.995 );
    this->oneSidedTestPValues.push_back( 0.9975 );
    this->oneSidedTestPValues.push_back( 0.999 );
    this->oneSidedTestPValues.push_back( 0.9995 );
    this->oneSidedTestPValues.push_back( INT_MAX );

    //50%	60%	70%	80%	90%	95%	98%	99%	99.5%	99.8%	99.9%
    this->twoSidedTestPValues.push_back( 0.5 );
    this->twoSidedTestPValues.push_back( 0.6 );
    this->twoSidedTestPValues.push_back( 0.7 );
    this->twoSidedTestPValues.push_back( 0.8 );
    this->twoSidedTestPValues.push_back( 0.9 );
    this->twoSidedTestPValues.push_back( 0.95 );
    this->twoSidedTestPValues.push_back( 0.98 );
    this->twoSidedTestPValues.push_back( 0.99 );
    this->twoSidedTestPValues.push_back( 0.995 );
    this->twoSidedTestPValues.push_back( 0.998 );
    this->twoSidedTestPValues.push_back( 0.999 );
    this->twoSidedTestPValues.push_back( INT_MAX );
}

template <typename DescriptorOfTopology>
tStudentTest<DescriptorOfTopology>::tStudentTest(std::vector< DescriptorOfTopology >& firstPopulation ,
             std::vector< DescriptorOfTopology >& secondPopulation,
             double (*characteristic)(const DescriptorOfTopology&) , bool canVariancesOfPopulationsBeAssumedEqual )
{
    this->initializetTestDistribution();
    this->firstPopulation.insert( this->firstPopulation.begin() , firstPopulation.begin() , firstPopulation.end() );
    this->secondPopulation.insert( this->secondPopulation.begin() , secondPopulation.begin() , secondPopulation.end() );
    this->averageOfPopulation = 0;
    this->characteristic = characteristic;
    this->canVariancesOfPopulationsBeAssumedEqual = canVariancesOfPopulationsBeAssumedEqual;

    this->computeBasicStatistics();
}

template <typename DescriptorOfTopology>
tStudentTest<DescriptorOfTopology>::tStudentTest(std::vector< DescriptorOfTopology >& firstPopulation , double averageOfSecondPopulation,double (*characteristic)(const DescriptorOfTopology&) )
{
    this->initializetTestDistribution();
    this->firstPopulation.insert( this->firstPopulation.begin() , firstPopulation.begin() , firstPopulation.end() );
    this->averageOfPopulation = averageOfSecondPopulation;
    this->characteristic = characteristic;
    this->computeBasicStatistics();
}


template <typename DescriptorOfTopology>
double tStudentTest<DescriptorOfTopology>::test()
{
    bool dbg = false;
    if ( this->firstPopulation.size() == 0 )
    {
        cerr << "Class not initialized, the program will now terminate. \n";
        throw("Class not initialized, the program will now terminate. \n");
    }
    if ( (this->firstPopulation.size() != 0) && (this->secondPopulation.size() != 0) )
    {
        //we are comparing two populations.
        if ( this->canVariancesOfPopulationsBeAssumedEqual )
        {
            if ( this->firstPopulation.size() == this->secondPopulation.size() )
            {
                //equal sizes, equal variances.
                if ( dbg ){std::cout << "equal sizes, equal variances. \n";}


                //QUESTION -- SHOULD WE HAVE FABS HERE?

                double t = fabs(this->averageFirstPopulation - this->averageSecondPopulation)
                            /
                            //(double)(
                            //    sqrt(  0.5*(pow(this->varianceFirstPopulation,2)+pow(this->varianceFirstPopulation,2) ) )
                            //    *
                            //    sqrt(2.0/(double)this->firstPopulation.size()));
                                sqrt((double)(
                                         this->varianceFirstPopulation/(double)this->firstPopulation.size() + this->varianceSecondPopulation/(double)this->secondPopulation.size()
                                        ));
                this->numberOfDegreesOfFreedom = 2*this->firstPopulation.size()-2;
                std::cout << "Number of degrees of freedom is : " << this->numberOfDegreesOfFreedom << std::endl;
                std::cout << "The value of a statistic t is : " << t << std::endl;
                std::cout << "For the p-values, please consult the appropriate tables." << std::endl;
                return t;
            }
            else
            {
                //not equal sizes, equal variances.
                if ( dbg ){std::cout << "not equal sizes, equal variances. \n";}


                //QUESTION  -- SHOULD WE HAVE fabs HERE?


                double t = fabs(this->averageFirstPopulation - this->averageSecondPopulation)
                            /
                            (double)(
                                sqrt(
                                        ( (this->firstPopulation.size()-1)*this->varianceFirstPopulation + (this->secondPopulation.size()-1)*this->varianceSecondPopulation )
                                        /
                                        (double)( this->firstPopulation.size() + this->secondPopulation.size() -2 )
                                    )
                                *
                                sqrt( 1.0/(double)this->firstPopulation.size() + 1.0/(double)this->secondPopulation.size() )
                            );
                this->numberOfDegreesOfFreedom = this->firstPopulation.size() + this->secondPopulation.size() -2;
                std::cout << "Number of degrees of freedom is : " << this->numberOfDegreesOfFreedom << std::endl;
                std::cout << "The value of a statistic t is : " << t << std::endl;
                std::cout << "For the p-values, please consult the appropriate tables." << std::endl;
                return t;
            }
        }
        else
        {
            //not equal variances.
            if ( dbg ){std::cout << "not equal variances. \n";}


            // QUESTION -- SHOULD WE HAVE FABSE HERE???


            double t = fabs(this->averageFirstPopulation - this->averageSecondPopulation)
                        /
                        ((double)(
                            sqrt( this->varianceFirstPopulation/(double)this->firstPopulation.size() + this->varianceSecondPopulation/(double)this->secondPopulation.size() )
                        ));

            double firstVarianceDividedPopulSize = this->varianceFirstPopulation/(double)this->firstPopulation.size();
            double secondVarianceDividedPopulSize = this->varianceSecondPopulation/(double)this->secondPopulation.size();

            this->numberOfDegreesOfFreedom =
                                            (size_t)(
                                                        pow(firstVarianceDividedPopulSize+secondVarianceDividedPopulSize,2)
                                                        /
                                                        (
                                                            pow(firstVarianceDividedPopulSize,2)*(this->firstPopulation.size()-1)
                                                            +
                                                            pow(secondVarianceDividedPopulSize,2)*(this->secondPopulation.size()-1)
                                                        )
                                                    );
            std::cout << "Number of degrees of freedom is : " << this->numberOfDegreesOfFreedom << std::endl;
            std::cout << "The value of a statistic t is : " << t << std::endl;
            std::cout << "For the p-values, please consult the appropriate tables." << std::endl;
            return t;
        }
    }
    else
    {
        //we are checking empirical mean.
        double t = (averageFirstPopulation-this->averageOfPopulation)/(double)(this->varianceFirstPopulation/sqrt(this->firstPopulation.size()));
        this->numberOfDegreesOfFreedom = this->firstPopulation.size()-1;
        std::cout << "Number of degrees of freedom is : " << this->numberOfDegreesOfFreedom << std::endl;
        std::cout << "The value of a statistic t is : " << t << std::endl;
        std::cout << "For the p-values, please consult the appropriate tables." << std::endl;
        return t;
    }
}//test


template <typename DescriptorOfTopology>
void tStudentTest<DescriptorOfTopology>::verifyNullHypotheris(double criticalT)
{
    //criticalT = 1 - criticalT;

    bool dbg = true;

    double t = this->test();

    size_t rowOfTable = 0;
    while ( this->intervalsForDOF[rowOfTable] < this->numberOfDegreesOfFreedom ){++rowOfTable;}

    size_t collumnOfTable = 0;
    while ( criticalT > this->oneSidedTestPValues[collumnOfTable] ){++collumnOfTable;}
    //if ( collumnOfTable )
    //{
    //    if ( fabs(criticalT-this->oneSidedTestPValues[collumnOfTable-1]) < fabs(criticalT-this->oneSidedTestPValues[collumnOfTable]) )
    //    {
    //        collumnOfTable--;
    //    }
    //}
    size_t collumnOfTableTwoSidedTest = 0;
    while ( criticalT > this->twoSidedTestPValues[collumnOfTableTwoSidedTest] ){++collumnOfTableTwoSidedTest;}

    //if ( collumnOfTableTwoSidedTest )
    //{
    //    if ( fabs(criticalT-this->twoSidedTestPValues[collumnOfTableTwoSidedTest]) < fabs(criticalT-this->twoSidedTestPValues[collumnOfTableTwoSidedTest]) )
    //    {
    //        collumnOfTableTwoSidedTest--;
    //    }
    //}


    if ( dbg )
    {
        cerr << "collumnOfTable : " << collumnOfTable << endl;
        cerr << "collumnOfTableTwoSidedTest : " << collumnOfTableTwoSidedTest << endl;
        cerr << "rowOfTable : " << rowOfTable << endl;
        cerr << "collumnOfTable : " << collumnOfTable << endl;
        cerr << "collumnOfTableTwoSidedTest : " << collumnOfTableTwoSidedTest << endl;
        cerr << "this->oneSidedTestPValues[collumnOfTable] : " << this->oneSidedTestPValues[collumnOfTable] << endl;
        cerr << "this->twoSidedTestPValues[collumnOfTableTwoSidedTest]  : " << this->twoSidedTestPValues[collumnOfTableTwoSidedTest]  <<endl;
    }


    std::cout << "The value of the variable t is : " << t << ". The number of degrees of freedom of this system is : " << this->numberOfDegreesOfFreedom << "." << std::endl;
    //TODO -- do we have to take 1-critical??
    std::cout << "For the confidence value : " << criticalT << " the critical value in **one tailed** t-Student is " <<  this->tTestDistribution[rowOfTable][collumnOfTable] << std::endl;


    std::cout << "For the confidence value : " << criticalT << " the critical value in **two tailed** t-Student is " << this->tTestDistribution[rowOfTable][collumnOfTableTwoSidedTest] << std::endl;


    if ( t < this->tTestDistribution[rowOfTable][collumnOfTable] )
    {
        std::cout << "Therefore we cannot reject the null hypothesis based on one sided test and we may assume that the averages in the two populations are equal with a probability at least " << criticalT << std::endl;
    }
    else
    {
        std::cout << "Therefore we have to reject the null hypothesis based on one sided test and therefore we cannot assume that the averages in the two populations are equal with a probability at least " << criticalT << std::endl;
    }

    if ( t < this->tTestDistribution[rowOfTable][collumnOfTableTwoSidedTest] )
    {
        std::cout << "Therefore we cannot reject the null hypothesis based on two sided test and we may assume that the averages in the two populations are equal with a probability at least " << criticalT << std::endl;
    }
    else
    {
        std::cout << "Therefore we have to reject the null hypothesis based on two sided test and therefore we cannot assume that the averages in the two populations are equal with a probability at least " << criticalT << std::endl;
    }

}
