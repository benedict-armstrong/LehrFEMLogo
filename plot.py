import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import imageio
from cmap import Colormap

images = []

lut = Colormap('bids:plasma')
files = os.listdir("build/src/out")

files = list(filter(lambda x: x.endswith("csv"), files))

files.sort(key=lambda x: int(x.split("_")[1][:-4]))

for file in list(reversed(files)):

    print(file)
    df = pd.read_csv(f"build/src/out/{file}")
    array = df.to_numpy()

    # invert image
    array = array * 200
    # print(np.min(array), np.max(array))

    x, y = array.shape

    array = np.array(array, dtype=np.uint8)
    array = np.where(array > 254, 254, array)

    array_colour = lut(array)

    array_colour = np.array(array_colour * 254, dtype=np.uint8)
    array_colour[..., 3] = np.where(array > 50, array, 0)

    images.append(array_colour)

for _ in range(3):
    images.append(images[-1])

imageio.v3.imwrite('test.gif', images, loop=True, fps=30)
