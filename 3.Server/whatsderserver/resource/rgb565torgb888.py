from PIL import Image,ImageDraw
import os
import uuid
import numpy as np

FileAdd = r'C:\project\embedded\WhatsDer\3.Server\whatsderserver\resource\0.txt' #源文件路径

OutFile = r"C:\project\embedded\WhatsDer\3.Server\whatsderserver\resource\0.jpeg" #输出文件名称

def one_chanel_open():
    # 假设你知道图像的宽度和高度
    width = 680  # 替换为实际宽度
    height = 480  # 替换为实际高度

    # 打开二进制文件
    with open(FileAdd, 'rb') as f:
        # 读取二进制数据
        binary_data = f.read()

    # 将二进制数据转换为numpy数组
    image_array = np.frombuffer(binary_data, dtype=np.uint8)

    # 重塑数组为2D图像
    image_array = image_array.reshape((height, width))

    # 创建PIL图像
    image = Image.fromarray(image_array)

    # 显示图像
    image.show()

    # 保存图像（可选）
    image.save(OutFile)

def rgb565_to_rgb888(rgb565_data):
    # 将每个16位的RGB565数据转换为24位的RGB888格式
    r = ((rgb565_data >> 11) & 0x1F) << 3
    g = ((rgb565_data >> 5) & 0x3F) << 2
    b = (rgb565_data & 0x1F) << 3
    return (r, g, b)

def rgb565_img_to_rgb888_img():
    #定义图像大小
    w = 680     #宽
    h = 480     #高

    with open(FileAdd,'rb') as f:
        data = f.read()
    print("正在合成来自" + FileAdd + "的图像数据，合成为" + str(w) + "*" + str(h) + "的图像")

    rgb888_data = np.zeros((240, 240, 3), dtype=np.uint8)

    # 读取RGB565数据
    for y in range(240):
        for x in range(240):
            index = (x + y * 240) * 2  # 2 bytes per pixel
            # 从二进制数据中提取RGB565值
            rgb565 = int.from_bytes(data[index:index+2], 'big')
            # 转换为RGB888
            rgb888_data[y, x] = rgb565_to_rgb888(rgb565)

    # 创建图像
    random_uuid = uuid.uuid4()
    img = Image.fromarray(rgb888_data, 'RGB')
    print(OutFile)
    img.save(OutFile)
    print(f"Image saved as {OutFile}")

if __name__ == "__main__":
    rgb565_img_to_rgb888_img()
    # one_chanel_open()