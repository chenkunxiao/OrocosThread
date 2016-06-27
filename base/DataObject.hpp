#ifndef _CORELIB_DATAOBJECT_HPP
#define _CORELIB_DATAOBJECT_HPP
/**
 * @file DataObject.hpp
 * We can't use typedefs since C++ doesn't allow it for
 * templated classes without specifying all the template
 * parameters.
 */
//#define OROBLD_OS_NO_ASM
#if defined(OROBLD_OS_NO_ASM)
#include "DataObjectLocked.hpp"
#else
#include "DataObjectLocked.hpp"
#include "DataObjectLockFree.hpp"
#endif

namespace RTT
{ namespace base {

    /**
     * This object represents the default thread-safe data object implementation used
     * by Orocos objects.
     * @ingroup PortBuffers
     */
    template< class T >
    class DataObject
#if defined(OROBLD_OS_NO_ASM)
        : public DataObjectLocked<T>
#else
        : public DataObjectLockFree<T>
#endif
    {
    public:
        /**
         * The type of the data.
         */
        typedef T DataType;
        typedef T value_t;

    public:
        /**
         * Create a data object for holding a user defined struct.
         */
        DataObject( const T& initial_value = T())
#if defined(OROBLD_OS_NO_ASM)
            : DataObjectLocked<T>(initial_value)
#else
            : DataObjectLockFree<T>(initial_value)
#endif
        {}
    };
}}

#endif
