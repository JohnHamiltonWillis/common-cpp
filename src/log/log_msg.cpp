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
#include "log_msg.hpp"

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
const std::map<const IDPriority, std::string> MP_ID_PRIORITY_ST_ID_PRIORITY{ {IDPriority::idTrace, "TRACE"}, {IDPriority::idDebug, "DEBUG"}, {IDPriority::idInfo, "INFO"}, {IDPriority::idNotice, "NOTICE"}, {IDPriority::idWarning, "WARNING"}, {IDPriority::idErr, "ERR"}, {IDPriority::idCrit, "CRIT"}, {IDPriority::idAlert, "ALERT"}, {IDPriority::idEmerg, "EMERG"} };
constexpr const char* SZ_ID_PRIORITY_UNKNOWN = "UNKNOWN";

#ifdef __unix__
constexpr int ID_FACILITY = LOG_USER;

const std::map<const IDPriority, int> MP_ID_PRIORITY_SYSLOG_ID_PRIORITY{ {IDPriority::idTrace, LOG_DEBUG}, {IDPriority::idDebug, LOG_DEBUG}, {IDPriority::idInfo, LOG_INFO}, {IDPriority::idNotice, LOG_NOTICE}, {IDPriority::idWarning, LOG_WARNING}, {IDPriority::idErr, LOG_ERR}, {IDPriority::idCrit, LOG_CRIT}, {IDPriority::idAlert, LOG_ALERT}, {IDPriority::idEmerg, LOG_EMERG} };
#endif


/* Manually update with largest ST_ID_PRIORITY severity identifier string above. */
constexpr int CB_ST_ID_PRIORITY_LAST = sizeof("WARNING");

/* Strings may exceed these and shift message formatting. Change as needed. */
constexpr int CB_ST_ID_THREAD_LAST = 15;
constexpr int CB_ST_LOCATION_CODE_LAST = 15;

/*********************** Static Variable Initializations **********************/

/****************************** Type Definitions ******************************/

/***************************** Function Prototypes ****************************/

/**************************** Function Definitions ****************************/


/******************************************************************************/
/**
 * Function for getting string representation of severity.
 *
 * @param[in] idPriority The identifier of the severity.
 *
 * @return stPriority The string representation of severity.
 *
 * @note Unsupported severity levels are given a default string.
 *
 ******************************************************************************/
std::string stPriorityFromidPriority(IDPriority idPriority)
{
    if (MP_ID_PRIORITY_ST_ID_PRIORITY.find(idPriority) != MP_ID_PRIORITY_ST_ID_PRIORITY.end())
    {
        return MP_ID_PRIORITY_ST_ID_PRIORITY.at(idPriority);
    }
    else
    {
        return std::string(SZ_ID_PRIORITY_UNKNOWN);
    }
}

#ifdef __unix__
/******************************************************************************/
/**
 * Function for getting syslog representation of severity.
 *
 * @param[in] idPriority The identifier of the severity.
 *
 * @return stPriority The string representation of severity.
 *
 * @note Unsupported severity levels are given a default string.
 *
 ******************************************************************************/
std::string idPrioritySyslogFromidPriority(IDPriority idPriority)
{
    if (MP_ID_PRIORITY_SYSLOG_ID_PRIORITY.find(idPriority) != MP_ID_PRIORITY_SYSLOG_ID_PRIORITY.end())
    {
        return MP_ID_PRIORITY_SYSLOG_ID_PRIORITY.at(idPriority);
    }
    else
    {
        return std::string(LOG_DEBUG);
    }
}
#endif

/******************************************************************************/
/**
 * Function for padding the back of a string.
 *
 * @param[in] stString  The string to be padded.
 * @param[in] cCharLast The last count of characters for the resulting string.
 * @param[in] chPadding The character to pad the string with.
 *
 * @return None.
 *
 * @note
 *
 ******************************************************************************/
void padStringBack(std::string& stString, const size_t cCharLast, const char chPadding = ' ')
{
    if (cCharLast > stString.size())
    {
        stString.insert(stString.size(), cCharLast - stString.size(), chPadding);
    }
}

/******************************************************************************/
/**
 * Function for logging with severity level and message.
 *
 * @param[in] szFilename The zero-terminated string of the code filename.
 * @param[in] cLine      The count of the line number for the code.
 * @param[in] idPriority The identifier of the severity.
 * @param[in] szFilename The zero-terminated string of the code filename.
 *
 * @return None.
 *
 * @note
 *
 ******************************************************************************/
void logMsg(const char* const szFilename, const int cLine, const IDPriority idPriority, const std::string stMessage)
{
    /**- Get current time. */
    const std::time_t timeNow = std::time(0);  
    std::ostringstream ostrmstDateTime;
    ostrmstDateTime << std::put_time(std::localtime(&timeNow), "%F | %T %Z");

    /**- Format thread identifier string. */
    std::ostringstream ostrmstidThread;
    ostrmstidThread << std::this_thread::get_id();
    std::string stidThread(ostrmstidThread.str());
    padStringBack(stidThread, CB_ST_ID_THREAD_LAST);

    /**- Format priority identifier string. */
    std::string stidPriority(stPriorityFromidPriority(idPriority));
    padStringBack(stidPriority, CB_ST_ID_PRIORITY_LAST);

    /**- Format code location string. */
    std::string stLocationCode(szFilename + std::string(":") + std::to_string(cLine));
    padStringBack(stLocationCode, CB_ST_LOCATION_CODE_LAST);

    /**- Create message string. */
    const std::string stLogMessage(ostrmstDateTime.str() + " | "  + stidThread + " | " + stidPriority + " | " + stLocationCode + " | " + stMessage);


#ifdef __unix__
    /**- Log message. */
    syslog(ID_FACILITY | idPrioritySyslogFromidPriority(idPriority), stMessage);
#endif

    /**- Output message. */
    if (idPriority >= IDPriority::idNotice)
    {
        std::cerr << stLogMessage << std::endl;
    }
    else
    {
        std::cout << stLogMessage << std::endl;
    }
}
/** @} */