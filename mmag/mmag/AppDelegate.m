//
//  AppDelegate.m
//  mmag
//
//  Created by Lukas Murmann on 11/28/14.
//  Copyright (c) 2014 mit. All rights reserved.
//

#import "AppDelegate.h"
#import "LMVideoView.h"

@interface AppDelegate ()
@property (weak) IBOutlet NSSlider *lo_slider;
@property (weak) IBOutlet NSSlider *hi_slider;
@property (weak) IBOutlet NSSlider *lambda_slider;
@property (weak) IBOutlet NSTextField *lo_label;
@property (weak) IBOutlet NSTextField *hi_label;
@property (weak) IBOutlet NSTextField *lambda_label;
@property (weak) IBOutlet NSSlider *alpha_slider;
@property (weak) IBOutlet NSTextField *alpha_label;
@property (weak) IBOutlet LMVideoView *video_view;
@property (weak) IBOutlet NSWindow *window;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application

    [self.lambda_slider setIntValue:1];
    [self.alpha_slider setFloatValue:10];
    [self updateUI];
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    [self.video_view cleanup];
}

- (void) updateUI {
///
    float alpha = self.alpha_slider.floatValue;
    [self.alpha_label setStringValue:[NSString stringWithFormat:@"alpha = %.1f", alpha]];
    [self.video_view setAmplificationAlpha:alpha];

    ///
    int level = self.lambda_slider.intValue;
    int lambda = pow(2, level);
    [self.lambda_label setStringValue:[NSString stringWithFormat:@"lambda_cutoff = %d", lambda]];
    [self.video_view setSpatialCutoff:level];

    ///
    float lo = self.lo_slider.floatValue;
    float hi = self.hi_slider.floatValue;

    lo = fmin(lo, hi - 0.1);
    self.lo_slider.floatValue = lo;

    self.lo_label.stringValue = [NSString stringWithFormat:@"lo = %.1f", lo];
    self.hi_label.stringValue = [NSString stringWithFormat:@"hi = %.1f", hi];

    [self.video_view setBandpassLow: lo andHigh:hi];

}

- (IBAction)slider_changed:(id)sender {
    [self updateUI];
}


@end
