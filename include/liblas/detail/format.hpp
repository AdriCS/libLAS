/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS 1.0 writer implementation for C++ libLAS 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
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

#ifndef LIBLAS_DETAIL_FORMAT_HPP_INCLUDED
#define LIBLAS_DETAIL_FORMAT_HPP_INCLUDED

#include <liblas/detail/fwd.hpp>
#include <liblas/cstdint.hpp>
#include <liblas/lasheader.hpp>
// std
#include <iosfwd>

namespace liblas { namespace detail { 

class Format
{
public:

    Format(liblas::uint8_t major, liblas::uint8_t minor, liblas::LASHeader::PointSize& size);
    virtual ~Format();
    
    virtual liblas::LASHeader::PointSize GetByteSize() const = 0;
    virtual void SetByteSize(liblas::LASHeader::PointSize& size) const = 0;
    
    liblas::uint8_t GetVersionMajor() { return m_versionmajor; }
    void SetVersionMajor(liblas::uint8_t v) {m_versionmajor = v; }
    
    liblas::uint8_t GetVersionMinor() { return m_versionminor; }
    void SetVersionMinor(liblas::uint8_t v) {m_versionminor = v; }

    bool IsCompressed() { return m_compressed; } 
    void SetCompressed(bool v) {m_compressed = v;}

    virtual bool HasColor() const = 0;
    virtual bool HasTime() const = 0;
    
  
protected:
    
    liblas::LASHeader::PointSize m_size;
    uint8_t m_versionminor;
    uint8_t m_versionmajor;
    bool m_compressed;
    
private:

    // Blocked copying operations, declared but not defined.
    Format(Format const& other);
    Format& operator=(Format const& rhs);
    
    
};

class PointFormat : public Format
{
public:

    typedef Format Base;
    
    PointFormat(liblas::uint8_t major, liblas::uint8_t minor, liblas::LASHeader::PointSize& size);
    bool HasColor() { return m_hasColor; }
    bool HasTime() { return m_hasTime; }
    bool IsCompressed() { return false; }

private:

    bool m_hasColor;
    bool m_hasTime;
};

}} // namespace liblas::detail

#endif // LIBLAS_DETAIL_FORMAT_HPP_INCLUDED
