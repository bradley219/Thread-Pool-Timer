/*
 * Licensed to Bradley J. Snyder under one or more contributor 
 * license agreements.  See the NOTICE file distributed with 
 * this work for additional information regarding copyright 
 * ownership.  The ASF licenses this file to you under the 
 * Apache License, Version 2.0 (the "License"); you may not use 
 * this file except in compliance with the License.  You may 
 * obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#ifndef _TIMER_IMPL_HPP_
#define _TIMER_IMPL_HPP_

#include <chrono>
#include <mutex>
#include <iostream>
#include <functional>

namespace ThreadPoolTimer
{

class TimerRunLoop;

class TimerImpl
{
    public:
        TimerImpl( std::function<void()> task, const std::chrono::nanoseconds &interval = std::chrono::nanoseconds(0) );
        ~TimerImpl();
        
        static void stopRunLoop();
        static void startRunLoop();

        void setInterval( const std::chrono::nanoseconds &interval );
        const std::chrono::nanoseconds &getInterval();
        
        void setEnabled( bool enabled );
        bool getEnabled();

        void setNextTrigger( const std::chrono::steady_clock::time_point &trigger );
        std::chrono::steady_clock::time_point &getNextTrigger();

        void trigger();

    private:
        std::function<void()> _callback;

        TimerRunLoop &_runLoop;

        std::chrono::nanoseconds _interval;
        std::chrono::steady_clock::time_point _nextTrigger;
        
        bool _enabled;
};

}

#endif
