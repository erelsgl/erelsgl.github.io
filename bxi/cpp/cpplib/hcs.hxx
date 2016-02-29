/*
 * File cpplib/hcs.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/hcs.hxx
 */

/* hcs.hxx -- Hill Climbing Searcher */

#ifndef __HCS_HXX
#define __HCS_HXX

#include <cpplib/vector1.hxx>
#include <cpplib/mathtemp.h>



template <class Parameter> class HillClimbingSearcher {  

protected:
	Vector1<Parameter> par;          // the parameters

	virtual void init_the_parameters() { zero(par); }

	virtual void finish() {}

	virtual double score() const = 0; 
	// This function should evalutate the function at the current parameters.
	// The searcher will find the scores of the parameters for which 'score' gives
	//		The HIGHEST score!
	
	virtual void log (Parameter thestep, double thescore) {}


public:

	HillClimbingSearcher(Size num_of_pars): par(num_of_pars) {}
	~HillClimbingSearcher() { free(par); }

	void run_algorithm_a (Parameter the_initial_stepsize, Parameter the_final_stepsize) {
		init_the_parameters();
		double score_with_current_pars;
		Vector1<double> scores_when_increasing_pars (par.count());
		Vector1<double> scores_when_decreasing_pars (par.count());
		Parameter stepsize;
		for (Index i=1;;) {
			//stepsize = the_initial_stepsize / sqrt(i);
			stepsize=the_initial_stepsize * pow(0.8,i);
			if (stepsize<=the_final_stepsize) break;
			score_with_current_pars = score();
			log (stepsize,score_with_current_pars);
			{LOOPVECTOR(;,scores_when_increasing_pars,p) {
				par[p] += stepsize;
				scores_when_increasing_pars[p] = score();
				par[p] -= stepsize;
			}}
			{LOOPVECTOR(;,scores_when_decreasing_pars,p) {
				par[p] -= stepsize;
				scores_when_decreasing_pars[p] = score();
				par[p] += stepsize;
			}}
			Index pi = maxindex (scores_when_increasing_pars);  double score_i = scores_when_increasing_pars[pi];
			Index pd = maxindex (scores_when_decreasing_pars);  double score_d = scores_when_decreasing_pars[pd];
			if (score_with_current_pars >= min(score_i,score_d))       // can't improve neither by increasing nor by decreasing
				++i;
			else if (score_d >= score_i)                               // can improve by decreasing
				par[pd] -= stepsize;
			else 
				par[pi] += stepsize;                                  // can improve by increasing
		}
		finish();
	}	


	void run_algorithm_b (Parameter the_initial_stepsize, Parameter the_final_stepsize) {
		init_the_parameters();
		double score_with_current_pars;
		double score_when_increasing_par, score_when_decreasing_par;
		Parameter stepsize;
		short direction;
		
		for (bool I_am_still_improving=true; I_am_still_improving;) {
			I_am_still_improving=false;
			LOOPVECTOR(;,par,p) {          // find the best value for each parameter, in turns ("Iterative Weight Perturbation").
				for (stepsize=the_initial_stepsize; stepsize>the_final_stepsize; stepsize*=0.8) {
					score_with_current_pars = score();
					log (stepsize,score_with_current_pars);
					par[p] += stepsize;  score_when_increasing_par = score();  par[p] -= stepsize; 
					par[p] -= stepsize;  score_when_decreasing_par = score();  par[p] += stepsize; 
					if (score_with_current_pars >= min(score_when_increasing_par,score_when_decreasing_par))       // can't improve neither by increasing nor by decreasing
						continue;
					else {
						I_am_still_improving = true;
						direction=compare(score_when_increasing_par,score_when_decreasing_par);
						par[p] += direction*stepsize;
						for(;;) {
							score_with_current_pars = score();
							par[p] += direction*stepsize;
							score_when_increasing_par = score();
							if (score_with_current_pars >= score_when_increasing_par) break;  // can't improve in current direction
						}
					}
				}
			}
		}
		finish();
	}	

};

#endif

