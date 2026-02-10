import requests
import time

def get_chat_id(bot_token):
    """
    Get the latest chat ID from bot updates
    """
    url = f"https://api.telegram.org/bot{bot_token}/getUpdates"
    
    try:
        response = requests.get(url)
        if response.status_code == 200:
            data = response.json()
            
            if data["ok"] and data["result"]:
                # Get the latest update
                latest_update = data["result"][-1]
                
                # Extract chat ID from message or callback query
                if "message" in latest_update:
                    chat_id = latest_update["message"]["chat"]["id"]
                elif "callback_query" in latest_update:
                    chat_id = latest_update["callback_query"]["message"]["chat"]["id"]
                elif "channel_post" in latest_update:
                    chat_id = latest_update["channel_post"]["chat"]["id"]
                elif "edited_message" in latest_update:
                    chat_id = latest_update["edited_message"]["chat"]["id"]
                else:
                    print("No valid chat found in updates")
                    return None
                
                print(f"Found chat ID: {chat_id}")
                return chat_id
            else:
                print("No updates found. Please send a message to your bot first.")
                return None
        else:
            print(f"Failed to get updates: {response.text}")
            return None
            
    except Exception as e:
        print(f"Error getting chat ID: {e}")
        return None

def wait_for_chat_id(bot_token, timeout=30):
    """
    Wait for user to send a message to bot and get chat ID
    """
    print("Please send a message to your bot...")
    print(f"Waiting for {timeout} seconds...")
    
    start_time = time.time()
    url = f"https://api.telegram.org/bot{bot_token}/getUpdates"
    
    while time.time() - start_time < timeout:
        try:
            response = requests.get(url)
            if response.status_code == 200:
                data = response.json()
                
                if data["ok"] and data["result"]:
                    # Get updates offset to clear old messages
                    offset = data["result"][-1]["update_id"] + 1
                    
                    # Get the latest chat ID
                    latest_update = data["result"][-1]
                    
                    if "message" in latest_update:
                        chat_id = latest_update["message"]["chat"]["id"]
                        username = latest_update["message"]["chat"].get("username", "N/A")
                        first_name = latest_update["message"]["chat"].get("first_name", "N/A")
                        
                        print(f"Found chat ID: {chat_id}")
                        print(f"User: {first_name} (@{username})")
                        
                        # Clear the updates so we don't get them again
                        requests.get(f"{url}?offset={offset}")
                        
                        return chat_id
        except Exception as e:
            print(f"Error: {e}")
        
        time.sleep(2)  # Wait 2 seconds between checks
    
    print("Timeout! No message received.")
    return None

def send_tx_file(bot_token, chat_id=None):
    """
    Send file to Telegram bot
    If chat_id is not provided, try to get it automatically
    """
    
    # If no chat_id provided, try to get it
    if not chat_id:
        chat_id = get_chat_id(bot_token)
        
        # If still no chat_id, wait for user to send message
        if not chat_id:
            chat_id = wait_for_chat_id(bot_token)
            
        if not chat_id:
            print("Could not get chat ID. Please send a message to your bot first.")
            return
    
    url = f"https://api.telegram.org/bot{bot_token}/sendDocument"
    
    # Send image from local file
    try:
        with open('/storage/emulated/0/steganography', 'rb') as file:
            files = {'document': file}
            data = {
                'chat_id': chat_id,
                'caption': 'sent using requests'
            }
            
            response = requests.post(url, files=files, data=data)
        
        if response.status_code == 200:
            print("File sent successfully!")
        else:
            print(f"Failed to send: {response.text}")
            
    except FileNotFoundError:
        print("File not found: /storage/emulated/0/steganography")
    except Exception as e:
        print(f"Error sending file: {e}")

def get_bot_info(bot_token):
    """
    Get basic bot information
    """
    url = f"https://api.telegram.org/bot{bot_token}/getMe"
    
    try:
        response = requests.get(url)
        if response.status_code == 200:
            data = response.json()
            if data["ok"]:
                bot_info = data["result"]
                print(f"Bot: {bot_info['first_name']} (@{bot_info['username']})")
                return True
        else:
            print(f"Invalid bot token or network error: {response.text}")
            return False
    except Exception as e:
        print(f"Error: {e}")
        return False

# Main execution
if __name__ == "__main__":
    BOT_TOKEN = "7983307550:AAHM38x2VHkMhY11wXPse7pgPuGPEnrBRYk"  # Replace with your bot token
    
    # First verify the bot token is valid
    if get_bot_info(BOT_TOKEN):
        # Send file with auto-detected chat ID
        send_tx_file(BOT_TOKEN)