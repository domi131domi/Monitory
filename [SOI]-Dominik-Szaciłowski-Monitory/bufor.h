#ifndef BUFOR_H
#define BUFOR_H

#include <string>
#include <list>
#include <iostream>
#include <chrono>
#include <ctime>
#define EMPTY_MESS ""
#define EMPTY_ID -1
#define MMAX 64
#define MMIN 6

struct Message {
    std::string text = EMPTY_MESS;
    int sender_id = EMPTY_ID;
    bool isPriority = false;
    std::chrono::high_resolution_clock::time_point sended_at;
};

class Bufor {
public:
    Bufor(size_t maxSize);
    bool push_back(const Message& message);
    Message pop_front();
    size_t getSize();
    size_t getMaxSize();
private:
    std::list<Message> messageList;
    std::list<Message>::iterator firstNormalMess;
    size_t size = 0;
    size_t maxSize;
};

#endif // BUFOR_H
