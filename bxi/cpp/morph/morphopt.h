/*
 * File morph/morphopt.h
 * Written by Erel Segal (mailto:erelsgl@cs.technion.ac.il)
 * Released under the GPL (http://www.gnu.org/licenses/gpl.html)
 * Available online at http://www.cs.technion.ac.il/~erelsgl/bxi/cpp/morph/morphopt.h
 */

/* morphopt.h -- morphological options structures */

#ifndef __MORPHOPT_H
#define __MORPHOPT_H

#include <morph/morphinf.h>
#include <cpplib/vector1.hxx>


/* class MorphOptions is used to hold all the possible morphological options for *
 * a single word.																   */
typedef Vector1<MorphInfo> MorphOptions;
typedef const MorphOptions& MorphOptionsCR;

DEFINE_INLINE_IO_OPERATORS(MorphOptions);

#endif
