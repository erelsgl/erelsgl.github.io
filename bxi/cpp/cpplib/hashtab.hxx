/*
 * File cpplib/hashtab.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/hashtab.hxx
 */

/* hashtab.hxx -- a simple Hashtable */

#ifndef __HASHTAB_HXX
#define __HASHTAB_HXX

#include <cpplib/vector1.hxx>
#include <cpplib/array2.hxx>

#define TEMPLATE template <class Data>
#define HASHTAB HashTable<Data>
#define HASHTABCR const HASHTAB&
#define CURSOR HashCursor<Data>
#define CURSORCR const CURSOR&

/*********************************************************************************
 *             HashTable                                                         
 * The hash-table is implemented as a vector1 of buckets, each bucket is a vector1 
 *	of elements.																 
 * To access the elements in a Hashtable, you can use a HashCursor.
 * In order to use HashTable<Data>, the following functions must be declared for
 *	type Data:
 *		short compare (Data a, Data b)
 *		int hashvalue (Data a)
 *	(They are already defined for the basic types, in the file 'cpplib/typedefs.h').
 * Please look at the file 'testhash.cpp' for usage examples.
 *
 *********************************************************************************/

//#define Bucket Vector1<Data>
#define BUCKETCR const Bucket&
#define BucketCursor Array2Cursor<Data>
typedef Index Bucketnum;

/********************************************************************************/
/********           HashTable               *************************************/
/********************************************************************************/

TEMPLATE class HashCursor;

template<class _Data> class HashTable {

	public:
		typedef _Data Data;
		friend class HashCursor<Data>;
		typedef HashCursor<Data> Cursor;
		typedef Array2<Data> Bucket;

	protected:
		Vector1 < Bucket >  mybuckets;     

		Bucketnum bucket_num(DataCR thedata) const { 
			return hashvalue(thedata) % mybuckets.size(); 
		}
		Bucket& bucket (Bucketnum thenum) {  return mybuckets [thenum] ;}
		Bucket& bucket (DataCR thedata) {  return   bucket(bucket_num(thedata)) ;}
		const Bucket& bucket (Bucketnum thenum) const {  return mybuckets [thenum] ;}
		const Bucket& bucket (DataCR thedata) const   {  return   bucket(bucket_num(thedata)) ;}
	public:
		/* queries */
		Size buckets_count() const { return mybuckets.size(); }

		/* construct & destruct */
		HashTable(Size the_buckets_count=2): mybuckets(the_buckets_count) {}
		void free() { 
			::free (mybuckets); }
		~HashTable () { /* nothing is done automatically */ }


		bool contains (DataCR thedata) const {
			const Bucket& b = bucket(thedata);
			return ::contains(b,thedata);
		}

		status append (DataCR thedata) {
			return bucket(thedata).append(thedata);
		}

		status remove (DataCR thedata) {
			Bucket& b = bucket(thedata);
			return find_and_remove (thedata,b);
		}

		void remove_all() { 
			for (Index i=0;  i<buckets_count(); ++i)  // deallocate all buckets
				mybuckets[i].remove_all();
		}


		/* resize */
		status resize (Size the_new_buckets_count) {
			Vector1	< Bucket >  my_old_buckets;
			my_old_buckets .steal (mybuckets);
			DOr ( mybuckets .realloc (the_new_buckets_count) );
			for (Bucketnum i=0; i<my_old_buckets.size(); ++i) {
				BucketCursor c (my_old_buckets[i]);
				for (; c.isvalid(); ++c)
					DOr (append ( c.item() ));
			}
			return OK;
		}

		status share (HASHTABCR thehash) { return mybuckets.share(thehash.mybuckets); }
		status steal (HASHTAB& thehash) { return mybuckets.steal(thehash.mybuckets); }

		void write_as_table (ostream& out, Format format) const {
			write (out, mybuckets, format);
		}
};

TEMPLATE Data* findp (DataCR theitem, HASHTAB& thehash) {
	CURSOR c (thehash);
	if ( c.find(theitem)==ENOTFOUND )
		return NULL;
	else
		return &c.item();
}
TEMPLATE const Data* findcp (DataCR theitem, HASHTABCR thehash) {
	CURSOR c (thehash);
	if ( c.find(theitem)==ENOTFOUND )
		return NULL;
	else
		return &c.item();
}

TEMPLATE status find_and_remove (DataCR theitem, HASHTAB& thehash) {
	return thehash.remove (theitem); }




/********************************************************************************/
/********           HashCursor              *************************************/
/********************************************************************************/

TEMPLATE class HashCursor {
	protected:
		HASHTAB* myhash;
		Bucketnum mybucketnum;
		BucketCursor mylistcursor;

		void goto_bucket (Bucketnum num) {
			mybucketnum = num;
			if (mybucketnum < myhash->buckets_count() )
				mylistcursor.reset  ( myhash->bucket(mybucketnum) );
		}

		void next_without_error_check () {
			mylistcursor.next();		   // move the cursor in the current bucket
			if (!mylistcursor.isvalid())   // end of current bucket
				goto_bucket (mybucketnum+1);
 		}


	public:

		/* queries */
		bool isvalid() const { return mylistcursor.isvalid(); } 
		bool isnextvalid() const { return 
			mylistcursor.isnextvalid() ||  mybucketnum < myhash->buckets_count()-1; };

		//bool isnextvalid() const { return mylink!=NULL && mylink->next!=NULL; }
		Data& item() 
			{ assert(isvalid()); return mylistcursor.item(); }
		DataCR item() const
			{ assert(isvalid()); return mylistcursor.item(); }
		//Data& nextitem() const 
		//	{ assert(isnextvalid()); return mylink->next->data; }
		
		/* use */
		void reset () {
			goto_bucket(0);
			for (Index i=0; i<myhash->buckets_count() && !mylistcursor.isvalid(); ++i) {
				mybucketnum = i;
				mylistcursor.reset ( myhash->bucket(mybucketnum) );
			}
		}
		void reset (DataCR d) {
			goto_bucket(myhash->bucket_num(d)); }
		void reset (HASHTABCR thehash) { 
			myhash = (HASHTAB*)(&thehash);  reset(); }
		void reset (HASHTABCR thehash, DataCR d) { 
			myhash = (HASHTAB*)(&thehash);  reset(d); }
		void assign (CURSORCR c) { 
			myhash = c.myhash;
			mybucketnum = c.mybucketnum;
			mylistcursor = c.mylistcursor;
		}

		HashCursor (): mylistcursor() { myhash=NULL; }
		HashCursor (HASHTABCR thehash) { reset(thehash); }
		HashCursor (CURSORCR thecursor) { assign(thecursor); }
		HashCursor (HASHTABCR thehash, DataCR d) { reset(thehash,d); }

		void operator= (CURSORCR thecursor) { assign(thecursor); }

		void next_in_bucket() {
			if (isvalid())
				mylistcursor.next();		   // move the cursor in the current bucket
		}
		void next() { 
			if (isvalid()) {
				mylistcursor.next();		   // move the cursor in the current bucket
				if (!mylistcursor.isvalid())	// end of current bucket
				while (!mylistcursor.isvalid() && mybucketnum < myhash->buckets_count() )  
					goto_bucket (mybucketnum+1);
			}
		}
		void operator++() { next(); }

		CURSOR* clone() const { return new CURSOR (*this);	}

		status find (Data d) {
			goto_bucket ( myhash->bucket_num(d) );
			return ::find (d, mylistcursor);
		}

};



/* 
	IO operations
*/

/* 
Format options:
	'T' as the first format char means to print the hash at table format. 
		The next format string gives the format for the vector of buckets and 
		for each bucket.
		for example, "T\n," means that '\n' will be the bucket-separator and 
			',' will be the item-separator (each bucket will be printed in a 
			different row, and in each bucket, the items will be comma-separated.
	'P' as the first char means to print the hash as a list of elements, enclosed 
		by parenthesis. the next char is the separator. for example, "P\n" means 
		that each element will be printed in a different row. "P " means that
		the elements will be space-separated.
	if '*' is the first char it will be skipped.
	any other char means that the hash will be printed as a list with no parenthesis
		(the first char is the separator).
*/
TEMPLATE void write (ostream& out, HASHTABCR thehash, Format format) {
	if (format[0]=='T')       //    Table output format
		thehash.write_as_table(out,format.next());
	else {                     //    List output format
		if (format[0]=='P') {      // List with parenthesis
			out << '{';
			write (out,thehash,format.next());
			out << '}';
		}
		else {					   // List without parenthesis
			if (format[0]=='*')  format=format.next();
			CURSOR c(thehash);
			if (c.isvalid())   write (out,c.item(),format.next()); // write first element
			for (++c; c.isvalid(); ++c) {
				format.separate(out);
				write (out, c.item(), format.next() );  
			}
		}
	}
}

TEMPLATE void write (ostream& out, HASHTABCR thehash) { write (out,thehash,Format("P")); }

TEMPLATE status read (istream& in, HASHTAB& thehash, Format format) { 
//	if (format[0]=='P')   {         // Parenthesized format: [n: a1 a2 ... an]
		format = format.next();
		DOr(testchar(in,'{'));
		return read_container (in,thehash, /*separator*/format[0], /*stopper*/'}', format.next()); 
//	}
}




#undef TEMPLATE
#undef HASHTAB
#undef HASHTABCR
#undef CURSOR
#undef CURSORCR

#endif
