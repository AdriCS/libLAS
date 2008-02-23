// $Id$
//
// (C) Copyright Howard Butler 2008
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/detail/file.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/laswriter.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>

namespace liblas { namespace detail {

FileImpl::FileImpl(std::string const& filename)
    : m_mode(0), m_filename(filename),
        m_istrm(0), m_ostrm(0), m_reader(0), m_writer(0)
{
    if (filename == "stdin")
    {
        m_istrm = &std::cin;
    }
    else
    {
        std::ios::openmode const mode = std::ios::in | std::ios::binary;
        m_istrm = new std::ifstream(m_filename.c_str(), mode);
        
        if (!m_istrm->good())
        {
            delete m_istrm;
            throw_no_file_error();
        }
    }

    assert(0 != m_istrm);
    m_reader = new LASReader(*m_istrm);
    m_header = m_reader->GetHeader();

    assert(0 == m_ostrm);
}

FileImpl::FileImpl(std::string const& filename, LASHeader const& header, int mode)
    : m_mode(mode), m_filename(filename),
        m_istrm(0), m_ostrm(0), m_reader(0), m_writer(0), m_header(header)
{
    if (filename == "stdout")
    {
        m_ostrm = &std::cout;
    }
    else
    {
        std::ios::openmode m;
        if (mode == 2) // append mode
            m = std::ios::out | std::ios::in | std::ios::binary | std::ios::ate;
        else
            m = std::ios::out | std::ios::in | std::ios::binary | std::ios::ate;
        
        m_ostrm = new std::ofstream(m_filename.c_str(), m);

        if (!m_ostrm->good())
        {
            delete m_ostrm;
            throw_no_file_error();
        }
    }

    assert(0 != m_ostrm);
    m_writer = new LASWriter(*m_ostrm, m_header);

    assert(0 == m_istrm);
}

FileImpl::~FileImpl()
{
    // NOTE: The order of destruction is very important.
    // reader/writer first, then streams.
    // In other words, kill clients first, suppliers afterwards.

    if (m_istrm != &std::cin && 0 != m_istrm)
    {
        assert(0 == m_writer);
        assert(0 == m_ostrm);
        delete m_reader;
        m_reader = 0;
        delete m_istrm;
        m_istrm = 0;
    }

    if (m_ostrm != &std::cout && 0 != m_ostrm)
    {
        assert(0 == m_reader);
        assert(0 == m_istrm);
        delete m_writer;
        m_writer = 0;
        delete m_ostrm;
        m_ostrm = 0;
    }
}

std::string FileImpl::GetName() const
{
    return m_filename;
}

int FileImpl::GetMode() const
{
    return m_mode;
}

LASHeader const& FileImpl::GetHeader() const
{
    return m_header;
}

LASReader& FileImpl::GetReader()
{
    if (0 == m_reader)
    {
        // TODO: Define specialized exception type for this error
        std::string msg("Reader is file write-only: " + m_filename);
        throw std::runtime_error(msg);
    }

    return (*m_reader);
}

LASWriter& FileImpl::GetWriter()
{
    if (0 == m_writer)
    {
        // TODO: Define specialized exception type for this error
        std::string msg("Writer is file read-only: " + m_filename);
        throw std::runtime_error(msg);
    }

    return (*m_writer);
}

void FileImpl::throw_no_file_error() const
{
    // TODO: Define specialized exception type for this error
    // for example:  liblas::open_error or liblas::no_file_error
    std::string msg("can not open file " + m_filename);
    throw std::runtime_error(msg);
}

}} // namespace liblas::detail
