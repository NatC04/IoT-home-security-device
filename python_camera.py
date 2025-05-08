import serial
import cv2
import time
import os
import smtplib
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart
from email import encoders


# Set the SMTP port and server
smtp_port = 587
smtp_server = "smtp.gmail.com"

email_from = "doorseciot@gmail.com"
email_to = input("Enter the email address to send alerts to: ")

pswd = "cxfecqqamsoyvlab"


def send_emails(filepath, email_to):
    # Create the email
    msg = MIMEMultipart()
    msg["From"] = email_from
    msg["Subject"] = subject
    msg["To"] = email_to

    # Add the body of the email
    body = MIMEText("This is a security alert. An image has been captured.")
    msg.attach(body)

    # Attach the image
    attachment = open(filepath, "rb")

    #Encode the attachment
    attachment_package = MIMEBase("application", "octet-stream")
    attachment_package.set_payload(attachment.read())
    encoders.encode_base64(attachment_package)
    attachment_package.add_header("Content-Disposition", f'attachment; filename="{os.path.basename(filepath)}"')
    msg.attach(attachment_package)
    
    text = msg.as_string()

    print("Connecting to SMTP server...")
    TIE_server = smtplib.SMTP(smtp_server, smtp_port)
    TIE_server.starttls()
    TIE_server.login(email_from, pswd)
    print("Successfully connected to SMTP server.")
    
    print("Sending email to:", email_to)
    TIE_server.sendmail(email_from, email_to, text)
    print("Email sent to:", email_to)

    TIE_server.quit()
    print("Disconnected from SMTP server.")


# Set the COM port and baud rate
serial = serial.Serial('COM5', 9600)

# Set the directory where images will be saved
save_dir = "Captured_Images"

# Create the directory if it doesn't exist
if not os.path.exists(save_dir):
    os.makedirs(save_dir)

# Open the default camera (0 = built-in webcam)
capture = cv2.VideoCapture(0)

if not capture.isOpened():
    print("Error: Could not open webcam.")
    exit()

print("Waiting for Arduino command...")

while True:
    if serial.in_waiting > 0:
        command = serial.readline().decode('utf-8').strip()
        print(f"Received command: {command}")

        if command == "TAKE_PICTURE":
            # Capture a frame
            ret, frame = capture.read()
            if ret:
                # Create a timestamp
                timestamp = time.strftime("%Y-%m-%d_%H-%M-%S")
                filename = f"image_{timestamp}.jpg"
                filepath = os.path.join(save_dir, filename)

                # Save the image
                cv2.imwrite(filepath, frame)
                print(f"Image saved at: {os.path.abspath(filepath)}")
                subject = "DoorSec security Alert at " + timestamp
                send_emails(filepath, email_to)
            else:
                print("Error: Failed to capture image.")