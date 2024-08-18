import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import imageio
from cmap import Colormap

images = []

# lut = np.asarray(imageio.imread("lut.png"), dtype=np.uint8)
lut = np.array((
    np.concatenate((np.zeros(127), np.arange(128))),
    np.zeros(255),
    np.concatenate((np.arange(-128, 0) * -1, np.zeros(127))),
    # np.arange(-255, 0) * -1
), dtype=np.uint8).transpose()

print(lut)

for file in os.listdir("build/src/out"):
    if file.endswith(".csv"):
        df = pd.read_csv(f"build/src/out/{file}")
        array = df.to_numpy()

        # invert image
        array = array * 254

        x, y = array.shape

        array = np.array(array, dtype=np.uint8)
        array = np.where(array > 254, 254, array)

        array_colour = np.zeros((x, y, 4), dtype=np.uint8)

        array_colour[..., 0] = np.take(lut[..., 0], array)
        array_colour[..., 1] = np.take(lut[..., 1], array)
        array_colour[..., 2] = np.take(lut[..., 2], array)
        array_colour[..., 3] = np.where(array > 30, array, 0)

        images.append(array_colour)

imageio.v3.imwrite('test.gif', images, loop=True, fps=10)
