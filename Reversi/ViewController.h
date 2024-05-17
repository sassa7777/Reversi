//
//  ViewController.h
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import <Cocoa/Cocoa.h>
#import "reversi.h"

@interface ViewController : NSViewController

@property (nonatomic, weak) IBOutlet NSButton *aa;
@property (nonatomic, weak) IBOutlet NSButton *ab;
@property (nonatomic, weak) IBOutlet NSButton *ac;
@property (nonatomic, weak) IBOutlet NSButton *ad;
@property (nonatomic, weak) IBOutlet NSButton *ae;
@property (nonatomic, weak) IBOutlet NSButton *af;
@property (nonatomic, weak) IBOutlet NSButton *ag;
@property (nonatomic, weak) IBOutlet NSButton *ah;
@property (nonatomic, weak) IBOutlet NSButton *ba;
@property (nonatomic, weak) IBOutlet NSButton *bb;
@property (nonatomic, weak) IBOutlet NSButton *bc;
@property (nonatomic, weak) IBOutlet NSButton *bd;
@property (nonatomic, weak) IBOutlet NSButton *be;
@property (nonatomic, weak) IBOutlet NSButton *bf;
@property (nonatomic, weak) IBOutlet NSButton *bg;
@property (nonatomic, weak) IBOutlet NSButton *bh;
@property (nonatomic, weak) IBOutlet NSButton *ca;
@property (nonatomic, weak) IBOutlet NSButton *cb;
@property (nonatomic, weak) IBOutlet NSButton *cc;
@property (nonatomic, weak) IBOutlet NSButton *cd;
@property (nonatomic, weak) IBOutlet NSButton *ce;
@property (nonatomic, weak) IBOutlet NSButton *cf;
@property (nonatomic, weak) IBOutlet NSButton *cg;
@property (nonatomic, weak) IBOutlet NSButton *ch;
@property (nonatomic, weak) IBOutlet NSButton *da;
@property (nonatomic, weak) IBOutlet NSButton *db;
@property (nonatomic, weak) IBOutlet NSButton *dc;
@property (nonatomic, weak) IBOutlet NSButton *dd;
@property (nonatomic, weak) IBOutlet NSButton *de;
@property (nonatomic, weak) IBOutlet NSButton *df;
@property (nonatomic, weak) IBOutlet NSButton *dg;
@property (nonatomic, weak) IBOutlet NSButton *dh;
@property (nonatomic, weak) IBOutlet NSButton *ea;
@property (nonatomic, weak) IBOutlet NSButton *eb;
@property (nonatomic, weak) IBOutlet NSButton *ec;
@property (nonatomic, weak) IBOutlet NSButton *ed;
@property (nonatomic, weak) IBOutlet NSButton *ee;
@property (nonatomic, weak) IBOutlet NSButton *ef;
@property (nonatomic, weak) IBOutlet NSButton *eg;
@property (nonatomic, weak) IBOutlet NSButton *eh;
@property (nonatomic, weak) IBOutlet NSButton *fa;
@property (nonatomic, weak) IBOutlet NSButton *fb;
@property (nonatomic, weak) IBOutlet NSButton *fc;
@property (nonatomic, weak) IBOutlet NSButton *fd;
@property (nonatomic, weak) IBOutlet NSButton *fe;
@property (nonatomic, weak) IBOutlet NSButton *ff;
@property (nonatomic, weak) IBOutlet NSButton *fg;
@property (nonatomic, weak) IBOutlet NSButton *fh;
@property (nonatomic, weak) IBOutlet NSButton *ga;
@property (nonatomic, weak) IBOutlet NSButton *gb;
@property (nonatomic, weak) IBOutlet NSButton *gc;
@property (nonatomic, weak) IBOutlet NSButton *gd;
@property (nonatomic, weak) IBOutlet NSButton *ge;
@property (nonatomic, weak) IBOutlet NSButton *gf;
@property (nonatomic, weak) IBOutlet NSButton *gg;
@property (nonatomic, weak) IBOutlet NSButton *gh;
@property (nonatomic, weak) IBOutlet NSButton *ha;
@property (nonatomic, weak) IBOutlet NSButton *hb;
@property (nonatomic, weak) IBOutlet NSButton *hc;
@property (nonatomic, weak) IBOutlet NSButton *hd;
@property (nonatomic, weak) IBOutlet NSButton *he;
@property (nonatomic, weak) IBOutlet NSButton *hf;
@property (nonatomic, weak) IBOutlet NSButton *hg;
@property (nonatomic, weak) IBOutlet NSButton *hh;
@property (nonatomic, weak) IBOutlet NSImageView *hakoface;
@property (nonatomic, weak) IBOutlet NSButton *close;
@property (nonatomic, weak) IBOutlet NSButton *restart;
@property (nonatomic, weak) IBOutlet NSTextField *hakotext;
@property (nonatomic, weak) IBOutlet NSTextField *white_cnt;
@property (nonatomic, weak) IBOutlet NSTextField *black_cnt;
@property (nonatomic, weak) IBOutlet NSTextField *lev_txt;

-(IBAction)put:(NSButton*)sender;
-(IBAction)close:(id)sender;
-(IBAction)restart:(id)sender;

-(void)switchbuttons:(BOOL)s;
-(void)botput;
-(void)results;
-(void)reloadview;



@end

