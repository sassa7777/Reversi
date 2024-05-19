//
//  Wrapper.m
//  Reversi
//
//  Created by sasa on 2024/05/18.
//
#import "Wrapper.h"
#import "ViewController.h"
#import <objc/message.h>

void update_think_percent() {
    dispatch_async(dispatch_get_main_queue(), ^(void) {
        dispatch_async(dispatch_get_main_queue(), ^{
          NSWindow *keyWindow = [[NSApplication sharedApplication] keyWindow];
          if (keyWindow && [[keyWindow contentViewController] isKindOfClass:[ViewController class]]) {
            ViewController *viewController = (ViewController *)[keyWindow contentViewController];
              viewController.hakotext.stringValue = [NSString stringWithFormat:@"考え中...(%d%%)\n(時間がかかることがあります)", think_percent];
          }
        });
    });
}
