//
//  AsyncSenTestCase.m
//  SPDYCocoa
//
//  Created by James Thompson on 5/25/13.
//  Copyright (c) 2013 Voxer. All rights reserved.
//

#import "AsyncXCTestCase.h"

@interface AsyncXCTestCase ()

@property (nonatomic, strong) NSMutableDictionary *streamEventDict;

@end

@implementation AsyncXCTestCase

+ (NSThread *)eventListenerThread
{
    static dispatch_once_t onceToken;
    static NSThread *eventListenerThread = nil;
    
    dispatch_once(&onceToken, ^{
        
        eventListenerThread = [[NSThread alloc] initWithTarget:self selector:@selector(eventListenerMain) object:nil];
        eventListenerThread.name = @"com.async-xctest.event-listener-thread";
        [eventListenerThread start];
    });
    
    return eventListenerThread;
}

+ (void)eventListenerMain
{
    do
    {
        @autoreleasepool
        {
            [[NSRunLoop currentRunLoop] run];
        }
    } while (YES);
}

- (id)initWithSelector:(SEL)aSelector
{
    if (self = [super initWithSelector:aSelector])
    {
        self.currentTestMethodName = NSStringFromSelector(aSelector);
    }
    
    return self;
}

- (void)setUp
{
    [super setUp];
    
    _streamEventDict = [NSMutableDictionary new];
}

- (void)tearDown
{
    [super tearDown];
    
    _streamEventDict = nil;
}

- (void)setupStream:(NSStream *)stream
{
    [self performSelector:@selector(addStream:)
                 onThread:[[self class] eventListenerThread]
               withObject:stream
            waitUntilDone:YES];
}

- (void)addStream:(NSStream *)stream
{
    stream.delegate = self;
    [stream scheduleInRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    [stream open];
}

- (void)teardownStream:(NSStream *)stream
{
    [self performSelector:@selector(removeStream:)
                 onThread:[[self class] eventListenerThread]
               withObject:stream
            waitUntilDone:YES];
}

- (void)removeStream:(NSStream *)stream
{
    [stream close];
    [stream removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
    stream.delegate = nil;
}

- (void)createAndBindInputStream:(NSInputStream **)inputStream
                  toOutputStream:(NSOutputStream **)outputStream
              transferBufferSize:(NSUInteger)transferBufferSize
{
    CFReadStreamRef readStream;
    CFWriteStreamRef writeStream;
    
    CFStreamCreateBoundPair(kCFAllocatorDefault,
                            &readStream,
                            &writeStream,
                            transferBufferSize);
    
    *inputStream = (__bridge NSInputStream *)readStream;
    *outputStream = (__bridge NSOutputStream *)writeStream;
}

- (void)pollMainThreadWithInterval:(NSTimeInterval)interval
                           timeout:(NSTimeInterval)timeoutSec
                         stopToken:(BOOL *)done
{    
    NSDate *timeoutDate = [NSDate dateWithTimeInterval:timeoutSec sinceDate:[NSDate date]];
    
    while ([timeoutDate timeIntervalSinceNow] > 0 && *done == NO)
    {
        NSDate *intervalDate = [NSDate dateWithTimeInterval:interval
                                                  sinceDate:[NSDate date]];
        
        // Set up the initial timeout interval.
        [[NSRunLoop currentRunLoop] runUntilDate:intervalDate];
    }
    
    // If control reaches this point and done is still NO, the test has timed out and failed.
    if (*done == NO)
    {
        XCTFail(@"-testAsyncWithBlock:timeout: timed out in %@ before flagging done.",
               self.currentTestMethodName);
    }
}

- (void)testAsyncWithBlock:(AsyncBlock)block timeout:(NSTimeInterval)timeoutSeconds
{
    if (!block) return;
    
    XCTAssertNotNil(block, @"AsyncBlock must not be nil!");
    
    __block BOOL done = NO;
    
    block(&done);
    
    [self pollMainThreadWithInterval:0.25f
                             timeout:timeoutSeconds
                           stopToken:&done];
}

- (void)handleStreamEvent:(NSStreamEvent)eventCode
                withBlock:(HandleStreamEventBlock)block
{
    XCTAssertNotNil(block, @"block must not be nil in %s", __PRETTY_FUNCTION__);
    
    [self.streamEventDict setObject:block forKey:[NSNumber numberWithUnsignedInteger:eventCode]];
}

- (void)stream:(NSStream *)aStream handleEvent:(NSStreamEvent)eventCode
{
    switch (eventCode)
    {
        case NSStreamEventNone:
        {
            [self.streamEventDict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
                
                NSStreamEvent handledEvent = [(NSNumber *)key unsignedIntegerValue];
                HandleStreamEventBlock eventBlock = obj;
                
                if (handledEvent & NSStreamEventNone && eventBlock)
                {
                    eventBlock(aStream, eventCode);
                }
            }];
        }
            break;
            
        case NSStreamEventOpenCompleted:
        {
            [self.streamEventDict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
                
                NSStreamEvent handledEvent = [(NSNumber *)key unsignedIntegerValue];
                HandleStreamEventBlock eventBlock = obj;
                
                if (handledEvent & NSStreamEventOpenCompleted && eventBlock)
                {
                    eventBlock(aStream, eventCode);
                }
            }];
        }
            break;
            
        case NSStreamEventHasBytesAvailable:
        {
            [self.streamEventDict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
                
                NSStreamEvent handledEvent = [(NSNumber *)key unsignedIntegerValue];
                HandleStreamEventBlock eventBlock = obj;
                
                if (handledEvent & NSStreamEventHasBytesAvailable && eventBlock)
                {
                    eventBlock(aStream, eventCode);
                }
            }];
        }
            break;
            
        case NSStreamEventHasSpaceAvailable:
        {
            [self.streamEventDict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
                
                NSStreamEvent handledEvent = [(NSNumber *)key unsignedIntegerValue];
                HandleStreamEventBlock eventBlock = obj;
                
                if (handledEvent & NSStreamEventHasSpaceAvailable && eventBlock)
                {
                    eventBlock(aStream, eventCode);
                }
            }];
        }
            break;
            
        case NSStreamEventErrorOccurred:
        {
            [self.streamEventDict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
                
                NSStreamEvent handledEvent = [(NSNumber *)key unsignedIntegerValue];
                HandleStreamEventBlock eventBlock = obj;
                
                if (handledEvent & NSStreamEventErrorOccurred && eventBlock)
                {
                    eventBlock(aStream, eventCode);
                }
            }];
        }
            break;
            
        case NSStreamEventEndEncountered:
        {
            [self.streamEventDict enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
                
                NSStreamEvent handledEvent = [(NSNumber *)key unsignedIntegerValue];
                HandleStreamEventBlock eventBlock = obj;
                
                if (handledEvent & NSStreamEventErrorOccurred && eventBlock)
                {
                    eventBlock(aStream, eventCode);
                }
            }];
        }
            break;
    }
}

@end
