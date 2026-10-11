

#ifndef ARC_UTIL_LOGHADNLER_HPP
#define ARC_UTIL_LOGHADNLER_HPP
#include <iostream>
#include "ColorCodes.hpp"
namespace arc::util {
class LogHandler {
public:
    virtual void log(const std::string_view text) {
  //Думаю, сам подтянется <string>
    std::cerr << text << "\n"<<arc::util::ColorCodes::reset<< std::flush;


    }
    virtual void logLevel(const std::string_view text, int logLevelGlobal, int logLevel){
      if(logLevel <= logLevelGlobal) {
           std::cerr << text << "\n"<<arc::util::ColorCodes::reset<< std::flush;
    }


    }

    virtual ~LogHandler() = default;
};


}





#endif
