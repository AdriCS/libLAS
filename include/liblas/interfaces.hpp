/******************************************************************************
* $Id$
*
* Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
* Purpose:  LAS interface declarations  
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

#ifndef LIBLAS_INTERFACES_HPP_INCLUDED
#define LIBLAS_INTERFACES_HPP_INCLUDED

#include <liblas/lasversion.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lasvariablerecord.hpp>
#include <liblas/lasspatialreference.hpp>
#include <liblas/detail/fwd.hpp>

// std
#include <iosfwd>
#include <string>
#include <memory>
#include <vector>
#include <cstddef>

namespace liblas {

class ReaderI
{
public:

    virtual LASHeader const& ReadHeader() = 0;
    virtual LASPoint const& ReadNextPoint(const LASHeader& header) = 0;
    virtual LASPoint const& ReadPointAt(std::size_t n, const LASHeader& header) = 0;

    virtual void Reset(const LASHeader& header) = 0;
    virtual void SetInputSRS(const LASSpatialReference& srs) = 0;
    virtual void SetOutputSRS(const LASSpatialReference& srs, const LASHeader& header) = 0;

    virtual ~ReaderI() {};    
};

class WriterI
{
public:

    virtual LASHeader const& WriteHeader(const LASHeader& header) = 0;
    virtual void UpdateHeader(const LASHeader& header) = 0;
    virtual void WritePoint(const LASPoint& point, const LASHeader& header) = 0;

    virtual void SetInputSRS(const LASSpatialReference& srs) = 0;
    virtual void SetOutputSRS(const LASSpatialReference& srs, const LASHeader& header) = 0;

    virtual ~WriterI() {};    
    // virtual LASHeader const& ReadHeader() = 0;
    // virtual LASPoint const& ReadNextPoint(const LASHeader& header) = 0;
    // virtual LASPoint const& ReadPointAt(std::size_t n, const LASHeader& header) = 0;
    // 
    // virtual void Reset(const LASHeader& header) = 0;
    // virtual void SetInputSRS(const LASSpatialReference& srs) = 0;
    // virtual void SetOutputSRS(const LASSpatialReference& srs, const LASHeader& header) = 0;
    // 
    // virtual ~ReaderI() {};    
};
} // namespace liblas

#endif // ndef LIBLAS_INTERFACES_HPP_INCLUDED
