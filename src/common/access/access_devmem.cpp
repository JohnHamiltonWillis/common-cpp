/* This file makes use of Doxygen syntax for generating documentation. */
/******************************************************************************/
/**
 * @file access_devmem.cpp
 * @brief File containing /dev/mem access functions.
 *
 * This contains the function definitions for an API to the /dev/mem driver. It
 * provides functions for accessing the memory accessible by /dev/mem.
 *
 * @author John H. Willis (willisjh)
 * @date 29 March 2022
 *
 * @bug No known bugs.
 *
 * @addtogroup AccessDevMem
 * @{
 *
 ******************************************************************************/


/****************************** Include Files *********************************/
#include "common/access/access_devmem.hpp"

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <cstdio>

#include "common/exception_priority/exception_priority.hpp"

/*************************** Constant Definitions *****************************/

/***************************** Type Definitions *******************************/

/****************** Macros (Inline Functions) Definitions *********************/

/*************************** Function Prototypes ******************************/

/*************************** Variable Definitions *****************************/


/******************************************************************************/
/**
 * Function for reading 32 bits from an address on /dev/mem.
 *
 * @param[in]  addr_physical The byte-aligned physical address to read from.
 * @param[out] val_read      The value read.
 *
 * @return None.
 *
 * @note None.
 *
 ******************************************************************************/
void ReadDevMem(const uintptr_t addr_physical, uint32_t& val_read)
{
    /**- Open /dev/mem driver. */
    const auto fd_devmem = open("/dev/mem", O_RDONLY);
    if (fd_devmem == -1)
    {
        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not open /dev/mem!");
    }

    /* Calculate page-aligned address and offset. */
    const auto cb_page = sysconf(_SC_PAGESIZE);
    const auto addr_physical_page = (addr_physical & ~(cb_page - 1));
    const auto baddr_physical_page_src = addr_physical - addr_physical_page;

    /**- MMap /dev/mem memory space. */
    void* const p_mmap = mmap(nullptr, cb_page, PROT_READ, MAP_SHARED, fd_devmem, addr_physical_page);
    if (p_mmap == MAP_FAILED)
    {
        close(fd_devmem);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not mmap address: " + std::to_string(addr_physical) + "!");
    }

    /**- Read from /dev/mem memory space. */
    val_read = *static_cast<uint32_t*>(static_cast<void*>(static_cast<uint8_t*>(p_mmap) + baddr_physical_page_src));

    /**- Close out resources. */
    close(fd_devmem);
    munmap(p_mmap, cb_page);
}

/******************************************************************************/
/**
 * Function for writing 32 bits from an address on /dev/mem.
 *
 * @param[in]  addr_physical The byte-aligned physical address to write to.
 * @param[out] val_write     The value to be written.
 *
 * @return None.
 *
 * @note None.
 *
 ******************************************************************************/
void WriteDevMem(const uintptr_t addr_physical, const uint32_t val_write)
{
    /**- Open /dev/mem driver. */
    const auto fd_devmem = open("/dev/mem", O_RDWR);
    if (fd_devmem == -1)
    {
        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not open /dev/mem");
    }

    /* Calculate page-aligned address and offset. */
    const auto cb_page = sysconf(_SC_PAGESIZE);
    const auto addr_physical_page = (addr_physical & ~(cb_page - 1));
    const auto baddr_physical_page_dest = addr_physical - addr_physical_page;

    /**- MMap /dev/mem memory space. */
    void* const p_mmap = mmap(nullptr, cb_page, PROT_READ, MAP_SHARED, fd_devmem, addr_physical_page);
    if (p_mmap == MAP_FAILED)
    {
        close(fd_devmem);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not mmap address: " + std::to_string(addr_physical) + "!");
    }

    /**- Write to /dev/mem memory space. */
    *(static_cast<uint32_t*>(static_cast<void*>(static_cast<uint8_t*>(p_mmap) + baddr_physical_page_dest))) = val_write;

    /**- Close out resources. */
    close(fd_devmem);
    munmap(p_mmap, cb_page);
}

/*****************************************************************************/
/**
 * Function for reading from an address on /dev/mem to a vector.
 *
 * @param[in]  addr_physical The byte-aligned physical address to read from.
 * @param[in]  cwrgval_read  The count of words to read.
 * @param[out] vval_read     The vector of values read.
 *
 * @return None.
 *
 * @note None.
 *
 *****************************************************************************/
void ReadDevMemVector(const uintptr_t addr_physical, const size_t cwrgval_read, std::vector<uint32_t>& vval_read)
{
    if (!cwrgval_read)
    {
        /* No operation to be done. */
        return;
    }

    /**- Open /dev/mem driver. */
    const auto fd_devmem = open("/dev/mem", O_RDONLY);
    if (fd_devmem == -1)
    {
        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not open /dev/mem");
    }

    /* Calculate page-aligned address and offset. */
    const auto cb_page = sysconf(_SC_PAGESIZE);
    const auto addr_physical_page = addr_physical & ~(cb_page - 1);
    const auto baddr_physical_page_src = addr_physical - addr_physical_page;

    /* Calculate size of mmap necessary with page utilization. */
    const size_t cbrgval_read = sizeof(vval_read[0]) * vval_read.size();
    const size_t cb_mmap = (1 + ((cbrgval_read + baddr_physical_page_src - 1) / cb_page)) * cb_page;

    /**- MMap /dev/mem memory space. */
    void* const p_mmap = mmap(nullptr, cb_mmap, PROT_READ, MAP_SHARED, fd_devmem, addr_physical_page);
    if (p_mmap == MAP_FAILED)
    {
        close(fd_devmem);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not mmap address: " + std::to_string(addr_physical) + "!");
    }

    /**- Read from /dev/mem memory space to vector. */
    vval_read.reserve(cwrgval_read);
    std::memcpy(vval_read.data(), static_cast<uint8_t*>(p_mmap) + baddr_physical_page_src, cbrgval_read);

    /**- Close out resources. */
    close(fd_devmem);
    munmap(p_mmap, cb_mmap);
}

/******************************************************************************/
/**
 * Function for writing to an address on /dev/mem from a vector.
 *
 * @param[in] addr_physical The byte-aligned physical address to write to.
 * @param[in] vval_write    The vector of values to write.
 *
 * @return None.
 *
 * @note None.
 *
 ******************************************************************************/
void WriteDevMemVector(const uintptr_t addr_physical, const std::vector<uint32_t>& vval_write)
{
    if (!vval_write.size())
    {
        /* No operation to be done. */
        return;
    }

    /**- Open /dev/mem driver. */
    const auto fd_devmem = open("/dev/mem", O_RDWR);
    if (fd_devmem == -1)
    {
        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not open /dev/mem");
    }

    /* Calculate page-aligned address and offset. */
    const auto cb_page = sysconf(_SC_PAGESIZE);
    const auto addr_physical_page = addr_physical & ~(cb_page - 1);
    const auto baddr_physical_page_dest = addr_physical - addr_physical_page;

    /* Calculate size of mmap necessary with page utilization. */
    const size_t cbrgval_write = sizeof(vval_write[0]) * vval_write.size();
    const size_t cb_mmap = (1 + ((cbrgval_write + baddr_physical_page_dest - 1) / cb_page)) * cb_page;

    /**- MMap /dev/mem memory space. */
    void* const p_mmap = mmap(nullptr, cb_mmap, PROT_READ, MAP_SHARED, fd_devmem, addr_physical_page);
    if (p_mmap == MAP_FAILED)
    {
        close(fd_devmem);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not mmap address: " + std::to_string(addr_physical) + "!");
    }

    /**- Write vector to /dev/mem memory space. */
    std::memcpy(static_cast<uint8_t*>(p_mmap) + baddr_physical_page_dest, vval_write.data(), cbrgval_write);

    /**- Close out resources. */
    close(fd_devmem);
    munmap(p_mmap, cb_mmap);
}

/******************************************************************************/
/**
 * Function for reading from an address on /dev/mem to a file.
 *
 * @param[in] addr_physical The byte-aligned physical address to read from.
 * @param[in] cb_read       The count of bytes to read.
 * @param[in] sz_filename   The zero-terminated string of the local filename to write to.
 *
 * @return None.
 *
 * @note None.
 *
 ******************************************************************************/
void FileFromDevMem(const uintptr_t addr_physical, const size_t cb_read, const char* sz_filename)
{
    if (cb_read == 0)
    {
        /* No operation to be done. */
        return;
    }

    /**- Open /dev/mem driver. */
    const auto fd_devmem = open("/dev/mem", O_RDONLY);
    if (fd_devmem == -1)
    {
        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not open /dev/mem");
    }

    /* Calculate page-aligned address and offset. */
    const auto cb_page = sysconf(_SC_PAGESIZE);
    const auto addr_physical_page = addr_physical & ~(cb_page - 1);
    const auto baddr_physical_page_src = addr_physical - addr_physical_page;

    /* Calculate size of mmap necessary with page utilization. */
    const size_t cb_mmap = (1 + ((cb_read + baddr_physical_page_src - 1) / cb_page)) * cb_page;

    /**- MMap /dev/mem memory space. */
    void* const p_mmap = mmap(nullptr, cb_mmap, PROT_READ, MAP_SHARED, fd_devmem, addr_physical_page);
    if (p_mmap == MAP_FAILED)
    {
        close(fd_devmem);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not mmap address: " + std::to_string(addr_physical) + "!");
    }

    /**- Open file to write to. */
    FILE* const pfile_out = std::fopen(sz_filename, "wb");
    if (pfile_out == nullptr)
    {
        close(fd_devmem);
        munmap(p_mmap, cb_mmap);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not create file: " + static_cast<std::string>(sz_filename) + "!");
    }

    /**- Read /dev/mem memory space to file. */
    if (std::fwrite(static_cast<uint8_t*>(p_mmap) + baddr_physical_page_src, cb_read, 1, pfile_out) != 1)
    {
        close(fd_devmem);
        munmap(p_mmap, cb_mmap);
        std::fclose(pfile_out);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not write to file: " + static_cast<std::string>(sz_filename) + "!");
    }

    /**- Close out resources. */
    close(fd_devmem);
    munmap(p_mmap, cb_mmap);
    std::fclose(pfile_out);
}

/******************************************************************************/
/**
 * Function for writing an address on /dev/mem from a file.
 *
 * @param[in] addr_physical The byte-aligned physical address to write to.
 * @param[in] sz_filename   The zero-terminated string of the local filename to read from.
 *
 * @return None.
 *
 * @note None.
 *
 ******************************************************************************/
void DevmemFromFile(const uintptr_t addr_physical, const char* sz_filename)
{
    /**- Open file to read from. */
    FILE* const pfile_in = std::fopen(sz_filename, "rb");
    if (pfile_in == nullptr)
    {
        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not open file: " + static_cast<std::string>(sz_filename) + "1");
    }

    std::fseek(pfile_in, 0, SEEK_END);
    const auto cb_file = std::ftell(pfile_in);
    std::fseek(pfile_in, 0, SEEK_SET);

    if (cb_file == 0)
    {
        /* No operation to be done. */
        return;
    }

    /**- Open /dev/mem driver. */
    const auto fd_devmem = open("/dev/mem", O_RDWR);
    if (fd_devmem == -1)
    {
        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not open /dev/mem");
    }

    /* Calculate page-aligned address and offset. */
    const auto cb_page = sysconf(_SC_PAGESIZE);
    const auto addr_physical_page = addr_physical & ~(cb_page - 1);
    const auto baddr_physical_page_dest = addr_physical - addr_physical_page;

    /* Calculate size of mmap necessary with page utilization. */
    const size_t cb_mmap = (1 + ((cb_file + baddr_physical_page_dest - 1) / cb_page)) * cb_page;

    /**- MMap /dev/mem memory space. */
    void* const p_mmap = mmap(nullptr, cb_mmap, PROT_READ, MAP_SHARED, fd_devmem, addr_physical_page);
    if (p_mmap == MAP_FAILED)
    {
        close(fd_devmem);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not mmap address: " + std::to_string(addr_physical) + "!");
    }

    /**- Write file to /dev/mem memory space. */
    if (std::fread(static_cast<uint8_t*>(p_mmap) + baddr_physical_page_dest, 1, cb_file, pfile_in) != cb_file)
    {
        close(fd_devmem);
        std::fclose(pfile_in);
        munmap(p_mmap, cb_mmap);

        throw ExceptionPriority<std::runtime_error>(IDPriority::kIdErr, static_cast<std::string>(strerror(errno)) + " Could not read entire file: " + static_cast<std::string>(sz_filename) + "!");
    }

    /**- Close out resources. */
    close(fd_devmem);
    std::fclose(pfile_in);
    munmap(p_mmap, cb_mmap);
}
/** @} */ /* @addtogroup */