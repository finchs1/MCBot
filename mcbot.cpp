#include "mcbot.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <arpa/inet.h>

#define CREATIVE xxxxxxxxxxxxxxxx//replace this and other one with discord channel IDs
#define SURVIVIAL xxxxxxxxxxxxxxxxx

extern MCBot *instance;

void set_sockaddr(sockaddr_in& in, int port)
{
    memset(&in, 0, sizeof (in));
    in.sin_family = AF_INET;
    in.sin_addr.s_addr = INADDR_ANY;
    in.sin_port = htons(port);
}

void quit_bot(int signum)
{
    delete instance;
    instance = nullptr;
}

void MCBot::msg_callback(bool creative)
{
    if (!this->str_vec.empty())
    {
        dpp::message m;
        m.channel_id = creative ? CREATIVE : SURVIVIAL;
        m.set_content(this->str_vec.at(0));
        this->message_create(m);
    }
}

void MCBot::status_callback(bool creative)
{
    if (!this->str_vec.empty())
    {
        std::string tmp_status = creative ? "\nCreative\n===========\n" : "\nSurvival\n===========\n";
        std::string tmp = this->str_vec.at(0);
        int len = tmp.length();
        for (int i = 0; i < len; i++)
        {
            if (tmp[i] == '*') 
            {
                tmp_status += '\n';
                continue;
            }
            tmp_status += tmp[i];
        }
        if (creative)
            this->c_status = tmp_status;
        else 
            this->s_status = tmp_status;

        dpp::presence p(dpp::presence_status::ps_idle, dpp::activity_type::at_watching, this->c_status + this->s_status);
        this->set_presence(p);
    }
}

MCBot::MCBot(const std::string& token) : dpp::cluster(token, 98045 | dpp::intents::i_message_content)
{
    signal(SIGINT, quit_bot);

    sockaddr_in cserver, sserver;
    set_sockaddr(cserver, 60019);
    set_sockaddr(sserver, 60020);
    cserver.sin_addr.s_addr = inet_addr("127.0.0.1");//replace with Minecraft server address
    sserver.sin_addr.s_addr = inet_addr("172.0.0.1");

    this->on_log(dpp::utility::cout_logger());
 
    this->on_ready([this](const dpp::ready_t & event) {
        if (dpp::run_once<struct register_bot_commands>())
        {


        }
    });

    this->on_message_create([cserver, sserver, this](const dpp::message_create_t& event)
    {
        if (event.msg.channel_id == CREATIVE && event.msg.author.id != xxxxxxxxxxxxxxxx)//replace with discord bot ID, TODO: change to bot's own id with API
        {
            std::string msg = event.msg.member.nickname + ": " + event.msg.content;

            sendto(this->c_socket_out, msg.c_str(), msg.length() > 256 ? 256 : msg.length(), 
            MSG_DONTWAIT, (sockaddr*)&cserver, sizeof cserver);
        }
        else if (event.msg.channel_id == SURVIVIAL && event.msg.author.id != xxxxxxxxxxxxxxxx)//replace with discord bot ID
        {
            std::string msg = event.msg.member.nickname + ": " + event.msg.content;

            sendto(this->s_socket_out, msg.c_str(), msg.length() > 256 ? 256 : msg.length(), 
            MSG_DONTWAIT, (sockaddr*)&sserver, sizeof sserver);
        }
    });
    
    this->start(true);
}
