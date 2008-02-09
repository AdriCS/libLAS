// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// (C) Copyright Phil Vachon 2007, philippe@cowpig.ca
// (C) Copyright Howard Butler 2007
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#ifndef LIBLAS_LASERROR_HPP_INCLUDED
#define LIBLAS_LASERROR_HPP_INCLUDED

//std
#include <iosfwd>
#include <string>

namespace liblas {

/// \todo To be documented.
class LASError
{
public:

    LASError(int code, std::string const& message, std::string const& method);
    LASError(LASError const& other);
    LASError& operator=(LASError const& rhs);

    // TODO - mloskot: What about replacing string return by copy with const char* ?
    //        char const* GetMethod() const { return m_method.c_str(); }, etc.

    std::string GetMethod() const { return m_method; };
    std::string GetMessage() const { return m_message; };
    int GetCode() const {return m_code; };

private:
    int m_code;
    std::string m_message;
    std::string m_method;
};

} // namespace liblas

#endif
