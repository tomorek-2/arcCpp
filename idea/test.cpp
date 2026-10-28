#include <SDL2/SDL.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "arcCpp/util/Log.hpp"
#include "arcCpp/util/OS.hpp"
#include "arcCpp/Events.hpp"
#include "arcCpp/util/Threads.hpp"
#include "arcCpp/util/io/Reads.hpp"
#include "arcCpp/util/io/Writes.hpp"
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

arc::util::io::Writes writes(arrayWWWWW, 8);
    delete[] arrayWWWWW;
    writes.i(6);
    arrayWWWWW = writes.toBytes();
    arc::util::io::Reads reads(arrayWWWWW, 8);
arc::util::Log::logLevel = reads.i();

arc::util::io::Writes writes2(arrayWWWWW, 8);
    writes2.str("wasd");
 //   delete[] arrayWWWWW;
    arrayWWWWW = writes2.toBytes();
    arc::util::io::Reads reads2(arrayWWWWW, 8);
    arc::util::Log::log(reads2.str(8));


    arc::util::Log::err("   "+std::to_string(arc::util::Log::logLevel) + " " + std::to_string( reads.i()));
    arc::util::Log::log("if you see this, logLevel dont work");
    arc::util::Log::warn("if you see this, logLevel work");
    int numSocketFile = open("/dev/shm/testWWW", O_RDONLY);


    if(numSocketFile == -1) {
        arc::util::Log::log(" WA");

    } else if(numSocketFile != -1) {
    if(  read(numSocketFile, arrayWWWWW, 8) <= 4) {

      arc::util::io::Reads reads3(arrayWWWWW, 8);
      arc::util::Log::log(" " + std::to_string(reads3.i()));
    }

    }
    close(numSocketFile);
    arc::util::Log::info(std::string(" \033[91m") + "red");
    arc::util::Log::info(arc::util::OS::userHome());
    arc::util::Log::info(arc::util::OS::username());
    arc::util::Log::info(arc::util::OS::getPathUser());
    arc::util::Log::log(arc::util::OS::isLinux() ? "OS: Linux" : arc::util::OS::isUnix() ? "OS: Unix" : "OS: Unknown");
    arc::util::Log::info(arc::util::OS::OSVersionArchitecture());
    arc::Events::on([](){
        arc::util::Log::info(arc::util::OS::OSVersionArchitecture() + "   1");
    }, "outputArchitecture");
        arc::Events::on([](){
            arc::util::Log::info(arc::util::OS::OSVersionArchitecture() + "   2");
        }, "outputArchitecture");
    arc::util::Threads::daemon("outputArchitecture", []() {
        arc::Events::fire("outputArchitecture");
        //  arc::Events::fire("outputArchitecture2");
    });

    SDL_Surface* surface  = SDL_LoadBMP("map.bmp");
    SDL_Surface* surface2  = SDL_LoadBMP("architect.bmp");
    SDL_Surface* surface3  = SDL_LoadBMP("energyTexture.bmp");
    SDL_Surface* surfaceFabric  = SDL_LoadBMP("silicon-crucible.bmp");
    SDL_Surface* surfacePlanB  = SDL_LoadBMP("planB=base.bmp");
        //SDL_FillRect(surface, nullptr, SDL_MapRGBA(surface->format, 255, 0, 0, 255));
    Uint32 blackKey = SDL_MapRGB(surfacePlanB->format, 0, 0, 0);
    SDL_SetColorKey(surfacePlanB, SDL_TRUE, blackKey);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
    SDL_Texture* textureFabric = SDL_CreateTextureFromSurface(renderer, surfaceFabric);
    SDL_Texture* texturePlanB = SDL_CreateTextureFromSurface(renderer, surfacePlanB);
    SDL_FreeSurface(surface); // Поверхность больше не нужна после создания текстуры
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_FreeSurface(surfaceFabric);
    SDL_FreeSurface(surfacePlanB);
    // 4. Задание координат и размеров для вывода текстуры
    SDL_Rect dstRect{
        .x = walkX, // Координата X
        .y = walkY, // Координата Y
        .w = 909,  // Ширина
        .h = 677   // Высота
    };

    SDL_Rect dstRect2{
            .x = 0, // Координата X
            .y = 0, // Координата Y
            .w = 50,  // Ширина
            .h = 50   // Высота
    };
    SDL_Rect dstRect3{
            .x = 0, // Координата X
            .y = 0, // Координата Y
            .w = 71,  // Ширина
            .h = 112   // Высота
    };
    SDL_Rect dstRectFabric{
            .x = 0, // Координата X
            .y = 48, // Координата Y
            .w = 48,  // Ширина
            .h = 48   // Высота
    };
    SDL_Rect dstRectPlanB{
            .x = 0, // Координата X
            .y = 0, // Координата Y
            .w = 200,  // Ширина
            .h = 200   // Высота
    };
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            }
            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.scancode) {
                    case SDL_SCANCODE_W:
                        walkY -= 1;
                        break;
                    case SDL_SCANCODE_S:
                        walkY += 1;
                        break;
                    case SDL_SCANCODE_A:
                        walkX -= 1;
                        break;
                    case SDL_SCANCODE_D:
                        walkX += 1;
                        break;
                }

            }

            }


        dstRect.x = 0 - walkX;
        dstRect.y = 0 - walkY;
        dstRect2.x = 400 - walkX;
        dstRect2.y = 400 - walkY;
        dstRect3.x = 230 - walkX;
        dstRect3.y = 215 - walkY;
        dstRectFabric.x = 430 - walkX;
        dstRectFabric.y = 100 - walkY;
        dstRectPlanB.x = 0 - walkX;
        dstRectPlanB.y = 170 - walkY;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // R, G, B, A
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150); // Красный

        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
        SDL_RenderCopy(renderer, texture2, nullptr, &dstRect2);
        SDL_RenderCopy(renderer, texture3, nullptr, &dstRect3);
        SDL_RenderCopy(renderer, textureFabric, nullptr, &dstRectFabric);
        SDL_RenderCopy(renderer, texturePlanB, nullptr, &dstRectPlanB);
        SDL_RenderPresent(renderer);


    }

    // Очистка
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    arc::Events::clear();
   // delete[] arrayWWWWW;
    delete xqe; // Память освобождена
    xqe = nullptr; // Хороший тон: занулить указатель, чтобы не использовать его случайно
    delete sdltest;
    return 0;
}
