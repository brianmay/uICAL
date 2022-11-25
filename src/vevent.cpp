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
#include "uICAL/tzmap.h"

namespace uICAL
{
    static DateTime parse_datetime(const VLine &vline, const TZMap_ptr &tzmap, bool &has_time)
    {
        string value = vline.getParam("VALUE");
        if (value == "" || value == "DATE-TIME")
        {
            has_time = true;
            string tzid = vline.getParam("TZID");
            if (tzid.empty())
            {
                return DateTime(vline.value, tzmap);
            }
            else
            {
                TZ_ptr tz = tzmap->getTZ(tzid);
                return DateTime(vline.value, tz);
            }
        }
        else if (value == "DATE")
        {
            has_time = false;
            Date date(vline.value);
            Time time(0, 0, 0);
            return DateTime(date, time, tz_UTC);
        }
        else
        {
            throw ValueError(string("Bad VALUE: \"") + value + "\"");
        }
    }

    VEvent::VEvent(const VObject_ptr &obj, const TZMap_ptr &tzmap)
    {

        VLine_ptr dtStart = obj->getPropertyByName("DTSTART");
        VLine_ptr dtEnd = obj->getPropertyByName("DTEND");
        VLine_ptr rRule = obj->getPropertyByName("RRULE");
        VLine_ptr summary = obj->getPropertyByName("SUMMARY");
        VLine_ptr location = obj->getPropertyByName("LOCATION");
        VLine_ptr uid = obj->getPropertyByName("UID");
        VLine_ptr recurrence = obj->getPropertyByName("RECURRENCE-ID");

        this->start = parse_datetime(*dtStart, tzmap, this->start_has_time);
        this->end = parse_datetime(*dtEnd, tzmap, this->end_has_time);
        this->summary = summary->value;

        if (location != nullptr)
        {
            this->location = location->value;
        }
        else
        {
            this->location = string("");
        }

        if (rRule != nullptr)
        {
            this->rrule = new_ptr<RRule>(rRule->value, this->start);
        else
        {
            this->rrule = new_ptr<RRule>(string(""), this->start);
        }

        if (uid != nullptr)
        {
            this->uid = uid->value;
        }

        if (recurrence != nullptr)
        {
            this->recurrence = parse_datetime(*recurrence, tzmap, this->recurrence_has_time);
        }

        auto exdates = obj->getPropertysByName("EXDATE");
        for (VLine_ptr &exdate : exdates)
        {
            bool has_time;
            DateTime dt = parse_datetime(*exdate, tzmap, has_time);
            VEvent::exdate_t result = std::make_tuple(dt, has_time);
            this->exdates.push_back(result);
        }
    }

    void VEvent::str(ostream &out) const
    {
        out << "VEVENT: " << this->summary << uICAL::endl;
        out << " - start: " << this->start << uICAL::endl;
        out << " - end: " << this->end << uICAL::endl;
        out << " - uid: " << this->uid << uICAL::endl;
        out << " - recurrence: " << this->recurrence << uICAL::endl;
        out << " - rrule: " << this->rrule << uICAL::endl;
        for (const VEvent::exdate_t &exdate : this->exdates)
        {
            out << " - exdate: " << std::get<0>(exdate) << " " << std::get<1>(exdate) << uICAL::endl;
        }
    }

}
