

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
#include <string>
#include <cstring>
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

static int stepCounter = 0;
static std::chrono::steady_clock::time_point waitMS; //Я тут подумал, чтобы избежать в главном потоке зависаний, нужно для wait логику как в горутинах (я не знаю что такое горутины).
static bool isWaitingMLog = false;

class parser {
  public:

    struct InstructionWith3Arg : public Instruction { //Как нибудь попозже займусь организацией файлов, пока так будет.
        std::string* arg2; std::string* arg3; std::string* code;
        std::size_t* cacheHashCode;
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
        InstructOpMap.put("add", [](std::string &inputResult) {

          poolString.clear();
          poolString2.clear();
          poolString3.clear();

            int i = 0;
            bool breakInFor = false;
            parser parserw;
            std::string tmpStrings[3];
            parserw.parserStrings(inputResult, tmpStrings);
            poolString = tmpStrings[0];
            poolString2 = tmpStrings[1];
            poolString3 = tmpStrings[2];

            if(poolString[0] == 0 && !poolString.empty()) poolString.pop_back();
            if(poolString2[0] == 0 && !poolString2.empty()) poolString2.pop_back();
            if(poolString3[0] == 0 && !poolString3.empty()) poolString3.pop_back();
            double vaw = 0.0;
            double vaw2 = 0.000000;
            auto [ptr, ec] = std::from_chars(poolString2.data(), poolString2.data() + poolString2.size(), vaw);
            auto [ptr2, ec2] = std::from_chars(poolString3.data(), poolString3.data() + poolString3.size(), vaw2);
            // arc::util::Log::log(std::string(inputResult) + ":;" + poolString +  ":;" + poolString2 + ":;" + poolString3 + ":;:" + std::to_string(poolString.length()) + ":" + std::to_string(poolString2.length()) + ":" + std::to_string(poolString3.length()));
            Var v = doubleMap.get(poolString);
            if( (ec == std::errc{}) && (ptr == poolString2.data() + poolString2.size())) {
                if ((ec2 == std::errc{}) && (ptr2 == poolString3.data() + poolString3.size())) {
                    v.num = vaw + vaw2;
                doubleMap.put(poolString, v);


            } else if(doubleMap.contains(poolString3)) {
                    v = doubleMap.get(poolString3);
                    if(v.type == 1) {
                        return;
                    }

                    v.num = vaw + v.num;
                    doubleMap.put(poolString, v);
                } else   {
                    auto v =  doubleMap.get(poolString3);
               v.num = vaw;
                    doubleMap.put(poolString, v);
                }

            } else if(doubleMap.contains(poolString2)) {
                v = doubleMap.get(poolString2);
                if(v.type == 1) {
                    return;
                }

                v.num = ((ec2 == std::errc{}) ? vaw2 : doubleMap.get(poolString3).num) + v.num;
                doubleMap.put(poolString, v);
            } else   {
              auto v =  doubleMap.get(poolString2);  //создание временного объекта.
              v.num += (ec2 == std::errc{}) ? vaw2 : doubleMap.get(poolString3).num; //Идёт сама операция сложения: если poolString3 не смог преобразаваться, то берётся из карты значение.
              doubleMap.put(poolString, v);
            }
        });
        OQmap.put("print", [](std::string inputResult) {
             auto bytes = inputResult.c_str();
             std::string  result2;
             std::string result2A;
             auto b = bytes[0];
             int l = 0;
             bool isString = false;
             for(int i = 0; i < inputResult.length(); i++) {
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

        OQmap.put("end", [](std::string_view result2) {
            stepCounter = -1;

        });



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
        OQmap.put("op", [](std::string &inputResult) {
             //arc::util::Log::info("a0" + std::string(inputResult));
            poolString.clear();
            poolString2.clear();
            std::string_view operat = "";
            std::string_view result = "";
            std::string tmpStrings[3] = {"", "", ""};

            parserStrings(inputResult, tmpStrings);
            poolString = tmpStrings[0];
            poolString5 = tmpStrings[1];
            poolString5 += " ";
            poolString5 += tmpStrings[2];


          auto resultVoid = InstructOpMap.get(poolString);
          if(resultVoid) {
              //arc::util::Log::log("aa" + poolString2);
              resultVoid(poolString5);
              return;
          } else {
              arc::util::Log::warn(
                      "operat was null" + poolString + ":" + std::to_string(poolString.length()) + ":" + std::to_string((int)poolString[0]));
              for(int ww = 0; ww < poolString.length(); ww++) {
                  arc::util::Log::warn("" + std::to_string((int)poolString[ww]));


              }
          }
        });
        OQmap.put("set", [](std::string_view inputResult) {
                 std::string result = "";
                 std::string result2 = "";
                 int i = 0;
                 int16_t wIsString = 0;
                 int16_t wIsString2 = 0;

                 bool breakInFor = false;
                 for(int l = 0; i < inputResult.length(); i++) {

                     auto b = inputResult[i];
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
                     auto b = inputResult[i];
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
    int operatLogic(Instruction* inputResult) { //Этот метод хорошо бы в so перенести, чтобы на лету менять команды.
        //1 это операция сложения.



        switch(*inputResult->type) {
            case 0:
                switch(*inputResult->cacheHashCommand) {
                    case calculateHash("print"): {
                        if(*inputResult->anythingMarker == 0) {
                             auto input = doubleMap.get(*inputResult->argument);
                             if(input.type == 0) {
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
            case 1:
                InstructionWith3Arg* instruct = static_cast<InstructionWith3Arg*>(inputResult);
                switch (*instruct->cacheHashCommand) {
                    case calculateHash("op"):
                        switch(calculateHashRun(*instruct->code)) {
                            case calculateHash("add"): {
                                double vaw = 0.0;
                                double vaw2 = 0.000000;
                                auto [ptr, ec] = std::from_chars((*instruct->arg2).data(),
                                                                 (*instruct->arg2).data() + (*instruct->arg2).size(),
                                                                 vaw);
                                auto [ptr2, ec2] = std::from_chars((*instruct->arg3).data(),
                                                                   (*instruct->arg3).data() + (*instruct->arg3).size(),
                                                                   vaw2);
                                if ((ec == std::errc{}) &&
                                    (ptr == (*instruct->arg2).data() + (*instruct->arg2).size())) {
                                    if ((ec2 == std::errc{}) &&
                                        (ptr2 == (*instruct->arg3).data() + (*instruct->arg3).size())) {
                                        auto val = doubleMap.get((*instruct->argument));
                                        val.type = 0;
                                        val.num = vaw + vaw2;
                                        doubleMap.put((*instruct->argument), val);

                                    } else  {
                                        auto val =doubleMap.get((*instruct->arg3));
                                        val.type = 0; val.num = vaw - val.num;
                                        doubleMap.put((*instruct->argument), val);
                                    }
                                } else  {
                                    auto val = doubleMap.get((*instruct->arg2));
                                    if(val.type == 1) {
                                        return 0;
                                    }

                                    if(ec2 != std::errc{}) {
                                        auto val2 = doubleMap.get((*instruct->arg3));
                                        if(val2.type == 0) vaw2 = val2.num;
                                    }
                                    val.num = val.num + vaw2;
                                    doubleMap.put((*instruct->argument), val);
                                }
                                return 0;
                            }
                                break;
                            case  calculateHash("sub"): {

                                double vaw = 0.0;
                                double vaw2 = 0.000000;
                                auto [ptr, ec] = std::from_chars((*instruct->arg2).data(),
                                                                 (*instruct->arg2).data() + (*instruct->arg2).size(),
                                                                 vaw);
                                auto [ptr2, ec2] = std::from_chars((*instruct->arg3).data(),
                                                                   (*instruct->arg3).data() + (*instruct->arg3).size(),
                                                                   vaw2);
                                if ((ec == std::errc{}) &&
                                    (ptr == (*instruct->arg2).data() + (*instruct->arg2).size())) {
                                    if ((ec2 == std::errc{}) &&
                                        (ptr2 == (*instruct->arg3).data() + (*instruct->arg3).size())) {
                                        auto val = doubleMap.get((*instruct->argument));
                                        val.type = 0;
                                        val.num = vaw - vaw2;
                                        doubleMap.put((*instruct->argument), val);
                                    } else  {
                                        auto val =doubleMap.get((*instruct->arg3));
                                        val.type = 0; val.num = vaw - val.num;
                                        doubleMap.put((*instruct->argument), val);
                                    }
                                } else  {
                                    auto val = doubleMap.get((*instruct->arg2));
                                    if(val.type == 1) {
                                        return 0;
                                    }

                                    if(ec2 != std::errc{}) {
                                        auto val2 = doubleMap.get((*instruct->arg3));
                                        if(val2.type == 0) vaw2 = val2.num;
                                    }
                                    val.num = val.num - vaw2;
                                    doubleMap.put((*instruct->argument), val);
                                }
                                return 0;
                            }
                                break;

                        }
                        break;

                }
                break;
        }

        return 0;

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
                    seq.add(instu);
                    break;
                }
                case calculateHash("mul"): {
                    InstructionWith3Arg *instu = helpMetodForCompileFunc(firstArgument);
                    *instu->command = "op";
                    *instu->code = "mul";
                    *instu->cacheHashCommand = calculateHashRun(*instu->command);
                    *instu->cacheHashCode = calculateHashRun(*instu->code);
                    seq.add(instu);
                    break;
                }
                case calculateHash("sub"): {
                    InstructionWith3Arg *instu = helpMetodForCompileFunc(firstArgument);
                    *instu->command = "op";
                    *instu->code = "sub";
                    *instu->cacheHashCommand = calculateHashRun(*instu->command);
                    *instu->cacheHashCode = calculateHashRun(*instu->code);
                    seq.add(instu);
                    break;
                }
                case calculateHash("div"): {
                    InstructionWith3Arg *instu = helpMetodForCompileFunc(firstArgument);
                    *instu->command = "op";
                    *instu->code = "div";
                    *instu->cacheHashCommand = calculateHashRun(*instu->command);
                    *instu->cacheHashCode = calculateHashRun(*instu->code);
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
                                 }
                                 exitFor = true;
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

        }

    }
    void exec(Instruction* instruct) {
        poolString.clear();
        poolString2.clear();
        poolString3.clear();
        poolString4.clear();
        poolString5.clear();

             if(   operatLogic(instruct) == 0) return;


       /* poolString.clear();
        poolString2.clear();
        poolString3.clear();
        poolString4.clear();
        poolString5.clear();
        const char* bytes = lines.c_str();
        int i = 0;
        int ww = 0;
        for(int l = 0; i < lines.length(); i++) {
            auto b = bytes[i];
            if (b == 32) {
            i++;
            poolString  = lines.substr(0, l);
            break;
            }
            l++;
            poolString += b;
        }

     if(poolString == "") {
         arc::util::Log::warn("Введён неверный аргумент." + poolString);
         return;
     }

     for( int w = 0; i < lines.length(); i++) {

         auto b = bytes[i];



         if (b == 10) {
             break;
         }
         poolString4 += b;
     }

      auto resultA = OQmap.get(poolString);
      if(resultA) {
         // arc::util::Log::log(" " + poolString + " " + poolString2);
            resultA(poolString4);
            return;
            } else {
        //  if(doubleMap.contains(poolString)) return;
            arc::util::Log::warn("Неизвестная команда: "+poolString + "/Size" + std::to_string(poolString.length()));
            return;
        }
        return; */
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


