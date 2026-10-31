//Вайбкод.
#include <iostream>
#include <string>
#include <cstring>
#include <charconv>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "arcCpp/util/Log.hpp"

// Тестовый TCP-сервер, который возвращает СТРОГО ТОЛЬКО ЧИСЛО
int main() {
    int port = 6567;
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        arc::util::Log::err("Ошибка создания сокета сервера!");
        return 1;
    }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        arc::util::Log::err("Ошибка bind на порт " + std::to_string(port));
        close(serverFd);
        return 1;
    }

    if (listen(serverFd, 5) < 0) {
        arc::util::Log::err("Ошибка listen!");
        close(serverFd);
        return 1;
    }

    arc::util::Log::info("<Тестовый сервер запущен на порту " + std::to_string(port) + ">");

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        int clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrLen);

        if (clientFd < 0) {
            arc::util::Log::err("Ошибка accept клиента!");
            continue;
        }

        arc::util::Log::info("Клиент успешно подключился!");

        char buffer[1024] = {0};
        ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

        if (bytesRead > 0) {
            std::string receivedStr(buffer, bytesRead);
            arc::util::Log::info("Сервер получил от MlogI запрос: [" + receivedStr + "]");

            // Ищем первое число в полученной строке:
            double number = 0.0;
            bool foundNumber = false;

            for (size_t i = 0; i < receivedStr.length(); ++i) {
                if (std::isdigit(receivedStr[i]) || receivedStr[i] == '-') {
                    auto [ptr, ec] = std::from_chars(receivedStr.data() + i, receivedStr.data() + receivedStr.size(), number);
                    if (ec == std::errc{}) {
                        foundNumber = true;
                        break;
                    }
                }
            }

            // Формируем ответ: ТОЛЬКО ЧИСЛО без лишнего текста!
            std::string response;
            if (foundNumber) {
                response = std::to_string(number);
            } else {
                response = "0"; // Если чисел в запросе не было, возвращаем "0"
            }

            // Отправляем строго только число обратно в MlogI:
            send(clientFd, response.c_str(), response.length(), 0);
            arc::util::Log::info("Сервер отправил ЧИСЛО: " + response);
        }

        close(clientFd);
        arc::util::Log::info("Клиент отключен. Ожидание следующего подключения...");
    }

    close(serverFd);
    return 0;
}
