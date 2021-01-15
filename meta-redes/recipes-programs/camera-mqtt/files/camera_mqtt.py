#!/usr/bin/env python3

import cv2
import time
import sys
import numpy as np
import paho.mqtt.client as mqtt


def connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected success")
    else:
        print(f"Connected fail with code {rc}")
        sys.exit()


def findBall(mask):
    x = 0
    y = 0
    radius = 0
    center = (0, 0)

    contours, _ = cv2.findContours(
        mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(c)
        M = cv2.moments(c)

        if radius > 10:
            center = (int(M["m10"] / M["m00"]), int(M["m01"] / M["m00"]))

    return x, y, radius, center


def move(x, radius):
    global client
    if x > 350:
        print('MOVE LEFT')
        client.publish("piCam", "2")
    elif x < 300:
        print('MOVE RIGHT')
        client.publish("piCam", "1")
    else:
        print('CENTERED')
        client.publish("piCam", "3")


# Start and configure MQTT
client = mqtt.Client("raspberry1")
client.username_pw_set("python_user", "python1234")
client.on_connect = connect

try:
    client.connect("127.0.0.1", 1883)
except:
    print("MQTT Connect Error")
    sys.exit()

client.loop_start()

# Configure OpenCV Camre
cap = cv2.VideoCapture(0)

# Configure mask values
l_b = np.array([40, 175, 80])
u_b = np.array([255, 255, 255])

while cap.isOpened():
    ret, frame = cap.read()

    if ret:
        frame_hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        mask = cv2.inRange(frame_hsv, l_b, u_b)

        x, y, radius, center = findBall(mask)

        cv2.circle(frame, (int(x), int(y)), int(radius),
                   (0, 255, 255), 2)
        cv2.circle(frame, center, 5, (0, 0, 255), -1)

        #cv2.imshow('frame', frame)

        move(x, radius)
        print("X position = {}".format(x))

        if cv2.waitKey(1) & 0xFF == 27:
            break
    else:
        break

    time.sleep(0.5)

cap.release()
cv2.destroyAllWindows()
