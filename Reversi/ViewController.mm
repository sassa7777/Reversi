//
//  ViewController.m
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import "ViewController.h"

using namespace std;

NSImage *hako_default = [NSImage imageNamed:@"hako"];
NSImage *hako_think = [NSImage imageNamed:@"thinking"];
NSImage *hako_win = [NSImage imageNamed:@"win"];
NSImage *hako_lose = [NSImage imageNamed:@"lose"];
NSImage *hako_draw = [NSImage imageNamed:@"draw"];
NSImage *null_icon = [NSImage imageNamed:@"null"];
NSImage *null_icon2 = [NSImage imageNamed:@"null2"];
NSImage *black_stone = [NSImage imageNamed:@"black"];
NSImage *black_stone2 = [NSImage imageNamed:@"blackb"];
NSImage *white_stone = [NSImage imageNamed:@"white"];
NSImage *white_stone2 = [NSImage imageNamed:@"whiteb"];

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.hakoface setImage:hako_default];
    self.close.hidden = YES;
    self.restart.hidden = YES;
    reset();
    [self reloadview];
    _lev_txt.stringValue = [NSString stringWithFormat:@"Level: %d", Level];
    if(botplayer == BLACK_TURN) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (0.4f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [self botput];
        });
    } else {
        [self botput];
    }
    [self reloadview];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

- (void)close:(id)sender __attribute__((ibaction)) {
    [[self.view window] close];
}

- (void)put:(NSButton *)sender __attribute__((ibaction)) {
    int results;
    if(sender == self.aa) results = putstone(0, 0);
    else if(sender == self.ab) results = putstone(0, 1);
    else if(sender == self.ac) results = putstone(0, 2);
    else if(sender == self.ad) results = putstone(0, 3);
    else if(sender == self.ae) results = putstone(0, 4);
    else if(sender == self.af) results = putstone(0, 5);
    else if(sender == self.ag) results = putstone(0, 6);
    else if(sender == self.ah) results = putstone(0, 7);
    else if(sender == self.ba) results = putstone(1, 0);
    else if(sender == self.bb) results = putstone(1, 1);
    else if(sender == self.bc) results = putstone(1, 2);
    else if(sender == self.bd) results = putstone(1, 3);
    else if(sender == self.be) results = putstone(1, 4);
    else if(sender == self.bf) results = putstone(1, 5);
    else if(sender == self.bg) results = putstone(1, 6);
    else if(sender == self.bh) results = putstone(1, 7);
    else if(sender == self.ca) results = putstone(2, 0);
    else if(sender == self.cb) results = putstone(2, 1);
    else if(sender == self.cc) results = putstone(2, 2);
    else if(sender == self.cd) results = putstone(2, 3);
    else if(sender == self.ce) results = putstone(2, 4);
    else if(sender == self.cf) results = putstone(2, 5);
    else if(sender == self.cg) results = putstone(2, 6);
    else if(sender == self.ch) results = putstone(2, 7);
    else if(sender == self.da) results = putstone(3, 0);
    else if(sender == self.db) results = putstone(3, 1);
    else if(sender == self.dc) results = putstone(3, 2);
    else if(sender == self.dd) results = putstone(3, 3);
    else if(sender == self.de) results = putstone(3, 4);
    else if(sender == self.df) results = putstone(3, 5);
    else if(sender == self.dg) results = putstone(3, 6);
    else if(sender == self.dh) results = putstone(3, 7);
    else if(sender == self.ea) results = putstone(4, 0);
    else if(sender == self.eb) results = putstone(4, 1);
    else if(sender == self.ec) results = putstone(4, 2);
    else if(sender == self.ed) results = putstone(4, 3);
    else if(sender == self.ee) results = putstone(4, 4);
    else if(sender == self.ef) results = putstone(4, 5);
    else if(sender == self.eg) results = putstone(4, 6);
    else if(sender == self.eh) results = putstone(4, 7);
    else if(sender == self.fa) results = putstone(5, 0);
    else if(sender == self.fb) results = putstone(5, 1);
    else if(sender == self.fc) results = putstone(5, 2);
    else if(sender == self.fd) results = putstone(5, 3);
    else if(sender == self.fe) results = putstone(5, 4);
    else if(sender == self.ff) results = putstone(5, 5);
    else if(sender == self.fg) results = putstone(5, 6);
    else if(sender == self.fh) results = putstone(5, 7);
    else if(sender == self.ga) results = putstone(6, 0);
    else if(sender == self.gb) results = putstone(6, 1);
    else if(sender == self.gc) results = putstone(6, 2);
    else if(sender == self.gd) results = putstone(6, 3);
    else if(sender == self.ge) results = putstone(6, 4);
    else if(sender == self.gf) results = putstone(6, 5);
    else if(sender == self.gg) results = putstone(6, 6);
    else if(sender == self.gh) results = putstone(6, 7);
    else if(sender == self.ha) results = putstone(7, 0);
    else if(sender == self.hb) results = putstone(7, 1);
    else if(sender == self.hc) results = putstone(7, 2);
    else if(sender == self.hd) results = putstone(7, 3);
    else if(sender == self.he) results = putstone(7, 4);
    else if(sender == self.hf) results = putstone(7, 5);
    else if(sender == self.hg) results = putstone(7, 6);
    else results = putstone(7, 7);
    if(!results) return;
    swapboard();
    if(isPass()) {
        swapboard();
    }
    [self reloadview];
    [self botput];
}

- (void)reloadview {
    NSArray *buttons = @[
        @[self.aa, self.ab, self.ac, self.ad, self.ae, self.af, self.ag, self.ah],
        @[self.ba, self.bb, self.bc, self.bd, self.be, self.bf, self.bg, self.bh],
        @[self.ca, self.cb, self.cc, self.cd, self.ce, self.cf, self.cg, self.ch],
        @[self.da, self.db, self.dc, self.dd, self.de, self.df, self.dg, self.dh],
        @[self.ea, self.eb, self.ec, self.ed, self.ee, self.ef, self.eg, self.eh],
        @[self.fa, self.fb, self.fc, self.fd, self.fe, self.ff, self.fg, self.fh],
        @[self.ga, self.gb, self.gc, self.gd, self.ge, self.gf, self.gg, self.gh],
        @[self.ha, self.hb, self.hc, self.hd, self.he, self.hf, self.hg, self.hh]
    ];
    
    uint64_t legalboard = makelegalBoard(&b.playerboard, &b.oppenentboard);
    uint64_t mask = 0x8000000000000000;
    for (char i = 0; i < 8; ++i) {
        for (char j = 0; j < 8; ++j) {
            if(nowTurn == BLACK_TURN) {
                if((b.playerboard & mask) != 0) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:black_stone2];
                    } else {
                        [buttons[i][j] setImage:black_stone];
                    }
                } else if(((b.oppenentboard & mask) != 0)) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:white_stone2];
                    } else {
                        [buttons[i][j] setImage:white_stone];
                    }
                } else if((botplayer == WHITE_TURN && (legalboard & mask) != 0)) {
                    [buttons[i][j] setImage:null_icon2];
                } else {
                    [buttons[i][j] setImage:null_icon];
                }
            } else {
                if((b.oppenentboard & mask) != 0) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:black_stone2];
                    } else {
                        [buttons[i][j] setImage:black_stone];
                    }
                } else if(((b.playerboard & mask) != 0)) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:white_stone2];
                    } else {
                        [buttons[i][j] setImage:white_stone];
                    }
                } else if((botplayer == BLACK_TURN && (legalboard & mask) != 0)) {
                    [buttons[i][j] setImage:null_icon2];
                } else {
                    [buttons[i][j] setImage:null_icon];
                }
            }
            mask >>=1;
        }
    }
    if(nowTurn == BLACK_TURN) {
        _black_cnt.stringValue = [NSString stringWithFormat:@"黒: %d", bitcount(b.playerboard)];
        _white_cnt.stringValue = [NSString stringWithFormat:@"白: %d", bitcount(b.oppenentboard)];
    } else {
        _black_cnt.stringValue = [NSString stringWithFormat:@"黒: %d", bitcount(b.oppenentboard)];
        _white_cnt.stringValue = [NSString stringWithFormat:@"白: %d", bitcount(b.playerboard)];
    }
    NSLog(@"完了");
    if(isFinished()) {
        [self results];
        [self performSegueWithIdentifier:@"results" sender:nil];
        self.close.hidden = NO;
        self.restart.hidden = NO;
    }
}

- (void)restart:(id)sender __attribute__((ibaction)) {
    [self viewDidLoad];
}

- (void)results {
    if(botplayer == WHITE_TURN) {
        switch (winner()) {
            case 1:
                NSLog(@"黒の勝ち！");
                [self.hakoface setImage:hako_win];
                _hakotext.stringValue = @"君の勝ち！";
                break;
            case 2:
                NSLog(@"白の勝ち！");
                [self.hakoface setImage:hako_lose];
                _hakotext.stringValue = @"僕の勝ち！";
                break;
            default:
                NSLog(@"引き分け");
                [self.hakoface setImage:hako_draw];
                _hakotext.stringValue = @"引き分け！";
                break;
        }
    } else {
        switch (winner()) {
            case 1:
                NSLog(@"黒の勝ち！");
                [self.hakoface setImage:hako_lose];
                _hakotext.stringValue = @"僕の勝ち！";
                break;
            case 2:
                NSLog(@"白の勝ち！");
                [self.hakoface setImage:hako_win];
                _hakotext.stringValue = @"君の勝ち！";
                break;
            default:
                NSLog(@"引き分け");
                [self.hakoface setImage:hako_draw];
                _hakotext.stringValue = @"引き分け！";
                break;
        }
    }
}

- (void)switchbuttons:(BOOL)s {
    NSArray<NSArray<NSButton *> *> *buttons = @[
        @[self.aa, self.ab, self.ac, self.ad, self.ae, self.af, self.ag, self.ah],
        @[self.ba, self.bb, self.bc, self.bd, self.be, self.bf, self.bg, self.bh],
        @[self.ca, self.cb, self.cc, self.cd, self.ce, self.cf, self.cg, self.ch],
        @[self.da, self.db, self.dc, self.dd, self.de, self.df, self.dg, self.dh],
        @[self.ea, self.eb, self.ec, self.ed, self.ee, self.ef, self.eg, self.eh],
        @[self.fa, self.fb, self.fc, self.fd, self.fe, self.ff, self.fg, self.fh],
        @[self.ga, self.gb, self.gc, self.gd, self.ge, self.gf, self.gg, self.gh],
        @[self.ha, self.hb, self.hc, self.hd, self.he, self.hf, self.hg, self.hh]
    ];
    
    for (NSArray<NSButton *> *buttonArray in buttons) {
        for (NSButton *swiftButton in buttonArray) {
            NSButtonCell *buttonCell = [swiftButton cell];
            if ([buttonCell isKindOfClass:[NSButtonCell class]]) {
                [(NSButtonCell *)buttonCell setImageDimsWhenDisabled:NO];
            }
        }
    }
    uint64_t legalboard = makelegalBoard(&b.playerboard, &b.oppenentboard);
    uint64_t mask = 0x8000000000000000;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if((legalboard & mask) != 0) {
                if(s == YES) buttons[i][j].enabled = YES;
                else buttons[i][j].enabled = NO;
            } else {
                buttons[i][j].enabled = NO;
            }
            mask >>= 1;
        }
    }
    
}

- (void)botput {
    if(isFinished() == false) {
        [self switchbuttons:NO];
        think_percent = 0;
        _hakotext.stringValue = [NSString stringWithFormat:@"考え中...(%d%%)\n(時間がかかることがあります)", think_percent];
        [self.hakoface setImage:hako_think];
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
            int results = ai();
            if(results == 1) {
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (0.4f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                    swapboard();
                    if(isPass()) {
                        swapboard();
                    }
                    [self reloadview];
                    [self botput];
                });
            } else {
                dispatch_async(dispatch_get_main_queue(), ^(void) {
                    [self switchbuttons:YES];
                    self->_hakotext.stringValue = @"君の番だよ！\n置く場所を選んでね！";
                    [self.hakoface setImage:hako_default];
                });
            }
        });
    }
}



@end
