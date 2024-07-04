//
//  ViewController.h
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>
#import "reversi.h"
#import <map>

@interface ViewController : NSViewController
{
    ankerl::unordered_dense::map<NSButton*, std::pair<char, char>> coordinate;
};

@property (nonatomic) IBOutlet NSButton *aa;
@property (nonatomic) IBOutlet NSButton *ab;
@property (nonatomic) IBOutlet NSButton *ac;
@property (nonatomic) IBOutlet NSButton *ad;
@property (nonatomic) IBOutlet NSButton *ae;
@property (nonatomic) IBOutlet NSButton *af;
@property (nonatomic) IBOutlet NSButton *ag;
@property (nonatomic) IBOutlet NSButton *ah;
@property (nonatomic) IBOutlet NSButton *ba;
@property (nonatomic) IBOutlet NSButton *bb;
@property (nonatomic) IBOutlet NSButton *bc;
@property (nonatomic) IBOutlet NSButton *bd;
@property (nonatomic) IBOutlet NSButton *be;
@property (nonatomic) IBOutlet NSButton *bf;
@property (nonatomic) IBOutlet NSButton *bg;
@property (nonatomic) IBOutlet NSButton *bh;
@property (nonatomic) IBOutlet NSButton *ca;
@property (nonatomic) IBOutlet NSButton *cb;
@property (nonatomic) IBOutlet NSButton *cc;
@property (nonatomic) IBOutlet NSButton *cd;
@property (nonatomic) IBOutlet NSButton *ce;
@property (nonatomic) IBOutlet NSButton *cf;
@property (nonatomic) IBOutlet NSButton *cg;
@property (nonatomic) IBOutlet NSButton *ch;
@property (nonatomic) IBOutlet NSButton *da;
@property (nonatomic) IBOutlet NSButton *db;
@property (nonatomic) IBOutlet NSButton *dc;
@property (nonatomic) IBOutlet NSButton *dd;
@property (nonatomic) IBOutlet NSButton *de;
@property (nonatomic) IBOutlet NSButton *df;
@property (nonatomic) IBOutlet NSButton *dg;
@property (nonatomic) IBOutlet NSButton *dh;
@property (nonatomic) IBOutlet NSButton *ea;
@property (nonatomic) IBOutlet NSButton *eb;
@property (nonatomic) IBOutlet NSButton *ec;
@property (nonatomic) IBOutlet NSButton *ed;
@property (nonatomic) IBOutlet NSButton *ee;
@property (nonatomic) IBOutlet NSButton *ef;
@property (nonatomic) IBOutlet NSButton *eg;
@property (nonatomic) IBOutlet NSButton *eh;
@property (nonatomic) IBOutlet NSButton *fa;
@property (nonatomic) IBOutlet NSButton *fb;
@property (nonatomic) IBOutlet NSButton *fc;
@property (nonatomic) IBOutlet NSButton *fd;
@property (nonatomic) IBOutlet NSButton *fe;
@property (nonatomic) IBOutlet NSButton *ff;
@property (nonatomic) IBOutlet NSButton *fg;
@property (nonatomic) IBOutlet NSButton *fh;
@property (nonatomic) IBOutlet NSButton *ga;
@property (nonatomic) IBOutlet NSButton *gb;
@property (nonatomic) IBOutlet NSButton *gc;
@property (nonatomic) IBOutlet NSButton *gd;
@property (nonatomic) IBOutlet NSButton *ge;
@property (nonatomic) IBOutlet NSButton *gf;
@property (nonatomic) IBOutlet NSButton *gg;
@property (nonatomic) IBOutlet NSButton *gh;
@property (nonatomic) IBOutlet NSButton *ha;
@property (nonatomic) IBOutlet NSButton *hb;
@property (nonatomic) IBOutlet NSButton *hc;
@property (nonatomic) IBOutlet NSButton *hd;
@property (nonatomic) IBOutlet NSButton *he;
@property (nonatomic) IBOutlet NSButton *hf;
@property (nonatomic) IBOutlet NSButton *hg;
@property (nonatomic) IBOutlet NSButton *hh;
@property (nonatomic, weak) IBOutlet NSImageView *hakoface;
@property (nonatomic, weak) IBOutlet NSButton *close;
@property (nonatomic, weak) IBOutlet NSButton *restart;
@property (nonatomic, strong) IBOutlet NSTextField *hakotext;
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

