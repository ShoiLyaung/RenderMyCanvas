#include "Network.h"

RMC::Network::Network(std::string name)
{
	std::cout << "Network testing" << std::endl;
	ws_client.init_asio();

	ws_client.set_open_handler([this, name](websocketpp::connection_hdl hdl) {
		std::cout << "Connected to server!" << std::endl;
		connection_hdl = hdl;  // 存储连接句柄，以便发送消息

		// 连接成功后发送玩家名称
		send_message(name);
		});

	ws_client.set_message_handler([this](websocketpp::connection_hdl, client::message_ptr msg) {
		// 接收消息时的回调函数
		std::cout << "Received message: " << msg->get_payload() << std::endl;
		recv_message(msg->get_payload());
		});

	ws_client.set_fail_handler([](websocketpp::connection_hdl) {
		std::cout << "Connection failed!" << std::endl;
		});

	ws_client.set_close_handler([](websocketpp::connection_hdl) {
		std::cout << "Connection closed!" << std::endl;
		});

	websocketpp::lib::error_code ec;
	auto con = ws_client.get_connection("ws://shoilyaung.space:25555/ws", ec);
	if (ec) {
		std::cout << "Error: " << ec.message() << std::endl;
		return;
	}

	ws_client.connect(con);
	ws_client.run();
};

// 发送消息
void RMC::Network::send_message(const std::string& message) 
{
	if (!connection_hdl.lock()) {
		std::cout << "No active connection to send message." << std::endl;
		return;
	}

	websocketpp::lib::error_code ec;
	ws_client.send(connection_hdl, message, websocketpp::frame::opcode::text, ec);

	if (ec) {
		std::cout << "Send failed: " << ec.message() << std::endl;
	}
	else {
		std::cout << "Message sent: " << message << std::endl;
	}
}

void RMC::Network::recv_message(const std::string& message)
{
	if (!game_started)
	{
		start_game(message);
	}
}

void RMC::Network::start_game(const std::string& message)
{
	/*
	* {
    "code" : 200, 
    "msg": "game start", 
    "players":
        [
            {
                "id": 0,
                "pos": [1,2,3],
                "weight": 10,
                "alive": true
            },
            {
                "id": 1,
                "pos": [4,5,6],
                "weight": 20,
                "alive": true
            }
        ]
	}
	*/
	nlohmann::json jsonObj = nlohmann::json::parse(message);


}
