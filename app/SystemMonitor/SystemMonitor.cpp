#include "SystemMonitor.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <string>
#include <vector>

struct RawMemData
{
    long total = 0;
    long available = 0;
};

SystemMonitor::SystemMonitor() :
    lastUserTime(0),
    lastNiceTime(0),
    lastSystemTime(0),
    lastIdleTime(0)
{ }

SystemStats SystemMonitor::getStats()
{
    SystemStats stats;
    auto memoryData = updateMemoryInfo();
    if (memoryData.has_value())
    {
        stats.totalMemory = memoryData.value().totalMemory;
        stats.usedMemory = memoryData.value().usedMemory;

        if (stats.totalMemory > 0)
        {
            stats.memUsagePercentage = (static_cast<float>(stats.usedMemory) / stats.totalMemory) * 100.0f;
        }
    }
    else
    {
        std::cerr << "[Monitor] Failed to fetch memory info" << std::endl;
    }

    stats.cpuUsagePercentage = calculateCpuLoad();
    return stats;
}

std::optional<MemoryInfo> SystemMonitor::updateMemoryInfo()
{
    std::ifstream memoryFile("/proc/meminfo");
    if (!memoryFile.is_open())
    {
        return std::nullopt;
    }

    std::string line;
    long totalMemory = 0;
    long availableMemory = 0;
    int foundCount = 0;
    while (std::getline(memoryFile, line))
    {
        std::stringstream ss(line);
        std::string label;
        long value;

        if (line.find("MemTotal:") == 0)
        {
            ss >> label >> value;
            totalMemory = value;
            foundCount++;
        }
        else if (line.find("MemAvailable:") == 0)
        {
            ss >> label >> value;
            availableMemory = value;
            foundCount++;
        }

        if (foundCount == 2)
        {
            break;
        }
    }

    if (foundCount < 2)
    {
        return std::nullopt;
    }
    return MemoryInfo{totalMemory, totalMemory - availableMemory};
}

float SystemMonitor::calculateCpuLoad()
{
    std::ifstream statFile("/proc/stat");
    std::string line;
    if (!std::getline(statFile, line)) 
    {
        return 0.0;
    }

    std::stringstream ss(line);
    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    ss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long currentIdleTime = idle + iowait;
    long currentActiveTime = user + nice + system + irq + softirq + steal;
    long currentTotalTime = currentIdleTime + currentActiveTime;

    static long lastTotalTime = 0;
    static long lastIdleTime = 0;

    long totalDifference = currentTotalTime - lastTotalTime;
    long idleDifference = currentIdleTime - lastIdleTime;
    lastTotalTime = currentTotalTime;
    lastIdleTime = currentIdleTime;

    if (totalDifference == 0)
    {
        return 0.0;
    }

    return 100.0 * (static_cast<float>(totalDifference - idleDifference) / totalDifference);
}