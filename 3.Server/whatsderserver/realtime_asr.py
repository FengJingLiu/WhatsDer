# -*- coding: utf-8 -*-
"""
实时流式识别
需要安装websocket-client库
使用方式 python realtime_asr.py 16k-0.pcm
"""
import websocket

import threading
import time
import uuid
import json
import logging
import sys
import const
from thread_helper import ASR_INPUT, WEBSOCKET_OUTPUT

EXIT_FLAG = False

if len(sys.argv) < 2:
    pcm_file = "16k-0.pcm"
else:
    pcm_file = sys.argv[1]

class RecodeEvent:
    def __init__(self, event, data=None) -> None:
        self.event = event
        self.data = data
        

logger = logging.getLogger()
logger.setLevel(logging.INFO)  # 调整为logging.INFO，日志会少一点
# logger.setLevel(logging.DEBUG)  # 调整为logging.INFO，日志会少一点

"""

1. 连接 ws_app.run_forever()
2. 连接成功后发送数据 on_open()
2.1 发送开始参数帧 send_start_params()
2.2 发送音频数据帧 send_audio()
2.3 库接收识别结果 on_message()
2.4 发送结束帧 send_finish()
3. 关闭连接 on_close()

库的报错 on_error()
"""


def send_start_params(ws):
    """
    开始参数帧
    :param websocket.WebSocket ws:
    :return:
    """
    req = {
        "type": "START",
        "data": {
            "appid": const.APPID,  # 网页上的appid
            "appkey": const.APPKEY,  # 网页上的appid对应的appkey
            "dev_pid": const.DEV_PID,  # 识别模型
            "cuid": "yourself_defined_user_id",  # 随便填不影响使用。机器的mac或者其它唯一id，百度计算UV用。
            "sample": 16000,  # 固定参数
            "format": "pcm"  # 固定参数
        }
    }
    body = json.dumps(req)
    ws.send(body, websocket.ABNF.OPCODE_TEXT)
    logger.info("send START frame with params:" + body)

def send_audio(ws):
    """
    发送二进制音频数据，注意每个帧之间需要有间隔时间
    :param  websocket.WebSocket ws:
    :return:
    """
    chunk_ms = 160  # 160ms的录音
    chunk_len = int(16000 * 2 / 1000 * chunk_ms)
    with open(pcm_file, 'rb') as f:
        pcm = f.read()

    index = 0
    total = len(pcm)
    logger.info("send_audio total={}".format(total))
    while index < total:
        end = index + chunk_len
        if end >= total:
            # 最后一个音频数据帧
            end = total
        body = pcm[index:end]
        logger.debug("try to send audio length {}, from bytes [{},{})".format(len(body), index, end))
        ws.send(body, websocket.ABNF.OPCODE_BINARY)
        index = end
        time.sleep(chunk_ms / 1000.0)  # ws.send 也有点耗时，这里没有计算

def send_finish(ws):
    """
    发送结束帧
    :param websocket.WebSocket ws:
    :return:
    """
    req = {
        "type": "FINISH"
    }
    body = json.dumps(req)
    if ws.sock and ws.sock.connected:
        ws.send(body, websocket.ABNF.OPCODE_TEXT)
    logger.info("send FINISH frame")

def send_cancel(ws):
    """
    发送取消帧
    :param websocket.WebSocket ws:
    :return:
    """
    req = {
        "type": "CANCEL"
    }
    body = json.dumps(req)
    ws.send(body, websocket.ABNF.OPCODE_TEXT)
    logger.info("send Cancel frame")

def on_record_open(ws):
    """
    连接后发送数据帧
    :param  websocket.WebSocket ws:
    :return:
    """
    logger.debug("start")
    def run(*args):
        """
        发送数据帧
        :param args:
        :return:
        """
        send_start_flag = 1
        logger.debug("start")

        # EXIT_FLAG 为 True 的时候还需要传送队列剩余数据再关闭
        while(not (EXIT_FLAG and ASR_INPUT.empty())):
            asr_data = ASR_INPUT.get()
            # logger.debug("asr_data:{}".format(asr_data))

            if send_start_flag == 1:
                send_start_params(ws)
                send_start_flag = 0
                
            ws.send(asr_data, websocket.ABNF.OPCODE_BINARY)
            logger.debug("try to send audio length {}".format(len(asr_data)))
            

        send_finish(ws)
        logger.debug("thread terminating")
    logger.debug("end")
    threading.Thread(target=run).start()

def on_message(ws, message):
    """
    接收服务端返回的消息
    :param ws:
    :param message: json格式，自行解析
    :return:
    """
    data = json.loads(message)
    if 'type' in data.keys() and data['type'] == 'HEARTBEAT':
        ...
    elif data['err_no'] != 0:
        logger.error("error message: " + message)
    else:
        new_data = {key: data[key] for key in data if key in ["log_id", "type","result"]}
        new_json_str = json.dumps(new_data)
        WEBSOCKET_OUTPUT.put(new_json_str)
        logger.info("on_message Response: " + new_json_str)

def on_error(ws, error):
    """
    库的报错，比如连接超时
    :param ws:
    :param error: json格式，自行解析
    :return:
        """
    logger.error("error: " + str(error))

def on_close(ws, arg1, arg2):
    """
    Websocket关闭
    :param websocket.WebSocket ws:
    :return:
    """
    logger.info("ws close ..." + str(arg1) + "||||" + str(arg2))
    # ws.close()

def recoder_thread():
    logging.info("recode thread start")
    def run():
        
        uri = const.URI + "?sn=" + str(uuid.uuid1())
        ws_app = websocket.WebSocketApp(uri,
                                        on_open=on_record_open,  # 连接建立后的回调
                                        on_message=on_message,  # 接收消息的回调
                                        on_error=on_error,  # 库遇见错误的回调
                                        on_close=on_close)  # 关闭后的回调
        
        ws_app.run_forever()
        logging.info("recode thread end")
    
    threading.Thread(target=run).start()

    
if __name__ == "__main__":
    logging.basicConfig(format='[%(asctime)-15s] [%(funcName)s()][%(levelname)s] %(message)s')
    logger.setLevel(logging.DEBUG)  # 调整为logging.INFO，日志会少一点
    logger.info("begin")
    
    # websocket.enableTrace(True)
    uri = const.URI + "?sn=" + str(uuid.uuid1())
    logger.info("uri is "+ uri)
    ws_app = websocket.WebSocketApp(uri,
                                    on_open=on_record_open,  # 连接建立后的回调
                                    on_message=on_message,  # 接收消息的回调
                                    on_error=on_error,  # 库遇见错误的回调
                                    on_close=on_close)  # 关闭后的回调
    ws_app.run_forever()