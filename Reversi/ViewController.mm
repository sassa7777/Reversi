//
//  ViewController.m
//  Reversi
//
//  Created by sasa on 2024/05/17.
//

#import "ViewController.h"

NSImage *hako_default = [[NSImage alloc] initWithContentsOfFile:@"hako"];
NSImage *hako_think = [[NSImage alloc] initWithContentsOfFile:@"thinking"];
NSImage *hako_win = [[NSImage alloc] initWithContentsOfFile:@"win"];
NSImage *hako_lose = [[NSImage alloc] initWithContentsOfFile:@"lose"];

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.hakoface setImage:hako_default];
    self.close.hidden = YES;
    self.restart.hidden = YES;
    reset();
    switch (Level) {
        case 1:
            _lev_txt.stringValue = @"Level: 1";
            break;
        case 2:
            _lev_txt.stringValue = @"Level: 2";
            break;
        case 3:
            _lev_txt.stringValue = @"Level: 3";
            break;
        case 4:
            _lev_txt.stringValue = @"Level: 4";
            break;
        case 5:
            _lev_txt.stringValue = @"Level: 5";
            break;
        default:
            break;
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
    else if(sender == self.hh) results = putstone(7, 7);
    
}

- (void)reloadview {
    
}

- (void)restart:(id)sender __attribute__((ibaction)) {
}

- (void)results {
}

- (void)switchbuttons:(BOOL)s {
}

- (void)botput {
}

@end
