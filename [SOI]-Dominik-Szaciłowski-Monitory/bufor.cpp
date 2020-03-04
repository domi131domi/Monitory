#include "bufor.h"



Bufor::Bufor(size_t maxSize) : maxSize(maxSize) {
    firstNormalMess = messageList.end();
}

bool Bufor::push_back(const Message& message) {
    if(size == maxSize)
        return false;
    if(message.text.size() > MMAX || message.text.size() <= MMIN)
        return false;
    if(message.isPriority == false) {
        try {
            messageList.push_back(message);
        }
        catch(...) {
            return false;
        }
        if(firstNormalMess == messageList.end())
            --firstNormalMess;
        ++size;
    }
    else {
            try {
                messageList.insert(firstNormalMess, message);
            }
            catch(...) {
                return false;
            }
            ++size;
    }
    return true;
}

Message Bufor::pop_front() {
    Message result;
    if(size == 0)
        throw "Nie mozna uzyc pop_front na pustym buforze!";
    result = messageList.front();
    messageList.pop_front();
    --size;
    return result;
}

size_t Bufor::getSize() {
    return size;
}

size_t Bufor::getMaxSize() {
    return maxSize;
}
