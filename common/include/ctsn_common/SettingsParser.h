#ifndef SETTINGS_PARSER_H_
#define SETTINGS_PARSER_H_

#include <map>
#include <string>
#include <vector>

namespace CTSNCommon {

class Settings {
    public:
        static Settings &getInstance();

        ~Settings();

        /**
         * \throw std::runtime_error if file does not exist.
         */
        void parseFromFile(const std::string &fileName);

        /**
         * \brief Parses raw xml data.
         */
        void parse(const std::string &xmlData);

        /**
         * \brief Get a string setting.
         * \throw std::out_of_range if setting does not exist
         */
        const std::string &getSetting(const std::string &setting) const;


        /**
         * \brief Gets a setting that is a short
         * \throw std::out_of_range if a setting does not exist.
         */
        const short getShortSetting(const std::string &setting) const;

        /**
         * \brief iterates through both settings maps, and returns
         *        true if all the passed in settings has a value.
         * \throw std::out_of_range if a passed in setting does not exist.
         * \return true if all passed in settings have a value.
         */
        bool isValid(const std::vector<std::string> &expectedStringSettings,
                     const std::vector<std::string> &expectedShortSettings) const;

    private:
        static const std::string FILE_ERROR;
        static const std::string XML_ERROR;

        Settings();
        Settings(const Settings&) = delete;
        Settings &operator=(const Settings&) = delete;

        std::map<std::string, std::string> m_stringSettings;
        std::map<std::string, short> m_shortSettings;
};

}

#endif

