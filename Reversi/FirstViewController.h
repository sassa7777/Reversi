//
//  FirstViewController.h
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import <UIKit/UIKit.h>
#import "reversi.h"


@interface FirstViewController : UIViewController

@property (nonatomic, weak) IBOutlet UISegmentedControl *strength_select;
@property (nonatomic, weak) IBOutlet UISegmentedControl *selectplayer;

-(IBAction)select_strength:(UISegmentedControl*)sender;
-(IBAction)select_player:(UISegmentedControl*)sender;
-(IBAction)tapstart:(id)sender;

@end

