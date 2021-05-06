#include <SimpleMon/server/alerts.h>

#include <thread>
#include <algorithm>
#include <iostream>

AlertManager::AlertManager(int downTime):
    allowedDownTime(1)
    {}

void AlertManager::HostReport (const std::string& host){

    const std::scoped_lock<std::mutex> lock (reportsMutex);

    std::list<Report>::iterator it = std::find_if (reports.begin(), reports.end(), 
                                            [host](const Report& rep){ return rep.first == host;});

    if (it == reports.end()){
        reports.push_back({host, std::chrono::steady_clock::now()});
        return;
    }

    it->second = std::chrono::steady_clock::now();
}


void AlertManager::SystemNotify(const std::string& host){

    std::string notificationHeader = std::string("Alert: ") + host + " is down";
    std::string notificationBody = "Simplemon: host hasn't reported for " + std::to_string(allowedDownTime.count()) + " minutes.";
/*
    notify_init("Alert");

    NotifyNotification* n = notify_notification_new (notificationHeader.c_str(), 
                                 notificationBody.c_str(),
                                  0);
*/
    std::cout << notificationHeader << std::endl;
}

void AlertManager::CheckingLoop (){

    while(1) {

        for (auto& rep : reports){
            std::chrono::steady_clock::duration delay = std::chrono::steady_clock::now() - rep.second;
            if (delay >= allowedDownTime){
                SystemNotify(rep.first);

            }
        }

        for (std::list<Report>::iterator it = reports.begin(); it!= reports.end();){
            std::chrono::steady_clock::duration delay = std::chrono::steady_clock::now() - it->second;
            if( delay > allowedDownTime ) {
                it = reports.erase( it );
            }
            else {
                ++it ;
            }
        }

        std::this_thread::sleep_for (allowedDownTime/10);
    }


}

