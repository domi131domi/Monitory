#include "buforwithmonit.h"
#define TEST

BuforWithMonit::BuforWithMonit(size_t maxSize): bufor(Bufor(maxSize)){

}

bool BuforWithMonit::addMessage(Message message) {
    bool result = true;
    enter();
    #ifdef TEST
    std::cout << "Proba dodania wiadomosci o tresci: "<< message.text << " ID: " << message.sender_id << " Priorytet: " << message.isPriority << std::endl;
    #endif
    if(bufor.getSize() == bufor.getMaxSize()) {
        #ifdef TEST
        std::cout << "Czekam na sciagniecie zeby dodac wiadomosc: "<< message.text << " ID: " << message.sender_id << " Priorytet: " << message.isPriority << std::endl;
        #endif
        wait(full);
    }
    message.sended_at = std::chrono::high_resolution_clock::now();
    if( !bufor.push_back(message) )
        result = false;
    #ifdef TEST
    if(result == true)
    std::cout << "Dodano do bufora wiadomosc o tresci : "<< message.text << " ID: " << message.sender_id << " Priorytet: " << message.isPriority << std::endl;
    #endif
    if(bufor.getSize() == 1)
        signal(empty);
    leave();
    return result;
}

Message BuforWithMonit::readMessage() {
    Message result;
    enter();
    if(bufor.getSize() == 0) {
    #ifdef TEST
    std::cout << "Bufor jest pusty. Nastepuje zawieszenie readera..." << std::endl;
    #endif
        wait(empty);
    }
    result = bufor.pop_front();
    #ifdef TEST
    std::cout << "Sciagnieto z bufora wiadomosc o tresci: "<< result.text << " ID: " << result.sender_id << " Priorytet: " << result.isPriority << std::endl;
    #endif
    if(bufor.getSize() == bufor.getMaxSize() - 1)
        signal(full);
    leave();
    return result;
}
