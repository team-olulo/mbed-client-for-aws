/*
 * Copyright (c) 2020 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "mbed_trace.h"

#define TRACE_GROUP "AWSPort_Thread"

extern "C" {
#include "iot_threads.h"
}

bool Iot_CreateDetachedThread( IotThreadRoutine_t threadRoutine,
                               void * pArgument,
                               int32_t priority,
                               size_t stackSize ) {
    auto thread = new Thread {
        osPriorityLow,
        (stackSize==0)? MBED_CONF_RTOS_THREAD_STACK_SIZE : stackSize,
        nullptr,
        "detachable thread"
    };
    thread->start([=] {
        tr_info("entering detachable thread");
        threadRoutine(pArgument);
        tr_info("exiting detatchable thread %p", thread);
        EventQueue *queue = mbed_event_queue();
        queue->call_in(100ms, callback([=] { delete thread; }));
        // delete thread;
    });

    return true;
}
