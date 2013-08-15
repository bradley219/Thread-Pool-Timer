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
#ifndef _THREADPOOLTIMERTEST_SOURCE_
#define _THREADPOOLTIMERTEST_SOURCE_

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include "tp_timer.hpp"

int main( int argc, char *argv[] )
{
    int retval = 0;

    int timercount = 10;
    int triggercount = 0;
    std::vector<ThreadPoolTimer::Timer*> timers;

    try
    {

        for( int i = 0; i < timercount; i++ )
        {
            ThreadPoolTimer::Timer *timer = new ThreadPoolTimer::Timer([i,&triggercount](){
                triggercount++;
                std::cout << std::setw(2) << "timer " << i << " count=" << std::setw(5) << triggercount << std::endl;
            });

            timer->setInterval(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(100)));
            timer->setEnabled(true);
            timers.push_back(timer);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        //while(1)
        {
            sleep(1);
        }
       
        for( auto &timer : timers )
        {
            delete timer;
        }
        
        ThreadPoolTimer::Timer::stopRunLoop();

    }
    catch( ThreadPoolTimer::ThreadPoolTimerException &e )
    {
        std::cerr << "An exception was caught: " << e.what() << std::endl;
    }

    return retval;
}

#endif
