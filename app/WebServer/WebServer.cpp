#include "WebServer.h"

WebServer::WebServer(SystemMonitor& monitor) :
    systemMonitor(monitor)
{
    server.Get("/api/stats", [this](const httplib::Request&, httplib::Response& response) 
    {
        auto stats = systemMonitor.getStats();

        nlohmann::json j;
        j["cpuUsage"] = stats.cpuUsagePercentage;
        j["memUsage"] = stats.memUsagePercentage;

        j["memTotal"] = static_cast<float>(stats.totalMemory) / (1024.0 * 1024.0);
        j["memUsed"] = static_cast<float>(stats.usedMemory) / (1024.0 * 1024.0);
        
        response.set_content(j.dump(), "application/json");
        response.set_header("Access-Control-Allow-Origin", "*");
    });

    server.set_mount_point("/", "./webUI");
}

void WebServer::start(const std::string& host, int port)
{
    server.listen(host.c_str(), port);
}

void WebServer::stop()
{
    server.stop();
}