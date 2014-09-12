#include <mysql/mysql.h>
#include <stdexcept>
#include <string>

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

}

