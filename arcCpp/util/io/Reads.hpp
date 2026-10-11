#pragma once
#include <cstring>
#include <cstdint>
#include "arcCpp/struct/Seq.hpp"
namespace arc::util::io {


    class Reads {
    private:
        std::uint8_t* arrayBytes = nullptr;
        int sizeArray = 0;
        int pos = 0;
    public:
        Reads(std::uint8_t* threadLocall, ssize_t size) {
            if(arrayBytes == nullptr) {
                arrayBytes = new std::uint8_t[size];
                std::memcpy(arrayBytes, threadLocall, size);
                sizeArray = size;
            }
    }
    void skipByte(int sizeSkip) {
            if(pos + sizeSkip >= sizeArray) {
                return;

            } else pos += sizeSkip;


        }
int i(){
      if(arrayBytes == nullptr || sizeArray < 4 || pos > sizeArray) {
          return 0;

      }

    int result = 0;
    std::memcpy(&result, arrayBytes + pos,  4);
    skipByte(4);
return result;


        }

        std::string str(int size) {

            if(arrayBytes == nullptr || sizeArray < size || pos > sizeArray) {
                return 0;

            }
            std::string* tmpstr = new std::string;
            memccpy(tmpstr, arrayBytes + pos, size, 0);
            return tmpstr;

        }
        ~Reads(){
delete[] arrayBytes;

        }
    };
}