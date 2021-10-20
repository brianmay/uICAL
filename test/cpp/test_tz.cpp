#include "../catch.hpp"

#include "uICAL.h"

#include <iostream>
#include <fstream>
#include <list>

TEST_CASE("TZ::test1", "[uICAL][TZ]") {
    std::ifstream input(std::string("test/data/ical2.txt"));
    uICAL::istream_stl ical(input);

    uICAL::TZMap_ptr tzmap = uICAL::new_ptr<uICAL::TZMap>();
    auto vcalendar = uICAL::Calendar::load(ical, tzmap);
    {
        auto ds = uICAL::DateStamp("20191105T175555");

        std::list<uICAL::TZ_ptr> tzList;

        tzList.push_back(uICAL::new_ptr<uICAL::OffsetTZ>("-1000", "-1000"));
        tzList.push_back(uICAL::new_ptr<uICAL::OffsetTZ>("+0000", "+0000"));
        tzList.push_back(uICAL::new_ptr<uICAL::OffsetTZ>("+1000", "+1000"));

        auto dt = uICAL::DateTime(ds, uICAL::tz_UTC);

        auto it = tzList.begin();
        auto next = [&]() {
            if (it == tzList.end()) {
                return uICAL::string("END");
            }
            uICAL::string ret = dt.datestamp(*it).as_str() + uICAL::string(" ") + (*it)->as_str();
            it++;
            return ret;
        };

        REQUIRE(next() == "20191105T075555 -1000");
        REQUIRE(next() == "20191105T175555 +0000");
        REQUIRE(next() == "20191106T035555 +1000");
        REQUIRE(next() == "END");
    }
}
