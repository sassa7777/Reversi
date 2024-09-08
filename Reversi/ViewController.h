//
//  ViewController.h
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import <UIKit/UIKit.h>
#import "reversi.h"

@interface ViewController : UIViewController
{
    ankerl::unordered_dense::map<UIButton*, std::pair<int_fast8_t, int_fast8_t>> coordinate_y_x;
};

@property (nonatomic, weak) IBOutlet UIButton *aa;
@property (nonatomic, weak) IBOutlet UIButton *ab;
@property (nonatomic, weak) IBOutlet UIButton *ac;
@property (nonatomic, weak) IBOutlet UIButton *ad;
@property (nonatomic, weak) IBOutlet UIButton *ae;
@property (nonatomic, weak) IBOutlet UIButton *af;
@property (nonatomic, weak) IBOutlet UIButton *ag;
@property (nonatomic, weak) IBOutlet UIButton *ah;
@property (nonatomic, weak) IBOutlet UIButton *ba;
@property (nonatomic, weak) IBOutlet UIButton *bb;
@property (nonatomic, weak) IBOutlet UIButton *bc;
@property (nonatomic, weak) IBOutlet UIButton *bd;
@property (nonatomic, weak) IBOutlet UIButton *be;
@property (nonatomic, weak) IBOutlet UIButton *bf;
@property (nonatomic, weak) IBOutlet UIButton *bg;
@property (nonatomic, weak) IBOutlet UIButton *bh;
@property (nonatomic, weak) IBOutlet UIButton *ca;
@property (nonatomic, weak) IBOutlet UIButton *cb;
@property (nonatomic, weak) IBOutlet UIButton *cc;
@property (nonatomic, weak) IBOutlet UIButton *cd;
@property (nonatomic, weak) IBOutlet UIButton *ce;
@property (nonatomic, weak) IBOutlet UIButton *cf;
@property (nonatomic, weak) IBOutlet UIButton *cg;
@property (nonatomic, weak) IBOutlet UIButton *ch;
@property (nonatomic, weak) IBOutlet UIButton *da;
@property (nonatomic, weak) IBOutlet UIButton *db;
@property (nonatomic, weak) IBOutlet UIButton *dc;
@property (nonatomic, weak) IBOutlet UIButton *dd;
@property (nonatomic, weak) IBOutlet UIButton *de;
@property (nonatomic, weak) IBOutlet UIButton *df;
@property (nonatomic, weak) IBOutlet UIButton *dg;
@property (nonatomic, weak) IBOutlet UIButton *dh;
@property (nonatomic, weak) IBOutlet UIButton *ea;
@property (nonatomic, weak) IBOutlet UIButton *eb;
@property (nonatomic, weak) IBOutlet UIButton *ec;
@property (nonatomic, weak) IBOutlet UIButton *ed;
@property (nonatomic, weak) IBOutlet UIButton *ee;
@property (nonatomic, weak) IBOutlet UIButton *ef;
@property (nonatomic, weak) IBOutlet UIButton *eg;
@property (nonatomic, weak) IBOutlet UIButton *eh;
@property (nonatomic, weak) IBOutlet UIButton *fa;
@property (nonatomic, weak) IBOutlet UIButton *fb;
@property (nonatomic, weak) IBOutlet UIButton *fc;
@property (nonatomic, weak) IBOutlet UIButton *fd;
@property (nonatomic, weak) IBOutlet UIButton *fe;
@property (nonatomic, weak) IBOutlet UIButton *ff;
@property (nonatomic, weak) IBOutlet UIButton *fg;
@property (nonatomic, weak) IBOutlet UIButton *fh;
@property (nonatomic, weak) IBOutlet UIButton *ga;
@property (nonatomic, weak) IBOutlet UIButton *gb;
@property (nonatomic, weak) IBOutlet UIButton *gc;
@property (nonatomic, weak) IBOutlet UIButton *gd;
@property (nonatomic, weak) IBOutlet UIButton *ge;
@property (nonatomic, weak) IBOutlet UIButton *gf;
@property (nonatomic, weak) IBOutlet UIButton *gg;
@property (nonatomic, weak) IBOutlet UIButton *gh;
@property (nonatomic, weak) IBOutlet UIButton *ha;
@property (nonatomic, weak) IBOutlet UIButton *hb;
@property (nonatomic, weak) IBOutlet UIButton *hc;
@property (nonatomic, weak) IBOutlet UIButton *hd;
@property (nonatomic, weak) IBOutlet UIButton *he;
@property (nonatomic, weak) IBOutlet UIButton *hf;
@property (nonatomic, weak) IBOutlet UIButton *hg;
@property (nonatomic, weak) IBOutlet UIButton *hh;
@property (nonatomic, weak) IBOutlet UIImageView *hakoface;
@property (nonatomic, weak) IBOutlet UIButton *close;
@property (nonatomic, weak) IBOutlet UIButton *restart;
@property (nonatomic, weak) IBOutlet UILabel *hakotext;
@property (nonatomic, weak) IBOutlet UILabel *white_cnt;
@property (nonatomic, weak) IBOutlet UILabel *black_cnt;
@property (nonatomic, weak) IBOutlet UILabel *lev_txt;

-(IBAction)put:(UIButton*)sender;
-(IBAction)close:(id)sender;
-(IBAction)restart:(id)sender;

-(void)switchbuttons:(BOOL)s;
-(void)botput;
-(void)results;
-(void)reloadview;

@end

