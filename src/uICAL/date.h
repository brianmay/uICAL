/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_date_h
#define uical_date_h

#include "uICAL/types.h"
#include "uICAL/base.h"
#include "uICAL/datetime.h"
#include "uICAL/epochtime.h"

namespace uICAL {
    class Date : public Base {
        public:
            Date();
            Date(const string& date);
            Date(unsigned year, unsigned month, unsigned day);
            Date(const DateTime& datetime);

            void str(ostream& out) const;

            unsigned year;
            unsigned month;
            unsigned day;

            bool valid() const;

            DateTime::Day dayOfWeek() const;
            unsigned weekNo() const;
            unsigned dayOfYear() const;
            unsigned daysInMonth() const;
            unsigned daysInYear() const;

            void incYear(unsigned n);
            void incMonth(unsigned n);
            void incWeek(unsigned n, DateTime::Day wkst);
            void incDay(unsigned n);

            void decDay(unsigned n);
            void decMonth(unsigned n);

            void setWeekNo(unsigned n);

            Date(const Date&) = default;
            Date& operator = (const Date& ds);

            bool operator > (const Date& ds) const;
            bool operator < (const Date& ds) const;
            bool operator <= (const Date& ds) const;
            bool operator == (const Date& ds) const;
            bool operator != (const Date& ds) const;

            unsigned operator - (const Date& other) const;

        protected:
            seconds_t index() const;
            void validate() const;
            DateTime::Day getWeekDay(unsigned days) const;
    };
}
#endif
