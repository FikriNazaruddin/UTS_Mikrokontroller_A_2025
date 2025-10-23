import serial
import time
import tkinter as tk
from tkinter import ttk

esp = serial.Serial('COM5', 115200, timeout=1)
time.sleep(2) 

def send_command(cmd):
    esp.write((cmd + '\n').encode())

def set_speed(val):
    try:
        speed = int(float(val))
        send_command(f"SPEED {speed}")
    except ValueError:
        pass

def motor_on():
    send_command("ON")

def motor_off():
    send_command("OFF")

def motor_rev():
    send_command("REV")

# === GUI Setup ===
root = tk.Tk()
root.title("Kontrol Motor ESP32")
root.geometry("300x250")

title = ttk.Label(root, text="Kontrol Motor DC ESP32", font=("Arial", 14, "bold"))
title.pack(pady=10)

# Tombol kontrol
frame_buttons = ttk.Frame(root)
frame_buttons.pack(pady=10)

btn_on = ttk.Button(frame_buttons, text="ON", command=motor_on)
btn_on.grid(row=0, column=0, padx=5)

btn_off = ttk.Button(frame_buttons, text="OFF", command=motor_off)
btn_off.grid(row=0, column=1, padx=5)

btn_rev = ttk.Button(frame_buttons, text="REV", command=motor_rev)
btn_rev.grid(row=0, column=2, padx=5)

# Slider kecepatan
lbl_speed = ttk.Label(root, text="Kecepatan Motor:")
lbl_speed.pack(pady=5)

speed_slider = ttk.Scale(root, from_=0, to=255, orient='horizontal', command=set_speed)
speed_slider.set(128)
speed_slider.pack(fill='x', padx=20)

lbl_value = ttk.Label(root, text="Nilai: 128")
lbl_value.pack()

def update_label(val):
    val_int = int(float(val))
    lbl_value.config(text=f"Nilai: {val_int}")

def on_slider_move(val):
    update_label(val)
    set_speed(val)

speed_slider.config(command=on_slider_move)

root.mainloop()

