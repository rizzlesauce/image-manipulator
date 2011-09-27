#include <string>
#include <iostream>
#include "Debugger.h"

using namespace std;

Debugger::Debugger() {
    enabled_ = false;
}

Debugger&
Debugger::getInstance() {
    static Debugger instance;

    return instance;
}

void
Debugger::setEnabled(bool enabled) {
    enabled_ = enabled;
}

bool
Debugger::isEnabled() {
    return enabled_;
}

void
Debugger::print(string message) {
    if (enabled_) {
        /*
         cout << message;
         fflush(stdout);
         */

        emit messagePrinted(QString(message.c_str()));
    }
}
