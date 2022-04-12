/******************************************************************************/
/**
 * @file execute_file.cpp
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


 /******************************** Include Files *******************************/
#include "common/execute_file/execute_file.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <thread>

#include <sys/stat.h>
#include <spawn.h>
#include <unistd.h>
#include <wait.h>

#include "common/log_msg/log_msg.hpp"

/**************************** Constant Definitions ****************************/
const size_t kCBBufferStdout = 256;

/****************************** Type Definitions ******************************/

/************************* Inline Function Definitions ************************/

/****************************** Global Variables ******************************/

/**************************** Function Definitions ****************************/

/******************************************************************************/
/**
 * Function for executing files and reading execution output.
 *
 * @param[in] exec_cmd           The string of the file execution line.
 * @param[in] time_poll_interval The time between polls on the execution status.
 *
 * @return result_stdout The string of the output results.
 *
 * @note None.
 *
 ******************************************************************************/
std::string ExecFile(const std::string exec_cmd, const std::chrono::duration<float> time_poll_interval)
{
    int rc_status = -1;


    /**- Redirect STDOUT by creating a file actions object and adding a "close" action to it. */
    int rg_fildes_pipe_stdout[2] = {};
    pipe(rg_fildes_pipe_stdout);
    posix_spawn_file_actions_t struct_file_actions;

    rc_status = posix_spawn_file_actions_init(&struct_file_actions);
    if (rc_status != 0)
    {
        throw std::runtime_error("posix_spawn_file_actions_init() failed! RC: " + std::to_string(rc_status) + " Error: " + static_cast<std::string>(strerror(errno)));
    }

    rc_status = posix_spawn_file_actions_addclose(&struct_file_actions, rg_fildes_pipe_stdout[0]);
    if (rc_status != 0)
    {
        throw std::runtime_error("posix_spawn_file_actions_addclose() failed! RC: " + std::to_string(rc_status) + " Error: " + static_cast<std::string>(strerror(errno)));
    }

    rc_status = posix_spawn_file_actions_adddup2(&struct_file_actions, rg_fildes_pipe_stdout[1], STDOUT_FILENO);
    if (rc_status != 0)
    {
        throw std::runtime_error("posix_spawn_file_actions_adddup2() failed! RC: " + std::to_string(rc_status) + " Error: " + static_cast<std::string>(strerror(errno)));
    }

    /**- Spawn the child process with provided execution string. */
    /* The environment of the program executed in the child is made the same as the parent's environment. */
    char* sz_exec_cmd = new char[exec_cmd.size() + 1];
    std::copy(exec_cmd.begin(), exec_cmd.end(), sz_exec_cmd);
    sz_exec_cmd[exec_cmd.size()] = '\0';
    char* prgch_argv[] = { "/bin/sh", "-c", sz_exec_cmd, NULL };

    pid_t id_child_pid;
    char** environ;
    rc_status = posix_spawnp(&id_child_pid, prgch_argv[0], &struct_file_actions, NULL, prgch_argv, environ);
    if (rc_status != 0)
    {
        throw std::runtime_error("posix_spawn() failed! RC: " + std::to_string(rc_status) + " Error: " + static_cast<std::string>(strerror(errno)));
    }

    /**- Destroy any objects that we created earlier. */
    delete[] sz_exec_cmd;

    if (&struct_file_actions != nullptr)
    {
        rc_status = posix_spawn_file_actions_destroy(&struct_file_actions);
        if (rc_status != 0)
        {
            throw std::runtime_error("posix_spawn_file_actions_destroy() failed! RC: " + std::to_string(rc_status) + " Error: " + static_cast<std::string>(strerror(errno)));
        }
    }

    /**- Monitor status of the child process until it terminates. */
    int id_child_status = -1;
    do
    {
        rc_status = waitpid(id_child_pid, &id_child_status, WUNTRACED | WCONTINUED);
        if (rc_status == -1)
        {
            throw std::runtime_error("waitpid() failed!");
        }

        if (WIFSIGNALED(id_child_status))
        {
            throw std::runtime_error("ExecFile child process killed by signal: " + std::to_string(WTERMSIG(id_child_status)));
        }
        else if (WIFSTOPPED(id_child_status))
        {
            LOG_MSG_DEBUG("ExecFile child process stopped by signal: ", std::to_string(WSTOPSIG(id_child_status)));
        }
        else if (WIFCONTINUED(id_child_status))
        {
            LOG_MSG_DEBUG("ExecFile child process continued.");
        }

        std::this_thread::sleep_for(time_poll_interval);
    } while (!WIFEXITED(id_child_status));

    /**- Read in STDOUT pipe. */
    close(rg_fildes_pipe_stdout[1]);
    std::string result_stdout;
    int cb_read = 0;
    do
    {
        char rgch_buffer_stdout[kCBBufferStdout] = {};

        cb_read = read(rg_fildes_pipe_stdout[0], rgch_buffer_stdout, kCBBufferStdout);
        if (cb_read == -1)
        {
            throw std::runtime_error("read() failed! RC: " + std::to_string(cb_read) + " Error: " + static_cast<std::string>(strerror(errno)));
        }

        result_stdout += rgch_buffer_stdout;
    } while (cb_read == kCBBufferStdout);

    /**- Return STDOUT string. */
    return result_stdout;
}

/** @} */