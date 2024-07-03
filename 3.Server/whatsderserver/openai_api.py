import time
from openai import OpenAI, AssistantEventHandler
from typing_extensions import override
from openai.types.beta.threads import Text, TextDelta
import asyncio
import json
from thread_helper import WEBSOCKET_OUTPUT
import queue
import logging

logger = logging.getLogger()
# logger.setLevel(logging.INFO)  
logger.setLevel(logging.DEBUG)  
OPENAI_INPUT = queue.Queue()

API_SECRET_KEY = "sk-zk29e86b67dd7244cd85028752b2a6c3332666d8fc626e20";
BASE_URL = "https://api.zhizengzeng.com/v1"; #智增增的base_url


class EventHandler(AssistantEventHandler):
    @override
    def on_text_created(self, text: Text) -> None:
        print(f"\nassistant > ", end="", flush=True)
        data = {
            "type" : "OPENAI_START",
            "result" : ""
        }
        WEBSOCKET_OUTPUT.put(json.dumps(data))

    @override
    def on_text_delta(self, delta: TextDelta, snapshot: Text):
        print(delta.value, end="", flush=True)
        data = {
            "type" : "OPENAI_DATA",
            "result" : delta.value
        }
        WEBSOCKET_OUTPUT.put(json.dumps(data))

    @override
    def on_text_done(self, text: Text):
        print(f"text done {text}", end="", flush=True)
        data = {
            "type" : "OPENAI_END",
            "result" : ""
        }
        WEBSOCKET_OUTPUT.put(json.dumps(data))


def chat_completions(question: str, model: str = "gpt-3.5-turbo") -> str:
    client = OpenAI(api_key=API_SECRET_KEY, base_url=BASE_URL)

    rsp = client.chat.completions.create(
        model=model,
        messages=[{"role": "assistant", "content": "用中文回答我的问题"},
                    {"role": "user", "content": question}],
    )
    if rsp.choices[0].message.content is not None:
        return rsp.choices[0].message.content
    else:
        return ''


class Assistant:
    model: str = "gpt-3.5-turbo"
    assistant = None
    thread = None
    run = None
    message_data = None
    message_new = None
    run = None
    client = OpenAI(api_key=API_SECRET_KEY, base_url=BASE_URL)

    def __init__(self, model: str = "gpt-3.5-turbo"):
        self.model = model
        self.get_assistant()    
        if self.assistant is None:
            self.create_assistant()
        self.create_thread()        

    def create_assistant(self):
        self.assistant = self.client.beta.assistants.create(
            name="baby teacher",
            instructions="我是一个三岁婴儿，并且输入经过stt转换可能存在谐音或者表述不准确，你的输出应该让我能听懂。",
            model=self.model,
        )

    # assistant
    def get_assistant(self):
        logger.debug("start")
        assistant_list = self.client.beta.assistants.list()
        if assistant_list.data[0] is not None:
            self.assistant = assistant_list.data[0]

    def create_thread(self):
        logger.debug("start")
        self.thread = self.client.beta.threads.create()

    def add_message(self, message):
        logger.debug("start")
        self.client.beta.threads.messages.create(
            thread_id=self.thread.id, # 助手的会话id要从上一步获取得到
            role="user",
            content=message
        )

    # run a assistant
    def run_retrieve(self):
        self.run = self.client.beta.threads.runs.create(
            thread_id=self.thread.id,  # 助手的会话id要从上一步获取得到
            assistant_id=self.assistant.id, # 助手的id要从上一步获取得到
        )
        retry_time = 10
        while True:
            retry_time -= 1
            self.run = self.client.beta.threads.runs.retrieve(
            thread_id=self.thread.id,
            run_id=self.run.id
            )
            if self.run.status == 'completed':
                self.list_messages()
                break
            if retry_time == 0:
                break
            time.sleep(1)
        

    # 获取助手的回答
    # Once the Run completes, you can retrieve the Messages added by the Assistant to the Thread.
    def list_messages(self):
        messages = self.client.beta.threads.messages.list(
                thread_id=self.thread.id
            )
        self.message_diff(messages.data)
        self.message_data = messages.data
        # return messages.data[0].content[0].text.value

    def get_messages(self):
        return self.message_data
    
    def get_new_message(self):
        return self.message_new
    
    def message_diff(self, new_message_data):
        if (self.message_data is not None and len(new_message_data) <= len(self.message_data)) or len(new_message_data) == 0:
            return
        if self.message_new is None:
            diff = len(new_message_data)
        else:
            diff = len(new_message_data) - len(self.message_data)
        for msg in new_message_data[0:diff]:
            if msg.role == 'assistant':
                self.message_new = msg
                break;
    
    def stream_run(self, message):
        logger.debug("start")
        self.add_message(message)
        with self.client.beta.threads.runs.stream(
            thread_id=self.thread.id,
            assistant_id=self.assistant.id,
            event_handler=EventHandler(),
            # event_handler=EventHandler(websockey),
            ) as stream:
            stream.until_done()

if __name__ == '__main__':
    ast = Assistant()
    asyncio.run(ast.async_init())
    asyncio.run(ast.add_message("红色加黄色会变成什么颜色"))
    asyncio.run(ast.stream_run(None))
    ast.add_message("红色加黄色会变成什么颜色")
    ast.stream_run()
    # msg = ast.get_messages()
    msg = ast.stream_over_text()
    # ast.get_message_assistant_role_json()
    ast.add_message("红色加黄色会变成什么颜色?")
    ast.add_message("长沙是哪里啊？")
    ast.add_message("湘潭又是哪里？")
    ast.stream_iterating_run()
    ast.stream_over_text()

    print(msg)
    
    