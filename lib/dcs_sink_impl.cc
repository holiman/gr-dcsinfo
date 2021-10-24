/* -*- c++ -*- */
/* 
 * Copyright 2014 Martin Holst Swende
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

/*
*  CDCSS, also called DCS, is a digital subaudible selective 
*  signalling system. It uses a code composed of 23 bits sent 
*  repeatedly at rate of 134.3 bits per second. The code is 
*  based on the Golay (23,12) code first published in 1949. 
*  This code has the ability to detect and correct any three 
*  bit or less error that occurs in the 23 bit word. The word 
*  is composed of a 12 bit data field and an 11 bit parity vector. 
*  In CDCSS usage the 12 bits are divided into 4 octal digits, the 
*  first always being set to 100 (octal), the 2nd, 3rd and 4th digits 
*  form the three octal digit DCS code number. The word format is:
*
*  Shift Direction --> Out
*  PPPPPPPPPPPFFFCCCCCCCCC
*
*  Where P's are parity bits, F's are the fixed octal digit bits, 
*  and C's are the octal digit bits. 
*
*  [Source](http://mmi-comm.tripod.com/dcs.html)
**/

/*
make ../ && make && python/qa_dcs_sink_test.sh
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "dcs_sink_impl.h"

#define DCS_CACHE_SIZE 12
namespace gr {

  namespace dcsinfo {

    uint32_t m_buff;
    uint32_t m_buff_neg;

    int current_DCS [DCS_CACHE_SIZE] = {0};
    int current_DCS_num = 0;
    dcs_sink::sptr
    dcs_sink::make()
    {
      return gnuradio::get_initial_sptr
        (new dcs_sink_impl());
    }

    /*
     * The private constructor
     */
    dcs_sink_impl::dcs_sink_impl()
      : gr::sync_block("dcs_sink",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 0, 0))
    {
      
      /*
      //Some testcode
      m_buff = 0x13; // 000010011 (023), should have golay code 11101100011
      golay();
      */
      m_buff = 0;
    }

    /*
     * Our virtual destructor.
     */
    dcs_sink_impl::~dcs_sink_impl()
    {
    }


    /**
    * This method is a more verbose function to calculate
    * Golay code and do printouts. This method is mostly for 
    * debugging, not optimized for speed. 
    **/
    int dcs_sink_impl::golay(uint32_t buff)
    {
      uint16_t parity = 0;
      bool c1,c2,c3,c4,c5,c6,c7,c8,c9;
      bool p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11;
      bool r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11;
      bool f1,f2,f3;

      c1 = buff & 1;
      c2 = (buff >> 1) & 1;
      c3 = (buff >> 2) & 1;
      c4 = (buff >> 3) & 1;
      c5 = (buff >> 4) & 1;
      c6 = (buff >> 5) & 1;
      c7 = (buff >> 6) & 1;
      c8 = (buff >> 7) & 1;
      c9 = (buff >> 8) & 1;
        

      f1 = (buff >> 9) & 1;
      f2 = (buff >> 10) & 1;
      f3 = (buff >> 11) & 1;

      if (!(f1 == 0 && f2 == 0 && f3 == 1)) {
        return 1;
      }
      printf("c9c8c7..c1: %d%d%d%d%d%d%d%d%d\n" , c9,c8,c7,c6,c5,c4,c3,c2,c1);
      printf("f3f2f1: %d%d%d\n" , f3,f2,f1);

      p1 = c1 ^ c2 ^ c3 ^ c4 ^ c5 ^ c8 ;
      p2 = !( c2 ^ c3 ^ c4 ^ c5 ^ c6 ^ c9 );
      p3 = c1 ^ c2 ^ c6 ^ c7 ^ c8 ;
      p4 =  !( c2 ^ c3 ^ c7 ^ c8 ^ c9 );
      p5 =  !( c1 ^ c2 ^ c5 ^ c9 );
      p6 =  !( c1 ^ c4 ^ c5 ^ c6 ^ c8) ;
      p7 = c1 ^ c3 ^ c4 ^ c6 ^ c7 ^ c8 ^ c9;
      p8 = c2 ^ c4 ^ c5 ^ c7 ^ c8 ^ c9;
      p9 = c3 ^ c5 ^ c6 ^ c8 ^ c9;
      p10 = !(c4 ^ c6 ^ c7 ^ c9 );
      p11 = !(c1 ^ c2 ^ c3 ^ c4 ^ c7 );


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
      printf("GOL (recv):  %d%d%d%d%d%d%d%d%d%d%d\n\n" ,r11,r10,r9,r8,r7,r6,r5,r4,r3,r2,r1);


      return 0;
    }
    /**
    * invalidGolayCode
    * Checks if the received parity (golay code) is valid. 
    * 
    * http://mmi-comm.tripod.com/dcs.html:
    * 
    * The original Golay algorithm used a binary polynomial to generate the 
    * parity bits, but this method uses too much microcontroller time and memory.
    * Keeping all 512 23 bit words in rom would take a minimum of 1472 bytes, 
    * keeping the parity bits only in rom would take a minimum of 704 bytes. 
    * It is possible to save even more rom without taking a lot of microcontroller 
    * time by using a simpler way to calculate the needed parity bits. 
    * This method uses short calculations, one for each parity bit, based on 
    * selected bits from the 9 bit DCS code number:
    * 
    *   P1 = C1 + C2 + C3 + C4 + C5 + C8 (MODULO TWO ADDITION)
    *   P2 = NOT ( C2 + C3 + C4 + C5 + C6 + C9 )
    *   P3 = C1 + C2 + C6 + C7 + C8
    *   P4 = NOT ( C2 + C3 + C7 + C8 + C9 )
    *   P5 = NOT ( C1 + C2 + C5 + C9 )
    *   P6 = NOT ( C1 + C4 + C5 + C6 + C8 )
    *   P7 = C1 + C3 + C4 + C6 + C7 + C8 + C9
    *   P8 = C2 + C4 + C5 + C7 + C8 + C9
    *   P9 = C3 + C5 + C6 + C8 + C9
    *   P10 = NOT ( C4 + C6 + C7 + C9 )
    *   P11 = NOT ( C1 + C2 + C3 + C4 + C7 ) 
    * 
    * 
    * @return 0 if code is valid. If code is invalid, returnvalue
    * of 100 means the magic 0-0-1 before parity bits was not present. 
    * Return values of 1-11 points to the (first) erroneous parity bit found
    **/
    int dcs_sink_impl::invalidGolayCode(uint32_t buff)
    {

      /**
      * Check if the 'magic' 0-0-1 is present
      **/ 
 
      if(!(
        !(buff & 0x200) && // F1=0
        !(buff & 0x400) && // F2=0
        (buff & 0x800))) { // F3=1
        return 100;
      } 

      bool c1,c2,c3,c4,c5,c6,c7,c8,c9;
      
      c1 = buff & 1;
      c2 = (buff >> 1) & 1;
      c3 = (buff >> 2) & 1;
      c4 = (buff >> 3) & 1;
      c5 = (buff >> 4) & 1;
      c8 = (buff >> 7) & 1;


      if( ((buff >> 12) & 1) != c1 ^ c2 ^ c3 ^ c4 ^ c5 ^ c8 ) return 1;

      c6 = (buff >> 5) & 1;
      c9 = (buff >> 8) & 1;

      if( ((buff >> 13) & 1) != !( c2 ^ c3 ^ c4 ^ c5 ^ c6 ^ c9 )) return 2;

      c7 = (buff >> 6) & 1;

      if( ((buff >> 14) & 1) != c1 ^ c2 ^ c6 ^ c7 ^ c8)  return 3;
      if( ((buff >> 15) & 1) !=  !( c2 ^ c3 ^ c7 ^ c8 ^ c9 ) ) return 4;
      if( ((buff >> 16) & 1) !=  !( c1 ^ c2 ^ c5 ^ c9 ) ) return 5;
      if( ((buff >> 17) & 1) !=  !( c1 ^ c4 ^ c5 ^ c6 ^ c8))  return 6;
      if( ((buff >> 18) & 1) != c1 ^ c3 ^ c4 ^ c6 ^ c7 ^ c8 ^ c9) return 7;
      if( ((buff >> 19) & 1) != c2 ^ c4 ^ c5 ^ c7 ^ c8 ^ c9 ) return 8;
      if( ((buff >> 20) & 1) != c3 ^ c5 ^ c6 ^ c8 ^ c9) return 9;
      if( ((buff >> 21) & 1) != !(c4 ^ c6 ^ c7 ^ c9 )) return 10;
      if( ((buff >> 22) & 1) != !(c1 ^ c2 ^ c3 ^ c4 ^ c7 )) return 11;

     return 0;
    }

    int
    dcs_sink_impl::work(int noutput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items)
    {
        const float *in = (const float *) input_items[0];
        float *out = (float *) output_items[0];


        int r,r2;

        for (int i = 0 ; i < noutput_items ; i++)
        {
          
          //Shift buffer contents right
          m_buff >>= 1;
          
          // Insert latest sample from left
          // value > 0 ==> 1, 
          // value <= 0 ==> 0
          m_buff |= (in[i]  > 0) << 22;
          m_buff_neg = ~m_buff;
          //golay(m_buff);

          r = invalidGolayCode(m_buff);
          r2 = invalidGolayCode(m_buff_neg);
          

          if(r && r2){
            /*
            No valid golay code
            Check if end-transmission is happening
            */
            if ((m_buff & 0x7FFFFF) == 0x555555)// 101 0101 0101 0101 0101 0101
            {
              /*
              We want the buffer to be filled a few more times before
              signalling end of squelch (EOS)

              The EOS has a duration of about 180 ms, 
              which means that during 180ms, the pattern 01010... is sent. 
              When we detect 101...01 (23 bits), we set eos_counter to 1. 
              
              23 bits are shifted in in about 171 ms, so we have only ~25 bits of 101010
              before the tail burst is finished. So, if pattern is correct, we fire 
              DCS:END
    
              We don't want to send for each new bit arriving though, so 
              clear the buffer here
              **/
              m_buff = 0;
              //Show DCS
              printf("DCS: END\n");            
              memset(current_DCS,0,DCS_CACHE_SIZE);
              current_DCS_num = 0;
              //Signal EOS
              /*
              gr::block::add_item_tag(
                    0, //which output
                    i + gr::block::nitems_written(0), //uint64_t offset
                    pmt::string_to_symbol("dcs"),// pmt::pmt_t key
                    pmt::string_to_symbol("stop")// pmt::pmt_t value
              );
              */
            }
            continue;
          }
          //At this point, we have a valid DCS           
          char value[20]= {0};
          int dcs = m_buff & 0x7F; // 0b 111 111 111 == 0b 0111 1111 == 0x7F
          int alreadyShown = 0;
          //Do we display it or not ?
          for(int i = 0 ; !alreadyShown && i < DCS_CACHE_SIZE ; i++)
          {
            if(current_DCS[i] == dcs)//Already displayed
            {
              //printf(".");
              alreadyShown = 1;
            }
          }
          if(alreadyShown) continue; 

          current_DCS[current_DCS_num++] = dcs;
          current_DCS_num %= DCS_CACHE_SIZE;

          if(!r)
          {
            snprintf(value, sizeof(value),"+%d%d%d",(m_buff >> 6) & 0x7, (m_buff >> 3) & 0x7,m_buff & 0x7);
          }else if(!r2) //Can't be both at the same time
          {
            snprintf(value, sizeof(value),"-%d%d%d",(m_buff >> 6) & 0x7, (m_buff >> 3) & 0x7,m_buff & 0x7);
          }

          //Show DCS
          printf("DCS: %s\n" , value);            
          //Create stream-tag
          /**
          gr::block::add_item_tag(
                0, //which output
                i + gr::block::nitems_written(0), //uint64_t offset
                pmt::string_to_symbol("dcs"),// pmt::pmt_t key
                pmt::string_to_symbol(value)// pmt::pmt_t value
          );
          **/

        }

        //consume_each(noutput_items);
        
        // Tell runtime system how many output items we produced.
        return noutput_items;
        //return 0;
    }


  } /* namespace dcsinfo */
} /* namespace gr */

