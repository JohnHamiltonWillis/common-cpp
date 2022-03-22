/* This file makes use of Doxygen syntax for generating documentation. */
/******************************************************************************/
/**
 *
 * @file log_msg.cpp
 * @brief Function definitions for log message functions.
 *
 * This contains the function definitions for the log message functions.
 *
 * @author John H. Willis (willisjh)
 * @date 19 September 2021
 *
 * @bug No known bugs.
 *
 * @addtogroup LogMsg
 * @{
 *
 ******************************************************************************/


/******************************** Include Files *******************************/
#include "common/log_msg/log_msg.hpp"

#include <ctime>
#include <iomanip>
#include <vector>
#include <thread>
#include <sstream>
#include <map>
#include <iostream>

#ifdef __unix__
#include <syslog.h>
#endif

/**************************** Constant Definitions ****************************/
const std::map<const IDPriority, const std::string> kMpIdPriorityStringPriority{ {IDPriority::kIdTrace, "TRACE"}, {IDPriority::kIdDebug, "DEBUG"}, {IDPriority::kIdInfo, "INFO"}, {IDPriority::kIdNotice, "NOTICE"}, {IDPriority::kIdWarning, "WARNING"}, {IDPriority::kIdErr, "ERR"}, {IDPriority::kIdCrit, "CRIT"}, {IDPriority::kIdAlert, "ALERT"}, {IDPriority::kIdEmerg, "EMERG"} };
constexpr const char* kSzIdPriorityUnknown = "UNKNOWN";

#ifdef __unix__
constexpr auto kIdFacility = LOG_USER;

const std::map<const IDPriority, const auto> kMpIdPriorityIdPrioritySyslog{ {IDPriority::kIdTrace, LOG_DEBUG}, {IDPriority::kIdDebug, LOG_DEBUG}, {IDPriority::kIdInfo, LOG_INFO}, {IDPriority::kIdNotice, LOG_NOTICE}, {IDPriority::kIdWarning, LOG_WARNING}, {IDPriority::kIdErr, LOG_ERR}, {IDPriority::kIdCrit, LOG_CRIT}, {IDPriority::kIdAlert, LOG_ALERT}, {IDPriority::kIdEmerg, LOG_EMERG} };
#endif


/* Manually update with largest kMpIdPriorityStringPriority priority identifier string above. */
constexpr auto kCBStringIdPriorityLast = sizeof("WARNING");

/* Strings may exceed these and shift message formatting. Change as needed. */
constexpr auto kCBStringIdThreadLast = 15;
constexpr auto kCBStringLocationCodeLast = 15;

/*********************** Static Variable Initializations **********************/

/****************************** Type Definitions ******************************/

/***************************** Function Prototypes ****************************/

/**************************** Function Definitions ****************************/


/******************************************************************************/
/**
 * Function for getting string representation of priority.
 *
 * @param[in] id_priority The identifier of the priority.
 *
 * @return string_priority The string representation of priority.
 *
 * @note Unsupported priority levels are given a default string.
 *
 ******************************************************************************/
std::string StringPriorityFromidPriority(IDPriority id_priority)
{
    if (kMpIdPriorityStringPriority.find(id_priority) != kMpIdPriorityStringPriority.end())
    {
        return kMpIdPriorityStringPriority.at(id_priority);
    }
    else
    {
        return std::string(kSzIdPriorityUnknown);
    }
}

#ifdef __unix__
/******************************************************************************/
/**
 * Function for getting syslog identifier of priority.
 *
 * @param[in] id_priority The identifier of the priority.
 *
 * @return id_priority_syslog The identifier of priority for syslog.
 *
 * @note Unsupported priority levels are given a default string.
 *
 ******************************************************************************/
int IdPrioritySyslogFromidPriority(IDPriority id_priority)
{
    if (kMpIdPriorityIdPrioritySyslog.find(id_priority) != kMpIdPriorityIdPrioritySyslog.end())
    {
        return kMpIdPriorityIdPrioritySyslog.at(id_priority);
    }
    else
    {
        return LOG_DEBUG;
    }
}
#endif

/******************************************************************************/
/**
 * Function for padding the back of a string.
 *
 * @param[in] string     The string to be padded.
 * @param[in] c_ch_last  The last count of characters for the resulting string.
 * @param[in] ch_padding The character to pad the string with.
 *
 * @return None.
 *
 * @note
 *
 ******************************************************************************/
void PadStringBack(std::string& string, const size_t c_ch_last, const char ch_padding = ' ')
{
    if (c_ch_last > string.size())
    {
        string.insert(string.size(), c_ch_last - string.size(), ch_padding);
    }
}

/******************************************************************************/
/**
 * Function for logging with priority level and message.
 *
 * @param[in] sz_filename The zero-terminated string of the code filename.
 * @param[in] c_line      The count of the line number for the code.
 * @param[in] id_priority The identifier of the priority.
 * @param[in] message     The message for logging.
 *
 * @return None.
 *
 * @note
 *
 ******************************************************************************/
void LogMsg(const char* const sz_filename, const int c_line, const IDPriority id_priority, const std::string message)
{
    /**- Get current time. */
    const std::time_t timeNow = std::time(0);
    std::ostringstream ostrm_string_datetime;
    ostrm_string_datetime << std::put_time(std::localtime(&timeNow), "%F | %T %Z");

    /**- Format thread identifier string. */
    std::ostringstream ostrmstidThread;
    ostrmstidThread << std::this_thread::get_id();
    std::string string_id_thread(ostrmstidThread.str());
    PadStringBack(string_id_thread, kCBStringIdThreadLast);

    /**- Format priority identifier string. */
    std::string string_id_priority(StringPriorityFromidPriority(id_priority));
    PadStringBack(string_id_priority, kCBStringIdPriorityLast);

    /**- Format code location string. */
    std::string string_location_code(sz_filename + std::string(":") + std::to_string(c_line));
    PadStringBack(string_location_code, kCBStringLocationCodeLast);

    /**- Create message string. */
    const std::string stLogMessage(ostrm_string_datetime.str() + " | " + string_id_thread + " | " + string_id_priority + " | " + string_location_code + " | " + message);


#ifdef __unix__
    /**- Log message. */
    syslog(kIdFacility | IdPrioritySyslogFromidPriority(idPriority), stMessage.c_str());
#endif

    /**- Output message. */
    if (id_priority >= IDPriority::kIdNotice)
    {
        std::cerr << stLogMessage << std::endl;
    }
    else
    {
        std::cout << stLogMessage << std::endl;
    }
}
/** @} */
