/*
 * File cpplib/sgt.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/sgt.cpp
 */

/* sgt.cpp -- Simple Good-Turing counter */

// Some code in this unit was adapted from a file whose introductory comment is cited below:
/*
 *
 *
 *      Simple Good-Turing Frequency Estimator
 *
 *
 *      Geoffrey Sampson, with help from Miles Dennis
 *
 *      School of Cognitive and Computing Sciences
 *      University of Sussex, England
 *
 *      http://www.grs.u-net.com/
 *
 *
 *      First release:  27 June 1995
 *      Revised release:  24 July 2000
 *
 *
 *      Takes a set of (frequency, frequency-of-frequency) pairs, and 
 *      applies the "Simple Good-Turing" technique for estimating 
 *      the probabilities corresponding to the observed frequencies, 
 *      and P.0, the joint probability of all unobserved species.
 *      The Simple Good-Turing technique was devised by William A. Gale
 *      of AT&T Bell Labs, and described in Gale & Sampson, 
 *      "Good-Turing Frequency Estimation Without Tears" (JOURNAL
 *      OF QUANTITATIVE LINGUISTICS, vol. 2, pp. 217-37 -- reprinted in
 *      Geoffrey Sampson, EMPIRICAL LINGUISTICS, Continuum, 2001).
 *
 *      Anyone is welcome to take copies of this program and use it
 *      for any purpose, at his or her own risk.  If it is used in
 *      connexion with published work, acknowledgment of Sampson and
 *      the University of Sussex would be a welcome courtesy.
 *
 *      The program is written to take input from "stdin" and send output
 *      to "stdout"; redirection can be used to take input from and
 *      send output to permanent files.  The code is in ANSI standard C.
 *
 *      The input file should be a series of lines separated by newline
 *      characters, where all nonblank lines contain two positive integers
 *      (an observed frequency, followed by the frequency of that frequency)
 *      separated by whitespace.  (Blank lines are ignored.)
 *      The lines should be in ascending order of frequency.
 *
 *      No checks are made for linearity; the program simply assumes that the 
 *      requirements for using the SGT estimator are met.
 *
 *      The output is a series of lines each containing an integer followed  
 *      by a probability (a real number between zero and one), separated by a 
 *      tab.  In the first line, the integer is 0 and the real number is the 
 *      estimate for P.0.  In subsequent lines, the integers are the  
 *      successive observed frequencies, and the reals are the estimated  
 *      probabilities corresponding to those frequencies.
 *
 *      The revised release cures a bug to which Martin Jansche of Ohio
 *      State University kindly drew attention.  No warranty is given 
 *      as to absence of further bugs.
 *
 *
 */


#include <cpplib/sgt.h>



// returns C(n,k) = the number of ways to choose a group of k items from n different items.
// C(n,k) = n! / k! / (n-k)!
//        = n*(n-1)*(n-2)*...*(n-k+1)  /  (k)!
//        = n*(n-1)*(n-2)*...*(k+1)  /  (n-k)!
static double combinations (int n, int k) {
	if (k<0 || n<k)
		return 0;

	double result = 1;
	if (k > n-k) {
		int i;
		for (i=n; i>k; --i)
			result *= i;
		for (i=n-k; i>1; --i)
			result /= i;
	}
	else {  // k <= n-k
		int i;
		for (i=n; i>n-k; --i)
			result *= i;
		for (i=k; i>1; --i)
			result /= i;
	}
	return result;
}



SimpleGoodTuringEstimator::SimpleGoodTuringEstimator(CounterVector1Sorted<int>& the_frequency_of_frequencies_counter): 
	my_number_of_different_frequencies (the_frequency_of_frequencies_counter.count()-1),
	r(my_number_of_different_frequencies), 
	n(my_number_of_different_frequencies), 
	Z(my_number_of_different_frequencies), 
	log_r(my_number_of_different_frequencies), 
	log_Z(my_number_of_different_frequencies), 
	rStar(my_number_of_different_frequencies), 
	p(my_number_of_different_frequencies),
	n_for_Euler_transformation(my_number_of_different_frequencies) {

	CounterVector1Sorted<int>::Cursor cursor(the_frequency_of_frequencies_counter);
	++cursor;

	for (rows=0; cursor.isvalid(); ++cursor, ++rows) {
		r[rows] = cursor.data();
		n[rows] = cursor.count();
	}

	analyseInput();
	estimateNumberOfUnseenSpecies();
}



void SimpleGoodTuringEstimator::findBestFit(void) {
    double XYs, Xsquares, meanX, meanY;
    int i;

    XYs = Xsquares = meanX = meanY = 0.0;
    for (i = 0; i < rows; ++i) {
            meanX += log_r[i];
            meanY += log_Z[i];
    }
    meanX /= rows;
    meanY /= rows;
    for (i = 0; i < rows; ++i) {
            XYs += (log_r[i] - meanX) * (log_Z[i] - meanY);
            Xsquares += sq(log_r[i] - meanX);
    }
    slope = XYs / Xsquares;
    intercept = meanY - slope * meanX;
}


double SimpleGoodTuringEstimator::smoothed(int i) {
    return(exp(intercept + slope * log(i)));
}
    
int SimpleGoodTuringEstimator::row(int i) {
    int j = 0;
    
    while (j < rows && r[j] < i)
            ++j;
    return((j < rows && r[j] == i) ? j : -1);
}


void SimpleGoodTuringEstimator::analyseInput(void) {
    int i, j, next_n;
    double k, x, y;
    bool indiffValsSeen = false;

    bigN = 0;
    for (j = 0; j < rows; ++j)
            bigN += r[j] * n[j];
    PZero = n[row(1)] / (double) bigN;
    for (j = 0; j < rows; ++j) {
            i = (j == 0 ? 0 : r[j - 1]);
            if (j == rows - 1)
                    k = (double) (2 * r[j] - i);
            else
                    k = (double) r[j + 1];
            Z[j] = 2 * n[j] / (k - i);
            log_r[j] = log(r[j]);
            log_Z[j] = log(Z[j]);
    }
    findBestFit();
    for (j = 0; j < rows; ++j) {
            y = (r[j] + 1) * smoothed(r[j] + 1) / smoothed(r[j]);
            if (row(r[j] + 1) < 0)
                    indiffValsSeen = true;
            if (! indiffValsSeen) {
                    x = (r[j] + 1) * (next_n = n[row(r[j] + 1)]) / 
                                    (double) n[j];
                    if (fabs(x - y) <= 1.96 * sqrt(sq(r[j] + 1.0) *
                                    next_n / (sq((double) n[j])) * 
                                    (1 + next_n / (double) n[j])))
                            indiffValsSeen = true;
                    else
                            rStar[j] = x;
            }
            if (indiffValsSeen)
                    rStar[j] = y;
    }
    bigNprime = 0.0;
    for (j = 0; j < rows; ++j)
            bigNprime += n[j] * rStar[j];
    for (j = 0; j < rows; ++j)
            p[j] = (1 - PZero) * rStar[j] / bigNprime;


	if (logstream.is_open()) {
		logstream << "0* = " << "\t" << PZero*bigN << " / number_of_unseen_species" << endl;
		for (j = 0; j < rows; ++j)
			logstream << r[j] << "* = \t" << (p[j]*bigN) << endl;
	}
}

void SimpleGoodTuringEstimator::estimateNumberOfUnseenSpecies(void) {

	// calculate the Euler transorfmation coefficients.
	// Uses formula (4.1) of Efron & Thisted (1976), with etha[x] estimated by n[x].
	int y;
	for (y=1; y<(int)n_for_Euler_transformation.size(); ++y) {
		n_for_Euler_transformation[y] = 0;
		for (int j=0; j<rows && r[j]<=y; ++j) {
			double x = r[j];
			n_for_Euler_transformation[y] += 
				combinations(y-1,x-1) * pow(-1,x+1) / pow(2,y) * n[j];
		}
		if (n_for_Euler_transformation[y]<0) {
			n_for_Euler_transformation.resize(y);
			break;
		}
	}

	// estimate the number of unseen species.
	// Uses formula (4.3) of Efron & Thisted (1976), with t=infinity (u=2).
	double u = 2;
	estimated_number_of_unseen_species = 0;
	for (y=1; y<(int)n_for_Euler_transformation.count(); ++y)
		estimated_number_of_unseen_species +=  n_for_Euler_transformation[y] * pow(u, y);
	logstream << "0* = " << "\t" << PZero*bigN/estimated_number_of_unseen_species << endl;
}


void SimpleGoodTuringEstimator::get_probabilities(Map1<int,double>& the_probabilities_of_frequencies, int the_number_of_unseen_species) const {
	the_probabilities_of_frequencies.insert(0, PZero/the_number_of_unseen_species);
	for (int j=0; j<rows; ++j)
		the_probabilities_of_frequencies.insert(r[j], p[j]);
}

void SimpleGoodTuringEstimator::get_fixed_frequencies(Map1<int,double>& the_fixed_frequencies_of_frequencies, int the_number_of_unseen_species) const {
	the_fixed_frequencies_of_frequencies.insert(0, PZero/the_number_of_unseen_species*bigN);
	for (int j=0; j<rows; ++j)
		the_fixed_frequencies_of_frequencies.insert(r[j], p[j]*bigN);
}

void SimpleGoodTuringEstimator::get_fixed_frequencies(Map1<int,double>& the_fixed_frequencies_of_frequencies) const {
	get_fixed_frequencies(the_fixed_frequencies_of_frequencies, (int)estimated_number_of_unseen_species);
}

ofstream SimpleGoodTuringEstimator::logstream;

