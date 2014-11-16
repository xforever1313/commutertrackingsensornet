#include <stdexcept>
#include <string>
#include <vector>

#include "EventExecutorInterface.h"
#include "gateway/Node.h"
#include "gateway/ErrorEvent.h"
#include "gateway/ErrorNumbers.h"
#include "gateway/MariaDBInterface.h"
#include "gateway/MariaDBWrapper.h"
#include "gateway/NodeCheckEvent.h"
#include "gateway/NodeContainerInterface.h"
#include "io/LoggerBase.h"

namespace Gateway {

// it takes 1 hour before the nodes become unknown.
const std::string NodeCheckEvent::OUT_OF_DATE_NODE_QUERY = 
    "SELECT * FROM node WHERE TIMESTAMPDIFF(HOUR, node.update_time, NOW()) >= 1";

NodeCheckEvent::NodeCheckEvent(Common::EventExecutorInterface *eventExecutor,
                               MariaDBInterface *mariadb,
                               NodeContainerInterface *nodes,
                               Common::IO::LoggerBase &errLogger /*= 
                               Common::IO::ConsoleLogger::err*/) :

    m_eventExecutor(eventExecutor),
    m_mariadb(mariadb),
    m_result(new MariaDBWrapper::MariaDBResult(m_mariadb)),
    m_nodes(nodes),
    m_errLogger(errLogger)
{

}

NodeCheckEvent::~NodeCheckEvent() {
    delete m_result;
}

void NodeCheckEvent::execute() {
    try {
        m_nodes->refreshNodes();

        m_mariadb->mysql_real_query(OUT_OF_DATE_NODE_QUERY);
        m_result->storeResult();
    
        std::vector<std::string> nodeIDs;
        nodeIDs = m_result->getValuesFromColumn("id");

        m_result->freeResult();

        for (size_t i = 0; i < nodeIDs.size(); ++i) {
            try {
                Node node = m_nodes->convertStringToNode(nodeIDs[i]);
                // Dont do anything if the id is 1.  1 is the gateway,
                // and an external process handles it.
                if ((node.getID() != 1) && (node.getStatus() != Node::NodeStatus::OFFLINE)) {
                    bool changed = m_nodes->setNodeStatus(node.getID(),
                                                          Node::NodeStatus::UNKNOWN);

                    // If the status changed, alert the admins
                    if (changed) {
                        std::shared_ptr<ErrorEvent> event(new ErrorEvent(ErrorNumber::NODE_HAS_UNKNOWN_STATUS,
                                                                         node,
                                                                         m_mariadb));
                        m_eventExecutor->addEvent(event);
                    }
                }
            }
            catch (const std::exception &e) {
                m_errLogger.writeLineWithTimeStamp(e.what());
            }
        }
    }
    catch (const std::exception &e) {
        m_errLogger.writeLineWithTimeStamp(e.what());
        m_result->freeResult();
    }
}

}

