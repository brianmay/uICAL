/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/util.h"
#include "uICAL/datetime.h"
#include "uICAL/rrule.h"
#include "uICAL/vevent.h"
#include "uICAL/vline.h"
#include "uICAL/vobject.h"
#include "uICAL/error.h"
#include "uICAL/tz.h"
#include "uICAL/date.h"
#include "uICAL/time.h"

namespace uICAL {
    static DateTime parse_datetime(const VLine &vline, const TZMap_ptr& tzmap, bool &has_time) {
        string value = vline.getParam("VALUE");
        if (value == "" || value == "DATE-TIME") {
            has_time = true;
            return DateTime(vline.value + vline.getParam("TZID"), tzmap);
        } else if (value == "DATE") {
            has_time = false;
            Date date(vline.value);
            Time time(0, 0, 0);
            TZ tz("Z");
            return DateTime(date, time, std::make_shared<TZ>(tz));
        } else {
            throw ValueError(string("Bad VALUE: \"") + value + "\"");
        }
    }

    VEvent::VEvent(const VObject_ptr& obj, const TZMap_ptr& tzmap) {

        VLine_ptr dtStart = obj->getPropertyByName("DTSTART");
        VLine_ptr dtEnd = obj->getPropertyByName("DTEND");
        VLine_ptr rRule = obj->getPropertyByName("RRULE");
        VLine_ptr summary = obj->getPropertyByName("SUMMARY");
        VLine_ptr location = obj->getPropertyByName("LOCATION");

        this->start = parse_datetime(*dtStart, tzmap, this->start_has_time);
        this->end = parse_datetime(*dtEnd, tzmap, this->end_has_time);
        this->summary = summary->value;

        if (location != nullptr) {
            this->location = location->value;
        } else {
            this->location = String("");
        }

        if (rRule != nullptr) {
            this->rrule = new_ptr<RRule>(rRule->value, this->start);
        } else {
            this->rrule = new_ptr<RRule>(String(""), this->start);
        }
    }

    void VEvent::str(ostream& out) const {
        out << "VEVENT: " << this->summary << uICAL::endl;
        out << " - start: " << this->start << uICAL::endl;
        out << " - end: " << this->end << uICAL::endl;
        out << " - rrule: " << this->rrule << uICAL::endl;
    }

}
