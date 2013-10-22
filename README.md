BWTools
=======

Useful tools for Cocoa development.

## AsyncXCTestCase

AsyncXCTestCase is a subclass of XCTestCase that is used for testing asynchronous code that exposes 
some kind of callback mechanism, be it a delegate or a block. AsyncXCTestCase also has support for 
testing APIs that expose an NSStream by using I/O stream bound pairs, NSStreamDelegate and an NSThread
with an NSRunLoop for scheduling.

Here is an example AsyncXCTestCase subclass that tests some async code:
    
    #import "AsyncXCTestCase.h"

    @interface ChatListViewControllerTests : AsyncXCTestCase

    @end

    @implementation ChatListViewControllerTests

    - (void)testAsync
    {
        [self testAsyncWithBlock:^(BOOL *done) {
            
            // Put your async code here... For example let's use dispatch_async:
            dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                
                // The test would normally succeed/fail before this is even executed...
                XCTAssert(YES, @"Test passed!");
                
                // You MUST flag that this block is done otherwise it will timeout after 5.0 seconds (specified by 
                // timeout argument below.) This tells AsyncXCTestCase to stop polling the main NSRunLoop.
                *done = YES;
            });
        } timeout:5.0]; // Note: Timeout is specified in seconds. If timeout elapses, the test fails.
    }
    

    @end
