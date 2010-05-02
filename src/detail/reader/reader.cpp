/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS 1.0 reader implementation for C++ libLAS 
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

#include <liblas/detail/reader/reader.hpp>

#include <liblas/detail/utility.hpp>
#include <liblas/liblas.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>

// std
#include <fstream>
#include <istream>
#include <iostream>
#include <stdexcept>
#include <cstddef> // std::size_t
#include <cstdlib> // std::free
#include <cassert>

namespace liblas { namespace detail { 

ReaderImpl::ReaderImpl(std::istream& ifs) :
    m_ifs(ifs), m_size(0), m_current(0),
    m_transform(0), m_in_ref(0), m_out_ref(0), m_point_reader(0),     
    m_header_reader(new reader::Header(m_ifs))

{
}

ReaderImpl::~ReaderImpl()
{
#ifdef HAVE_GDAL
    if (m_transform)
    {
        OCTDestroyCoordinateTransformation(m_transform);
    }
    if (m_in_ref)
    {
        OSRDestroySpatialReference(m_in_ref);
    }
    if (m_out_ref)
    {
        OSRDestroySpatialReference(m_out_ref);
    }

#endif

    delete m_point_reader;
    delete m_header_reader;
}

std::istream& ReaderImpl::GetStream() const
{
    return m_ifs;
}

void ReaderImpl::Reset(liblas::Header const& header)
{
    m_ifs.clear();
    m_ifs.seekg(0);

    // Reset sizes and set internal cursor to the beginning of file.
    m_current = 0;
    m_size = header.GetPointRecordsCount();
    
    // If we reset the reader, we're ready to start reading points, so 
    // we'll create a point reader at this point.
    if (m_point_reader == 0) {
        if (m_transform != 0) {
            m_point_reader = new reader::Point(m_ifs, header, m_transform);
        } else {
            m_point_reader = new reader::Point(m_ifs, header);
        }
    } 
}

void ReaderImpl::SetOutputSRS(const SpatialReference& srs, const liblas::Header& header)
{
    m_out_srs = srs;
    CreateTransform();
    
    // reset the point reader to include our new transform
    if (m_point_reader != 0) {
        delete m_point_reader;
        m_point_reader = 0;
        m_point_reader = new reader::Point(m_ifs, header, m_transform);
    }
}

void ReaderImpl::SetSRS(const SpatialReference& srs, const liblas::Header& header)
{
    SetOutputSRS(srs, header);
}

void ReaderImpl::SetInputSRS(const SpatialReference& srs)
{
    m_in_srs = srs;
}


void ReaderImpl::CreateTransform(){
#ifdef HAVE_GDAL
    
    if (m_transform)
    {
        OCTDestroyCoordinateTransformation(m_transform);
    }
    if (m_in_ref)
    {
        OSRDestroySpatialReference(m_in_ref);
    }
    if (m_out_ref)
    {
        OSRDestroySpatialReference(m_out_ref);
    }
    
    m_in_ref = OSRNewSpatialReference(0);
    m_out_ref = OSRNewSpatialReference(0);

    int result = OSRSetFromUserInput(m_in_ref, m_in_srs.GetWKT().c_str());
    if (result != OGRERR_NONE) 
    {
        std::ostringstream msg; 
        msg << "Could not import input spatial reference for Reader::" << CPLGetLastErrorMsg() << result;
        std::string message(msg.str());
        throw std::runtime_error(message);
    }
    
    result = OSRSetFromUserInput(m_out_ref, m_out_srs.GetWKT().c_str());
    if (result != OGRERR_NONE) 
    {
        std::ostringstream msg; 
        msg << "Could not import output spatial reference for Reader::" << CPLGetLastErrorMsg() << result;
        std::string message(msg.str());
        throw std::runtime_error(message);
    }

    m_transform = OCTNewCoordinateTransformation( m_in_ref, m_out_ref);
    
#endif
}


liblas::Header const& ReaderImpl::ReadHeader()
{
    m_header_reader->read();
    const liblas::Header& header = m_header_reader->GetHeader();
    
    Reset(header);
    
    // keep a copy on the reader in case we're going to reproject data 
    // on the way out.
    m_in_srs = header.GetSRS();
    
    return header;
}

liblas::Point const& ReaderImpl::ReadNextPoint(const liblas::Header& header)
{
    if (0 == m_current)
    {
        m_ifs.clear();
        m_ifs.seekg(header.GetDataOffset(), std::ios::beg);

    }

    if (m_current < m_size)
    {
        m_point_reader->read();
        const liblas::Point& point = m_point_reader->GetPoint();
        ++m_current;
        return point;
        // return true;
    } else if (m_current == m_size ){
        throw std::out_of_range("file has no more points to read, end of file reached");
    } else {
        throw std::runtime_error("ReadNextPoint: m_current > m_size, something has gone extremely awry");
    }

}

liblas::Point const& ReaderImpl::ReadPointAt(std::size_t n, const liblas::Header& header)
{
    // FIXME: Throw in this case.
    if (m_size == n) {
        throw std::out_of_range("file has no more points to read, end of file reached");
    } else if (m_size < n) {
        std::ostringstream output;
        output << "ReadPointAt:: Inputted value: " << n << " is greater than the number of points: " << m_size;
        std::string out(output.str());
        throw std::runtime_error(out);
    } 

    std::streamsize pos = (static_cast<std::streamsize>(n) * header.GetDataRecordLength()) + header.GetDataOffset();    

    m_ifs.clear();
    m_ifs.seekg(pos, std::ios::beg);

    m_point_reader->read();
    const liblas::Point& point = m_point_reader->GetPoint();
    
    return point;
}

CachedReaderImpl::CachedReaderImpl(std::istream& ifs , liblas::uint64_t size) :
    ReaderImpl(ifs), m_cache_size(size), m_cache_start_position(0), m_cache_read_position(0)
{
}


liblas::Header const& CachedReaderImpl::ReadHeader()
{
    const liblas::Header& header = ReaderImpl::ReadHeader();
    
    // If we were given no cache size, try to cache the whole thing
    if (m_cache_size == 0) {
        m_cache_size = header.GetPointRecordsCount();
    }

    if (m_cache_size > header.GetPointRecordsCount()) {
        m_cache_size = header.GetPointRecordsCount();
    }
    m_cache.resize(m_cache_size);
    
    // Mark all positions as uncached and build up the mask
    // to the size of the number of points in the file
    for (uint32_t i = 0; i < header.GetPointRecordsCount(); ++i) {
        m_mask.push_back(false);
    }

    
    return header;
}

liblas::Point const& CachedReaderImpl::GetPoint(liblas::uint32_t position, const liblas::Header& header) {
    
    int32_t cache_position = position - m_cache_start_position ;

    if (cache_position == -1 ) {
            // printf("we're not here!\n");
            cache_position = 0;
    }
        // printf("--- checking for cached point at %d with position %d@%d as the cached point\n", 
        // m_cache_read_position, 
        // cache_position, 
        // m_cache_start_position   );

        // std::cout << "MASK: ";
        // std::vector<bool>::iterator it;
        // for (it = m_mask.begin(); it != m_mask.end(); ++it) {
        //     std::cout << *it << ",";
        // }
        // std::cout << std::endl;
        
        // std::cout << "m_mask[cache_position]: " << m_mask[cache_position] << std::endl;
    if (m_mask[m_cache_read_position] == true) {
        // printf("---- have cached point at %d giving %d as the cached point @%d\n", m_cache_read_position, cache_position,m_cache_start_position);
        ++m_cache_read_position;
        
        // uint32_t position = m_cache_read_position - m_cache_start_position - 1;
        
        // If we're reading the very first point, we have already incremented 
        // the m_cache_read_position

        // printf("** returning cache position %d\n", cache_position);

        return m_cache[cache_position];
    } else {
        
        cache_position = 0; // starting the cache over
        int32_t old_cache_start_position = m_cache_start_position;

        uint32_t left_to_cache = std::min(m_cache_size, header.GetPointRecordsCount() - m_cache_start_position);
        // printf("number of points left to cache: %d\n", left_to_cache);
        // printf("cache position(s) old_cache_start_position: %d m_cache_start_position: %d cache_position: %d\n", 
        // old_cache_start_position,
        // m_cache_start_position , 
        // cache_position);
        // // 
        // std::cout << "MASK: ";
        // std::vector<bool>::iterator it;
        // for (it = m_mask.begin(); it != m_mask.end(); ++it) {
        //     std::cout << *it << ",";
        // }
        // std::cout << std::endl;
        
        // Mark old points as uncached
        
        // if (old_cache_start_position > 0 ) {
        uint32_t to_mark = std::max(m_cache_size, static_cast<liblas::uint64_t>(left_to_cache));
        for (uint32_t i = 0; i < to_mark; ++i) {
            // printf ("Marking %d position false\n", m_cache_start_position + i);
            m_mask[old_cache_start_position + i] = false;
        }
        // printf("number of points to mark: %d\n", to_mark);
     // }


        for (uint32_t i = 0; i < left_to_cache; ++i) {
            
            try {
                m_mask[m_current] = true;
                m_cache[i] = ReaderImpl::ReadNextPoint(header);
            } catch (std::out_of_range&) {
                // cached to the end
                break;
            }
            // printf("cached %d points at position %d with m_current: %d\n", i, m_cache_start_position, m_current);

        }
        
        // std::cout << "MASK: ";
        // for (it = m_mask.begin(); it != m_mask.end(); ++it) {
        //     std::cout << *it << ",";
        // }
        // std::cout << std::endl;

        m_cache_start_position =  m_cache_read_position;
        ++m_cache_read_position;

        // printf("new m_cache_start_position: %d m_cache_read_position: %d cache_position:%d \n", m_cache_start_position, m_cache_read_position, cache_position);
        

        // printf("*** returning cache position %d\n", cache_position);
        return m_cache[cache_position ];
    }
    
}

liblas::Point const& CachedReaderImpl::ReadNextPoint(const liblas::Header& header)
{
    // uint32_t pos = m_current;
    // std::cout   << "m_mask[m_cache_read_position]: " << m_mask[m_cache_read_position] 
    //             << " m_current: " << m_current
    //             << " m_size: " << m_size 
    //             << " m_cache_read_position: " << m_cache_read_position 
    //             << " m_cache_start_position: " << m_cache_start_position 
    // //             <<std::endl;
    if (m_cache_read_position == m_size ){
        throw std::out_of_range("file has no more points to read, end of file reached");
    }
    
     
    // ++m_cache_read_position;
    liblas::Point const& point = GetPoint(m_cache_read_position, header);
    if (point.GetX() == 0 || point.GetY() == 0.0 || point.GetReturnNumber() == 0) {
        printf("X is zero for position %d\n", m_cache_read_position);
    }
    return point;
    
    // if (m_mask[m_cache_read_position] == true) {
    //     // printf("have cached point at %d giving %d as the cached point\n", m_cache_read_position, m_cache_read_position - m_cache_start_position);
    //     ++m_cache_read_position;
    //     
    //     uint32_t position = m_cache_read_position - m_cache_start_position - 1;
    //     
    //     // // If we're reading the very first point, we have already incremented 
    //     // // the m_cache_read_position
    //     // if (m_cache_read_position == 0 && m_cache_start_position == 0) {
    //     //     position = position + 1;
    //     // }
    //     return m_cache[position];
    // } else {
    //     
    // 
    //     
    //     uint32_t left_to_cache = std::min(m_cache_size, header.GetPointRecordsCount() - m_cache_start_position);
    //     // printf("number of points left to cache: %d\n", left_to_cache);
    //     // printf("old cache position %d\n", m_cache_start_position);
    //     
    //     // std::cout << "MASK: ";
    //     // std::vector<bool>::iterator it;
    //     // for (it = m_mask.begin(); it != m_mask.end(); ++it) {
    //     //     std::cout << *it << ",";
    //     // }
    //     // std::cout << std::endl;
    //     
    //     // Mark old points as uncached
    //     uint32_t to_mark = std::max(m_cache_size, static_cast<liblas::uint64_t>(left_to_cache));
    //     for (uint32_t i = 0; i < to_mark; ++i) {
    //         // printf ("Marking %d position false\n", m_cache_start_position + i);
    //         m_mask[m_cache_start_position + i] = false;
    //     }
    //     // printf("number of points to mark: %d\n", to_mark);
    //     
    //     m_cache_start_position =  m_cache_read_position;
    // 
    //     for (uint32_t i = 0; i < left_to_cache; ++i) {
    //         
    //         try {
    //             m_mask[m_current] = true;
    //             m_cache[i] = ReaderImpl::ReadNextPoint(header);
    //         } catch (std::out_of_range&) {
    //             // cached to the end
    //             break;
    //         }
    //         // printf("cached %d points at position %d with m_current: %d\n", i, m_cache_start_position, m_current);
    // 
    //     }
    //     // 
    //     // std::cout << "MASK: ";
    //     // for (it = m_mask.begin(); it != m_mask.end(); ++it) {
    //     //     std::cout << *it << ",";
    //     // }
    //     // std::cout << std::endl;
    //     // printf("new m_cache_start_position: %d m_cache_read_position: %d\n", m_cache_start_position, m_cache_read_position);
    //     ++m_cache_read_position;
    //     
    //     uint32_t position = m_cache_read_position - m_cache_start_position - 1;
    //     // if (m_cache_read_position == 0 && m_cache_start_position == 0) {
    //     //     position = position + 1;
    //     // }
    //     // printf("returning cache position %d\n", position);
    //     return m_cache[position];
    // }
    // return ReaderImpl::ReadNextPoint(header);
}

liblas::Point const& CachedReaderImpl::ReadPointAt(std::size_t n, const liblas::Header& header)
{
    
    return ReaderImpl::ReadPointAt(n, header);
}

// CachedReaderImpl::~CachedReaderImpl()
// {
//     ~ReaderImpl();
// }

ReaderImpl* ReaderFactory::Create(std::istream& ifs)
{
    if (!ifs)
    {
        throw std::runtime_error("input stream state is invalid");
    }

    // Determine version of given LAS file and
    // instantiate appropriate reader.
    // uint8_t verMajor = 0;
    // uint8_t verMinor = 0;
    // ifs.seekg(24, std::ios::beg);
    // detail::read_n(verMajor, ifs, 1);
    // detail::read_n(verMinor, ifs, 1);

    return new CachedReaderImpl(ifs, 3);
    
    // if (1 == verMajor && 0 == verMinor)
    // {
    // 
    //     return new ReaderImpl(ifs);
    // }
    // else if (1 == verMajor && 1 == verMinor)
    // {
    //     return new v11::ReaderImpl(ifs);
    // }
    // else if (1 == verMajor && 2 == verMinor)
    // {
    //     return new v12::ReaderImpl(ifs);
    // }
    // else if (2 == verMajor && 0 == verMinor )
    // {
    //     // TODO: LAS 2.0 read/write support
    //     throw std::runtime_error("LAS 2.0+ file detected but unsupported");
    // }

    // throw std::runtime_error("LAS file of unknown version");
}


void ReaderFactory::Destroy(ReaderImpl* p) 
{
    delete p;
    p = 0;
}
}} // namespace liblas::detail

