#pragma once

#include <chrono>
#include <glib.h>
#include <libnotify/notify.h>
#include <list>
#include <mutex>
#include <string>

using Report = std::pair<std::string, std::chrono::steady_clock::time_point>;
using Minutes = std::chrono::minutes;

class AlertManager
{
    Minutes allowedDownTime;
    std::list<Report> reports;
    std::mutex reportsMutex;

  public:
    AlertManager(int downTime);

    void HostReport(const std::string &host);
    void SystemNotify(const std::string &host);
    void CheckingLoop();
};