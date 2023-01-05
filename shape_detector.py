import cv2
import numpy as np
from serial import Serial
import time

arduino = Serial('/dev/ttyUSB0', 9600, timeout=1)



sig = 0

cap = cv2.VideoCapture(2)
def writeonimg(frame,text):
    font                   = cv2.FONT_HERSHEY_SIMPLEX
    bottomLeftCornerOfText = (10,50)
    fontScale              = 1
    fontColor              = (0,255,0)
    lineType               = 3

    cv2.putText(frame,text,
        bottomLeftCornerOfText,
        font,
        fontScale,
        fontColor,
        lineType)


while not cv2.waitKey(1)==ord('k') :
    try:
            impo , frame = cap.read()
            blurred_frame = cv2.GaussianBlur(frame, (5, 5), 0)
            hsv = cv2.cvtColor(blurred_frame, cv2.COLOR_BGR2HSV)
            lower_blue = np.array([30, 110, 0])
            upper_blue = np.array([124, 255, 255])
            mask = cv2.inRange(hsv, lower_blue, upper_blue)
            contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)
            contours_sizes= [(cv2.contourArea(cnt), cnt) for cnt in contours]
            biggest_contour = max(contours_sizes, key=lambda x: x[0])[1]


            approx = cv2.approxPolyDP(biggest_contour , 0.03*cv2.arcLength (biggest_contour,True),True)

            cv2.drawContours(frame, [approx] ,-1, (0,255,0), 2)

                #print ( len(approx))



            if len(approx) == 3:

                writeonimg(frame,"triangle "+str(len(approx)))
                sig=2


            elif len(approx) == 4:
                l1=[approx[3,0,0]-approx[0,0,0],approx[3,0,1]-approx[0,0,1]]
                l2=[approx[1,0,0]-approx[0,0,0],approx[1,0,1]-approx[0,0,1]]
                l3=[approx[2,0,0]-approx[1,0,0],approx[2,0,1]-approx[1,0,1]]
                l4=[approx[2,0,0]-approx[3,0,0],approx[2,0,1]-approx[3,0,1]]
                lenn=[np.sqrt(l1[0]**2+l1[1]**2),
                      np.sqrt(l2[0]**2+l2[1]**2),
                      np.sqrt(l3[0]**2+l3[1]**2),
                      np.sqrt(l4[0]**2+l4[1]**2)]
                rat=max(lenn)-min(lenn)
                if rat<50:
                    writeonimg(frame,"square "+str(len(approx)))
                    sig=3
                    len(approx)-1
                else:
                    writeonimg(frame,"trapezoidal "+str(len(approx)))
                    sig=4



            elif len(approx) == 5:
                writeonimg(frame,"pentagon "+str(len(approx)))
                sig=5



            elif len(approx) == 8:
                writeonimg(frame,"circle "+str(len(approx)))
                sig=8


            cv2.imshow("frame", frame)
        #cv2.imshow("blurrd", blurred_frame)
        #cv2.imshow("hsv", hsv)
            cv2.imshow("Mask", mask)
            if cv2.waitKey(1) == ord('s'):
            	arduino.write(bytes([sig]))
           
    except:
            time.sleep(0.2)