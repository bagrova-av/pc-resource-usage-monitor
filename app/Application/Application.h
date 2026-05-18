#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "./SystemMonitor/SystemMonitor.h"
#include "./WebServer/WebServer.h"

#include <atomic>

class Application
{
public:
    Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();
    void shutdown();

    static void handleSignal(int signal);

private:
    SystemMonitor systemMonitor;
    WebServer webServer;
    
    static std::atomic<bool> shouldExit;
    static Application* instance;
};

#endif