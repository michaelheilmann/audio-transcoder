#pragma once

extern "C" {

#include "Nucleus/FileSystem.h"

} // extern "C"

#include <string>
#include <memory>
#include <stdexcept>

namespace nucleus {

// An input iterator for iterating (non-recursively) over the contents of a directory.
// It is basically a wrapper around the Nucleus_DirectoryStream (formerly known as
// Nucleus_DirectoryEnumerator) (https://github.com/primordialmachine/nucleus) and
// has semantics similar to the directory iterators in std::filesystem.
struct directory_iterator
{
    using iterator_category = ::std::input_iterator_tag;
    using difference_type = ::std::ptrdiff_t;
    using value_type = ::std::string;
    using reference = ::std::string&;
    using pointer = ::std::string*;

    static std::shared_ptr<Nucleus_DirectoryEnumerator> makeDirectoryEnumerator()
    {
        Nucleus_Status status;
        Nucleus_DirectoryEnumerator *directoryEnumerator;
        status = Nucleus_createDirectoryEnumerator(&directoryEnumerator);
        if (status) throw ::std::runtime_error("Nucleus_createDirectoryEnumerator failed");
        return std::shared_ptr<Nucleus_DirectoryEnumerator>(directoryEnumerator, [](Nucleus_DirectoryEnumerator *p) { Nucleus_DirectoryEnumerator_destroy(p); });
    }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    directory_iterator()
        : m_impl(makeDirectoryEnumerator()),
          m_filename()
    {}

    directory_iterator(const ::std::string& pathname)
        : m_impl(makeDirectoryEnumerator()),
          m_filename()
    {
        Nucleus_Status status;
        // Open the enumerator.
        // Remark: std::shared_ptr<T>::get and std::string::c_str() are noexcept since C++ 11 so no exception handling is required.         
        status = Nucleus_DirectoryEnumerator_open(m_impl.get(), pathname.c_str());
        if (status)
        { throw ::std::runtime_error("Nucleus_DirectoryEnumerator_open failed"); }
        // Check if there is a value.
        Nucleus_Boolean hasValue;
        status = Nucleus_DirectoryEnumerator_hasValue(m_impl.get(), &hasValue);
        if (status)
        { throw ::std::runtime_error("Nucleus_DirectoyEnumerator_hasValue failed"); }
        // If there is a value, update the cached value.
        if (hasValue)
        {
            const char *bytesOfFilename;
            Nucleus_Size numberOfBytesInFilename;
            status = Nucleus_DirectoryEnumerator_getValue(m_impl.get(), &bytesOfFilename, &numberOfBytesInFilename);
            if (status)
            { throw ::std::runtime_error("Nucleus_DirectoryEnumerator_getValue failed"); }
            m_filename = bytesOfFilename; // TODO: Wasn't there std::string(const char *, size_t)?
        }
    }

    // Same behaviour as std::filesystem; copying shares the underlying state.
    directory_iterator(const directory_iterator& other)
        : m_impl(other.m_impl),
          m_filename(other.m_filename)
    {}

    ~directory_iterator()
    {
        m_impl = nullptr;
    }

    // Same behaviour as std::filesystem; copying shares the underlying state.
    directory_iterator& operator=(const directory_iterator& other)
    { m_impl = other.m_impl; m_filename = other.m_filename; return *this; }

    // Equal if they share the same underlying state or if the different underlying states are both closed.
    bool operator==(const directory_iterator& other) const
    { 
        if (m_impl == other.m_impl) return true;
        return m_impl->state == Nucleus_DirectoryEnumerator_State_Closed
            && m_impl->state == Nucleus_DirectoryEnumerator_State_Closed;
    }

    directory_iterator& operator++()
    {
        Nucleus_Status status;
        // Increment regardless of the state.
        status = Nucleus_DirectoryEnumerator_nextValue(m_impl.get());
        if (status)
        {
            if (status == Nucleus_Status_FileClosed)
            { return *this; }
            else
            { throw ::std::runtime_error("Nucleus_DirectoyEnumerator_nextValue failed"); }
        }
        // Check if there is a value.
        Nucleus_Boolean hasValue;
        status = Nucleus_DirectoryEnumerator_hasValue(m_impl.get(), &hasValue);
        if (status)
        { throw ::std::runtime_error("Nucleus_DirectoyEnumerator_hasValue failed"); }
        // If there is a value, update the cached value.
        if (hasValue)
        {
            const char *bytesOfFilename;
            Nucleus_Size numberOfBytesInFilename;
            status = Nucleus_DirectoryEnumerator_getValue(m_impl.get(), &bytesOfFilename, &numberOfBytesInFilename);
            if (status)
            { throw ::std::runtime_error("Nucleus_DirectoryEnumerator_getValue failed"); }
            m_filename = bytesOfFilename;
        }
        return *this;
    }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    // Easy as defined in terms of already implemented C++ operators.
    bool operator!=(const directory_iterator& other) const
    { return !((*this) == other); }

    // Easy as defined in terms of already implemented C++ operators.
    directory_iterator operator++(int) const
    {
        auto t = *this;
        ++t;
        return t;
    }

    // Easy as C++.
    const std::string& operator*() const
    { return m_filename; }

    // Easy as C++.
    const std::string* operator->() const
    { return &m_filename; }

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

private:
    std::shared_ptr<Nucleus_DirectoryEnumerator> m_impl;
    ::std::string m_filename;

}; // struct directory_iterator

} // namespace nucleus
