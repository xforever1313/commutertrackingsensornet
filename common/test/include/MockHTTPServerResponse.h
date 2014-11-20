#ifndef MOCKHTTPSERVERRESPONSE_H_
#define MOCKHTTPSERVERRESPONSE_H_

#include <gmock/gmock.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <string>
#include <sstream>

namespace MockPoco {
namespace Net{

class MockHTTPServerResponse : public Poco::Net::HTTPServerResponse {
    public:
        virtual ~MockHTTPServerResponse() {}

        MOCK_METHOD0(sendContinue, void(void));
        MOCK_METHOD2(sendFile, void(const std::string &path, const std::string &mediaType));
        MOCK_METHOD2(sendBuffer, void(const void* pBuffer, std::size_t length));
        MOCK_METHOD2(redirect, void(const std::string& uri, HTTPStatus status));
        MOCK_METHOD1(requireAuthentication, void(const std::string& realm));
        MOCK_CONST_METHOD0(sent, bool(void));


        std::ostream &send() override {
            return m_response;
        }

        std::stringstream m_response;
};

}
}

#endif
