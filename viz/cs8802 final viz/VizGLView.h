//
//  VizGLView.h
//  cs8802 final viz
//
//  Created by Jacob Fugal on 5/7/14.
//  Copyright (c) 2014 Jacob Fugal. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "VizRenderer.h"

@interface VizGLView : NSOpenGLView
{
    CVDisplayLinkRef mDisplayLink;
    bool mViewReady;
    VizRenderer *mRenderer;
}
- (void) awakeFromNib;
- (int) getIteration;
- (void) drawRect: (NSRect) bounds;
@end