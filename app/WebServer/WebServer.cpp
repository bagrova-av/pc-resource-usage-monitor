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
        
        response.set_content(j.dump(), "application/json");
        response.set_header("Access-Control-Allow-Origin", "*");
    });

    server.set_mount_point("/", "./webui");
}

void WebServer::start(const std::string& host, int port)
{
    server.listen(host.c_str(), port);
}