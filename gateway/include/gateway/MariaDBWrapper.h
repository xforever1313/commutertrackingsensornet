#ifndef MARIA_DB_WRAPPER_H_
#define MARIA_DB_WRAPPER_H_

#include <string>

#include "gateway/MariaDBInterface.h"

namespace Gateway {

/**
 * \brief Wraps MariaDB so it can be abstracted out and mocked.
 * \warning Ensure mysql_library_init() is called before using this class, and mysql_library_end() is 
 *          called after this class is destoryed.
 */
class MariaDBWrapper : public MariaDBInterface {
    public:
        MariaDBWrapper();

        /**
         * \brief Closes the MySql connection, if there is one.
         */
        ~MariaDBWrapper();

        /**
         * \throws std::runtime_error if it can not connect to the database.
         */
        void mysql_init() override;

        /**
         * \throws std::runtime_error if an error occurs.
         */
        void mysql_real_connect(const char *host,
                                const char *user,
                                const char *password,
                                const char *db,
                                unsigned int port,
                                const char *unix_socket,
                                unsigned long flags) override;

        /**
         * \throws std::runtime_error if an error occurs.
         * \warning Ensure real_connect was called first, or a nullptr will occur
         */
        void mysql_real_query(const std::string &query) override;

        /**
         * \throws std::runtime_error if an error occurs
         * \warning Ensure real_connect was called first, or a nullptr will occur.
         */
        void mysql_commit() override;
 
    private:
        static const std::string COULD_NOT_CONNECT_ERROR;

        MYSQL_t *m_connection;
};

}

#endif

