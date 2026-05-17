#include <iostream>

#include "./SystemMonitor/SystemMonitor.h"
#include "./WebServer/WebServer.h"

int main()
{
    SystemMonitor systemMonitor;
    WebServer webServer(systemMonitor);

    std::cout << "Server started at http://localhost:8080" << '\n';
    webServer.start("0.0.0.0", 8080);

    return 0;
}