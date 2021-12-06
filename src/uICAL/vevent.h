/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_vevent_h
#define uical_vevent_h

#include "uICAL/base.h"
#include "uICAL/datetime.h"

namespace uICAL {
    class VObject;

    class VEvent : public Base {
        public:
            VEvent(const VObject_ptr& event, const TZMap_ptr& tzmap);

            void str(ostream& out) const;

            string summary;
            string location;
            DateTime start;
            DateTime end;
            RRule_ptr rrule;
            bool start_has_time;
            bool end_has_time;
            string uid;
            DateTime recurrence;
            bool recurrence_has_time;

            using exdate_t = std::tuple<DateTime, bool>;
            std::vector<exdate_t> exdates;

            friend class VEventIter;
    };
}
#endif
