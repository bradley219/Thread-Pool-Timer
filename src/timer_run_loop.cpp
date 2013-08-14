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
#ifndef _TIMER_RUN_LOOP_SOURCE_
#define _TIMER_RUN_LOOP_SOURCE_

#include <thread>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include "timer_run_loop.hpp"

namespace ThreadPoolTimer
{

TimerRunLoop::TimerRunLoop() :
    _thread(NULL)
{
    _start();
}

TimerRunLoop::~TimerRunLoop()
{
    _stop();
    delete _thread;
}

TimerRunLoop& TimerRunLoop::getSingleton()
{
    static TimerRunLoop *singleton = new TimerRunLoop();
    return *singleton;
}

bool TimerRunLoop::getRunning()
{
    std::lock_guard<std::mutex> lock( _timersMutex );
    return _running;
}

void TimerRunLoop::start()
{
    _start();
}

void TimerRunLoop::stop()
{
    _stop();
}
        
void TimerRunLoop::registerTimer( TimerImpl &timer )
{
    // Lock the mutex
    std::lock_guard<std::mutex> lock( _timersMutex );

    // Add to timers list
    _timers.push_back(&timer);

    // Lock-in the current time and set next trigger time
    _updateNowTime();
    _resetTimer(timer);
   
    // Re-sort all the timers
    _sortTimers();

    // Notify the run loop thread that something's changed
    _timersCondition.notify_one();
}

void TimerRunLoop::unRegisterTimer( TimerImpl &timer )
{
    // Lock the mutex
    std::lock_guard<std::mutex> lock( _timersMutex );

    // Find and delete the timer from the list
    int i = 0;
    for( auto &tp : _timers )
    {
        if( tp == &timer ) {
            _timers.erase(_timers.begin() + i);
            break;
        }
        i++;
    }
    
    // Notify the run loop thread that something's changed
    _timersCondition.notify_one();
}

bool timerCompare( TimerImpl *timerA, TimerImpl *timerB )
{
    return (*timerA).getNextTrigger() < (*timerB).getNextTrigger();
}

void TimerRunLoop::_sortTimers()
{
    std::sort( _timers.begin(), _timers.end(), timerCompare );
}

void TimerRunLoop::_updateNowTime()
{
    _nowTime = std::chrono::steady_clock::now();
}

void TimerRunLoop::_resetTimer( TimerImpl &timer )
{
    // Update the timer's next trigger time using its interval
    timer.setNextTrigger( _nowTime + timer.getInterval() );
} 

void TimerRunLoop::_triggerTimers()
{
    _updateNowTime();

    int triggered = 0;
    for( auto &tp : _timers )
    {
        TimerImpl &timer = *tp;

        if( timer.getNextTrigger() <= _nowTime )
        {
            _resetTimer(timer);
            triggered++;

            // TODO: create execution threads with a queue rather than giving each 
            // event its own thread
            std::thread thread([&](){
                timer.trigger();
            });
            thread.detach();
        }
        else
        {
            break;
        }
    }
    if( triggered )
    {
        _sortTimers();
    }
}

void TimerRunLoop::_start()
{
    std::lock_guard<std::mutex> lock( _timersMutex );
    if( !_running )
    {
        _running = true;
        _thread = new std::thread([&](){
            _loopTask();
        });
    }
}

void TimerRunLoop::_stop()
{
    bool stopping = false;
    
    std::unique_lock<std::mutex> lock( _timersMutex );
    std::unique_lock<std::mutex> slock( _stopMutex );

    if( _running )
    {
        stopping = true;
        _running = false;
        _timersCondition.notify_one();
        lock.unlock();

        _stopCondition.wait( slock );
        slock.unlock();
    }
    _thread->join();
}

void TimerRunLoop::_loopTask()
{
    std::unique_lock<std::mutex> lock(_timersMutex);

    while(_running)
    {
        _triggerTimers();

        std::chrono::steady_clock::time_point tillTime;

        if( _timers.size() > 0 )
        {
            TimerImpl &t = *(_timers[0]);
            tillTime = t.getNextTrigger();
        }
        else
        {
            tillTime = _nowTime + std::chrono::seconds(1);
        }

        std::cv_status result = _timersCondition.wait_until( lock, tillTime );
    }
    lock.unlock();

    std::unique_lock<std::mutex> slock( _stopMutex );
    _stopCondition.notify_one();
    slock.unlock();
}

}

#endif
