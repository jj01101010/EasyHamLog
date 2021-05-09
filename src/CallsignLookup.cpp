#include <CallsignLookup.h>
#include <QSODatabaseInterface.h>
#include <string>

#define PREFIX_DATABASE_DIR "settings/prefix_lookup.xml"

std::vector<EasyHamLog::Callsign_Prefix*> EasyHamLog::CallsignLookup::prefixes;

bool EasyHamLog::CallsignLookup::Initialize()
{

    QSO_DATABASE* database = nullptr;
    QSO_DATABASE_ELEMENT root;

    // Read the Prefix list and store it in the unsorted_list
    if (!EasyHamLog::QSODatabaseInterface::readDatabase(PREFIX_DATABASE_DIR, database, &root)) {
        return false;
    }

    QSO_DATABASE_ELEMENT current_prefix;

    while (!(current_prefix = EasyHamLog::QSODatabaseInterface::nextElement(&root)).isNull()) {
        EasyHamLog::Callsign_Prefix* prefix = new EasyHamLog::Callsign_Prefix;
        prefix->country_name = current_prefix.attribute("name", "").toStdString();
        prefix->prefix = current_prefix.attribute("prefix", "").toStdString();
        prefix->cq_region = current_prefix.attribute("cq", "").toInt();
        prefix->itu_region = current_prefix.attribute("itu", "").toInt();

        prefixes.push_back(prefix);
    }

    delete database;

    return true;
}

void EasyHamLog::CallsignLookup::Destroy()
{
    for (auto& prefix : prefixes) {
        delete prefix;
    }
}

// Helper function to split a string by one seperator
std::vector<std::string> splitString(const char seperator, const std::string& s) {
    std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

    return output;
}

// Get a prefix object with the country name by looking up the callsign prefix in the lookup table
EasyHamLog::Callsign_Prefix* EasyHamLog::CallsignLookup::getPrefix(const QString& call_prefix)
{
    QString c_prefix = call_prefix.toUpper();

    for (auto& prefix : prefixes) {
        std::vector<std::string> splitted = splitString('-', prefix->prefix);   // Split the current callsign prefix by - (from xa to xz is xa-xz)

        // If there is only splitted object it means there is only one prefix for a country (e.g. 4X for israel)
        if (splitted.size() == 1) {
            if (splitted[0] == c_prefix.toStdString()) {
                return prefix;
            }
        }
        // If there is a range of callsign prefixes we want to see if the first letter matches in both the callsign prefix and also the current prefix
        // Also we need to check if the second letter of the callsign prefix is in the range of the current prefix
        else {
            if (call_prefix.size() < 2) {
                continue;
            }

            if (splitted[0][1] <= c_prefix[1] && splitted[1][1] >= c_prefix[1] && splitted[0][0] == c_prefix[0]) {
                return prefix;
            }
        }
    }

    return nullptr;
}
