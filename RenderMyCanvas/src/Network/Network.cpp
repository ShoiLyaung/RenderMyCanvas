#include "Network.h"
#include <chrono>

RMC::Network::Network()
{
	std::cout << "Network testing" << std::endl;
	ws_client.init_asio();

	ws_client.set_open_handler([this](websocketpp::connection_hdl hdl) {
		//std::cout << "Connected to server!" << std::endl;
		connection_hdl = hdl;  // 存储连接句柄，以便发送消息
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
	std::thread([this]() {
		ws_client.run();  // 在新的线程中运行
		}).detach();  // 使线程分离，允许主线程继续执行

	std::thread([this]() {
		while (1)
		{
			auto start = std::chrono::high_resolution_clock::now();

			// 执行要计时的函数
			if (message_to_send.length()>0)
				send_message(message_to_send);



			// 结束计时
			auto stop = std::chrono::high_resolution_clock::now();

			// 计算耗时
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

			int remaining_sleep_time = 100 - duration;  // 期望总时间为100ms

			// 检查剩余的暂停时间是否为正值
			if (remaining_sleep_time > 0) {
				// 暂停剩余的时间
				std::this_thread::sleep_for(std::chrono::milliseconds(remaining_sleep_time));
			}
		}
		}).detach();  // 使线程分离，允许主线程继续执行

	
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
	else
	{
		recv_data(message);
	}
}

void RMC::Network::start_game(const std::string& message)
{
	std::cout << message << std::endl;
	nlohmann::json jsonObj = nlohmann::json::parse(message);
	std::cout << "json done" << std::endl;
	if (jsonObj.contains("id"))
	{
		std::cout << "Player ID: " << jsonObj["id"] << std::endl;
		game_started = true;
	}
	m_playerID = jsonObj["id"];
}

void RMC::Network::recv_data(const std::string& message)
{
	// 解析 JSON 数据
	if (!jsonObj_lock)
		m_jsonObj = nlohmann::json::parse(message);
}

void RMC::Network::send_data(int frame_idx, std::string player_id, uint32_t x, uint32_t y, uint32_t z)
{
	std::vector<int> pos;
	pos.push_back(x);
	pos.push_back(y);
	pos.push_back(z);

	nlohmann::json jsonData;
	jsonData["frame_idx"] = frame_idx;
	jsonData["id"] = player_id;
	jsonData["pos"] = pos;

	// 输出 JSON 数据 (可以替换为实际发送逻辑)
	message_to_send = jsonData.dump(4);
}
