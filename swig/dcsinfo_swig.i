/* -*- c++ -*- */

#define DCSINFO_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dcsinfo_swig_doc.i"

%{
#include "dcsinfo/dcs_sink.h"
#include "dcsinfo/dcs_source_f.h"
%}


%include "dcsinfo/dcs_sink.h"
GR_SWIG_BLOCK_MAGIC2(dcsinfo, dcs_sink);
%include "dcsinfo/dcs_source_f.h"
GR_SWIG_BLOCK_MAGIC2(dcsinfo, dcs_source_f);
