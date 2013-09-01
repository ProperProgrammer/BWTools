//
//  benchmark_util.h
//  CoreDataTest1
//
//  Created by James Thompson on 8/14/13.
//  Copyright (c) 2013 James. All rights reserved.
//

#include <sys/time.h>
#include <sys/resource.h>

static double _benchmarkDelta;

static inline double get_time()
{
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec * 1e-6;
}

static inline void benchmark_timer_start(NSString *str, ...)
{
    _benchmarkDelta = get_time();
    
    str = [NSString stringWithFormat:@"%@ 🚩 Benchmark Timer Began 🚩", str];
    
    va_list args;
    va_start(args, str);
    NSLogv(str, args);
    va_end(args);
}

static inline void benchmark_timer_stop(NSString *str, ...)
{
    _benchmarkDelta = get_time() - _benchmarkDelta;
    
    str = [NSString stringWithFormat:@"%@ ⏰ Completed In %f Seconds ⏰", str, _benchmarkDelta];
    
    va_list args;
    va_start(args, str);
    NSLogv(str, args);
    va_end(args);
}