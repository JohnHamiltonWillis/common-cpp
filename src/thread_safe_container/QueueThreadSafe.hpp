/******************************************************************************/
/**
 *
 * @file QueueThreadSafe.hpp
 * @brief Thread-safe wrapper of std::queue.
 *
 * This contains the template class definition for the thread-safe wrapper of 
 * std::queue container. See std::queue documentation for implemented
 * methods.
 *
 * @author John H. Willis (willisjh)
 * @date 23 November 2021
 *
 * @bug No known bugs.
 *
 * @addtogroup QueueThreadSafe
 * @{
 *
 ******************************************************************************/

#ifndef QUEUETHREADSAFE_HPP
#define QUEUETHREADSAFE_HPP

 /******************************** Include Files *******************************/
#include <queue>
#include <mutex>

/**************************** Constant Definitions ****************************/

/****************************** Type Definitions ******************************/

/**************************** Template Definitions ****************************/

template<class T, class Container = std::deque<T>>
class QueueThreadSafe
{
public:
    /******************************************************************************/
    /**
     * @brief Default constructor for QueueThreadSafe.
     *
     * @return None.
     *
     * @note Using implicit default constructor.
     *
     ******************************************************************************/
    QueueThreadSafe() = default;

    /******************************************************************************/
    /**
     * @brief Copy constructor for QueueThreadSafe.
     *
     * @return None.
     *
     * @note Disabled implicit copy constructor.
     *
     ******************************************************************************/
    QueueThreadSafe(const QueueThreadSafe& qOriginal) = delete;

    /******************************************************************************/
    /**
     * @brief Copy assignment operator for QueueThreadSafe.
     *
     * @return None.
     *
     * @note Disabled implicit copy assignment operator.
     *
     ******************************************************************************/
    QueueThreadSafe& operator=(const QueueThreadSafe& qOriginal) = delete;

    /******************************************************************************/
    /**
     * @brief Move constructor for QueueThreadSafe.
     *
     * @return None.
     *
     * @note Disabled implicit move constructor.
     *
     ******************************************************************************/
    QueueThreadSafe(QueueThreadSafe&& qOriginal) = delete;

    /******************************************************************************/
    /**
     * @brief Move assignment operator for QueueThreadSafe.
     *
     * @return None.
     *
     * @note Disabled implicit move assignment operator.
     *
     ******************************************************************************/
    QueueThreadSafe& operator=(QueueThreadSafe&& qOriginal) = delete;

    /******************************************************************************/
    /**
     * @brief Deconstructor for QueueThreadSafe.
     *
     * @return None.
     *
     * @note None.
     *
     ******************************************************************************/
    ~QueueThreadSafe()
    {
        std::lock_guard<std::mutex> lock(m_mutexQueueThreadUnsafe);
    }


    T front()
    {
        std::lock_guard<std::mutex> lock(m_mutexQueueThreadUnsafe);

       return  m_qThreadUnsafe.front();
    }

    T back()
    {
        std::lock_guard<std::mutex> lock(m_mutexQueueThreadUnsafe);

        return m_qThreadUnsafe.back();
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutexQueueThreadUnsafe);

        return m_qThreadUnsafe.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(m_mutexQueueThreadUnsafe);

        return m_qThreadUnsafe.size();
    }

    void push(const T& value)
    {
        std::lock_guard<std::mutex> lock(m_mutexQueueThreadUnsafe);
        m_qThreadUnsafe.push(value);
    }

    void pop()
    {
        std::lock_guard<std::mutex> lock(m_mutexQueueThreadUnsafe);
        m_qThreadUnsafe.pop();
    } 

private:
    std::queue<T, Container> m_qThreadUnsafe;
    std::mutex m_mutexQueueThreadUnsafe;

};

#endif /* #ifndef QUEUETHREADSAFE_HPP */
/** @} */ /* @addtogroup */