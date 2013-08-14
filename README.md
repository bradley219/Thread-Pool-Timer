Thread-Pool-Timer
=================

Introduction
------------

A C++11 library which allows lambda functions (anonymous/closure functions) to be registered to periodic, asynchronous timer events with independent intervals. Timers can be registered and de-registered with a managed thread pool at runtime. Timer events are dispatched at their respective intervals/rates by a timer thread pool manager running in its own private thread. Each timer event is dispatched in its own new thread, or optionally, queued and dequeued by a separate worker in a dedicated thread.

Features
--------
* Requires only one private thread to manage timing events.
* Timers can be dynamically registered and de-registered with the thread pool at runtime.
* Timing intervals can be specified with nanosecond granularity.
* In practice, trigger events are usually accurate to within 10 microseconds under even the heaviest of loads (high registered timer volume with heavy re-scheduling load).
* Timer events can be attached directly to a C++11 lambda function, executing in the scope of the declaration.
* Simple to incorporate into existing projects. All of the framework needed to maintain the thread pool is bootstrapped after the first timer is registered.

Requirements
------------

As the Thread-Pool-Timer library makes use of several features specific to the C++11 standard (std::thread, std::chrono, std::function and others), source files using the library must be compiled with the C++11 standard. In most cases, this is as simple as adding `-std=c++11` to your gcc compile command, but may require additional modifications.

Installation
------------

To install the library run the following commands from the repository's root directory:

```
$ ./configure --enable-shared
$ make
$ sudo make install
$ sudo ldconfig
```

Usage
-----

Linker flags required when using the library: `-lthreadpooltimer -lpthread`

An example use of the library is shown below. This file can be compiled with these gcc options: `g++ -std=c++11 -o test test.cpp -lthreadpooltimer -lpthread`

```cpp
/* test.cpp */

#include <iostream>
#include <thread>
#include <tp_timer.hpp>

using namespace std;
using namespace ThreadPoolTimer;

int main()
{
    string s = "my scoped variable";

    /* Create a new Timer object with a lambda function to be called when
     * the timer triggers. */
    Timer t([&]{
        cout << s << endl;
    });

    /* Set the timer's interval */
    t.setInterval( chrono::nanoseconds(1000000000) );

    /* Start the timer */
    t.setEnabled(true);

    /* Wait for some time to allow the timer to trigger */
    this_thread::sleep_for( chrono::seconds(10) );
    
    /* Stop and reset the timer */
    t.setEnabled(false);

    return 0;
}
```

License
-------

Licensed to Bradley J. Snyder under one or more contributor license agreements.  See the NOTICE file distributed with this work for additional information regarding copyright ownership.  The ASF licenses this file to you under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the License for the specific language governing permissions and limitations under the License.
