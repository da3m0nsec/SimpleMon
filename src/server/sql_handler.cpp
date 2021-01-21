#include <SimpleMon/server/sql_handler.h>

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


using namespace std;

int sqlTest()
{
    const string url = EXAMPLE_HOST;
    const string user = EXAMPLE_USER;
    const string pass = EXAMPLE_PASS;
    const string database = EXAMPLE_DB;
    /*
	sql::Driver* driver = get_driver_instance();
	std::unique_ptr<sql::Connection> con(driver->connect(url, user, pass));
	con->setSchema(database);
    sql::Schema* schema = con->getSchema();
    std::vector<string> tables;
    tables = con.getTa
    */
   // https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-examples-complete-example-2.html
    cout << endl;
    cout << "Let's have MySQL count from 10 to 1..." << endl;

    try
    {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmt;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "simplemond", "pass");
        /* Connect to the MySQL test database */
        con->setSchema("SimpleMon_test");

        stmt = con->createStatement();
        //stmt->execute("DROP TABLE IF EXISTS test");
        stmt->execute("CREATE TABLE IF NOT EXISTS test(id INT, ram INT, disk INT, date DATE, time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP, PRIMARY KEY (id, time))");
        delete stmt;

        /* '?' is the supported placeholder syntax */
        pstmt = con->prepareStatement("INSERT INTO test(id, ram, disk) VALUES (?, ? ,?)");
        for (int i = 1; i <= 10; i++)
        {
            pstmt->setInt(1, i);
            pstmt->setInt(2, rand()%100);
            pstmt->setInt(3, rand()%1024);
            pstmt->executeUpdate();
        }
        delete pstmt;

        /* Select in ascending order */
        pstmt = con->prepareStatement("SELECT id, ram, disk, time FROM test ORDER BY id ASC");
        res = pstmt->executeQuery();

        /* Fetch in reverse = descending order! */
        res->afterLast();
        while (res->previous())
            cout << "\t... PC stats: " <<  res->getString("time") << res->getInt("id") << res->getInt("ram") << res->getInt("disk") << endl;
        
        delete res;
        delete pstmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    cout << endl;

    return EXIT_SUCCESS;
    return 0;
}