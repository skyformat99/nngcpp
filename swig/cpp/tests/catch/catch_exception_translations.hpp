#ifndef NNGCPP_CATCH_EXCEPTION_TRANSLATIONS_HPP
#define NNGCPP_CATCH_EXCEPTION_TRANSLATIONS_HPP

#include <catch.hpp>

#include <core/nng_exception.h>

#ifndef STRINGIFY
#define STRINGIFY(x)    #x
#endif // STRINGIFY

/* Translating from std::exception parlance, we have:
http://www.cplusplus.com/reference/exception/exception/what/ */
CATCH_TRANSLATE_EXCEPTION(std::exception& ex) {
    const auto ex_what_ = std::string(ex.what());
    return ex_what_.empty()
        ? "unknown " STRINGIFY(std::exception)
        : ("std::exception: " + ex_what_).c_str();
}

CATCH_TRANSLATE_EXCEPTION(nng::nng_exception& ex) {
    // TODO: TBD: consider making this a part of the nng::nng_exception::what method...
    std::ostringstream os;
    const auto errnum = to_int(ex.error_code);
    os << STRINGIFY(nng::nng_exception) << ": " << "error_code = " << errnum;
    return os.str();
}

#endif // NNGCPP_CATCH_EXCEPTION_TRANSLATIONS_HPP