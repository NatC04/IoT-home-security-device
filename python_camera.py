import serial
import cv2
import time
import os

# Set the COM port and baud rate
ser = serial.Serial('COM5', 9600)  # <-- Change 'COM3' to your real port

# Set the directory where images will be saved
save_dir = "Captured_Images"

# Create the directory if it doesn't exist
if not os.path.exists(save_dir):
    os.makedirs(save_dir)

# Open the default camera (0 = built-in webcam)
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

print("Waiting for Arduino command...")

while True:
    if ser.in_waiting > 0:
        command = ser.readline().decode('utf-8').strip()
        print(f"Received command: {command}")

        if command == "TAKE_PICTURE":
            # Capture a frame
            ret, frame = cap.read()
            if ret:
                # Create a timestamp
                timestamp = time.strftime("%Y-%m-%d_%H-%M-%S")
                filename = f"image_{timestamp}.jpg"
                filepath = os.path.join(save_dir, filename)

                # Save the image
                cv2.imwrite(filepath, frame)
                print(f"Image saved at: {os.path.abspath(filepath)}")
            else:
                print("Error: Failed to capture image.")
