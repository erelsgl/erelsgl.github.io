/*
 * File morph/hajlamot.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/hajlamot.cpp
 */

/* hajlamot.cpp -- ha$lamot l-p&alim */

#include <morph/hajlamot.h>
#include <morph/hpmaio.h>

MoneHajlamot mone_hajlamot_pealim;


void MoneHajlamot1::cor_tavnit_hajlama (MorphInfoCR y, MorphInfoPattern& ha_hajlama) {
		MorphInfo yy = y.mila_rijona_info();
		HeleqDiber yhd=yy.heleqdiber();
		if (yhd==MILAT_YAXAS||yhd==MILAT_XIBUR) 
			ha_hajlama.set_baseword(yy);
		else 
			ha_hajlama.set_heleqdiber(yhd);
}

status MoneHajlamot1::add (MorphInfoCR y, Sikui ha_sikui) {
		MorphInfoPattern ha_hajlama;  cor_tavnit_hajlama(y,ha_hajlama);
		DOr(CounterVector1<MorphInfoPattern>::add(ha_hajlama,ha_sikui));
		sikui_kolel += ha_sikui;
		return OK;
}

Sikui MoneHajlamot1::sikui (MorphInfoCR y) {
		MorphInfoPattern ha_hajlama;  cor_tavnit_hajlama(y,ha_hajlama);
		return CounterVector1<MorphInfoPattern>::count(ha_hajlama);
}

double MoneHajlamot1::sikui_yaxasi (MorphInfoCR y) {
		return double(sikui(y)) / sikui_kolel;  
}

double MoneHajlamot1::sikui_yaxasi_mtuqan (MorphInfoCR y) {
		double sy = sikui(y);
		if (sy>0) return sy/sikui_kolel;
		else return 100./sikui_kolel;
}


status MoneHajlamot::add (MorphInfoCR x, MorphInfoCR y, Sikui hasikui) {
		MoneHajlamot1* ha_mone_p = itemp(x.baseword());
		if (ha_mone_p==NULL) {
			MoneHajlamot1 mone_xadaj;
			mone_xadaj.add(y,hasikui);
			return insert(x.baseword(), mone_xadaj);
		}
		else {
			return ha_mone_p->add(y,hasikui);
		}
}

MoneHajlamot1* MoneHajlamot::mone_hajlamot_p (MorphInfoCR ha_poal) {
	StringTemp bw (ha_poal.baseword());
	Index i = charindex ('-',bw.str);
	if (i<bw.len)
		bw[i]='\0';
	return itemp(bw.str);
}

Sikui MoneHajlamot::sikui (MorphInfoCR x, MorphInfoCR y) {
		MoneHajlamot1* ha_mone_p = mone_hajlamot_p(x);
		if (ha_mone_p==NULL) 
			return 0;
		else 
			return ha_mone_p->sikui(y);
}

double MoneHajlamot::sikui_yaxasi (MorphInfoCR x, MorphInfoCR y) {
		MoneHajlamot1* ha_mone_p = mone_hajlamot_p(x);
		if (ha_mone_p==NULL) 
			return 0;
		else 
			return ha_mone_p->sikui_yaxasi(y);
}

double MoneHajlamot::sikui_yaxasi_mtuqan (MorphInfoCR x, MorphInfoCR y) {
		MoneHajlamot1* ha_mone_p = mone_hajlamot_p(x);
		if (ha_mone_p==NULL) 
			return 0.1;    // bimqom 0/0
		else 
			return ha_mone_p->sikui_yaxasi_mtuqan(y);
}




void sfor_hajlama(MorphInfoCR x, MorphInfoCR y, Sikui hasikui) {
	if (!x.hu(POAL)) return;
	mone_hajlamot_pealim.add(x,y,hasikui);
}

/*
void sfor_hajlamot() {
	extern Corpus thetext;
	for(;;) {
		DOEOFx(thetext.qra_qelet(input,ms_maamarim));
		thetext.natax_qelet_1();
		for (Index w=0; w+1<thetext.kol_hanituxim_laqelet.count(); ++w) {
			if (w%100==0) cerr<<w<<" ";
			MorphOptionsWithSikuiCR kol_hanituxim_lx = thetext.kol_hanituxim_laqelet[w];
			MorphOptionsWithSikuiCR kol_hanituxim_ly = thetext.kol_hanituxim_laqelet[w+1];
			LOOPVECTOR (;,kol_hanituxim_lx,ox) { MorphInfo x=kol_hanituxim_lx.info(ox);  if (siman_pisuq(x)) continue;
				LOOPVECTOR (;,kol_hanituxim_ly,oy) { MorphInfo y=kol_hanituxim_ly.info(oy);  if (siman_pisuq(y)) continue;
					sfor_hajlama (x,y, product(kol_hanituxim_lx.sikui(ox),kol_hanituxim_ly.sikui(oy) ) );
				}
			}
		}
	}
}
*/

void ktov_hajlamot(MoneHajlamot& hahajlamot, Sikui hasaf) {
	extern ofstream bituyimlogfile;
	for (MoneHajlamot::Cursor c1(hahajlamot); c1.isvalid(); ++c1) {
		bituyimlogfile << "  " << c1.key() << ": " << c1.data().sikui_kolel << endl;
		for (MoneHajlamot1::Cursor c2(c1.data());  c2.isvalid(); ++c2) {
			if (c2.count() >= hasaf) 
				bituyimlogfile << "    " << c2.data() << " " << c2.count() << endl;
		}
	}
}


status qra_hajlamot (CStr path_to_hajlamot) {
	ifstream hajlamot_infile;
	StringTemp thefilename = concat_path_to_filename(path_to_hajlamot,"hajlamot.ma");
	cerr << "reading hajlamot file " << thefilename << endl;
	DOr(open(thefilename.str,hajlamot_infile));
	skip_comments(hajlamot_infile,'%');
	DOr(read(hajlamot_infile, mone_hajlamot_pealim, Format("P ")));
	hajlamot_infile.close();
	return OK;
}

void ktov_hajlamot (CStr path_to_hajlamot) {
	ofstream out;
	StringTemp thefilename = concat_path_to_filename(path_to_hajlamot,"hajlamotb.ma");
	DO(open(thefilename.str,out,0));
	writeln (out, mone_hajlamot_pealim, Format("P "));
	out.close();
}



