#include "Types.hpp"

//#include "rtt-config.h"

namespace RTT
{
    using namespace std;
    using namespace detail;

    namespace types
    {
        TypeInfoRepository::shared_ptr Types()
        {
          return TypeInfoRepository::Instance();
        }
    }
}
