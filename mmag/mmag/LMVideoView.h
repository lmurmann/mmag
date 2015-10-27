//
//  LMVideoView.h
//  mmag
//
//  Created by Lukas Murmann on 11/28/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface LMVideoView : NSOpenGLView

-(void) play;
-(void) cleanup;
-(void) setAmplificationAlpha: (float) alpha;
-(void) setSpatialCutoff: (int) level;
-(void) setBandpassLow: (float)lo andHigh: (float)hi;

@end
