/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#ifndef uical_component_h
#define uical_component_h

#include "uICAL/base.h"
#include "uICAL/icalline.h"

namespace uICAL {
    class VComponentStream;

    class VComponent : public Base {
        public:
            using ptr = std::shared_ptr<VComponent>;
            static ptr init();
            static ptr init(VLine::ptr beginline);

            VComponent();
            VComponent(VLine::ptr beginline);

            bool empty() const;

            const string& getName() const;

            VLine::ptr getPropertyByName(const string& name);
            using vector = std::vector<ptr>;
            vector listComponents(const string& name);

            void str(ostream& out) const;

        protected:
            string name;
            std::vector<VLine::ptr> lines;
            std::vector<VComponent::ptr> children;

            friend VComponentStream;
    };


    class VComponentStream {
        public:
            VComponentStream(VLineStream& stm);

            VComponent::ptr next();

        protected:
            void load(VComponent::ptr comp);
            VLineStream& stm;
    };

}
#endif
