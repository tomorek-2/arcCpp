#include "Threads.hpp"
#include <thread>
#include <functional>
#include <string>
#include "arcCpp/struct/Seq.hpp"
#include <atomic>
namespace arc::util {


        std::atomic<bool> isBusy = false;
        arc::structures::Seq<std::function<void()>>* arrayWork = new arc::structures::Seq<std::function<void()>>();




    std::function<void()>* arrayFunc = nullptr;

    void Threads::daemon(std::string name, const std::function<void()> func) {
        if(arrayFunc == nullptr) {
            createArray();
        }
        std::thread thread(func);





        thread.detach();
    };
void Threads::executor(std::function<void()> &func) {




};
void Threads::createArray() {
if(arrayFunc == nullptr) {
    arrayFunc = new std::function<void()>[100];
} //Не знаю зачем эта проверка


};

}

