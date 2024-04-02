#include "mcbot.h"
#include <thread>
#include <chrono>

const char* t = "********************";//Discord bot token, replace with own token
MCBot *instance;

int main()
{
    instance = new MCBot(t);

    auto c_socket = socket(AF_INET, SOCK_DGRAM, 0);
    //auto s_socket = socket(AF_INET, SOCK_DGRAM, 0);
    auto status_socket = socket(AF_INET, SOCK_DGRAM, 0);
    auto c_socket_out = socket(AF_INET, SOCK_DGRAM, 0);
    auto s_socket_out = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in creative, survival, creative_out, survival_out, status;
    set_sockaddr(creative, 60009);
    //set_sockaddr(survival, 60010);
    set_sockaddr(status, 60010);
    set_sockaddr(creative_out, 60019);
    set_sockaddr(survival_out, 60020);

    bind(c_socket, (sockaddr*)&creative, sizeof creative);
    bind(status_socket, (sockaddr*)&status, sizeof status);
    //bind(s_socket, (sockaddr*)&survival, sizeof survival);

    char buffer[257] = { 0 };
    ssize_t bytes_read;

    instance->s_socket_out = s_socket_out;
    instance->c_socket_out = c_socket_out;

    while (instance != nullptr) 
    {
        if ((bytes_read = recv(c_socket, buffer, 257, MSG_DONTWAIT)) != -1)
        {
            buffer[256] = '\0';
            instance->str_vec.push_back(buffer + 1);
            instance->msg_callback(buffer[0]);
            instance->str_vec.clear();
            memset(buffer, 0, 257);
        }
        if ((bytes_read = recv(status_socket, buffer, 257, MSG_DONTWAIT)) != -1)
        {
            buffer[256] = '\0';
            instance->str_vec.push_back(buffer + 1);
            instance->status_callback(buffer[0]);
            instance->str_vec.clear();
            memset(buffer, 0, 257);
        }
        /*if ((bytes_read = recv(s_socket, buffer, 256, MSG_DONTWAIT)) != -1)
        {
            buffer[255] = '\0';
            instance->str_vec.push_back(buffer);
            instance->msg_callback(false);
            instance->str_vec.clear();
            memset(buffer, 0, 256);
        }*/
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\nShutting down instance...\n";
    close(c_socket);
    close(status_socket);
    //close(s_socket);
    return 0;
}
