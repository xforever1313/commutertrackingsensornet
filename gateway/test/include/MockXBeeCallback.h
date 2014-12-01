#ifndef MOCK_XBEE_CALLBACK_H_
#define MOCK_XBEE_CALLBACK_H_

#include <cstdint>
#include <gmock/gmock.h>
#include <string>
#include <vector>

#include "gateway/XBeeCallbackInterface.h"
#include "ctsn_common/XBeeConstants.h"

namespace Gateway {

class MockXBeeCallback : public XBeeCallbackInterface {
    public:
        virtual ~MockXBeeCallback(){}

        MOCK_METHOD1(successfulParse, void(const std::string &));
        MOCK_METHOD1(incompleteMessage, void(const std::vector<std::uint8_t>&));
        MOCK_METHOD1(badChecksum, void(const std::vector<std::uint8_t>&));
        MOCK_METHOD1(badState, void(const std::vector<std::uint8_t>&));
        MOCK_METHOD0(hardwareReset, void());
        MOCK_METHOD0(watchdogTimerReset, void());
        MOCK_METHOD0(networkWokeUp, void());
        MOCK_METHOD0(networkWentToSleep, void());
        MOCK_METHOD1(invalidModemStatus, void(std::uint8_t));
        MOCK_METHOD1(badModemStatusPacket, void(const std::vector<std::uint8_t> &));
        MOCK_METHOD1(badTxStatusPacket, void(const std::vector<std::uint8_t> &));
        MOCK_METHOD1(invalidPacketFrame, void(uint8_t));
        MOCK_METHOD2(transmitSuccess, void(uint8_t, CTSNCommon::XBeeConstants::DiscoveryStatus ));
        MOCK_METHOD3(transmitFailure, void(uint8_t, CTSNCommon::XBeeConstants::TxStatus errorNumber,
                                           CTSNCommon::XBeeConstants::DiscoveryStatus discovery));
};

}

#endif

