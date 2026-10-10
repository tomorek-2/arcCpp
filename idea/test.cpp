#include <SDL2/SDL.h>
#include <iostream>
#include "arcCpp/util/Log.hpp"
#include "arcCpp/util/OS.hpp"
#include "arcCpp/Events.hpp"
#include "arcCpp/util/Threads.hpp"
#include "arcCpp/util/io/Reads.hpp"
class SdlTest {
    SDL_Window* sdlwin;
public:  SDL_Window* Window() {
        sdlwin = SDL_CreateWindow("Singularity Draw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
        return sdlwin;
    }
    ~SdlTest() {
      SDL_DestroyWindow(sdlwin);
    }
};

//Это же свалка идей, скоро я почищу код от решения БЯМ.
int main() {
    // 1. Инициализация
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    // 2. Создаем окно
//    SDL_Window* window = SDL_CreateWindow("Singularity Draw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
SdlTest* sdltest = new SdlTest();
SDL_Window* window = sdltest->Window();

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event event;
int* xqe=new int(0);
int xq = *xqe;
int xqr;
float rq = 0;
int walkX = 0;
int walkY = 0;
int orderChar = 0;
bool orderTwoI = false;
bool orderThreeI = false;
char charTest = 0;
static int numberI = 0;
arc::util::Log::log("Hello, World!");
arc::util::Log::warn("Warning!");
uint8_t* arrayWWWWW = new uint8_t[8]{0, 1, 4, 4, 1, 2, 3, 2};
arc::util::io::Reads reads(arrayWWWWW, 8);

arc::util::Log::logLevel = reads.i();
arc::util::Log::err("   "+std::to_string(arc::util::Log::logLevel) + " " + std::to_string( reads.i()));
arc::util::Log::log("if you see this, logLevel dont work");
arc::util::Log::warn("if you see this, logLevel work");
    arc::util::Log::info(std::string(" \033[91m") + "red");
arc::util::Log::info(arc::util::OS::userHome());
    arc::util::Log::info(arc::util::OS::username());
    arc::util::Log::info(arc::util::OS::getPathUser());
    arc::util::Log::log(arc::util::OS::isLinux() ? "OS: Linux" : arc::util::OS::isUnix() ? "OS: Unix" : "OS: Unknown");
arc::util::Log::info(arc::util::OS::OSVersionArchitecture());
arc::Events::on([](){
    arc::util::Log::info(arc::util::OS::OSVersionArchitecture());
}, "outputArchitecture");
arc::util::Threads::daemon("outputArchitecture", []() {
    arc::Events::fire("outputArchitecture");
});
//arc::util::Log::info(arc::util::OS::isArm() ? "OS: ARM " : arc::util::OS::isX64() ? "OS: X64 " + arc::util::OS::osVersion() : "OS: Unknown");
    // ГЛАВНЫЙ ЦИКЛ (Game Loop)
 /* SDL_Renderer* renderer = SDL_CreateRenderer(
    window, -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
); */

// 3. Создание тестовой текстуры (красный квадрат 50x50)
SDL_Surface* surface  = SDL_LoadBMP("silicon-crucible.bmp");
//SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 255, 0, 0, 255));

SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
SDL_FreeSurface(surface); // Поверхность больше не нужна после создания текстуры

// 4. Задание координат и размеров для вывода текстуры
SDL_Rect dstRect{
    .x = 0, // Координата X
    .y = 150, // Координата Y
    .w = 50,  // Ширина
    .h = 50   // Высота
};
    while (running) {
orderChar++;
if(orderChar > 255) {
    orderChar = 0;
    orderTwoI = !orderTwoI;
}
if(orderTwoI && orderChar > 255) orderThreeI = true;
if(orderTwoI) {
    charTest = 256 + orderChar;
} else if (orderThreeI) {
    charTest = 256 + 256 + orderChar;
} else charTest = orderChar;
        // Проверяем события (чтобы окно не зависло)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { running = false;
            } else if (event.type == SDL_TEXTINPUT) {

                char ch = event.text.text[0];

                char ch1 = event.text.text[1];
               switch(ch) {
                   case 'w':
                       walkY = -1;


                break;
                   case 's':
                       walkY = 1;

                   break;
                   case 'a':
                       walkX = -1;

                       break;
                   case 'd':
                       walkX = 1;

                       break;
default:

    walkX = 0;
    walkY = 0;
    break;
            }



            } else {
                walkX = 0;
                walkY = 0;
            }
        }

        numberI++;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // R, G, B, A
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150); // Красный
      //  SDL_RenderFillRect(renderer, &rect);

        if(xqr ==1) rq+=(255.0/900.0); else rq-=(255.0/900.0);

        if(rq > 255) rq = 0;
if(xqr ==1) xq++; else xq--;
if(xq > 800) xqr = -1;
if(xq < -1) xqr = 1;
dstRect.y += walkY;
dstRect.x += walkX;

SDL_RenderCopy(renderer, texture, nullptr, &dstRect);


        SDL_RenderPresent(renderer);


    }

    // Очистка
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete[] arrayWWWWW;
    delete xqe; // Память освобождена
    xqe = nullptr; // Хороший тон: занулить указатель, чтобы не использовать его случайно
delete sdltest;
    return 0;
}
