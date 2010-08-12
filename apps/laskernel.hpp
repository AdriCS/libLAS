/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  Processing Kernel
 * Author:   Howard Butler, hobu.inc at gmail.com
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

#ifndef LIBLAS_KERNEL_HPP_INCLUDED
#define LIBLAS_KERNEL_HPP_INCLUDED

#include <liblas/liblas.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <functional>

#include <boost/program_options.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
namespace po = boost::program_options;

#define SEPARATORS ",|"
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

bool IsDualRangeFilter(std::string parse_string) ;

liblas::FilterI* MakeReturnFilter(std::string return_string, liblas::FilterI::FilterType ftype) ;
liblas::FilterI* MakeClassFilter(std::string class_string, liblas::FilterI::FilterType ftype) ;
liblas::FilterI* MakeBoundsFilter(std::string bounds_string, liblas::FilterI::FilterType ftype) ;
liblas::FilterI* MakeIntensityFilter(std::string intensities, liblas::FilterI::FilterType ftype) ;
liblas::FilterI* MakeTimeFilter(std::string times, liblas::FilterI::FilterType ftype) ;
liblas::FilterI* MakeScanAngleFilter(std::string intensities, liblas::FilterI::FilterType ftype) ;
liblas::FilterI* MakeColorFilter(liblas::Color const& low, liblas::Color const& high, liblas::FilterI::FilterType ftype); 


po::options_description GetFilteringOptions();
po::options_description GetTransformationOptions();

std::vector<liblas::FilterI*> GetFilters(po::variables_map vm, bool verbose);
std::vector<liblas::TransformI*> GetTransforms(po::variables_map vm, bool verbose, liblas::Header& header);


#endif // LIBLAS_ITERATOR_HPP_INCLUDED
