/*
 * File cpplib/trie0.hxx
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/cpplib/trie0.hxx
 */

/* trie0.hxx -- a trie (a data structure to hold words). */


#ifndef __TRIE0_HXX
#define __TRIE0_HXX

#include <cpplib/map1.hxx>
#include <cpplib/stack.hxx>
#include <cpplib/stringt.h>


#define TEMPLATE template <class Data>
#define TRIE Trie0<Data>
#define TRIECR const TRIE&
#define CURSOR Trie0Cursor<Data>
#define CURSORCR const CURSOR&

#define LINK Trie0Link<Data>
#define LINKCR const LINK&

#define LINKCURSOR Trie0LinkCursor <Data>


/**************************************************************************
	A Trie0 is a data structure that functions as an map indexed by strings.
	The 'Data' must be a simple data type, such as a number or a pointer.
	If the data is equal to 0, it means that it doesn't exist (for example, a NULL pointer).
	For a usage example, see the file 'TestTrie.cpp'.

  implementation details:
	A Trie is implemented as a variable-degree tree. Each Trie0Link has a variable
		number of sons, which are labelled by characters (The sons are 
		implemented by a Map1 <char,LINK*> ).
	The data in each LINK corresponds to the string of characters from the root
		to this link. The data in the root corresponds to the empty string.		
**************************************************************************/


/**************************************************************************/
/**********        Trie0Link                        ***********************/
/**************************************************************************/

TEMPLATE struct Trie0Link {
	Data mydata;                   // each link has an associated data.
	Map1 <char,LINK*> mysons;      // for each char, there is a link downwards.

	Trie0Link (Data thedata=0): mysons() { mydata = thedata; };

	status insert_son (char c, LINK* theson) { return mysons.insert (c,theson); }
	status remove_son (char c) { return mysons.remove (c); }
	status contains_son (char c) const { return mysons.contains (c); }
	status sons_count() const { return mysons.count(); }
	status has_sons() const { return mysons.count() > 0; }

	LINK* sonp (char c) {
		// returns the son that corresponds to char c. If there is none -- returns NULL.
		LINK** sonpp = mysons.itemp(c);
		if (sonpp==NULL)	return NULL;
		else return *sonpp;
	}
	const LINK* sonp (char c) const {
		// returns the son that corresponds to char c. If there is none -- returns NULL.
		LINK* const * sonpp = mysons.itemp(c);
		if (sonpp==NULL)	return NULL;
		else return *sonpp;
	}

	LINK* sonp_insert (char c) {
		// returns the son that corresponds to char c. If there is none -- creates one and returns it.
		LINK** sonpp = mysons.itemp(c);
		if (sonpp==NULL) {    // insert a new son
			LINK* sonp = new LINK();
			if (sonp==NULL) return NULL;  // NO MEMORY!
			DOx (insert_son (c,sonp));
			return sonp;
		}
		else 
			return *sonpp;
	}
};


TEMPLATE struct Trie0LinkCursor: public Map1Cursor <char,LINK*> {
	void reset (LINK* thelink) { Map1Cursor<char,LINK*>::reset(thelink->mysons); }
};



/**************************************************************************/
/**********        Trie0                            ***********************/
/**************************************************************************/

TEMPLATE class Trie0Cursor; 


template <class Data_>  class Trie0 {
	public:
		typedef Data_ Data;
		friend class CURSOR;
	protected:
		LINK* myroot;

		LINK* link_with_key (CStr thekey, Size keylength) const {
			LINK* l = myroot;
			for (Index i=0; i<keylength; ++i) {
				l = l->sonp (thekey[i]);
				if (l==NULL) return NULL;
			}
			return l;
		}
		LINK* link_with_key (CStr thekey) const {
			return link_with_key (thekey,strlen(thekey));
		}

		LINK* link_with_key_insert (CStr thekey) {
			LINK* l = myroot;
			for (Index i=0; i<strlen(thekey); ++i) {
				l = l->sonp_insert (thekey[i]);
				if (l==NULL) return NULL;
			}
			return l;
		}


	public:
		typedef CURSOR Cursor;

		#define LOOPTHISTRIE(cursor) for (Cursor cursor(*this);  cursor.isvalid();  ++cursor)
		#define LOOPTRIE(init,thetrie,cursor) init; for (cursor.reset(thetrie); cursor.isvalid(); ++cursor)

		typedef CStr Key;
		/****** queries ***********/
		bool isempty() const { return myroot==NULL; }
		Data& myrootitem() { assert(!isempty());  return myroot->mydata; }
		Size  count() const { return 0; }
		status resize (Size) { return true; }

		/***** construct *******/
		Trie0 (Size thesize=0) { 
			DOPx (myroot=new LINK() ); 
		}
		void free() { }      // not implemented yet
		~Trie0() { /* nothing is done */ }

		/* insert -- 
			Inserts the data to this trie, using the key as index.
			If there is already a data corresponding to this key, it will be 
				replaced.  
			theitemp is set to point to the new data inserted.
		*/
		status insert (CStr thekey, Data thedata, Data*& theitemp) {
			if (thedata==0) {  
				remove (thekey);        // 0 means that thedata does not exist
				return OK;
			}
			LINK* l = link_with_key_insert (thekey);
			if (l==NULL) return ENOMEM;
			l->mydata = thedata;
			theitemp = &l->mydata;
			return OK;
		}

		status insert (CStr thekey, Data thedata) {
			Data* dummy;
			return insert (thekey,thedata, dummy); 
		}


		/* insert_if_absent -- 
			Inserts the data to this trie, using the key as index.
			If there is already a data corresponding to this key, it will be 
				returned.  
			theitemp is set to point to the new data inserted.
		status insert (CStr thekey, Data thedata, Data*& theitemp) {
			if (thedata==0) {  
				remove (thekey);        // 0 means that thedata does not exist
				return OK;
			}
			LINK* l = link_with_key_insert (thekey);
			if (l==NULL) return ENOMEM;
			l->mydata = thedata;
			theitemp = &l->mydata;
			return OK;
		}
		*/




		/* remove -- */
		/*	 Returns ENOTFOUND if the key is not in this map  */
		/*	 If there are more than one -- removes one of them (no error check). */
		/*   If the removed link is at the end of a 'wire' -- only the end will be */
		/*		removed, not the entire 'wire'.										*/
		status remove (CStr thekey) {
			Size len = strlen(thekey);
			char lastchar = thekey[len-1];
			LINK* father_of_removed = link_with_key (thekey,len-1);
			if (father_of_removed==NULL) return ENOTFOUND;
			LINK* removed = father_of_removed->sonp(lastchar);
			if (removed==NULL) return ENOTFOUND;
			if ( removed->sons_count() == 0 ) {
				DOx ( father_of_removed->remove_son (lastchar) ); }
			else 
				removed->mydata = 0;
			return OK;
		}

		void remove_all() { /* not implemented */ }



		/* itemp --  */
		/*	return a pointer to the item that corresponds to the key (or NULL if not found). */
		/*	 if there are more than one -- returns one of them (no error check). */
		Data* itemp (CStr thekey) {
			LINK* l = link_with_key (thekey);
			if (l==NULL) return NULL;
			return &l->mydata;
		}
		const Data* itemp (CStr thekey) const {
			LINK* l = link_with_key (thekey);
			if (l==NULL) return NULL;
			return &l->mydata;
		}

		Data item (CStr thekey) const {       
			// returns the item if it exists, or 0 if it does not exist.
			const Data* theitemp = itemp(thekey);
			if (theitemp==NULL) return 0;
			else return *theitemp;
		}
			
		bool contains (CStr thekey) const {
			const Data* theitemp = itemp(thekey);
			if (theitemp==NULL) return false;
			else return (!(*theitemp == 0));
		}

		bool contains (CStr thekey, DataR thedata) const {
			const Data* theitemp = itemp(thekey);
			if (theitemp==NULL || *theitemp==0) return false;
			else { thedata=*theitemp;  return true; }
		}

		//void remove_all() { while(!isempty())  remove_head(); }

		/* assignments 
		status assign (TRIECR thetrie) {
			// assignment is done by copying all elements. 
			remove_all();
			return insert_before_head (thetrie);
		}
		void operator= (TRIECR thetrie) { assign(thetrie); } */

		status share (TRIECR thetrie) {
			// If you use this assignment function, the elements of the TRIEs will be shared.
			myroot = thetrie.myroot;
			return OK;
		}

		status steal (TRIE& thetrie)  { share(thetrie); thetrie.myroot=NULL; return OK; }
       
		status read_dense (istream& in, Format format) {
			if (in.peek()=='.')  {       // empty trie
				in.get();
				return OK;
			}
			Stack<LINK*> thestack (30); // the maximum word size
			LINK* thelink=myroot;
			for (ulong i=0;;) {
				DOr(read(in,thelink->mydata,format));
				char c;
				for (;;) {     // search for a new son
				    c=in.get();  
					++i; //if (i%1000==0) cerr<<i<<" ";
					if (c==EOF) return EBADFORMAT;    
					if (c=='.') {                     // . means 'pop'.
						if (thestack.isempty()) return OK; // end of input
						thestack.pop(thelink);             // return upwards. 
					}
					else break;
				}
				DOx(thestack.push(thelink));       // push 'return address'
				thelink = thelink->sonp_insert(c); // insert the new son.
			}
		}      
};

TEMPLATE void free (TRIE& t) {
	for (CURSOR c(t); c.isvalid(); ++c) {
		free(c.item());
	}
	t.free();
}


TEMPLATE void duplicate (TRIE& to, TRIECR from) {
	free(to);
	for (CURSOR c(from); c.isvalid(); ++c) {
		to.insert ( duplicate(c.item()) );
	}
}




/**************************************************************************/
/**********        Trie0Cursor                      ***********************/
/**************************************************************************/

/* 
	A Trie0Cursor is used to traverse a Trie preorder. This means that the strings
		will be visited alphabetically. 
	Every string in the Trie will be visited,
		even a string that does not exist (such as a prefix of an existing string).
		So you should check the cursor's item() to see if it is a valid item. 
		(if item() returns NULL, it may mean that the string does not exist).
*/

TEMPLATE class Trie0Cursor { 
	protected:
		LINK* mylink;
		LINKCURSOR mylinkcursor;        // a cursor for the sons of the link
		Stack<LINKCURSOR> mystack;      // used to avoid recursion in trie-visiting.
		StringTemp mystring;            // the current string (from the root to here).

		void goto_link (LINK* thelink) {
			mylink = thelink;
			mylinkcursor.reset(mylink);
		}
	public:

		/* queries */
		bool isvalid() const { 
			return ( mylinkcursor.isvalid() || 
					 !mystack.isempty()        );   
			// see 'next()' below to understand this line.
		} 
		Data& item()  
			{ assert(isvalid()); return mylink->mydata; }
		Data& data() { return item(); }

		DataCR item() const
			{ assert(isvalid()); return mylink->mydata; }
		DataCR data() const { return item(); }


		StringTempCR string() const { return mystring; }
		const CStr& key() const { return mystring.str; }

		/* construct */
		Trie0Cursor (Size thestacksize=30): 
		  mystack(thestacksize), mystring(thestacksize+1)
			{ mylink=NULL; }

		Trie0Cursor (TRIECR thetrie, Size thestacksize=30): 
		  mystack(thestacksize), mystring(thestacksize+1) 
			{ reset(thetrie); }

		/* use */
		void reset (TRIECR thetrie) { 
			goto_link(thetrie.myroot); 
			mystack.truncate();
			mystring.truncate(0);
			while ( isvalid() && item()==0 ) {next();}
		}

		void next() { 
			if ( mylinkcursor.isvalid() )  {  // there is a new son! visit it!
				char  cur_char  = mylinkcursor.key();
				LINK* next_link = mylinkcursor.data();
				mylinkcursor.next();               // mylinkcursor is the "return address".
				DOx( mystack.push(mylinkcursor) ); // push the return address.
				DOx( mystring.append(cur_char) );
				goto_link (next_link);
			}
			else {  // there are no more sons! return to the father!
				if (mystack.isempty())  return;     // the visit in the trie is over.
				mystack.pop(mylinkcursor);
				mystring.backspace();
				next();                             // We have already been in the father -- so go on.
			}
		}

		void next_and_write (ostream& out) {       // used in the 'write' function
			if ( mylinkcursor.isvalid() )  {  // there is a new son! visit it!
				char  cur_char  = mylinkcursor.key();
				LINK* next_link = mylinkcursor.data();
				mylinkcursor.next();               // mylinkcursor is the "return address".
				DOx( mystack.push(mylinkcursor) ); // push the return address.
				out << cur_char;
				//DOx( mystring.append(cur_char) );
				goto_link (next_link);
			}
			else {  // there are no more sons! return to the father!
				if (mystack.isempty())  return;     // the visit in the trie is over.
				mystack.pop(mylinkcursor);
				out << ".";
				//mystring.backspace();
				next_and_write(out);                             // We have already been in the father -- so go on.
			}
		}


		void operator++() { 
			do { next(); } 
			while ( isvalid() && item()==0 );
		}
};



/**************************************************************************/
/**********        IO operations                    ***********************/
/**************************************************************************/

/*
	general IO operations
*/

TEMPLATE void write_trie (ostream& out, TRIECR thetrie, CStr the_keydata_separator, CStr the_line_separator, 
					 Format dataformat) {
	for (CURSOR c(thetrie);  c.isvalid();  ++c)   {
		out << c.string();
		out << the_keydata_separator; 
		write (out, c.item(), dataformat );
		out << the_line_separator;
	}
}

TEMPLATE status read_trie (istream& in, TRIE& thetrie, 
					 char the_keydata_separator, char the_line_separator, char the_stopper,
					 Format dataformat) {
	if (the_line_separator==' ' || the_line_separator=='\n')  the_line_separator='\0';
	StringTemp curkey (30);
	skip_comments(in, '%');
	if (testchar(in,the_stopper)==OK) return OK;
	for (Index i=0;;++i) {
		Data curdata;        // DON'T put it out of the loop! Think what would happen if Data is a container!
		//in.ipfx();
		skip_comments(in, '%');
		DOr ( curkey.readline(in,the_keydata_separator) );
		assert (OK==read (in, curdata, dataformat) );
		DOr ( thetrie.insert ( curkey.str, curdata ) );
		skip_comments(in, '%');
		if (testchar(in,the_stopper)==OK) return OK;
		DOr ( testchar(in,the_line_separator) );
	}
}



/*
	format IO operations
*/
/*
	The first letter of the format should be one of:
		T/P -- parenthesized table format (string1 value1\n string2 value2\n...)
			(format.next() is the seperator between string and value))
		t   -- not parenthesized, table format.
		D -- dense format (preserves the tree structure - with chars and dots).
*/
TEMPLATE void write (ostream& out, TRIECR thetrie, Format format)  {
	char f0 = format[0]; format = format.next();
	if (f0=='T' || f0=='P' || f0=='t') {  // table format with parenthesis: format.next() is the table separator
		if (f0!='t')  out << "{";
		if (format[0]==' ')	write_trie (out,thetrie,str(" "),str("\n"), format.next());
		else				write_trie (out,thetrie,str(": "),str("\n"), format.next());
		if (f0!='t')  out << "}";
	}
	else if (f0=='L')  {                   // single-line format
		out << "{ ";
		write_trie (out,thetrie,str(" "),str("  "), format.next());
		out << " }";
	}
	else /* if (f0=='D') */ {  // dense format
		for (CURSOR c(thetrie);  c.isvalid();  c.next_and_write(out) ) {
			write (out, c.item(), format );
		}
		out << '.';
	}
}

TEMPLATE void write (ostream& out, TRIECR thetrie) { write (out,thetrie,Format("T ")); }


TEMPLATE status read (istream& in, TRIE& thetrie, Format format) {
	char f0 = format[0]; format = format.next();
	if (f0=='P' || f0=='T' || f0=='t')  {  // parenthesize: format.next() is the table separator
		if (f0!='t')  DOr(testchar(in,'{'));
		if (format[0]==' ')	return read_trie (in,thetrie, ' ','\n','}', format.next() );
		else				return read_trie (in,thetrie, ':','\n','}', format.next() );
	}
	else  if (f0=='D')  {  // dense format
		return thetrie.read_dense(in,format);
	}
	else 
		return EINVAL;
}



#undef TEMPLATE 
#undef CURSOR 
#undef CURSORCR
#undef TRIE
#undef TRIECR 

#endif
