//
//  Objective-C-Bridge.m
//  othello
//
//  Created by sasa on 2024/03/20.
//

#import "Objective-C-Wrapper.h"

void update_hakostring(void) {
    [SwiftFunctions thinking_hako];
}

void error_hakostring(void) {
    [SwiftFunctions error_hako];
}

int bit_count(uint64_t *bit) {
    return (int)[SwiftFunctions bitcounter:*bit];
}
