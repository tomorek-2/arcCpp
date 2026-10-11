#pragma once

#include <string>

namespace arc::util {
    class ColorCodes {
    public:
  static inline  std::string flush = "\033[H\033[2J",
                reset = "\u001B[0m",
                bold = "\u001B[1m",
                italic = "\u001B[3m",
                underline = "\u001B[4m",
                black = "\u001B[30m",
                red = "\u001B[31m",
                green = "\u001B[32m",
                yellow = "\u001B[33m",
                blue = "\u001B[34m",
                purple = "\u001B[35m",
                cyan = "\u001B[36m",
                lightBlack = "\u001b[90m",
                lightRed = "\u001B[91m",
                lightGreen = "\u001B[92m",
                lightYellow = "\u001B[93m",
                lightBlue = "\u001B[94m",
                lightMagenta = "\u001B[95m",
                lightCyan = "\u001B[96m",
                lightWhite = "\u001b[97m",
                white = "\u001B[37m",

                backDefault = "\u001B[49m",
                backRed = "\u001B[41m",
                backGreen = "\u001B[42m",
                backYellow = "\u001B[43m",
                backBlue = "\u001B[44m";


    };



}