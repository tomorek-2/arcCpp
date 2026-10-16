#pragma once
#include <cstring>
#include <cstdint>
#include "arcCpp/struct/Seq.hpp"
 namespace arc::util::io {


    class Writes {
    private:
        std::uint8_t* arrayBytes = nullptr;
        int* sizeArray = new int(0);
        int* pos =  new int(0);
    public:
        Writes(std::uint8_t* threadLocall, ssize_t size) {
            if(arrayBytes == nullptr) {
                arrayBytes = new std::uint8_t[size];
                std::memcpy(arrayBytes, threadLocall, size);
                *sizeArray = size;
            }
        }
        void skipByte(int sizeSkip) {
            if(pos + sizeSkip >= sizeArray) {
                return;

            } else pos += sizeSkip;


        }
        void i(int i){
            if(arrayBytes == nullptr || *sizeArray < 4 || *pos > *sizeArray) {
                return;

            }
           std::memcpy(arrayBytes + *pos, &i, 4);
            skipByte(4);
            return;


        }

        void i16(int i16){ //i это int, 16 это кол-во бит.
            if(arrayBytes == nullptr || *sizeArray < 2 || *pos > *sizeArray) {
                return;

            }

            std::memcpy(arrayBytes + *pos, &i16, 2);
            skipByte(2);
            return;

//Зачем метод? Мне нужен просто метод std::string str()
        }

        void str(std::string str) {

            if(arrayBytes == nullptr || *sizeArray < str.length() || *pos > *sizeArray) {
                return;

            }
            std::memcpy(arrayBytes + *pos, str.data(), str.length());

        }
auto toBytes() {

    return arrayBytes;

        }
auto operator=(const Writes& other) {

    return arrayBytes;
        }
        ~Writes(){
            delete[] arrayBytes;
            delete pos;
            delete sizeArray;
        }
    };
 }