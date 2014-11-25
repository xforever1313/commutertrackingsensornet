#include <memory>
#include <string>

#include "UnitTest.h"

#include "gateway/PictureParseEvent.h"
#include "gateway/PictureParseHTTPRequestHandler.h"
#include "MockHTTPPoster.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"

TEST_GROUP(PictureParseHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_httpPoster = new testing::StrictMock<CTSNCommon::MockHTTPPoster>();

        m_uut = new Gateway::PictureParseHTTPRequestHandler(m_eventExecutor,
                                                            m_httpPoster);

        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_httpPoster, m_httpPoster);

        m_nodeStr = "2";
        m_partStr = "1";
        m_encodedData = "ThisIsEncodedData";
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_httpPoster;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
    }

    std::string m_nodeStr;
    std::string m_partStr;
    std::string m_encodedData;

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    testing::StrictMock<CTSNCommon::MockHTTPPoster> *m_httpPoster;
    Gateway::PictureParseHTTPRequestHandler *m_uut;

};

/////
/// Post Tests
/////
TEST(PictureParseHTTPRequestHandlerTest, postSuccess) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << Gateway::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeStr << "&"
                    << Gateway::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_partStr << "&"
                    << Gateway::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;


    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    Gateway::PictureParseEvent *ppEvent = 
        dynamic_cast<Gateway::PictureParseEvent*>(event.get());

    CHECK(ppEvent != nullptr);

    POINTERS_EQUAL(ppEvent->m_httpPoster, m_httpPoster);
    CHECK_EQUAL(ppEvent->m_nodeStr, m_nodeStr);
    CHECK_EQUAL(ppEvent->m_partStr, m_partStr);
    CHECK_EQUAL(ppEvent->m_encodedData, m_encodedData);

    CHECK_EQUAL(m_response->m_response.str(), 
                Gateway::PictureParseHTTPRequestHandler::POST_SUCCESS);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

/// Missing data tests
TEST(PictureParseHTTPRequestHandlerTest, missingNodeTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << Gateway::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_partStr << "&"
                    << Gateway::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                Gateway::PictureParseHTTPRequestHandler::POST_MISSING_FIELD);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParseHTTPRequestHandlerTest, missingPicturePartTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << Gateway::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeStr << "&"
                    << Gateway::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                Gateway::PictureParseHTTPRequestHandler::POST_MISSING_FIELD);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParseHTTPRequestHandlerTest, missingDataTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << Gateway::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeStr << "&"
                    << Gateway::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_partStr;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                Gateway::PictureParseHTTPRequestHandler::POST_MISSING_FIELD);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/////
/// Get Tests
/////
TEST(PictureParseHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                Gateway::PictureParseHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

