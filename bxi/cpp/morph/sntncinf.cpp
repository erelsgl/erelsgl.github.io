/*
 * File morph/sntncinf.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncinf.cpp
 */

/* sntncinf.cpp */

#include <morph/sntncinf.h>


status SentenceInfo::hafred_whosef (MorphInfoCR theinfoc)  {
	MorphInfo theinfo = theinfoc;
	if (theinfo.vav()) { DOr(append(miW)); ; theinfo.setvav(false); }
	if (theinfo.jiabud()!=NO_JIABUD) { DOr(append(jiabud_info(theinfo.jiabud()))); theinfo.setjiabud(NO_JIABUD); }
	if (theinfo.otiotyaxas()!=NO_OY) { DOr(append(otiotyaxas_info(theinfo.otiotyaxas()))); theinfo.setotiotyaxas(NO_OY); }
	return append (theinfo);
}


/*status SentenceInfo::hafred ()  {
	LOOPTHISVECTOR (;,i) {
		MorphInfo& curinfo = at(i);
		if (curinfo.vav()) { DOr(insert(i,miW)); i++; at(i).setvav(false); }
		if (curinfo.jiabud()!=NO_JIABUD) { DOr(insert(i,jiabud_info(curinfo.jiabud()))); i++; at(i).setjiabud(NO_JIABUD); }
		if (curinfo.otiotyaxas()!=NO_OY) { DOr(insert(i,otiotyaxas_info(curinfo.otiotyaxas()))); i++; at(i).setotiotyaxas(NO_OY); }
	}
	return OK;
}*/

status SentenceInfo::hafred_txiliot (SentenceInfo& the_result) const  {
	the_result.reset(count());
	LOOPTHISVECTOR (;,i) {
		MorphInfo curinfo = at(i);
		if (curinfo.vav()) { DOr(the_result.append(miW)); curinfo.setvav(false); }
		if (curinfo.jiabud()!=NO_JIABUD) { DOr(the_result.append(jiabud_info(curinfo.jiabud()))); curinfo.setjiabud(NO_JIABUD); }
		if (curinfo.otiotyaxas()!=NO_OY) { DOr(the_result.append(otiotyaxas_info(curinfo.otiotyaxas()))); curinfo.setotiotyaxas(NO_OY); }
		DOr(the_result.append(curinfo));
	}
	return OK;
}

status SentenceInfo::hafred_txiliot_yaxas_wjiabud (SentenceInfo& the_result) const  {
	the_result.reset(count());
	LOOPTHISVECTOR (;,i) {
		MorphInfo curinfo = at(i);
		if (!curinfo.vav()) { 
			if (curinfo.jiabud()!=NO_JIABUD) { DOr(the_result.append(jiabud_info(curinfo.jiabud()))); curinfo.setjiabud(NO_JIABUD); }
			if (curinfo.otiotyaxas()!=NO_OY) { DOr(the_result.append(otiotyaxas_info(curinfo.otiotyaxas()))); curinfo.setotiotyaxas(NO_OY); }
		}
		DOr(the_result.append(curinfo));
	}
	return OK;
}

status SentenceInfo::hafred_txiliot_yaxas (SentenceInfo& the_result) const  {
	the_result.reset(count());
	LOOPTHISVECTOR (;,i) {
		MorphInfo curinfo = at(i);
		if (!curinfo.vav() && curinfo.jiabud()==NO_JIABUD) {
			if (curinfo.otiotyaxas()!=NO_OY) { DOr(the_result.append(otiotyaxas_info(curinfo.otiotyaxas()))); curinfo.setotiotyaxas(NO_OY); }
		}
		DOr(the_result.append(curinfo));
	}
	return OK;
}

/*
status SentenceInfo::axed_smikuyot (SentenceInfo& the_result) const  {
	the_result.reset(count());
	for (Index i=0; i<count(); ++i) {
		MorphInfo curinfo = at(i);
		if (curinfo.smikut()==NISMAK) { 
			curinfo.setyidua (KNMEYUDA);
			curinfo.setsmikut (NIFRAD);
			for (i++; i<count(); ++i) {        // skip all nismakim
				MorphInfo nextinfo = at(i);
				if (!nextinfo.can_be_somek())  return EBADFORMAT;
				else if (nextinfo.smikut()!=NISMAK) break;
			}
		}
		DOr(the_result.append(curinfo));
	}
	return OK;
}
*/

bool identical (SentenceInfoCR a, SentenceInfoCR b) {
	if (a.count()!=b.count()) return false;
	LOOPVECTOR(;,a,i)  if (!identical(a[i],b[i])) return false;
	return true;
}


void sfor_hevdelim (SentenceInfoCR a, SentenceInfoCR b, uint& hevdelim_klaliim, uint& hevdelei_txiliot, uint& hevdelei_baseword) {
	if (a.count()!=b.count()) {
		hevdelim_klaliim = hevdelei_baseword = hevdelei_txiliot =
			a.count()+b.count()+1; // &erk gadol bmyuxxad.
		return;
	}
	LOOPVECTOR (;,a,w) {
		if (!identical_baseword(a[w],b[w]))  ++hevdelei_baseword;
		if (!identical_except_txiliot(a[w],b[w]))  ++hevdelei_txiliot;
		if (!identical(a[w],b[w]))  ++hevdelim_klaliim;
		//if (!identical(a[w],b[w])) {  // log the mistake	
		//	errorlogfile	<< "M#" << num_of_words << " (" << curword << "): "
		//					<< hanitux_jelanu << "    C\"L   " << hanitux_hanakon << endl;
		//}
	}
}



/*********** SentenceInfo IO ********************/


status SentenceInfo::read (istream& in) {
	remove_all();
	MorphInfo curinfo;
	for (;;) {
		DOEOFr(::read(in,curinfo));
		DOr(append (curinfo));
		char c = curinfo.baseword()[0];
		if (c=='.' || c=='?' || c=='!') return OK;
	}
	return OK;
}

void SentenceInfo::write (ostream& out) const {
	write_vector (out,*this," ",Format("")); }




/*******************************************************/
/*********** SentenceInfo Self Test ********************/
/*******************************************************/

#ifdef TEST_SNTNCINF

#include <cpplib/cmdline1.h>

ifstream input;
ofstream output;
SentenceInfo cursentence;

void main (int argc, char* argv[]) {
	set_synopsis ("\nHAFRED *.to *.to2\n");
	parse_command_line (argc,argv,2,2,"","");
	open_infile (variable(0),input);
	open_outfile (variable(1),output,0);
	SentenceInfo s1;
	for (;;) {
		DOEOFx( cursentence.read(input,".") );
		cursentence.hafred_txiliot(s1);
		s1.write (output);
		output << endl;
	}
}


#endif
