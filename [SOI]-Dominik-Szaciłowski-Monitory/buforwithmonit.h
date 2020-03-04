#ifndef MYMONITOR_H
#define MYMONITOR_H
#include "monitor.h"
#include "bufor.h"

class BuforWithMonit : private Monitor {
public:
    BuforWithMonit(size_t maxSize);
    bool addMessage(Message message);
    Message readMessage();
private:
    Condition empty, full;
    Bufor bufor;
};

#endif // MYMONITOR_H
