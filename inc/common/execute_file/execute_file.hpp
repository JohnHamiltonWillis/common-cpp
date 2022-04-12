/******************************************************************************/
/**
 * @file execute_file.hpp
 * @brief File containing function for executing files.
 *
 * This file set implements POSIX posix_spawn to execute files. It contains a
 * function for executing files which includes shell commands.
 *
 * @author willisjh
 * @date 11 April 2022
 *
 * @addtogroup ExecuteFile
 * @{
 *
 ******************************************************************************/

#ifndef EXECUTE_FILE_HPP
#define EXECUTE_FILE_HPP

 /******************************** Include Files *******************************/
#include <chrono>
#include <string>

/**************************** Constant Definitions ****************************/

/****************************** Type Definitions ******************************/

/************************* Inline Function Definitions ************************/

/************************ Template Function Definitions ***********************/

/***************************** Function Prototypes ****************************/
std::string ExecFile(const std::string exec_cmd, const std::chrono::duration<float> time_poll_interval);

#endif  /* #ifndef EXECUTE_FILE_HPP */
/** @} */