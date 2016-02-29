/* queue.hxx -- a one-way, variable length queue  */

/*

	This unit defines the template-class Queue2 (see below).

*/





#ifndef __QUEUE2_HXX

#define __QUEUE2_HXX



#include <cpplib/klli1.h>

#include <cpplib/io2.h>





#define TEMPLATE template <class Data>

#define QUEUE Queue2<Data>

#define QUEUECR const QUEUE&





/**************************************************************************

**************************************************************************/







TEMPLATE struct Queue2 {

protected:



	Pointer1<Data> myinfo;

	Index head, tail;

	// elements are inserted at 'tail' and removed from 'head' (tail >= head always).



	void remove_dequeued_items () {

		myinfo.copy (0, myinfo.p+head, tail-head);

		tail -= head;

		head = 0;

	}



public:



	Queue2 (Size thesize=0) { DOx(myinfo.alloc(thesize)); head=tail=0; }

	~Queue2 () { free(); }



	bool isempty() const { return head>=tail; }

	Size count() const { return tail-head; }



	DataCR at(Index i) const { return myinfo[i+head]; } 

	Data& at(Index i) { return myinfo[i+head]; } 



	DataCR operator[](Index i) const { return myinfo[i+head]; } 

	Data& operator[](Index i) { return myinfo[i+head]; } 



	DataCR athead () { return at(0); }



	status removeat (Index i) {

		if (i>=count())  return ERANGE;

		::free (at(i));

		myinfo.copy (head+i, myinfo.p+head+i+1, tail-head-i-1);

		--tail;

		return OK;

	}	



	status dequeue () {           // remove from head

		if (isempty())  return EEMPTY;

		::free (at(0));

		++head;

		return OK;

	}



	status enqueue (DataCR thedata) {           // append at tail

		if (tail >= myinfo.size) {

			if (head>0)

				remove_dequeued_items ();

			else {

				Data* old_p;  DOr(myinfo.mrealloc(tail+1,-1,old_p));   // -1 means -- exponential growth.

				if (old_p!=NULL)  myinfo.copy(0, old_p,tail); 

			}

		}

		myinfo[tail] = thedata;

		++tail;

		return OK;

	}



	void truncate() { head=tail=0; }



	void free () {
		LOOPVECTOR(;,(*this),i)
			::free (at(i));
		myinfo.free();
		head=tail=0;  
    }
};

TEMPLATE void free (QUEUE& q) {
	q.free();
}

TEMPLATE void write (ostream& out, QUEUECR thequeue, Format format="") {
	if (format[0]=='P')   {         // Parenthesized format: [a1 a2 ... an]
		format = format.next();
		out << '[';
		write_vector (out,thequeue, str(format[0]),format.next() );
		out << ']';
	}
	else {                          // Simple format: a1 a2 ... an
		if (format[0]=='*' || format[0]=='L')  format=format.next();
		write_vector (out,thequeue, str(format[0]),format.next() );
	}
}







TEMPLATE bool contains (QUEUECR thequeue, DataCR theitem) {

	LOOPVECTOR(;,thequeue,i) 

		if ( identical(thequeue.at(i),theitem) )  return true;

	return false;

}









#undef TEMPLATE

#endif

