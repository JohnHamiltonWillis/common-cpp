/* This file makes use of Doxygen syntax for generating documentation. */
/******************************************************************************/
/**
 * @file ExceptionPriority.hpp
 * @brief Class definition for extending exceptions with a priority parameter.
 *
 * This contains a templated class definition for extending any exception that
 * has a std::string constructor with a priority parameter.
 *
 * @author John H. Willis (willisjh)
 * @date 19 September 2021
 *
 * @bug No known bugs.
 *
 * @addtogroup ExceptionPriority
 * @{
 *
 ******************************************************************************/

#ifndef EXCEPTION_PRIORITY_HPP
#define EXCEPTION_PRIORITY_HPP

/******************************** Include Files *******************************/
#include <exception>
#include <string>

/**************************** Constant Definitions ****************************/

/****************************** Type Definitions ******************************/
enum class IDPriority
{
	  kIdTrace     /* Only for "tracing" the code and trying to find one part of a function specifically. */
	, kIdDebug     /* Debugging is not useful for day to day operations. This type of level is only useful for developers who are identifying and removing existing and potential errors in the software code. */
	, kIdInfo      /* Logs containing information about normal operations. These messages do not require any action whatsoever. These can be used for creating reports, or planning for capacity. Examples are when an application started or ended successfully, throughput metrics, etc. */
	, kIdNotice    /* This is usually a normal condition but that must be considered. An event that could lead into a potential issue in the future which might require special care but no immediate action is needed. Examples are unsupported commands, installed license notifications, root admin actions, etc. */
	, kIdWarning   /* A notice that if action is not taken an error will occur. Warnings are usually message logs such as “file system is 89%” or “there is only 2GB remaining of free space.” */
	, kIdErr       /* A failure in a system that is usually not urgent but should be dealt within a reasonable time frame. A few examples of errors are when an application exceeded the storage, memory allocation, file not found, etc. */
	, kIdCrit      /* This level indicates that something bad is about to happen and must be corrected right away. It usually indicates failure in a primary system (or link) of a backup solution. It could be a failure in a system’s primary application or an expired license. */
	, kIdAlert     /* An alert is a condition that must be dealt with immediately. Alert indicates that something already happened. It could be a corrupted database or loss in the primary connection to the ISP. */
	, kIdEmerg     /* An urgent crisis that must be dealt with immediately. This condition is usually affecting everything in the network, all remote sites, applications, and servers. This kind of priority is rare but it could be destructive. */
};

/***************************** Function Prototypes ****************************/

/***************************** Class Declarations *****************************/

/*
 * ExceptionPriority class defined in ExceptionPriority.hpp.
 */
template<typename T_EXCEPTION>
class ExceptionPriority : public T_EXCEPTION
{
	public:
		/******************************************************************************/
		/**
		 * @brief Constructor for initialization.
		 *
		 * @param[in] id_except_priority The identifier for the exception priority.
		 * @param[in] message            The string for the exception message.
		 *
		 * @return None.
		 *
		 * @note T_EXCEPTION type requires std::string constructor.
		 *
		 ******************************************************************************/
		ExceptionPriority(const IDPriority id_except_priority, const std::string message)
			: T_EXCEPTION(message)
			, id_except_priority_(id_except_priority)
		{
		}

		/******************************************************************************/
		/**
		* @brief Method for returning the priority of the exception.
		*
		* @param None.
		*
		* @return id_except_priority The identifier of the exception priority.
		*
		* @note
		*
		******************************************************************************/
		IDPriority GetIdExceptPriority() const noexcept
		{
			return id_except_priority_;
		}

	private:
		IDPriority id_except_priority_;
};

#endif  /* #ifndef EXCEPTION_PRIORITY_HPP */
/** @} */ /* @addtogroup */
