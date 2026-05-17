#pragma once
#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <nlohmann/json.hpp>
#include "httplib.h"

#include "../SystemMonitor/SystemMonitor.h"

class WebServer
{
public:
    WebServer(SystemMonitor& monitor);
    void start(const std::string& host, int port);

private:
    httplib::Server server;
    SystemMonitor& systemMonitor;
};

#endif