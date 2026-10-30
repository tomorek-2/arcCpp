#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <functional>
namespace arc::net {
    class Client {

    public:
        int socketNum = 0;
        int targetPort = 0;
        sockaddr_in targetAddr{};
        std::string targetIp =  "127.0.0.1";
        uint8_t buffer[4096];
        Client(int port) {
        targetPort = port;
        }
        void init() {
            socketNum = socket(AF_INET, SOCK_STREAM, 0);
            if(socketNum == -1) {
               throw std::runtime_error("Ошибка создания сокета");
                return;
            }

            targetAddr.sin_family = AF_INET;
            targetAddr.sin_port = htons(targetPort);
            inet_pton(AF_INET, targetIp.c_str(), &targetAddr.sin_addr);

        }
        void send(uint8_t* data, int size) {
            if(socketNum != -1 && socketNum != 0) {
                ::send(socketNum, data, size, 0);

            }
        }

        void read(std::function<void(uint8_t*, int)> callback) {

            if(socketNum != -1 && socketNum != 0) {
                int status =  ::recv(socketNum, buffer, sizeof(buffer), 0);
                callback(buffer, status);
            }
        }



    };
}