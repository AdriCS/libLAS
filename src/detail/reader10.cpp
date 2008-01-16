#include <liblas/lasheader.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/detail/reader.hpp>
// std
#include <fstream>
#include <cassert>
#include <cstdlib> // std::size_t
#include <stdexcept>

namespace liblas { namespace detail { namespace v10 {

ReaderImpl::ReaderImpl(std::ifstream& ifs) : Base(), m_ifs(ifs)
{
}

std::size_t ReaderImpl::GetVersion() const
{
    // TODO: use defined macro or review this calculation
    return (1 * 100000 + 0);
}

bool ReaderImpl::ReadHeader(LASHeader& header)
{
    header.Read(m_ifs);

    // TODO: which one is better:
    // if (4 != header.GetFileSignature().size())
    if (header.GetFileSignature().empty())
        return false;

    m_offset = header.GetDataOffset();
    m_size = header.GetPointRecordsCount();
    return true;
}

bool ReaderImpl::ReadPoint(LASPointRecord& point)
{
    if (0 == m_current)
        m_ifs.seekg(m_offset, std::ios::beg);

    if (m_current < m_size)
    {
        // TODO: static assert sizeof(LASPoint) == 20
        m_ifs.read(bytes_of(point), sizeof(LASPointRecord));
        ++m_current;

        return true;
    }

    return false;
}

}}} // namespace liblas::detail::v10
