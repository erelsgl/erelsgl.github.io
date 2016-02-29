/* morphst.cpp -- morphological options trie with sikuiim */

#include <morph/morphst2.h>
#include <morph/morphanl.h>

// tries to find the word-analysis in the trie, and if not found -- analyzes it and enters it to the trie.
MorphOptionsWithSikui MorphOptionsWithSikuiTrie::analysis (CStr hamila) {
	MorphOptionsWithSikui theoptions;
	if (!contains(hamila, theoptions)) {
		theoptions = natax_im_sikui_axid(hamila); 
		if (!last_analysis_used_the_default())
			insert (hamila,theoptions);
	}
	return theoptions;
}


status MorphOptionsWithSikuiTrie::analyze_words_in_file  (istream& in, Index from_wordnum, Index to_wordnum, CStr the_stopper) {
	StringTemp cur_word(30);
	for (Index wordnum=0; wordnum<to_wordnum; ++wordnum) {
		skip_comments (in,'%');
		DOEOF (cur_word.readline(in," \t\n"));
		if (compare(cur_word.str,the_stopper)==0) return OK;

		if (wordnum<from_wordnum)     // skip words until from_wordnum
			continue;

		if (contains(cur_word.str))  continue;
		DOr(insert (cur_word.str, natax_im_sikui_axid(cur_word.str) ));
		if (wordnum%100==0) cerr << (wordnum/100) << " " << cur_word << " ";
	}
	return OK;
}



	status MorphOptionsWithSikuiTrie::read_analyses_from_file (istream& in) {  
		// read the analyses of each word, assign equal probability to each option, and insert it to the trie
		return read (in, (Trie0<MorphOptionsWithSikui>&)(*this) , Format("T:I")); }

	void MorphOptionsWithSikuiTrie::set_sikui_axid () {	
		// set_sikui_axid for all the words
		LOOPTHISTRIE(cursor) 
			cursor.item().set_sikui_axid();
	}

	status MorphOptionsWithSikuiTrie::insert (CStr hamila, MorphOptionsWithSikui theoptions) {
		return Trie0<MorphOptionsWithSikui>::insert (hamila,theoptions); }

	status MorphOptionsWithSikuiTrie::insert (CStr hamila, Index thecorrectindex) {
		MorphOptionsWithSikui& theoptions = *(itemp(hamila));
		if (thecorrectindex<theoptions.count())
			return theoptions.add_bli_nirmul (thecorrectindex,SIKUI1);
		else return OK;        // not found
	}

	status MorphOptionsWithSikuiTrie::insert (CStr hamila, MorphInfoCR hanitux) {
		MorphOptionsWithSikui theoptions;
		MorphOptionsWithSikui* theoptionsp = itemp(hamila);
		if (theoptionsp!=NULL) theoptions = *theoptionsp;
		if (theoptions.isempty()) 
			theoptions = natax_im_sikui_axid (hamila);
		Index index_hanitux = theoptions.indexof (hanitux);
		if (index_hanitux<theoptions.count())
			return theoptions.add_bli_nirmul (index_hanitux,SIKUI1);
		else return ENOTFOUND; // OK;
	}

	status MorphOptionsWithSikuiTrie::add (CStr hamila, MorphOptionsWithSikui thecorrectoptions) {
		MorphOptionsWithSikui& theoptions = *(itemp(hamila));
		return theoptions.add_bli_nirmul (thecorrectoptions);
	}

	Sikui MorphOptionsWithSikuiTrie::sikui (CStr hamila, Index theindex) const {
		MorphOptionsWithSikuiCR theoptions = item (hamila);
		if (theindex>=theoptions.count()) return 0;
		else return theoptions.sikui(theindex);
	}














// Format("P") puts parenthesis arround the trie. Format(" ") omits the parenthesis.
void write (ostream& out, MorphOptionsWithSikuiTrieCR theinfo, Format format) {
	write (out,  (Trie0<MorphOptionsWithSikui>)theinfo,  Format(
		format[0]=='P'? "T L  ": format[0]=='D'? "D": "t L  "
	));
}
//status read  (istream& in, MorphOptionsWithSikuiTrie& theinfo) { return read (in,theinfo,Format("T  ")); }


// koll ha-funqciot $e-mod_dot merxaq manixot $e-ha-mibnim mekilim bdiuq ^otan millim,
//	w-ha-hebdelim beineihem hem raqq ba-sikkuyim.

double merxaq_hamming (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b) {
	MorphOptionsWithSikuiTrie::Cursor cursor_a(a), cursor_b(b);
	double skum=0;
	for (; cursor_a.isvalid()&&cursor_b.isvalid(); ++cursor_a,++cursor_b) {
		double merxaq_nokxi = merxaq_hamming (cursor_a.item(), cursor_b.item());
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	if (cursor_a.isvalid() || cursor_b.isvalid())     // the number of words in a and b is different ==> ERROR
		return double(SIKUI_MAX);
	return skum;
}

double merxaq_euclid (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b) {
	MorphOptionsWithSikuiTrie::Cursor cursor_a(a), cursor_b(b);
	double skum=0;
	for (; cursor_a.isvalid()&&cursor_b.isvalid(); ++cursor_a,++cursor_b) {
		double merxaq_nokxi = merxaq_euclid (cursor_a.item(), cursor_b.item());
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	if (cursor_a.isvalid() || cursor_b.isvalid())     // the number of words in a and b is different ==> ERROR
		return double(SIKUI_MAX);
	return skum;
}

double merxaq_maximum (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b) {
	MorphOptionsWithSikuiTrie::Cursor cursor_a(a), cursor_b(b);
	double skum=0;
	for (; cursor_a.isvalid()&&cursor_b.isvalid(); ++cursor_a,++cursor_b) {
		double merxaq_nokxi = merxaq_maximum (cursor_a.item(), cursor_b.item());
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	if (cursor_a.isvalid() || cursor_b.isvalid())     // the number of words in a and b is different ==> ERROR
		return double(SIKUI_MAX);
	return skum;
}

double merxaq_maximum_mjuqlal (MorphOptionsWithSikuiTrieCR a, MorphOptionsWithSikuiTrieCR b) {
	MorphOptionsWithSikuiTrie::Cursor cursor_a(a), cursor_b(b);
	double skum=0;
	for (; cursor_a.isvalid()&&cursor_b.isvalid(); ++cursor_a,++cursor_b) {
		double merxaq_nokxi = merxaq_maximum_mjuqlal (cursor_a.item(), cursor_b.item());
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	if (cursor_a.isvalid() || cursor_b.isvalid())     // the number of words in a and b is different ==> ERROR
		return double(SIKUI_MAX);
	return skum;
}

double merxaq_hamming__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference) {
	MorphOptionsWithSikuiTrie::Cursor cursor_m(measured);
	double skum=0;
	for (; cursor_m.isvalid(); ++cursor_m) {
		CStr curword = cursor_m.string().str;
		double merxaq_nokxi = merxaq_hamming (cursor_m.item(), reference.item(curword) );
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	return skum;
}

double merxaq_euclid__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference) {
	MorphOptionsWithSikuiTrie::Cursor cursor_m(measured);
	double skum=0;
	for (; cursor_m.isvalid(); ++cursor_m) {
		CStr curword = cursor_m.string().str;
		double merxaq_nokxi = merxaq_euclid (cursor_m.item(), reference.item(curword) );
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	return skum;
}

double merxaq_maximum__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference) {
	MorphOptionsWithSikuiTrie::Cursor cursor_m(measured);
	double skum=0;
	for (; cursor_m.isvalid(); ++cursor_m) {
		CStr curword = cursor_m.string().str;
		double merxaq_nokxi = merxaq_maximum (cursor_m.item(), reference.item(curword) );
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	return skum;
}

double merxaq_maximum_mjuqlal__measured_to_reference (MorphOptionsWithSikuiTrieCR measured, MorphOptionsWithSikuiTrieCR reference) {
	MorphOptionsWithSikuiTrie::Cursor cursor_m(measured);
	double skum=0;
	for (; cursor_m.isvalid(); ++cursor_m) {
		CStr curword = cursor_m.string().str;
		double merxaq_nokxi = merxaq_maximum_mjuqlal (cursor_m.item(), reference.item(curword) );
		if (merxaq_nokxi==double(SIKUI_MAX)) return double(SIKUI_MAX);
		skum += merxaq_nokxi;
	}
	return skum;
}
