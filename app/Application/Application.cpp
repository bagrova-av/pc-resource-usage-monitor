#include "Application.h"

#include <iostream>
#include <csignal>

Application* Application::instance = nullptr;
std::atomic<bool> Application::shouldExit{false};

Application::Application() : 
    webServer(systemMonitor)
{
    instance = this;
}

void Application::handleSignal(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        std::cout << '\n' << "[App] Shutdown signal received" << '\n';
        shouldExit = true;
        if (instance)
        {
            instance->shutdown();
        }
    }
}

void Application::run()
{
    std::signal(SIGINT, Application::handleSignal);
    std::signal(SIGTERM, Application::handleSignal);

    std::cout << "[App] Server starting on http://localhost:8080" << '\n';
    webServer.start("0.0.0.0", 8080);
}

void Application::shutdown()
{
    std::cout << "[App] Graceful shutdown initiated" << '\n';
    webServer.stop();
}