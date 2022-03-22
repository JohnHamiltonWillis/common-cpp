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
void LogMsg(const char* const sz_filename, const int c_line, const IDPriority id_priority, const std::string message);

/******************************* Macro Functions ******************************/
#define LOG_MSG(id_priority, message) LogMsg(__FILE__, __LINE__, id_priority, message)

#define LOG_MSG_TRACE(message)   LOG_MSG(IDPriority::idTrace, message)
#define LOG_MSG_DEBUG(message)   LOG_MSG(IDPriority::idDebug, message)
#define LOG_MSG_INFO(message)    LOG_MSG(IDPriority::idInfo, message)
#define LOG_MSG_NOTICE(message)  LOG_MSG(IDPriority::idNotice, message)
#define LOG_MSG_WARNING(message) LOG_MSG(IDPriority::idWarning, message)
#define LOG_MSG_ERR(message)     LOG_MSG(IDPriority::idErr, message)
#define LOG_MSG_CRIT(message)    LOG_MSG(IDPriority::idCrit, message)
#define LOG_MSG_ALERT(message)   LOG_MSG(IDPriority::idAlert, message)
#define LOG_MSG_EMERG(message)   LOG_MSG(IDPriority::idEmerg, message)

#endif  /* #ifndef LOG_MSG_HPP */
/** @} */
