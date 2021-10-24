/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "dcs_source_f_impl.h"

namespace gr {
  namespace dcsinfo {

    uint32_t dcs_bits;
    uint32_t index;

    dcs_source_f::sptr
    dcs_source_f::make(int dcs_code)
    {
      return gnuradio::get_initial_sptr
        (new dcs_source_f_impl(dcs_code));
    }

    /*
     * The private constructor
     */
    dcs_source_f_impl::dcs_source_f_impl(int dcs_code)
      : gr::sync_block("dcs_source_f",
                gr::io_signature::make(0,0, 0),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
      
      index = 1;
      dcs_bits = calcGolay(dcs_code);
    }

    /*
     * Our virtual destructor.
     */
    dcs_source_f_impl::~dcs_source_f_impl()
    {
    }

    /**
    *
    */
    uint32_t dcs_source_f_impl::calcGolay(int golay)
    {
      bool negate = golay < 0;
      if(negate){
        golay = -golay;
      }
      uint32_t buff = 0;
      uint16_t parity = 0;
      bool c1,c2,c3,c4,c5,c6,c7,c8,c9;
      bool p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11;
      /**
      * The DCS code itself
      * We have an integer, say 023 (dec), we want 
      * to split it up into 0, 2, 3
      */ 
      int d1 = 0x7 & (golay % 10);//least significant part
      int d2 = 0x7 & (((golay  - d1)/10) % 10);
      int d3 = 0x7 & (((golay- 10*d2 -d3)/100) % 10);//most significant part

      //Least significant goes rightmost in the buffer
      buff |= (d3 << 6) | (d2 <<3) | d1;

      /**
      * The magic marker
      * xxxxxxxxxxx-FFF-ccccccccc
      */
      buff |= 0x800;// 1000 0000 0000
      /*
      * Parity calculation
      */
      c1 = buff & 1;
      c2 = (buff >> 1) & 1;
      c3 = (buff >> 2) & 1;
      c4 = (buff >> 3) & 1;
      c5 = (buff >> 4) & 1;
      c6 = (buff >> 5) & 1;
      c7 = (buff >> 6) & 1;
      c8 = (buff >> 7) & 1;
      c9 = (buff >> 8) & 1;


      p1 =  ( c1 ^ c2 ^ c3 ^ c4 ^ c5 ^ c8  )     ;
      p2 =  ( !( c2 ^ c3 ^ c4 ^ c5 ^ c6 ^ c9 ) ) ;
      p3 =  ( c1 ^ c2 ^ c6 ^ c7 ^ c8  )          ;
      p4 =  (  !( c2 ^ c3 ^ c7 ^ c8 ^ c9 ) )     ;
      p5 =  (  !( c1 ^ c2 ^ c5 ^ c9 ) )          ;
      p6 =  (  !( c1 ^ c4 ^ c5 ^ c6 ^ c8)  )     ;
      p7 =  ( c1 ^ c3 ^ c4 ^ c6 ^ c7 ^ c8 ^ c9 ) ;
      p8 =  ( c2 ^ c4 ^ c5 ^ c7 ^ c8 ^ c9 )      ;
      p9 =  ( c3 ^ c5 ^ c6 ^ c8 ^ c9 )           ;
      p10 = (  !(c4 ^ c6 ^ c7 ^ c9 ) )           ;
      p11 = (  !(c1 ^ c2 ^ c3 ^ c4 ^ c7 ) )      ;

      buff |= ((p11 <<10 | p10 << 9 | p9 << 8 | p8 << 7 | 
              p7 << 6 | p6 << 5| p5 << 4 | p4 <<3 |
              p3 << 2| p2 << 1| p1 ) << 12);

      /* Validation code 
        bool f1,f2,f3;
        bool r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11;

        c1 = buff & 1;
        c2 = (buff >> 1) & 1;
        c3 = (buff >> 2) & 1;
        c4 = (buff >> 3) & 1;
        c5 = (buff >> 4) & 1;
        c6 = (buff >> 5) & 1;
        c7 = (buff >> 6) & 1;
        c8 = (buff >> 7) & 1;
        c9 = (buff >> 8) & 1;
        printf("c9..c1: %d%d%d%d%d%d%d%d%d\n" , c9,c8,c7,c6,c5,c4,c3,c2,c1);
        f1 = (buff >> 9) & 1;
        f2 = (buff >> 10) & 1;
        f3 = (buff >> 11) & 1;
        printf("f3f2f1: %d%d%d\n" , f3,f2,f1);
        r1 = (buff >> 12) & 1;
        r2 = (buff >> 13) & 1;
        r3 = (buff >> 14) & 1;
        r4 = (buff >> 15) & 1;
        r5 = (buff >> 16) & 1;
        r6 = (buff >> 17) & 1;
        r7 = (buff >> 18) & 1;
        r8 = (buff >> 19) & 1;
        r9 = (buff >> 20) & 1;
        r10 =(buff >> 21) & 1;
        r11 =(buff >> 22) & 1;
        printf("DCS : %d%d%d\n" , (buff >> 6) & 0x7, (buff >> 3) & 0x7,buff & 0x7);
        printf("GOL (calc):  %d%d%d%d%d%d%d%d%d%d%d\n" ,p11,p10,p9,p8,p7,p6,p5,p4,p3,p2,p1);
        printf("GOL (made):  %d%d%d%d%d%d%d%d%d%d%d\n\n" ,r11,r10,r9,r8,r7,r6,r5,r4,r3,r2,r1);

        printf("initialized\n");  
      }
      */
      if(negate){
        buff = ~buff;
      }
      return buff;

    }


    int
    dcs_source_f_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        //const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];
        for (int i = 0 ; i < noutput_items ; i++)
        {
          out[i] = (dcs_bits & index) > 0;
          index <<= 1;
          if ( index > 0x400000) // 0100 0000 0000 0000 0000 0000
          {
            index = 1;
          }
        }
        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace dcsinfo */
} /* namespace gr */

