/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS indexing data stream class 
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2009, Howard Butler
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

#ifndef LIBLAS_INDEX_DATASTREAM_HPP_INCLUDED
#define LIBLAS_INDEX_DATASTREAM_HPP_INCLUDED

#include <liblas/lasvariablerecord.hpp>
#include <liblas/laspoint.hpp>


#ifndef _MSC_VER
#include <spatialindex/SpatialIndex.h>
#else
#include <SpatialIndex.h>
#endif

//std
#include <string>
#include <vector>
#include <stack>
#include <sys/stat.h>

namespace liblas {



class LASIndexDataStream : public SpatialIndex::IDataStream
{
public:
    LASIndexDataStream(LASReader* reader, long dimension);
    ~LASIndexDataStream();

    SpatialIndex::IData* getNext();
    bool hasNext() throw (Tools::NotSupportedException);

    uint32_t size() throw (Tools::NotSupportedException);
    void rewind() throw (Tools::NotSupportedException);

    /// Sets the index dimension
    /// \param v - dimension value.  Defaults to 3.
    void SetDimension(uint32_t v) { m_idxDimension = v; }

    /// Get index dimension
    /// \return index dimension value.
    uint32_t GetDimension() { return m_idxDimension; }
    
protected:
    liblas::LASReader* m_reader;
    SpatialIndex::RTree::Data* m_pNext;
    SpatialIndex::id_type m_id;

    uint32_t m_idxDimension;
    
    bool readPoint();

};



}

#endif // LIBLAS_INDEX_DATASTREAM_HPP_INCLUDED
