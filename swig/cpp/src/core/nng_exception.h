#ifndef NNGCPP_NNG_EXCEPTION_H
#define NNGCPP_NNG_EXCEPTION_H

#include "../nngcpp_integration.h"

// nng should be in the include path.
#include <exception>
#include <string>

namespace nng {

    class nng_exception : public std::exception {
        public:

            int error_code;

            std::string message;

        public:

            nng_exception();

            nng_exception(int error_code);

            virtual ~nng_exception();
    };
}

#define THROW_NNG_EXCEPTION() throw nng::nng_exception();
#define THROW_NNG_EXCEPTION_EC(ec) if (ec < 0) { throw nng:nng_exception(ec); }

#endif // NNGCPP_NNG_EXCEPTION_H