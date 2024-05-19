//
//  ResultsViewController。h.h
//  Reversi
//
//  Created by sasa on 2024/05/18.
//

#import <Cocoa/Cocoa.h>
#import "reversi.hpp"

@interface ResultsViewController : NSViewController

@property (nonatomic, weak) IBOutlet NSTextField *results;
@property (nonatomic, weak) IBOutlet NSTextField *win_player;

-(IBAction)close:(id)sender;

@end
