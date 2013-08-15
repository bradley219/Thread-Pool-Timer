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

/// Asynchronous timer object
class Timer
{
    public:

        /// Construct a Timer object with a callback
        /**
         * @param callback Any callable target (lambda expressions, bind expressions, etc)
         * @param interval See setInterval( const std::chrono::nanoseconds &interval );
         */
        Timer( std::function<void()> callback, const std::chrono::nanoseconds &interval = std::chrono::nanoseconds(0) );
        ~Timer();


        /// Set the timer's event interval
        /**
         * @param interval The timing interval specified in nanoseconds. Any std::chrono::duration
         * value can be converted to nanoseconds with std::chrono::duration_cast<std::chrono::nanoseconds(x).
         * If the timer is enabled when the interval is set, the timer will first be disabled, the
         * interval set, and then re-enabled. Setting the interval to 0 will disable the timer.
         */
        void setInterval( const std::chrono::nanoseconds &interval );

        /// Enable or disable the timer
        /**
         * Enabling the timer registers the timer with the thread pool, and events will begin to 
         * trigger at the specified interval. Attempting to enable the timer without previously
         * setting an interval, or while the interval is set to zero, will throw a TimerException.
         * Disabling the timer de-registers it from the thread pool. No further events will fire 
         * after setEnabled() returns. It is possible, however, that an event may fire after 
         * setEnabled() is called, but before it returns.
         * @param enabled true to enable the timer; false to disable
         */
        void setEnabled( bool enabled );

        /// Get the timer's enabled status
        bool getEnabled();

        /// Stop the thread pool's run loop
        /**
         * After the thread pool's run loop is stopped, no further events will trigger on any
         * registered Timer objects.
         */
        static void stopRunLoop();

        /// Start the thread pool's run loop
        /**
         * This should only be called if the run loop has previously been stopped with stopRunLoop().
         * The run loop is normally started automatically when the first Timer object is enabled.
         */
        static void startRunLoop();

    private:
        TimerImpl *_impl;
};


}

#endif
