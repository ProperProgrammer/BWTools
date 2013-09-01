//
//  AsyncXCTestCase.h
//  SPDYCocoa
//
//  Created by James Thompson on 5/25/13.
//  Copyright (c) 2013 Voxer. All rights reserved.
//

#import <XCTest/XCTest.h>

typedef void (^AsyncBlock)(BOOL *done);
typedef void (^HandleStreamEventBlock)(NSStream *aStream, NSStreamEvent eventCode);

@interface AsyncXCTestCase : XCTestCase <NSStreamDelegate>

@property (nonatomic, copy) NSString *currentTestMethodName;

/**
 * Returns the thread on which the async code will be tested.
 *
 * @return The event listener thread.
 */ 
+ (NSThread *)eventListenerThread;

/**
 * Sets stream's delegate to self, schedules it on eventListenerThread and opens it.
 *
 * @param stream An NSStream instance that should be handled on eventListenerThread.
 * 
 * @discussion You must keep a reference to stream if you intend to close it and remove it from
 * the run loop. See teardownStream:
 *
 * @warning Do not attempt to schedule stream in an NSRunLoop or open it prior to using this method;
 * this method already does that for you.
 */
- (void)setupStream:(NSStream *)stream;

/**
 * Removes the specified stream from eventListenerThread's run loop, closes it and sets its delegate
 * to nil.
 *
 * @param stream The stream to be torn down.
 *
 * @discussion setupStream: must have been called with the same NSStream instance in order for this
 * method to have any effect. 
 */
- (void)teardownStream:(NSStream *)stream;

/**
 * Creates and binds an NSInputStream to an NSOutputStream. This is useful in cases where you are
 * testing a class that exposes an NSOutputStream and you wish to test the data it is being written
 * to it.
 *
 * @param inputStream A pointer to an NSInputStream variable.
 * @param outputStream A pointer to an NSOutputStream variable.
 * @param transferBufferSize The size of the buffer used to transfer data between streams.
 */
- (void)createAndBindInputStream:(NSInputStream **)inputStream
                  toOutputStream:(NSOutputStream **)outputStream
              transferBufferSize:(NSUInteger)transferBufferSize;

/**
 * A method for testing async code that's passed an inline block callback.
 *
 * @param block An AsyncBlock that wraps the async code to be tested.
 * @param timeoutSeconds The number of seconds to wait for the method being tested to call back.
 * If timeoutSeconds elapses, the test fails.
 *
 * @discussion You __MUST__ set the blocks done paramater to YES at the end of the callback block being
 * tested. If done isn't set to YES, the block will eventually time out, failing the test. The method's
 * callback block can be called on any thread, including the main thread. The main run loop is polled
 * fequently for timeoutSeconds in order to prevent the test from ending prematurely while checking
 * the done flag.
 */
- (void)testAsyncWithBlock:(AsyncBlock)block timeout:(double)timeoutSeconds;

/**
 * A simple, inline way of handling one or more stream events in a block.
 * 
 * @param eventCode An NSStreamEvent enum. You may bitwise OR multiple fields together.
 * @param block The block that has a method signature that resembles NSStreamDelegate's one and only
 * delegate method: stream:handleEvent:
 *
 * @discussion Use of this method assumes that you've at least setup the stream to be tested with
 * setupStream: Also see createAndBindInputStream:toOutputStream:transferBufferSize: if you need
 * to read data from an output stream.
 *
 * @warning If you mask multiple eventCodes together, be sure to implement a switch or series of if /
 * else if clauses to handle said events. (Just as you would with NSStreamDelegate.)
 */
- (void)handleStreamEvent:(NSStreamEvent)eventCode
                withBlock:(HandleStreamEventBlock)block;

@end
