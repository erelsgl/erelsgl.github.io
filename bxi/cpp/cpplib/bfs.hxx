/*
 * File cpplib/bfs.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/bfs.hxx
 */

/* bfs.hxx -- a general implementation of the BFS (Breadth First Search) algorithm,
			and of the Best First Search algorithm.
*/

#ifndef __BFS_HXX
#define __BFS_HXX

#include <cpplib/queue2.hxx>


/****************             Searcher              ********************/
/****************             Searcher              ********************/
/****************             Searcher              ********************/

template <class Data> class Searcher {

protected:
	Queue2<Data> open_nodes;
	status enqueue (Data& thedata) {
		if (!contains(open_nodes,thedata))
			return open_nodes.enqueue(thedata);
		else {
			free (thedata);
			return OK;
		}
	}
	void dequeue () { open_nodes.dequeue(); }
	DataCR athead() const { return open_nodes.at(0); }
	bool queue_isempty() const { return open_nodes.count()==0; }


	virtual status enqueue_first_nodes() = 0;
	// This method should insert the first node (or nodes) into the queue, using 'enqueue'.

	virtual status enqueue_next_nodes(DataCR current_node) = 0;
	// This method should insert all the neighbours of 'current_node' into the queue, using 'enqueue'.
	// It should also check if the 'goal' is reached. If so -- it sould return EOF. 

	virtual status finish () { return OK; }
	// This method should perform any cleanup necessary for the end of the routine.

public:

	Searcher (Size initial_queue_size=0): open_nodes(initial_queue_size) { }
	virtual status run () = 0;
	// This method is called by the user of the class, in order to begin the search.
};




/****************             Breadth First Searcher              ********************/
/****************             Breadth First Searcher              ********************/
/****************             Breadth First Searcher              ********************/


template <class Data> class BreadthFirstSearcher: public Searcher<Data> {

protected:
	Index head;
	virtual void dequeue () { head++; }        // The first element is not really deleted from open_nodes.
	DataCR athead() const { return open_nodes.at(head); }
	bool queue_isempty() const { return open_nodes.count()==head; }

public:

	BreadthFirstSearcher(Size initial_queue_size=0): Searcher<Data>(initial_queue_size) { head=0; }

	status run () {
		DOr (enqueue_first_nodes());
		for (;;) {
			if (queue_isempty()) break;
			DataCR curdata = athead();
			DOEOFr(enqueue_next_nodes(curdata));
			dequeue();
		}
		free (open_nodes);
		DOr (finish());
		return OK;
	}
};




/****************             Best First Searcher              ********************/
/****************             Best First Searcher              ********************/
/****************             Best First Searcher              ********************/

template <class Data> class BestFirstSearcher: public Searcher<Data> {

protected:
	virtual double score (DataCR thedata) = 0;
	// This function is used to select the next node that will be developed (It will be the node with the highest score).
	
	Index index_of_best_node () {
		Index cur_best_index = 0;
		double cur_best_score = //open_nodes[0].my_realsikui;
			score(open_nodes[0]);
		LOOPVECTOR (;,open_nodes,n) {
			double cur_score = //open_nodes[n].my_realsikui;
				score(open_nodes[n]);
			if (cur_score > cur_best_score) {
				cur_best_score = cur_score;
				cur_best_index = n;
			}
		}
		return cur_best_index;
	}

public:

	BestFirstSearcher(Size initial_queue_size=0): Searcher<Data>(initial_queue_size) {}

	status run () {
		DOr (enqueue_first_nodes());
		for (;;) {
			if (queue_isempty()) break;
			Index IOBN = index_of_best_node();
			DataCR curdata = open_nodes.at(IOBN);
			DOEOFr(enqueue_next_nodes(curdata));
			open_nodes.removeat (IOBN);
		}
		DOr(finish());
		free (open_nodes);
		return OK;
	}
};





#endif
