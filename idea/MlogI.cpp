
#include "arcCpp/util/Log.hpp"
#include "arcCpp/struct/ObjectMap.hpp"
#include "arcCpp/struct/Seq.hpp"
#include "arcCpp/net/Client.hpp"
#include "arcCpp/util/Threads.hpp"
#include "arcCpp/util/io/Reads.hpp"
#include <sstream>
#include <unistd.h>
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
#include <string>
#include <cstring>
#include <poll.h>
/*Зачем я пишу проект на С++? Мне надо расти в IT и как личность, но при этом я могу натворить дел так, что у меня весь Си
 будет вайбкодиться. Поэтому, я не перейду на rust пока.


  */
std::size_t calculateHashRun(std::string_view object) {
    std::uint64_t FnvBasis = 14695981039346656037ULL;
    std::uint64_t FnvPrime = 1099511628211ULL;


    std::uint64_t hash = FnvBasis;
    for (char c : object) {
        hash ^= static_cast<unsigned char>(c);
        hash *= FnvPrime;
    }
    return static_cast<std::size_t>(hash);
}
struct Instruction {
    int* type; // Нужен, чтобы определять тип инструкции.
    std::size_t* cacheHashCommand;
    std::string* command;
    std::string* argument;
    int* anythingMarker;
     Instruction() {

        type = new int(0);
         cacheHashCommand = new std::size_t(0);
        command = new std::string("");
        argument = new std::string("");
        anythingMarker = new int(0);
    }
    virtual ~Instruction() {

     delete type;
        delete command;
        delete argument;
        delete anythingMarker;
    }
};

struct Var {
    int type = 0; //0 Это num, 1 это str. bool это int. 2 ЭтоМаркер?
    std::string str = "";
    double num = 0;

};

constexpr std::size_t calculateHash(std::string_view object) {
constexpr std::uint64_t FnvBasis = 14695981039346656037ULL;
constexpr std::uint64_t FnvPrime = 1099511628211ULL;

std::uint64_t hash = FnvBasis;
for (char c : object) {
hash ^= static_cast<unsigned char>(c);
hash *= FnvPrime;
}
return static_cast<std::size_t>(hash);
}

static  arc::structures::ObjectMap<std::string, std::function<void(std::string&)>> OQmap;
static    std::atomic<bool> isRunning = false;
static std::atomic<bool> canEditWWWLine1{true};
static char* WWWLineBuffer;
static int SizeWWWLineBuffer;
static arc::structures::ObjectMap<std::string, Var> doubleMap;
static arc::structures::ObjectMap<std::string, std::function<void( std::string&)>> InstructOpMap;
static arc::structures::Seq<Instruction*> seq; //Команды.
static arc::structures::Seq<std::string> seqL;
static int ipt = 0;
static  std::string poolString, poolString2, poolString3, poolString4, poolString5;
static arc::net::Client netClient; //не знаю зачем, но пусть будет.
static int stepCounter = 0;
static std::chrono::steady_clock::time_point waitMS; //Я тут подумал, чтобы избежать в главном потоке зависаний, нужно для wait логику как в горутинах (я не знаю что такое горутины).
static bool isWaitingMLog = false;

class parser {
  public:

    struct InstructionWith3Arg : public Instruction { //Как нибудь попозже займусь организацией файлов, пока так будет.
        std::string* arg2; std::string* arg3; std::string* code;
        std::size_t* cacheHashCode;
        double firstArgument, secondArgument, thirdArgument;
       InstructionWith3Arg() {
           cacheHashCode = new std::size_t(0);
           *type = 1;
           arg2 = new std::string;
           arg3 = new std::string;
           code = new std::string;
       }

        ~InstructionWith3Arg() {
            delete arg2;
            delete arg3;
            delete code;
            delete cacheHashCode;
       }
    };
    static void  parserStrings(std::string &inputResult, std::string* output) { //Я добавил этот метод для расширения op.
   // std::string result, result2, result3;
     //arc::util::Log::info(inputResult);
    output[0] = "";
    output[1] = "";
    output[2] = "";
    auto bytes = inputResult.data();
    int i = 0;
    bool breakInFor = false;

    for(int w = 0; i < inputResult.length(); i++) {
        auto charB = bytes[i];
        switch(charB) {
            case 36:
            case 32:

                i++;
                breakInFor = true;
                break;

            case 10:

              //  arc::util::Log::warn("Неккоректный аргумент");
                return;
                break;
            case 0:
             //   arc::util::Log::log("Итерация" + std::to_string(i));
                if(!output[0].empty()) {
                    output[0].pop_back();
                }
                w++;
                continue;
                break;
            default:
                output[0] += charB;
                w++;

                break;
        }
        if(breakInFor) {
            breakInFor = false;
            break;
        }

    }
   // arc::util::Log::log(output[0]);

    for(int w = 0; i < inputResult.length(); i++) {
        auto charB = bytes[i];
        switch (charB) {
            case 0:
                w++;
                break;
            case 36:
            case 32:

                i++;
                breakInFor = true;
                break;
            case 10:

              //  arc::util::Log::warn("Неккоректный аргумент");
             //   output[0] = result;
                return;
                break;
            default:
                output[1] += charB;
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
            case 0:
                w++;
                break;
            case 13:
            case 10:


                return;
                break;
            default:
                output[2] += charB;
                w++;
                break;
        }
        if(breakInFor) {
            breakInFor = false;
            break;
        }
    }
        //arc::util::Log::log("aa0" + output[0] + ";" + output[1] + ";" + output[2]);

     }
     void init() {
        doubleMap.replace = true;





        OQmap.put("wait", [](std::string_view inputResult) {
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



            }
        });

    };
    int start() {

            parser p;
        for(int w = 0; stepCounter < seq.totalSpace; w) {
            if (ipt < 10000) {

                Instruction* tmpInstruct = seq.get(stepCounter);


                    p.exec(tmpInstruct);
                    if(isWaitingMLog) {


                        return 1;
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
            compileFunc(line);
           // seq.add(line);
        }
    }
    int stringToInt(const std::string& str) {
        int result = 0;

        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

        if (ec == std::errc{}) {
            return result;
        }

        return 0;
    }
    int operatLogic(Instruction* inputResult) { //Этот метод хорошо бы в so перенести, чтобы на лету менять команды.




        switch(*inputResult->type) {
            case 0:
                switch(*inputResult->cacheHashCommand) {
                    case calculateHash("print"): {
                        if (*inputResult->anythingMarker == 0) {
                            auto input = doubleMap.get(*inputResult->argument);
                            if (input.type == 0) {
                                arc::util::Log::log(std::to_string(input.num));
                            } else
                                arc::util::Log::log(input.str);
                        } else arc::util::Log::log(*inputResult->argument);


                        break;
                    }
                    case calculateHash("end"): {
                        stepCounter = -1; //Костыль.
                        break;
                    }
                }
                break;
            case 1: {
                InstructionWith3Arg* instruct = static_cast<InstructionWith3Arg*>(inputResult);
                switch (*instruct->cacheHashCommand) {
                    case calculateHash("op"): {
                        switch (*instruct->cacheHashCode) {
                            case calculateHash("add"): {
                                Var val, val2;
                                    if(instruct->secondArgument == 0) {
                                         val = doubleMap.get(*instruct->arg2);
                                    } else { val.num = instruct->secondArgument;}
                                if(instruct->thirdArgument == 0) {
                                    val2 = doubleMap.get(*instruct->arg3);
                                } else { val2.num = instruct->thirdArgument;}
                                    val.num += val2.num;
                                doubleMap.put(*instruct->argument, val);
                                return 0;
                            }
                                break;
                            case calculateHash("sub"): {
                                Var val, val2;
                                if(instruct->secondArgument == 0) {
                                    val = doubleMap.get(*instruct->arg2);
                                } else { val.num = instruct->secondArgument;}
                                if(instruct->thirdArgument == 0) {
                                    val2 = doubleMap.get(*instruct->arg3);
                                } else { val2.num = instruct->thirdArgument;}
                                val.num -= val2.num;
                                doubleMap.put(*instruct->argument, val);
                                return 0;

                            }
                            case calculateHash("div"): {
                                Var val, val2;
                                if(instruct->secondArgument == 0) {
                                    val = doubleMap.get(*instruct->arg2);
                                } else { val.num = instruct->secondArgument;}
                                if(instruct->thirdArgument == 0) {
                                    val2 = doubleMap.get(*instruct->arg3);
                                } else { val2.num = instruct->thirdArgument;}
                                val.num /= val2.num;
                                doubleMap.put(*instruct->argument, val);
                                return 0;

                            }
                            case calculateHash("mul"): {
                                Var val, val2;
                                if(instruct->secondArgument == 0) {
                                    val = doubleMap.get(*instruct->arg2);
                                } else { val.num = instruct->secondArgument;}
                                if(instruct->thirdArgument == 0) {
                                    val2 = doubleMap.get(*instruct->arg3);
                                } else { val2.num = instruct->thirdArgument;}
                                val.num *= val2.num;
                                doubleMap.put(*instruct->argument, val);
                                return 0;

                            }
                            case calculateHash("set"): {


                                auto val = doubleMap.get(*instruct->argument);
                                if (val.type == 0) { val.num = instruct->firstArgument; }
                                else if (val.type == 1) { val.str = *instruct->arg2; }
                                doubleMap.put(*instruct->argument, val);


                                break;
                            }
                                break;

                        }

                        break;
                        case calculateHash("set"): {


                            auto val = doubleMap.get(*instruct->argument);
                            if (val.type == 0) { val.num = instruct->firstArgument; }
                            else if (val.type == 1) { val.str = *instruct->arg2; }
                            doubleMap.put(*instruct->argument, val);


                            break;
                        }
                        case calculateHash("net"): {
                            switch (*instruct->cacheHashCode) {
                                case calculateHash("connect"): {
                                    netClient.targetIp = *instruct->argument;
                                    netClient.targetPort = stringToInt(*instruct->arg2);
                                    netClient.init();
                                    netClient.connect(false);


                                    break;
                                }
                                case calculateHash("send"): {
                                    netClient.send(
                                            reinterpret_cast<const uint8_t *>(instruct->argument->data()),
                                            instruct->argument->size());

                                    break;
                                }
                                case calculateHash("read"): {
                                    netClient.read([&](uint8_t *data, int status){
                                        if(status == -1) {
                                            return;
                                        }
                                        arc::util::io::Reads readDataW(data, status);
                                        auto string = readDataW.str(status);
                                        auto val = doubleMap.get(string);
                                        auto valw = stringToDouble(string);
                                        if(valw == 0.0) { val.type = 1; val.str = string; } else {val.type = 0; val.num = valw;}
                                        doubleMap.put(*instruct->argument, val);

                                    });

                                    break;
                                }
                                case calculateHash("disconnect"): {
                                    netClient.disconnect();


                                    break;
                                }
                            }

                            break;
                        }
                    }
                }
                }
            break;
            default:
                arc::util::Log::info("Команда не найдена");
                break;
        }

        return 0;

    }
    double stringToDouble(const std::string& str) {
        double result = 0.0;


        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);


        if (ec == std::errc{} && ptr == str.data() + str.size()) {
            return result;
        }
        return 0.0;
    }
            InstructionWith3Arg* helpMetodForCompileFunc(std::string inputResult) {
                std::string  firstArgument, secondArgument, thirdArgument;
                std::string tmpStrings[3];
                std::string tmpString;
                this->parserStrings(inputResult, tmpStrings);
                auto* inst = new InstructionWith3Arg();

                *inst->argument = tmpStrings[0];
                *inst->arg2 = tmpStrings[1];
                *inst->arg3 = tmpStrings[2];
                return inst;




         }
    void compileFunc(std::string_view inputResult) {
        //Это должно быть в so: один для компиляции, другой для выполнения.
        bool breakInFor = false;
        auto bytes = inputResult.data();
        std::string tmpStrings[3];
        std::string OpType, OpCode, firstArgument, secondArgument, thirdArgument, outputString, tmpStringWW;
        tmpStringWW = inputResult;
        this->parserStrings(tmpStringWW, tmpStrings);
        OpCode = tmpStrings[0];
        OpType = tmpStrings[1];
        firstArgument = tmpStrings[2];

             switch(calculateHashRun(OpCode)) {
            case calculateHash("op"):
            switch(calculateHashRun(OpType)) {
                case calculateHash("add"): {
                    InstructionWith3Arg *instu = helpMetodForCompileFunc(firstArgument);
                    *instu->command = "op";
                    *instu->code = "add";
                    *instu->cacheHashCommand = calculateHashRun(*instu->command);
                    *instu->cacheHashCode = calculateHashRun(*instu->code);
                    instu->firstArgument = stringToDouble(*instu->argument);
                    instu->secondArgument = stringToDouble(*instu->arg2);
                    instu->thirdArgument = stringToDouble(*instu->arg3);
                    seq.add(instu);
                    break;
                }
                case calculateHash("mul"): {
                    InstructionWith3Arg *instu = helpMetodForCompileFunc(firstArgument);
                    *instu->command = "op";
                    *instu->code = "mul";
                    *instu->cacheHashCommand = calculateHashRun(*instu->command);
                    *instu->cacheHashCode = calculateHashRun(*instu->code);
                    instu->firstArgument = stringToDouble(*instu->argument);
                    instu->secondArgument = stringToDouble(*instu->arg2);
                    instu->thirdArgument = stringToDouble(*instu->arg3);
                    seq.add(instu);
                    break;
                }
                case calculateHash("sub"): {
                    InstructionWith3Arg *instu = helpMetodForCompileFunc(firstArgument);
                    *instu->command = "op";
                    *instu->code = "sub";
                    *instu->cacheHashCommand = calculateHashRun(*instu->command);
                    *instu->cacheHashCode = calculateHashRun(*instu->code);
                    instu->firstArgument = stringToDouble(*instu->argument);
                    instu->secondArgument = stringToDouble(*instu->arg2);
                    instu->thirdArgument = stringToDouble(*instu->arg3);
                    seq.add(instu);
                    break;
                }
                case calculateHash("div"): {
                    InstructionWith3Arg *instu = helpMetodForCompileFunc(firstArgument);
                    *instu->command = "op";
                    *instu->code = "div";
                    *instu->cacheHashCommand = calculateHashRun(*instu->command);
                    *instu->cacheHashCode = calculateHashRun(*instu->code);
                    instu->firstArgument = stringToDouble(*instu->argument);
                    instu->secondArgument = stringToDouble(*instu->arg2);
                    instu->thirdArgument = stringToDouble(*instu->arg3);
                    seq.add(instu);
                    break;
                }
            }
                break;
                 case calculateHash("print"): {
                     auto *inst = new Instruction();
                     *inst->command = "print";
                     *inst->argument = OpType + firstArgument;
                     *inst->cacheHashCommand = calculateHashRun(*inst->command);
                     std::string tmpStringWWW = *inst->argument;
                     std::uint8_t isString = 0;
                     bool exitFor = false;
                     for(int w = 0; w < tmpStringWWW.length(); w++) {
                         switch(tmpStringWWW[w]) {
                             case 34:
                                 isString++;
                                 if(isString == 2) {
                                     *inst->type = 1; //1 означает, что там есть кавычки.
                                     exitFor = true;
                                 }

                                 break;
                         }
                         if(exitFor) break;
                     }
                     seq.add(inst);
                     break;
                 }
                 case calculateHash("end"): {
                     auto  *inst = new Instruction();
                     *inst->command = "end";
                     *inst->cacheHashCommand = calculateHashRun(*inst->command);
                     seq.add(inst);
                     break;
                 }
                 case calculateHash("set"): {
                     InstructionWith3Arg *instu = new InstructionWith3Arg();
                     *instu->argument = OpType;
                     *instu->arg2 = firstArgument;
                     *instu->command = "set";
                     *instu->cacheHashCommand = calculateHashRun(*instu->command);
                     instu->firstArgument = stringToDouble(*instu->arg2);
                     seq.add(instu);
                 }

                     case calculateHash("net"): {
                         switch(calculateHashRun(OpType)) {
                               case calculateHash("connect"): {
                                   auto *instru = helpMetodForCompileFunc(firstArgument);
                                   *instru->command = "net";
                                   *instru->code = "connect";
                                   *instru->cacheHashCommand = calculateHashRun(*instru->command);
                                   *instru->cacheHashCode = calculateHashRun(*instru->code);
                                   seq.add(instru);
                                   break;
                               }
                               case calculateHash("send"): {
                                   auto *instru = helpMetodForCompileFunc(firstArgument);
                                   *instru->command = "net";
                                   *instru->code = "send";
                                   *instru->cacheHashCommand = calculateHashRun(*instru->command);
                                   *instru->cacheHashCode = calculateHashRun(*instru->code);
                                   seq.add(instru);
                                   break;
                               }
                                   case calculateHash("read"): {
                                       auto *instru = helpMetodForCompileFunc(firstArgument);
                                       *instru->command = "net";
                                       *instru->code = "read";
                                       *instru->cacheHashCommand = calculateHashRun(*instru->command);
                                       *instru->cacheHashCode = calculateHashRun(*instru->code);
                                       seq.add(instru);
                                       break;
                                   }
                                    case calculateHash("disconnect"): {
                                            auto *instru = helpMetodForCompileFunc(firstArgument);
                                           *instru->command = "net";
                                           *instru->code = "disconnect";
                                           *instru->cacheHashCommand = calculateHashRun(*instru->command);
                                           *instru->cacheHashCode = calculateHashRun(*instru->code);
                                           seq.add(instru);
                                           break;
                             }
                         }
                         break;
                 }
                 default:
                     arc::util::Log::warn("Неизвестная команда: " + OpType);
                     break;
        }

    }
    void exec(Instruction* instruct) {
        poolString.clear();
        poolString2.clear();
        poolString3.clear();
        poolString4.clear();
        poolString5.clear();
             if(   operatLogic(instruct) == 0) return;

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
       pollfd pfd;
       pfd.fd = STDIN_FILENO;
       pfd.events = POLLIN;

       while(true) {
       line = "";
       int tmp =  poll(&pfd, 1, -1);

       ioctl(0, FIONREAD, &bytesInTermL);
       if (tmp > 0) {
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
       usleep(1000);
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

                 std::string tmpString(WWWLineBuffer, SizeWWWLineBuffer);
                 SizeWWWLineBuffer = 0;


                 if (tmpString != "") {

                     if (tmpString == "start") {

                              int code = p.start();
                              if (code == 0)
                              seq.clear();
                               break;
                          }
                     if(tmpString == "exit") return 0;
                      p.add(tmpString);
                      } else if (isRunning) {
                   p.start();

                 } else
                   usleep(1000);
         }
         arc::util::Log::log("<MLog>");
        }
     return 0;
    }


