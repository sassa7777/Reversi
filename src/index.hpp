//
//  index.hpp
//  Reversi
//
//  Created by sasa on 2025/04/19.
//

#ifndef index_h
#define index_h

#include "reversi.hpp"

inline void SYNC_INDEX(const uint64_t put, const uint64_t rev, board &b) {
    uint64_t diff_p = put | rev;
    uint64_t diff_o = rev;
    if (diff_p & INDEX_A1) {
        b.index_p.diagonal8_0 += pow3[7];
        b.index_p.edge_2x_0 += pow3[9];
        b.index_p.edge_2x_3 += pow3[2];
        b.index_p.corner_3x3_0 += pow3[8];
        b.index_p.edge_x_side_0 += pow3[9];
        b.index_p.edge_block_0 += pow3[9];
        b.index_p.edge_block_3 += pow3[4];
        b.index_p.triangle_0 += pow3[9];
        b.index_p.corner_2x5_0 += pow3[9];
        b.index_p.corner_2x5_4 += pow3[9];
    }
    if (diff_p & INDEX_B1) {
        b.index_p.diagonal7_0 += pow3[6];
        b.index_p.edge_2x_0 += pow3[8];
        b.index_p.h_v_2_3 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[8];
        b.index_p.triangle_0 += pow3[8];
        b.index_p.corner_2x5_0 += pow3[8];
        b.index_p.corner_2x5_4 += pow3[4];
    }
    if (diff_p & INDEX_C1) {
        b.index_p.diagonal6_0 += pow3[5];
        b.index_p.edge_2x_0 += pow3[7];
        b.index_p.h_v_3_3 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[6];
        b.index_p.edge_x_side_0 += pow3[7];
        b.index_p.edge_block_0 += pow3[8];
        b.index_p.triangle_0 += pow3[7];
        b.index_p.corner_2x5_0 += pow3[7];
    }
    if (diff_p & INDEX_D1) {
        b.index_p.diagonal5_0 += pow3[4];
        b.index_p.edge_2x_0 += pow3[6];
        b.index_p.h_v_4_3 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[6];
        b.index_p.edge_x_side_1 += pow3[0];
        b.index_p.edge_block_0 += pow3[7];
        b.index_p.triangle_0 += pow3[6];
        b.index_p.corner_2x5_0 += pow3[6];
        b.index_p.corner_2x5_5 += pow3[5];
    }
    if (diff_p & INDEX_E1) {
        b.index_p.diagonal5_3 += pow3[0];
        b.index_p.edge_2x_0 += pow3[5];
        b.index_p.h_v_4_1 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[5];
        b.index_p.edge_x_side_1 += pow3[1];
        b.index_p.edge_block_0 += pow3[6];
        b.index_p.triangle_1 += pow3[0];
        b.index_p.corner_2x5_0 += pow3[5];
        b.index_p.corner_2x5_5 += pow3[6];
    }
    if (diff_p & INDEX_F1) {
        b.index_p.diagonal6_3 += pow3[0];
        b.index_p.edge_2x_0 += pow3[4];
        b.index_p.h_v_3_1 += pow3[7];
        b.index_p.corner_3x3_1 += pow3[2];
        b.index_p.edge_x_side_1 += pow3[2];
        b.index_p.edge_block_0 += pow3[5];
        b.index_p.triangle_1 += pow3[2];
        b.index_p.corner_2x5_5 += pow3[7];
    }
    if (diff_p & INDEX_G1) {
        b.index_p.diagonal7_3 += pow3[0];
        b.index_p.edge_2x_0 += pow3[3];
        b.index_p.h_v_2_1 += pow3[7];
        b.index_p.corner_3x3_1 += pow3[5];
        b.index_p.edge_x_side_1 += pow3[4];
        b.index_p.triangle_1 += pow3[5];
        b.index_p.corner_2x5_1 += pow3[4];
        b.index_p.corner_2x5_5 += pow3[8];
    }
    if (diff_p & INDEX_H1) {
        b.index_p.diagonal8_1 += pow3[7];
        b.index_p.edge_2x_0 += pow3[2];
        b.index_p.edge_2x_1 += pow3[9];
        b.index_p.corner_3x3_1 += pow3[8];
        b.index_p.edge_x_side_1 += pow3[9];
        b.index_p.edge_block_0 += pow3[4];
        b.index_p.edge_block_1 += pow3[9];
        b.index_p.triangle_1 += pow3[9];
        b.index_p.corner_2x5_1 += pow3[9];
        b.index_p.corner_2x5_5 += pow3[9];
    }
    if (diff_p & INDEX_A2) {
        b.index_p.diagonal7_2 += pow3[6];
        b.index_p.edge_2x_3 += pow3[3];
        b.index_p.h_v_2_0 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[5];
        b.index_p.edge_x_side_0 += pow3[4];
        b.index_p.triangle_0 += pow3[5];
        b.index_p.corner_2x5_0 += pow3[4];
        b.index_p.corner_2x5_4 += pow3[8];
    }
    if (diff_p & INDEX_B2) {
        b.index_p.diagonal8_0 += pow3[6];
        b.index_p.edge_2x_0 += pow3[1];
        b.index_p.edge_2x_3 += pow3[0];
        b.index_p.h_v_2_0 += pow3[6];
        b.index_p.h_v_2_3 += pow3[6];
        b.index_p.corner_3x3_0 += pow3[4];
        b.index_p.edge_x_side_0 += pow3[3];
        b.index_p.triangle_0 += pow3[4];
        b.index_p.corner_2x5_0 += pow3[3];
        b.index_p.corner_2x5_4 += pow3[3];
    }
    if (diff_p & INDEX_C2) {
        b.index_p.diagonal7_0 += pow3[5];
        b.index_p.h_v_2_0 += pow3[5];
        b.index_p.h_v_3_3 += pow3[6];
        b.index_p.corner_3x3_0 += pow3[3];
        b.index_p.edge_block_0 += pow3[3];
        b.index_p.triangle_0 += pow3[3];
        b.index_p.corner_2x5_0 += pow3[2];
    }
    if (diff_p & INDEX_D2) {
        b.index_p.diagonal6_0 += pow3[4];
        b.index_p.diagonal5_3 += pow3[1];
        b.index_p.h_v_2_0 += pow3[4];
        b.index_p.h_v_4_3 += pow3[6];
        b.index_p.edge_block_0 += pow3[2];
        b.index_p.corner_2x5_0 += pow3[1];
        b.index_p.corner_2x5_5 += pow3[0];
    }
    if (diff_p & INDEX_E2) {
        b.index_p.diagonal6_3 += pow3[1];
        b.index_p.diagonal5_0 += pow3[3];
        b.index_p.h_v_2_0 += pow3[3];
        b.index_p.h_v_4_1 += pow3[6];
        b.index_p.edge_block_0 += pow3[1];
        b.index_p.corner_2x5_0 += pow3[0];
        b.index_p.corner_2x5_5 += pow3[1];
    }
    if (diff_p & INDEX_F2) {
        b.index_p.diagonal7_3 += pow3[1];
        b.index_p.h_v_2_0 += pow3[2];
        b.index_p.h_v_3_1 += pow3[6];
        b.index_p.corner_3x3_1 += pow3[1];
        b.index_p.edge_block_0 += pow3[0];
        b.index_p.triangle_1 += pow3[1];
        b.index_p.corner_2x5_5 += pow3[2];
    }
    if (diff_p & INDEX_G2) {
        b.index_p.diagonal8_1 += pow3[6];
        b.index_p.edge_2x_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[1];
        b.index_p.h_v_2_0 += pow3[1];
        b.index_p.h_v_2_1 += pow3[6];
        b.index_p.corner_3x3_1 += pow3[4];
        b.index_p.edge_x_side_1 += pow3[3];
        b.index_p.triangle_1 += pow3[4];
        b.index_p.corner_2x5_1 += pow3[3];
        b.index_p.corner_2x5_5 += pow3[3];
    }
    if (diff_p & INDEX_H2) {
        b.index_p.diagonal7_1 += pow3[6];
        b.index_p.edge_2x_1 += pow3[8];
        b.index_p.h_v_2_0 += pow3[0];
        b.index_p.corner_3x3_1 += pow3[7];
        b.index_p.edge_x_side_1 += pow3[8];
        b.index_p.triangle_1 += pow3[8];
        b.index_p.corner_2x5_1 += pow3[8];
        b.index_p.corner_2x5_5 += pow3[4];
    }
    if (diff_p & INDEX_A3) {
        b.index_p.diagonal6_2 += pow3[5];
        b.index_p.edge_2x_3 += pow3[4];
        b.index_p.h_v_3_0 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[2];
        b.index_p.edge_x_side_0 += pow3[2];
        b.index_p.edge_block_3 += pow3[5];
        b.index_p.triangle_0 += pow3[2];
        b.index_p.corner_2x5_4 += pow3[7];
    }
    if (diff_p & INDEX_B3) {
        b.index_p.diagonal7_2 += pow3[5];
        b.index_p.h_v_2_3 += pow3[5];
        b.index_p.h_v_3_0 += pow3[6];
        b.index_p.corner_3x3_0 += pow3[1];
        b.index_p.edge_block_3 += pow3[0];
        b.index_p.triangle_0 += pow3[1];
        b.index_p.corner_2x5_4 += pow3[2];
    }
    if (diff_p & INDEX_C3) {
        b.index_p.diagonal8_0 += pow3[5];
        b.index_p.diagonal5_3 += pow3[2];
        b.index_p.h_v_3_0 += pow3[5];
        b.index_p.h_v_3_3 += pow3[5];
        b.index_p.corner_3x3_0 += pow3[0];
    }
    if (diff_p & INDEX_D3) {
        b.index_p.diagonal7_0 += pow3[4];
        b.index_p.diagonal6_3 += pow3[2];
        b.index_p.h_v_3_0 += pow3[4];
        b.index_p.h_v_4_3 += pow3[5];
    }
    if (diff_p & INDEX_E3) {
        b.index_p.diagonal7_3 += pow3[2];
        b.index_p.diagonal6_0 += pow3[3];
        b.index_p.h_v_3_0 += pow3[3];
        b.index_p.h_v_4_1 += pow3[5];
    }
    if (diff_p & INDEX_F3) {
        b.index_p.diagonal8_1 += pow3[5];
        b.index_p.diagonal5_0 += pow3[2];
        b.index_p.h_v_3_0 += pow3[2];
        b.index_p.h_v_3_1 += pow3[5];
        b.index_p.corner_3x3_1 += pow3[0];
    }
    if (diff_p & INDEX_G3) {
        b.index_p.diagonal7_1 += pow3[5];
        b.index_p.h_v_2_1 += pow3[5];
        b.index_p.h_v_3_0 += pow3[1];
        b.index_p.corner_3x3_1 += pow3[3];
        b.index_p.edge_block_1 += pow3[3];
        b.index_p.triangle_1 += pow3[3];
        b.index_p.corner_2x5_1 += pow3[2];
    }
    if (diff_p & INDEX_H3) {
        b.index_p.diagonal6_1 += pow3[0];
        b.index_p.edge_2x_1 += pow3[7];
        b.index_p.h_v_3_0 += pow3[0];
        b.index_p.corner_3x3_1 += pow3[6];
        b.index_p.edge_x_side_1 += pow3[7];
        b.index_p.edge_block_1 += pow3[8];
        b.index_p.triangle_1 += pow3[7];
        b.index_p.corner_2x5_1 += pow3[7];
    }
    if (diff_p & INDEX_A4) {
        b.index_p.diagonal5_2 += pow3[4];
        b.index_p.edge_2x_3 += pow3[5];
        b.index_p.h_v_4_0 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[1];
        b.index_p.edge_x_side_3 += pow3[5];
        b.index_p.edge_block_3 += pow3[6];
        b.index_p.triangle_0 += pow3[0];
        b.index_p.corner_2x5_3 += pow3[5];
        b.index_p.corner_2x5_4 += pow3[6];
    }
    if (diff_p & INDEX_B4) {
        b.index_p.diagonal6_2 += pow3[4];
        b.index_p.diagonal5_3 += pow3[3];
        b.index_p.h_v_2_3 += pow3[4];
        b.index_p.h_v_4_0 += pow3[6];
        b.index_p.edge_block_3 += pow3[1];
        b.index_p.corner_2x5_3 += pow3[0];
        b.index_p.corner_2x5_4 += pow3[1];
    }
    if (diff_p & INDEX_C4) {
        b.index_p.diagonal7_2 += pow3[4];
        b.index_p.diagonal6_3 += pow3[3];
        b.index_p.h_v_3_3 += pow3[4];
        b.index_p.h_v_4_0 += pow3[5];
    }
    if (diff_p & INDEX_D4) {
        b.index_p.diagonal8_0 += pow3[4];
        b.index_p.diagonal7_3 += pow3[3];
        b.index_p.h_v_4_0 += pow3[4];
        b.index_p.h_v_4_3 += pow3[4];
    }
    if (diff_p & INDEX_E4) {
        b.index_p.diagonal8_1 += pow3[4];
        b.index_p.diagonal7_0 += pow3[3];
        b.index_p.h_v_4_0 += pow3[3];
        b.index_p.h_v_4_1 += pow3[4];
    }
    if (diff_p & INDEX_F4) {
        b.index_p.diagonal7_1 += pow3[4];
        b.index_p.diagonal6_0 += pow3[2];
        b.index_p.h_v_3_1 += pow3[4];
        b.index_p.h_v_4_0 += pow3[2];
    }
    if (diff_p & INDEX_G4) {
        b.index_p.diagonal6_1 += pow3[1];
        b.index_p.diagonal5_0 += pow3[1];
        b.index_p.h_v_2_1 += pow3[4];
        b.index_p.h_v_4_0 += pow3[1];
        b.index_p.edge_block_1 += pow3[2];
        b.index_p.corner_2x5_1 += pow3[1];
        b.index_p.corner_2x5_6 += pow3[0];
    }
    if (diff_p & INDEX_H4) {
        b.index_p.diagonal5_1 += pow3[0];
        b.index_p.edge_2x_1 += pow3[6];
        b.index_p.h_v_4_0 += pow3[0];
        b.index_p.edge_x_side_1 += pow3[6];
        b.index_p.edge_x_side_2 += pow3[0];
        b.index_p.edge_block_1 += pow3[7];
        b.index_p.triangle_1 += pow3[6];
        b.index_p.corner_2x5_1 += pow3[6];
        b.index_p.corner_2x5_6 += pow3[5];
    }
    if (diff_p & INDEX_A5) {
        b.index_p.diagonal5_3 += pow3[4];
        b.index_p.edge_2x_3 += pow3[6];
        b.index_p.h_v_4_2 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[0];
        b.index_p.edge_x_side_3 += pow3[6];
        b.index_p.edge_block_3 += pow3[7];
        b.index_p.triangle_3 += pow3[6];
        b.index_p.corner_2x5_3 += pow3[6];
        b.index_p.corner_2x5_4 += pow3[5];
    }
    if (diff_p & INDEX_B5) {
        b.index_p.diagonal6_3 += pow3[4];
        b.index_p.diagonal5_2 += pow3[3];
        b.index_p.h_v_2_3 += pow3[3];
        b.index_p.h_v_4_2 += pow3[6];
        b.index_p.edge_block_3 += pow3[2];
        b.index_p.corner_2x5_3 += pow3[1];
        b.index_p.corner_2x5_4 += pow3[0];
    }
    if (diff_p & INDEX_C5) {
        b.index_p.diagonal7_3 += pow3[4];
        b.index_p.diagonal6_2 += pow3[3];
        b.index_p.h_v_3_3 += pow3[3];
        b.index_p.h_v_4_2 += pow3[5];
    }
    if (diff_p & INDEX_D5) {
        b.index_p.diagonal8_1 += pow3[3];
        b.index_p.diagonal7_2 += pow3[3];
        b.index_p.h_v_4_2 += pow3[4];
        b.index_p.h_v_4_3 += pow3[3];
    }
    if (diff_p & INDEX_E5) {
        b.index_p.diagonal8_0 += pow3[3];
        b.index_p.diagonal7_1 += pow3[3];
        b.index_p.h_v_4_1 += pow3[3];
        b.index_p.h_v_4_2 += pow3[3];
    }
    if (diff_p & INDEX_F5) {
        b.index_p.diagonal7_0 += pow3[2];
        b.index_p.diagonal6_1 += pow3[2];
        b.index_p.h_v_3_1 += pow3[3];
        b.index_p.h_v_4_2 += pow3[2];
    }
    if (diff_p & INDEX_G5) {
        b.index_p.diagonal6_0 += pow3[1];
        b.index_p.diagonal5_1 += pow3[1];
        b.index_p.h_v_2_1 += pow3[3];
        b.index_p.h_v_4_2 += pow3[1];
        b.index_p.edge_block_1 += pow3[1];
        b.index_p.corner_2x5_1 += pow3[0];
        b.index_p.corner_2x5_6 += pow3[1];
    }
    if (diff_p & INDEX_H5) {
        b.index_p.diagonal5_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[5];
        b.index_p.h_v_4_2 += pow3[0];
        b.index_p.edge_x_side_1 += pow3[5];
        b.index_p.edge_x_side_2 += pow3[1];
        b.index_p.edge_block_1 += pow3[6];
        b.index_p.triangle_2 += pow3[0];
        b.index_p.corner_2x5_1 += pow3[5];
        b.index_p.corner_2x5_6 += pow3[6];
    }
    if (diff_p & INDEX_A6) {
        b.index_p.diagonal6_3 += pow3[5];
        b.index_p.edge_2x_3 += pow3[7];
        b.index_p.h_v_3_2 += pow3[7];
        b.index_p.corner_3x3_3 += pow3[6];
        b.index_p.edge_x_side_3 += pow3[7];
        b.index_p.edge_block_3 += pow3[8];
        b.index_p.triangle_3 += pow3[7];
        b.index_p.corner_2x5_3 += pow3[7];
    }
    if (diff_p & INDEX_B6) {
        b.index_p.diagonal7_3 += pow3[5];
        b.index_p.h_v_2_3 += pow3[2];
        b.index_p.h_v_3_2 += pow3[6];
        b.index_p.corner_3x3_3 += pow3[3];
        b.index_p.edge_block_3 += pow3[3];
        b.index_p.triangle_3 += pow3[3];
        b.index_p.corner_2x5_3 += pow3[2];
    }
    if (diff_p & INDEX_C6) {
        b.index_p.diagonal8_1 += pow3[2];
        b.index_p.diagonal5_2 += pow3[2];
        b.index_p.h_v_3_2 += pow3[5];
        b.index_p.h_v_3_3 += pow3[2];
        b.index_p.corner_3x3_3 += pow3[0];
    }
    if (diff_p & INDEX_D6) {
        b.index_p.diagonal7_1 += pow3[2];
        b.index_p.diagonal6_2 += pow3[2];
        b.index_p.h_v_3_2 += pow3[4];
        b.index_p.h_v_4_3 += pow3[2];
    }
    if (diff_p & INDEX_E6) {
        b.index_p.diagonal7_2 += pow3[2];
        b.index_p.diagonal6_1 += pow3[3];
        b.index_p.h_v_3_2 += pow3[3];
        b.index_p.h_v_4_1 += pow3[2];
    }
    if (diff_p & INDEX_F6) {
        b.index_p.diagonal8_0 += pow3[2];
        b.index_p.diagonal5_1 += pow3[2];
        b.index_p.h_v_3_1 += pow3[2];
        b.index_p.h_v_3_2 += pow3[2];
        b.index_p.corner_3x3_2 += pow3[0];
    }
    if (diff_p & INDEX_G6) {
        b.index_p.diagonal7_0 += pow3[1];
        b.index_p.h_v_2_1 += pow3[2];
        b.index_p.h_v_3_2 += pow3[1];
        b.index_p.corner_3x3_2 += pow3[1];
        b.index_p.edge_block_1 += pow3[0];
        b.index_p.triangle_2 += pow3[1];
        b.index_p.corner_2x5_6 += pow3[2];
    }
    if (diff_p & INDEX_H6) {
        b.index_p.diagonal6_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[4];
        b.index_p.h_v_3_2 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[2];
        b.index_p.edge_x_side_2 += pow3[2];
        b.index_p.edge_block_1 += pow3[5];
        b.index_p.triangle_2 += pow3[2];
        b.index_p.corner_2x5_6 += pow3[7];
    }
    if (diff_p & INDEX_A7) {
        b.index_p.diagonal7_3 += pow3[6];
        b.index_p.edge_2x_3 += pow3[8];
        b.index_p.h_v_2_2 += pow3[7];
        b.index_p.corner_3x3_3 += pow3[7];
        b.index_p.edge_x_side_3 += pow3[8];
        b.index_p.triangle_3 += pow3[8];
        b.index_p.corner_2x5_3 += pow3[8];
        b.index_p.corner_2x5_7 += pow3[4];
    }
    if (diff_p & INDEX_B7) {
        b.index_p.diagonal8_1 += pow3[1];
        b.index_p.edge_2x_2 += pow3[0];
        b.index_p.edge_2x_3 += pow3[1];
        b.index_p.h_v_2_2 += pow3[6];
        b.index_p.h_v_2_3 += pow3[1];
        b.index_p.corner_3x3_3 += pow3[4];
        b.index_p.edge_x_side_3 += pow3[3];
        b.index_p.triangle_3 += pow3[4];
        b.index_p.corner_2x5_3 += pow3[3];
        b.index_p.corner_2x5_7 += pow3[3];
    }
    if (diff_p & INDEX_C7) {
        b.index_p.diagonal7_1 += pow3[1];
        b.index_p.h_v_2_2 += pow3[5];
        b.index_p.h_v_3_3 += pow3[1];
        b.index_p.corner_3x3_3 += pow3[1];
        b.index_p.edge_block_2 += pow3[0];
        b.index_p.triangle_3 += pow3[1];
        b.index_p.corner_2x5_7 += pow3[2];
    }
    if (diff_p & INDEX_D7) {
        b.index_p.diagonal6_1 += pow3[4];
        b.index_p.diagonal5_2 += pow3[1];
        b.index_p.h_v_2_2 += pow3[4];
        b.index_p.h_v_4_3 += pow3[1];
        b.index_p.edge_block_2 += pow3[1];
        b.index_p.corner_2x5_2 += pow3[0];
        b.index_p.corner_2x5_7 += pow3[1];
    }
    if (diff_p & INDEX_E7) {
        b.index_p.diagonal6_2 += pow3[1];
        b.index_p.diagonal5_1 += pow3[3];
        b.index_p.h_v_2_2 += pow3[3];
        b.index_p.h_v_4_1 += pow3[1];
        b.index_p.edge_block_2 += pow3[2];
        b.index_p.corner_2x5_2 += pow3[1];
        b.index_p.corner_2x5_7 += pow3[0];
    }
    if (diff_p & INDEX_F7) {
        b.index_p.diagonal7_2 += pow3[1];
        b.index_p.h_v_2_2 += pow3[2];
        b.index_p.h_v_3_1 += pow3[1];
        b.index_p.corner_3x3_2 += pow3[3];
        b.index_p.edge_block_2 += pow3[3];
        b.index_p.triangle_2 += pow3[3];
        b.index_p.corner_2x5_2 += pow3[2];
    }
    if (diff_p & INDEX_G7) {
        b.index_p.diagonal8_0 += pow3[1];
        b.index_p.edge_2x_1 += pow3[0];
        b.index_p.edge_2x_2 += pow3[1];
        b.index_p.h_v_2_1 += pow3[1];
        b.index_p.h_v_2_2 += pow3[1];
        b.index_p.corner_3x3_2 += pow3[4];
        b.index_p.edge_x_side_2 += pow3[3];
        b.index_p.triangle_2 += pow3[4];
        b.index_p.corner_2x5_2 += pow3[3];
        b.index_p.corner_2x5_6 += pow3[3];
    }
    if (diff_p & INDEX_H7) {
        b.index_p.diagonal7_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[3];
        b.index_p.h_v_2_2 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[5];
        b.index_p.edge_x_side_2 += pow3[4];
        b.index_p.triangle_2 += pow3[5];
        b.index_p.corner_2x5_2 += pow3[4];
        b.index_p.corner_2x5_6 += pow3[8];
    }
    if (diff_p & INDEX_A8) {
        b.index_p.diagonal8_1 += pow3[0];
        b.index_p.edge_2x_2 += pow3[2];
        b.index_p.edge_2x_3 += pow3[9];
        b.index_p.corner_3x3_3 += pow3[8];
        b.index_p.edge_x_side_3 += pow3[9];
        b.index_p.edge_block_2 += pow3[4];
        b.index_p.edge_block_3 += pow3[9];
        b.index_p.triangle_3 += pow3[9];
        b.index_p.corner_2x5_3 += pow3[9];
        b.index_p.corner_2x5_7 += pow3[9];
    }
    if (diff_p & INDEX_B8) {
        b.index_p.diagonal7_1 += pow3[0];
        b.index_p.edge_2x_2 += pow3[3];
        b.index_p.h_v_2_3 += pow3[0];
        b.index_p.corner_3x3_3 += pow3[5];
        b.index_p.edge_x_side_3 += pow3[4];
        b.index_p.triangle_3 += pow3[5];
        b.index_p.corner_2x5_3 += pow3[4];
        b.index_p.corner_2x5_7 += pow3[8];
    }
    if (diff_p & INDEX_C8) {
        b.index_p.diagonal6_1 += pow3[5];
        b.index_p.edge_2x_2 += pow3[4];
        b.index_p.h_v_3_3 += pow3[0];
        b.index_p.corner_3x3_3 += pow3[2];
        b.index_p.edge_x_side_3 += pow3[2];
        b.index_p.edge_block_2 += pow3[5];
        b.index_p.triangle_3 += pow3[2];
        b.index_p.corner_2x5_7 += pow3[7];
    }
    if (diff_p & INDEX_D8) {
        b.index_p.diagonal5_1 += pow3[4];
        b.index_p.edge_2x_2 += pow3[5];
        b.index_p.h_v_4_3 += pow3[0];
        b.index_p.edge_x_side_2 += pow3[5];
        b.index_p.edge_x_side_3 += pow3[1];
        b.index_p.edge_block_2 += pow3[6];
        b.index_p.triangle_3 += pow3[0];
        b.index_p.corner_2x5_2 += pow3[5];
        b.index_p.corner_2x5_7 += pow3[6];
    }
    if (diff_p & INDEX_E8) {
        b.index_p.diagonal5_2 += pow3[0];
        b.index_p.edge_2x_2 += pow3[6];
        b.index_p.h_v_4_1 += pow3[0];
        b.index_p.edge_x_side_2 += pow3[6];
        b.index_p.edge_x_side_3 += pow3[0];
        b.index_p.edge_block_2 += pow3[7];
        b.index_p.triangle_2 += pow3[6];
        b.index_p.corner_2x5_2 += pow3[6];
        b.index_p.corner_2x5_7 += pow3[5];
    }
    if (diff_p & INDEX_F8) {
        b.index_p.diagonal6_2 += pow3[0];
        b.index_p.edge_2x_2 += pow3[7];
        b.index_p.h_v_3_1 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[6];
        b.index_p.edge_x_side_2 += pow3[7];
        b.index_p.edge_block_2 += pow3[8];
        b.index_p.triangle_2 += pow3[7];
        b.index_p.corner_2x5_2 += pow3[7];
    }
    if (diff_p & INDEX_G8) {
        b.index_p.diagonal7_2 += pow3[0];
        b.index_p.edge_2x_2 += pow3[8];
        b.index_p.h_v_2_1 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[7];
        b.index_p.edge_x_side_2 += pow3[8];
        b.index_p.triangle_2 += pow3[8];
        b.index_p.corner_2x5_2 += pow3[8];
        b.index_p.corner_2x5_6 += pow3[4];
    }
    if (diff_p & INDEX_H8) {
        b.index_p.diagonal8_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[2];
        b.index_p.edge_2x_2 += pow3[9];
        b.index_p.corner_3x3_2 += pow3[8];
        b.index_p.edge_x_side_2 += pow3[9];
        b.index_p.edge_block_1 += pow3[4];
        b.index_p.edge_block_2 += pow3[9];
        b.index_p.triangle_2 += pow3[9];
        b.index_p.corner_2x5_2 += pow3[9];
        b.index_p.corner_2x5_6 += pow3[9];
    }
    

    if (diff_o & INDEX_A1) {
        b.index_o.diagonal8_0 -= pow3[7];
        b.index_o.edge_2x_0 -= pow3[9];
        b.index_o.edge_2x_3 -= pow3[2];
        b.index_o.corner_3x3_0 -= pow3[8];
        b.index_o.edge_x_side_0 -= pow3[9];
        b.index_o.edge_block_0 -= pow3[9];
        b.index_o.edge_block_3 -= pow3[4];
        b.index_o.triangle_0 -= pow3[9];
        b.index_o.corner_2x5_0 -= pow3[9];
        b.index_o.corner_2x5_4 -= pow3[9];
    }
    if (diff_o & INDEX_B1) {
        b.index_o.diagonal7_0 -= pow3[6];
        b.index_o.edge_2x_0 -= pow3[8];
        b.index_o.h_v_2_3 -= pow3[7];
        b.index_o.corner_3x3_0 -= pow3[7];
        b.index_o.edge_x_side_0 -= pow3[8];
        b.index_o.triangle_0 -= pow3[8];
        b.index_o.corner_2x5_0 -= pow3[8];
        b.index_o.corner_2x5_4 -= pow3[4];
    }
    if (diff_o & INDEX_C1) {
        b.index_o.diagonal6_0 -= pow3[5];
        b.index_o.edge_2x_0 -= pow3[7];
        b.index_o.h_v_3_3 -= pow3[7];
        b.index_o.corner_3x3_0 -= pow3[6];
        b.index_o.edge_x_side_0 -= pow3[7];
        b.index_o.edge_block_0 -= pow3[8];
        b.index_o.triangle_0 -= pow3[7];
        b.index_o.corner_2x5_0 -= pow3[7];
    }
    if (diff_o & INDEX_D1) {
        b.index_o.diagonal5_0 -= pow3[4];
        b.index_o.edge_2x_0 -= pow3[6];
        b.index_o.h_v_4_3 -= pow3[7];
        b.index_o.edge_x_side_0 -= pow3[6];
        b.index_o.edge_x_side_1 -= pow3[0];
        b.index_o.edge_block_0 -= pow3[7];
        b.index_o.triangle_0 -= pow3[6];
        b.index_o.corner_2x5_0 -= pow3[6];
        b.index_o.corner_2x5_5 -= pow3[5];
    }
    if (diff_o & INDEX_E1) {
        b.index_o.diagonal5_3 -= pow3[0];
        b.index_o.edge_2x_0 -= pow3[5];
        b.index_o.h_v_4_1 -= pow3[7];
        b.index_o.edge_x_side_0 -= pow3[5];
        b.index_o.edge_x_side_1 -= pow3[1];
        b.index_o.edge_block_0 -= pow3[6];
        b.index_o.triangle_1 -= pow3[0];
        b.index_o.corner_2x5_0 -= pow3[5];
        b.index_o.corner_2x5_5 -= pow3[6];
    }
    if (diff_o & INDEX_F1) {
        b.index_o.diagonal6_3 -= pow3[0];
        b.index_o.edge_2x_0 -= pow3[4];
        b.index_o.h_v_3_1 -= pow3[7];
        b.index_o.corner_3x3_1 -= pow3[2];
        b.index_o.edge_x_side_1 -= pow3[2];
        b.index_o.edge_block_0 -= pow3[5];
        b.index_o.triangle_1 -= pow3[2];
        b.index_o.corner_2x5_5 -= pow3[7];
    }
    if (diff_o & INDEX_G1) {
        b.index_o.diagonal7_3 -= pow3[0];
        b.index_o.edge_2x_0 -= pow3[3];
        b.index_o.h_v_2_1 -= pow3[7];
        b.index_o.corner_3x3_1 -= pow3[5];
        b.index_o.edge_x_side_1 -= pow3[4];
        b.index_o.triangle_1 -= pow3[5];
        b.index_o.corner_2x5_1 -= pow3[4];
        b.index_o.corner_2x5_5 -= pow3[8];
    }
    if (diff_o & INDEX_H1) {
        b.index_o.diagonal8_1 -= pow3[7];
        b.index_o.edge_2x_0 -= pow3[2];
        b.index_o.edge_2x_1 -= pow3[9];
        b.index_o.corner_3x3_1 -= pow3[8];
        b.index_o.edge_x_side_1 -= pow3[9];
        b.index_o.edge_block_0 -= pow3[4];
        b.index_o.edge_block_1 -= pow3[9];
        b.index_o.triangle_1 -= pow3[9];
        b.index_o.corner_2x5_1 -= pow3[9];
        b.index_o.corner_2x5_5 -= pow3[9];
    }
    if (diff_o & INDEX_A2) {
        b.index_o.diagonal7_2 -= pow3[6];
        b.index_o.edge_2x_3 -= pow3[3];
        b.index_o.h_v_2_0 -= pow3[7];
        b.index_o.corner_3x3_0 -= pow3[5];
        b.index_o.edge_x_side_0 -= pow3[4];
        b.index_o.triangle_0 -= pow3[5];
        b.index_o.corner_2x5_0 -= pow3[4];
        b.index_o.corner_2x5_4 -= pow3[8];
    }
    if (diff_o & INDEX_B2) {
        b.index_o.diagonal8_0 -= pow3[6];
        b.index_o.edge_2x_0 -= pow3[1];
        b.index_o.edge_2x_3 -= pow3[0];
        b.index_o.h_v_2_0 -= pow3[6];
        b.index_o.h_v_2_3 -= pow3[6];
        b.index_o.corner_3x3_0 -= pow3[4];
        b.index_o.edge_x_side_0 -= pow3[3];
        b.index_o.triangle_0 -= pow3[4];
        b.index_o.corner_2x5_0 -= pow3[3];
        b.index_o.corner_2x5_4 -= pow3[3];
    }
    if (diff_o & INDEX_C2) {
        b.index_o.diagonal7_0 -= pow3[5];
        b.index_o.h_v_2_0 -= pow3[5];
        b.index_o.h_v_3_3 -= pow3[6];
        b.index_o.corner_3x3_0 -= pow3[3];
        b.index_o.edge_block_0 -= pow3[3];
        b.index_o.triangle_0 -= pow3[3];
        b.index_o.corner_2x5_0 -= pow3[2];
    }
    if (diff_o & INDEX_D2) {
        b.index_o.diagonal6_0 -= pow3[4];
        b.index_o.diagonal5_3 -= pow3[1];
        b.index_o.h_v_2_0 -= pow3[4];
        b.index_o.h_v_4_3 -= pow3[6];
        b.index_o.edge_block_0 -= pow3[2];
        b.index_o.corner_2x5_0 -= pow3[1];
        b.index_o.corner_2x5_5 -= pow3[0];
    }
    if (diff_o & INDEX_E2) {
        b.index_o.diagonal6_3 -= pow3[1];
        b.index_o.diagonal5_0 -= pow3[3];
        b.index_o.h_v_2_0 -= pow3[3];
        b.index_o.h_v_4_1 -= pow3[6];
        b.index_o.edge_block_0 -= pow3[1];
        b.index_o.corner_2x5_0 -= pow3[0];
        b.index_o.corner_2x5_5 -= pow3[1];
    }
    if (diff_o & INDEX_F2) {
        b.index_o.diagonal7_3 -= pow3[1];
        b.index_o.h_v_2_0 -= pow3[2];
        b.index_o.h_v_3_1 -= pow3[6];
        b.index_o.corner_3x3_1 -= pow3[1];
        b.index_o.edge_block_0 -= pow3[0];
        b.index_o.triangle_1 -= pow3[1];
        b.index_o.corner_2x5_5 -= pow3[2];
    }
    if (diff_o & INDEX_G2) {
        b.index_o.diagonal8_1 -= pow3[6];
        b.index_o.edge_2x_0 -= pow3[0];
        b.index_o.edge_2x_1 -= pow3[1];
        b.index_o.h_v_2_0 -= pow3[1];
        b.index_o.h_v_2_1 -= pow3[6];
        b.index_o.corner_3x3_1 -= pow3[4];
        b.index_o.edge_x_side_1 -= pow3[3];
        b.index_o.triangle_1 -= pow3[4];
        b.index_o.corner_2x5_1 -= pow3[3];
        b.index_o.corner_2x5_5 -= pow3[3];
    }
    if (diff_o & INDEX_H2) {
        b.index_o.diagonal7_1 -= pow3[6];
        b.index_o.edge_2x_1 -= pow3[8];
        b.index_o.h_v_2_0 -= pow3[0];
        b.index_o.corner_3x3_1 -= pow3[7];
        b.index_o.edge_x_side_1 -= pow3[8];
        b.index_o.triangle_1 -= pow3[8];
        b.index_o.corner_2x5_1 -= pow3[8];
        b.index_o.corner_2x5_5 -= pow3[4];
    }
    if (diff_o & INDEX_A3) {
        b.index_o.diagonal6_2 -= pow3[5];
        b.index_o.edge_2x_3 -= pow3[4];
        b.index_o.h_v_3_0 -= pow3[7];
        b.index_o.corner_3x3_0 -= pow3[2];
        b.index_o.edge_x_side_0 -= pow3[2];
        b.index_o.edge_block_3 -= pow3[5];
        b.index_o.triangle_0 -= pow3[2];
        b.index_o.corner_2x5_4 -= pow3[7];
    }
    if (diff_o & INDEX_B3) {
        b.index_o.diagonal7_2 -= pow3[5];
        b.index_o.h_v_2_3 -= pow3[5];
        b.index_o.h_v_3_0 -= pow3[6];
        b.index_o.corner_3x3_0 -= pow3[1];
        b.index_o.edge_block_3 -= pow3[0];
        b.index_o.triangle_0 -= pow3[1];
        b.index_o.corner_2x5_4 -= pow3[2];
    }
    if (diff_o & INDEX_C3) {
        b.index_o.diagonal8_0 -= pow3[5];
        b.index_o.diagonal5_3 -= pow3[2];
        b.index_o.h_v_3_0 -= pow3[5];
        b.index_o.h_v_3_3 -= pow3[5];
        b.index_o.corner_3x3_0 -= pow3[0];
    }
    if (diff_o & INDEX_D3) {
        b.index_o.diagonal7_0 -= pow3[4];
        b.index_o.diagonal6_3 -= pow3[2];
        b.index_o.h_v_3_0 -= pow3[4];
        b.index_o.h_v_4_3 -= pow3[5];
    }
    if (diff_o & INDEX_E3) {
        b.index_o.diagonal7_3 -= pow3[2];
        b.index_o.diagonal6_0 -= pow3[3];
        b.index_o.h_v_3_0 -= pow3[3];
        b.index_o.h_v_4_1 -= pow3[5];
    }
    if (diff_o & INDEX_F3) {
        b.index_o.diagonal8_1 -= pow3[5];
        b.index_o.diagonal5_0 -= pow3[2];
        b.index_o.h_v_3_0 -= pow3[2];
        b.index_o.h_v_3_1 -= pow3[5];
        b.index_o.corner_3x3_1 -= pow3[0];
    }
    if (diff_o & INDEX_G3) {
        b.index_o.diagonal7_1 -= pow3[5];
        b.index_o.h_v_2_1 -= pow3[5];
        b.index_o.h_v_3_0 -= pow3[1];
        b.index_o.corner_3x3_1 -= pow3[3];
        b.index_o.edge_block_1 -= pow3[3];
        b.index_o.triangle_1 -= pow3[3];
        b.index_o.corner_2x5_1 -= pow3[2];
    }
    if (diff_o & INDEX_H3) {
        b.index_o.diagonal6_1 -= pow3[0];
        b.index_o.edge_2x_1 -= pow3[7];
        b.index_o.h_v_3_0 -= pow3[0];
        b.index_o.corner_3x3_1 -= pow3[6];
        b.index_o.edge_x_side_1 -= pow3[7];
        b.index_o.edge_block_1 -= pow3[8];
        b.index_o.triangle_1 -= pow3[7];
        b.index_o.corner_2x5_1 -= pow3[7];
    }
    if (diff_o & INDEX_A4) {
        b.index_o.diagonal5_2 -= pow3[4];
        b.index_o.edge_2x_3 -= pow3[5];
        b.index_o.h_v_4_0 -= pow3[7];
        b.index_o.edge_x_side_0 -= pow3[1];
        b.index_o.edge_x_side_3 -= pow3[5];
        b.index_o.edge_block_3 -= pow3[6];
        b.index_o.triangle_0 -= pow3[0];
        b.index_o.corner_2x5_3 -= pow3[5];
        b.index_o.corner_2x5_4 -= pow3[6];
    }
    if (diff_o & INDEX_B4) {
        b.index_o.diagonal6_2 -= pow3[4];
        b.index_o.diagonal5_3 -= pow3[3];
        b.index_o.h_v_2_3 -= pow3[4];
        b.index_o.h_v_4_0 -= pow3[6];
        b.index_o.edge_block_3 -= pow3[1];
        b.index_o.corner_2x5_3 -= pow3[0];
        b.index_o.corner_2x5_4 -= pow3[1];
    }
    if (diff_o & INDEX_C4) {
        b.index_o.diagonal7_2 -= pow3[4];
        b.index_o.diagonal6_3 -= pow3[3];
        b.index_o.h_v_3_3 -= pow3[4];
        b.index_o.h_v_4_0 -= pow3[5];
    }
    if (diff_o & INDEX_D4) {
        b.index_o.diagonal8_0 -= pow3[4];
        b.index_o.diagonal7_3 -= pow3[3];
        b.index_o.h_v_4_0 -= pow3[4];
        b.index_o.h_v_4_3 -= pow3[4];
    }
    if (diff_o & INDEX_E4) {
        b.index_o.diagonal8_1 -= pow3[4];
        b.index_o.diagonal7_0 -= pow3[3];
        b.index_o.h_v_4_0 -= pow3[3];
        b.index_o.h_v_4_1 -= pow3[4];
    }
    if (diff_o & INDEX_F4) {
        b.index_o.diagonal7_1 -= pow3[4];
        b.index_o.diagonal6_0 -= pow3[2];
        b.index_o.h_v_3_1 -= pow3[4];
        b.index_o.h_v_4_0 -= pow3[2];
    }
    if (diff_o & INDEX_G4) {
        b.index_o.diagonal6_1 -= pow3[1];
        b.index_o.diagonal5_0 -= pow3[1];
        b.index_o.h_v_2_1 -= pow3[4];
        b.index_o.h_v_4_0 -= pow3[1];
        b.index_o.edge_block_1 -= pow3[2];
        b.index_o.corner_2x5_1 -= pow3[1];
        b.index_o.corner_2x5_6 -= pow3[0];
    }
    if (diff_o & INDEX_H4) {
        b.index_o.diagonal5_1 -= pow3[0];
        b.index_o.edge_2x_1 -= pow3[6];
        b.index_o.h_v_4_0 -= pow3[0];
        b.index_o.edge_x_side_1 -= pow3[6];
        b.index_o.edge_x_side_2 -= pow3[0];
        b.index_o.edge_block_1 -= pow3[7];
        b.index_o.triangle_1 -= pow3[6];
        b.index_o.corner_2x5_1 -= pow3[6];
        b.index_o.corner_2x5_6 -= pow3[5];
    }
    if (diff_o & INDEX_A5) {
        b.index_o.diagonal5_3 -= pow3[4];
        b.index_o.edge_2x_3 -= pow3[6];
        b.index_o.h_v_4_2 -= pow3[7];
        b.index_o.edge_x_side_0 -= pow3[0];
        b.index_o.edge_x_side_3 -= pow3[6];
        b.index_o.edge_block_3 -= pow3[7];
        b.index_o.triangle_3 -= pow3[6];
        b.index_o.corner_2x5_3 -= pow3[6];
        b.index_o.corner_2x5_4 -= pow3[5];
    }
    if (diff_o & INDEX_B5) {
        b.index_o.diagonal6_3 -= pow3[4];
        b.index_o.diagonal5_2 -= pow3[3];
        b.index_o.h_v_2_3 -= pow3[3];
        b.index_o.h_v_4_2 -= pow3[6];
        b.index_o.edge_block_3 -= pow3[2];
        b.index_o.corner_2x5_3 -= pow3[1];
        b.index_o.corner_2x5_4 -= pow3[0];
    }
    if (diff_o & INDEX_C5) {
        b.index_o.diagonal7_3 -= pow3[4];
        b.index_o.diagonal6_2 -= pow3[3];
        b.index_o.h_v_3_3 -= pow3[3];
        b.index_o.h_v_4_2 -= pow3[5];
    }
    if (diff_o & INDEX_D5) {
        b.index_o.diagonal8_1 -= pow3[3];
        b.index_o.diagonal7_2 -= pow3[3];
        b.index_o.h_v_4_2 -= pow3[4];
        b.index_o.h_v_4_3 -= pow3[3];
    }
    if (diff_o & INDEX_E5) {
        b.index_o.diagonal8_0 -= pow3[3];
        b.index_o.diagonal7_1 -= pow3[3];
        b.index_o.h_v_4_1 -= pow3[3];
        b.index_o.h_v_4_2 -= pow3[3];
    }
    if (diff_o & INDEX_F5) {
        b.index_o.diagonal7_0 -= pow3[2];
        b.index_o.diagonal6_1 -= pow3[2];
        b.index_o.h_v_3_1 -= pow3[3];
        b.index_o.h_v_4_2 -= pow3[2];
    }
    if (diff_o & INDEX_G5) {
        b.index_o.diagonal6_0 -= pow3[1];
        b.index_o.diagonal5_1 -= pow3[1];
        b.index_o.h_v_2_1 -= pow3[3];
        b.index_o.h_v_4_2 -= pow3[1];
        b.index_o.edge_block_1 -= pow3[1];
        b.index_o.corner_2x5_1 -= pow3[0];
        b.index_o.corner_2x5_6 -= pow3[1];
    }
    if (diff_o & INDEX_H5) {
        b.index_o.diagonal5_0 -= pow3[0];
        b.index_o.edge_2x_1 -= pow3[5];
        b.index_o.h_v_4_2 -= pow3[0];
        b.index_o.edge_x_side_1 -= pow3[5];
        b.index_o.edge_x_side_2 -= pow3[1];
        b.index_o.edge_block_1 -= pow3[6];
        b.index_o.triangle_2 -= pow3[0];
        b.index_o.corner_2x5_1 -= pow3[5];
        b.index_o.corner_2x5_6 -= pow3[6];
    }
    if (diff_o & INDEX_A6) {
        b.index_o.diagonal6_3 -= pow3[5];
        b.index_o.edge_2x_3 -= pow3[7];
        b.index_o.h_v_3_2 -= pow3[7];
        b.index_o.corner_3x3_3 -= pow3[6];
        b.index_o.edge_x_side_3 -= pow3[7];
        b.index_o.edge_block_3 -= pow3[8];
        b.index_o.triangle_3 -= pow3[7];
        b.index_o.corner_2x5_3 -= pow3[7];
    }
    if (diff_o & INDEX_B6) {
        b.index_o.diagonal7_3 -= pow3[5];
        b.index_o.h_v_2_3 -= pow3[2];
        b.index_o.h_v_3_2 -= pow3[6];
        b.index_o.corner_3x3_3 -= pow3[3];
        b.index_o.edge_block_3 -= pow3[3];
        b.index_o.triangle_3 -= pow3[3];
        b.index_o.corner_2x5_3 -= pow3[2];
    }
    if (diff_o & INDEX_C6) {
        b.index_o.diagonal8_1 -= pow3[2];
        b.index_o.diagonal5_2 -= pow3[2];
        b.index_o.h_v_3_2 -= pow3[5];
        b.index_o.h_v_3_3 -= pow3[2];
        b.index_o.corner_3x3_3 -= pow3[0];
    }
    if (diff_o & INDEX_D6) {
        b.index_o.diagonal7_1 -= pow3[2];
        b.index_o.diagonal6_2 -= pow3[2];
        b.index_o.h_v_3_2 -= pow3[4];
        b.index_o.h_v_4_3 -= pow3[2];
    }
    if (diff_o & INDEX_E6) {
        b.index_o.diagonal7_2 -= pow3[2];
        b.index_o.diagonal6_1 -= pow3[3];
        b.index_o.h_v_3_2 -= pow3[3];
        b.index_o.h_v_4_1 -= pow3[2];
    }
    if (diff_o & INDEX_F6) {
        b.index_o.diagonal8_0 -= pow3[2];
        b.index_o.diagonal5_1 -= pow3[2];
        b.index_o.h_v_3_1 -= pow3[2];
        b.index_o.h_v_3_2 -= pow3[2];
        b.index_o.corner_3x3_2 -= pow3[0];
    }
    if (diff_o & INDEX_G6) {
        b.index_o.diagonal7_0 -= pow3[1];
        b.index_o.h_v_2_1 -= pow3[2];
        b.index_o.h_v_3_2 -= pow3[1];
        b.index_o.corner_3x3_2 -= pow3[1];
        b.index_o.edge_block_1 -= pow3[0];
        b.index_o.triangle_2 -= pow3[1];
        b.index_o.corner_2x5_6 -= pow3[2];
    }
    if (diff_o & INDEX_H6) {
        b.index_o.diagonal6_0 -= pow3[0];
        b.index_o.edge_2x_1 -= pow3[4];
        b.index_o.h_v_3_2 -= pow3[0];
        b.index_o.corner_3x3_2 -= pow3[2];
        b.index_o.edge_x_side_2 -= pow3[2];
        b.index_o.edge_block_1 -= pow3[5];
        b.index_o.triangle_2 -= pow3[2];
        b.index_o.corner_2x5_6 -= pow3[7];
    }
    if (diff_o & INDEX_A7) {
        b.index_o.diagonal7_3 -= pow3[6];
        b.index_o.edge_2x_3 -= pow3[8];
        b.index_o.h_v_2_2 -= pow3[7];
        b.index_o.corner_3x3_3 -= pow3[7];
        b.index_o.edge_x_side_3 -= pow3[8];
        b.index_o.triangle_3 -= pow3[8];
        b.index_o.corner_2x5_3 -= pow3[8];
        b.index_o.corner_2x5_7 -= pow3[4];
    }
    if (diff_o & INDEX_B7) {
        b.index_o.diagonal8_1 -= pow3[1];
        b.index_o.edge_2x_2 -= pow3[0];
        b.index_o.edge_2x_3 -= pow3[1];
        b.index_o.h_v_2_2 -= pow3[6];
        b.index_o.h_v_2_3 -= pow3[1];
        b.index_o.corner_3x3_3 -= pow3[4];
        b.index_o.edge_x_side_3 -= pow3[3];
        b.index_o.triangle_3 -= pow3[4];
        b.index_o.corner_2x5_3 -= pow3[3];
        b.index_o.corner_2x5_7 -= pow3[3];
    }
    if (diff_o & INDEX_C7) {
        b.index_o.diagonal7_1 -= pow3[1];
        b.index_o.h_v_2_2 -= pow3[5];
        b.index_o.h_v_3_3 -= pow3[1];
        b.index_o.corner_3x3_3 -= pow3[1];
        b.index_o.edge_block_2 -= pow3[0];
        b.index_o.triangle_3 -= pow3[1];
        b.index_o.corner_2x5_7 -= pow3[2];
    }
    if (diff_o & INDEX_D7) {
        b.index_o.diagonal6_1 -= pow3[4];
        b.index_o.diagonal5_2 -= pow3[1];
        b.index_o.h_v_2_2 -= pow3[4];
        b.index_o.h_v_4_3 -= pow3[1];
        b.index_o.edge_block_2 -= pow3[1];
        b.index_o.corner_2x5_2 -= pow3[0];
        b.index_o.corner_2x5_7 -= pow3[1];
    }
    if (diff_o & INDEX_E7) {
        b.index_o.diagonal6_2 -= pow3[1];
        b.index_o.diagonal5_1 -= pow3[3];
        b.index_o.h_v_2_2 -= pow3[3];
        b.index_o.h_v_4_1 -= pow3[1];
        b.index_o.edge_block_2 -= pow3[2];
        b.index_o.corner_2x5_2 -= pow3[1];
        b.index_o.corner_2x5_7 -= pow3[0];
    }
    if (diff_o & INDEX_F7) {
        b.index_o.diagonal7_2 -= pow3[1];
        b.index_o.h_v_2_2 -= pow3[2];
        b.index_o.h_v_3_1 -= pow3[1];
        b.index_o.corner_3x3_2 -= pow3[3];
        b.index_o.edge_block_2 -= pow3[3];
        b.index_o.triangle_2 -= pow3[3];
        b.index_o.corner_2x5_2 -= pow3[2];
    }
    if (diff_o & INDEX_G7) {
        b.index_o.diagonal8_0 -= pow3[1];
        b.index_o.edge_2x_1 -= pow3[0];
        b.index_o.edge_2x_2 -= pow3[1];
        b.index_o.h_v_2_1 -= pow3[1];
        b.index_o.h_v_2_2 -= pow3[1];
        b.index_o.corner_3x3_2 -= pow3[4];
        b.index_o.edge_x_side_2 -= pow3[3];
        b.index_o.triangle_2 -= pow3[4];
        b.index_o.corner_2x5_2 -= pow3[3];
        b.index_o.corner_2x5_6 -= pow3[3];
    }
    if (diff_o & INDEX_H7) {
        b.index_o.diagonal7_0 -= pow3[0];
        b.index_o.edge_2x_1 -= pow3[3];
        b.index_o.h_v_2_2 -= pow3[0];
        b.index_o.corner_3x3_2 -= pow3[5];
        b.index_o.edge_x_side_2 -= pow3[4];
        b.index_o.triangle_2 -= pow3[5];
        b.index_o.corner_2x5_2 -= pow3[4];
        b.index_o.corner_2x5_6 -= pow3[8];
    }
    if (diff_o & INDEX_A8) {
        b.index_o.diagonal8_1 -= pow3[0];
        b.index_o.edge_2x_2 -= pow3[2];
        b.index_o.edge_2x_3 -= pow3[9];
        b.index_o.corner_3x3_3 -= pow3[8];
        b.index_o.edge_x_side_3 -= pow3[9];
        b.index_o.edge_block_2 -= pow3[4];
        b.index_o.edge_block_3 -= pow3[9];
        b.index_o.triangle_3 -= pow3[9];
        b.index_o.corner_2x5_3 -= pow3[9];
        b.index_o.corner_2x5_7 -= pow3[9];
    }
    if (diff_o & INDEX_B8) {
        b.index_o.diagonal7_1 -= pow3[0];
        b.index_o.edge_2x_2 -= pow3[3];
        b.index_o.h_v_2_3 -= pow3[0];
        b.index_o.corner_3x3_3 -= pow3[5];
        b.index_o.edge_x_side_3 -= pow3[4];
        b.index_o.triangle_3 -= pow3[5];
        b.index_o.corner_2x5_3 -= pow3[4];
        b.index_o.corner_2x5_7 -= pow3[8];
    }
    if (diff_o & INDEX_C8) {
        b.index_o.diagonal6_1 -= pow3[5];
        b.index_o.edge_2x_2 -= pow3[4];
        b.index_o.h_v_3_3 -= pow3[0];
        b.index_o.corner_3x3_3 -= pow3[2];
        b.index_o.edge_x_side_3 -= pow3[2];
        b.index_o.edge_block_2 -= pow3[5];
        b.index_o.triangle_3 -= pow3[2];
        b.index_o.corner_2x5_7 -= pow3[7];
    }
    if (diff_o & INDEX_D8) {
        b.index_o.diagonal5_1 -= pow3[4];
        b.index_o.edge_2x_2 -= pow3[5];
        b.index_o.h_v_4_3 -= pow3[0];
        b.index_o.edge_x_side_2 -= pow3[5];
        b.index_o.edge_x_side_3 -= pow3[1];
        b.index_o.edge_block_2 -= pow3[6];
        b.index_o.triangle_3 -= pow3[0];
        b.index_o.corner_2x5_2 -= pow3[5];
        b.index_o.corner_2x5_7 -= pow3[6];
    }
    if (diff_o & INDEX_E8) {
        b.index_o.diagonal5_2 -= pow3[0];
        b.index_o.edge_2x_2 -= pow3[6];
        b.index_o.h_v_4_1 -= pow3[0];
        b.index_o.edge_x_side_2 -= pow3[6];
        b.index_o.edge_x_side_3 -= pow3[0];
        b.index_o.edge_block_2 -= pow3[7];
        b.index_o.triangle_2 -= pow3[6];
        b.index_o.corner_2x5_2 -= pow3[6];
        b.index_o.corner_2x5_7 -= pow3[5];
    }
    if (diff_o & INDEX_F8) {
        b.index_o.diagonal6_2 -= pow3[0];
        b.index_o.edge_2x_2 -= pow3[7];
        b.index_o.h_v_3_1 -= pow3[0];
        b.index_o.corner_3x3_2 -= pow3[6];
        b.index_o.edge_x_side_2 -= pow3[7];
        b.index_o.edge_block_2 -= pow3[8];
        b.index_o.triangle_2 -= pow3[7];
        b.index_o.corner_2x5_2 -= pow3[7];
    }
    if (diff_o & INDEX_G8) {
        b.index_o.diagonal7_2 -= pow3[0];
        b.index_o.edge_2x_2 -= pow3[8];
        b.index_o.h_v_2_1 -= pow3[0];
        b.index_o.corner_3x3_2 -= pow3[7];
        b.index_o.edge_x_side_2 -= pow3[8];
        b.index_o.triangle_2 -= pow3[8];
        b.index_o.corner_2x5_2 -= pow3[8];
        b.index_o.corner_2x5_6 -= pow3[4];
    }
    if (diff_o & INDEX_H8) {
        b.index_o.diagonal8_0 -= pow3[0];
        b.index_o.edge_2x_1 -= pow3[2];
        b.index_o.edge_2x_2 -= pow3[9];
        b.index_o.corner_3x3_2 -= pow3[8];
        b.index_o.edge_x_side_2 -= pow3[9];
        b.index_o.edge_block_1 -= pow3[4];
        b.index_o.edge_block_2 -= pow3[9];
        b.index_o.triangle_2 -= pow3[9];
        b.index_o.corner_2x5_2 -= pow3[9];
        b.index_o.corner_2x5_6 -= pow3[9];
    }


    
}

inline void INIT_INDEX(board &b) {
    uint64_t diff_p = b.p;
    uint64_t diff_o = b.o;
    if (diff_p & INDEX_A1) {
        b.index_p.diagonal8_0 += pow3[7];
        b.index_p.edge_2x_0 += pow3[9];
        b.index_p.edge_2x_3 += pow3[2];
        b.index_p.corner_3x3_0 += pow3[8];
        b.index_p.edge_x_side_0 += pow3[9];
        b.index_p.edge_block_0 += pow3[9];
        b.index_p.edge_block_3 += pow3[4];
        b.index_p.triangle_0 += pow3[9];
        b.index_p.corner_2x5_0 += pow3[9];
        b.index_p.corner_2x5_4 += pow3[9];
    }
    if (diff_p & INDEX_B1) {
        b.index_p.diagonal7_0 += pow3[6];
        b.index_p.edge_2x_0 += pow3[8];
        b.index_p.h_v_2_3 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[8];
        b.index_p.triangle_0 += pow3[8];
        b.index_p.corner_2x5_0 += pow3[8];
        b.index_p.corner_2x5_4 += pow3[4];
    }
    if (diff_p & INDEX_C1) {
        b.index_p.diagonal6_0 += pow3[5];
        b.index_p.edge_2x_0 += pow3[7];
        b.index_p.h_v_3_3 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[6];
        b.index_p.edge_x_side_0 += pow3[7];
        b.index_p.edge_block_0 += pow3[8];
        b.index_p.triangle_0 += pow3[7];
        b.index_p.corner_2x5_0 += pow3[7];
    }
    if (diff_p & INDEX_D1) {
        b.index_p.diagonal5_0 += pow3[4];
        b.index_p.edge_2x_0 += pow3[6];
        b.index_p.h_v_4_3 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[6];
        b.index_p.edge_x_side_1 += pow3[0];
        b.index_p.edge_block_0 += pow3[7];
        b.index_p.triangle_0 += pow3[6];
        b.index_p.corner_2x5_0 += pow3[6];
        b.index_p.corner_2x5_5 += pow3[5];
    }
    if (diff_p & INDEX_E1) {
        b.index_p.diagonal5_3 += pow3[0];
        b.index_p.edge_2x_0 += pow3[5];
        b.index_p.h_v_4_1 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[5];
        b.index_p.edge_x_side_1 += pow3[1];
        b.index_p.edge_block_0 += pow3[6];
        b.index_p.triangle_1 += pow3[0];
        b.index_p.corner_2x5_0 += pow3[5];
        b.index_p.corner_2x5_5 += pow3[6];
    }
    if (diff_p & INDEX_F1) {
        b.index_p.diagonal6_3 += pow3[0];
        b.index_p.edge_2x_0 += pow3[4];
        b.index_p.h_v_3_1 += pow3[7];
        b.index_p.corner_3x3_1 += pow3[2];
        b.index_p.edge_x_side_1 += pow3[2];
        b.index_p.edge_block_0 += pow3[5];
        b.index_p.triangle_1 += pow3[2];
        b.index_p.corner_2x5_5 += pow3[7];
    }
    if (diff_p & INDEX_G1) {
        b.index_p.diagonal7_3 += pow3[0];
        b.index_p.edge_2x_0 += pow3[3];
        b.index_p.h_v_2_1 += pow3[7];
        b.index_p.corner_3x3_1 += pow3[5];
        b.index_p.edge_x_side_1 += pow3[4];
        b.index_p.triangle_1 += pow3[5];
        b.index_p.corner_2x5_1 += pow3[4];
        b.index_p.corner_2x5_5 += pow3[8];
    }
    if (diff_p & INDEX_H1) {
        b.index_p.diagonal8_1 += pow3[7];
        b.index_p.edge_2x_0 += pow3[2];
        b.index_p.edge_2x_1 += pow3[9];
        b.index_p.corner_3x3_1 += pow3[8];
        b.index_p.edge_x_side_1 += pow3[9];
        b.index_p.edge_block_0 += pow3[4];
        b.index_p.edge_block_1 += pow3[9];
        b.index_p.triangle_1 += pow3[9];
        b.index_p.corner_2x5_1 += pow3[9];
        b.index_p.corner_2x5_5 += pow3[9];
    }
    if (diff_p & INDEX_A2) {
        b.index_p.diagonal7_2 += pow3[6];
        b.index_p.edge_2x_3 += pow3[3];
        b.index_p.h_v_2_0 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[5];
        b.index_p.edge_x_side_0 += pow3[4];
        b.index_p.triangle_0 += pow3[5];
        b.index_p.corner_2x5_0 += pow3[4];
        b.index_p.corner_2x5_4 += pow3[8];
    }
    if (diff_p & INDEX_B2) {
        b.index_p.diagonal8_0 += pow3[6];
        b.index_p.edge_2x_0 += pow3[1];
        b.index_p.edge_2x_3 += pow3[0];
        b.index_p.h_v_2_0 += pow3[6];
        b.index_p.h_v_2_3 += pow3[6];
        b.index_p.corner_3x3_0 += pow3[4];
        b.index_p.edge_x_side_0 += pow3[3];
        b.index_p.triangle_0 += pow3[4];
        b.index_p.corner_2x5_0 += pow3[3];
        b.index_p.corner_2x5_4 += pow3[3];
    }
    if (diff_p & INDEX_C2) {
        b.index_p.diagonal7_0 += pow3[5];
        b.index_p.h_v_2_0 += pow3[5];
        b.index_p.h_v_3_3 += pow3[6];
        b.index_p.corner_3x3_0 += pow3[3];
        b.index_p.edge_block_0 += pow3[3];
        b.index_p.triangle_0 += pow3[3];
        b.index_p.corner_2x5_0 += pow3[2];
    }
    if (diff_p & INDEX_D2) {
        b.index_p.diagonal6_0 += pow3[4];
        b.index_p.diagonal5_3 += pow3[1];
        b.index_p.h_v_2_0 += pow3[4];
        b.index_p.h_v_4_3 += pow3[6];
        b.index_p.edge_block_0 += pow3[2];
        b.index_p.corner_2x5_0 += pow3[1];
        b.index_p.corner_2x5_5 += pow3[0];
    }
    if (diff_p & INDEX_E2) {
        b.index_p.diagonal6_3 += pow3[1];
        b.index_p.diagonal5_0 += pow3[3];
        b.index_p.h_v_2_0 += pow3[3];
        b.index_p.h_v_4_1 += pow3[6];
        b.index_p.edge_block_0 += pow3[1];
        b.index_p.corner_2x5_0 += pow3[0];
        b.index_p.corner_2x5_5 += pow3[1];
    }
    if (diff_p & INDEX_F2) {
        b.index_p.diagonal7_3 += pow3[1];
        b.index_p.h_v_2_0 += pow3[2];
        b.index_p.h_v_3_1 += pow3[6];
        b.index_p.corner_3x3_1 += pow3[1];
        b.index_p.edge_block_0 += pow3[0];
        b.index_p.triangle_1 += pow3[1];
        b.index_p.corner_2x5_5 += pow3[2];
    }
    if (diff_p & INDEX_G2) {
        b.index_p.diagonal8_1 += pow3[6];
        b.index_p.edge_2x_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[1];
        b.index_p.h_v_2_0 += pow3[1];
        b.index_p.h_v_2_1 += pow3[6];
        b.index_p.corner_3x3_1 += pow3[4];
        b.index_p.edge_x_side_1 += pow3[3];
        b.index_p.triangle_1 += pow3[4];
        b.index_p.corner_2x5_1 += pow3[3];
        b.index_p.corner_2x5_5 += pow3[3];
    }
    if (diff_p & INDEX_H2) {
        b.index_p.diagonal7_1 += pow3[6];
        b.index_p.edge_2x_1 += pow3[8];
        b.index_p.h_v_2_0 += pow3[0];
        b.index_p.corner_3x3_1 += pow3[7];
        b.index_p.edge_x_side_1 += pow3[8];
        b.index_p.triangle_1 += pow3[8];
        b.index_p.corner_2x5_1 += pow3[8];
        b.index_p.corner_2x5_5 += pow3[4];
    }
    if (diff_p & INDEX_A3) {
        b.index_p.diagonal6_2 += pow3[5];
        b.index_p.edge_2x_3 += pow3[4];
        b.index_p.h_v_3_0 += pow3[7];
        b.index_p.corner_3x3_0 += pow3[2];
        b.index_p.edge_x_side_0 += pow3[2];
        b.index_p.edge_block_3 += pow3[5];
        b.index_p.triangle_0 += pow3[2];
        b.index_p.corner_2x5_4 += pow3[7];
    }
    if (diff_p & INDEX_B3) {
        b.index_p.diagonal7_2 += pow3[5];
        b.index_p.h_v_2_3 += pow3[5];
        b.index_p.h_v_3_0 += pow3[6];
        b.index_p.corner_3x3_0 += pow3[1];
        b.index_p.edge_block_3 += pow3[0];
        b.index_p.triangle_0 += pow3[1];
        b.index_p.corner_2x5_4 += pow3[2];
    }
    if (diff_p & INDEX_C3) {
        b.index_p.diagonal8_0 += pow3[5];
        b.index_p.diagonal5_3 += pow3[2];
        b.index_p.h_v_3_0 += pow3[5];
        b.index_p.h_v_3_3 += pow3[5];
        b.index_p.corner_3x3_0 += pow3[0];
    }
    if (diff_p & INDEX_D3) {
        b.index_p.diagonal7_0 += pow3[4];
        b.index_p.diagonal6_3 += pow3[2];
        b.index_p.h_v_3_0 += pow3[4];
        b.index_p.h_v_4_3 += pow3[5];
    }
    if (diff_p & INDEX_E3) {
        b.index_p.diagonal7_3 += pow3[2];
        b.index_p.diagonal6_0 += pow3[3];
        b.index_p.h_v_3_0 += pow3[3];
        b.index_p.h_v_4_1 += pow3[5];
    }
    if (diff_p & INDEX_F3) {
        b.index_p.diagonal8_1 += pow3[5];
        b.index_p.diagonal5_0 += pow3[2];
        b.index_p.h_v_3_0 += pow3[2];
        b.index_p.h_v_3_1 += pow3[5];
        b.index_p.corner_3x3_1 += pow3[0];
    }
    if (diff_p & INDEX_G3) {
        b.index_p.diagonal7_1 += pow3[5];
        b.index_p.h_v_2_1 += pow3[5];
        b.index_p.h_v_3_0 += pow3[1];
        b.index_p.corner_3x3_1 += pow3[3];
        b.index_p.edge_block_1 += pow3[3];
        b.index_p.triangle_1 += pow3[3];
        b.index_p.corner_2x5_1 += pow3[2];
    }
    if (diff_p & INDEX_H3) {
        b.index_p.diagonal6_1 += pow3[0];
        b.index_p.edge_2x_1 += pow3[7];
        b.index_p.h_v_3_0 += pow3[0];
        b.index_p.corner_3x3_1 += pow3[6];
        b.index_p.edge_x_side_1 += pow3[7];
        b.index_p.edge_block_1 += pow3[8];
        b.index_p.triangle_1 += pow3[7];
        b.index_p.corner_2x5_1 += pow3[7];
    }
    if (diff_p & INDEX_A4) {
        b.index_p.diagonal5_2 += pow3[4];
        b.index_p.edge_2x_3 += pow3[5];
        b.index_p.h_v_4_0 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[1];
        b.index_p.edge_x_side_3 += pow3[5];
        b.index_p.edge_block_3 += pow3[6];
        b.index_p.triangle_0 += pow3[0];
        b.index_p.corner_2x5_3 += pow3[5];
        b.index_p.corner_2x5_4 += pow3[6];
    }
    if (diff_p & INDEX_B4) {
        b.index_p.diagonal6_2 += pow3[4];
        b.index_p.diagonal5_3 += pow3[3];
        b.index_p.h_v_2_3 += pow3[4];
        b.index_p.h_v_4_0 += pow3[6];
        b.index_p.edge_block_3 += pow3[1];
        b.index_p.corner_2x5_3 += pow3[0];
        b.index_p.corner_2x5_4 += pow3[1];
    }
    if (diff_p & INDEX_C4) {
        b.index_p.diagonal7_2 += pow3[4];
        b.index_p.diagonal6_3 += pow3[3];
        b.index_p.h_v_3_3 += pow3[4];
        b.index_p.h_v_4_0 += pow3[5];
    }
    if (diff_p & INDEX_D4) {
        b.index_p.diagonal8_0 += pow3[4];
        b.index_p.diagonal7_3 += pow3[3];
        b.index_p.h_v_4_0 += pow3[4];
        b.index_p.h_v_4_3 += pow3[4];
    }
    if (diff_p & INDEX_E4) {
        b.index_p.diagonal8_1 += pow3[4];
        b.index_p.diagonal7_0 += pow3[3];
        b.index_p.h_v_4_0 += pow3[3];
        b.index_p.h_v_4_1 += pow3[4];
    }
    if (diff_p & INDEX_F4) {
        b.index_p.diagonal7_1 += pow3[4];
        b.index_p.diagonal6_0 += pow3[2];
        b.index_p.h_v_3_1 += pow3[4];
        b.index_p.h_v_4_0 += pow3[2];
    }
    if (diff_p & INDEX_G4) {
        b.index_p.diagonal6_1 += pow3[1];
        b.index_p.diagonal5_0 += pow3[1];
        b.index_p.h_v_2_1 += pow3[4];
        b.index_p.h_v_4_0 += pow3[1];
        b.index_p.edge_block_1 += pow3[2];
        b.index_p.corner_2x5_1 += pow3[1];
        b.index_p.corner_2x5_6 += pow3[0];
    }
    if (diff_p & INDEX_H4) {
        b.index_p.diagonal5_1 += pow3[0];
        b.index_p.edge_2x_1 += pow3[6];
        b.index_p.h_v_4_0 += pow3[0];
        b.index_p.edge_x_side_1 += pow3[6];
        b.index_p.edge_x_side_2 += pow3[0];
        b.index_p.edge_block_1 += pow3[7];
        b.index_p.triangle_1 += pow3[6];
        b.index_p.corner_2x5_1 += pow3[6];
        b.index_p.corner_2x5_6 += pow3[5];
    }
    if (diff_p & INDEX_A5) {
        b.index_p.diagonal5_3 += pow3[4];
        b.index_p.edge_2x_3 += pow3[6];
        b.index_p.h_v_4_2 += pow3[7];
        b.index_p.edge_x_side_0 += pow3[0];
        b.index_p.edge_x_side_3 += pow3[6];
        b.index_p.edge_block_3 += pow3[7];
        b.index_p.triangle_3 += pow3[6];
        b.index_p.corner_2x5_3 += pow3[6];
        b.index_p.corner_2x5_4 += pow3[5];
    }
    if (diff_p & INDEX_B5) {
        b.index_p.diagonal6_3 += pow3[4];
        b.index_p.diagonal5_2 += pow3[3];
        b.index_p.h_v_2_3 += pow3[3];
        b.index_p.h_v_4_2 += pow3[6];
        b.index_p.edge_block_3 += pow3[2];
        b.index_p.corner_2x5_3 += pow3[1];
        b.index_p.corner_2x5_4 += pow3[0];
    }
    if (diff_p & INDEX_C5) {
        b.index_p.diagonal7_3 += pow3[4];
        b.index_p.diagonal6_2 += pow3[3];
        b.index_p.h_v_3_3 += pow3[3];
        b.index_p.h_v_4_2 += pow3[5];
    }
    if (diff_p & INDEX_D5) {
        b.index_p.diagonal8_1 += pow3[3];
        b.index_p.diagonal7_2 += pow3[3];
        b.index_p.h_v_4_2 += pow3[4];
        b.index_p.h_v_4_3 += pow3[3];
    }
    if (diff_p & INDEX_E5) {
        b.index_p.diagonal8_0 += pow3[3];
        b.index_p.diagonal7_1 += pow3[3];
        b.index_p.h_v_4_1 += pow3[3];
        b.index_p.h_v_4_2 += pow3[3];
    }
    if (diff_p & INDEX_F5) {
        b.index_p.diagonal7_0 += pow3[2];
        b.index_p.diagonal6_1 += pow3[2];
        b.index_p.h_v_3_1 += pow3[3];
        b.index_p.h_v_4_2 += pow3[2];
    }
    if (diff_p & INDEX_G5) {
        b.index_p.diagonal6_0 += pow3[1];
        b.index_p.diagonal5_1 += pow3[1];
        b.index_p.h_v_2_1 += pow3[3];
        b.index_p.h_v_4_2 += pow3[1];
        b.index_p.edge_block_1 += pow3[1];
        b.index_p.corner_2x5_1 += pow3[0];
        b.index_p.corner_2x5_6 += pow3[1];
    }
    if (diff_p & INDEX_H5) {
        b.index_p.diagonal5_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[5];
        b.index_p.h_v_4_2 += pow3[0];
        b.index_p.edge_x_side_1 += pow3[5];
        b.index_p.edge_x_side_2 += pow3[1];
        b.index_p.edge_block_1 += pow3[6];
        b.index_p.triangle_2 += pow3[0];
        b.index_p.corner_2x5_1 += pow3[5];
        b.index_p.corner_2x5_6 += pow3[6];
    }
    if (diff_p & INDEX_A6) {
        b.index_p.diagonal6_3 += pow3[5];
        b.index_p.edge_2x_3 += pow3[7];
        b.index_p.h_v_3_2 += pow3[7];
        b.index_p.corner_3x3_3 += pow3[6];
        b.index_p.edge_x_side_3 += pow3[7];
        b.index_p.edge_block_3 += pow3[8];
        b.index_p.triangle_3 += pow3[7];
        b.index_p.corner_2x5_3 += pow3[7];
    }
    if (diff_p & INDEX_B6) {
        b.index_p.diagonal7_3 += pow3[5];
        b.index_p.h_v_2_3 += pow3[2];
        b.index_p.h_v_3_2 += pow3[6];
        b.index_p.corner_3x3_3 += pow3[3];
        b.index_p.edge_block_3 += pow3[3];
        b.index_p.triangle_3 += pow3[3];
        b.index_p.corner_2x5_3 += pow3[2];
    }
    if (diff_p & INDEX_C6) {
        b.index_p.diagonal8_1 += pow3[2];
        b.index_p.diagonal5_2 += pow3[2];
        b.index_p.h_v_3_2 += pow3[5];
        b.index_p.h_v_3_3 += pow3[2];
        b.index_p.corner_3x3_3 += pow3[0];
    }
    if (diff_p & INDEX_D6) {
        b.index_p.diagonal7_1 += pow3[2];
        b.index_p.diagonal6_2 += pow3[2];
        b.index_p.h_v_3_2 += pow3[4];
        b.index_p.h_v_4_3 += pow3[2];
    }
    if (diff_p & INDEX_E6) {
        b.index_p.diagonal7_2 += pow3[2];
        b.index_p.diagonal6_1 += pow3[3];
        b.index_p.h_v_3_2 += pow3[3];
        b.index_p.h_v_4_1 += pow3[2];
    }
    if (diff_p & INDEX_F6) {
        b.index_p.diagonal8_0 += pow3[2];
        b.index_p.diagonal5_1 += pow3[2];
        b.index_p.h_v_3_1 += pow3[2];
        b.index_p.h_v_3_2 += pow3[2];
        b.index_p.corner_3x3_2 += pow3[0];
    }
    if (diff_p & INDEX_G6) {
        b.index_p.diagonal7_0 += pow3[1];
        b.index_p.h_v_2_1 += pow3[2];
        b.index_p.h_v_3_2 += pow3[1];
        b.index_p.corner_3x3_2 += pow3[1];
        b.index_p.edge_block_1 += pow3[0];
        b.index_p.triangle_2 += pow3[1];
        b.index_p.corner_2x5_6 += pow3[2];
    }
    if (diff_p & INDEX_H6) {
        b.index_p.diagonal6_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[4];
        b.index_p.h_v_3_2 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[2];
        b.index_p.edge_x_side_2 += pow3[2];
        b.index_p.edge_block_1 += pow3[5];
        b.index_p.triangle_2 += pow3[2];
        b.index_p.corner_2x5_6 += pow3[7];
    }
    if (diff_p & INDEX_A7) {
        b.index_p.diagonal7_3 += pow3[6];
        b.index_p.edge_2x_3 += pow3[8];
        b.index_p.h_v_2_2 += pow3[7];
        b.index_p.corner_3x3_3 += pow3[7];
        b.index_p.edge_x_side_3 += pow3[8];
        b.index_p.triangle_3 += pow3[8];
        b.index_p.corner_2x5_3 += pow3[8];
        b.index_p.corner_2x5_7 += pow3[4];
    }
    if (diff_p & INDEX_B7) {
        b.index_p.diagonal8_1 += pow3[1];
        b.index_p.edge_2x_2 += pow3[0];
        b.index_p.edge_2x_3 += pow3[1];
        b.index_p.h_v_2_2 += pow3[6];
        b.index_p.h_v_2_3 += pow3[1];
        b.index_p.corner_3x3_3 += pow3[4];
        b.index_p.edge_x_side_3 += pow3[3];
        b.index_p.triangle_3 += pow3[4];
        b.index_p.corner_2x5_3 += pow3[3];
        b.index_p.corner_2x5_7 += pow3[3];
    }
    if (diff_p & INDEX_C7) {
        b.index_p.diagonal7_1 += pow3[1];
        b.index_p.h_v_2_2 += pow3[5];
        b.index_p.h_v_3_3 += pow3[1];
        b.index_p.corner_3x3_3 += pow3[1];
        b.index_p.edge_block_2 += pow3[0];
        b.index_p.triangle_3 += pow3[1];
        b.index_p.corner_2x5_7 += pow3[2];
    }
    if (diff_p & INDEX_D7) {
        b.index_p.diagonal6_1 += pow3[4];
        b.index_p.diagonal5_2 += pow3[1];
        b.index_p.h_v_2_2 += pow3[4];
        b.index_p.h_v_4_3 += pow3[1];
        b.index_p.edge_block_2 += pow3[1];
        b.index_p.corner_2x5_2 += pow3[0];
        b.index_p.corner_2x5_7 += pow3[1];
    }
    if (diff_p & INDEX_E7) {
        b.index_p.diagonal6_2 += pow3[1];
        b.index_p.diagonal5_1 += pow3[3];
        b.index_p.h_v_2_2 += pow3[3];
        b.index_p.h_v_4_1 += pow3[1];
        b.index_p.edge_block_2 += pow3[2];
        b.index_p.corner_2x5_2 += pow3[1];
        b.index_p.corner_2x5_7 += pow3[0];
    }
    if (diff_p & INDEX_F7) {
        b.index_p.diagonal7_2 += pow3[1];
        b.index_p.h_v_2_2 += pow3[2];
        b.index_p.h_v_3_1 += pow3[1];
        b.index_p.corner_3x3_2 += pow3[3];
        b.index_p.edge_block_2 += pow3[3];
        b.index_p.triangle_2 += pow3[3];
        b.index_p.corner_2x5_2 += pow3[2];
    }
    if (diff_p & INDEX_G7) {
        b.index_p.diagonal8_0 += pow3[1];
        b.index_p.edge_2x_1 += pow3[0];
        b.index_p.edge_2x_2 += pow3[1];
        b.index_p.h_v_2_1 += pow3[1];
        b.index_p.h_v_2_2 += pow3[1];
        b.index_p.corner_3x3_2 += pow3[4];
        b.index_p.edge_x_side_2 += pow3[3];
        b.index_p.triangle_2 += pow3[4];
        b.index_p.corner_2x5_2 += pow3[3];
        b.index_p.corner_2x5_6 += pow3[3];
    }
    if (diff_p & INDEX_H7) {
        b.index_p.diagonal7_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[3];
        b.index_p.h_v_2_2 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[5];
        b.index_p.edge_x_side_2 += pow3[4];
        b.index_p.triangle_2 += pow3[5];
        b.index_p.corner_2x5_2 += pow3[4];
        b.index_p.corner_2x5_6 += pow3[8];
    }
    if (diff_p & INDEX_A8) {
        b.index_p.diagonal8_1 += pow3[0];
        b.index_p.edge_2x_2 += pow3[2];
        b.index_p.edge_2x_3 += pow3[9];
        b.index_p.corner_3x3_3 += pow3[8];
        b.index_p.edge_x_side_3 += pow3[9];
        b.index_p.edge_block_2 += pow3[4];
        b.index_p.edge_block_3 += pow3[9];
        b.index_p.triangle_3 += pow3[9];
        b.index_p.corner_2x5_3 += pow3[9];
        b.index_p.corner_2x5_7 += pow3[9];
    }
    if (diff_p & INDEX_B8) {
        b.index_p.diagonal7_1 += pow3[0];
        b.index_p.edge_2x_2 += pow3[3];
        b.index_p.h_v_2_3 += pow3[0];
        b.index_p.corner_3x3_3 += pow3[5];
        b.index_p.edge_x_side_3 += pow3[4];
        b.index_p.triangle_3 += pow3[5];
        b.index_p.corner_2x5_3 += pow3[4];
        b.index_p.corner_2x5_7 += pow3[8];
    }
    if (diff_p & INDEX_C8) {
        b.index_p.diagonal6_1 += pow3[5];
        b.index_p.edge_2x_2 += pow3[4];
        b.index_p.h_v_3_3 += pow3[0];
        b.index_p.corner_3x3_3 += pow3[2];
        b.index_p.edge_x_side_3 += pow3[2];
        b.index_p.edge_block_2 += pow3[5];
        b.index_p.triangle_3 += pow3[2];
        b.index_p.corner_2x5_7 += pow3[7];
    }
    if (diff_p & INDEX_D8) {
        b.index_p.diagonal5_1 += pow3[4];
        b.index_p.edge_2x_2 += pow3[5];
        b.index_p.h_v_4_3 += pow3[0];
        b.index_p.edge_x_side_2 += pow3[5];
        b.index_p.edge_x_side_3 += pow3[1];
        b.index_p.edge_block_2 += pow3[6];
        b.index_p.triangle_3 += pow3[0];
        b.index_p.corner_2x5_2 += pow3[5];
        b.index_p.corner_2x5_7 += pow3[6];
    }
    if (diff_p & INDEX_E8) {
        b.index_p.diagonal5_2 += pow3[0];
        b.index_p.edge_2x_2 += pow3[6];
        b.index_p.h_v_4_1 += pow3[0];
        b.index_p.edge_x_side_2 += pow3[6];
        b.index_p.edge_x_side_3 += pow3[0];
        b.index_p.edge_block_2 += pow3[7];
        b.index_p.triangle_2 += pow3[6];
        b.index_p.corner_2x5_2 += pow3[6];
        b.index_p.corner_2x5_7 += pow3[5];
    }
    if (diff_p & INDEX_F8) {
        b.index_p.diagonal6_2 += pow3[0];
        b.index_p.edge_2x_2 += pow3[7];
        b.index_p.h_v_3_1 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[6];
        b.index_p.edge_x_side_2 += pow3[7];
        b.index_p.edge_block_2 += pow3[8];
        b.index_p.triangle_2 += pow3[7];
        b.index_p.corner_2x5_2 += pow3[7];
    }
    if (diff_p & INDEX_G8) {
        b.index_p.diagonal7_2 += pow3[0];
        b.index_p.edge_2x_2 += pow3[8];
        b.index_p.h_v_2_1 += pow3[0];
        b.index_p.corner_3x3_2 += pow3[7];
        b.index_p.edge_x_side_2 += pow3[8];
        b.index_p.triangle_2 += pow3[8];
        b.index_p.corner_2x5_2 += pow3[8];
        b.index_p.corner_2x5_6 += pow3[4];
    }
    if (diff_p & INDEX_H8) {
        b.index_p.diagonal8_0 += pow3[0];
        b.index_p.edge_2x_1 += pow3[2];
        b.index_p.edge_2x_2 += pow3[9];
        b.index_p.corner_3x3_2 += pow3[8];
        b.index_p.edge_x_side_2 += pow3[9];
        b.index_p.edge_block_1 += pow3[4];
        b.index_p.edge_block_2 += pow3[9];
        b.index_p.triangle_2 += pow3[9];
        b.index_p.corner_2x5_2 += pow3[9];
        b.index_p.corner_2x5_6 += pow3[9];
    }
    
    
    if (diff_o & INDEX_A1) {
        b.index_o.diagonal8_0 += pow3[7];
        b.index_o.edge_2x_0 += pow3[9];
        b.index_o.edge_2x_3 += pow3[2];
        b.index_o.corner_3x3_0 += pow3[8];
        b.index_o.edge_x_side_0 += pow3[9];
        b.index_o.edge_block_0 += pow3[9];
        b.index_o.edge_block_3 += pow3[4];
        b.index_o.triangle_0 += pow3[9];
        b.index_o.corner_2x5_0 += pow3[9];
        b.index_o.corner_2x5_4 += pow3[9];
    }
    if (diff_o & INDEX_B1) {
        b.index_o.diagonal7_0 += pow3[6];
        b.index_o.edge_2x_0 += pow3[8];
        b.index_o.h_v_2_3 += pow3[7];
        b.index_o.corner_3x3_0 += pow3[7];
        b.index_o.edge_x_side_0 += pow3[8];
        b.index_o.triangle_0 += pow3[8];
        b.index_o.corner_2x5_0 += pow3[8];
        b.index_o.corner_2x5_4 += pow3[4];
    }
    if (diff_o & INDEX_C1) {
        b.index_o.diagonal6_0 += pow3[5];
        b.index_o.edge_2x_0 += pow3[7];
        b.index_o.h_v_3_3 += pow3[7];
        b.index_o.corner_3x3_0 += pow3[6];
        b.index_o.edge_x_side_0 += pow3[7];
        b.index_o.edge_block_0 += pow3[8];
        b.index_o.triangle_0 += pow3[7];
        b.index_o.corner_2x5_0 += pow3[7];
    }
    if (diff_o & INDEX_D1) {
        b.index_o.diagonal5_0 += pow3[4];
        b.index_o.edge_2x_0 += pow3[6];
        b.index_o.h_v_4_3 += pow3[7];
        b.index_o.edge_x_side_0 += pow3[6];
        b.index_o.edge_x_side_1 += pow3[0];
        b.index_o.edge_block_0 += pow3[7];
        b.index_o.triangle_0 += pow3[6];
        b.index_o.corner_2x5_0 += pow3[6];
        b.index_o.corner_2x5_5 += pow3[5];
    }
    if (diff_o & INDEX_E1) {
        b.index_o.diagonal5_3 += pow3[0];
        b.index_o.edge_2x_0 += pow3[5];
        b.index_o.h_v_4_1 += pow3[7];
        b.index_o.edge_x_side_0 += pow3[5];
        b.index_o.edge_x_side_1 += pow3[1];
        b.index_o.edge_block_0 += pow3[6];
        b.index_o.triangle_1 += pow3[0];
        b.index_o.corner_2x5_0 += pow3[5];
        b.index_o.corner_2x5_5 += pow3[6];
    }
    if (diff_o & INDEX_F1) {
        b.index_o.diagonal6_3 += pow3[0];
        b.index_o.edge_2x_0 += pow3[4];
        b.index_o.h_v_3_1 += pow3[7];
        b.index_o.corner_3x3_1 += pow3[2];
        b.index_o.edge_x_side_1 += pow3[2];
        b.index_o.edge_block_0 += pow3[5];
        b.index_o.triangle_1 += pow3[2];
        b.index_o.corner_2x5_5 += pow3[7];
    }
    if (diff_o & INDEX_G1) {
        b.index_o.diagonal7_3 += pow3[0];
        b.index_o.edge_2x_0 += pow3[3];
        b.index_o.h_v_2_1 += pow3[7];
        b.index_o.corner_3x3_1 += pow3[5];
        b.index_o.edge_x_side_1 += pow3[4];
        b.index_o.triangle_1 += pow3[5];
        b.index_o.corner_2x5_1 += pow3[4];
        b.index_o.corner_2x5_5 += pow3[8];
    }
    if (diff_o & INDEX_H1) {
        b.index_o.diagonal8_1 += pow3[7];
        b.index_o.edge_2x_0 += pow3[2];
        b.index_o.edge_2x_1 += pow3[9];
        b.index_o.corner_3x3_1 += pow3[8];
        b.index_o.edge_x_side_1 += pow3[9];
        b.index_o.edge_block_0 += pow3[4];
        b.index_o.edge_block_1 += pow3[9];
        b.index_o.triangle_1 += pow3[9];
        b.index_o.corner_2x5_1 += pow3[9];
        b.index_o.corner_2x5_5 += pow3[9];
    }
    if (diff_o & INDEX_A2) {
        b.index_o.diagonal7_2 += pow3[6];
        b.index_o.edge_2x_3 += pow3[3];
        b.index_o.h_v_2_0 += pow3[7];
        b.index_o.corner_3x3_0 += pow3[5];
        b.index_o.edge_x_side_0 += pow3[4];
        b.index_o.triangle_0 += pow3[5];
        b.index_o.corner_2x5_0 += pow3[4];
        b.index_o.corner_2x5_4 += pow3[8];
    }
    if (diff_o & INDEX_B2) {
        b.index_o.diagonal8_0 += pow3[6];
        b.index_o.edge_2x_0 += pow3[1];
        b.index_o.edge_2x_3 += pow3[0];
        b.index_o.h_v_2_0 += pow3[6];
        b.index_o.h_v_2_3 += pow3[6];
        b.index_o.corner_3x3_0 += pow3[4];
        b.index_o.edge_x_side_0 += pow3[3];
        b.index_o.triangle_0 += pow3[4];
        b.index_o.corner_2x5_0 += pow3[3];
        b.index_o.corner_2x5_4 += pow3[3];
    }
    if (diff_o & INDEX_C2) {
        b.index_o.diagonal7_0 += pow3[5];
        b.index_o.h_v_2_0 += pow3[5];
        b.index_o.h_v_3_3 += pow3[6];
        b.index_o.corner_3x3_0 += pow3[3];
        b.index_o.edge_block_0 += pow3[3];
        b.index_o.triangle_0 += pow3[3];
        b.index_o.corner_2x5_0 += pow3[2];
    }
    if (diff_o & INDEX_D2) {
        b.index_o.diagonal6_0 += pow3[4];
        b.index_o.diagonal5_3 += pow3[1];
        b.index_o.h_v_2_0 += pow3[4];
        b.index_o.h_v_4_3 += pow3[6];
        b.index_o.edge_block_0 += pow3[2];
        b.index_o.corner_2x5_0 += pow3[1];
        b.index_o.corner_2x5_5 += pow3[0];
    }
    if (diff_o & INDEX_E2) {
        b.index_o.diagonal6_3 += pow3[1];
        b.index_o.diagonal5_0 += pow3[3];
        b.index_o.h_v_2_0 += pow3[3];
        b.index_o.h_v_4_1 += pow3[6];
        b.index_o.edge_block_0 += pow3[1];
        b.index_o.corner_2x5_0 += pow3[0];
        b.index_o.corner_2x5_5 += pow3[1];
    }
    if (diff_o & INDEX_F2) {
        b.index_o.diagonal7_3 += pow3[1];
        b.index_o.h_v_2_0 += pow3[2];
        b.index_o.h_v_3_1 += pow3[6];
        b.index_o.corner_3x3_1 += pow3[1];
        b.index_o.edge_block_0 += pow3[0];
        b.index_o.triangle_1 += pow3[1];
        b.index_o.corner_2x5_5 += pow3[2];
    }
    if (diff_o & INDEX_G2) {
        b.index_o.diagonal8_1 += pow3[6];
        b.index_o.edge_2x_0 += pow3[0];
        b.index_o.edge_2x_1 += pow3[1];
        b.index_o.h_v_2_0 += pow3[1];
        b.index_o.h_v_2_1 += pow3[6];
        b.index_o.corner_3x3_1 += pow3[4];
        b.index_o.edge_x_side_1 += pow3[3];
        b.index_o.triangle_1 += pow3[4];
        b.index_o.corner_2x5_1 += pow3[3];
        b.index_o.corner_2x5_5 += pow3[3];
    }
    if (diff_o & INDEX_H2) {
        b.index_o.diagonal7_1 += pow3[6];
        b.index_o.edge_2x_1 += pow3[8];
        b.index_o.h_v_2_0 += pow3[0];
        b.index_o.corner_3x3_1 += pow3[7];
        b.index_o.edge_x_side_1 += pow3[8];
        b.index_o.triangle_1 += pow3[8];
        b.index_o.corner_2x5_1 += pow3[8];
        b.index_o.corner_2x5_5 += pow3[4];
    }
    if (diff_o & INDEX_A3) {
        b.index_o.diagonal6_2 += pow3[5];
        b.index_o.edge_2x_3 += pow3[4];
        b.index_o.h_v_3_0 += pow3[7];
        b.index_o.corner_3x3_0 += pow3[2];
        b.index_o.edge_x_side_0 += pow3[2];
        b.index_o.edge_block_3 += pow3[5];
        b.index_o.triangle_0 += pow3[2];
        b.index_o.corner_2x5_4 += pow3[7];
    }
    if (diff_o & INDEX_B3) {
        b.index_o.diagonal7_2 += pow3[5];
        b.index_o.h_v_2_3 += pow3[5];
        b.index_o.h_v_3_0 += pow3[6];
        b.index_o.corner_3x3_0 += pow3[1];
        b.index_o.edge_block_3 += pow3[0];
        b.index_o.triangle_0 += pow3[1];
        b.index_o.corner_2x5_4 += pow3[2];
    }
    if (diff_o & INDEX_C3) {
        b.index_o.diagonal8_0 += pow3[5];
        b.index_o.diagonal5_3 += pow3[2];
        b.index_o.h_v_3_0 += pow3[5];
        b.index_o.h_v_3_3 += pow3[5];
        b.index_o.corner_3x3_0 += pow3[0];
    }
    if (diff_o & INDEX_D3) {
        b.index_o.diagonal7_0 += pow3[4];
        b.index_o.diagonal6_3 += pow3[2];
        b.index_o.h_v_3_0 += pow3[4];
        b.index_o.h_v_4_3 += pow3[5];
    }
    if (diff_o & INDEX_E3) {
        b.index_o.diagonal7_3 += pow3[2];
        b.index_o.diagonal6_0 += pow3[3];
        b.index_o.h_v_3_0 += pow3[3];
        b.index_o.h_v_4_1 += pow3[5];
    }
    if (diff_o & INDEX_F3) {
        b.index_o.diagonal8_1 += pow3[5];
        b.index_o.diagonal5_0 += pow3[2];
        b.index_o.h_v_3_0 += pow3[2];
        b.index_o.h_v_3_1 += pow3[5];
        b.index_o.corner_3x3_1 += pow3[0];
    }
    if (diff_o & INDEX_G3) {
        b.index_o.diagonal7_1 += pow3[5];
        b.index_o.h_v_2_1 += pow3[5];
        b.index_o.h_v_3_0 += pow3[1];
        b.index_o.corner_3x3_1 += pow3[3];
        b.index_o.edge_block_1 += pow3[3];
        b.index_o.triangle_1 += pow3[3];
        b.index_o.corner_2x5_1 += pow3[2];
    }
    if (diff_o & INDEX_H3) {
        b.index_o.diagonal6_1 += pow3[0];
        b.index_o.edge_2x_1 += pow3[7];
        b.index_o.h_v_3_0 += pow3[0];
        b.index_o.corner_3x3_1 += pow3[6];
        b.index_o.edge_x_side_1 += pow3[7];
        b.index_o.edge_block_1 += pow3[8];
        b.index_o.triangle_1 += pow3[7];
        b.index_o.corner_2x5_1 += pow3[7];
    }
    if (diff_o & INDEX_A4) {
        b.index_o.diagonal5_2 += pow3[4];
        b.index_o.edge_2x_3 += pow3[5];
        b.index_o.h_v_4_0 += pow3[7];
        b.index_o.edge_x_side_0 += pow3[1];
        b.index_o.edge_x_side_3 += pow3[5];
        b.index_o.edge_block_3 += pow3[6];
        b.index_o.triangle_0 += pow3[0];
        b.index_o.corner_2x5_3 += pow3[5];
        b.index_o.corner_2x5_4 += pow3[6];
    }
    if (diff_o & INDEX_B4) {
        b.index_o.diagonal6_2 += pow3[4];
        b.index_o.diagonal5_3 += pow3[3];
        b.index_o.h_v_2_3 += pow3[4];
        b.index_o.h_v_4_0 += pow3[6];
        b.index_o.edge_block_3 += pow3[1];
        b.index_o.corner_2x5_3 += pow3[0];
        b.index_o.corner_2x5_4 += pow3[1];
    }
    if (diff_o & INDEX_C4) {
        b.index_o.diagonal7_2 += pow3[4];
        b.index_o.diagonal6_3 += pow3[3];
        b.index_o.h_v_3_3 += pow3[4];
        b.index_o.h_v_4_0 += pow3[5];
    }
    if (diff_o & INDEX_D4) {
        b.index_o.diagonal8_0 += pow3[4];
        b.index_o.diagonal7_3 += pow3[3];
        b.index_o.h_v_4_0 += pow3[4];
        b.index_o.h_v_4_3 += pow3[4];
    }
    if (diff_o & INDEX_E4) {
        b.index_o.diagonal8_1 += pow3[4];
        b.index_o.diagonal7_0 += pow3[3];
        b.index_o.h_v_4_0 += pow3[3];
        b.index_o.h_v_4_1 += pow3[4];
    }
    if (diff_o & INDEX_F4) {
        b.index_o.diagonal7_1 += pow3[4];
        b.index_o.diagonal6_0 += pow3[2];
        b.index_o.h_v_3_1 += pow3[4];
        b.index_o.h_v_4_0 += pow3[2];
    }
    if (diff_o & INDEX_G4) {
        b.index_o.diagonal6_1 += pow3[1];
        b.index_o.diagonal5_0 += pow3[1];
        b.index_o.h_v_2_1 += pow3[4];
        b.index_o.h_v_4_0 += pow3[1];
        b.index_o.edge_block_1 += pow3[2];
        b.index_o.corner_2x5_1 += pow3[1];
        b.index_o.corner_2x5_6 += pow3[0];
    }
    if (diff_o & INDEX_H4) {
        b.index_o.diagonal5_1 += pow3[0];
        b.index_o.edge_2x_1 += pow3[6];
        b.index_o.h_v_4_0 += pow3[0];
        b.index_o.edge_x_side_1 += pow3[6];
        b.index_o.edge_x_side_2 += pow3[0];
        b.index_o.edge_block_1 += pow3[7];
        b.index_o.triangle_1 += pow3[6];
        b.index_o.corner_2x5_1 += pow3[6];
        b.index_o.corner_2x5_6 += pow3[5];
    }
    if (diff_o & INDEX_A5) {
        b.index_o.diagonal5_3 += pow3[4];
        b.index_o.edge_2x_3 += pow3[6];
        b.index_o.h_v_4_2 += pow3[7];
        b.index_o.edge_x_side_0 += pow3[0];
        b.index_o.edge_x_side_3 += pow3[6];
        b.index_o.edge_block_3 += pow3[7];
        b.index_o.triangle_3 += pow3[6];
        b.index_o.corner_2x5_3 += pow3[6];
        b.index_o.corner_2x5_4 += pow3[5];
    }
    if (diff_o & INDEX_B5) {
        b.index_o.diagonal6_3 += pow3[4];
        b.index_o.diagonal5_2 += pow3[3];
        b.index_o.h_v_2_3 += pow3[3];
        b.index_o.h_v_4_2 += pow3[6];
        b.index_o.edge_block_3 += pow3[2];
        b.index_o.corner_2x5_3 += pow3[1];
        b.index_o.corner_2x5_4 += pow3[0];
    }
    if (diff_o & INDEX_C5) {
        b.index_o.diagonal7_3 += pow3[4];
        b.index_o.diagonal6_2 += pow3[3];
        b.index_o.h_v_3_3 += pow3[3];
        b.index_o.h_v_4_2 += pow3[5];
    }
    if (diff_o & INDEX_D5) {
        b.index_o.diagonal8_1 += pow3[3];
        b.index_o.diagonal7_2 += pow3[3];
        b.index_o.h_v_4_2 += pow3[4];
        b.index_o.h_v_4_3 += pow3[3];
    }
    if (diff_o & INDEX_E5) {
        b.index_o.diagonal8_0 += pow3[3];
        b.index_o.diagonal7_1 += pow3[3];
        b.index_o.h_v_4_1 += pow3[3];
        b.index_o.h_v_4_2 += pow3[3];
    }
    if (diff_o & INDEX_F5) {
        b.index_o.diagonal7_0 += pow3[2];
        b.index_o.diagonal6_1 += pow3[2];
        b.index_o.h_v_3_1 += pow3[3];
        b.index_o.h_v_4_2 += pow3[2];
    }
    if (diff_o & INDEX_G5) {
        b.index_o.diagonal6_0 += pow3[1];
        b.index_o.diagonal5_1 += pow3[1];
        b.index_o.h_v_2_1 += pow3[3];
        b.index_o.h_v_4_2 += pow3[1];
        b.index_o.edge_block_1 += pow3[1];
        b.index_o.corner_2x5_1 += pow3[0];
        b.index_o.corner_2x5_6 += pow3[1];
    }
    if (diff_o & INDEX_H5) {
        b.index_o.diagonal5_0 += pow3[0];
        b.index_o.edge_2x_1 += pow3[5];
        b.index_o.h_v_4_2 += pow3[0];
        b.index_o.edge_x_side_1 += pow3[5];
        b.index_o.edge_x_side_2 += pow3[1];
        b.index_o.edge_block_1 += pow3[6];
        b.index_o.triangle_2 += pow3[0];
        b.index_o.corner_2x5_1 += pow3[5];
        b.index_o.corner_2x5_6 += pow3[6];
    }
    if (diff_o & INDEX_A6) {
        b.index_o.diagonal6_3 += pow3[5];
        b.index_o.edge_2x_3 += pow3[7];
        b.index_o.h_v_3_2 += pow3[7];
        b.index_o.corner_3x3_3 += pow3[6];
        b.index_o.edge_x_side_3 += pow3[7];
        b.index_o.edge_block_3 += pow3[8];
        b.index_o.triangle_3 += pow3[7];
        b.index_o.corner_2x5_3 += pow3[7];
    }
    if (diff_o & INDEX_B6) {
        b.index_o.diagonal7_3 += pow3[5];
        b.index_o.h_v_2_3 += pow3[2];
        b.index_o.h_v_3_2 += pow3[6];
        b.index_o.corner_3x3_3 += pow3[3];
        b.index_o.edge_block_3 += pow3[3];
        b.index_o.triangle_3 += pow3[3];
        b.index_o.corner_2x5_3 += pow3[2];
    }
    if (diff_o & INDEX_C6) {
        b.index_o.diagonal8_1 += pow3[2];
        b.index_o.diagonal5_2 += pow3[2];
        b.index_o.h_v_3_2 += pow3[5];
        b.index_o.h_v_3_3 += pow3[2];
        b.index_o.corner_3x3_3 += pow3[0];
    }
    if (diff_o & INDEX_D6) {
        b.index_o.diagonal7_1 += pow3[2];
        b.index_o.diagonal6_2 += pow3[2];
        b.index_o.h_v_3_2 += pow3[4];
        b.index_o.h_v_4_3 += pow3[2];
    }
    if (diff_o & INDEX_E6) {
        b.index_o.diagonal7_2 += pow3[2];
        b.index_o.diagonal6_1 += pow3[3];
        b.index_o.h_v_3_2 += pow3[3];
        b.index_o.h_v_4_1 += pow3[2];
    }
    if (diff_o & INDEX_F6) {
        b.index_o.diagonal8_0 += pow3[2];
        b.index_o.diagonal5_1 += pow3[2];
        b.index_o.h_v_3_1 += pow3[2];
        b.index_o.h_v_3_2 += pow3[2];
        b.index_o.corner_3x3_2 += pow3[0];
    }
    if (diff_o & INDEX_G6) {
        b.index_o.diagonal7_0 += pow3[1];
        b.index_o.h_v_2_1 += pow3[2];
        b.index_o.h_v_3_2 += pow3[1];
        b.index_o.corner_3x3_2 += pow3[1];
        b.index_o.edge_block_1 += pow3[0];
        b.index_o.triangle_2 += pow3[1];
        b.index_o.corner_2x5_6 += pow3[2];
    }
    if (diff_o & INDEX_H6) {
        b.index_o.diagonal6_0 += pow3[0];
        b.index_o.edge_2x_1 += pow3[4];
        b.index_o.h_v_3_2 += pow3[0];
        b.index_o.corner_3x3_2 += pow3[2];
        b.index_o.edge_x_side_2 += pow3[2];
        b.index_o.edge_block_1 += pow3[5];
        b.index_o.triangle_2 += pow3[2];
        b.index_o.corner_2x5_6 += pow3[7];
    }
    if (diff_o & INDEX_A7) {
        b.index_o.diagonal7_3 += pow3[6];
        b.index_o.edge_2x_3 += pow3[8];
        b.index_o.h_v_2_2 += pow3[7];
        b.index_o.corner_3x3_3 += pow3[7];
        b.index_o.edge_x_side_3 += pow3[8];
        b.index_o.triangle_3 += pow3[8];
        b.index_o.corner_2x5_3 += pow3[8];
        b.index_o.corner_2x5_7 += pow3[4];
    }
    if (diff_o & INDEX_B7) {
        b.index_o.diagonal8_1 += pow3[1];
        b.index_o.edge_2x_2 += pow3[0];
        b.index_o.edge_2x_3 += pow3[1];
        b.index_o.h_v_2_2 += pow3[6];
        b.index_o.h_v_2_3 += pow3[1];
        b.index_o.corner_3x3_3 += pow3[4];
        b.index_o.edge_x_side_3 += pow3[3];
        b.index_o.triangle_3 += pow3[4];
        b.index_o.corner_2x5_3 += pow3[3];
        b.index_o.corner_2x5_7 += pow3[3];
    }
    if (diff_o & INDEX_C7) {
        b.index_o.diagonal7_1 += pow3[1];
        b.index_o.h_v_2_2 += pow3[5];
        b.index_o.h_v_3_3 += pow3[1];
        b.index_o.corner_3x3_3 += pow3[1];
        b.index_o.edge_block_2 += pow3[0];
        b.index_o.triangle_3 += pow3[1];
        b.index_o.corner_2x5_7 += pow3[2];
    }
    if (diff_o & INDEX_D7) {
        b.index_o.diagonal6_1 += pow3[4];
        b.index_o.diagonal5_2 += pow3[1];
        b.index_o.h_v_2_2 += pow3[4];
        b.index_o.h_v_4_3 += pow3[1];
        b.index_o.edge_block_2 += pow3[1];
        b.index_o.corner_2x5_2 += pow3[0];
        b.index_o.corner_2x5_7 += pow3[1];
    }
    if (diff_o & INDEX_E7) {
        b.index_o.diagonal6_2 += pow3[1];
        b.index_o.diagonal5_1 += pow3[3];
        b.index_o.h_v_2_2 += pow3[3];
        b.index_o.h_v_4_1 += pow3[1];
        b.index_o.edge_block_2 += pow3[2];
        b.index_o.corner_2x5_2 += pow3[1];
        b.index_o.corner_2x5_7 += pow3[0];
    }
    if (diff_o & INDEX_F7) {
        b.index_o.diagonal7_2 += pow3[1];
        b.index_o.h_v_2_2 += pow3[2];
        b.index_o.h_v_3_1 += pow3[1];
        b.index_o.corner_3x3_2 += pow3[3];
        b.index_o.edge_block_2 += pow3[3];
        b.index_o.triangle_2 += pow3[3];
        b.index_o.corner_2x5_2 += pow3[2];
    }
    if (diff_o & INDEX_G7) {
        b.index_o.diagonal8_0 += pow3[1];
        b.index_o.edge_2x_1 += pow3[0];
        b.index_o.edge_2x_2 += pow3[1];
        b.index_o.h_v_2_1 += pow3[1];
        b.index_o.h_v_2_2 += pow3[1];
        b.index_o.corner_3x3_2 += pow3[4];
        b.index_o.edge_x_side_2 += pow3[3];
        b.index_o.triangle_2 += pow3[4];
        b.index_o.corner_2x5_2 += pow3[3];
        b.index_o.corner_2x5_6 += pow3[3];
    }
    if (diff_o & INDEX_H7) {
        b.index_o.diagonal7_0 += pow3[0];
        b.index_o.edge_2x_1 += pow3[3];
        b.index_o.h_v_2_2 += pow3[0];
        b.index_o.corner_3x3_2 += pow3[5];
        b.index_o.edge_x_side_2 += pow3[4];
        b.index_o.triangle_2 += pow3[5];
        b.index_o.corner_2x5_2 += pow3[4];
        b.index_o.corner_2x5_6 += pow3[8];
    }
    if (diff_o & INDEX_A8) {
        b.index_o.diagonal8_1 += pow3[0];
        b.index_o.edge_2x_2 += pow3[2];
        b.index_o.edge_2x_3 += pow3[9];
        b.index_o.corner_3x3_3 += pow3[8];
        b.index_o.edge_x_side_3 += pow3[9];
        b.index_o.edge_block_2 += pow3[4];
        b.index_o.edge_block_3 += pow3[9];
        b.index_o.triangle_3 += pow3[9];
        b.index_o.corner_2x5_3 += pow3[9];
        b.index_o.corner_2x5_7 += pow3[9];
    }
    if (diff_o & INDEX_B8) {
        b.index_o.diagonal7_1 += pow3[0];
        b.index_o.edge_2x_2 += pow3[3];
        b.index_o.h_v_2_3 += pow3[0];
        b.index_o.corner_3x3_3 += pow3[5];
        b.index_o.edge_x_side_3 += pow3[4];
        b.index_o.triangle_3 += pow3[5];
        b.index_o.corner_2x5_3 += pow3[4];
        b.index_o.corner_2x5_7 += pow3[8];
    }
    if (diff_o & INDEX_C8) {
        b.index_o.diagonal6_1 += pow3[5];
        b.index_o.edge_2x_2 += pow3[4];
        b.index_o.h_v_3_3 += pow3[0];
        b.index_o.corner_3x3_3 += pow3[2];
        b.index_o.edge_x_side_3 += pow3[2];
        b.index_o.edge_block_2 += pow3[5];
        b.index_o.triangle_3 += pow3[2];
        b.index_o.corner_2x5_7 += pow3[7];
    }
    if (diff_o & INDEX_D8) {
        b.index_o.diagonal5_1 += pow3[4];
        b.index_o.edge_2x_2 += pow3[5];
        b.index_o.h_v_4_3 += pow3[0];
        b.index_o.edge_x_side_2 += pow3[5];
        b.index_o.edge_x_side_3 += pow3[1];
        b.index_o.edge_block_2 += pow3[6];
        b.index_o.triangle_3 += pow3[0];
        b.index_o.corner_2x5_2 += pow3[5];
        b.index_o.corner_2x5_7 += pow3[6];
    }
    if (diff_o & INDEX_E8) {
        b.index_o.diagonal5_2 += pow3[0];
        b.index_o.edge_2x_2 += pow3[6];
        b.index_o.h_v_4_1 += pow3[0];
        b.index_o.edge_x_side_2 += pow3[6];
        b.index_o.edge_x_side_3 += pow3[0];
        b.index_o.edge_block_2 += pow3[7];
        b.index_o.triangle_2 += pow3[6];
        b.index_o.corner_2x5_2 += pow3[6];
        b.index_o.corner_2x5_7 += pow3[5];
    }
    if (diff_o & INDEX_F8) {
        b.index_o.diagonal6_2 += pow3[0];
        b.index_o.edge_2x_2 += pow3[7];
        b.index_o.h_v_3_1 += pow3[0];
        b.index_o.corner_3x3_2 += pow3[6];
        b.index_o.edge_x_side_2 += pow3[7];
        b.index_o.edge_block_2 += pow3[8];
        b.index_o.triangle_2 += pow3[7];
        b.index_o.corner_2x5_2 += pow3[7];
    }
    if (diff_o & INDEX_G8) {
        b.index_o.diagonal7_2 += pow3[0];
        b.index_o.edge_2x_2 += pow3[8];
        b.index_o.h_v_2_1 += pow3[0];
        b.index_o.corner_3x3_2 += pow3[7];
        b.index_o.edge_x_side_2 += pow3[8];
        b.index_o.triangle_2 += pow3[8];
        b.index_o.corner_2x5_2 += pow3[8];
        b.index_o.corner_2x5_6 += pow3[4];
    }
    if (diff_o & INDEX_H8) {
        b.index_o.diagonal8_0 += pow3[0];
        b.index_o.edge_2x_1 += pow3[2];
        b.index_o.edge_2x_2 += pow3[9];
        b.index_o.corner_3x3_2 += pow3[8];
        b.index_o.edge_x_side_2 += pow3[9];
        b.index_o.edge_block_1 += pow3[4];
        b.index_o.edge_block_2 += pow3[9];
        b.index_o.triangle_2 += pow3[9];
        b.index_o.corner_2x5_2 += pow3[9];
        b.index_o.corner_2x5_6 += pow3[9];
    }
    
}

#endif /* index_h */
