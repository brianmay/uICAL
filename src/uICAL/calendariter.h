/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_calendariter_h
#define uical_calendariter_h

#include <tuple>
#include <unordered_set>
#include "uICAL/datetime.h"
#include "uICAL/hash_tuple.h"

namespace uICAL {
    class CalendarIter {
        public:
            CalendarIter(const Calendar_ptr cal, const DateTime& begin, const DateTime& end);

            bool next();
            CalendarEntry_ptr current() const;

            using recurence_id_t = std::tuple<string, DateTime>;

        protected:
            const Calendar_ptr cal;

            using events_t = std::vector<VEventIter_ptr>;
            events_t events;

            VEvent_ptr currentEvent;
            CalendarEntry_ptr currentEntry;

            std::unordered_set<recurence_id_t, hash_tuple::hash<recurence_id_t>> recurence_id_set;

            bool next_unchecked();
    };
}
#endif
