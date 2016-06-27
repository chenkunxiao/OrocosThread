#ifndef CORELIB_DATAOBJECTINTERFACE_HPP
#define CORELIB_DATAOBJECTINTERFACE_HPP

#include <boost/shared_ptr.hpp>
namespace RTT
{
  namespace base
  {
    template <class T>
    class DataObjectInterface
    {
    public:
      typedef typename boost::shared_ptr<DataObjectInterface<T> > shared_ptr;
      DataObjectInterface()
      {
      }

      virtual ~DataObjectInterface(){}
      typedef T DataType;
      virtual void Get(DataType& pull)const = 0;
      virtual DataType Get() const = 0;

      virtual void Set(const DataType& push) = 0;
      virtual void data_sample( const DataType& sample ) = 0;
    };

  }
}
#endif
