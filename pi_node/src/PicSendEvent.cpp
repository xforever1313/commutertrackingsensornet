#include <chrono>
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "b64/encode.h"
#include "ctsn_common/XBeeTxEvent.h"
#include "ctsn_common/UartInterface.h"
#include "io/ConsoleLogger.h"
#include "pi_node/PicSendEvent.h"

namespace PiNode {

PicSendEvent::PicSendEvent(const std::string &pictureFile,
                           CTSNCommon::UartInterface *uart) :
    m_pictureFile(pictureFile),
    m_uart(uart)
{

}

PicSendEvent::~PicSendEvent() {

}

void PicSendEvent::execute() {
    try {
        std::ifstream inFile(m_pictureFile, std::ios::binary);
        if (!inFile.is_open()) {
            throw std::runtime_error("Could not open picture file: " + m_pictureFile);
        }

        std::vector<uint8_t> data;
        while (!inFile.eof()) {
            data.push_back(inFile.get());
            inFile.peek();
        }

        inFile.close();

        base64::encoder encoder;
        std::stringstream encodedData(encoder.encode(data));
        CTSNCommon::Node node(1, 0x13A20040B27E49);
        for (unsigned int i = 1; !encodedData.eof(); ++i) {
            std::string messagePiece;
            std::getline(encodedData, messagePiece);
            std::string message = "/data\tnode=" + 
                                  //NODE_ID is set by the -D flag
                                  std::to_string(NODE_ID) + 
                                  "|part=" + 
                                  std::to_string(i) + 
                                  "|data=" + messagePiece;

            CTSNCommon::XBeeTxEvent txEvent(node,
                                            message,
                                            m_uart);
            txEvent.execute();

            std::chrono::milliseconds sleepTime(500);
            std::this_thread::sleep_for(sleepTime);
        }
        
    }
    catch (const std::exception &e) {
        Common::IO::ConsoleLogger::err.writeLineWithTimeStamp(e.what());
    }
}

}

