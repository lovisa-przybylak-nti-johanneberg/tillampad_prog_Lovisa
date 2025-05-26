import serial
import time
import os
import psutil


arduino = serial.Serial(port='COM4', baudrate=115200, timeout=0.1)
time.sleep(2)


prev = ""
notepad_open = False
discord_open = False
app_open = False

def kill_process(name):
    for proc in psutil.process_iter(['pid', 'name']):
        if name.lower() in proc.info['name'].lower():
            proc.kill()

while True:
    line = arduino.readline().decode('utf-8').strip()

    if line and line != prev:
        if line == "1":
            if notepad_open:
                kill_process("notepad.exe")
                notepad_open = False
            else:
                os.startfile("notepad.exe")
                notepad_open = True

        elif line == "2":
            print('hej 2');
            if discord_open:
                kill_process("Discord.exe")
                discord_open = False
            else:
                os.startfile("C:/Users/lovisa.przybylak/AppData/Local/Discord/app-1.0.9192/Discord.exe")
                discord_open = True
        
        elif line == "3":
            if app_open:
                kill_process("chrome.exe")
                app_open = False
            else:
                os.startfile("C:/Program Files/Google/Chrome/Application/chrome.exe")
                app_open = True
    prev = line
    ##time.sleep(0.1)