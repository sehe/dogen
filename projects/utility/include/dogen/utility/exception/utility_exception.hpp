/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_UTILITY_EXCEPTION_EXCEPTION_HPP
#define DOGEN_UTILITY_EXCEPTION_EXCEPTION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <boost/exception/info.hpp>

namespace dogen {
namespace utility {

/**
 * @brief
 */
namespace exception {

/**
 * @brief base class for all exceptions within dogen.
 *
 * Exceptions should have a descriptive error message which must be
 * passed in as a string literal; any additional (variable) exception
 * information must be passed in via tags. See "Transporting of
 * Arbitrary Data to the Catch Site" in the boost::exception
 * documentation.
 */
class exception : public virtual std::exception,
                  public virtual boost::exception {
public:
    // to use BOOST_THROW_EXCEPTION we need to provide a default constructor.
    exception() : message_(0) { }

    /**
     * @param message Error message. Must be a string literal.
     */
    exception(std::string message) : message_(message) { }
    virtual ~exception() throw() { }

public:
    const char* what() const throw() { return(message_.c_str()); }

private:
    const std::string message_;
};

} } }

#endif
