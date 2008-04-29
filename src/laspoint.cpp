// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/laspoint.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/cstdint.hpp>
#include <liblas/detail/utility.hpp>
// std
#include <cstring>

namespace liblas {

LASPoint::LASPoint() :
    m_intensity(0), m_flags(0), m_class(0),
        m_angleRank(0), m_userData(0), m_gpsTime(0)
{
    std::memset(m_coords, 0, sizeof(m_coords));
}

LASPoint::LASPoint(LASPoint const& other) :
    m_intensity(other.m_intensity),
        m_flags(other.m_flags), m_class(other.m_class),
            m_angleRank(other.m_angleRank), m_userData(other.m_userData),
                m_gpsTime(other.m_gpsTime)
{
    std::memcpy(m_coords, other.m_coords, sizeof(m_coords));
}

LASPoint& LASPoint::operator=(LASPoint const& rhs)
{
    if (&rhs != this)
    {
        m_coords[0] = rhs.m_coords[0];
        m_coords[1] = rhs.m_coords[1];
        m_coords[2] = rhs.m_coords[2];
        m_intensity = rhs.m_intensity;
        m_flags = rhs.m_flags;
        m_class = rhs.m_class;
        m_angleRank = rhs.m_angleRank;
        m_userData = rhs.m_userData;
        m_gpsTime = rhs.m_gpsTime;
    }
    return *this;
}

void LASPoint::SetCoordinates(LASHeader const& header, double x, double y, double z)
{
    double const cx = x * header.GetScaleX() + header.GetOffsetX();
    double const cy = y * header.GetScaleY() + header.GetOffsetY();
    double const cz = z * header.GetScaleZ() + header.GetOffsetZ();

    SetCoordinates(cx, cy, cz);
}

void LASPoint::SetReturnNumber(uint16_t const& num)
{
    // Store value in bits 1,2,3
    uint8_t val = static_cast<uint8_t>(num);
    uint16_t const begin = 1;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetNumberOfReturns(uint16_t const& num)
{
    // Store value in bits 4,5,6
    uint8_t val = static_cast<uint8_t>(num);
    uint16_t const begin = 4;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetScanDirection(uint16_t const& dir)
{
    // Store value in bit 7th
    uint8_t val = static_cast<uint8_t>(dir);
    uint16_t const begin = 7;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetFlightLineEdge(uint16_t const& edge)
{
    // Store value in bit 8th
    uint8_t val = static_cast<uint8_t>(edge);
    uint16_t const begin = 8;
    uint8_t mask = uint8_t(~0);
    m_flags &= ~(mask << (begin - 1)); 
    m_flags |= ((val & mask) << (begin - 1));
}

void LASPoint::SetScanAngleRank(int8_t const& rank)
{
    m_angleRank = rank;
}

void LASPoint::SetUserData(uint8_t const& data)
{
    m_userData = data;
}

bool LASPoint::equal(LASPoint const& other) const
{
    double const epsilon = std::numeric_limits<double>::epsilon(); 

    double const dx = m_coords[0] - other.m_coords[0];
    double const dy = m_coords[1] - other.m_coords[1];
    double const dz = m_coords[2] - other.m_coords[2];

    // TODO: Should we compare other data members, besides the coordinates?

    if (((dx <= epsilon) && (dx >= -epsilon ))
        || ((dy <= epsilon) && (dy >= -epsilon ))
        || ((dz <= epsilon) && (dz >= -epsilon )))
    {
        return true;
    }

    return false;
}

bool LASPoint::Validate() const
{
    
    if (eScanAngleRankMin > this->GetScanAngleRank() || this->GetScanAngleRank() > eScanAngleRankMax)
        throw std::out_of_range("scan angle rank out of range");

    if (this->GetFlightLineEdge() > 0x01)
        throw std::out_of_range("edge of flight line out of range");

    if (this->GetScanDirection() > 0x01)
        throw std::out_of_range("scan direction flag out of range");

    if (this->GetNumberOfReturns() > 0x07)
        throw std::out_of_range("number of returns out of range");

    if (this->GetReturnNumber() > 0x07)
        throw std::out_of_range("return number out of range");

    if (this->GetTime() < 0.0)
        throw std::out_of_range("time value is < 0 ");
    
    if (this->GetClassification() > 31)
        throw std::out_of_range("classification value is > 31");


    return true;
}
} // namespace liblas
