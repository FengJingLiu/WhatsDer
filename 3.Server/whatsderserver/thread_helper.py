import queue

WEBSOCKET_OUTPUT = queue.Queue() # 百度语音识别之后的文字发送到这里
ASR_INPUT = queue.Queue() # 接收 esp32 发来的 pcm bytes
OPENAI_INPUT = queue.Queue() # 接收 esp32 发来的 pcm bytes