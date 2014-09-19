#ifndef MOCK_MARIADB_H_

#include <gmock/gmock.h>
#include <string>
#include <vector>

#include "gateway/MariaDBInterface.h"

namespace Gateway {

struct MYSQL_t {
    MYSQL_t() {

    }

    ~MYSQL_t() {

    }
};

struct MYSQL_RES_t {

};

class MockMariaDB : public MariaDBInterface {
    public:
        MockMariaDB()
        {

        }

        virtual ~MockMariaDB(){}

        MOCK_METHOD0(mysql_init, void());
        MOCK_METHOD7(mysql_real_connect, void(const char *host, const char *user,
                                              const char *password, const char *db,
                                              unsigned int port, const char *unix_socket,
                                              unsigned long flags));

        MOCK_METHOD1(mysql_real_query, void(const std::string &query));
        MOCK_METHOD0(mysql_commit, void());

        const MYSQL_t *const getConnection() const override {
            return &m_connection;
        }

        const MYSQL_t m_connection;
};

class MockMariaDBResult : public MariaDBResultInterface {
    public:
        virtual ~MockMariaDBResult() {}

        MOCK_METHOD0(storeResult, void());
        MOCK_METHOD1(getValuesFromColumn, std::vector<std::string>(const std::string &columnName));
};

}

#endif

