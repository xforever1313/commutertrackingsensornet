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

        class MariaDBResult : public MariaDBResultInterface {
            public:
                /**
                 * \warning ensure the passed in mariaDB does not
                 *          get deleted while this object is alive
                 */
                MariaDBResult(MariaDBInterface *mariaDB);

                /**
                * \brief Destorys the result, if storeResult() was called.
                */
                ~MariaDBResult();

                /**
                 * \brief Stores the result from the last quiery.
                 * \throw std::runtime_error if an error occurs.
                 */
                void storeResult() override;

                /**
                 * \brief Frees the result if storeResult() was called.
                 */
                void freeResult() override;

                /**
                 * \brief retreives the entire column of the result
                 * \param columnName The column name
                 * \throw std::out_of_range if column name does not exist
                 */
                std::vector<std::string> getValuesFromColumn(const std::string &columnName) override;

            private:
                static const std::string INVALID_HEADER;

                const MYSQL_t *const m_connection;
                MYSQL_RES_t *m_result;
        };
 
    private:
        static const std::string COULD_NOT_CONNECT_ERROR;

        const MYSQL_t *const getConnection() const override;

        MYSQL_t *m_connection;
};

}

#endif

