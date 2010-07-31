/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS Schema implementation for C++ libLAS 
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Howard Butler
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#ifndef LIBLAS_SCHEMA_HPP_INCLUDED
#define LIBLAS_SCHEMA_HPP_INCLUDED

#include <liblas/detail/fwd.hpp>
// boost
#include <boost/cstdint.hpp>
// std
#include <iosfwd>

namespace liblas {  

class Schema
{
public:

    Schema(boost::uint8_t major, boost::uint8_t minor, boost::uint16_t size);
    Schema(boost::uint8_t major, boost::uint8_t minor, boost::uint16_t size, bool bColor, bool bTime);

    Schema& operator=(Schema const& rhs);
    Schema(Schema const& other);
    
    ~Schema() {};

    /// Fetch byte size
    boost::uint16_t GetByteSize() const;

    /// Set value of the red image channel 
    void SetByteSize(boost::uint16_t const& value);

    /// Get the base size (only accounting for Time, Color, etc )
    boost::uint16_t GetBaseByteSize() const;
    
    boost::uint8_t GetVersionMajor() const; 
    void SetVersionMajor(boost::uint8_t const& value);
    
    boost::uint8_t GetVersionMinor() const;
    void SetVersionMinor(boost::uint8_t const& value);

    bool HasColor() const;
    void Color(bool const& bColor); // updatesize(); }
    bool HasTime() const; 
    void Time(bool const& bTime); // {m_hasTime = bTime; updatesize(); }
  
protected:
    
    boost::uint16_t m_size;
    boost::uint8_t m_versionminor;
    boost::uint8_t m_versionmajor;

    bool m_hasColor;
    bool m_hasTime;
    
    boost::uint16_t m_base_size;

private:
    void updatesize();
    void updatesize(boost::uint16_t new_size);
    boost::uint16_t calculate_base_size();
};

} // namespace liblas

#endif // LIBLAS_SCHEMA_HPP_INCLUDED
