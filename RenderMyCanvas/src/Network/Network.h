#pragma once
#include "Network/json.h"
#include <iostream>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;

namespace RMC
{
	class Network
	{
	public:
		Network();
		void send_message(const std::string& message);
		void recv_message(const std::string& message);
		void start_game(const std::string& message);
		void recv_data(const std::string& message);
		void send_data(int frame_idx, std::string player_id, uint32_t x, uint32_t y, uint32_t z);

		client ws_client;
		websocketpp::connection_hdl connection_hdl; // ´æ´¢Á¬½Ó¾ä±ú
		bool game_started = false;
		std::string m_playerID;
		nlohmann::json m_jsonObj;
	};
}
