#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>
namespace arc::net {
    class Client {

    public:
        int socketNum = 0;
        int targetPort = 0;
        sockaddr_in* targetAddr = new sockaddr_in;
        std::string* targetIp = new std::string("127.0.0.1");
        Client(int port) {

        }
        void init() {
            socketNum = socket(AF_INET, SOCK_STREAM, 0);
            if(socketNum == -1) {
               throw std::runtime_error("Ошибка создания сокета");
                return;
            }

            targetAddr->sin_family = AF_INET;
            targetAddr->sin_port = htons(targetPort);
            inet_pton(AF_INET, "127.0.0.1", &targetAddr.sin_addr);

        }
        void send() {

        }

void read() {

        }



    };
}