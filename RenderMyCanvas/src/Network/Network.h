#pragma once
#include "Network/json.h"
#include "curl/curl.h"
#include <iostream>
namespace RMC
{
	class Network
	{
    public:
        Network(std::string name);
        ~Network() = default;
	private:
		CURL *ws_curl;

	};

}
