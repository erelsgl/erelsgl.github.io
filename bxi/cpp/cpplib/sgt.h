/*
 * File cpplib/sgt.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/sgt.h
 */

/* sgt.h -- Simple Good-Turing counter */

#ifndef __SGT_H
#define __SGT_H


/*
	This unit provides Simple Good-Turing estimators for the
	'frequency of frequencies'.
*/




#include <cpplib/vector1.hxx>
#include <cpplib/map.hxx>
#include <cpplib/map1.hxx>
#include <cpplib/countc.hxx>
#include <cpplib/stattemp.h>
#include <math.h>
#include <fstream.h>

class SimpleGoodTuringEstimator;
typedef const SimpleGoodTuringEstimator& SimpleGoodTuringEstimatorCR;

class SimpleGoodTuringEstimator {
	unsigned my_number_of_different_frequencies;

	// temporary variables for SGT calculation
	Vector1<int> r, n;
	Vector1<double> Z, log_r, log_Z, rStar, p;
	int rows, bigN;
	double PZero, bigNprime, slope, intercept;

	// temporary variable for number_of_unseen_species estimation.
	// See Efron & Thisted (1976).
	Vector1<double> n_for_Euler_transformation;
	double estimated_number_of_unseen_species;

	void findBestFit(void);
	double smoothed(int i);
	int row(int i);
	void analyseInput(void);

	void estimateNumberOfUnseenSpecies(void);

	static double sq(double x) { return(x * x); }


public:

	static ofstream logstream;

	/* construct and calculate sums */
	SimpleGoodTuringEstimator(CounterVector1Sorted<int>& the_frequency_of_frequencies_counter);

	/* extract probabilities into the given structure. */
	// the_number_of_unseen_species is used to calculate the probability of each unseen species. 
	void get_probabilities(Map1<int,double>& the_probabilities_of_frequencies, int the_number_of_unseen_species) const;

	/* extract probabilities into the given structure. */
	// the_number_of_unseen_species is used to calculate the fixed frequency of each unseen species. 
	void get_fixed_frequencies(Map1<int,double>& the_fixed_frequencies_of_frequencies, int the_number_of_unseen_species) const;

	/* extract probabilities into the given structure. */
	// the number of unseen species is calculated using formula (2.3) of Efron & Thisted (1976).
	void get_fixed_frequencies(Map1<int,double>& the_fixed_frequencies_of_frequencies) const;
};



#endif
