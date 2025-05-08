import os
from PIL import Image

input_folder = "/home/sabarinath/Documents/faeture_phone/home/assets"
output_folder = "resized_images"

os.makedirs(output_folder, exist_ok=True)

supported_formats = ('.jpg', '.jpeg', '.png', '.bmp', '.gif', '.tiff')

for filename in os.listdir(input_folder):
    if filename.lower().endswith(supported_formats):
        img_path = os.path.join(input_folder, filename)
        with Image.open(img_path) as img:
            img_resized = img.resize((32, 32), Image.Resampling.LANCZOS)
            output_path = os.path.join(output_folder, filename)
            img_resized.save(output_path)
            print(f"Resized and saved: {output_path}")

print("All images have been resized to 32x32.")
