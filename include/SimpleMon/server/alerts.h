#pragma once

#include <chrono>
#include <string>
//#include <libnotify/notify.h>
//#include <glib.h>
#include <mutex>
#include <list>



using Report = std::pair<std::string, std::chrono::steady_clock::time_point>;
using Minutes = std::chrono::minutes;

class AlertManager{
    Minutes allowedDownTime; 
    std::list<Report> reports;
    std::mutex reportsMutex;

public:
    AlertManager(int downTime);

    

    void HostReport (const std::string& host);
    void SystemNotify (const std::string& host);
    void CheckingLoop ();
};
