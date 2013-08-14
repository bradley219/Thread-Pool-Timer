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
#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <chrono>
#include <functional>
#include "thread_pool_timer_exception.hpp"

namespace ThreadPoolTimer 
{

class TimerException : public ThreadPoolTimerException
{
    public:
        TimerException( const char *msg ) :
            ThreadPoolTimerException(msg) {}
};

class TimerImpl;

class Timer
{
    public:
        Timer( std::function<void()> task );
        ~Timer();

        void setInterval( const std::chrono::nanoseconds &interval );

        void setEnabled( bool enabled );
        bool getEnabled();

        static void stopRunLoop();
        static void startRunLoop();

    private:
        TimerImpl *_impl;
};


}

#endif
