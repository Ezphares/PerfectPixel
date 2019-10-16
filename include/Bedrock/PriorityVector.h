#pragma once

#include <vector>

namespace perfectpixel { namespace bedrock {

template <typename T>
class PriorityVector
{
public:
    void insert(uint16_t priority, const T &element)
    {
        if (m_priorities.empty() || m_priorities.back() <= priority)
        {
            m_priorities.push_back(priority);
            m_data.push_back(element);

			return;
        }

        for (std::size_t index = 0; index < m_priorities.size(); ++index)
        {
            if (m_priorities[index] > priority)
            {
                m_priorities.insert(m_priorities.begin() + index, priority);
                m_data.insert(m_data.begin() + index, element);
            }
        }
    }

    inline typename std::vector<T>::iterator begin()
    {
        return m_data.begin();
    }

    inline typename std::vector<T>::iterator end()
    {
        return m_data.end();
    }

    inline typename std::vector<T>::reverse_iterator rbegin()
    {
        return m_data.rbegin();
    }

     inline typename std::vector<T>::reverse_iterator rend()
    {
        return m_data.rbegin();
    }

private:
    std::vector<T> m_data;
    std::vector<uint16_t> m_priorities;
};

}} // namespace perfectpixel::bedrock
