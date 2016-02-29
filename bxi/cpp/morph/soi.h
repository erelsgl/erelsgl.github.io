/*
 * File morph/soi.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/soi.h
 */

/* soi.h -- Sentence Option Identifier
	An auxiliary structure that identifies one option to analyze a sentence.
*/

#ifndef __SOI_H
#define __SOI_H

#include <morph/morphsik.h>
#include <morph/sntncinf.h>



typedef Vector1<MorphOptionsWithSikui> ArrayOfMOWS;
typedef const ArrayOfMOWS& ArrayOfMOWSCR;

void sader_lfi_sikui_bseder_yored(ArrayOfMOWS& the_eachword_options);


inline void write (ostream& out, ArrayOfMOWSCR theinfo) { write (out, theinfo, Format("P|")); }
inline status read  (istream& in, ArrayOfMOWS& theinfo) { return read (in, theinfo, Format("P|")); }
DEFINE_INLINE_IO_OPERATORS(ArrayOfMOWS);



/******            struct SentenceOptionIdentifier;                 ******/

/*
	SentenceOptionIdentifier hu^ mibne $e-m$amme$ l-hagdarat nittux morpologi msuyyam $ell 
		mi$pa@ $alem.
	hu^ kolel veq@or $ell ^indeqsim, $e-myaxxes l-koll milla ba-mi$pa@ -- ^indeqs $ell nittux
		mi-tok ha-ma&arak "my_eachword_options".
*/


struct SentenceOptionIdentifier;
typedef const SentenceOptionIdentifier&  SentenceOptionIdentifierCR;
struct SentenceOptionIdentifier: public Vector1<Index> {
	double ciyun_curani;
	ArrayOfMOWS my_eachword_options;
	SentenceOptionIdentifier (): Vector1<Index>() {}
	SentenceOptionIdentifier (const ArrayOfMOWS& eachword_options) {
		my_eachword_options = eachword_options;
		Vector1<Index>::reset (my_eachword_options.count()); 
	}
	SentenceOptionIdentifier (const ArrayOfMOWS& the_eachword_options, const Vector1<Index>& the_option_indices) {
		if (the_eachword_options.count() != the_option_indices.count()) {
			cerr << "ERROR: " << 
				"the_eachword_options.count()==" << the_eachword_options.count() <<
				", the_option_indices.count()==" << the_option_indices.count() <<
				"! They should be equal." << endl;
			throw 1;
		}
		my_eachword_options = the_eachword_options;
		::duplicate ( (Vector1<Index>&)(*this), the_option_indices);
	}


	double realsikui (Index the_wordnum) const {
		return my_eachword_options[the_wordnum].realsikui(at(the_wordnum)); 
	}
	MorphInfoCR wordinfo (Index the_wordnum) const {	
		return the_wordnum<count()?
			my_eachword_options[the_wordnum].info(at(the_wordnum)):
			miNQUDA;
	}


	void xajev_sikui_golmi () {
		LOOPTHISVECTOR(ciyun_curani=0,w)  { 
			ciyun_curani += log10(realsikui(w)+0.01);
		}
	}
	void zero() {
		LOOPTHISVECTOR(;,w)  { at(w) = 0; }
	}
	void haxlef (Index wordindex, Index optionindex) {
		ciyun_curani -= log10(realsikui(wordindex)+0.01);
		at(wordindex) = optionindex;
		ciyun_curani += log10(realsikui(wordindex)+0.01);
	}

	/* ycirat ha-nitux ha-morpologi $ell ha-mi$pa@:
			1. "mufrad" == mafrid ^et ha-txiliot mi-ha-millim $e-^axreihen, w-maknis ^otan
			   la-mi$pa@ k-millim nipradot.
			2. "lo_mufrad" == lo^ mafrid ^et ha-txiliot.
	*/

	void create_sentenceinfo_mufrad (SentenceInfo& the_sentenceinfo) const {
		the_sentenceinfo.remove_all();
		LOOPTHISVECTOR(;,w)  
			the_sentenceinfo.hafred_whosef ( wordinfo(w) ); 
	}
	SentenceInfo sentenceinfo_mufrad () const {
		SentenceInfo the_sentenceinfo;
		create_sentenceinfo_mufrad (the_sentenceinfo);
		return the_sentenceinfo;
	}

	void create_sentenceinfo_lo_mufrad (SentenceInfo& the_sentenceinfo) const {
		the_sentenceinfo.remove_all();
		LOOPTHISVECTOR(;,w)   
			the_sentenceinfo.append ( wordinfo(w) ); 
	}
	SentenceInfo sentenceinfo_lo_mufrad () const {
		SentenceInfo the_sentenceinfo;
		create_sentenceinfo_lo_mufrad (the_sentenceinfo);
		return the_sentenceinfo;
	}

	friend void duplicate (SentenceOptionIdentifier& to, SentenceOptionIdentifierCR from);
	friend bool identical (SentenceOptionIdentifierCR a, SentenceOptionIdentifierCR b);
};

DEFINE_INLINE_IO_OPERATORS(SentenceOptionIdentifier);




#endif
