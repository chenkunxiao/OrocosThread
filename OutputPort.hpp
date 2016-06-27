/***************************************************************************
  tag: Peter Soetens  Thu Oct 22 11:59:08 CEST 2009  OutputPort.hpp

                        OutputPort.hpp -  description
                           -------------------
    begin                : Thu October 22 2009
    copyright            : (C) 2009 Sylvain Joyeux
    email                : sylvain.joyeux@m4x.org

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public                   *
 *   License as published by the Free Software Foundation;                 *
 *   version 2 of the License.                                             *
 *                                                                         *
 *   As a special exception, you may use this file as part of a free       *
 *   software library without restriction.  Specifically, if other files   *
 *   instantiate templates or use macros or inline functions from this     *
 *   file, or you compile this file and link it with other files to        *
 *   produce an executable, this file does not by itself cause the         *
 *   resulting executable to be covered by the GNU General Public          *
 *   License.  This exception does not however invalidate any other        *
 *   reasons why the executable file might be covered by the GNU General   *
 *   Public License.                                                       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public             *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/


#ifndef ORO_OUTPUT_PORT_HPP
#define ORO_OUTPUT_PORT_HPP

#include "base/OutputPortInterface.hpp"
#include "base/DataObject.hpp"
#include "internal/ConnFactory.hpp"
#include "InputPort.hpp"
#include "internal/rtt-internal-fwd.hpp"
#include <iostream>
using namespace std;
using namespace RTT::detail;

namespace RTT
{
    template<typename T>
    class OutputPort : public base::OutputPortInterface
    {
        friend class internal::ConnInputEndpoint<T>;

        bool do_write(typename base::ChannelElement<T>::param_t sample, const internal::ConnectionManager::ChannelDescriptor& descriptor)
        {
            typename base::ChannelElement<T>::shared_ptr output
                = boost::static_pointer_cast< base::ChannelElement<T> >(descriptor.get<1>());
            if (output->write(sample))
                return false;
            else
            {
              cout << "A channel of port " << getName() << " has been invalidated during write(), it will be removed" << endl;
                return true;
            }
        }

        bool do_init(typename base::ChannelElement<T>::param_t sample, const internal::ConnectionManager::ChannelDescriptor& descriptor)
        {
            typename base::ChannelElement<T>::shared_ptr output
                = boost::static_pointer_cast< base::ChannelElement<T> >(descriptor.get<1>());
            if (output->data_sample(sample))
                return false;
            else
            {
              cout << "A channel of port " << getName() << " has been invalidated during setDataSample(), it will be removed" << endl;
                return true;
            }
        }

        virtual bool connectionAdded( base::ChannelElementBase::shared_ptr channel_input, ConnPolicy const& policy ) {
            // Initialize the new channel with last written data if requested
            // (and available)

            // This this the input channel element of the whole connection
            typename base::ChannelElement<T>::shared_ptr channel_el_input =
                static_cast< base::ChannelElement<T>* >(channel_input.get());

            if (has_initial_sample)
            {
                T const& initial_sample = sample->Get();
                if ( channel_el_input->data_sample(initial_sample) ) {
                    if ( has_last_written_value && policy.init )
                        return channel_el_input->write(initial_sample);
                    return true;
                } else {
                    cout << "Failed to pass data sample to data channel. Aborting connection."<< endl;
                    return false;
                }
            }
            // even if we're not written, test the connection with a default sample.
            return channel_el_input->data_sample( T() );
        }

        /// True if \c sample has been set at least once by a call to write()
        bool has_last_written_value;
        /// True if \c sample has been written at least once, either by calling
        // data_sample or by calling write() with keeps_next_written_value or
        // keeps_last_written_value to true
        bool has_initial_sample;
        /// If true, the next call to write() will save the sample in \c sample.
        // This is used to initialize connections with a known sample
        bool keeps_next_written_value;
        /// If true, all calls to write() will save the sample in \c sample.
        // This is used to allow the use of the 'init' connection policy option
        bool keeps_last_written_value;
        typename base::DataObjectInterface<T>::shared_ptr sample;

        OutputPort( OutputPort const& orig );
        OutputPort& operator=(OutputPort const& orig);

    public:
        OutputPort(std::string const& name = "unnamed", bool keep_last_written_value = true)
            : base::OutputPortInterface(name)
            , has_last_written_value(false)
            , has_initial_sample(false)
            , keeps_next_written_value(false)
            , keeps_last_written_value(false)
            , sample( new base::DataObject<T>() )
        {
            if (keep_last_written_value)
                keepLastWrittenValue(true);
        }

        void keepNextWrittenValue(bool keep)
        {
            keeps_next_written_value = keep;
        }

        void keepLastWrittenValue(bool keep)
        {
            keeps_last_written_value = keep;
        }

        bool keepsLastWrittenValue() const { return keeps_last_written_value; }

        T getLastWrittenValue() const
        {
            return sample->Get();
        }

        bool getLastWrittenValue(T& sample) const
        {
            if (has_last_written_value)
            {
                this->sample->Get(sample);
                return true;
            }
            return false;
        }

        void setDataSample(const T& sample)
        {
            this->sample->Set(sample);
            has_initial_sample = true;
            has_last_written_value = false;

            cmanager.delete_if( boost::bind(
                        &OutputPort<T>::do_init, this, boost::ref(sample), _1)
                    );
        }

        void write(const T& sample)
        {
            if (keeps_last_written_value || keeps_next_written_value)
            {
                keeps_next_written_value = false;
                has_initial_sample = true;
                this->sample->Set(sample);
            }
            has_last_written_value = keeps_last_written_value;

            cmanager.delete_if( boost::bind(
                        &OutputPort<T>::do_write, this, boost::ref(sample), boost::lambda::_1)
                    );
        }

        virtual base::PortInterface* clone() const
        {
          return new OutputPort<T>(this->getName());
        }

        virtual base::PortInterface* antiClone() const
        { return new InputPort<T>(this->getName()); }

        using base::OutputPortInterface::createConnection;

        virtual bool createConnection(base::InputPortInterface& input_port, ConnPolicy const& policy)
        {
           return internal::ConnFactory::createConnection(*this, input_port, policy);
           //return internal::testconn::testconnname();
        }
    };

}

#endif

