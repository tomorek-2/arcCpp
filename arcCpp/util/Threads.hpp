#ifndef ARC_UTIL_THREADS_HPP
#define ARC_UTIL_THREADS_HPP


#include <thread>
#include <functional>
#include <atomic>
#include "arcCpp/struct/Seq.hpp"

namespace arc::util {

struct ThreadForThreads {
public:
std::atomic<bool> isBusy = false;
arc::structures::Seq<std::function<void()>>* arrayWork = new arc::structures::Seq<std::function<void()>>();



};
    class Threads {
    public:
static void daemon(std::string name, std::function<void()> func);
static void executor(std::function<void()> &func);

private:
static void createArray();
    };
}

#endif
