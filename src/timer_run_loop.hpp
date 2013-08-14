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
#ifndef _TIMER_RUN_LOOP_HPP_
#define _TIMER_RUN_LOOP_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <unordered_set>
#include "timer_impl.hpp"

namespace ThreadPoolTimer
{

class TimerRunLoop
{
    public:
        TimerRunLoop();
        ~TimerRunLoop();
        static TimerRunLoop &getSingleton();

        bool getRunning();
        void stop();
        void start();

        void registerTimer( TimerImpl &timer );
        void unRegisterTimer( TimerImpl &timer );

    private:
        std::thread *_thread;

        std::vector<TimerImpl*> _timers;
        std::mutex _timersMutex;
        std::condition_variable _timersCondition;
        
        std::mutex _stopMutex;
        std::condition_variable _stopCondition;

        std::chrono::steady_clock::time_point _nowTime;
        void _sortTimers();
        void _updateNowTime();
        void _resetTimer( TimerImpl &timer );
        
        void _triggerTimers();

        void _loopTask();
        void _start();
        void _stop();
        bool _running;
};


}

#endif
