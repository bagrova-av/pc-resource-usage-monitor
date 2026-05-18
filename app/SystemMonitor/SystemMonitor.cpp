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
    totalMemoryKb(0),
    usedMemoryKb(0),
    lastUserTime(0),
    lastNiceTime(0),
    lastSystemTime(0),
    lastIdleTime(0)
{ }

SystemStats SystemMonitor::getStats()
{
    SystemStats stats;
    updateMemoryInfo();
    stats.totalMemory = totalMemoryKb;
    stats.usedMemory = usedMemoryKb;
    if (stats.totalMemory > 0)
    {
        stats.memUsagePercentage = (static_cast<float>(stats.usedMemory) / stats.totalMemory) * 100;
    }

    stats.cpuUsagePercentage = calculateCpuLoad();
    return stats;
}

 void SystemMonitor::updateMemoryInfo()
{
    std::ifstream memFile("/proc/meminfo");
    std::string line;

    if (!memFile.is_open())
    {
        std::cerr << "Error: Could not open /proc/meminfo" << std::endl;
        return;
    }

    while (std::getline(memFile, line))
    {
        std::stringstream ss(line);
        std::string label;
        long value;

        if (line.find("MemTotal:") == 0)
        {
            ss >> label >> value;
            this->totalMemoryKb = value;
        }
        else if (line.find("MemAvailable:") == 0)
        {
            ss >> label >> value;
            long availableMemoryKb = value;
            this->usedMemoryKb = this->totalMemoryKb - availableMemoryKb;
        }
    }
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