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
    NSLog(@"start");
    botplayer = WHITE_TURN;
    firstDEPTH = 11;
    Level = 5;
}

- (void)select_player:(UISegmentedControl *)sender __attribute__((ibaction)) {
    switch (sender.selectedSegmentIndex) {
        case 0:
            botplayer = WHITE_TURN;
            NSLog(@"bot: WHITE");
            break;
        case 1:
            botplayer = BLACK_TURN;
            NSLog(@"bot: BLACK");
            break;
        default:
            botplayer = WHITE_TURN;
            break;
    }
}

- (void)select_strength:(UISegmentedControl *)sender __attribute__((ibaction)) {
    switch (sender.selectedSegmentIndex) {
        case 0:
            NSLog(@"L1");
            Level = 1;
            firstDEPTH = 1;
            break;
        case 1:
            NSLog(@"L2");
            Level = 2;
            firstDEPTH = 2;
            break;
        case 2:
            NSLog(@"L3");
            Level = 3;
            firstDEPTH = 5;
            break;
        case 3:
            NSLog(@"L4");
            Level = 4;
            firstDEPTH = 8;
            break;
        case 4:
            NSLog(@"L5");
            Level = 5;
            firstDEPTH = 11;
            break;
        case 5:
            NSLog(@"L6");
            Level = 6;
            firstDEPTH = 12;
            break;
        default:
            firstDEPTH = 10;
            break;
    }
}

- (void)tapstart:(id)sender __attribute__((ibaction)) {
    [self performSegueWithIdentifier:@"start" sender:nil];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    if ([segue.identifier isEqualToString:@"start"]) {
        UIViewController *vc = (FirstViewController *)segue.destinationViewController;
        // modalPresentationStyleをfullScreenに設定します
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
}

@end
