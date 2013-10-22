//
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

typedef void (^benchmark_block_t)(void);

static inline double get_time()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec * 1e-6;
}

// This compiler flag gives some smarts to the analyzer to pick up on format errors.
static inline void benchmark_timer_start(NSString *str, ...) NS_FORMAT_FUNCTION(1, 2);

static inline void benchmark_timer_start(NSString *str, ...)
{
    _benchmarkDelta = get_time();
    
    str = [NSString stringWithFormat:@"%@ 🚩 Benchmark Timer Began 🚩", str];
    
    va_list args;
    va_start(args, str);
    NSLogv(str, args);
    va_end(args);
}

// This compiler flag gives some smarts to the analyzer to pick up on format errors.
static inline void benchmark_timer_stop(NSString *str, ...) NS_FORMAT_FUNCTION(1, 2);

static inline void benchmark_timer_stop(NSString *str, ...)
{
    _benchmarkDelta = get_time() - _benchmarkDelta;
    
    str = [NSString stringWithFormat:@"%@ ⏰ Completed In %f Seconds ⏰", str, _benchmarkDelta];
    
    va_list args;
    va_start(args, str);
    NSLogv(str, args);
    va_end(args);
}

static inline void benchmark_block(benchmark_block_t benchmarkBlock, NSString *logString, ...) NS_FORMAT_FUNCTION(2, 3);

static inline void benchmark_block(benchmark_block_t benchmarkBlock, NSString *logString, ...)
{
    if (benchmarkBlock == NULL)
        return;
    
    NSString *startLogString = [NSString stringWithFormat:@"%@ 🚩 Benchmark Timer Began 🚩", (logString) ? logString : @""];
    
    va_list args;
    
    va_start(args, logString);
    NSLogv(startLogString, args);
    va_end(args);
    
    double dt = get_time();
    
    benchmarkBlock();
    
    dt = get_time() - dt;
    
    NSString *endLogString = [NSString stringWithFormat:@"%@ ⏰ Completed In %f Seconds ⏰", logString, dt];
    
    va_start(args, logString);
    NSLogv(endLogString, args);
    va_end(args);
}

static inline void benchmark_block_avg(benchmark_block_t benchmarkBlock, int numIterations, NSString *logString, ...) NS_FORMAT_FUNCTION(3, 4);

static inline void benchmark_block_avg(benchmark_block_t benchmarkBlock, int numIterations, NSString *logString, ...)
{
    if (benchmarkBlock == NULL)
        return;
    
    NSString *startLogString = [NSString stringWithFormat:@"%@ 🚩 Benchmark Timer Began. Performing %d Iterations. 🚩", (logString) ? logString : @"", numIterations];
    
    va_list args;
    
    va_start(args, logString);
    NSLogv(startLogString, args);
    va_end(args);
    
    double totalDelta = 0;
    
    for (int i = 0; i < numIterations; i ++)
    {
        double curDelta = get_time();
        
        benchmarkBlock();
        
        totalDelta += get_time() - curDelta;
    }
    
    double dt = totalDelta / numIterations;
    
    NSString *endLogString = [NSString stringWithFormat:@"%@ ⏰ Mean Execution Time Was: %f Seconds ⏰", logString, dt];
    
    va_start(args, logString);
    NSLogv(endLogString, args);
    va_end(args);
}
