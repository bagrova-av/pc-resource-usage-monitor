#include "SystemMonitor.h"

#include <fstream>
#include <sstream>

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
    stats.memUsagePercentage = calculateMemoryLoad();
    stats.cpuUsagePercentage = calculateCpuLoad();
    return stats;
}

float SystemMonitor::calculateMemoryLoad()
{
    std::ifstream memFile("/proc/meminfo");
    if (!memFile.is_open())
    {
        return 0.0;
    }

    std::string line;
    long totalMem = 0;
    long availableMem = 0;

    while (std::getline(memFile, line))
    {
        if (line.find("MemTotal:") == 0 || line.find("MemAvailable:") == 0)
        {
            std::stringstream ss(line);
            std::string label;
            long value;

            ss >> label >> value;
            if (label == "MemTotal:")
            {
                totalMem = value;
            }
            else
            {
                availableMem = value;
            }
        }

        if (totalMem > 0 && availableMem > 0)
        {
            break;
        }
    }

    if (totalMem == 0)
    {
        return 0.0;
    }
    long usedMem = totalMem - availableMem;
    double percentage = (static_cast<float>(usedMem) / totalMem) * 100.0;
    return percentage;
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