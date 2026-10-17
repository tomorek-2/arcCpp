

#include "arcCpp/util/Log.hpp"
#include "arcCpp/struct/ObjectMap.hpp"
#include "arcCpp/struct/Seq.hpp"
#include "arcCpp/util/Threads.hpp"
#include <sstream>
#include <functional>
#include <iostream>
#include <charconv>
#include <unistd.h>
#include <atomic>
#include <sys/ioctl.h>
#include <chrono>
#include <sys/socket.h>
#include "arpa/inet.h"
#include <netdb.h>
#include <cstdint>

#include <cstring>
struct Var {
    int type = 0; //0 Это num, 1 это str. bool это int. 2 ЭтоМаркер?
    std::string str = "";
    double num = 0;

};
static  arc::structures::ObjectMap<std::string, std::function<void(const std::string&)>> OQmap;
static    std::atomic<bool> isRunning = false;
static std::atomic<bool> canEditWWWLine1{true};
static char* WWWLineBuffer;
static int SizeWWWLineBuffer;
static arc::structures::ObjectMap<std::string, Var> doubleMap;
static arc::structures::ObjectMap<std::string, std::function<void(const std::string&)>> InstructOpMap;
static arc::structures::Seq<std::string> seq; //Команды.
static arc::structures::Seq<std::string> seqL;
static int ipt = 0;
static int stepCounter = 0;
static std::chrono::steady_clock::time_point waitMS; //Я тут подумал, чтобы избежать в главном потоке зависаний, нужно для wait логику как в горутинах (я не знаю что такое горутины).
static bool isWaitingMLog = false;
class parser {
  public:
void  parserStrings(auto inputResult, std::string* output) { //Я добавил этот метод для расширения op.
    std::string result, result2, result3;
    auto bytes = inputResult.c_str();
    int i = 0;
    bool breakInFor = false;
    for(int w = 0; i < inputResult.length(); i++) {
        auto charB = bytes[i];
        switch(charB) {
            case 32:

                i++;
                result = inputResult.substr(0, w);
                breakInFor = true;
                break;

            case 10:

              //  arc::util::Log::warn("Неккоректный аргумент");
                return;
                break;

        }
        if(breakInFor) {
            breakInFor = false;
            break;
        }
        result += charB;
        w++;
    }


    for(int w = 0; i < inputResult.length(); i++) {
        auto charB = bytes[i];
        switch (charB) {
            case 32:

                i++;
                breakInFor = true;
                break;
            case 10:

              //  arc::util::Log::warn("Неккоректный аргумент");
                output[0] = result;
                return;
                break;
            default:
                result2 += charB;
                w++;
                break;
        }
        if(breakInFor) {
            breakInFor = false;
            break;
        }
    }
    for(int w = 0; i < inputResult.length(); i++) {
        auto charB = bytes[i];
        switch (charB) {
            case 32:

                i++;
                breakInFor = true;
                break;
            case 13:
            case 10:
               // arc::util::Log::warn("Неккоректный аргумент");
               output[0] = result;
               output[1] = result2;
                return;
                break;
            default:
                result3 += charB;
                w++;
                break;
        }
        if(breakInFor) {
            breakInFor = false;
            break;
        }
    }
    output[0] = result;
    output[1] = result2;
    output[2] = result3;

     }
     void init() {

        doubleMap.replace = true;
         InstructOpMap.put("sub", [](std::string inputResult) {
             std::string result, result2, result3;
             auto bytes = inputResult.c_str();
             int i = 0;
             bool breakInFor = false;
             parser parserw;
             std::string tmpStrings[3];
             parserw.parserStrings(inputResult, tmpStrings);
             result = tmpStrings[0];
             result2 = tmpStrings[1];
             result3 = tmpStrings[2];
             double vaw = 0.0;
             double vaw2 = 0.000000;
             auto [ptr, ec] = std::from_chars(result2.data(), result2.data() + result2.size(), vaw);
             auto [ptr2, ec2] = std::from_chars(result3.data(), result3.data() + result3.size(), vaw2);

             Var v = doubleMap.get(result);
             if( (ec == std::errc{}) && (ptr == result2.data() + result2.size())) {
                 if ((ec2 == std::errc{}) && (ptr2 == result3.data() + result3.size())) {
                     v.num = vaw - vaw2;
                     doubleMap.put(result, v);


                 } else if(doubleMap.contains(result3)) {
                     v = doubleMap.get(result3);
                     if(v.type == 1) {
                         return;
                     }

                     v.num = vaw - v.num;
                     doubleMap.put(result, v);
                 } else   {
                     auto v =  doubleMap.get(result3);
                     v.num = vaw;
                     doubleMap.put(result, v);
                 }

             } else if(doubleMap.contains(result2)) {
                 v = doubleMap.get(result2);
                 if(v.type == 1) {
                     return;
                 }

                 v.num =  v.num - ((ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num) ;
                 doubleMap.put(result, v);
             } else   {
                 auto v =  doubleMap.get(result2);  //создание временного объекта.
                 v.num -= (ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num; //Идёт сама операция сложения: если result3 не смог преобразаваться, то берётся из карты значение.
                 doubleMap.put(result, v);
             }
         });
         InstructOpMap.put("div", [](std::string inputResult) {
             std::string result, result2, result3;
             auto bytes = inputResult.c_str();
             int i = 0;
             bool breakInFor = false;
             parser parserw;
             std::string tmpStrings[3];
             parserw.parserStrings(inputResult, tmpStrings);
             result = tmpStrings[0];
             result2 = tmpStrings[1];
             result3 = tmpStrings[2];
             double vaw = 0.0;
             double vaw2 = 0.000000;
             auto [ptr, ec] = std::from_chars(result2.data(), result2.data() + result2.size(), vaw);
             auto [ptr2, ec2] = std::from_chars(result3.data(), result3.data() + result3.size(), vaw2);

             Var v = doubleMap.get(result);
             if( (ec == std::errc{}) && (ptr == result2.data() + result2.size())) {
                 if ((ec2 == std::errc{}) && (ptr2 == result3.data() + result3.size())) {
                     v.num = vaw / vaw2;
                     doubleMap.put(result, v);


                 } else if(doubleMap.contains(result3)) {
                     v = doubleMap.get(result3);
                     if(v.type == 1) {
                         return;
                     }

                     v.num = vaw / v.num;
                     doubleMap.put(result, v);
                 } else   {
                     auto v =  doubleMap.get(result3);
                     v.num = vaw;
                     doubleMap.put(result, v);
                 }

             } else if(doubleMap.contains(result2)) {
                 v = doubleMap.get(result2);
                 if(v.type == 1) {
                     return;
                 }

                 v.num =  v.num / ((ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num);
                 doubleMap.put(result, v);
             } else   {
                 auto v =  doubleMap.get(result2);  //создание временного объекта.
                 v.num /= (ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num; //Идёт сама операция сложения: если result3 не смог преобразаваться, то берётся из карты значение.
                 doubleMap.put(result, v);
             }
         });
         InstructOpMap.put("mul", [](std::string inputResult) {
             std::string result, result2, result3;
             auto bytes = inputResult.c_str();
             int i = 0;
             bool breakInFor = false;
             parser parserw;
             std::string tmpStrings[3];
             parserw.parserStrings(inputResult, tmpStrings);
             result = tmpStrings[0];
             result2 = tmpStrings[1];
             result3 = tmpStrings[2];
             double vaw = 0.0;
             double vaw2 = 0.000000;
             auto [ptr, ec] = std::from_chars(result2.data(), result2.data() + result2.size(), vaw);
             auto [ptr2, ec2] = std::from_chars(result3.data(), result3.data() + result3.size(), vaw2);

             Var v = doubleMap.get(result);
             if( (ec == std::errc{}) && (ptr == result2.data() + result2.size())) {
                 if ((ec2 == std::errc{}) && (ptr2 == result3.data() + result3.size())) {
                     v.num = vaw * vaw2;
                     doubleMap.put(result, v);


                 } else if(doubleMap.contains(result3)) {
                     v = doubleMap.get(result3);
                     if(v.type == 1) {
                         return;
                     }

                     v.num = vaw * v.num;
                     doubleMap.put(result, v);
                 } else   {
                     auto v =  doubleMap.get(result3);
                     v.num = vaw;
                     doubleMap.put(result, v);
                 }

             } else if(doubleMap.contains(result2)) {
                 v = doubleMap.get(result2);
                 if(v.type == 1) {
                     return;
                 }

                 v.num = ((ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num) * v.num;
                 doubleMap.put(result, v);
             } else   {
                 auto v =  doubleMap.get(result2);  //создание временного объекта.
                 v.num *= (ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num; //Идёт сама операция сложения: если result3 не смог преобразаваться, то берётся из карты значение.
                 doubleMap.put(result, v);
             }
         });
        InstructOpMap.put("add", [](std::string inputResult) {
            std::string result, result2, result3;
            auto bytes = inputResult.c_str();
            int i = 0;
            bool breakInFor = false;
            parser parserw;
            std::string tmpStrings[3];
           parserw.parserStrings(inputResult, tmpStrings);
            result = tmpStrings[0];
            result2 = tmpStrings[1];
            result3 = tmpStrings[2];
            double vaw = 0.0;
            double vaw2 = 0.000000;
            auto [ptr, ec] = std::from_chars(result2.data(), result2.data() + result2.size(), vaw);
            auto [ptr2, ec2] = std::from_chars(result3.data(), result3.data() + result3.size(), vaw2);

            Var v = doubleMap.get(result);
            if( (ec == std::errc{}) && (ptr == result2.data() + result2.size())) {
                if ((ec2 == std::errc{}) && (ptr2 == result3.data() + result3.size())) {
                    v.num = vaw + vaw2;
                doubleMap.put(result, v);


            } else if(doubleMap.contains(result3)) {
                    v = doubleMap.get(result3);
                    if(v.type == 1) {
                        return;
                    }

                    v.num = vaw + v.num;
                    doubleMap.put(result, v);
                } else   {
                    auto v =  doubleMap.get(result3);
               v.num = vaw;
                    doubleMap.put(result, v);
                }

            } else if(doubleMap.contains(result2)) {
                v = doubleMap.get(result2);
                if(v.type == 1) {
                    return;
                }

                v.num = ((ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num) + v.num;
                doubleMap.put(result, v);
            } else   {
              auto v =  doubleMap.get(result2);  //создание временного объекта.
              v.num += (ec2 == std::errc{}) ? vaw2 : doubleMap.get(result3).num; //Идёт сама операция сложения: если result3 не смог преобразаваться, то берётся из карты значение.
              doubleMap.put(result, v);
            }
        });
        OQmap.put("print", [](std::string result2) {
             const char* bytes = result2.c_str();
             std::string result2A;
             auto b = bytes[0];
             int l = 0;
             bool isString = false;
             for(int i = 0; i < result2.length(); i++) {
                 b = bytes[i];
                 if(b == 34) {
                     l++;
                 } else    result2A += b;
                 if(l == 2) {
                     isString = true;
                     break;
                 }




            }
            if(!isString) {
                if(doubleMap.get(result2A).type == 0) {
                result2 = std::to_string(doubleMap.get(result2A).num);
                } else if(doubleMap.get(result2A).type == 1) {
                 result2 = doubleMap.get(result2A).str;
            }
            } else result2 = result2A;

            arc::util::Log::info(result2);
        });

        OQmap.put("end", [](std::string result2) {
            stepCounter = -1;

        });



        OQmap.put("wait", [](std::string inputResult) {
            double vaw = 0.0;
            auto [ptr, ec] = std::from_chars(inputResult.data(), inputResult.data() + inputResult.size(), vaw);


            if( (ec == std::errc{}) && (ptr == inputResult.data() + inputResult.size())) {
            if(isWaitingMLog) {
                auto currentTime = std::chrono::steady_clock::now();
             //   std::chrono::duration<double, std::milli> doubleCurrentTime;
                std::chrono::duration<double, std::milli> difference = currentTime - waitMS;
            if(difference.count() > 0 ) {
            isWaitingMLog = false;
            }

            } else {
             auto delay = std::chrono::milliseconds((int)(vaw * 1000));
            waitMS = std::chrono::steady_clock::now() + delay;


            isWaitingMLog = true;
            }



            } else arc::util::Log::info("  " +inputResult);
        });
        OQmap.put("op", [](std::string inputResult) {
            std::string operat = "";
            std::string result = "";
            std::string result2 = "";
            int i = 0;
            const char* bytes = inputResult.c_str();
            for(int w = 0; i < inputResult.length(); i++) {
            auto charB = bytes[i];
            if(charB == 32) {
                i++;
                operat = inputResult.substr(0, w);
                 break;
            }
             if(charB == 10) {
                 arc::util::Log::info("Нету второго аргумента");
                 return;
            }
             operat += charB;
             w++;


              }
          for(int w = 0; i < inputResult.length(); i++) {
                auto charB = bytes[i];
                if(charB==32) {
                    i++;
                   result = inputResult.substr(operat.length() + 1);
                   break;
                }
                result += charB;


          }

          auto resultVoid = InstructOpMap.get(operat);
          if(resultVoid) {
              resultVoid(result);
              return;
          } else arc::util::Log::warn("operat was null");

        });
        OQmap.put("set", [](std::string inputResult) {
                 std::string result = "";
                 std::string result2 = "";
                 int i = 0;
                 int16_t wIsString = 0;
                 int16_t wIsString2 = 0;
                 const char* bytes = inputResult.c_str();
                 bool breakInFor = false;
                 for(int l = 0; i < inputResult.length(); i++) {

                     auto b = bytes[i];
                     switch(b) {
                         case 32:
                             i++;
                             result = inputResult.substr(0, l);

                             breakInFor = true;
                             break;
                         case 34:

                            arc::util::Log::warn("Введён неверный аргумент. 35" + result);
                             return;


                             break;
                     }
                     if(breakInFor) break;
                     result += b;
                     l++;
                 }
                 breakInFor = false;
                 if(result == "") {
                     arc::util::Log::warn("Введён неверный аргумент. 36" + result);
                     return;
                 }

                 for( int w = 0; i < inputResult.length(); i++) {
                     auto b = bytes[i];
                     switch(b) {
                         case 32:
                             i++;
                             breakInFor = true;
                          //   result2 = std::to_string(doubleMap.get(result2).num);
                             break;
                         case 34:

                             wIsString2++;

                             break;
                         case 10:


                             break;
                         default:
                             result2 += b;
                             break;
                     }
                     if(breakInFor) break;
                    // result2 += b;

                 }


                 if(result2 == "") {
                     arc::util::Log::warn("Введён неверный аргумент. 48" + result2 + std::to_string(i) + result);
                     return;
                 }
                 if( wIsString2 == 2) {
                     Var v = doubleMap.get(result); v.type = 1; v.str = result2; doubleMap.put(result, v);
                     return;


                 }
                 if(true) {
              double vaw = 0.0;
              auto [ptr, ec] = std::from_chars(result2.data(), result2.data() + result2.size(), vaw);


              if( (ec == std::errc{}) && (ptr == result2.data() + result2.size())) {
                  if(result == "@counter") { //Должен быть гибрид хеш карты и массива для эффективности, но пока на условиях.
                       stepCounter = (int)vaw;
                       return;
                  }

                  Var v = doubleMap.get(result); v.type = 0; v.num = vaw; doubleMap.put(result, v);
              } else if(doubleMap.contains(result2)) {
                  Var v = doubleMap.get(result2);
                  if(v.type == 1) {
                      Var v2 = doubleMap.get(result); v2.type = 1; v2.str = v.str; doubleMap.put(result, v2);

                  }
              } else
              arc::util::Log::warn("66 строка ошибка");
              return;
                 }

                 return;
        });
    };
    int start() {

            parser p;
        for(int w = 0; stepCounter < seq.totalSpace; w) {
            if (ipt < 10010) {

                std::string tmpString = "";
                tmpString = seq.get(stepCounter);
                if (tmpString != "") {

                    p.exec(tmpString);
                    if(isWaitingMLog) {


                        return 1;
                    }
                }
                stepCounter++;
                ipt++;
            } else break;
        }
        stepCounter = 0;
        ipt = 0;
        return  0;
    }
    void add(std::string line) {
        if(line != "") {
            seq.add(line);
        }
    }
    void exec(std::string lines) {
        std::string result = "";
        std::string result2 = "";
        const char* bytes = lines.c_str();
        int i = 0;
        int ww = 0;
        for(int l = 0; i < lines.length(); i++) {
            auto b = bytes[i];
            if (b == 32) {
            i++;
            result = lines.substr(0, l);
            break;
            }
            l++;
            result += b;
        }

     if(result == "") {
         arc::util::Log::warn("Введён неверный аргумент." + result);
         return;
     }

     for( int w = 0; i < lines.length(); i++) {

         auto b = bytes[i];


         if (b == 10) {
             break;
         }
         result2 += b;
     }

      auto resultA = OQmap.get(result);
      if(resultA) {
            resultA(result2);
            return;
            } else {
          if(doubleMap.contains(result)) return;
            arc::util::Log::warn("Неизвестная команда: "+result);
            return;
        }
        return;
    };

};


 int main() {

    std::string command = "";
    arc::util::Log::log("Парсер начинает работу, введите код. # чтобы включить выполнение.");
   // std::getline(std::cin, command);
   WWWLineBuffer = new char[1024];
   arc::util::Threads::executor([]() { //Я пытался заставить работать код, но забыл про то, что executor ничего не делал.
      // arc::util::Log::log("Фоновой поток иницилизировался");
       int bytesInTermL = 0;
    std::string line;
   while(true) {
       line = "";
       ioctl(0, FIONREAD, &bytesInTermL);
       if (bytesInTermL > 0) {
           char buffer[1024];
           int input = read(0, buffer, 1023);
           if (input < 0) {
               arc::util::Log::log("input равен" + std::to_string(input));
               input = 0;
           }
           buffer[input] = '\0';
           std::string tmpString(buffer, 0, input - 1);
if(tmpString == "#") {
    isRunning = !isRunning;
}

          if (buffer[input - 1] != 8) {
               line = tmpString;
           } else line = std::string(buffer, 0, input - 2);
       } else line = "";
       if (line != "") {
           std::memcpy(WWWLineBuffer, line.c_str(), line.length());
           SizeWWWLineBuffer = line.length();
       } else SizeWWWLineBuffer = 0;
       sleep(0.01);
   }
});
    std::ios::sync_with_stdio(false);
    std::string line;
    parser p;
    p.init();
    bool running = false;
    int bytesInTerm = 0;
     arc::util::Log::log("<MLog>");
    while(true) {
         while(true) {
            // if(canEditWWWLine1) {
                 std::string tmpString(WWWLineBuffer, SizeWWWLineBuffer);
                 SizeWWWLineBuffer = 0;
             //}

          /*       if (tmpString == "#") {
                     if (running) {
                      running = false;
                    } else running = true;
                     break;
                 } */
                 if (tmpString != "") {

                     if (tmpString == "start") {

                         int code = p.start();
                         if (code == 0)
                        seq.clear();
                        break;
                     }
                    p.add(tmpString);
                 } else if (isRunning) {
                p.start();

                } else
                usleep(0.001);
        }
        arc::util::Log::log("<MLog>");
    }
  return 0;
}


