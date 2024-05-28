//
//  ResultsViewController.mm
//  Reversi
//
//  Created by sasa on 2024/05/18.
//

#import "ResultsViewController.h"

using namespace std;

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
        self.results.stringValue = [NSString stringWithFormat:@"黒: %d 白: %d", popcount(b.playerboard), popcount(b.oppenentboard)];
    } else {
        self.results.stringValue = [NSString stringWithFormat:@"黒: %d 白: %d", popcount(b.oppenentboard), popcount(b.playerboard)];
    }
}

- (void)close:(id)sender __attribute__((ibaction)) {
    [self dismissViewController:self];
}

@end
