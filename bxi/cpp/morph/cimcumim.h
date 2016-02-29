/*
 * File morph/cimcumim.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/cimcumim.h
 */

/* cimcumim.h -- cimcumim &abur mnattex taxbiri */


#ifndef __CIMCUMIM_H
#define __CIMCUMIM_H

#include <cpplib/vector1.hxx>
#include <cpplib/array2.hxx>
#include <morph/mip.h>
#include <morph/sntncinf.h>



/******************************   XoqCimcum   ***********************************************/
/******************************   XoqCimcum   ***********************************************/
/******************************   XoqCimcum   ***********************************************/

class XoqCimcum;
typedef const XoqCimcum& XoqCimcumCR;

class XoqCimcum {
protected:
	Array2<MorphInfoPattern> reija;
	MorphInfoPattern seifa;
	uint sug;
	Index index_bsis_hatocaa;        // ha-basis $ell ha-toca^a hu^  hamijpat[index_bsis_hatocaa];

	void atxel_et_index_bsis_hatocaa();

	status hosef_maqaf () { 
		DOr(reija.insert(1,miMAQAF)); 
		seifa.baseword()[2] = 'c';
		atxel_et_index_bsis_hatocaa();
		return OK;
	}

	double ciyun_cimcumi (MorphInfoCR the_w, MorphInfoCR the_x, MorphInfoCR the_y) const;

public:
	XoqCimcum(Size thesize=4): reija(thesize) {}

	uint ork () const { return reija.count(); }
		// ha-xoqq hu^:    reija[0] reija[1] ... reija[ork-1]  ==>  seifa  

	bool match (Index theindex, MorphInfoCR theinfo) const { return reija[theindex].match(theinfo); }
	bool match (SentenceInfoCR ha_mijpat, Index ha_hatxala) const;
		// bodeq ^im ha-xoqq mat^im la-qe@& $ell "ha_mijpat" $e-matxil b-"ha_hatxala"
	bool match (SentenceInfoCR ha_mijpat, Index ha_hatxala, MorphInfo& ha_tocaa, double* ha_ciyun_p=NULL) const;
		// bodeq ^im ha-xoqq mat^im la-qe@& $ell "ha_mijpat" $e-matxil b-"ha_hatxala", w-^im
		//	ken -- maxzir ^et "ha_tocaa" $e-titqabbel mi-ha-cimcum $ell qe@& ze.

	void ktov_barur (ostream& out) const;
	status read  (istream& in);
	void duplicate (XoqCimcumCR from) {
		::duplicate (reija, from.reija);
		::duplicate (seifa, from.seifa);
		sug = from.sug;
		index_bsis_hatocaa = from.index_bsis_hatocaa;
	}

	friend status qra_cimcumim (CStr path_to_cimcumim);
	friend void ktov_cimcumim_barur (CStr path_to_cimcumim);
};

inline status read  (istream& in, XoqCimcum& theinfo, Format format="") { return theinfo.read (in); }
inline void write (ostream& out, XoqCimcumCR theinfo, Format format="") { theinfo.ktov_barur(out); }
inline void duplicate (XoqCimcum& to, const XoqCimcum& from) { to.duplicate(from); }
inline void free (XoqCimcum& m) { };


bool lapoal_yej_nose (MorphInfoCR the_poal);
bool lapoal_yej_musa_yajir (MorphInfoCR the_poal);
double ciyun_cimcumi (MorphInfoCR the_poal);


extern Vector1<XoqCimcum> hacimcumim;



#endif
