//
//  FirstViewController.m
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import "FirstViewController.h"

@implementation FirstViewController 

-(void)viewDidLoad {
    [super viewDidLoad];
    _strength_txt.stringValue = @"初心者よりはできる人向け向け";
    NSLog(@"start");
    botplayer = WHITE_TURN;
    firstDEPTH = 3;
}


- (void)select_player:(NSSegmentedControl *)sender __attribute__((ibaction)) {
}

- (void)select_strength:(NSSegmentedControl *)sender __attribute__((ibaction)) {
    switch (sender.selectedSegment) {
        case 0:
            firstDEPTH = 1;
            NSLog(@"L1");
            _strength_txt.stringValue = @"初心者向け";
            break;
        case 1:
            firstDEPTH = 2;
            NSLog(@"L2");
            _strength_txt.stringValue = @"初心者よりはできる人向け";
            break;
        case 2:
            NSLog(@"L3");
            firstDEPTH = 4;
            _strength_txt.stringValue = @"ある程度できる人向け";
            break;
        case 3:
            NSLog(@"L4");
            firstDEPTH = 6;
            _strength_txt.stringValue = @"少し自信がある人向け";
            break;
        case 4:
            NSLog(@"L5");
            firstDEPTH = 10;
            _strength_txt.stringValue = @"自信がある人向け";
            break;
        default:
            firstDEPTH = 10;
            break;
    }
}

- (void)tapstart:(id)sender __attribute__((ibaction)) {
    [self performSegueWithIdentifier:@"start" sender:self];
}

@end
