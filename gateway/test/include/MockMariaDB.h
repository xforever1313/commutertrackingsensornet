#ifndef MOCK_MARIADB_H_

#include <gmock/gmock.h>
#include <string>

#include "gateway/MariaDBInterface.h"

struct MYSQL_t {

};

namespace Gateway {

class MockMariaDB : public MariaDBInterface {
    public:
        virtual ~MockMariaDB(){}

        MOCK_METHOD0(mysql_init, void());
        MOCK_METHOD7(mysql_real_connect, void(const char *host, const char *user,
                                              const char *password, const char *db,
                                              unsigned int port, const char *unix_socket,
                                              unsigned long flags));

        MOCK_METHOD1(mysql_real_query, void(const std::string &query));
        MOCK_METHOD0(mysql_commit, void());
};

}

#endif

