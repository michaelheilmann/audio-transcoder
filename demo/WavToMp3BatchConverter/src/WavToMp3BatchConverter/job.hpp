#pragma once

#include <string>

namespace nucleus {

/// @brief A conversion job.
/// @details Consists of a source filename and target filename.
struct job
{
private:
    ::std::string m_source;
    ::std::string m_target;

public:
    job(const ::std::string& source, const ::std::string& target)
        : m_source(source), m_target(target)
    {}

    const ::std::string& get_source() const noexcept
    { return m_source; }

    const ::std::string& get_target() const noexcept
    { return m_target; }

}; // struct job

/// @brief Get if a string is a true suffix of another string.
/// @param w the string
/// @param v the string which is tested if it is a true suffix of @a w
/// @return @a true if @a v is a true suffix of @a w, @a false otherwise
template <typename CharType>
bool is_true_suffix(const std::basic_string<CharType>& w, const std::basic_string<CharType>& v)
{
    if (v.size() >= w.size()) return false;
    return ::std::equal(v.crbegin(), v.crend(), w.crbegin());
}

/// @brief Get if a character is a true suffix of a string.
/// @param w the string
/// @param v the character which is tested if it is a true suffix of @a w
/// @return @a true if @a v is a true suffix of @a w, @a false otherwise
template <typename CharType>
bool is_true_suffix(const ::std::basic_string<CharType>& w, const CharType& v)
{ 
    if (w.size() < 2) return false; // If |w| < 2, then v can not be a true suffix of w.
    return v == w[w.size()-1];
}

} // namespace nucleus
