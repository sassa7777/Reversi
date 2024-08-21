//
//  ViewController.mm
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import "ViewController.h"

using namespace std;

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.hakoface setImage:[NSImage imageNamed:@"hako"]];
    self.close.hidden = YES;
    self.restart.hidden = YES;
    reset();
    transpose_table_up.clear();
    transpose_table_low.clear();
    former_transpose_table_up.clear();
    former_transpose_table_low.clear();
    coordinate_y_x.clear();
    [self reloadview];
    _lev_txt.stringValue = [NSString stringWithFormat:@"Level: %d", Level];
    [self botput];
    [self reloadview];
    NSArray<NSArray<NSButton *>*> *buttons = @[
        @[self.aa, self.ab, self.ac, self.ad, self.ae, self.af, self.ag, self.ah],
        @[self.ba, self.bb, self.bc, self.bd, self.be, self.bf, self.bg, self.bh],
        @[self.ca, self.cb, self.cc, self.cd, self.ce, self.cf, self.cg, self.ch],
        @[self.da, self.db, self.dc, self.dd, self.de, self.df, self.dg, self.dh],
        @[self.ea, self.eb, self.ec, self.ed, self.ee, self.ef, self.eg, self.eh],
        @[self.fa, self.fb, self.fc, self.fd, self.fe, self.ff, self.fg, self.fh],
        @[self.ga, self.gb, self.gc, self.gd, self.ge, self.gf, self.gg, self.gh],
        @[self.ha, self.hb, self.hc, self.hd, self.he, self.hf, self.hg, self.hh]
    ];
    for (auto i = 0; i < 8; ++i) {
        for (auto j = 0; j < 8; ++j) {
            coordinate_y_x[buttons[i][j]] = make_pair(i, j);
        }
    }
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
    results = putstone(coordinate_y_x[sender].first, coordinate_y_x[sender].second);
    if(!results) return;
    swapboard();
    if(isPass()) {
        swapboard();
    }
    [self reloadview];
    [self botput];
}

- (void)reloadview {
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
    
    uint64_t legalboard = makelegalboard(b.playerboard, b.opponentboard);
    uint64_t mask = 0x8000000000000000;
    for (auto i = 0; i < 8; ++i) {
        for (auto j = 0; j < 8; ++j) {
            if(nowTurn == BLACK_TURN) {
                if(b.playerboard & mask) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"blackb"]];
                    } else {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"black"]];
                    }
                } else if(b.opponentboard & mask) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"whiteb"]];
                    } else {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"white"]];
                    }
                } else if((botplayer == WHITE_TURN && (legalboard & mask) != 0)) {
                    [buttons[i][j] setImage:[NSImage imageNamed:@"null2"]];
                } else {
                    [buttons[i][j] setImage:[NSImage imageNamed:@"null"]];
                }
            } else {
                if(b.opponentboard & mask) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"blackb"]];
                    } else {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"black"]];
                    }
                } else if(b.playerboard & mask) {
                    if(tmpx == j && tmpy == i) {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"whiteb"]];
                    } else {
                        [buttons[i][j] setImage:[NSImage imageNamed:@"white"]];
                    }
                } else if((botplayer == BLACK_TURN && (legalboard & mask) != 0)) {
                    [buttons[i][j] setImage:[NSImage imageNamed:@"null2"]];
                } else {
                    [buttons[i][j] setImage:[NSImage imageNamed:@"null"]];
                }
            }
            mask >>=1;
        }
    }
    if(nowTurn == BLACK_TURN) {
        _black_cnt.stringValue = [NSString stringWithFormat:@"黒: %d", __builtin_popcountll(b.playerboard)];
        _white_cnt.stringValue = [NSString stringWithFormat:@"白: %d", __builtin_popcountll(b.opponentboard)];
    } else {
        _black_cnt.stringValue = [NSString stringWithFormat:@"黒: %d", __builtin_popcountll(b.opponentboard)];
        _white_cnt.stringValue = [NSString stringWithFormat:@"白: %d", __builtin_popcountll(b.playerboard)];
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
                [self.hakoface setImage:[NSImage imageNamed:@"win"]];
                _hakotext.stringValue = @"君の勝ち！";
                break;
            case 2:
                NSLog(@"白の勝ち！");
                [self.hakoface setImage:[NSImage imageNamed:@"lose"]];
                _hakotext.stringValue = @"僕の勝ち！";
                break;
            default:
                NSLog(@"引き分け");
                [self.hakoface setImage:[NSImage imageNamed:@"draw"]];
                _hakotext.stringValue = @"引き分け！";
                break;
        }
    } else {
        switch (winner()) {
            case 1:
                NSLog(@"黒の勝ち！");
                [self.hakoface setImage:[NSImage imageNamed:@"lose"]];
                _hakotext.stringValue = @"僕の勝ち！";
                break;
            case 2:
                NSLog(@"白の勝ち！");
                [self.hakoface setImage:[NSImage imageNamed:@"win"]];
                _hakotext.stringValue = @"君の勝ち！";
                break;
            default:
                NSLog(@"引き分け");
                [self.hakoface setImage:[NSImage imageNamed:@"draw"]];
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
    uint64_t legalboard = makelegalboard(b.playerboard, b.opponentboard);
    uint64_t mask = 0x8000000000000000;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if((legalboard & mask)  && s == YES) {
                buttons[i][j].enabled = YES;
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
        [self.hakoface setImage:[NSImage imageNamed:@"thinking"]];
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void) {
            int results = ai();
            if(results == 1) {
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (0.2f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
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
                    [self.hakoface setImage:[NSImage imageNamed:@"hako"]];
                });
            }
        });
    }
}

@end
