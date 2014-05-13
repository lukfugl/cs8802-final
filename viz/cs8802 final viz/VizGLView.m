//
//  VizGLView.m
//  cs8802 final viz
//
//  Created by Jacob Fugal on 5/7/14.
//  Copyright (c) 2014 Jacob Fugal. All rights reserved.
//

#import "VizGLView.h"
#import "VizAppDelegate.h"
#include <OpenGL/gl.h>

@implementation VizGLView

-(void) awakeFromNib
{
    mViewReady = true;
    mRenderer = [VizRenderer alloc];
}

-(int) getIteration
{
    return [(VizAppDelegate*) [[NSApplication sharedApplication] delegate] iteration];
}

-(void) drawRect: (NSRect) bounds
{
    [self drawFRD];
}

-(void) drawFRD
{
    if (mViewReady) [mRenderer render: [self getIteration]];
}

- (void)renderFrame
{
    NSOpenGLContext    *currentContext = [self openGLContext];
    [currentContext makeCurrentContext];
    
    // must lock GL context because display link is threaded
    CGLLockContext((CGLContextObj)[currentContext CGLContextObj]);
    
    [self drawFRD];
    
    [currentContext flushBuffer];
    
    CGLUnlockContext((CGLContextObj)[currentContext CGLContextObj]);
}

- (void)prepareOpenGL
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Synchronize buffer swaps with vertical refresh rate
    GLint swapInt = 1;
    [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];
    
    // Create a display link capable of being used with all active displays
    CVDisplayLinkCreateWithActiveCGDisplays(&mDisplayLink);
    
    // Set the renderer output callback function
    CVDisplayLinkSetOutputCallback(mDisplayLink, &MyDisplayLinkCallback, (__bridge void*)self);
    
    // Set the display link for the current renderer
    CGLContextObj cglContext = [[self openGLContext] CGLContextObj];
    CGLPixelFormatObj cglPixelFormat = [[self pixelFormat] CGLPixelFormatObj];
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(mDisplayLink, cglContext, cglPixelFormat);
    
    // Activate the display link
    CVDisplayLinkStart(mDisplayLink);
}

// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge VizGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    if ([self lockFocusIfCanDraw] == NO) return kCVReturnError;
    [self renderFrame];
    [self unlockFocus];
    return kCVReturnSuccess;
}

- (void)dealloc
{
    // Release the display link
    CVDisplayLinkRelease(mDisplayLink);
}

@end
