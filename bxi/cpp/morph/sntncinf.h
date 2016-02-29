/*
 * File morph/sntncinf.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncinf.h
 */

/* sntncinf.h -- 
	Sentence Info:
		a structure that holds information about a whole sentence.

*/

#ifndef __SNTNCINF_H
#define __SNTNCINF_H

#include <morph/morphinf.h>
#include <cpplib/array2.hxx>

struct SentenceInfo: public Array2<MorphInfo> {
	SentenceInfo (): Array2<MorphInfo> () {}
	SentenceInfo (Size thesize): Array2<MorphInfo> (thesize) {}
	void operator= (int ZERO) { Array2<MorphInfo>::operator=(0); }
	status hafred_txiliot (SentenceInfo& the_result) const;  // mafrid ^et ha-txiliot mi-hamilla.
	status hafred_txiliot_yaxas_wjiabud (SentenceInfo& the_result) const;  // mafrid raqq ^et ^otiot ha-yaxas w-^et txiliot ha-$i&bud (lo^ ^et w' ha-xibbur)
	status hafred_txiliot_yaxas (SentenceInfo& the_result) const;  // mafrid raqq ^et ^otiot ha-yaxas (lo^ ^et w' ha-xibbur w-lo^ ^et txiliot ha-$i&bud)
	status hafred_whosef (MorphInfoCR theinfo);

	MorphInfo wordinfo (Index w) { return  w<count()? at(w): miNQUDA; }
	status read (istream& in);             // &ocer b-^exad mitok ha-$lo$a:    . ! ? 
	void write (ostream& out) const;
};

typedef const SentenceInfo& SentenceInfoCR;

inline void write (ostream& out, SentenceInfoCR theinfo, Format format="") {
	write (out, (const Array2<MorphInfo>&)theinfo, Format(" ")); }

inline status read  (istream& in, SentenceInfo& theinfo, Format format="") {
	return read (in,(Array2<MorphInfo>&)theinfo, Format(" ")); }


bool identical (SentenceInfoCR a, SentenceInfoCR b); 
void sfor_hevdelim (SentenceInfoCR a, SentenceInfoCR b, uint& hevdelim_klaliim, uint& hevdelei_txiliot, uint& hevdelei_baseword);

DEFINE_INLINE_IO_OPERATORS(SentenceInfo)

#endif
