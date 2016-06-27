#ifndef ORO_CORELIB_TYPES_HPP
#define ORO_CORELIB_TYPES_HPP

//#include "TypeInfo.hpp"
//#include "TypeConstructor.hpp"
#include "TypeInfoRepository.hpp"

/**
 * \file Types.hpp We need some information on types if we want to make
 * properties, variables or corba types of them, the TypeInfoRepository
 * provides that information.
 */

namespace RTT
{
  namespace types
  {
    /**
     * Obtain a pointer to the global type system.
     * This is a short notation for TypeInfoRepository::Instance().
     */
    TypeInfoRepository::shared_ptr Types();
  }
}

#endif
