#include "Events.hpp"
#include "map"
#include <functional>
#include <string>
#include "arcCpp/struct/Seq.hpp"
#include "arcCpp/struct/ObjectMap.hpp"
//Я не могу написать Seq, ObjectMap в один день с событиями.
namespace arc {

 arc::structures::ObjectMap<std::string, arc::structures::Seq<std::function<void()>>*> Events::eventsMa;

void Events::on(const std::function<void()>& callbackF, std::string name) {

    if(!eventsMa.contains(name)) {
        auto* seq = new arc::structures::Seq<std::function<void()>>(7, callbackF);
        eventsMa.put(name, seq);
        return;
    }
   auto* seq = eventsMa.get(name);
seq->add(callbackF);
    return;

};

void Events::fire(std::string name) {


if(!eventsMa.contains(name)) {
    return;
}

auto* seq = eventsMa.get(name);
for(int i = 0; i < seq->totalSpace; i++) {
    seq->items[i]();
}

};

void Events::remove(std::string name) {
delete eventsMa.get(name);
eventsMa.remove(name);


};
void Events::clear() {
    eventsMa.clear();


}
}






