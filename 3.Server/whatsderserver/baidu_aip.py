import requests
import json
import urllib.parse


""" 你的 APPID AK SK """
APP_ID = '75882674'
API_KEY = 'C7UHcBu62dTzrJh7S64SSlap'
SECRET_KEY = 'tyySegR96kdrd4UkOTEBUwGPOFgYmAGl'

def get_test_pcm_content():
    with open("C:\\Users\\sdsaf\\Downloads\\text2audio.pcm", 'rb') as fp:
        binary_content = fp.read()
        return binary_content

def urlencode_string(string, encoding='utf-8'):
    """
    将字符串转换为URL编码

    :param string: 要转换的字符串
    :param encoding: 编码方式，默认使用 'utf-8'
    :return: 转换后的URL编码字符串
    """
    try:
        encoded_string = urllib.parse.quote(string, encoding=encoding)
        return encoded_string
    except Exception as e:
        print(f"An error occurred while encoding the string: {e}")
        return None
    
""" 语音转文字 """
def baidu_stt(pcm_data: bytes) -> str:
        
    url = "https://vop.baidu.com/pro_api"
    
    params = {
    'dev_pid': '80001',
    'cuid': 'C7UHcBu62dTzrJh7S64SSlap',
    'token': get_access_token()
    }

    headers = {
        'Content-Type': 'audio/pcm;rate=16000',
    }

    response = requests.request("POST", url, headers=headers, params=params, data=pcm_data)
    
    response_json = json.loads(response.text)
    err_no = response_json["err_no"]
    result = response_json["result"]
    
    if err_no == 0:
        # 处理结果
        if result and result[0]:
            return response_json['result'][0]
        else:
            return ""
    else:
        return ""
    

def baidu_tts(text_data: str) -> bytes:
    url = "https://tsn.baidu.com/text2audio"
    
    payload = 'tex=' + urlencode_string(text_data) + '&tok='+ get_access_token() +'&cuid=C7UHcBu62dTzrJh7S64SSlap&ctp=1&lan=zh&spd=5&pit=5&vol=5&per=1&aue=3'
    headers = {
        'Content-Type': 'application/x-www-form-urlencoded',
        'Accept': '*/*'
    }
    
    response = requests.request("POST", url, headers=headers, data=payload)
    
    return response.content

def save_binary_to_file(file_path, binary_data):
    """
    将二进制数据保存为文件

    :param file_path: 要保存的文件路径
    :param binary_data: 要保存的二进制数据
    """
    try:
        with open(file_path, 'wb') as file:
            file.write(binary_data)
        print(f"Binary data saved to {file_path}")
    except Exception as e:
        print(f"An error occurred while saving the file: {e}")


def get_access_token():
    """
    使用 AK，SK 生成鉴权签名（Access Token）
    :return: access_token，或是None(如果错误)
    """
    url = "https://aip.baidubce.com/oauth/2.0/token"
    params = {"grant_type": "client_credentials", "client_id": API_KEY, "client_secret": SECRET_KEY}
    return str(requests.post(url, params=params).json().get("access_token"))

if __name__ == '__main__':
    # baidu_stt(get_test_pcm_content())
    rsp = baidu_tts("长沙是中国湖南省的省会城市，位于中国中部，它是一个历史悠久，文化丰富，风景优美的城市。")
    save_binary_to_file("tts_rsp_test.mp3", rsp)
    
