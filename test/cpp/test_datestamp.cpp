#include "../catch.hpp"

#include <cstdio>
#include <iostream>
#include <map>

#include "uICAL.h"

uICAL::DateStamp ds(const std::string& value) {
    uICAL::TZMap_ptr tzmap = uICAL::new_ptr<uICAL::TZMap>();
    using namespace uICAL;
    DateTime dt(value, tzmap);
    return dt.datestamp();
}

void cmp_datestamp(const std::string test, uICAL::DateStamp res, uICAL::DateStamp exp) {
    using namespace uICAL;

    REQUIRE(res == exp);
    if (res != exp) {
        std::cout << "FAIL: " << test << std::endl;
        std::cout << " res: " << res.as_str() << std::endl;
        std::cout << " exp: " << exp.as_str() << std::endl;
    }
}

TEST_CASE("DateStamp::test1", "[uICAL][DateStamp]") {
    using namespace uICAL;

    DateStamp dt1, exp;

    // Leap year - Feburary rollover
    dt1 = ds("19840228T090000");
    dt1.incDay(1);
    cmp_datestamp("Leap year - Feburary rollover 1", dt1, ds("19840229T090000"));
    dt1.incDay(1);
    cmp_datestamp("Leap year - Feburary rollover 2", dt1, ds("19840301T090000"));

    // Non leap year - Feburary rollover
    dt1 = ds("19850228T090000");
    dt1.incDay(1);
    cmp_datestamp("Non leap year - Feburary rollover", dt1, ds("19850301T090000"));

    // September roll over
    dt1 = ds("19970929T090000");
    dt1.incDay(1);
    cmp_datestamp("September roll over 1", dt1, ds("19970930T090000"));
    dt1.incDay(1);
    cmp_datestamp("September roll over 2", dt1, ds("19971001T090000"));
}

TEST_CASE("DateStamp::weekNo", "[uICAL][DateStamp]") {
    using namespace uICAL;

    auto t = [](const std::string datetime, DateTime::Day dayOfWeek, unsigned weekNo) {
        DateStamp dt = ds(datetime);
        DateTime::Day d = dt.dayOfWeek();
        unsigned w = dt.weekNo();

        REQUIRE(d == dayOfWeek);
        REQUIRE(w == weekNo);

    };

    // Starts on Mon
    t("19960101T090000", DateTime::Day::MON, 1);
    t("19960107T090000", DateTime::Day::SUN, 1);
    t("19960108T090000", DateTime::Day::MON, 2);

    // Starts on Tues
    t("19910101T090000", DateTime::Day::TUE, 1);
    t("19910106T090000", DateTime::Day::SUN, 1);
    t("19910107T090000", DateTime::Day::MON, 2);

    // Starts on Weds
    t("19970101T090000", DateTime::Day::WED, 1);
    t("19970105T090000", DateTime::Day::SUN, 1);
    t("19970106T090000", DateTime::Day::MON, 2);

    // Starts on Thurs
    t("19980101T090000", DateTime::Day::THU, 1);
    t("19980104T090000", DateTime::Day::SUN, 1);
    t("19980105T090000", DateTime::Day::MON, 2);
    t("19980111T090000", DateTime::Day::SUN, 2);
    t("19980112T090000", DateTime::Day::MON, 3);

    t("19980118T090000", DateTime::Day::SUN, 3);
    t("19980119T090000", DateTime::Day::MON, 4);
    t("19980125T090000", DateTime::Day::SUN, 4);
    t("19980126T090000", DateTime::Day::MON, 5);
    t("19980201T090000", DateTime::Day::SUN, 5);
    t("19980202T090000", DateTime::Day::MON, 6);
    t("19980208T090000", DateTime::Day::SUN, 6);
    t("19980209T090000", DateTime::Day::MON, 7);
    t("19980215T090000", DateTime::Day::SUN, 7);
    t("19980216T090000", DateTime::Day::MON, 8);
    t("19980222T090000", DateTime::Day::SUN, 8);
    t("19980223T090000", DateTime::Day::MON, 9);

    // Starts on Fri
    t("19990101T090000", DateTime::Day::FRI, 53);
    t("19990103T090000", DateTime::Day::SUN, 53);
    t("19990104T090000", DateTime::Day::MON, 1);
    t("19990110T090000", DateTime::Day::SUN, 1);
    t("19990111T090000", DateTime::Day::MON, 2);

    // Starts on Sat
    t("20000101T090000", DateTime::Day::SAT, 52);
    t("20000102T090000", DateTime::Day::SUN, 52);
    t("20000103T090000", DateTime::Day::MON, 1);
    t("20000109T090000", DateTime::Day::SUN, 1);
    t("20000110T090000", DateTime::Day::MON, 2);

    // Starts on Sun
    t("20060101T090000", DateTime::Day::SUN, 52);
    t("20060102T090000", DateTime::Day::MON, 1);
    t("20060108T090000", DateTime::Day::SUN, 1);
    t("20060109T090000", DateTime::Day::MON, 2);
}

static const char* datetime1 = "20191016T102000Z";
static const char* datetime2 = "20200605T064937Z";

TEST_CASE("DateStamp::str", "[uICAL][DateStamp]") {
    uICAL::TZMap_ptr tzmap = uICAL::new_ptr<uICAL::TZMap>();
    uICAL::DateTime dt1(datetime1, tzmap);
    uICAL::DateTime dt2(datetime2, tzmap);

    REQUIRE(dt1.as_str() == datetime1);

    auto diff = dt2 - dt1;
    REQUIRE(diff.as_str() == "P232DT20H29M37S");

    auto ds = dt2.datestamp();

    REQUIRE(ds.as_str() == "20200605T064937");
}

TEST_CASE("DateStamp::throw", "[uICAL][DateStamp]") {
    REQUIRE_THROWS_WITH(uICAL::DateStamp("20200605T064937-"), "ValueError: Bad datestamp: \"20200605T064937-\"");

    REQUIRE_THROWS_WITH(uICAL::DateStamp("YYYY0605T064937"), "ValueError: Invalid year: 0");
    REQUIRE_THROWS_WITH(uICAL::DateStamp("2020MM05T064937"), "ValueError: Invalid month: 0");
    REQUIRE_THROWS_WITH(uICAL::DateStamp("202006DDT064937"), "ValueError: Invalid day: 0");

    REQUIRE_THROWS_WITH(uICAL::DateStamp("20200605x064937"), "ValueError: Bad datestamp: \"20200605x064937\"");

    REQUIRE_THROWS_WITH(uICAL::DateStamp("20200605T244937"), "ValueError: Invalid hour: 24");
    REQUIRE_THROWS_WITH(uICAL::DateStamp("20200605T066037"), "ValueError: Invalid minute: 60");
    REQUIRE_THROWS_WITH(uICAL::DateStamp("20200605T064975"), "ValueError: Invalid second: 75");
}
