#include <memory>
#include <string>

#include "UnitTest.h"

#include "MockCVRunner.h"
#include "MockHTTPPoster.h"
#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "picture_parser/PictureParseEvent.h"
#include "picture_parser/PictureParseHTTPRequestHandler.h"

TEST_GROUP(PictureParseHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>;
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>;
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_pc = new PictureParser::PictureContainer();
        m_cvRunner = new testing::StrictMock<PictureParser::MockCVRunner>();
        m_httpPoster = new testing::StrictMock<CTSNCommon::MockHTTPPoster>();

        m_uut = new PictureParser::PictureParseHTTPRequestHandler(m_eventExecutor,
                                                                  m_pc,
                                                                  m_cvRunner,
                                                                  m_httpPoster);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
        POINTERS_EQUAL(m_uut->m_pc, m_pc);
        POINTERS_EQUAL(m_uut->m_cvRunner, m_cvRunner);
        POINTERS_EQUAL(m_uut->m_httpPoster, m_httpPoster);

        m_nodeID = 2;
        m_picturePart = 1;
        m_encodedData = "ThisIsEncodedData";
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_httpPoster;
        delete m_cvRunner;
        delete m_pc;
        delete m_eventExecutor;

        delete m_response;
        delete m_request;
    }

    unsigned int m_nodeID;
    unsigned int m_picturePart;
    std::string m_encodedData;

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;

    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    PictureParser::PictureContainer *m_pc;
    testing::StrictMock<PictureParser::MockCVRunner> *m_cvRunner;
    testing::StrictMock<CTSNCommon::MockHTTPPoster> *m_httpPoster;
    PictureParser::PictureParseHTTPRequestHandler *m_uut;

};

/////
/// Post Tests
/////
TEST(PictureParseHTTPRequestHandlerTest, postSuccess) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeID << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_picturePart << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;


    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_eventExecutor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    PictureParser::PictureParseEvent *ppEvent = 
        dynamic_cast<PictureParser::PictureParseEvent*>(event.get());

    CHECK(ppEvent != nullptr);

    POINTERS_EQUAL(ppEvent->m_httpPoster, m_httpPoster);
    POINTERS_EQUAL(ppEvent->m_pc, m_pc);
    POINTERS_EQUAL(ppEvent->m_cvRunner, m_cvRunner);
    CHECK_EQUAL(ppEvent->m_nodeID, m_nodeID);
    CHECK_EQUAL(ppEvent->m_picturePiece, m_picturePart);
    CHECK_EQUAL(ppEvent->m_encodedData, m_encodedData);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_SUCCESS);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

/// Missing data tests
TEST(PictureParseHTTPRequestHandlerTest, missingNodeTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_picturePart << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_MISSING_FIELD);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParseHTTPRequestHandlerTest, missingPicturePartTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeID << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_MISSING_FIELD);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParseHTTPRequestHandlerTest, missingDataTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeID << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_picturePart;

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_MISSING_FIELD);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/// Not a int tests
TEST(PictureParseHTTPRequestHandlerTest, badNodeNumber1) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << "123derp" << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_picturePart << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_INVALID_ARGUMENT);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParseHTTPRequestHandlerTest, badNodeNumber2) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << "derp" << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << m_picturePart << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_INVALID_ARGUMENT);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParseHTTPRequestHandlerTest, badPartNumber1) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeID << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << "123derp" << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_INVALID_ARGUMENT);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParseHTTPRequestHandlerTest, badPartNumber2) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);
    
    m_request->m_ss << PictureParser::PictureParseHTTPRequestHandler::NODE_FORM_DATA << "="
                    << m_nodeID << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::PART_FORM_DATA << "="
                    << "derp" << "&"
                    << PictureParser::PictureParseHTTPRequestHandler::DATA_FORM_DATA << "="
                    << m_encodedData;


    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), 
                PictureParser::PictureParseHTTPRequestHandler::POST_INVALID_ARGUMENT);

    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

/////
/// Get Tests
/////
TEST(PictureParseHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(),
                PictureParser::PictureParseHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

