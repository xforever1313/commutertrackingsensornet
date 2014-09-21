#include <mysql/mysql.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "gateway/MariaDBInterface.h"
#include "gateway/MariaDBWrapper.h"

namespace Gateway {

struct MYSQL_t {
    public:
        MYSQL_t() :
            m_mysql(nullptr)
        {

        }

       ~MYSQL_t() {
        }

        MYSQL *m_mysql;
};

struct MYSQL_RES_t {
    public:
        MYSQL_RES_t() :
            m_mysql_res(nullptr)
        {
        }

        ~MYSQL_RES_t() {
        }

       MYSQL_RES *m_mysql_res; 
};

const std::string MariaDBWrapper::MariaDBResult::INVALID_HEADER = "Not a valid header for this table";

///MariaDBWrapper Functions
const std::string MariaDBWrapper::COULD_NOT_CONNECT_ERROR = "Could not connect to MariaDB";

MariaDBWrapper::MariaDBWrapper() :
    m_connection(new MYSQL_t)
{

}

MariaDBWrapper::~MariaDBWrapper() {
    if (m_connection->m_mysql != nullptr) {
        ::mysql_close(m_connection->m_mysql);
    }
    delete m_connection;
}

void MariaDBWrapper::mysql_init() {
    m_connection->m_mysql = ::mysql_init(nullptr);
    if (m_connection->m_mysql == nullptr) {
        throw std::runtime_error(COULD_NOT_CONNECT_ERROR);
    } 
}

void MariaDBWrapper::mysql_real_connect(const char *host,
                                        const char *user,
                                        const char *password,
                                        const char *db,
                                        unsigned int port,
                                        const char *unix_socket,
                                        unsigned long flags)
{
    if(::mysql_real_connect(m_connection->m_mysql, host, user, password,
                            db, port, unix_socket, flags) == nullptr)
    {
        throw std::runtime_error(::mysql_error(m_connection->m_mysql));
    }
}

void MariaDBWrapper::mysql_real_query(const std::string &query) {
    if (::mysql_real_query(m_connection->m_mysql, query.c_str(), query.size())) {
        throw std::runtime_error(::mysql_error(m_connection->m_mysql));
    }
}

void MariaDBWrapper::mysql_commit() {
    if (::mysql_commit(m_connection->m_mysql)) {
        throw std::runtime_error(::mysql_error(m_connection->m_mysql));
    }
}

const MYSQL_t *const MariaDBWrapper::getConnection() const {
    return m_connection;
}

///MariaDBResult Functions
MariaDBWrapper::MariaDBResult::MariaDBResult(MariaDBInterface *mariaDB) :
    m_connection(mariaDB->getConnection()),
    m_result(new MYSQL_RES_t)
{

}

MariaDBWrapper::MariaDBResult::~MariaDBResult() {
    if (m_result->m_mysql_res != nullptr) {
        mysql_free_result(m_result->m_mysql_res);
    }
    delete m_result;
}

void MariaDBWrapper::MariaDBResult::storeResult() {
    MYSQL_RES *res = mysql_store_result(m_connection->m_mysql);
    if (res == nullptr) {
        throw std::runtime_error(mysql_error(m_connection->m_mysql));
    }
    m_result->m_mysql_res = res;
}

std::vector<std::string> MariaDBWrapper::MariaDBResult::getValuesFromColumn(const std::string &columnName) {
    std::vector<std::string> v;

    MYSQL_FIELD *field;
    mysql_field_seek(m_result->m_mysql_res, 0); //Start from the beginning
    bool found = false;
    size_t index = 0;
    for (size_t i = 0; (field = mysql_fetch_field(m_result->m_mysql_res)) && !found; ++i) {
        if (columnName == field->name) {
            found = true;
            index = i;
        }
    }
    if (!found) {
        throw std::out_of_range(INVALID_HEADER + ": " + columnName);
    }

    mysql_data_seek(m_result->m_mysql_res, 0); //Reset to beginning of rows.

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(m_result->m_mysql_res))) {
        v.push_back(row[index]);  
    }

    return v;
}

}

