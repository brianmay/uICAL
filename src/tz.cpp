/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/error.h"
#include "uICAL/tz.h"
#include "uICAL/datestamp.h"
#include "uICAL/epochtime.h"
#include "uICAL/tzmap.h"

namespace uICAL {
    const TZ_ptr tz_unaware = new_ptr<UnawareTZ>();
    const TZ_ptr tz_UTC = new_ptr<OffsetTZ>(0);

    bool TZ::is_aware() const {
        return true;
    }

    UnawareTZ::UnawareTZ() {
    }

    seconds_t UnawareTZ::toUTC(seconds_t timestamp) const {
        return timestamp;
    }
    seconds_t UnawareTZ::fromUTC(seconds_t timestamp) const {
        return timestamp;
    }

    void UnawareTZ::str(ostream& out) const {
        out << "unaware";
    }

    OffsetTZ::OffsetTZ(int offsetMins) {
        this->offsetMins = offsetMins;
    }

    OffsetTZ::OffsetTZ(const string& tz) {
        this->offsetMins = OffsetTZ::parseOffset(tz);
    }

    int OffsetTZ::parseOffset(const string& tz) {
        try {
            if (tz.length() == 5) {
                char sign;
                unsigned tzH, tzM;

                // e.g.: +0200
                sign = tz.at(0);
                tzH = tz.substr(1, 2).as_int();
                tzM = tz.substr(3, 2).as_int();

                int offset = (tzH * 60) + tzM;
                if (sign == '-') {
                    offset *= -1;
                }
                return offset;
            }
        }
        catch (std::invalid_argument const &e)
        {}
        catch (std::out_of_range const &e)
        {}
        throw ValueError("Bad timezone: \"" + tz + "\"");
    }

    void OffsetTZ::offsetAsString(ostream& out, int offsetMins) {
        if (offsetMins != -1) {
            if (offsetMins == 0) {
                out << "Z";
            }
            else
            {
                if (offsetMins < 0) {
                    out << "-";
                    offsetMins *= -1;
                }
                else
                {
                    out << "+";
                }
                out << string::fmt(fmt_02d, offsetMins / 60);
                out << string::fmt(fmt_02d, offsetMins % 60);
            }
        }
    }

    int OffsetTZ::offset() const {
        if (this->offsetMins == -1)
            throw ImplementationError("Timezone not defined");
        return this->offsetMins;
    }

    seconds_t OffsetTZ::toUTC(seconds_t timestamp) const {
        return timestamp - (this->offset() * 60);
    }

    seconds_t OffsetTZ::fromUTC(seconds_t timestamp) const {
        return timestamp + (this->offset() * 60);
    }

    void OffsetTZ::str(ostream& out) const {
        if (this->offsetMins == -1)
            throw ImplementationError("Timezone not defined");
        OffsetTZ::offsetAsString(out, this->offsetMins);
    }
}
