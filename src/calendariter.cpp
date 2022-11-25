/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/error.h"
#include "uICAL/util.h"
#include "uICAL/logging.h"
#include "uICAL/calendar.h"
#include "uICAL/calendarentry.h"
#include "uICAL/calendariter.h"
#include "uICAL/tzmap.h"
#include "uICAL/vevent.h"
#include "uICAL/veventiter.h"
#include "uICAL/vline.h"
#include "uICAL/vlinestream.h"
#include "uICAL/vobject.h"
#include "uICAL/vobjectstream.h"

namespace uICAL
{
    CalendarIter::CalendarIter(const Calendar_ptr cal, const DateTime &begin, const DateTime &end)
        : cal(cal)
    {
        if (begin.valid() && end.valid() && end < begin)
        {
            log_error("Begin and end describe a negative range: %s -> %s", begin.as_str().c_str(), end.as_str().c_str());
            throw ValueError("Begin and end describe a negative range");
        }

        for (auto ev : this->cal->events)
        {
            VEventIter_ptr evIt = new_ptr<VEventIter>(ev, begin, end);

            if (evIt->next())
            { // Initialise and filter
                this->events.push_back(evIt);
            }
        }
    }

    bool CalendarIter::next_unchecked()
    {
        if (this->events.size() == 0)
        {
            return false;
        }

        // Get the next event
        auto minIt = this->events.begin();

        this->currentEvent = (*minIt)->event();
        this->currentEntry = (*minIt)->entry();

        // Get the next entry within the event
        if (!(*minIt)->next())
        {
            this->events.erase(minIt);
        }
        return true;
    }

    bool CalendarIter::next()
    {
        bool got_result = false;
        while (true)
        {
            got_result = next_unchecked();
            if (!got_result)
            {
                break;
            }
            VEvent_ptr event = this->currentEvent;
            CalendarEntry_ptr entry = this->currentEntry;
            CalendarIter::recurence_id_t recurence_id = std::make_tuple(event->uid, entry->start());
            // If we already have seen this event, don't add it again
            if (this->recurence_id_set.count(recurence_id) == 0)
            {
                break;
            }
        }
        if (got_result)
        {
            VEvent_ptr event = this->currentEvent;
            if (event != nullptr && event->recurrence.valid())
            {
                // If this is an instance of a reoccuring event, mark it so we don't process it twice
                CalendarIter::recurence_id_t recurence_id = std::make_tuple(event->uid, event->recurrence);
                this->recurence_id_set.insert(recurence_id);
            }
            for (VEvent::exdate_t &exdate : event->exdates)
            {
                DateTime dt = std::get<0>(exdate);
                CalendarIter::recurence_id_t recurence_id = std::make_tuple(event->uid, dt);
                this->recurence_id_set.insert(recurence_id);
            }
        }
        return got_result;
    }

    CalendarEntry_ptr CalendarIter::current() const
    {
        if (!this->currentEntry)
        {
            log_warning("%s", "No more entries");
            throw RecurrenceError("No more entries");
        }
        return this->currentEntry;
    }
}
