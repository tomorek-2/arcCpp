#include "Threads.hpp"
#include <thread>
#include <functional>
#include <string>
#include "arcCpp/struct/Seq.hpp"

namespace arc::util {
    std::function<void()>* arrayFunc = nullptr;

    void Threads::daemon(std::string name, const std::function<void()> func) {
        if(arrayFunc == nullptr) {
            createArray();
        }
        std::thread thread(func);





        thread.detach();
    };
void Threads::executor(std::function<void()>) {


};
void Threads::createArray() {
if(arrayFunc == nullptr) {
    arrayFunc = new std::function<void()>[100];
} //Не знаю зачемп эта проверка


};
}

