#include "../catch.hpp"

#include <cstdio>
#include <iostream>
#include <map>

#include "uICAL.h"

#include "uICAL/datetime.h"

TEST_CASE("DateTime::str", "[uICAL][DateTime]") {
    uICAL::TZMap_ptr tzmap = uICAL::new_ptr<uICAL::TZMap>();
    REQUIRE_THROWS_WITH(uICAL::DateTime("20200605T06493", tzmap), "ValueError: Bad datetime: \"20200605T06493\"");
}
