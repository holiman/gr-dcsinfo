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


#ifndef INCLUDED_DCSINFO_DCS_SOURCE_F_H
#define INCLUDED_DCSINFO_DCS_SOURCE_F_H

#include <dcsinfo/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace dcsinfo {

    /*!
     * \brief <+description of block+>
     * \ingroup dcsinfo
     *
     */
    class DCSINFO_API dcs_source_f : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<dcs_source_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of dcsinfo::dcs_source_f.
       *
       * To avoid accidental use of raw pointers, dcsinfo::dcs_source_f's
       * constructor is in a private implementation
       * class. dcsinfo::dcs_source_f::make is the public interface for
       * creating new instances.
       */
      static sptr make(int dcs_code);
    };

  } // namespace dcsinfo
} // namespace gr

#endif /* INCLUDED_DCSINFO_DCS_SOURCE_F_H */

