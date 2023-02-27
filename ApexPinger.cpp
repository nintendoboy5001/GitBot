#include "cpprest/http_client.h"
#include <chrono>
#include <thread>

using namespace web;
using namespace web::http;
using namespace web::http::client;

int main()
{
    std::string bot_token = "OTgxMjc5ODk3Nzk0MjQ4NzQ0.Ga50KZ.mpEbUuKKvjeh9YxUTSTXAtIFr0lMdXo0vGl5Ac";
    std::string channel_id = "1014129537220608075";
    std::string message = "<@686100459727093778> Donator 07 Lost connection!";
    http_client client(U("https://discord.com/api/v9"));
    client.add_default_header(U("Authorization"), U("Bot " + bot_token));

    int ping_attempts = 0;
    bool ip_responding = false;

    while (true)
    {
        try
        {
            for (int i = 0; i < 5; i++)
            {
                http_client client(U("95.217.89.19"));
                client.request(methods::HEAD).wait();
                ping_attempts++;
                ip_responding = true;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        catch (const std::exception&)
        {
            ping_attempts++;
            ip_responding = false;
        }

        if (!ip_responding && ping_attempts == 5)
        {
            http_request request(methods::POST);
            request.set_request_uri(U("/channels/" + channel_id + "/messages"));
            request.headers().add(U("Content-Type"), U("application/json"));
            request.set_body(U("{\"content\":\"" + message + "\"}"));
            client.request(request).wait();
            ping_attempts = 0;
            std::this_thread::sleep_for(std::chrono::minutes(5));
        }
        else
        {
            ping_attempts = 0;
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    }

    return 0;
}
