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
#ifndef _TIMER_SOURCE_
#define _TIMER_SOURCE_

#include "timer_impl.hpp"
#include "tp_timer.hpp"

namespace ThreadPoolTimer
{

Timer::Timer( std::function<void()> callback, const std::chrono::nanoseconds &interval )
{
    _impl = new TimerImpl( callback, interval );
}

Timer::~Timer()
{
    delete _impl;
}

void Timer::setInterval( const std::chrono::nanoseconds &interval )
{
    _impl->setInterval(interval);
}

void Timer::setEnabled( bool enabled )
{
    _impl->setEnabled(enabled);
}

bool Timer::getEnabled()
{
    return _impl->getEnabled();
}

void Timer::stopRunLoop()
{
    TimerImpl::stopRunLoop();
}

void Timer::startRunLoop()
{
    TimerImpl::startRunLoop();
}

}

#endif
