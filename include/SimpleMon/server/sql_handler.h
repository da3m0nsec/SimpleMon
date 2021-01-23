#include <vector>
#include <string>

#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "simplemond"
#define EXAMPLE_PASS ""
#define EXAMPLE_DB "SimpleMon_test"

using StatusVector = std::vector<std::pair<std::string, int>>;

int ingestToSql (StatusVector msg);
int sqlTest();

/*
time_t String2time_t(const string& strDateTime){
    tm t;
    strptime(strDateTime.c_str(), "%F %T", &t);
    return mktime(&t); 
}
*/