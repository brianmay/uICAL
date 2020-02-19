#include <iostream>

#include "uICAL/types.h"
#include "uICAL/error.h"

int main() {
    try {
        void test_rrule();
        test_rrule();
        void test_calendar();
        test_calendar();
        void test_tz();
        test_tz();
        void test_datestamp();
        test_datestamp();
    }
    catch(uICAL::Error e) {
        std::cout << "EXCEPTION: " << e.message << std::endl;
    }
    std::cout << "DONE" <<std::endl;
}
