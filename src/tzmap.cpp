/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/tzmap.h"
#include "uICAL/tz.h"
#include "uICAL/vline.h"
#include "uICAL/vobject.h"

namespace uICAL {
    TZMap::TZMap() {
        this->add("Z", "Z", tz_UTC);
    }

    void TZMap::add(const VObject_ptr& timezone) {
        string tzId = timezone->getPropertyByName("TZID")->value;

        auto standards = timezone->listObjects("STANDARD");
        for (auto standard : standards) {

            string offset = standard->getPropertyByName("TZOFFSETFROM")->value;
            string name = standard->getPropertyByName("TZNAME")->value;

            this->add(tzId, name, offset);
        }
    }

    void TZMap::add(const string& id, const string& name, const string& tz) {
        this->id_attrib_map[id].tz = new_ptr<OffsetTZ>(name, tz);
        this->id_attrib_map[id].name = name;
    }

    void TZMap::add(const string& id, const string& name, const TZ_ptr tz) {
        this->id_attrib_map[id].tz = tz;
        this->id_attrib_map[id].name = name;
    }

    const TZ_ptr TZMap::getTZ(const string& tzId) {
        return this->id_attrib_map[tzId].tz;
    }

    string TZMap::getName(const string& tzId) {
        return this->id_attrib_map[tzId].name;
    }

    const TZ_ptr TZMap::get_by_name(const string& name) const {
        for (auto& it : this->id_attrib_map) {
            if (it.second.name == name) {
                return it.second.tz;
            }
        }
        return nullptr;
    }

    void TZMap::str(ostream& out) const {
        for (auto i : this->id_attrib_map) {
            out << i.first << " : " << i.second.name << " : ";
            i.second.tz->output_details(out);
            out << uICAL::endl;
        }
    }
}
