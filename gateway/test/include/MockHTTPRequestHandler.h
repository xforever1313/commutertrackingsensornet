#ifndef MOCKHTTPREQUESTHANDLER_H_INCLUDED
#define MOCKHTTPREQUESTHANDLER_H_INCLUDED

#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "gateway/BaseHTTPRequestHandler.h"


namespace Gateway {

class MockHTTPRequestHandler : public BaseHTTPRequestHandler {
    public:
        MockHTTPRequestHandler() :
            m_postCalled(false),
            m_getCalled(false)
        {
        }

        ~MockHTTPRequestHandler() {
        }

        void handlePostRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override {
            m_postCalled = true;
        }
        void handleGetRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override {
            m_getCalled = true;
        }

        bool m_postCalled;
        bool m_getCalled;
};

}

#endif // MOCKHTTPREQUESTHANDLER_H_INCLUDED
