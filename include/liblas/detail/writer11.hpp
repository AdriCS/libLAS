// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#ifndef LIBLAS_DETAIL_WRITER11_HPP_INCLUDED
#define LIBLAS_DETAIL_WRITER11_HPP_INCLUDED

#include <liblas/detail/writer.hpp>
#include <liblas/detail/fwd.hpp>
#include <liblas/cstdint.hpp>
// std
#include <iosfwd>

namespace liblas { namespace detail { namespace v11 {

class WriterImpl : public Writer
{
public:

    typedef Writer Base;
    
    WriterImpl(std::ofstream& ofs);
    std::size_t GetVersion() const;
    void WriteHeader(LASHeader const& header);
    void UpdateHeader(LASHeader const& header);
    void WritePointRecord(PointRecord const& record);
    void WritePointRecord(PointRecord const& record, double const& time);

private:

    std::ofstream& m_ofs;
    liblas::uint32_t m_pointCount;
};

}}} // namespace liblas::detail::v11

#endif // LIBLAS_DETAIL_WRITER11_HPP_INCLUDED
