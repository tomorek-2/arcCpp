#include "Threads.hpp"
#include <thread>
#include <functional>
#include <string>
#include "arcCpp/struct/Seq.hpp"
#include <atomic>
#include <unistd.h>
#include <mutex>
namespace arc::util {


        std::atomic<bool> isBusy = false;
        arc::structures::Seq<std::function<void()>>* arrayWork = new arc::structures::Seq<std::function<void()>>();
        std::mutex queueMutex;



    std::function<void()>* arrayFunc = nullptr;

    void Threads::daemon(std::string name, const std::function<void()> func) {
        if(arrayFunc == nullptr) {
            createArray();
        }
        std::thread thread(func);
        thread.detach();
    };
void Threads::executor(std::function<void()> func) {
        /*if(arrayWork != nullptr) {
        while (true) {

                std::lock_guard<std::mutex> lock(queueMutex);

                arrayWork->add(func);

                break;

        }
    } */ std::thread thread(func); thread.detach();
};

void Threads::createArray() {
if(arrayFunc == nullptr) {
    arrayFunc = new std::function<void()>[100];
} //Не знаю зачем эта проверка


};
void Threads::initThread() {

    std::thread executorThread([this](){


       while(true) {

           std::lock_guard<std::mutex> lock(queueMutex);
             for(int i = 0; i < arrayWork->totalSpace; i++) {

                     this->daemon("aa", arrayWork->get(i));


             }
           queueMutex.unlock();
           usleep(1000);
       }
    });
    executorThread.detach();

}

}

