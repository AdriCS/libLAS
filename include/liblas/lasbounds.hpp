/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS bounds class 
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

#ifndef LIBLAS_LASBOUNDS_HPP_INCLUDED
#define LIBLAS_LASBOUNDS_HPP_INCLUDED

#include <liblas/detail/fwd.hpp>

#include <vector>

namespace liblas {

class Bounds
{
public:

    typedef std::vector<double> Vector;
    
    Bounds();
    Bounds(double minx, double miny, double maxx, double maxy, double minz, double maxz);
    Bounds(double minx, double miny, double maxx, double maxy);
    Bounds(const Point& min, const Point& max);
    Bounds( std::vector<double> const& low, std::vector<double> const& high);
    Bounds(Bounds const& other);
    Bounds& operator=(Bounds const& rhs);
    
    double min(std::size_t const& index) const;
    void min(std::size_t const& index, double v);
    double max(std::size_t const& index) const;
    void max(std::size_t const& index, double v);
    
    bool equal(Bounds const& other) const;
    bool intersects(Bounds const& other) const;
    uint32_t dimension() const;
    void dimension(uint32_t d);
        
private:
    Vector mins;
    Vector maxs;
    
    void verify();
};

inline bool operator==(Bounds const& lhs, Bounds const& rhs)
{
    return lhs.equal(rhs);
}

inline bool operator!=(Bounds const& lhs, Bounds const& rhs)
{
    return (!lhs.equal(rhs));
}

} // namespace liblas

#endif // ndef LIBLAS_LASBOUNDS_HPP_INCLUDED
