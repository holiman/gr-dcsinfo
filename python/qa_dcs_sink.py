#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks 
import dcsinfo_swig as dcsinfo

class qa_dcs_sink (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):

    	src_data = [ 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1,0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1,0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1,0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1,0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1]
        ending = [1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0] 
        #src_data.reverse()    
        src = blocks.vector_source_f(src_data+ending)
        dcs = dcsinfo.dcs_sink(960)
        #dst = blocks.vector_sink_f()

        self.tb.connect(src, dcs)
        #self.tb.connect(dcs, dst)

        # set up fg
        self.tb.run ()
        # check data
        #result_data = dst.data()
        #self.assertFloatTuplesAlmostEqual(src_data+ending, result_data, 6)

        """
        DCS : 207
        DCS : 732
        DCS : 164
        DCS : 207
        DCS : 732
        DCS : 164
        DCS : 207
        DCS : 732
        DCS : 164
        DCS : 207
        DCS : 732
        DCS : 164
        DCS : 207
        DCS : 732
        DCS : 164
        DCS : 207
        DCS : 732
        DCS : 164
        DCS : 207
        DCS : 732
        DCS : 164
        DCS : 207
        DCS : 732
        DCS : 164
        """


if __name__ == '__main__':
    gr_unittest.run(qa_dcs_sink, "qa_dcs_sink.xml")
