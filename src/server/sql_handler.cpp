#include <SimpleMon/server/sql_handler.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <vector>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

void ingestToSql(StatusVector msg)
{
    try
    {
        SQL_Config conf = parse_sql_config("/etc/simplemon-server/config/sql.conf");

        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::PreparedStatement *pstmt;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "simplemond", conf.password);
        /* Connect to the MySQL test database */
        con->setSchema("SimpleMon");

        stmt = con->createStatement();
        stmt->execute("CREATE TABLE IF NOT EXISTS monitor(hostname CHAR(32), uid INT, free_mem INT, free_disk INT, "
                      "used_cpu INT, date DATE, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, "
                      "PRIMARY KEY (uid, time))");
        delete stmt;

        /* '?' is the supported placeholder syntax */
        pstmt = con->prepareStatement(
            "INSERT INTO monitor(hostname, uid, free_mem, free_disk, used_cpu) VALUES (?, ?, ?, ?, ?)");
        pstmt->setString(1, msg.at(0).first);
        for (int i = 1; i < msg.size(); i++)
        {
            pstmt->setInt(i + 1, msg.at(i).second);
        }
        pstmt->executeUpdate();
        delete pstmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "# ERR: SQLException in " << __FILE__;
        std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
        std::cerr << "# ERR: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
    }
}

SQL_Config parse_sql_config(std::string filename)
{
    SQL_Config conf;
    std::ifstream cFile(filename);
    if (cFile.is_open())
    {
        std::string line;
        while (getline(cFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if (line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            // Parse options
            if (name == "ip_address")
            {
                conf.ip_address = value;
            }

            else if (name == "port")
            {
                conf.port = stoi(value);
            }
            else if (name == "logs")
            {
                conf.logs = value;
            }
            else if (name == "password")
            {
                conf.password = value;
            }
        }
    }
    else
    {
        std::cerr << "Couldn't open " + filename + " for reading.\n";
    }
    return conf;
}