//
//  LMVideoView.m
//  mmag
//
//  Created by Lukas Murmann on 11/28/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#import "LMVideoView.h"
#import <OpenGL/gl.h>
#import "videorenderer.h"
#import <string>

@implementation LMVideoView
{
    lum::videorenderer* vrenderer;
}

- (id) initWithCoder:(NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self != nil) {

        [self setWantsBestResolutionOpenGLSurface:YES];
        NSOpenGLPixelFormatAttribute attribs[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 16,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFASupersample,
            0
        };

        NSOpenGLPixelFormat* fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
        self.pixelFormat = fmt;
    }
    return self;
}

- (void) startDrawLoop {
    [self setNeedsDisplay:TRUE];
    if (vrenderer == NULL) {
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        [self startDrawLoop];
    });
}

-(void) setAmplificationAlpha: (float) alpha {
    vrenderer->set_alpha(alpha);
}
-(void) setSpatialCutoff: (int) level {
    vrenderer->set_spatial_cutoff(level);
}

-(void) setBandpassLow: (float)lo andHigh: (float)hi {
    vrenderer->set_bandpass(lo, hi);
}

- (void)prepareOpenGL {
    NSBundle* bundle = [NSBundle mainBundle];
    std::string basepath = [[bundle resourcePath] cStringUsingEncoding:NSUTF8StringEncoding];
    vrenderer = new lum::videorenderer{basepath};
    [self startDrawLoop];
}
- (void)cleanup
{
    delete vrenderer;
    vrenderer = NULL;
}

- (void)drawRect:(NSRect)dirtyRect {
    [self.openGLContext makeCurrentContext];
    if (vrenderer == NULL) {
        return;
    }

    double now = [NSDate timeIntervalSinceReferenceDate];
    vrenderer->draw(now);
    [[self openGLContext] flushBuffer];
}


-(void) play {
    double now = [NSDate timeIntervalSinceReferenceDate];
    vrenderer->play(now);
}

@end
