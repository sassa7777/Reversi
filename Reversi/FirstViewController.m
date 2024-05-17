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
    botplayer = WHITE_TURN;
    firstDEPTH = 3;
}


- (void)select_player:(NSSegmentedControl *)sender __attribute__((ibaction)) {
}

- (void)select_strength:(NSSegmentedControl *)sender __attribute__((ibaction)) {
}

- (void)tapstart:(id)sender __attribute__((ibaction)) {
}

@end
