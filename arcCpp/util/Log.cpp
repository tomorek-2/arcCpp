#include "Log.hpp"
#include <iostream>
#include "LogHandler.hpp"
//Что же, переписать Anuken/Arc будет не просто, но для меня будет большой опыт, если я не навайбкодю решение.
namespace arc::util {

    int Log::logLevel = 5; //Логика: 1. err. 2. warn. 3. debug. 4. info. 5. none. //0. Отключить деревья (log) //Как я мог допустить вайбкод, или я так шутил
arc::util::LogHandler logger;
    void Log::log(const std::string &str) {

        logger.logLevel(str, arc::util::Log::logLevel, 5);

    }
        void Log::warn(const std::string &str) {

              logger.logLevel(str, arc::util::Log::logLevel, 2);


        }

    void Log::err(const std::string_view &str) {

        logger.logLevel(str, arc::util::Log::logLevel, 1);


    }

    void Log::debug(const std::string &str) {

        logger.logLevel(str, arc::util::Log::logLevel, 3);

    }

    void Log::info(const std::string_view &str) {


            logger.logLevel(str, arc::util::Log::logLevel, 4);




    }

}
