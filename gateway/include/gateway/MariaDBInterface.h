#ifndef MariaDB_INTERFACE_H_
#define MariaDB_INTERFACE_H_

#include <string>
#include <vector>

namespace Gateway {

/**
 * \struct MYSQL_t
 * \brief Wraps the MYSQL Object
 */
 struct MYSQL_t;

/**
 * \struct MYSQL_RES_t
 * \brief Wraps a MYSQL_RES object
 */
struct MYSQL_RES_t;

/**
 * \class
 * \brief Interface so that MariaDB function calls can be abstracted out
 *        and mocked.
 */
class MariaDBInterface {
    public:
        virtual ~MariaDBInterface() {}

        virtual void mysql_init() = 0;
        
        virtual void mysql_real_connect(const char *host,
                                        const char *user,
                                        const char *password,
                                        const char *db,
                                        unsigned int port,
                                        const char *unix_socket,
                                        unsigned long flags) = 0;
        virtual void mysql_real_query(const std::string &query) = 0;

        virtual void mysql_commit() = 0;

        /**
         * \warning Don't be stupid and call delete on this pointer,
         *          it will break things.
         */
        virtual const MYSQL_t *const getConnection() const = 0;
};


/**
 * \class MysqlResult
 * \brief Wraps the MySql result
 */
class MariaDBResultInterface {
    public:
        virtual ~MariaDBResultInterface(){}
        
        virtual void storeResult() = 0;
        virtual std::vector<std::string> getValuesFromColumn(const std::string &columnName) =  0;
};

}

#endif

