//
//  ResultsViewController.mm
//  Reversi
//
//  Created by sasa on 2024/05/18.
//

#import "ResultsViewController.h"

@implementation ResultsViewController

-(void)viewDidLoad {
    [super viewDidLoad];
    switch (winner()) {
        case 1:
            _win_player.stringValue = @"黒の勝ち！";
            break;
        case 2:
            _win_player.stringValue = @"白の勝ち！";
            break;
        default:
            _win_player.stringValue = @"引き分け";
            break;
    }
    if(nowTurn == BLACK_TURN) {
        self.results.stringValue = [NSString stringWithFormat:@"黒: %d 白: %d", std::popcount(playerboard), std::popcount(oppenentboard)];
    } else {
        self.results.stringValue = [NSString stringWithFormat:@"黒: %d 白: %d", std::popcount(oppenentboard), std::popcount(playerboard)];
    }
}

- (void)close:(id)sender __attribute__((ibaction)) {
    [self dismissViewController:self];
}

@end
