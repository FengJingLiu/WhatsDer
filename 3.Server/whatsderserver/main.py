from typing import Union

from fastapi import FastAPI
from pydantic import BaseModel
from .openai_api import Assistant

app = FastAPI()
ast = Assistant()

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

@app.post("/assistant/add")
async def add_message(msg: Message):
    await ast.add_message(msg.text)
    await ast.run()
    return await ast.get_message_assistant_role_json()

@app.post("/assistant/add")
async def add_message(msg: Message):
    await ast.add_message(msg.text)
    await ast.run()
    return await ast.get_message_assistant_role_json()