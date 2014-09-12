#ifndef MariaDB_INTERFACE_H_
#define MariaDB_INTERFACE_H_

#include <string>

namespace Gateway {

/**
 * \struct MYSQL_t
 * \brief Wraps the MYSQL Object
 */
 struct MYSQL_t;

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
};

}

#endif

