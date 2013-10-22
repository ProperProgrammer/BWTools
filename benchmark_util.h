// The MIT License (MIT)
//
// Copyright (c) 2013 James Thompson
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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