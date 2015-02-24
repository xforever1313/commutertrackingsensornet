#include <fstream>
#include <map>
#include <string>
#include <rapidxml.hpp>

#include "ctsn_common/SettingsParser.h"

namespace CTSNCommon {

const std::string Settings::FILE_ERROR = "Could not open: ";
const std::string Settings::XML_ERROR = "Something is wrong with the xml file.";

Settings &Settings::getInstance() {
    static Settings settings;
    return settings;
}

Settings::Settings() :
    m_stringSettings({{"MAILGUN_KEY", ""},
                      {"MAILGUN_URL", ""},
                      {"MAILGUN_FROM", ""},
                      {"MARIADB_USER", ""},
                      {"MARIADB_PASS", ""},
                      {"MARIADB_IP", ""},
                      {"MARIADB_NAME", ""},
                      {"GATEWAY_IP", ""},
                      {"GATEWAY_AGENT", ""},
                      {"GATEWAY_SERIAL", ""},
                      {"NODE_AGENT", ""},
                      {"NODE_SERIAL", ""},
                      {"NODE_GATEWAY_ADDRESS", ""},
                      {"NODE_ADDRESS", ""},
                      {"NODE_PIC_OUTPUT", ""},
                      {"NODE_PI_CAM_LOCATION", ""}
                     }),

    m_shortSettings({{"MARIADB_PORT", -1},
                     {"GATEWAY_PORT", -1},
                     {"NODE_PORT", -1},
                     {"NODE_GATEWAY_ID", -1},
                     {"NODE_ID", -1}
                    })
{

}

Settings::~Settings() {

}

void Settings::parseFromFile(const std::string &fileName) {
    std::string fileContents = "";
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        throw std::runtime_error(FILE_ERROR + fileName);
    }
    else {
        while (!inFile.eof()) {
            char c;
            inFile.get(c);
            fileContents += c;
            inFile.peek();
        }
    }
    inFile.close();
    parse(fileContents);
}

void Settings::parse(const std::string &xmlData) {
    rapidxml::xml_document<char> doc;
    doc.parse<0>(const_cast<char*>(xmlData.data()));

    // Get the first node
    rapidxml::xml_node<> *firstNode = doc.first_node();
    
    //Ensure the first node is what we expect
    if (firstNode->name() != std::string("ctsn_settings")) {
        throw std::runtime_error(XML_ERROR);
    }

    // Iterate over each child node
    for (rapidxml::xml_node<> *n = doc.first_node()->first_node(); n; n=n->next_sibling()) {
        if (n->name() == std::string("gateway_settings")) {

            // iterate over each gateway settings and add them.
            for (rapidxml::xml_node<> *g = n->first_node(); g; g=g->next_sibling()) {
                if (g->name() == std::string("mailgun")) {
                    // iterate over mailgun settings
                    for (rapidxml::xml_attribute<> *a = g->first_attribute();
                         a != nullptr; 
                         a=a->next_attribute())
                     {
                        if (a->name() == std::string("key")) {
                            m_stringSettings["MAILGUN_KEY"] = a->value();
                        }
                        else if (a->name() == std::string("url")) {
                            m_stringSettings["MAILGUN_URL"] = a->value();
                        }
                        else if (a->name() == std::string("from")) {
                            m_stringSettings["MAILGUN_FROM"] = a->value();
                        }
                    }
                }
                else if (g->name() == std::string("mariadb")) {
                    // iterate over mariadb settings
                    for (rapidxml::xml_attribute<> *a = g->first_attribute();
                         a != nullptr; 
                         a=a->next_attribute())
                    {
                        if (a->name() == std::string("user")) {
                            m_stringSettings["MARIADB_USER"] = a->value();
                        }
                        else if (a->name() == std::string("pass")) {
                            m_stringSettings["MARIADB_PASS"] = a->value();
                        }
                        else if (a->name() == std::string("port")) {
                            m_shortSettings["MARIADB_PORT"] = std::stoi(a->value());
                        }
                        else if (a->name() == std::string("ip")) {
                            m_stringSettings["MARIADB_IP"] = a->value();
                        }
                        else if (a->name() == std::string("name")) {
                            m_stringSettings["MARIADB_NAME"] = a->value();
                        }
                    }
                }
                else if (g->name() == std::string("gateway")) {
                    // iterate over gateway settings
                    for (rapidxml::xml_attribute<> *a = g->first_attribute();
                         a != nullptr; 
                         a=a->next_attribute())
                    {
                        if (a->name() == std::string("ip")) {
                            m_stringSettings["GATEWAY_IP"] = a->value();
                        }
                        else if (a->name() == std::string("port")) {
                            m_shortSettings["GATEWAY_PORT"] = std::stoi(a->value());
                        }
                        else if (a->name() == std::string("serial")) {
                            m_stringSettings["GATEWAY_SERIAL"] = a->value();
                        }
                        else if (a->name() == std::string("agent")) {
                            m_stringSettings["GATEWAY_AGENT"] = a->value();
                        }
                    }
                }
            }
        }

        else if (n->name() == std::string("node_settings")) {
            // First, get node's attributes
            for (rapidxml::xml_attribute<> *a = n->first_attribute();
                 a != nullptr; 
                 a=a->next_attribute())
            {
                if (a->name() == std::string("agent")) {
                    m_stringSettings["NODE_AGENT"] = a->value();
                }
                else if (a->name() == std::string("serial")) {
                    m_stringSettings["NODE_SERIAL"] = a->value();
                }
                else if (a->name() == std::string("port")) {
                    m_shortSettings["NODE_PORT"] = std::stoi(a->value());
                }
                else if (a->name() == std::string("picture_output")) {
                    m_stringSettings["NODE_PIC_OUTPUT"] = a->value();
                }
                else if (a->name() == std::string("pi_cam_script")) {
                    m_stringSettings["NODE_PI_CAM_LOCATION"] = a->value();
                }
            }

            // iterate over each node settings and add them.
            for (rapidxml::xml_node<> *g = n->first_node();
                 g; 
                 g=g->next_sibling()) 
            {
                if (g->name() == std::string("gateway_node")) {

                    for (rapidxml::xml_attribute<> *a = g->first_attribute();
                         a != nullptr; 
                         a=a->next_attribute()) 
                    {
                        if (a->name() == std::string("id")) {
                            m_shortSettings["NODE_GATEWAY_ID"] = std::stoi(a->value());
                        }
                        else if (a->name() == std::string("address")) {
                            m_stringSettings["NODE_GATEWAY_ADDRESS"] = a->value();
                        }
                    }
                }
                else if (g->name() == std::string("this_node")) {
                    for (rapidxml::xml_attribute<> *a = g->first_attribute();
                         a != nullptr; 
                         a=a->next_attribute())
                    {
                        if (a->name() == std::string("id")) {
                            m_shortSettings["NODE_ID"] = std::stoi(a->value());
                        }
                        else if (a->name() == std::string("address")) {
                            m_stringSettings["NODE_ADDRESS"] = a->value();
                        }
                    }
                }
            }
        }
    }
}

const std::string &Settings::getSetting(const std::string &setting) const {
    return m_stringSettings.at(setting);
}

const short Settings::getShortSetting(const std::string &setting) const {
    return m_shortSettings.at(setting);
}

bool Settings::isValid(const std::vector<std::string> &expectedStringSettings,
                       const std::vector<std::string> &expectedShortSettings) const {

    bool success = true;

    for (size_t i = 0; (i < expectedStringSettings.size()) && success; ++i) {
        if (m_stringSettings.at(expectedStringSettings[i]) == "") {
            success = false;
        }
    }

    for (size_t i = 0; (i < expectedShortSettings.size()) && success; ++i) {
        if (m_shortSettings.at(expectedShortSettings[i]) == -1) {
            success = false;
        }
    }

    return success;
}

}

