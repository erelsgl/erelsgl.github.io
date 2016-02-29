/*
 * File morph/hpmaio.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/hpmaio.cpp
 */

/* hpmaio.cpp -- IO routines for HPMA */

#include <morph/hpmaio.h>
#include <cpplib/sgt.h>
#include <cpplib/cmdline1.h>


ifstream input, correct_analysis;
ofstream output;
CStr log_path;


StringTemp filename (CStr basic_filename) {
	return concat_path_to_filename (log_path, basic_filename);
}

StringTemp filename (CStr basic_filename, CStr extension) {
	return concat_filename_to_extension (
		concat_path_to_filename (log_path, basic_filename).str,
		extension);
}


ofstream errorlogfile;
void open_errorlogfile (CStr log_path) {
	StringTemp thefilename = concat_path_to_filename  (log_path,"logerr.ma2");
	open_outfile (thefilename.str,errorlogfile,ios::trunc);
}
void close_errorlogfile () { errorlogfile.close(); }


ofstream lexlogfile;
void open_lexlogfile (CStr log_path) {
	StringTemp thefilename = concat_path_to_filename  (log_path,"loglex.ma2");
	open_outfile (thefilename.str,lexlogfile,ios::trunc);
	lexlogfile << "% nittuxim ba-qorpus ha-mnuttax, $e-lo^ nimc^u b-qbucat ha-nittuxim $ell ha-mnattex ha-curani" << endl;
}
void close_lexlogfile () { errorlogfile.close(); }


ofstream bituyimlogfile;
void open_bituyimlogfile (CStr log_path) {
	StringTemp thefilename = concat_path_to_filename  (log_path,"logbituy.ma2");
	open_outfile (thefilename.str,bituyimlogfile,ios::trunc);
}
void close_bituyimlogfile () { bituyimlogfile.close(); }



ofstream tiqunimlogfile;
void open_tiqunimlogfile (CStr log_path, Index thenum) {
	StringTemp thefilename = concat_path_to_filename  (log_path,format("logtiq%d.ma2",thenum).str);
	open_outfile (thefilename.str,tiqunimlogfile,ios::trunc);
}
void close_tiqunimlogfile () { tiqunimlogfile.close(); }


ofstream sntncanllogfile;
void open_sntncanllogfile (CStr log_path) {
	StringTemp thefilename = concat_path_to_filename  (log_path,"logsntncanl.ma2");
	open_outfile (thefilename.str,sntncanllogfile,ios::trunc);
}
void close_sntncanllogfile () { sntncanllogfile.close(); }


ofstream sntncsiklogfile;
void open_sntncsiklogfile (CStr log_path) {
	StringTemp thefilename = concat_path_to_filename  (log_path,"logsntncsik.ma2");
	open_outfile (thefilename.str,sntncsiklogfile,ios::trunc);
}
void close_sntncsiklogfile () { sntncsiklogfile.close(); }

ofstream logfile;
void open_logfile (Index thenum) {
	StringTemp thefilename = concat_path_to_filename (log_path, format("log%d.ma2",thenum).str );
	open_outfile (thefilename.str,logfile,ios::trunc);
}

void open_sgtlogfile (CStr log_path) {
	StringTemp thefilename = concat_path_to_filename  (log_path,"logSGT.ma2");
	open_outfile (thefilename.str, SimpleGoodTuringEstimator::logstream, ios::trunc);
}
void close_sgtlogfile () { SimpleGoodTuringEstimator::logstream.close(); }


void open_infile_with_messages (CStr filename, ifstream& input) {
	cerr << "reading input file " << filename << endl;
	open_infile (filename,input);
}

void open_outfile_with_messages (CStr filename, ofstream& output, open_mode mode) {
	cerr << "writing into output file " << filename << " (mode=" << mode << ")" << endl;
	open_outfile (filename,output,mode);
}



void skip_spaces_and_stars (istream& input) {
	while (charinstring(input.peek()," \t\n*")) 
		input.get();
}


void log_the_mistake (Index ms_hamila, CStr hamila, MorphInfoCR hanitux_jelanu, MorphInfoCR hanitux_hanakon, ofstream& thelogfile) {
	if (thelogfile.is_open()) 
		thelogfile  << "    M#" << ms_hamila << " (" << hamila << "): "
		            << hanitux_jelanu << "    C\"L   " << hanitux_hanakon << endl;
}

void log_the_mistake_loud (Index ms_hamila, CStr hamila, MorphInfoCR hanitux_jelanu, MorphInfoCR hanitux_hanakon, ofstream& thelogfile) {
	if (thelogfile.is_open()) 
		thelogfile  << "    M#" << ms_hamila << " (" << hamila << "): "
		            << hanitux_jelanu << "    C\"L   " << hanitux_hanakon << endl;
	cerr	<< "    M#" << ms_hamila << " (" << hamila << "): "
			<< hanitux_jelanu << "    C\"L   " << hanitux_hanakon << endl;
}


