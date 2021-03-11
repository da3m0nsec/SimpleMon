#include <string>
#include <vector>

#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "simplemond"
#define EXAMPLE_PASS ""
#define EXAMPLE_DB "SimpleMon_test"

using StatusVector = std::vector<std::pair<std::string, int>>;

int ingestToSql(StatusVector msg);
int sqlTest();