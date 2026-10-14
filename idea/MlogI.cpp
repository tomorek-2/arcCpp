

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
struct Var {
    int type = 0; //0 Это num, 1 это str. bool это int.
    std::string str = "";
    double num = 0;

};
static  arc::structures::ObjectMap<std::string, std::function<void(const std::string&)>> OQmap;
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

     void init() {

        doubleMap.replace = true;
        InstructOpMap.put("add", [](std::string inputResult) {
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

                    arc::util::Log::warn("Неккоректный аргумент");
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

                        arc::util::Log::warn("Неккоректный аргумент");
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
                        arc::util::Log::warn("Неккоректный аргумент");
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

            double vaw = 0.0;
            double vaw2 = 0.0;
            auto [ptr, ec] = std::from_chars(result2.data(), result2.data() + result2.size(), vaw);
            auto [ptr2, ec2] = std::from_chars(result3.data(), result3.data() + result3.size(), vaw2);

            Var v = doubleMap.get(result);
            if( (ec == std::errc{}) && (ptr == result2.data() + result2.size())) {
                if ((ec2 == std::errc{}) && (ptr2 == result3.data() + result3.size())) {
                    v.num = vaw + vaw2;
                doubleMap.put(result, v);


            }else if (result3 == "true") {
                    v.num = vaw + 1;
                doubleMap.put(result, v);
            } else if (result3 == "false") {

            } else //Это плохая идея делать ветвление здесь, switch case был лучше, но для bool этого достаточно.
                arc::util::Log::warn("Что то случилось в конце add:"+ result + "result2"+result2+ "result3"+ result3 + std::to_string(i));
            } else if(doubleMap.contains(result2)) {
                v = doubleMap.get(result2);
                v.num = vaw + v.num;
                doubleMap.put(result, v);
            } else    arc::util::Log::warn("Что то случилось в конце add:"+ result + "result2"+result2+ "result3"+ result3 + std::to_string(i));
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
            stepCounter = 0;

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
              } else      arc::util::Log::warn("66 строка ошибка");
              return;
                 }

                 return;
        });
    };
    int start() {

            parser p;
        for(int w = 0; stepCounter < seq.totalSpace; w) {
            if (ipt < 10000) {

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

    std::ios::sync_with_stdio(false);
    std::string line;
    parser p;
    p.init();
    bool running = false;
    int bytesInTerm = 0;

    while(true) {
         arc::util::Log::log("<MLog>");
         while(true) {
                  line = "";
                  ioctl(0, FIONREAD, &bytesInTerm);
                  if (bytesInTerm > 0) {
                      char buffer[1024];
                      int input = read(0, buffer, 1023);
                      if (input < 0) {
                          arc::util::Log::log("input равен" + std::to_string(input));
                          input = 0;
                  }
                  buffer[input] = '\0';
                  std::string tmpString(buffer, 0, input - 1);
                  if (buffer[1] == '#') {
                  if (running) {
                  running = false;
                  } else running = true;
                  } else
                  if(buffer[input - 1] != 8) {
                      line = tmpString;
                 } else line = std::string(buffer, 0, input - 2);
                 } else line = "";

                 if (line == "#") {
                     if (running) {
                      running = false;
                    } else running = true;
                     break;
                 }
                 if (line != "") {
                     if (line == "start") {

                         int code = p.start();
                         if (code == 0)
                        seq.clear();
                        break;
                     }
                    p.add(line);
                 } else if (running) {
                p.start();
                }
                usleep(0.001);
        }
    }
  return 0;
}


