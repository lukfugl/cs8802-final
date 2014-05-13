//
//  VizAppDelegate.h
//  cs8802 final viz
//
//  Created by Jacob Fugal on 5/7/14.
//  Copyright (c) 2014 Jacob Fugal. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VizAppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;
@property (readwrite, assign) int iteration;

@end
