#include "basic_fixture.h"

#include <catch.hpp>

#include <nng/nng.h>

namespace nng {

    basic_fixture::~basic_fixture() {
        CHECK_NOTHROW(::nng_fini());
    }
}
