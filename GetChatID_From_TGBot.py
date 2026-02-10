import requests

def get_my_chat_id(bot_token):
    """
    Apna chat ID nikalne ka sabse aasan tarika
    Pehle apne bot ko koi bhi message bhejiye
    """
    url = f"https://api.telegram.org/bot{bot_token}/getUpdates"
    
    response = requests.get(url)
    data = response.json()
    
    if data.get('ok') and data.get('result'):
        for update in data['result']:
            if 'message' in update:
                chat_id = update['message']['chat']['id']
                username = update['message']['chat'].get('username', 'No username')
                print(f"Chat ID: {chat_id}")
                print(f"Username: @{username}")
                print(f"Name: {update['message']['chat'].get('first_name', '')} {update['message']['chat'].get('last_name', '')}")
                return chat_id
    
    print("Koi updates nahi mile. Pehle apne bot ko message bhejiye!")
    return None

# Usage
BOT_TOKEN = "7983307550:AAHM38x2VHkMhY11wXPse7pgPuGPEnrBRYk"
get_my_chat_id(BOT_TOKEN)