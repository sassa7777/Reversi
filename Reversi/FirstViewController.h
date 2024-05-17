//
//  FirstViewController.h
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import <Cocoa/Cocoa.h>
#import "reversi.h"


@interface FirstViewController : NSViewController

@property (nonatomic, weak) IBOutlet NSSegmentedControl *strength_select;
@property (nonatomic, weak) IBOutlet NSTextField *strength_txt;
@property (nonatomic, weak) IBOutlet NSSegmentedControl *selectplayer;

-(IBAction)select_strength:(NSSegmentedControl*)sender;
-(IBAction)select_player:(NSSegmentedControl*)sender;
-(IBAction)tapstart:(id)sender;

@end

