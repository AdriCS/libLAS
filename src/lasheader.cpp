/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS header class 
 * Author:   Mateusz Loskot, mateusz@loskot.net
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
 * Copyright (c) 2008, Phil Vachon
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

#include <liblas/cstdint.hpp>
#include <liblas/detail/utility.hpp>

#include <liblas/lasheader.hpp>
#include <liblas/guid.hpp>
#include <liblas/lasspatialreference.hpp>
#include <liblas/lasschema.hpp>

//std
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring> // std::memset, std::memcpy, std::strncpy
#include <cassert>
#include <ctime>


namespace liblas
{

char const* const Header::FileSignature = "LASF";
char const* const Header::SystemIdentifier = "libLAS";
char const* const Header::SoftwareIdentifier = "libLAS 1.2";

Header::Header() : m_format(1,2,0, false, false)
{
    Init();
}

Header::Header(Header const& other) :
    m_sourceId(other.m_sourceId),
    m_reserved(other.m_reserved),
    m_projectId1(other.m_projectId1),
    m_projectId2(other.m_projectId2),
    m_projectId3(other.m_projectId3),
    m_versionMajor(other.m_versionMajor),
    m_versionMinor(other.m_versionMinor),
    m_createDOY(other.m_createDOY),
    m_createYear(other.m_createYear),
    m_headerSize(other.m_headerSize),
    m_dataOffset(other.m_dataOffset),
    m_recordsCount(other.m_recordsCount),
    m_dataFormatId(other.m_dataFormatId),
    m_dataRecordLen(other.m_dataRecordLen),
    m_pointRecordsCount(other.m_pointRecordsCount),
    m_scales(other.m_scales),
    m_offsets(other.m_offsets),
    m_extents(other.m_extents),
    m_srs(other.m_srs),
    m_format(other.m_format)
{
    void* p = 0;

    p = std::memcpy(m_signature, other.m_signature, eFileSignatureSize);
    assert(p == m_signature);
    p = std::memcpy(m_projectId4, other.m_projectId4, eProjectId4Size); 
    assert(p == m_projectId4);
    p = std::memcpy(m_systemId, other.m_systemId, eSystemIdSize);
    assert(p == m_systemId);
    p = std::memcpy(m_softwareId, other.m_softwareId, eSoftwareIdSize);
    assert(p == m_softwareId);
    std::vector<uint32_t>(other.m_pointRecordsByReturn).swap(m_pointRecordsByReturn);
    assert(ePointsByReturnSize >= m_pointRecordsByReturn.size());
    
    std::vector<VariableRecord>(other.m_vlrs).swap(m_vlrs);

}

Header& Header::operator=(Header const& rhs)
{
    if (&rhs != this)
    {
        void* p = 0;
        p = std::memcpy(m_signature, rhs.m_signature, eFileSignatureSize);
        assert(p == m_signature);
        m_sourceId = rhs.m_sourceId;
        m_reserved = rhs.m_reserved;
        m_projectId1 = rhs.m_projectId1;
        m_projectId2 = rhs.m_projectId2;
        m_projectId3 = rhs.m_projectId3;
        p = std::memcpy(m_projectId4, rhs.m_projectId4, eProjectId4Size); 
        assert(p == m_projectId4);
        m_versionMajor = rhs.m_versionMajor;
        m_versionMinor = rhs.m_versionMinor;
        p = std::memcpy(m_systemId, rhs.m_systemId, eSystemIdSize);
        assert(p == m_systemId);
        p = std::memcpy(m_softwareId, rhs.m_softwareId, eSoftwareIdSize);
        assert(p == m_softwareId);
        m_createDOY = rhs.m_createDOY;
        m_createYear = rhs.m_createYear;
        m_headerSize = rhs.m_headerSize;
        m_dataOffset = rhs.m_dataOffset;
        m_recordsCount = rhs.m_recordsCount;
//        m_recordsCount = 0;
        m_dataFormatId = rhs.m_dataFormatId;
        m_dataRecordLen = rhs.m_dataRecordLen;
        m_pointRecordsCount = rhs.m_pointRecordsCount;
        
        std::vector<uint32_t>(rhs.m_pointRecordsByReturn).swap(m_pointRecordsByReturn);
        assert(ePointsByReturnSize >= m_pointRecordsByReturn.size());

        std::vector<VariableRecord>(rhs.m_vlrs).swap(m_vlrs);
        m_scales = rhs.m_scales;
        m_offsets = rhs.m_offsets;
        m_extents = rhs.m_extents;
        m_srs = rhs.m_srs;
        m_format = rhs.m_format;

    }
    return *this;
}

bool Header::operator==(Header const& other) const
{
    if (&other == this) return true;
    
    if (m_signature != other.m_signature) return false;
    if (m_sourceId != other.m_sourceId) return false;
    if (m_reserved != other.m_reserved) return false;
    if (m_projectId1 != other.m_projectId1) return false;
    if (m_projectId2 != other.m_projectId2) return false;
    if (m_projectId3 != other.m_projectId3) return false;
    if (m_projectId4 != other.m_projectId4) return false;
    if (m_versionMajor != other.m_versionMajor) return false;
    if (m_versionMinor != other.m_versionMinor) return false;
    if (m_systemId != other.m_systemId) return false;
    if (m_softwareId != other.m_softwareId) return false;
    if (m_createDOY != other.m_createDOY) return false;
    if (m_createYear != other.m_createYear) return false;
    if (m_headerSize != other.m_headerSize) return false;
    if (m_dataOffset != other.m_dataOffset) return false;
    if (m_recordsCount != other.m_recordsCount) return false;
    if (m_dataFormatId != other.m_dataFormatId) return false;
    if (m_dataRecordLen != other.m_dataRecordLen) return false;
    if (m_pointRecordsCount != other.m_pointRecordsCount) return false;
    if (m_pointRecordsByReturn != other.m_pointRecordsByReturn) return false;
    if (m_scales != other.m_scales) return false;
    if (m_offsets != other.m_offsets) return false;
    if (m_extents != other.m_extents) return false;
    
    return true;
}


std::string Header::GetFileSignature() const
{
    return std::string(m_signature, eFileSignatureSize);
}

void Header::SetFileSignature(std::string const& v)
{
    if (0 != v.compare(0, eFileSignatureSize, FileSignature))
        throw std::invalid_argument("invalid file signature");

    std::strncpy(m_signature, v.c_str(), eFileSignatureSize);
}

uint16_t Header::GetFileSourceId() const
{
    return m_sourceId;
}

void Header::SetFileSourceId(uint16_t v)
{
    // TODO: Should we warn or throw about type overflow occuring when
    //       user passes 65535 + 1 = 0
    m_sourceId = v;
}

uint16_t Header::GetReserved() const
{
    return m_reserved;
}

void Header::SetReserved(uint16_t v)
{
    // TODO: Should we warn or throw about type overflow occuring when
    //       user passes 65535 + 1 = 0
    m_reserved = v;
}

liblas::guid Header::GetProjectId() const
{
    return liblas::guid(m_projectId1, m_projectId2, m_projectId3, m_projectId4);
}

void Header::SetProjectId(guid const& v)
{
    v.output_data(m_projectId1, m_projectId2, m_projectId3, m_projectId4);
}

uint8_t Header::GetVersionMajor() const
{
    return m_format.GetVersionMajor();
}

void Header::SetVersionMajor(uint8_t v)
{
    if (eVersionMajorMin > v || v > eVersionMajorMax)
        throw std::out_of_range("version major out of range");

    m_format.SetVersionMajor(v);
}

uint8_t Header::GetVersionMinor() const
{
    return m_format.GetVersionMinor();
}

void Header::SetVersionMinor(uint8_t v)
{
    if (v > eVersionMinorMax)
        throw std::out_of_range("version minor out of range");
    
    m_format.SetVersionMinor(v);


}

std::string Header::GetSystemId(bool pad /*= false*/) const
{
    // copy array of chars and trim zeros if smaller than 32 bytes
    std::string tmp(std::string(m_systemId, eSystemIdSize).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < eSystemIdSize)
    {
        tmp.resize(eSystemIdSize, 0);
        assert(tmp.size() == eSystemIdSize);
    }

    assert(tmp.size() <= eSystemIdSize);
    return tmp;
}

void Header::SetSystemId(std::string const& v)
{
    if (v.size() > eSystemIdSize)
        throw std::invalid_argument("system id too long");

    std::fill(m_systemId, m_systemId + eSystemIdSize, 0);
    std::strncpy(m_systemId, v.c_str(), eSystemIdSize);
}

std::string Header::GetSoftwareId(bool pad /*= false*/) const
{
    std::string tmp(std::string(m_softwareId, eSoftwareIdSize).c_str());

    // pad right side with spaces
    if (pad && tmp.size() < eSoftwareIdSize)
    {
        tmp.resize(eSoftwareIdSize, 0);
        assert(tmp.size() == eSoftwareIdSize);
    }

    assert(tmp.size() <= eSoftwareIdSize);
    return tmp;
}

void Header::SetSoftwareId(std::string const& v)
{
    if (v.size() > eSoftwareIdSize)
        throw std::invalid_argument("generating software id too long");
    
//    m_softwareId = v;
    std::fill(m_softwareId, m_softwareId + eSoftwareIdSize, 0);
    std::strncpy(m_softwareId, v.c_str(), eSoftwareIdSize);
}

uint16_t Header::GetCreationDOY() const
{
    return m_createDOY;
}

void Header::SetCreationDOY(uint16_t v)
{
    if (v > 366)
        throw std::out_of_range("day of year out of range");

    m_createDOY = v;
}

uint16_t Header::GetCreationYear() const
{
    return m_createYear;
}

void Header::SetCreationYear(uint16_t v)
{
    // mloskot: I've taken these values arbitrarily
    if (v > 9999)
        throw std::out_of_range("year out of range");

    m_createYear = v;
}

uint16_t Header::GetHeaderSize() const
{
    return m_headerSize;
}

void Header::SetHeaderSize(uint16_t v)
{

    m_headerSize = v;
}

uint32_t Header::GetDataOffset() const
{
    return m_dataOffset;
}

void Header::SetDataOffset(uint32_t v)
{
    // uint32_t const dataSignatureSize = 2;
    // uint16_t const hsize = GetHeaderSize();
    // 
    // if ( (m_versionMinor == 0 && v < hsize + dataSignatureSize) ||
    //      (m_versionMinor == 1 && v < hsize) ||
    //      (m_versionMinor == 2 && v < hsize) )
    // {
    //     throw std::out_of_range("data offset out of range");
    // }
    
    m_dataOffset = v;
    
}

uint32_t Header::GetRecordsCount() const
{
    return m_recordsCount;
}

void Header::SetRecordsCount(uint32_t v)
{
    m_recordsCount = v;
}

liblas::PointFormatName Header::GetDataFormatId() const
{
    if (ePointFormat0 == m_dataFormatId)
        return ePointFormat0;
    else if (ePointFormat1 == m_dataFormatId)
        return ePointFormat1;
    else if (ePointFormat2 == m_dataFormatId)
        return ePointFormat2;
    else
        return ePointFormat3;
}

void Header::SetDataFormatId(liblas::PointFormatName v)
{
    m_dataFormatId = static_cast<uint8_t>(v);

    UpdateSchema();
    
    if (ePointFormat0 == m_dataFormatId)
        SetDataRecordLength(ePointSize0);
    else if (ePointFormat1 == m_dataFormatId) 
        SetDataRecordLength(ePointSize1);
    else if (ePointFormat2 == m_dataFormatId)
        SetDataRecordLength(ePointSize2);
    else if (ePointFormat3 == m_dataFormatId)
        SetDataRecordLength(ePointSize3);
    else
        SetDataRecordLength(ePointSize3);


}

uint16_t Header::GetDataRecordLength() const
{
    return m_format.GetByteSize();
}

void Header::SetDataRecordLength( uint16_t v )
{
    m_format.SetByteSize(v);
}
uint32_t Header::GetPointRecordsCount() const
{
    return m_pointRecordsCount;
}

void Header::SetPointRecordsCount(uint32_t v)
{
    m_pointRecordsCount = v;
}

Header::RecordsByReturnArray const& Header::GetPointRecordsByReturnCount() const
{
    return m_pointRecordsByReturn;
}

void Header::SetPointRecordsByReturnCount(std::size_t index, uint32_t v)
{
    assert(m_pointRecordsByReturn.size() == Header::ePointsByReturnSize);

    uint32_t& t = m_pointRecordsByReturn.at(index);
    t = v;
}


double Header::GetScaleX() const
{
    return m_scales.x;
}

double Header::GetScaleY() const
{
    return m_scales.y;
}

double Header::GetScaleZ() const
{
    return m_scales.z;
}

void Header::SetScale(double x, double y, double z)
{
    double const minscale = 0.01;
    m_scales.x = (0 == x) ? minscale : x;
    m_scales.y = (0 == y) ? minscale : y;
    m_scales.z = (0 == z) ? minscale : z;
}

double Header::GetOffsetX() const
{
    return m_offsets.x;
}

double Header::GetOffsetY() const
{
    return m_offsets.y;
}

double Header::GetOffsetZ() const
{
    return m_offsets.z;
}

void Header::SetOffset(double x, double y, double z)
{
    m_offsets = PointOffsets(x, y, z);
}

double Header::GetMaxX() const
{
    return m_extents.max.x;
}

double Header::GetMinX() const
{
    return m_extents.min.x;
}

double Header::GetMaxY() const
{
    return m_extents.max.y;
}

double Header::GetMinY() const
{
    return m_extents.min.y;
}

double Header::GetMaxZ() const
{
    return m_extents.max.z;
}

double Header::GetMinZ() const
{
    return m_extents.min.z;
}

void Header::SetMax(double x, double y, double z)
{
    m_extents.max = detail::Point<double>(x, y, z);
}

void Header::SetMin(double x, double y, double z)
{
    m_extents.min = detail::Point<double>(x, y, z);
}

void Header::AddVLR(VariableRecord const& v) 
{
    m_vlrs.push_back(v);
    m_recordsCount += 1;
}

VariableRecord const& Header::GetVLR(uint32_t index) const 
{
    return m_vlrs[index];
}

const std::vector<VariableRecord>& Header::GetVLRs() const
{
    return m_vlrs;
}

void Header::DeleteVLR(uint32_t index) 
{    
    if (index >= m_vlrs.size())
        throw std::out_of_range("index is out of range");

    std::vector<VariableRecord>::iterator i = m_vlrs.begin() + index;

    m_vlrs.erase(i);
    m_recordsCount = static_cast<uint32_t>(m_vlrs.size());

}


void Header::Init()
{
    // Initialize public header block with default
    // values according to LAS 1.2

    m_versionMajor = 1;
    m_versionMinor = 2;
    m_dataFormatId = ePointFormat0;
    m_dataRecordLen = ePointSize0;
    

    
    std::time_t now;
    std::time(&now);
    std::tm* ptm = std::gmtime(&now);
    assert(0 != ptm);
    
    m_createDOY = static_cast<uint16_t>(ptm->tm_yday);
    m_createYear = static_cast<uint16_t>(ptm->tm_year + 1900);

    m_headerSize = eHeaderSize;

    m_sourceId = m_reserved = m_projectId2 = m_projectId3 = uint16_t();
    m_projectId1 = uint32_t();
    std::memset(m_projectId4, 0, sizeof(m_projectId4)); 

    m_dataOffset = eHeaderSize; // excluding 2 bytes of Point Data Start Signature
    m_recordsCount = 0;
    m_pointRecordsCount = 0;

    std::memset(m_signature, 0, eFileSignatureSize);
    std::strncpy(m_signature, FileSignature, eFileSignatureSize);
//    m_signature = Header::FileSignature;

    std::memset(m_systemId, 0, eSystemIdSize);
    std::strncpy(m_systemId, SystemIdentifier, eSystemIdSize);
//    m_systemId = Header::SystemIdentifier;

    std::memset(m_softwareId, 0, eSoftwareIdSize);
    std::strncpy(m_softwareId, SoftwareIdentifier, eSoftwareIdSize);
//    m_softwareId = Header::SoftwareIdentifier;

    m_pointRecordsByReturn.resize(ePointsByReturnSize);

    // Zero scale value is useless, so we need to use a small value.
    SetScale(0.01, 0.01, 0.01);

    
}

void Header::ClearGeoKeyVLRs()
{
    std::string const uid("LASF_Projection");

    std::vector<VariableRecord> vlrs = m_vlrs;
    std::vector<VariableRecord>::const_iterator i;
    std::vector<VariableRecord>::iterator j;

    for (i = m_vlrs.begin(); i != m_vlrs.end(); ++i)
    {
        VariableRecord record = *i;

        if (record.GetUserId(false) == uid)
        {
            uint16_t const id = record.GetRecordId();
            if (34735 == id)
            {
                // Geotiff SHORT key
                for(j = vlrs.begin(); j != vlrs.end(); ++j)
                {
                    if (*j == *i)
                    {
                        vlrs.erase(j);
                        break;
                    }
                }
            }
            else if (34736 == id)
            {
                // Geotiff DOUBLE key
                for (j = vlrs.begin(); j != vlrs.end(); ++j)
                {
                    if (*j == *i)
                    {
                        vlrs.erase(j);
                        break;
                    }
                }
            }        
            else if (34737 == id)
            {
                // Geotiff ASCII key
                for (j = vlrs.begin(); j != vlrs.end(); ++j)
                {
                    if (*j == *i)
                    {
                        vlrs.erase(j);
                        break;
                    }
                }
            }
        } // uid == user
    }

    // Copy our list of surviving VLRs back to our member variable
    // and update header information
    m_vlrs = vlrs;
    m_recordsCount = static_cast<uint32_t>(m_vlrs.size());
}


void Header::SetGeoreference() 
{    
    std::vector<VariableRecord> vlrs = m_srs.GetVLRs();

    // Wipe the GeoTIFF-related VLR records off of the Header
    ClearGeoKeyVLRs();

    std::vector<VariableRecord>::const_iterator i;

    for (i = vlrs.begin(); i != vlrs.end(); ++i) 
    {
        AddVLR(*i);
    }
}

SpatialReference Header::GetSRS() const
{
    return m_srs;
}

void Header::SetSRS(SpatialReference& srs)
{
    m_srs = srs;
}

Schema Header::GetSchema() const
{
    
    return m_format;
}

void Header::UpdateSchema()
{
    if (GetDataFormatId() == liblas::ePointFormat3) {
        m_format.Color(true);
        m_format.Time(true);
    } else if (GetDataFormatId() == liblas::ePointFormat2) {
        m_format.Color(true);
        m_format.Time(false);
    } else if (GetDataFormatId() == liblas::ePointFormat1) {
        m_format.Color(false);
        m_format.Time(true);
    }
}

void Header::SetSchema(const Schema& format)
{

    // // A user can use the set the header's version information and 
    // // format information and sizes by using a PointFormat instance
    // // in addition to setting all the settings individually by hand
    // SetVersionMinor(format.GetVersionMinor());
    // SetVersionMajor(format.GetVersionMajor());
    // 
    // // The DataRecordLength will be set to the max of either the format's 
    // // byte size or the pointformat's specified size according to whether 
    // // or not it has color or time (FIXME: or waveform packets once we get to 1.3 )
    // // The extra space that is available can be used to store LASPoint::GetExtraData.
    // // We trim the format size to uint16_t because that's what the header stores 
    if (format.HasColor() && format.HasTime()) {
        SetDataFormatId(liblas::ePointFormat3);
        SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize3),
                                        static_cast<uint16_t>(format.GetByteSize())));
    } else if (format.HasColor()  && !format.HasTime()) {
        SetDataFormatId(liblas::ePointFormat2);
        SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize2),
                                        static_cast<uint16_t>(format.GetByteSize())));
    } else if (!format.HasColor()  && format.HasTime()) {
        SetDataFormatId(liblas::ePointFormat1);
        SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize1),
                                        static_cast<uint16_t>(format.GetByteSize())));
    } else {
        SetDataFormatId(liblas::ePointFormat0);
        SetDataRecordLength(std::max(   static_cast<uint16_t>(ePointSize0),
                                        static_cast<uint16_t>(format.GetByteSize())));
    }

    m_format = format;

} 
    
    
} // namespace liblas
