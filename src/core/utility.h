#ifndef UTILITY_H
#define UTILITY_H

namespace alive {

template<typename T>
bool in_bound(T min, T max, T val)
{
    return val >= min && val <= max;
}

class BooleanGuard
{
public:
    BooleanGuard(bool &var)
        : m_bool(var)
    {
        m_bool = true;
    }
    ~BooleanGuard() { m_bool = false; }

private:
    bool &m_bool;
};
} // namespace alive

#endif // UTILITY_H
