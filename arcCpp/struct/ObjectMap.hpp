#ifndef ARC_STRUCTURES_OBJECTMAP_HPP
#define ARC_STRUCTURES_OBJECTMAP_HPP
#include <functional>
#include <span>
#include "arcCpp/util/Log.hpp"
#include <type_traits>

namespace arc::structures {
    template <typename K, typename V>
    class ObjectMap {

        K *key = nullptr;
        V *value = nullptr;
        bool *freeSpace = nullptr;
        int length = 405;
        int totalSpace = 0;
int lastUse = 0;
    public:
        bool replace = true;
        K operator[](size_t index) {
            if(index >= length) return K{};
            if(key == nullptr) return K{};
            return key[index];
        }
        void resize(int size) {


            if (key == nullptr) {
                V *newValue = new V[size];
                value = newValue;
                K *newKey = new K[size];
                key = newKey;
                bool *newSpace = new bool[size];
                freeSpace = newSpace;
                lastUse = 0;
                for(int i = 0; i < size; i++) freeSpace[i] = true;
            } else {

                K *newKey = new K[size];
                V *newValue = new V[size];
                bool *newSpace = new bool[size];
                int i2 = 0;
                for(int i3 = 0; i3 < size; i3++) {
                    newSpace[i3] = true;



                }
                for(bool space : std::span<bool>(freeSpace, length)) {
                    newSpace[i2] = freeSpace[i2];
                    i2++;
                }


                int i = 0;
std::hash <K> hash;

while(true) {
if(i < length) {
    if (hash(key[i]) % length < length) {
        if (!freeSpace[i]) {
if(!newSpace[i]) {
          int  keyHash = hash(key[i]) % size;
            newKey[keyHash] = key[i];
            newValue[keyHash] = value[i];
            newSpace[keyHash] = false;
            i++;
}
        } else i++;
    } else break;
} else break;
}

                delete[] key;
                key = newKey;
                delete[] value;
                value = newValue;
                delete[] freeSpace;
                freeSpace = newSpace;
                lastUse = 0;
                length = size;
            }



        };

        void put(K keyP, V valueP) {
if(key == nullptr) resize(length);
if(value == nullptr) resize(length);
bool existsIsKey = false;
std::hash <K> hash;
int i4 = 0;
if(replace) {
    existsIsKey = contains(keyP);
}
            int keyHash = hash(keyP) % length;
while(true) {
    if(i4 >= length) {
        resize(length * 2);
       keyHash = hash(keyP) % length;
        i4 = 0;
    }
    if(keyHash >= length) {
        resize(length * 2);
      keyHash = hash(keyP) % length;
        i4 = 0;
    }
    if(freeSpace[keyHash] &&  !existsIsKey || replace) {
        key[keyHash] = keyP;
        value[keyHash] = valueP;
        freeSpace[keyHash] = false;
        totalSpace++;
        return;
    }
     if(key[keyHash] == keyP && replace) {
         key[keyHash] = keyP;
         value[keyHash] = valueP;
         freeSpace[keyHash] = false;

         return;
     }
     if(totalSpace > length * 0.50) {
         resize(length * 2);
keyHash = hash(keyP) % length;
i4 = 0;
     }
     keyHash++;
i4++;
}


        };
         bool contains(K keyP) {
            if(key == nullptr) resize(length);
            if(value == nullptr) resize(length);
            std::hash <K> hash;

            int keyHash = hash(keyP) % length;
            while (true) {
                if(keyHash >= length) return false;
                if (!freeSpace[keyHash] && keyP== key[keyHash]) {
                    return true;
                } else {
                    if(keyHash > length) return false;
                    keyHash++;
                }
            }
        };
        V get(K keyP) {
            if(key == nullptr) resize(length);
            if(value == nullptr) resize(length);
            std::hash <K> hash;
            auto keyWWW = hash(keyP); //Надо было поменять int на auto. Мне нужен такой компилятор, который находил на компиляции
            int keyHash = keyWWW % length;
            if(lastUse < length) {
                if(!freeSpace[lastUse] && key[lastUse] == keyP) {
                    return value[lastUse];
                }
            }
             // arc::util::Log::log("aOM001");
            while (true) {
               // arc::util::Log::log("aOM002");
                if (keyHash >= length) {
                    if constexpr(std::is_same_v<V, std::string>) {
                        return "";
                    } else return V{};

                }
                if (!freeSpace[keyHash] && keyP== key[keyHash] ) {
                     //arc::util::Log::log("aOM003");
                    lastUse = keyHash;
                    return value[keyHash];
                } else {
                    if(keyHash > length) return V{};

                    keyHash++;
                }
            }
        };
        void remove(K keyP) {
            if(key == nullptr) resize(length);
            if(value == nullptr) resize(length);
            std::hash <K> hash;
            int keyHash = hash(keyP) % length;
            while (true) {
                if (keyP== key[keyHash]) {
                    freeSpace[keyHash] = true;
                    totalSpace--;
                    return;
                } else {
                    if(keyHash > length* 0.8) return;
                    keyHash++;
                }


            }
    };
void clear() {
//Пусть будет.
    delete[] key;
    key = nullptr;
    delete[] value;
    value = nullptr;
    delete[] freeSpace;
    freeSpace = nullptr;
    length = 405;
    lastUse = 0;
    resize(length);
}
~ObjectMap() {
  delete[] key;
  key = nullptr;
  delete[] value;
  value = nullptr;
  delete[] freeSpace;
    freeSpace = nullptr;

};

    };
}
#endif
