/* This file makes use of Doxygen syntax for generating documentation. */
/******************************************************************************/
/**
 * @file access_devmem.hpp
 * @brief File containing /dev/mem access functions.
 *
 * This contains the function prototypes for an API to the /dev/mem driver. It
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

#ifndef ACCESS_DEVMEM_HPP
#define ACCESS_DEVMEM_HPP

 /******************************** Include Files *******************************/
#include <cstdint>
#include <vector>

/**************************** Constant Definitions ****************************/

/****************************** Type Definitions ******************************/

/***************************** Function Prototypes ****************************/

/***************************** Class Declarations *****************************/

/*
 * /dev/mem functions defined in access_devmem.cpp
 */
void ReadDevMem(const uintptr_t addr_physical, uint32_t& val_read);
void WriteDevMem(const uintptr_t addr_physical, const uint32_t val_write);
void ReadDevMemVector(const uintptr_t addr_physical, const size_t cwrgval_read, std::vector<uint32_t>& vval_read);
void WriteDevMemVector(const uintptr_t addr_physical, const std::vector<uint32_t>& vval_write);
void FileFromDevMem(const uintptr_t addr_physical, const size_t cb_read, const char* sz_filename);
void DevmemFromFile(const uintptr_t addr_physical, const char* sz_filename);

#endif  /* #ifndef ACCESS_DEVMEM_HPP */
/** @} */ /* @addtogroup */