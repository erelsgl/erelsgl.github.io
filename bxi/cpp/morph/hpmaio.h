/*
 * File morph/hpmaio.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/hpmaio.h
 */

/* hpmaio.h -- IO routines for HPMA */

#ifndef __HPMAIO_H
#define __HPMAIO_H

#include <cpplib/stringt.h>
#include <morph/morphinf.h>
#include <fstream.h>
#include <iomanip.h>
#include <cpplib/cmdline1.h>

extern ifstream input, correct_analysis;
extern ofstream output;
extern ofstream errorlogfile;
void open_errorlogfile (CStr log_path);
void close_errorlogfile ();
extern ofstream lexlogfile;
void open_lexlogfile (CStr log_path);
void close_lexlogfile ();
extern ofstream bituyimlogfile;
void open_bituyimlogfile (CStr log_path);
void close_bituyimlogfile ();
extern ofstream tiqunimlogfile;
void open_tiqunimlogfile (CStr log_path, Index thenum=0);
void close_tiqunimlogfile ();
extern ofstream sntncanllogfile;
void open_sntncanllogfile (CStr log_path);
void close_sntncanllogfile ();
extern ofstream sntncsiklogfile;
void open_sntncsiklogfile (CStr log_path);
void close_sntncsiklogfile ();
void open_sgtlogfile (CStr log_path);
void close_sgtlogfile ();

extern CStr log_path;
extern ofstream logfile;
void open_logfile (Index thenum);
void open_infile_with_messages (CStr filename, ifstream& input);
void open_outfile_with_messages (CStr filename, ofstream& output, open_mode mode);
void skip_spaces_and_stars (istream& input);
void log_the_mistake (Index ms_hamila, CStr hamila, MorphInfoCR hanitux_jelanu, MorphInfoCR hanitux_hanakon, ofstream& thelogfile=errorlogfile);
void log_the_mistake_loud (Index ms_hamila, CStr hamila, MorphInfoCR hanitux_jelanu, MorphInfoCR hanitux_hanakon, ofstream& thelogfile=errorlogfile);

StringTemp filename (CStr basic_filename, CStr extension);

#endif

