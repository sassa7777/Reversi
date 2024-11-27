from copy import deepcopy
import tensorflow as tf
from tensorflow.keras.layers import Add, Dense, Input, LeakyReLU, Concatenate
from tensorflow.keras.models import Model
from tensorflow.keras.callbacks import EarlyStopping, TensorBoard
import numpy as np
from tqdm import trange, tqdm
from random import shuffle
import subprocess
import datetime
import os
from othello_py import *

def digit(n, r):
    n = str(n)
    l = len(n)
    for _ in range(r - l):
        n = '0' + n
    return n

records = []
for num in range(1):
    with open('self_play/' + digit(num, 7) + '.txt', 'r') as f:
        records.extend(list(f.read().splitlines()))
data = []
evaluate_additional = subprocess.Popen('./evaluate.out'.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE)
for record in tqdm(records):
    o = othello()
    board_data = []
    for i in range(0, len(record), 2):
        board_str = ''
        for j in range(hw):
            for k in range(hw):
                board_str += '.' if o.grid[j][k] == vacant or o.grid[j][k] == legal else str(o.grid[j][k])
        evaluate_additional.stdin.write((str(o.player) + '\n' + board_str + '\n').encode('utf-8'))
        evaluate_additional.stdin.flush()
        additional_param = [float(elem) for elem in evaluate_additional.stdout.readline().decode().split()]
        board_data.append([board_str, o.player, additional_param[0]])
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
evaluate_additional.kill()
print('n_data', len(data))

# 学習
test_ratio = 0.001
n_epochs = 6

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
    
diagonal4_idx = [[4, 13, 22, 31], [32, 41, 50, 59], [3, 10, 17, 24], [39, 46, 53, 60]]
for pattern in deepcopy(diagonal4_idx):
    diagonal4_idx.append(list(reversed(pattern)))

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

pattern_idx = [diagonal8_idx, diagonal7_idx, diagonal6_idx, diagonal5_idx, diagonal4_idx, edge_2x_idx, h_v_2_idx, h_v_3_idx, h_v_4_idx, corner_3x3_idx, edge_x_side_idx, edge_block_idx, triangle_idx]
ln_in = sum([len(elem) for elem in pattern_idx]) + 1
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

def collect_data(board, player, v1, result):
    global all_data, all_labels
    v1 = float(v1)
    result = float(result) / 64
    player = int(player)
    idx = 0
    for i in range(len(pattern_idx)):
        lines = make_lines(board, pattern_idx[i], 0)
        for line in lines:
            all_data[idx].append(line)
            idx += 1
    all_data[idx].append([v1 / 30])
    all_labels.append(result)

x = [None for _ in range(ln_in)]
ys = []
names = ['diagonal8_idx', 'diagonal7_idx', 'diagonal6_idx', 'diagonal5_idx', 'diagonal4_idx', 'edge_2x_idx', 'h_v_2_idx', 'h_v_3_idx', 'h_v_4_idx', 'corner_3x3_idx', 'edge_x_side_idx', 'edge_block_idx', 'triangle_idx']
idx = 0
for i in range(len(pattern_idx)):
    layers = []
    layers.append(Dense(16, name=names[i] + '_dense0'))
    layers.append(LeakyReLU(negative_slope=0.01))
    layers.append(Dense(16, name=names[i] + '_dense1'))
    layers.append(LeakyReLU(negative_slope=0.01))
    layers.append(Dense(1, name=names[i] + '_out'))
    layers.append(LeakyReLU(negative_slope=0.01))
    add_elems = []
    for j in range(len(pattern_idx[i])):
        x[idx] = Input(shape=(len(pattern_idx[i][0]) * 2,), name=names[i] + '_in_' + str(j))
        tmp = x[idx]
        for layer in layers:
            tmp = layer(tmp)
        add_elems.append(tmp)
        idx += 1
    ys.append(Add()(add_elems))
y_pattern = Concatenate(axis=-1)(ys)
x[idx] = Input(shape=(1,), name='additional_input')
y_add = Dense(8, name='add_dense0')(x[idx])
y_add = LeakyReLU(negative_slope=0.01)(y_add)
y_add = Dense(1, name='add_dense1')(y_add)
y_add = LeakyReLU(negative_slope=0.01)(y_add)
y_all = Concatenate(axis=-1)([y_pattern, y_add])
y_all = Dense(1, name='all_dense0')(y_all)

model = Model(inputs=x, outputs=y_all)

model.summary()
model.compile(loss='mse', metrics=['mae'], optimizer='adam')

for i in trange(len(data)):
    collect_data(*data[i])
len_data = len(all_labels)
print(len_data)

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

n_train_data = int(len_data * (1.0 - test_ratio))
n_test_data = int(len_data * test_ratio)

train_data = [arr[0:n_train_data] for arr in all_data]
train_labels = all_labels[0:n_train_data]
test_data = [arr[n_train_data:len_data] for arr in all_data]
test_labels = all_labels[n_train_data:len_data]

log_dir = "logs/fit/" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
tensorboard_callback = TensorBoard(log_dir=log_dir, histogram_freq=1, write_graph=True, write_images=True)

def lr_schedule(epoch, lr):
    if epoch == 1 or epoch == 2:
        return lr/2
    else:
        return lr

lr_callback = tf.keras.callbacks.LearningRateScheduler(lr_schedule, verbose=1)


print(model.evaluate(test_data, test_labels))
checkpoint = ModelCheckpoint(
    'models/best_model.h5',
    monitor='val_loss',
    save_best_only=True
)
early_stop = EarlyStopping(monitor='val_loss', patience=1, restore_best_weights=True)
history = model.fit(train_data, train_labels, epochs=n_epochs, validation_data=(test_data, test_labels), callbacks=[early_stop, checkpoint, tensorboard_callback], batch_size=32)

now = datetime.datetime.today()
model.save('models/model.h5')

subprocess.run('/usr/bin/python3 output_model.py model.h5 model.txt'.split())
