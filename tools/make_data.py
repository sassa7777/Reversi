from copy import deepcopy
from tqdm import trange, tqdm
from random import shuffle
import subprocess
import datetime
import os
import sys
from othello_py import *
import numpy as np

def digit(n, r):
    n = str(n)
    l = len(n)
    for _ in range(r - l):
        n = '0' + n
    return n

records = []
num = int(sys.argv[1])
with open('self_play/'+ digit(num, 7) + '.txt', 'r') as f:
        records.extend(list(f.read().splitlines()))
# with open(digit(99999, 7) + '.txt', 'r') as f:
#         records.extend(list(f.read().splitlines()))
# with open(digit(100000, 7) + '.txt', 'r') as f:
#         records.extend(list(f.read().splitlines()))
data = []
# evaluate_additional = subprocess.Popen('./evaluate.out'.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE)
for record in records:
    o = othello()
    board_data = []
    for i in range(0, len(record), 2):
        board_str = ''
        for j in range(hw):
            for k in range(hw):
                board_str += '.' if o.grid[j][k] == vacant or o.grid[j][k] == legal else str(o.grid[j][k])
        # evaluate_additional.stdin.write((str(o.player) + '\n' + board_str + '\n').encode('utf-8'))
        # evaluate_additional.stdin.flush()
        # additional_param = [float(elem) for elem in evaluate_additional.stdout.readline().decode().split()]
        # if i > 38 and i < 80:
        if i > 38:
            board_data.append([board_str, o.player])
        if not o.check_legal():
            o.player = 1 - o.player
            o.check_legal()
        x = ord(record[i]) - ord('a')
        y = int(record[i + 1]) - 1
        o.move(y, x)
    result = o.n_stones[0] - o.n_stones[1]
    n_vacant = hw2 - (o.n_stones[0] + o.n_stones[1])
    if result > 0:
        result += n_vacant
    elif result < 0:
        result -= n_vacant
    for board_datum in board_data:
        board_datum.append(result)
        data.append(board_datum)
print(num)
# evaluate_additional.kill()
# data = []
# for non_record in records:
#     record = non_record.split()
#     board_str = ''
#     for i in range(64):
#         if record[0][i] == 'O':
#             board_str += '0'
#         elif record[0][i] == 'X':
#             board_str += '1'
#         else:
#             board_str += '.'
#     result = int(record[1])
#     data.append([board_str, 0, result])
# # print('n_data', len(data))

# 学習
test_ratio = 0.1
n_epochs = 50

diagonal8_idx = [[0, 9, 18, 27, 36, 45, 54, 63], [7, 14, 21, 28, 35, 42, 49, 56]]
for pattern in deepcopy(diagonal8_idx):
    diagonal8_idx.append(list(reversed(pattern)))

diagonal7_idx = [[1, 10, 19, 28, 37, 46, 55], [8, 17, 26, 35, 44, 53, 62], [6, 13, 20, 27, 34, 41, 48], [15, 22, 29, 36, 43, 50, 57]]
for pattern in deepcopy(diagonal7_idx):
    diagonal7_idx.append(list(reversed(pattern)))

diagonal6_idx = [[2, 11, 20, 29, 38, 47], [16, 25, 34, 43, 52, 61], [5, 12, 19, 26, 33, 40], [23, 30, 37, 44, 51, 58]]
for pattern in deepcopy(diagonal6_idx):
    diagonal6_idx.append(list(reversed(pattern)))

diagonal5_idx = [[3, 12, 21, 30, 39], [24, 33, 42, 51, 60], [4, 11, 18, 25, 32], [31, 38, 45, 52, 59]]
for pattern in deepcopy(diagonal5_idx):
    diagonal5_idx.append(list(reversed(pattern)))

edge_2x_idx = [[9, 0, 1, 2, 3, 4, 5, 6, 7, 14], [9, 0, 8, 16, 24, 32, 40, 48, 56, 49], [49, 56, 57, 58, 59, 60, 61, 62, 63, 54], [54, 63, 55, 47, 39, 31, 23, 15, 7, 14]]
for pattern in deepcopy(edge_2x_idx):
    edge_2x_idx.append(list(reversed(pattern)))

h_v_2_idx = [[8, 9, 10, 11, 12, 13, 14, 15], [48, 49, 50, 51, 52, 53, 54, 55], [1, 9, 17, 25, 33, 41, 49, 57], [6, 14, 22, 30, 38, 46, 54, 62]]
for pattern in deepcopy(h_v_2_idx):
    h_v_2_idx.append(list(reversed(pattern)))

h_v_3_idx = [[16, 17, 18, 19, 20, 21, 22, 23], [40, 41, 42, 43, 44, 45, 46, 47], [2, 10, 18, 26, 34, 42, 50, 58], [5, 13, 21, 29, 37, 45, 53, 61]]
for pattern in deepcopy(h_v_3_idx):
    h_v_3_idx.append(list(reversed(pattern)))

h_v_4_idx = [[24, 25, 26, 27, 28, 29, 30, 31], [32, 33, 34, 35, 36, 37, 38, 39], [3, 11, 19, 27, 35, 43, 51, 59], [4, 12, 20, 28, 36, 44, 52, 60]]
for pattern in deepcopy(h_v_4_idx):
    h_v_4_idx.append(list(reversed(pattern)))

corner_3x3_idx = [[0, 1, 8, 9, 2, 16, 10, 17, 18], [0, 8, 1, 9, 16, 2, 17, 10, 18], [7, 6, 15, 14, 5, 23, 13, 22, 21], [7, 15, 6, 14, 23, 5, 22, 13, 21], [56, 57, 48, 49, 58, 40, 50, 41, 42], [56, 48, 57, 49, 40, 58, 41, 50, 42], [63, 62, 55, 54, 61, 47, 53, 46, 45], [63, 55, 62, 54, 47, 61, 46, 53, 45]]

edge_x_side_idx = [[32, 24, 16, 8, 0, 9, 1, 2, 3, 4], [4, 3, 2, 1, 0, 9, 8, 16, 24, 32], [39, 31, 23, 15, 7, 14, 6, 5, 4, 3], [3, 4, 5, 6, 7, 14, 15, 23, 31, 39], [24, 32, 40, 48, 56, 49, 57, 58, 59, 60], [60, 59, 58, 57, 56, 49, 48, 40, 32, 24], [59, 60, 61, 62, 63, 54, 55, 47, 39, 31], [31, 39, 47, 55, 54, 63, 62, 61, 60, 59]]

edge_block_idx = [[0, 2, 3, 10, 11, 12, 13, 4, 5, 7], [7, 23, 31, 22, 30, 38, 46, 39, 47, 63], [56, 58, 59, 50, 51, 52, 53, 60, 61, 63], [0, 16, 24, 17, 25, 33, 41, 32, 40, 56]]
for pattern in deepcopy(edge_block_idx):
    edge_block_idx.append(list(reversed(pattern)))

triangle_idx = [
    [0, 1, 2, 3, 8, 9, 10, 16, 17, 24], [0, 8, 16, 24, 1, 9, 17, 2, 10, 3],
    [7, 6, 5, 4, 15, 14, 13, 23, 22, 31], [7, 15, 23, 31, 6, 14, 22, 5, 13, 4],
    [63, 62, 61, 60, 55, 54, 53, 47, 46, 39], [63, 55, 47, 39, 62, 54, 46, 61, 53, 60],
    [56, 57, 58, 59, 48, 49, 50, 40, 41, 32], [56, 48, 40, 32, 57, 49, 41, 58, 50, 59]
]

pattern_idx = [diagonal8_idx, diagonal7_idx, diagonal6_idx, diagonal5_idx, edge_2x_idx, h_v_2_idx, h_v_3_idx, h_v_4_idx, corner_3x3_idx, edge_x_side_idx, edge_block_idx, triangle_idx]
ln_in = sum([len(elem) for elem in pattern_idx])
all_data = [[] for _ in range(ln_in)]
all_labels = []

def make_lines(board, patterns, player):
    res = []
    for pattern in patterns:
        tmp = []
        for elem in pattern:
            tmp.append(1.0 if board[elem] == str(player) else 0.0)
        for elem in pattern:
            tmp.append(1.0 if board[elem] == str(1 - player) else 0.0)
        res.append(tmp)
    return res

def calc_n_stones(board):
    res = 0
    for elem in board:
        res += int(elem != '.')
    return res

def collect_data(board, player, result):
    global all_data, all_labels
    result = float(result)/64
    player = int(player)
    idx = 0
    for i in range(len(pattern_idx)):
        lines = make_lines(board, pattern_idx[i], 0)
        for line in lines:
            all_data[idx].append(line)
            idx += 1
    all_labels.append(result)



for i in range(len(data)):
    collect_data(*data[i])
len_data = len(all_labels)
print(num)
#
tmp_data = deepcopy(all_data)
tmp_labels = deepcopy(all_labels)
all_data = [[] for _ in range(len(tmp_data))]
all_labels = []
shuffled = list(range(len_data))
shuffle(shuffled)
for i in shuffled:
    all_labels.append(tmp_labels[i])
    for j in range(len(tmp_data)):
        all_data[j].append(tmp_data[j][i])

all_data = [np.array(arr) for arr in all_data]
all_labels = np.array(all_labels)

data_dir = f"data/{num}"

# 必要なディレクトリを作成
os.makedirs(data_dir, exist_ok=True)
# print(f"Number of samples: {len(all_data)}")
# print(f"Number of labels: {len(all_labels)}")


# 保存先のパス
i = 0
for data in all_data:
    np.save(f"{data_dir}/data_{i}.npy", data)
    # save_data(f"{data_dir}/data_{i}.blosc2", data)
    i += 1
# i = 0
# for data in train_labels:
#     np.save(f"{train_data_dir}/train_labels_{i}.npy", data)
#     i += 1
np.save(f"{data_dir}/labels.npy", all_labels)
# save_data(f"{data_dir}/labels.blosc2", all_labels)
#
