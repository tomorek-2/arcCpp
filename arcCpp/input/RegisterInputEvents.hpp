//Я брошу пока этот файл.
#include <atomic>
#include <cstdint>
#include <cstring>
namespace arc::input {
    class RegisterInputEvents {
      struct keyState {
          bool isBusy = true;
          char state = 0;
      };
        keyState *lastUpdate;
        std::atomic<int> lastUpdateSize;
        keyState *thisUpdate;
        std::atomic<int> thisUpdateSize;
        void init() {
            if(lastUpdate == nullptr)
            lastUpdate = new keyState[512];
            if(lastUpdate == nullptr)
            thisUpdate = new keyState[512];

        }
        void updateState(char* c) { //Массив из 512 элементов.
            if(lastUpdate == nullptr) init();


            switch(c) {
                case 'a':
                    if(lastUpdate[])
                    lastUpdate[static_cast(int)<'a'>] = keyState{false, static_cast(int)<'a'>};
                break;
            }
            std::memcpy(lastUpdate, thisUpdate, sizeof(*lastUpdate));
        }

    };
}
