#include <iostream>
#include "buforwithmonit.h"
#include <thread>
#define MAX_SIZE 6

void bufor_write(Message msg,BuforWithMonit& bufor, std::chrono::high_resolution_clock::time_point when) {
    while(std::chrono::high_resolution_clock::now() <= when) {}
    if(!bufor.addMessage(msg))
        std::cout << "Nie udalo sie wyslac wiadomosci. Wiadomosc za krotka lub za dluga." << std::endl;
}

void bufor_read(BuforWithMonit& bufor, unsigned how_many, std::chrono::high_resolution_clock::time_point when) {
    Message msgs[how_many];
    for(unsigned i = 0; i < how_many; i++) {
    msgs[i] = bufor.readMessage();
    }
    std::cout << std::endl;
    for(unsigned i = 0; i < how_many; i++) {
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(msgs[i].sended_at - when);
    std::cout << "Tekst: " << msgs[i].text << " ID:" << msgs[i].sender_id << " Priorytet:" << msgs[i].isPriority << " Czas(mikrosekundy):" << time.count() << std::endl;
    }
}

void bufor_read2(BuforWithMonit& bufor, unsigned how_many) {
    Message msg;
    for(unsigned i = 0; i < how_many; i++) {
    msg = bufor.readMessage();
    std::cout << "Wiadomosc odebrana: Tekst: " << msg.text << std::endl;
    }
}

void bufor_write2(Message msg,BuforWithMonit& bufor, int how_many) {
    for(int i = 0; i < how_many; i++) {
    msg.text = "Wiadomosc" + std::to_string(i+1);
    //if(i == how_many - 1)
      //  msg.isPriority = true;
    if(bufor.addMessage(msg))
        std::cout << "Wyslano wiadomosc o tresci : "<< msg.text << std::endl;
    }
}

int main() {
    int which = 1;
    while(which != 0) {
        std::cout << std::endl;
        std::cout << "Wybierz test:" << std::endl << "0. Wyjscie" << std::endl << "1. Zasymuluj wysylanie wiadomosci przez 5 uzytkownikow w jednym momencie." << std::endl << "2. Testuj przepelnienie bufora." << std::endl << "3. Testuj pobranie wiadomosci z pustego bufora." << std::endl << "4. Dodanie wiadomosci o zlej dlugosci." << std::endl;

        sleep(0.1);
        std::cin >> which;
        std::cout << std::endl;

        switch (which) {
        case 0:
            break;
        case 1:
        {
            std::cout <<"W tym tescie po 1s 5 uzytkownikow wysle po 1 wiadomosci w tym samym momencie.\nNastepnie zostanie odpalony reader, ktory wyswietli wiadomosci oraz czas w mikrosekundach od wyslania\nwiadomosci do dodania jej do bufora" << std::endl << std::endl;
            BuforWithMonit bufor(MAX_SIZE);
            std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);
            Message msg;
            msg.isPriority = false;
            msg.sender_id = 1;
            msg.text = "Wiadomosc1.";
            std::thread wr1(bufor_write,msg,std::ref(bufor),t);
            msg.text = "Wiadomosc2.";
            msg.sender_id = 2;
            std::thread wr2(bufor_write,msg,std::ref(bufor),t);
            msg.text = "Wiadomosc3.";
            msg.sender_id = 3;
            msg.isPriority = true;
            std::thread wr3(bufor_write,msg,std::ref(bufor),t);
            msg.sender_id = 4;
            msg.text = "Wiadomosc4.";
            std::thread wr4(bufor_write,msg,std::ref(bufor),t);
            msg.sender_id = 5;
            msg.text = "Wiadomosc5.";
            std::thread wr5(bufor_write,msg,std::ref(bufor),t);
            sleep(2);
            std::thread rd(bufor_read,std::ref(bufor),5,t);
            wr1.join();
            wr2.join();
            wr3.join();
            wr4.join();
            wr5.join();
            rd.join();
            break;
        }
        case 2:
        {
            std::cout<<"W tym tescie jeden uzytkownik probuje wyslac 7 wiadomosci z rzedu(maksymalny rozmiar bufora to 6).\nMozemy zaobserwowac ze pierwsze 6 zostaje dodane do bufora od razu po probie jej dodania.\n7 wiadomosc czeka jadnak na sciagniecie przynajmniej jednej wiadomosci przed dodaniem jej." << std::endl << std::endl;
            Message msg;
            BuforWithMonit bufor(MAX_SIZE);
            msg.sender_id = 1;
            msg.isPriority = false;
            std::thread wr(bufor_write2,msg,std::ref(bufor),7);
            sleep(1);
            std::thread rd(bufor_read2,std::ref(bufor),7);
            wr.join();
            rd.join();
            break;
        }
        case 3:
        {
            std::cout <<"Wywolujemy odbranie wiadomosci jednak nie wyslalismy wczesniej zadnej.\nProgram poprawnie zawiesza sie czakajac na dostarczenie wiadomosci.\nNiestety program trzeba sakonczyc wciskajac Ctrl+C" << std::endl << std::endl;
            BuforWithMonit bufor(MAX_SIZE);
            std::cout << "Odpalenie readera." << std::endl;
            std::thread rd(bufor_read2,std::ref(bufor),1);
            rd.join();
            break;
        }
        case 4:
        {
             BuforWithMonit bufor(MAX_SIZE);
             Message msg;
             msg.text = "Min";
             msg.isPriority = false;
             msg.sender_id = 1;
             if(!bufor.addMessage(msg))
                std::cout << "Nie udalo sie wyslac wiadomosci. Wiadomosc za krotka lub za dluga." << std::endl;
             msg.text = "Normalna Wiadomosc";
             if(!bufor.addMessage(msg))
                std::cout << "Nie udalo sie wyslac wiadomosci. Wiadomosc za krotka lub za dluga." << std::endl;
             std::thread rd(bufor_read2,std::ref(bufor),1);
             rd.join();
             break;
        }
        default:
            std::cout << "Nie wykryta komenda. Sprobuj ponownie." << std::endl;
            break;
        }
    }

    return 0;
}
