#pragma once
#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <string>

struct SystemStats
{
    float cpuUsagePercentage;
    float memUsagePercentage;
    long totalMemory;
    long usedMemory;
};

class SystemMonitor
{
public:
    SystemMonitor();

    SystemStats getStats();

private:
    float calculateCpuLoad();
    void updateMemoryInfo();

    long totalMemoryKb;
    long usedMemoryKb;

    long lastUserTime;
    long lastNiceTime;
    long lastSystemTime;
    long lastIdleTime;
};

#endif