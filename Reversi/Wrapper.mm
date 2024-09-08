//
//  Wrapper.mm
//  Reversi
//
//  Created by sasa on 2024/05/18.
//
#import "Wrapper.h"
#import "ViewController.h"

void update_think_percent() {
    dispatch_async(dispatch_get_main_queue(), ^{

        UIViewController *nowviewcontroler = [UIApplication sharedApplication].keyWindow.rootViewController.presentedViewController;
        
        ViewController *viewController = (ViewController *)nowviewcontroler;
        viewController.hakotext.text = [NSString stringWithFormat:@"考え中...(%d%%)\n(時間がかかることがあります)", think_percent];
    });
}
