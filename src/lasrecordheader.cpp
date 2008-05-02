/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS record header class 
 * Author:   Phil Vachon, philippe@cowpig.ca
 *
 ******************************************************************************
 * Copyright (c) 2008, Phil Vachon
 * Copyright (c) 2008, Mateusz Loskot, mateusz@loskot.net
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

#include <liblas/lasrecordheader.hpp>
#include <liblas/cstdint.hpp>
// std
#include <string>

namespace liblas {

LASRecordHeader::LASRecordHeader() :
    m_reserved(0), m_recordId(0), m_recordLength(0)
{    
}

LASRecordHeader::LASRecordHeader(LASRecordHeader const& other) :
    m_reserved(other.m_reserved),
    m_recordId(other.m_recordId),
    m_recordLength(other.m_recordLength),
    m_userId(other.m_userId),
    m_desc(other.m_desc)
{
}

LASRecordHeader& LASRecordHeader::operator=(LASRecordHeader const& rhs)
{
    if (this != &rhs)
    {
        m_reserved = rhs.m_reserved;
        m_recordId = rhs.m_recordId;
        m_recordLength = rhs.m_recordLength;
        m_userId = rhs.m_userId;
        m_desc = rhs.m_desc;
    }
    return (*this);
}

uint16_t LASRecordHeader::GetReserved() const
{
    return m_reserved;
}

std::string const& LASRecordHeader::GetUserId() const
{
    return m_userId;
}

uint16_t LASRecordHeader::GetRecordId() const
{
    return m_recordId;
}

uint16_t LASRecordHeader::GeRecordLength() const
{
    return m_recordLength;
}

std::string const& LASRecordHeader::GetDescription() const
{
    return m_desc;
}

bool LASRecordHeader::equal(LASRecordHeader const& other) const
{
    return (m_recordId == other.m_recordId
            && m_userId == other.m_userId 
            && m_desc == other.m_desc
            && m_reserved == other.m_reserved
            && m_recordLength == other.m_recordLength);
}

} // namespace liblas
