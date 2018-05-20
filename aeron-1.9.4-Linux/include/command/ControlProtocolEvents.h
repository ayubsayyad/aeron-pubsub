/*
 * Copyright 2014-2018 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef INCLUDED_AERON_COMMAND_CONTROLPROTOCOLEVENTS__
#define INCLUDED_AERON_COMMAND_CONTROLPROTOCOLEVENTS__

#include <cstdint>
#include <string>

namespace aeron { namespace command {

/**
* List of event types used in the control protocol between the
* media driver and the core.
*/
struct ControlProtocolEvents
{
    // Clients to Media Driver

    /** Add Publication */
    static const std::int32_t ADD_PUBLICATION = 0x01;
    /** Remove Publication */
    static const std::int32_t REMOVE_PUBLICATION = 0x02;
    /** Add Exclusive Publication */
    static const std::int32_t ADD_EXCLUSIVE_PUBLICATION = 0x03;
    /** Add Subscriber */
    static const std::int32_t ADD_SUBSCRIPTION = 0x04;
    /** Remove Subscriber */
    static const std::int32_t REMOVE_SUBSCRIPTION = 0x05;
    /** Keepalive from Client */
    static const std::int32_t CLIENT_KEEPALIVE = 0x06;
    /** Add Destination */
    static const std::int32_t ADD_DESTINATION = 0x07;
    /** Remove Destination */
    static const std::int32_t REMOVE_DESTINATION = 0x08;
    /** Add Counter */
    static const std::int32_t ADD_COUNTER = 0x09;
    /** Remove Counter */
    static const std::int32_t REMOVE_COUNTER = 0x0A;
    /** Client Close */
    static const std::int32_t CLIENT_CLOSE = 0x0B;

    // Media Driver to Clients

    /** Error Response */
    static const std::int32_t ON_ERROR = 0x0F01;
    /** New image Buffer Notification */
    static const std::int32_t ON_AVAILABLE_IMAGE = 0x0F02;
    /** New publication Buffer Notification */
    static const std::int32_t ON_PUBLICATION_READY = 0x0F03;
    /** Operation Succeeded */
    static const std::int32_t ON_OPERATION_SUCCESS = 0x0F04;
    /** Inform client of timeout and removal of inactive image */
    static const std::int32_t ON_UNAVAILABLE_IMAGE = 0x0F05;
    /** New Exclusive Publication Buffer notification */
    static const std::int32_t ON_EXCLUSIVE_PUBLICATION_READY = 0x0F06;
    /** New subscription notification */
    static const std::int32_t ON_SUBSCRIPTION_READY = 0x0F07;
    /** New counter notification */
    static const std::int32_t ON_COUNTER_READY = 0x0F08;
    /** inform clients of removal of counter */
    static const std::int32_t ON_UNAVAILABLE_COUNTER = 0x0F09;
};

}}
#endif
