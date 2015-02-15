#include <memory>

#include "UnitTest.h"

#include "MockEventExecutor.h"
#include "MockHTTPServerRequest.h"
#include "MockHTTPServerResponse.h"
#include "pi_node/PictureParseEvent.h"
#include "pi_node/PictureParseHTTPRequestHandler.h"

TEST_GROUP(PictureParserHTTPRequestHandlerTest) {
    TEST_SETUP() {
        m_request =  new testing::StrictMock<MockPoco::Net::MockHTTPServerRequest>();
        m_response = new testing::StrictMock<MockPoco::Net::MockHTTPServerResponse>();
        m_cvExeuctor = new testing::StrictMock<MockEventExecutor>();
        m_eventExecutor = new testing::StrictMock<MockEventExecutor>();
        m_uut = new PiNode::PictureParseHTTPRequestHandler(m_cvExeuctor, m_eventExecutor);

        POINTERS_EQUAL(m_uut->m_cvExecutor, m_cvExeuctor);
        POINTERS_EQUAL(m_uut->m_eventExecutor, m_eventExecutor);
    }

    TEST_TEARDOWN() {
        delete m_uut;
        delete m_eventExecutor;
        delete m_cvExeuctor;
        delete m_request;
        delete m_response;
    }

    testing::StrictMock<MockPoco::Net::MockHTTPServerRequest> *m_request;
    testing::StrictMock<MockPoco::Net::MockHTTPServerResponse> *m_response;
    testing::StrictMock<MockEventExecutor> *m_cvExeuctor;
    testing::StrictMock<MockEventExecutor> *m_eventExecutor;
    PiNode::PictureParseHTTPRequestHandler *m_uut;
};

TEST(PictureParserHTTPRequestHandlerTest, handlePostRequestRemoveFalseTest) {
    std::string picLocation = "derp.png";

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << PiNode::PictureParseHTTPRequestHandler::REMOVE_FORM_DATA << "=false&";
    m_request->m_ss << PiNode::PictureParseHTTPRequestHandler::PICTURE_FORM_DATA << "=" << picLocation;

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_cvExeuctor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    PiNode::PictureParseEvent *picEvent =
        dynamic_cast<PiNode::PictureParseEvent*>(event.get());

    CHECK(picEvent != nullptr);

    POINTERS_EQUAL(picEvent->m_eventExecutor, m_eventExecutor);
    CHECK(!picEvent->m_removePicture);
    CHECK_EQUAL(picEvent->m_pictureLocation, picLocation);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::PictureParseHTTPRequestHandler::POST_SUCCESS);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(PictureParserHTTPRequestHandlerTest, handlePostRequestRemoveTrueTest) {
    std::string picLocation = "derp.png";

    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << PiNode::PictureParseHTTPRequestHandler::REMOVE_FORM_DATA << "=true&";
    m_request->m_ss << PiNode::PictureParseHTTPRequestHandler::PICTURE_FORM_DATA << "=" << picLocation;

    std::shared_ptr<Common::EventInterface> event = nullptr;
    EXPECT_CALL(*m_cvExeuctor, addEvent(testing::_))
        .WillOnce(testing::SaveArg<0>(&event));

    m_uut->handleRequest(*m_request, *m_response);

    PiNode::PictureParseEvent *picEvent =
        dynamic_cast<PiNode::PictureParseEvent*>(event.get());

    CHECK(picEvent != nullptr);

    POINTERS_EQUAL(picEvent->m_eventExecutor, m_eventExecutor);
    CHECK(picEvent->m_removePicture);
    CHECK_EQUAL(picEvent->m_pictureLocation, picLocation);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::PictureParseHTTPRequestHandler::POST_SUCCESS);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}

TEST(PictureParserHTTPRequestHandlerTest, handlePostRequestCheckMissingEverything) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::PictureParseHTTPRequestHandler::POST_FAILURE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParserHTTPRequestHandlerTest, handlePostRequestCheckMissingPicture) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << PiNode::PictureParseHTTPRequestHandler::REMOVE_FORM_DATA << "=true";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::PictureParseHTTPRequestHandler::POST_FAILURE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParserHTTPRequestHandlerTest, handlePostRequestCheckMissingRemove) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_POST);

    // Form data from a post request
    m_request->m_ss << PiNode::PictureParseHTTPRequestHandler::PICTURE_FORM_DATA << "=true";

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::PictureParseHTTPRequestHandler::POST_FAILURE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
}

TEST(PictureParserHTTPRequestHandlerTest, getTest) {
    m_request->setMethod(Poco::Net::HTTPRequest::HTTP_GET);

    m_uut->handleRequest(*m_request, *m_response);

    CHECK_EQUAL(m_response->m_response.str(), PiNode::PictureParseHTTPRequestHandler::GET_MESSAGE);
    CHECK_EQUAL(m_response->_status, Poco::Net::HTTPResponse::HTTP_OK);
}
