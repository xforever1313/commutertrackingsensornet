/**
 * Please note, this file is a quick and dirty hack.
 * Never write code like this, or god will kill a kitten.
 */

#include <iostream>
#include <mysql/mysql.h>
#include <stdexcept>
#include <string>
#include <vector>

#include <Poco/Exception.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Util/ServerApplication.h>

#include "Secrets.py"

Poco::Net::ServerSocket *theSocket = nullptr;
Poco::Net::HTTPServer *server = nullptr;

MYSQL *con = nullptr;

class NotFoundHTTPRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest &request,
                           Poco::Net::HTTPServerResponse &response) override {
            response.setContentType("text/html");
            response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
            std::ostream &ostr = response.send();
            ostr << "404 - Not Found";
        }
};

class VoltageHTTPRequestHandler : public Poco::Net::HTTPRequestHandler {
    public:
        void handleRequest(Poco::Net::HTTPServerRequest &request,
                           Poco::Net::HTTPServerResponse &response) override {

            std::string resStr = "Success";
            Poco::Net::HTTPResponse::HTTPStatus status = Poco::Net::HTTPResponse::HTTP_OK;
            try {
                Poco::Net::HTMLForm form(request, request.stream());
                const std::string &voltage = form["voltage"];
                std::string query = "INSERT INTO windbelt_result VALUE(NULL, " + voltage + ", NULL);";
                if (mysql_query(con, query.c_str())) {
                    resStr = mysql_error(con);
                    status = Poco::Net::HTTPResponse::HTTP_BAD_REQUEST;
                }
            } 
            catch (const Poco::NotFoundException &e) {
                resStr = "Invalid Form Data";
                status = Poco::Net::HTTPResponse::HTTP_BAD_REQUEST;
            }
            response.setContentType("text/html");
            response.setStatus(status);
            std::ostream &ostr = response.send();
            ostr << resStr;
        }
};

class HTTPRequestFactory : public Poco::Net::HTTPRequestHandlerFactory {
    public:
        Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request) override {
            if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) {
                if (request.get("user-agent", "") == WINDBELT_USER_AGENT) {
                    if (request.getURI() == WINDBELT_URI) {
                        return new VoltageHTTPRequestHandler();
                    }
                }
            }
            return new NotFoundHTTPRequestHandler();
        }
};

void initHTTPServer() {
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams();
    params->setMaxQueued(100);
    params->setMaxThreads(2);

    theSocket = new Poco::Net::ServerSocket(WINDBELT_PORT);
    server = new Poco::Net::HTTPServer(new HTTPRequestFactory(), *theSocket, params);
}

void initMariaDB() {
    con = mysql_init(nullptr);
    if (con == nullptr) {
        throw std::runtime_error("Could not initalize mariadb");
    }
    if (mysql_real_connect(con, GATEWAY_IP, MARIADB_USER.c_str(),
                           MARIADB_PASSWORD.c_str(), "ctsn",
                           3306, nullptr, 0) == nullptr) {
        throw std::runtime_error(mysql_error(con));
    }
}

void cleanup() {
    delete server;
    delete theSocket;
    if (con != nullptr) {
        mysql_close(con);
    }
}

class Application : public Poco::Util::ServerApplication {
    protected:
        int main(const std::vector<std::string> &) override{
            initHTTPServer();
            server->start();
            std::cout << "Listening" << std::endl;
            waitForTerminationRequest();
            server->stop();
            return 0;
        }
};

int main(int argc, char **argv){
    mysql_library_init(argc, argv, nullptr);
    std::cout << "Starting Windbelt capturer..." << std::endl;
    try {
        initMariaDB();
        Application app;
        app.run(argc, argv);
        cleanup();
    }
    catch(const std::exception &e){
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
        cleanup();
    }
    mysql_library_end();
    std::cout << "Exiting Windbelt capturer" << std::endl;
    return 0;
}

