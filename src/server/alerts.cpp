#include <SimpleMon/server/alerts.h>

#include <algorithm>
#include <iostream>
#include <thread>

AlertManager::AlertManager(int downTime) : allowedDownTime(1)
{
}

void AlertManager::HostReport(const std::string &host)
{

    const std::scoped_lock<std::mutex> lock(reportsMutex);

    std::list<Report>::iterator it =
        std::find_if(reports.begin(), reports.end(), [host](const Report &rep) { return rep.first == host; });

    if (it == reports.end())
    {
        std::cout << host << "generates new report" << std::endl;

        reports.push_back({host, std::chrono::steady_clock::now()});
        return;
    }

    std::cout << host << "reporting again" << std::endl;

    it->second = std::chrono::steady_clock::now();
}

void AlertManager::SystemNotify(const std::string &host)
{

    std::string notificationHeader = std::string("Alert: ") + host + " is down";
    std::string notificationBody =
        "Simplemon: host hasn't reported for " + std::to_string(allowedDownTime.count()) + " minutes.";
}

void AlertManager::CheckingLoop()
{

    using namespace std;

    while (1)
    {
        std::this_thread::sleep_for(chrono::seconds(5));
        const std::scoped_lock<std::mutex> lock(reportsMutex);
        for (std::list<Report>::iterator it = reports.begin(); it != reports.end();)
        {

            auto delay = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - it->second);
            if (delay >= allowedDownTime)
            {
                std::cout << "Notifiying down " << it->first << std::endl;
                std::cout << "Reports size: " << reports.size() << std::endl;
                std::cout << "Delay: " << delay.count() << std::endl;

                SystemNotify(it->first);
                it = reports.erase(it);
            }
            else
            {
                ++it;
            }
        }

        reports.remove_if([this](Report &rep) {
            return allowedDownTime < chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - rep.second);
        });
    }
}
