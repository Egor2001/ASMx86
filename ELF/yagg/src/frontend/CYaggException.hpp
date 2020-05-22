#ifndef YAGG_EXCEPTION_HPP
#define YAGG_EXCEPTION_HPP

#include <stdexcept>
#include <string_view>

class CYaggException : public std::runtime_error
{
public:
    CYaggException(const char* what, const std::string_view& where):
        std::runtime_error(what),
        where_(where)
    {}

    CYaggException             (const CYaggException&) = default;
    CYaggException& operator = (const CYaggException&) = default;
    CYaggException             (CYaggException&&) = default;
    CYaggException& operator = (CYaggException&&) = default;

    virtual ~CYaggException() = default;

    virtual std::string_view where() const noexcept
    {
        return where_;
    }

private:
    std::string_view where_;
};

#endif //YAGG_EXCEPTION_HPP
