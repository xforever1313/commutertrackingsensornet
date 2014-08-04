#ifndef MOCKHTTPSERVERREQUEST_H_
#define MOCKHTTPSERVERREQUEST_H_

#include <gmock/gmock.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <sstream>

namespace MockPoco {
namespace Net {

class MockHTTPServerRequest : public Poco::Net::HTTPServerRequest {
    public:
        virtual ~MockHTTPServerRequest() {}

        std::istream &stream() override {
            return m_ss;
        }

        MOCK_CONST_METHOD0(expectContinue, bool(void));

        MOCK_CONST_METHOD0(clientAddress, Poco::Net::SocketAddress& (void));

        MOCK_CONST_METHOD0(serverAddress, Poco::Net::SocketAddress&(void));

        MOCK_CONST_METHOD0(serverParams, Poco::Net::HTTPServerParams&(void));

        MOCK_CONST_METHOD0(response, Poco::Net::HTTPServerResponse&(void));

        std::stringstream m_ss;
};

}
}

#endif
