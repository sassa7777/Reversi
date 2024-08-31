//
//  FirstViewController.mm
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
    firstDEPTH = 11;
    Level = 5;
    
}

- (void)select_player:(NSSegmentedControl *)sender __attribute__((ibaction)) {
    switch (sender.selectedSegment) {
        case 0:
            botplayer = BLACK_TURN;
            NSLog(@"bot: BLACK");
            break;
        case 1:
            botplayer = WHITE_TURN;
            NSLog(@"bot: WHITE");
            break;
        default:
            botplayer = WHITE_TURN;
            break;
    }
}

- (void)select_strength:(NSSegmentedControl *)sender __attribute__((ibaction)) {
    switch (sender.selectedSegment) {
        case 0:
            NSLog(@"L1");
            Level = 1;
            firstDEPTH = 1;
            _strength_txt.stringValue = @"初心者向け";
            break;
        case 1:
            NSLog(@"L2");
            Level = 2;
            firstDEPTH = 2;
            _strength_txt.stringValue = @"初心者よりはできる人向け";
            break;
        case 2:
            NSLog(@"L3");
            Level = 3;
            firstDEPTH = 5;
            _strength_txt.stringValue = @"ある程度できる人向け";
            break;
        case 3:
            NSLog(@"L4");
            Level = 4;
            firstDEPTH = 8;
            _strength_txt.stringValue = @"少し自信がある人向け";
            break;
        case 4:
            NSLog(@"L5");
            Level = 5;
            firstDEPTH = 11;
            _strength_txt.stringValue = @"それなりに自信がある人向け";
            break;
        case 5:
            NSLog(@"L6");
            Level = 6;
            firstDEPTH = 12;
            _strength_txt.stringValue = @"自信がある人向け";
            break;
        default:
            firstDEPTH = 10;
            break;
    }
}

- (void)tapstart:(id)sender __attribute__((ibaction)) {
    [self performSegueWithIdentifier:@"start" sender:nil];
}

@end
