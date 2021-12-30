/* This file makes use of Doxygen syntax for generating documentation. */
/******************************************************************************/
/**
 *
 * @file log_msg.hpp
 * @brief Function prototypes and macros for log functions.
 *
 * This contains the function prototypes and macros for the log message functions
 * and any constants the caller may need.
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

#ifndef LOG_MSG_HPP
#define LOG_MSG_HPP

/******************************** Include Files *******************************/
#include <string>

#include "../exception_priority/ExceptionPriority.hpp"

/**************************** Constant Definitions ****************************/

/****************************** Type Definitions ******************************/

/***************************** Function Prototypes ****************************/

/*
 * Log functions defined in log_msg.cpp.
 */
void logMsg(const char* const szFilename, const int cLine, const IDPriority idPriority, const std::string stMessage);

/******************************* Macro Functions ******************************/
#define LOG_MSG(idPriority, stMessage) logMsg(__FILE__, __LINE__, idPriority, stMessage)

#define LOG_MSG_TRACE(stMessage)   LOG_MSG(IDPriority::idTrace, stMessage)
#define LOG_MSG_DEBUG(stMessage)   LOG_MSG(IDPriority::idDebug, stMessage)
#define LOG_MSG_INFO(stMessage)    LOG_MSG(IDPriority::idInfo, stMessage)
#define LOG_MSG_NOTICE(stMessage)  LOG_MSG(IDPriority::idNotice, stMessage)
#define LOG_MSG_WARNING(stMessage) LOG_MSG(IDPriority::idWarning, stMessage)
#define LOG_MSG_ERR(stMessage)     LOG_MSG(IDPriority::idErr, stMessage)
#define LOG_MSG_CRIT(stMessage)    LOG_MSG(IDPriority::idCrit, stMessage)
#define LOG_MSG_ALERT(stMessage)   LOG_MSG(IDPriority::idAlert, stMessage)
#define LOG_MSG_EMERG(stMessage)   LOG_MSG(IDPriority::idEmerg, stMessage)

#endif  /* #ifndef LOG_MSG_HPP */
/** @} */
