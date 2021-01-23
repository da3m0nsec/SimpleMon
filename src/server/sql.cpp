#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <stdio.h>
#include <stdlib.h>
/*
// just going to input the general details and not the port numbers
struct connection_details
{
    char *server;
    char *user;
    char *password;
    char *database;
};
 
MYSQL* mysql_connection_setup(struct connection_details mysql_details)
{
     // first of all create a mysql instance and initialize the variables within
    MYSQL *connection = mysql_init(NULL);
 
    // connect to the database with the details attached.
    if (!mysql_real_connect(connection,mysql_details.server, mysql_details.user, mysql_details.password, mysql_details.database, 0, NULL, 0)) {
      printf("Conection error : %s\n", mysql_error(connection));
      exit(1);
    }
    return connection;
}
 
MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query)
{
   // send the query to the database
   if (mysql_query(connection, sql_query))
   {
      printf("MySQL query error : %s\n", mysql_error(connection));
      exit(1);
   }
 
   return mysql_use_result(connection);
}
 
int main()
{
  MYSQL *conn;		// the connection
  MYSQL_RES *res;	// the results
  MYSQL_ROW row;	// the results row (line by line)
 
  struct connection_details mysqlD;
  mysqlD.server = "localhost";  // where the mysql database is
  mysqlD.user = "mysqlusername";		// the root user of mysql	
  mysqlD.password = "mysqlpassword"; // the password of the root user in mysql
  mysqlD.database = "mysql";	// the databse to pick
 
  // connect to the mysql database
  conn = mysql_connection_setup(mysqlD);
 
  // assign the results return to the MYSQL_RES pointer
  res = mysql_perform_query(conn, "show tables");
 
  printf("MySQL Tables in mysql database:\n");
  while ((row = mysql_fetch_row(res)) !=NULL)
      printf("%s\n", row[0]);
 
  // clean up the database result set 
  mysql_free_result(res);
  // clean up the database link 
  mysql_close(conn);
 
  return 0;
}

using namespace std;

int main(void)
{
cout << endl;
cout << "Running 'SELECT 'Hello World!' » AS _message'..." << endl;

try {
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  driver = get_driver_instance();
  con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
  con->setSchema("test");

  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT 'Hello World!' AS _message"); // replace with your statement
  while (res->next()) {
    cout << "\t... MySQL replies: ";
    cout << res->getString("_message") << endl;
    cout << "\t... MySQL says it again: ";

    cout << res->getString(1) << endl;
  }
  delete res;
  delete stmt;
  delete con;

} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line "  << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}

cout << endl;

return EXIT_SUCCESS;
}
*/
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
#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "simplemond"
#define EXAMPLE_PASS ""
#define EXAMPLE_DB "SimpleMon_test"
using namespace std;
int main(int argc, const char **argv)
{
	const string url=EXAMPLE_HOST;
	const string user=EXAMPLE_USER;
	const string pass=EXAMPLE_PASS;
	const string database=EXAMPLE_DB;
	sql::Driver* driver = get_driver_instance();
	std::unique_ptr<sql::Connection> con(driver->connect(url, user, pass));
	con->setSchema(database);
  sql::Schema* schema = con->getSchema();
  std::vector<string> tables;
  tables = con.getTa
  return 0;
}