/******************************************************************************/
/**
 *
 * @file QueueThreadSafeExt.hpp
 * @brief Extension of QueueThreadSafe.
 *
 * This contains the template class definition for an extension of the
 * QueueThreadSafe class. This extends the class by providing additional
 * features that are useful for threaded use of std::queue.
 * methods.
 *
 * @author John H. Willis (willisjh)
 * @date 24 November 2021
 *
 * @bug No known bugs.
 *
 * @addtogroup QueueThreadSafeExt
 * @{
 *
 ******************************************************************************/

#ifndef QUEUETHREADSAFEEXT_HPP
#define QUEUETHREADSAFEEXT_HPP

 /******************************** Include Files *******************************/
#include <condition_variable>

/**************************** Constant Definitions ****************************/

/****************************** Type Definitions ******************************/

/**************************** Template Definitions ****************************/

template<class T, class Container = std::deque<T>>
class QueueThreadSafeExt : public QueueThreadSafe<T, Container>
{
public:
    /******************************************************************************/
    /**
     * @brief Method for waiting current thread until queue reaches requested size.
     *
     * @return None.
     *
     * @note Thread wake ups occur at every push().
     *
     ******************************************************************************/
    size_t sizeWait(size_t cElementReq) const
    {
        std::unique_lock<std::mutex> lock(QueueThreadSafe::m_mutexQueueThreadUnsafe);

        do
        {
            m_condSizeWait.wait(m_mutexQueueThreadUnsafe, [] { return m_fPush; });
            m_fPush = false;
        } 
        while (QueueThreadSafe::m_qThreadUnsafe.size() < cElementReq);

        auto cElementLast = QueueThreadSafe::m_qThreadUnsafe.size();
        lock.unlock();
        return cElementLast;
    }

    /******************************************************************************/
    /**
     * @brief Overriding method for push() necessary for sizeWait().
     *
     * @return None.
     *
     * @note This method will only notify ONE thread of any pushes.
     *
     ******************************************************************************/
    void push(const T& value)
    {
        std::lock_guard<std::mutex> lock(QueueThreadSafe::m_mutexQueueThreadUnsafe);

        QueueThreadSafe::m_qThreadUnsafe.push(value);
        
        m_fPush = true;
        m_condSizeWait.notify_one();
    }

    /******************************************************************************/
    /**
     * @brief Method for combining front() and pop() for same element.
     *
     * @return None.
     *
     * @note This method is necessary to guarantee that front() and pop() operation
     * occurs for the same element.
     *
     ******************************************************************************/
    T frontAndPop()
    {
        std::lock_guard<std::mutex> lock(QueueThreadSafe::m_mutexQueueThreadUnsafe);

        auto elementFront = QueueThreadSafe::m_qThreadUnsafe.front();
        QueueThreadSafe::m_qThreadUnsafe.pop();

        return elementFront;
    }

private:
    std::condition_variable m_condSizeWait;
    bool m_fPush;

};

#endif /* #ifndef QUEUETHREADSAFEEXT_HPP */
/** @} */ /* @addtogroup */