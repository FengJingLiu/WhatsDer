import time
from openai import OpenAI

API_SECRET_KEY = "sk-zk29e86b67dd7244cd85028752b2a6c3332666d8fc626e20";
BASE_URL = "https://api.zhizengzeng.com/v1"; #智增增的base_url


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
        assistant_list = self.client.beta.assistants.list()
        if assistant_list.data[0] is not None:
            self.assistant = assistant_list.data[0]

    def create_thread(self):
        self.thread = self.client.beta.threads.create()

    def add_message(self, message):
        message = self.client.beta.threads.messages.create(
            thread_id=self.thread.id, # 助手的会话id要从上一步获取得到
            role="user",
            content=message
        )

    # run a assistant
    def run(self):
        run = self.client.beta.threads.runs.create(
            thread_id=self.thread.id,  # 助手的会话id要从上一步获取得到
            assistant_id=self.assistant.id, # 助手的id要从上一步获取得到
        )
        retry_time = 10
        while True:
            retry_time -= 1
            self.run = self.client.beta.threads.runs.retrieve(
            thread_id=self.thread.id,
            run_id=run.id
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
    
    def message_diff(self, new_message_data):
        if len(new_message_data) <= len(self.message_data) or len(new_message_data) == 0:
            return
        
        diff = len(new_message_data) - len(self.message_data)
        self.message_new = new_message_data[0:diff]

    def get_message_assistant_role_json(self):
        return self.message_data[0].content[0].json()

if __name__ == '__main__':
    ast = Assistant()
    ast.add_message("讲个笑话给我听")
    ast.add_message("大概200个字左右的")
    ast.add_message("和童话有关的")
    ast.run()
    msg = ast.get_messages()
    ast.get_message_assistant_role_json()
    print(msg)
    
    