import asyncio
import logging
import threading
from typing import List
from typing import Union
import json
from fastapi import FastAPI, WebSocket, Request
from pydantic import BaseModel
import realtime_asr
from starlette.websockets import WebSocket, WebSocketDisconnect
from thread_helper import WEBSOCKET_OUTPUT
import openai_api

logger = logging.getLogger()
logger.setLevel(logging.INFO)  
# logger.setLevel(logging.DEBUG)  

AST = openai_api.Assistant()

app = FastAPI()
file_index = 0

class Item(BaseModel):
    name: str
    price: float
    is_offer: Union[bool, None] = None

class Message(BaseModel):
    text: str


@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.get("/items/{item_id}")
def read_item(item_id: int, q: Union[str, None] = None):
    return {"item_id": item_id, "q": q}


@app.put("/items/{item_id}")
def update_item(item_id: int, item: Item):
    return {"item_name": item.name, "item_id": item_id}

@app.websocket("/hls")
async def hls_endpoint(websocket: WebSocket):
    await websocket.accept()
    while True:
        # 从WebSocket接收HLS数据
        hls_data = await websocket.receive_text()
        await websocket.send_text(
            f"Session cookie or query token value is: {hls_data}"
        )

class Notifier:
    def __init__(self):
        self.connections: List[WebSocket] = []
        self.generator = self.get_notification_generator()
        self.queue_task_status = 0
        self.queue_task_stop_flag = 1

    async def get_notification_generator(self):
        while True:
            message = yield
            await self._notify(message)

    async def push(self, msg: str):
        await self.generator.asend(msg)

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.connections.append(websocket)
        if self.queue_task_status == 0:
            self.queue_task_status = 1;
            self.queue_task_stop_flag = 0;
            threading.Thread(target=websocket_queue_task).start()
            threading.Thread(target=openai_queue_task).start()

    def remove(self, websocket: WebSocket):
        self.connections.remove(websocket)
        if len(self.connections) == 0 and self.queue_task_status == 1:
            self.queue_task_stop_flag = 1;
            self.queue_task_status = 0;

    async def _notify(self, message: str):
        living_connections = []
        while len(self.connections) > 0:
            # Looping like this is necessary in case a disconnection is handled
            # during await websocket.send_text(message)
            websocket = self.connections.pop()
            await websocket.send_text(message)
            living_connections.append(websocket)
        self.connections = living_connections

notifier = Notifier()

def websocket_queue_task():
    logger.info("websocket_queue_task start")
    while not notifier.queue_task_stop_flag:
        try:
            data = WEBSOCKET_OUTPUT.get(timeout=3)
            asyncio.run(notifier.push(data))
        except:
            ...
    logger.info("websocket_queue_task end")

def openai_queue_task():
    logger.info("websocket_queue_task start")
    while not notifier.queue_task_stop_flag:
        try:
            data = openai_api.OPENAI_INPUT.get(timeout=3)
            AST.stream_run(data)
        except:
            ...
    logger.info("websocket_queue_task end")

@app.websocket("/recode")
async def recode_endpoint(websocket: WebSocket):
    await notifier.connect(websocket)
    # await websocket.accept()
    while True:
        try:
            bytes_data = await websocket.receive()
        except:
            notifier.remove(websocket)
            break
        # 语音识别的开始和结束信号由esp32发送"{\"action\":\"ASR_START\"}" 和 "{\"action\":\"ASR_STOP\"}"
        # 如果为bytes则为音频pcm数据
        # 如果需要发送给 openai 则 {action : OPENAI_ASSISTANT, message:xxxxxx}
        if ('text' in bytes_data.keys()):
            logger.info("get text:{}".format(bytes_data['text']))
            receive_json = json.loads(bytes_data['text'].rstrip('\x00'))
            action = receive_json['action']
            if (action == 'ASR_START'):
                realtime_asr.EXIT_FLAG = False
                realtime_asr.recoder_thread()
            elif (action == 'ASR_STOP' and not realtime_asr.EXIT_FLAG):
                realtime_asr.EXIT_FLAG = True
            elif (action == "OPENAI_ASSISTANT"):
                message = receive_json['message']
                openai_api.OPENAI_INPUT.put_nowait(message)
                
        elif ('bytes' in bytes_data.keys()):
            # logger.debug("get bytes")
            realtime_asr.ASR_INPUT.put_nowait(bytes_data['bytes'])

@app.on_event("startup")
async def startup():
    # Prime the push notification generator
    await notifier.generator.asend(None)

@app.get("/push/{message}")
async def push_to_connected_websockets(message: str):
    WEBSOCKET_OUTPUT.put(message)
    # await notifier.push(f"! Push notification: {message} !")

from PIL import Image
import numpy as  np
import time
import uuid

def rgb565_to_rgb888(rgb565_data):
    # 将每个16位的RGB565数据转换为24位的RGB888格式
    r = ((rgb565_data >> 11) & 0x1F) << 3
    g = ((rgb565_data >> 5) & 0x3F) << 2
    b = (rgb565_data & 0x1F) << 3
    return (r, g, b)



@app.post("/post_img")
async def save_img(request: Request):
    global file_index
    data = await request.body()
    random_uuid = uuid.uuid4()
    filename = "C:\\project\\embedded\\WhatsDer\\3.Server\\whatsderserver\\resource\\" + str(file_index) + ".txt"
    file_index += 1
    with open(filename, 'wb') as file:
        file.write(data)
    print(f"文件已保存: {filename}")
    # # file_size = len(data)
    # # return {"message": "File received", "size": file_size}
    # # 创建一个空的numpy数组存储RGB888数据
    # rgb888_data = np.zeros((240, 240, 3), dtype=np.uint8)
    
    # # 读取RGB565数据
    # for y in range(240):
    #     for x in range(240):
    #         index = (x + y * 240) * 2  # 2 bytes per pixel
    #         # 从二进制数据中提取RGB565值
    #         rgb565 = int.from_bytes(data[index:index+2], 'little')
    #         # 转换为RGB888
    #         rgb888_data[y, x] = rgb565_to_rgb888(rgb565)
    
    # # 创建图像
    # random_uuid = uuid.uuid4()
    # img = Image.fromarray(rgb888_data, 'RGB')
    # filename = f"C:\project\embedded\WhatsDer\3.Server\whatsderserver\resource\{random_uuid}" + ".png" 
    # print(filename)
    # img.save(filename)
    # print(f"Image saved as {filename}")
    return f"Image saved as {filename}"

if __name__ == "__main__":
    logging.basicConfig(format='[%(asctime)-15s] [%(funcName)s()][%(levelname)s] %(message)s')
    
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)