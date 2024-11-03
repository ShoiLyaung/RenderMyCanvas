#include "Network.h"

void write_callback()
{
}

RMC::Network::Network(std::string name)
{
	ws_curl = curl_easy_init();
    if (ws_curl) {
        curl_easy_setopt(ws_curl, CURLOPT_URL, "ws://shoilyaung.space:25555/ws");
        curl_easy_setopt(ws_curl, CURLOPT_TIMEOUT_MS, 3000);
        curl_easy_setopt(ws_curl, CURLOPT_CONNECT_ONLY, 2L);
        //curl_easy_setopt(ws_curl, CURLOPT_WRITEFUNCTION, write_callback); // 设置回调函数
        CURLcode res = curl_easy_perform(ws_curl);
        std::cout << curl_easy_strerror(res) << std::endl;
        if (res != CURLE_OK) {
            std::cout << "Failed to connect to server: " << curl_easy_strerror(res) << std::endl;
            return;
        }
        std::cout << "Connected to server" << std::endl;
        long sockfd;
        size_t sent;
        name = "{\"name\":\"" + name + "\"}";
        const char *c_name = name.c_str();
        SIZE_T name_length = strlen(c_name), send;
        res = curl_ws_send(ws_curl, c_name, strlen(c_name), &send, 0, CURLWS_TEXT);
        if (res == CURLE_OK)
        {
            size_t rlen;
            const struct curl_ws_frame* meta;
            char buffer[2560] = { 0 };
            CURLcode result = curl_ws_recv(ws_curl, buffer, sizeof(buffer), &rlen, &meta);
            if (result == CURLE_OK)
            {
                printf("%s\n", buffer);
            }
        }
    }
}
