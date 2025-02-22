import tensorflow as tf
from tensorflow.keras.models import load_model
from tensorflow.keras.losses import MeanSquaredError
import sys

# Python用モデルのパラメータをテキストに出力する

def get_layer_index(model, layer_name, not_found=None):
    for i, l in enumerate(model.layers):
        if l.name == layer_name:
            return i
    return not_found
custom_objects = {'mse': MeanSquaredError()}
model = load_model('models/' + sys.argv[1], custom_objects=custom_objects)

layer_names = ['diagonal8_idx', 'diagonal7_idx', 'diagonal6_idx', 'diagonal5_idx', 'edge_2x_idx', 'h_v_2_idx', 'h_v_3_idx', 'h_v_4_idx', 'corner_3x3_idx', 'edge_x_side_idx', 'edge_block_idx', 'triangle_idx']
names = []
for name in layer_names:
    names.append(name + '_dense0')
    names.append(name + '_dense1')
    names.append(name + '_out')
names.append('all_dense0')

with open('models/' + sys.argv[2], 'w') as f:
    for name in names:
        i = get_layer_index(model, name)
        try:
            #print(i, model.layers[i])
            dammy = model.layers[i]
            j = 0
            while True:
                try:
                    print(model.layers[i].weights[j].shape)
                    if len(model.layers[i].weights[j].shape) == 2:
                        for ii in range(model.layers[i].weights[j].shape[1]):
                            for jj in range(model.layers[i].weights[j].shape[0]):
                                f.write('{:.14f}'.format(model.layers[i].weights[j].numpy()[jj][ii]) + '\n')
                    elif len(model.layers[i].weights[j].shape) == 1:
                        for ii in range(model.layers[i].weights[j].shape[0]):
                            f.write('{:.14f}'.format(model.layers[i].weights[j].numpy()[ii]) + '\n')
                    j += 1
                except:
                    break
        except:
            break
