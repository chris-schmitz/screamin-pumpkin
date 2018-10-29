import board
import time
import touchio
import digitalio
import neopixel
import neopixel

# setup feather trigger
trigger = digitalio.DigitalInOut(board.D0)
trigger.direction = digitalio.Direction.OUTPUT

# setup neopxixel
pixelCount = 30
strip = neopixel.NeoPixel(board.A2, pixelCount, brightness=.2)
strip.fill((255, 0, 0))
strip.show()


# Touch state machine
touch = touchio.TouchIn(board.A1)
touch.threshold += 200
touchCheckInterval = 0.01
previousTouchMilliseconds = 0

# LED state machine
ledTransitionInterval = 0.1
ledBrightness = 1
ledBrightnessDirection = 'down'

previousLedMilliseconds = 0


def idleLights(ledBrightness, ledBrightnessDirection):

    if ledBrightness < 0.2:
        ledBrightnessDirection = 'up'
    if ledBrightness >= 1:
        ledBrightnessDirection = 'down'

    if ledBrightnessDirection == 'up':
        ledBrightness += 0.1
    else:
        ledBrightness -= 0.1

    strip.fill((255, 0, 0))
    strip.brightness = ledBrightness
    strip.show()

    return ledBrightness, ledBrightnessDirection


def manageTouch():
    print('checking touch')
    strip.show()
    if touch.value:
        print("A1 touched!")
        trigger.value = True
        time.sleep(0.01)
        trigger.value = False

        for i in range(11):
            if i % 2 == 0:
                strip.fill((255, 255, 255))
            else:
                strip.fill((148, 0, 211))
            strip.show()
            time.sleep(0.001)

        time.sleep(1)


while True:

    currentMilliseconds = time.monotonic()

    if currentMilliseconds - previousLedMilliseconds > ledTransitionInterval:
        previousLedMilliseconds = currentMilliseconds
        ledBrightness, ledBrightnessDirection = idleLights(
            ledBrightness, ledBrightnessDirection)

    if currentMilliseconds - previousTouchMilliseconds > touchCheckInterval:
        previousTouchMilliseconds = currentMilliseconds
        manageTouch()
