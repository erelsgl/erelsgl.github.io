/*
 * File morph/sntncsik.cpp
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/sntncsik.cpp
 */

/* sntncsik.cpp -- 
	Sentence Options with Sikuiim:
		a structure that holds all options to analyze a sentence, with "sikui" attached to 
		each option.
*/

#include <morph/sntncsik.h>
#include <morph/sntncanl.h>
#include <morph/taxbir.h>
#include <cpplib/sentence.h>
#include <cpplib/BFS.hxx>



/****************  implementation of create_sentence_options_from_eachword_options  ********************/
/****************  implementation of create_sentence_options_from_eachword_options  ********************/
/****************  implementation of create_sentence_options_from_eachword_options  ********************/

extern ofstream sntncsiklogfile;



/**********          SOI-BFS               **********/




struct SentenceConverter: public BestFirstSearcher<SentenceOptionIdentifier> {
	ArrayOfMOWS my_eachword_options;
	uint my_DNOSO;   // the desired number of sentence options
	SentenceOptionsWithSikui* my_sentence_options_p;
	#define my_sentence_options (*my_sentence_options_p)
	Vector1<double> my_sentence_options_sikuiim;	// DNOSO ha-sikuiim ha-@obim bioter &bur ha-mi$pa@ ha-nokxi  
	Index my_NOSOFSF;            // The number of sentence-options found so far [  0 <= my_NOSOFSF < my_DNOSO  ]

	SentenceInfo cur_sentence_option_mufrad;
	//SentenceInfo cur_sentence_option_lo_mufrad;


	SentenceConverter (ArrayOfMOWS the_eachword_options,	uint the_DNOSO,  SentenceOptionsWithSikui& the_sentence_options):
		BestFirstSearcher<SentenceOptionIdentifier> (the_eachword_options.count()) 
	{
		my_eachword_options = the_eachword_options;
		my_DNOSO = the_DNOSO;
		my_NOSOFSF = 0;
		my_sentence_options_p = &the_sentence_options;
		free (my_sentence_options);  my_sentence_options.realloc(the_DNOSO);
		my_sentence_options_sikuiim.realloc (the_DNOSO);
		zero (my_sentence_options_sikuiim);	
		sader_lfi_sikui_bseder_yored(my_eachword_options);
		sntncsiklogfile<<my_eachword_options<<endl;
	}

	
	double score (SentenceOptionIdentifierCR cur_soi)  {
		return cur_soi.my_realsikui; }


	status enqueue_first_nodes() {
		SentenceOptionIdentifier first_soi (my_eachword_options);
		first_soi.zero();      first_soi.xajev_sikui();
		return enqueue(first_soi);
	}


	status enqueue_next_node (SentenceOptionIdentifierCR cur_soi, Index w) {
		// w is the "direction" (or "coordinate").
		if (w>=cur_soi.count()) return OK;
		if (cur_soi[w]+1 >= cur_soi.my_eachword_options[w].count()) return OK; // no "next_node" in coordinate w.
		SentenceOptionIdentifier new_soi;
		duplicate (new_soi,cur_soi);
		new_soi[w] = cur_soi[w]+1;
		new_soi.xajev_sikui();
		// ^im ha-cerrup lo^ xuqi -- nasse lhosip ^et ha-ba^im ^axaraw 
		bool 
			cx1 = haceruf_xuqi(new_soi.wordinfo(w-1),new_soi.wordinfo(w)),
			cx2 = haceruf_xuqi(new_soi.wordinfo(w),new_soi.wordinfo(w+1));
		if (cx1 && cx2) 
			return enqueue(new_soi); 
		else if (!cx1 && cx2) {  
			DOr(enqueue_next_node (new_soi,w-1));
			return enqueue_next_node (new_soi,w);
		}
		else if (cx1 && !cx2) {  
			DOr(enqueue_next_node (new_soi,w));
			return enqueue_next_node (new_soi,w+1);
		}
		else if (!cx1 && !cx2) {  
			return enqueue_next_node (new_soi,w);
		}
		return EINVAL;         // I can't reach here anyway.
	}

	status enqueue_next_nodes(SentenceOptionIdentifierCR cur_soi) {
		sntncsiklogfile<<my_NOSOFSF<<":: "<<cur_soi.my_realsikui<<": "<<cur_soi<<endl;
		cur_soi.create_sentenceinfo_mufrad( cur_sentence_option_mufrad);
		//my_sentence_options_sikuiim[my_NOSOFSF] = cur_soi.my_realsikui;
		sntncsiklogfile<<cur_sentence_option_mufrad<<endl;

		cur_soi.create_sentenceinfo_lo_mufrad( my_sentence_options.info(my_NOSOFSF) );
		my_sentence_options_sikuiim[my_NOSOFSF] = cur_soi.my_realsikui;
		++my_NOSOFSF;  if (my_NOSOFSF>=my_DNOSO) return EOF;

		LOOPVECTOR (;,my_eachword_options,w) 
			DOr(enqueue_next_node (cur_soi,w));

		return OK;
	}


	status finish () {
		if (my_NOSOFSF==0) return ENOTFOUND;        // couldn't create any legal option!
		my_sentence_options.truncate(my_NOSOFSF);
		my_sentence_options_sikuiim.truncate(my_NOSOFSF);
		normalize (my_sentence_options_sikuiim,1.);
		for (Index i=0; i<my_NOSOFSF; ++i) 
			my_sentence_options.sikui(i) = sikui(my_sentence_options_sikuiim[i]);
		writeln (sntncsiklogfile,my_sentence_options);
		return OK;
	}


	~SentenceConverter () {
		free (my_sentence_options_sikuiim);
		free (cur_sentence_option_mufrad);
	}

};



/******************  void create_sentence_options_from_eachword_options  ***********************/

status convert_eachword_options_to_sentence_options (
	ArrayOfMOWS the_eachword_options,
	uint the_DNOSO,     // desired number of sentence-options
	SentenceOptionsWithSikui& the_sentence_options
) {
	SentenceConverter the_converter(the_eachword_options,the_DNOSO,the_sentence_options);
	return the_converter.run();
}






/******************  SNTNCSIK self-test  ***********************/
/******************  SNTNCSIK self-test  ***********************/
/******************  SNTNCSIK self-test  ***********************/

#ifdef TEST_SNTNCSIK


#include <morph/alghist.h>
#include <morph/mn.h>
#include <morph/tiqun2.h>
#include <morph/hpmaio2.h>

//#define LOG_PATH "..\\..\\..\\harc\\"
#define LOG_PATH "c:\\winnt\\profiles\\harc\\"

void atxel_klali_s() {
	atxel_global_database (NULL,NULL,LOG_PATH "harc10.nts");
	atxel_local_database (NULL,NULL,LOG_PATH "harc10a.nts");
	initialize_the_sentence_analyzer(LOG_PATH);
	atxel_tiqunim(LOG_PATH); adken_tiqunim (LOG_PATH "harc10.to"); 
	ktov_tiqunim(concat_path_to_filename(LOG_PATH,"tiqunim3.ma"));
	adken_monei_zugot_nituxim (LOG_PATH "harc10.to");
	open_logfile(6); ktov_monei_zugot_nituxim(logfile); logfile.close();
}




void bdoq() {
	Sentence cursentence(500);
	ArrayOfMOWS cursentence_eachword_options;
	SentenceOptionsWithSikui cursentence_options; // 8 ha-^ep$aruiot ha-@obot bioter &bur ha-mi$pa@ ha-nokxi
	for(;;) {
		//read_and_analyze_the_next_sentence (input, cursentence, cursentence_eachword_options);
		DOEOFx(read_and_analyze_the_next_sentence (input, cursentence, cursentence_eachword_options));
		//LOG (logfile, cursentence);
		taqen_sikuiim_zugot (cursentence_eachword_options);
		convert_eachword_options_to_sentence_options (cursentence_eachword_options, 8, cursentence_options);
		writeln (logfile, cursentence_options);
		logfile << endl << endl << endl;
	}
}

void main (void) {
	log_path = "..\\..\\..\\harc";
	atxel_klali_s();
	open_logfile(0);
	open_sntncanllogfile(log_path);
	open_sntncsiklogfile(log_path);
	open_infile ("..\\..\\..\\harc\\harc10a.txt",input);
	bdoq();
}

#endif
