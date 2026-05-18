#pragma once
#ifndef SYSTEM_MONITOR_H
#define SYSTEM_MONITOR_H

#include <optional>

struct MemoryInfo
{
    long totalMemory;
    long usedMemory;
};

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
    std::optional<MemoryInfo> updateMemoryInfo();

    long lastUserTime;
    long lastNiceTime;
    long lastSystemTime;
    long lastIdleTime;
};

#endif