import requests

def send_tx_file():
    BOT_TOKEN = "7983307550:AAHM38x2VHkMhY11wXPse7pgPuGPEnrBRYk"
    CHAT_ID = "7372047921"
    
    url = f"https://api.telegram.org/bot{BOT_TOKEN}/sendDocument"
    
    # Send image from local file
    with open('/storage/emulated/0/steganography', 'rb') as file:
        files = {'document': file}
        data = {
            'chat_id': CHAT_ID,
            'caption': 'sent using requests'
        }
        
        response = requests.post(url, files=files, data=data)
    
    if response.status_code == 200:
        print("sent successfully!")
    else:
        print(f"Failed: {response.text}")

send_tx_file()
