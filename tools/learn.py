from copy import deepcopy
import tensorflow as tf
from tensorflow.keras.layers import Add, Dense, Input, LeakyReLU, Concatenate, Dropout
from tensorflow.keras import mixed_precision
from tensorflow.keras.models import Model, load_model
from tensorflow.keras.optimizers import AdamW, Adam, SGD, Nadam
from tensorflow.keras.utils import Sequence
from tensorflow.keras.initializers import HeUniform, HeNormal
from keras.regularizers import l2
from tensorflow.keras.callbacks import EarlyStopping, TensorBoard, ModelCheckpoint, LearningRateScheduler
from tensorflow.keras.initializers import Zeros
import numpy as np
from tqdm import trange, tqdm
from random import shuffle
import subprocess
import datetime
import os
import glob
from othello_py import *
import gc

def digit(n, r):
    n = str(n)
    l = len(n)
    for _ in range(r - l):
        n = '0' + n
    return n

class BaseSchedule(tf.keras.optimizers.schedules.LearningRateSchedule):
    def __init__(self, lr, decay_start=0.0, decay_ratio=1.0, beta_2=0.999):
        super().__init__()  # Don't pass any arguments to the parent class
        self.lr = lr
        self.decay_start = decay_start
        self.decay_ratio = decay_ratio
        self.beta_2 = beta_2

    def get_config(self):
        config = {"lr": float(self.lr),
                  "decay_start": float(self.decay_start),
                  "decay_ratio": float(self.decay_ratio),
                  "beta_2": float(self.beta_2)
                  }
        print('get_config')
        return config

    def __call__(self, step):
        return tf.where(step >= self.decay_start, self.decay_ratio*self.lr, self.lr)

class RectifiedAdamSchedule(BaseSchedule):
    def __call__(self, step):
        lr = super().__call__(step)
        sma_inf = 2.0 / (1.0 - self.beta_2) - 1.0
        beta2_power = self.beta_2**step
        sma_t = sma_inf - 2.0 * step * beta2_power / (1.0 - beta2_power)
        r_t =  tf.sqrt( (sma_t - 4.0) / (sma_inf - 4.0) * (sma_t - 2.0) / (sma_inf - 2.0) * sma_inf / sma_t)
        return tf.where(sma_t > 4.0 , r_t*lr, 0.0)


lr_schedule = RectifiedAdamSchedule(BaseSchedule(lr=0.001, decay_start=1502124, decay_ratio=0.1))


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

x = [None for _ in range(ln_in)]
ys = []
names = ['diagonal8_idx', 'diagonal7_idx', 'diagonal6_idx', 'diagonal5_idx', 'edge_2x_idx', 'h_v_2_idx', 'h_v_3_idx', 'h_v_4_idx', 'corner_3x3_idx', 'edge_x_side_idx', 'edge_block_idx', 'triangle_idx']
idx = 0
for i in range(len(pattern_idx)):
    layers = []
    layers.append(Dense(128, name=names[i] + '_dense0', bias_initializer=Zeros()))
    layers.append(Dropout(0.2))
    layers.append(LeakyReLU(negative_slope=0.01))
    layers.append(Dense(128, name=names[i] + '_dense1', bias_initializer=Zeros()))
    layers.append(Dropout(0.2))
    layers.append(LeakyReLU(negative_slope=0.01))
    layers.append(Dense(1, name=names[i] + '_out', bias_initializer=Zeros()))
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
y_all = Concatenate(axis=-1)([y_pattern])
y_all = Dense(1, name='all_dense0', bias_initializer=Zeros())(y_all)

model = Model(inputs=x, outputs=y_all)

# model = load_model('models/model_06.keras')

# 学習
n_epochs = 20
n_initial_epoch = 0

opt=Adam(learning_rate=0.001)
# opt=AdamW(learning_rate=0.001, weight_decay=0.00001)
# opt=SGD(learning_rate=0.01, momentum=0.9, nesterov=True)
# opt=Nadam()
model.compile(loss='mse', metrics=['mae'], optimizer=opt)
model.summary()

test_data =[]

train_folder_path = f"data/{9999999}"
n_test_data = 92
for i in range(n_test_data):
    test_data.append(np.load(f"{train_folder_path}/data_{i}.npy"))
test_labels = np.load(f"{train_folder_path}/labels.npy")

class TrainDataGenerator(tf.keras.utils.PyDataset):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.use_multiprocessing = True
        self.workers = 10
        self.max_queue_size = 1000
        return

    def __getitem__(self, idx):
        train_data = []
        train_folder_path = f"data/{idx}"
        for i in range(92):
            train_data.append(np.load(f"{train_folder_path}/data_{i}.npy"))
        train_labels = np.load(f"{train_folder_path}/labels.npy")
        return tuple(train_data), train_labels

    def on_epoch_end(self):
        return

    def __len__(self):
        return  1722124



print(f"Number of test samples: {len(test_labels)}")

log_dir = "logs/fit/" + datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
tensorboard_callback = TensorBoard(log_dir=log_dir, histogram_freq=1, write_graph=True, write_images=True)


# print(model.evaluate(test_data, test_labels))

early_stop = EarlyStopping(monitor='val_loss', patience=3, restore_best_weights=True)
check_point = ModelCheckpoint('models/model_{epoch:02d}.keras', monitor='val_loss', verbose=1, save_best_only=False, save_weights_only=False, save_freq='epoch')

history = model.fit(TrainDataGenerator(), epochs=n_epochs, validation_data=(test_data, test_labels), callbacks=[early_stop, tensorboard_callback, check_point], batch_size=32, initial_epoch=n_initial_epoch)

now = datetime.datetime.today()
model.save('models/model.keras')

subprocess.run('/usr/bin/python3 output_model.py model.keras model_end.txt'.split())
