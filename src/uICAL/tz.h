/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_tz_h
#define uical_tz_h

#include "uICAL/base.h"

namespace uICAL {
    class DateStamp;
    class TZMap;

    class TZ : public Base {
        public:

            virtual seconds_t toUTC(seconds_t timestamp) const = 0;
            virtual seconds_t fromUTC(seconds_t timestamp) const = 0;
            virtual bool is_aware() const;
    };

    class UnawareTZ : public TZ {
        public:
            UnawareTZ();
            virtual seconds_t toUTC(seconds_t timestamp) const;
            virtual seconds_t fromUTC(seconds_t timestamp) const;
            virtual void str(ostream& out) const;
    };

    class OffsetTZ : public TZ {
        public:
            // OffsetTZ();
            OffsetTZ(bool aware);
            OffsetTZ(int offsetMins);
            OffsetTZ(const string& tz);
            // OffsetTZ(const string& tz, const TZMap_ptr& tzmap);

            virtual seconds_t toUTC(seconds_t timestamp) const;
            virtual seconds_t fromUTC(seconds_t timestamp) const;
            virtual void str(ostream& out) const;

            int offset() const;

            static int parseOffset(const string& offset);
            static void offsetAsString(ostream& out, int offsetMins);

        private:
            int offsetMins;
    };

    extern const TZ_ptr tz_unaware;
    extern const TZ_ptr tz_UTC;
}
#endif
