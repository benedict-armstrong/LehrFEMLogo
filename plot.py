import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import imageio

images = []

for file in os.listdir("build/src/out"):
    if file.endswith(".csv"):
        df = pd.read_csv(f"build/src/out/{file}")
        df.loc[len(df.index)] = [0]
        array = df.to_numpy().reshape((30, 76))

        images.append(array)

imageio.v2.mimwrite('test.gif', images)
