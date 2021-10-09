/*############################################################################
# Copyright (c) 2020 Source Simian  :  https://github.com/sourcesimian/uICAL #
############################################################################*/
#include "uICAL/cppstl.h"
#include "uICAL/types.h"
#include "uICAL/error.h"
#include "uICAL/util.h"
#include "uICAL/logging.h"
#include "uICAL/vline.h"
#include "uICAL/vlinestream.h"

namespace uICAL {
    VLineStream::VLineStream(istream& ical)
    : ical(ical)
    , currentLine(nullptr)
    , repeat(false)
    {}

    void VLineStream::repeatLine() {
        this->repeat = true;
    }

    const VLine_ptr VLineStream::next() {
        if (!this->repeat || this->currentLine == nullptr) {
            string line;
            if(line.readfrom(this->ical, '\n')) {
                line.rtrim();
                while (true) {
                    char next = this->ical.peek();
                    string line2;
                    if (next != 0x20 && next != 0x09) break;
                    this->ical.get();
                    if (!line2.readfrom(this->ical, '\n')) break;
                    line2.rtrim();
                    line = line + line2;
                }
                this->currentLine = new_ptr<VLine>(line);
            }
            else {
                this->currentLine = nullptr;
            }
        }
        this->repeat = false;
        return this->currentLine;
    }
}
