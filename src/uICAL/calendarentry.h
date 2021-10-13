/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_calendarentry_h
#define uical_calendarentry_h

#include "uICAL/base.h"
#include "uICAL/datetime.h"
#include "uICAL/dateperiod.h"

namespace uICAL {
    class CalendarEntry : public Base {
        public:
            enum class Type {
                NONE, EVENT,
            };

            CalendarEntry(Type type, const string& summary, const string& location, const DateTime& start, bool start_has_time, const DateTime& end, bool end_has_time);

            Type type() const;
            const string summary() const;
            const string location() const;
            DateTime start() const;
            DateTime end() const;
            bool start_has_time;
            bool end_has_time;

            void str(ostream& out) const;

            static string asString(Type type);

        protected:
            Type _type;
            string _summary;
            string _location;
            DateTime _start;
            DateTime _end;
    };
}

#endif
