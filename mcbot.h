#include <dpp/dpp.h>
#include <dpp/fmt/format.h>
#include <vector>
#include <atomic>
#include <string>

void set_sockaddr(sockaddr_in& in, int port);

class MCBot : public dpp::cluster
{
    public:
        MCBot(const std::string& token);
        std::vector<std::string> str_vec;
        std::string c_status;
        std::string s_status;
        void msg_callback(bool creative);
        void status_callback(bool creative);
        int c_socket_out = 0;
        int s_socket_out = 0;
    private:
        bool setup = false;
};