
from PIL import Image
import numpy as np
import pandas as pd

# Define the path to the PNG file
image_path = 'logo_small.png'

# Open the image file using PIL
image = Image.open(image_path)

# Convert the image to a NumPy array
image_array = np.array(image, np.float32)

# Print the shape of the image array to verify
print('Image array shape:', image_array.shape)

# image_array = ((image_array / 255) - 1) * -1000

# save to one large csv file
df = pd.DataFrame(image_array.reshape(-1, 1))

df.to_csv("build/src/initial_condition.csv", sep=",", index=False)
