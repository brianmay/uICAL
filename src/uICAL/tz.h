/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_tz_h
#define uical_tz_h

#include "uICAL/tzmap.h"

namespace uICAL {
    class DateStamp;

    class TZ {
        public:
            using ptr = std::shared_ptr<TZ>;

            static ptr init(int offsetMins);
            static ptr init(const string& tz);
            static ptr init(const string& tz, const TZMap::ptr& tzmap);

            static ptr undef();
            static ptr unaware();

            TZ();
            TZ(bool aware);
            TZ(int offsetMins);
            TZ(const string& tz);
            TZ(const string& tz, const TZMap::ptr& tzmap);

            virtual ~TZ() {}

            virtual seconds_t toUTC(seconds_t timestamp) const;
            virtual seconds_t fromUTC(seconds_t timestamp) const;

            bool is_aware() const;
            int offset() const;

            virtual void str(std::ostream& out) const;
            string str() const;

            static int parseOffset(const string& offset);
            static void offsetAsString(std::ostream& out, int offsetMins);
        
        protected:
            bool aware;

        private:
            int offsetMins;
            const TZMap::ptr idmap;
            string id;
    };
    
    std::ostream& operator << (std::ostream& out, const TZ::ptr& tz);
}
#endif
