/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_tzinfo_h
#define uical_tzinfo_h

#include "uICAL/base.h"

namespace uICAL {
    class VComponent;

    class TZMap : public Base {
        public:
            TZMap();

            void add(const VObject_ptr& timezone);
            void add(const string& id, const string& name, const TZ_ptr tz);

            // string findId(const string& nameOrId) const;

            const TZ_ptr getTZ(const string& tzId);
            string getName(const string& tzId);

            void str(ostream& out) const;

        protected:
            void add(const string& id, const string& name, const string& tz);
            int parseOffset(const string& offset) const;

            typedef struct {
                TZ_ptr tz;
                string name;
            } attribs_t;

            std::map<string, attribs_t> id_attrib_map;
    };
}
#endif
