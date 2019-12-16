# Arduino-elevator-simulator

Here, I tried to simulate an elevator with 4 floors. Each of the 4 buttons is used to call the elevator to that specific floor. The display shows the current floor where the elevator is and the 4 LEDs show which of the buttons are pressed until the elevator reaches them. The order of the elevator is nearest to furthest called floor. I also put a 'Panic' button that shuts down everything and resets the elevators position to first floor and a transistor to make sure that the display doesn't break from accidental high currents.

# Code

Loop function makes sure that the display is showing a correct number and it calls for pressedButton() function that checks if any of the buttons are pressed.
PressedButton function uses elapsedMillis library so that it can check for button pressing for exactly 100ms and I did that to use it instead of delay() because I needed to check the buttons as much as possible.
Moving function is to 'animate' movement of the elevator by first checking if it's going up or down and then a lot of checks for any changes.


If you have any questions you can contact me on:

Instagram: dejan_bogovac

Facebook: Dejan Bogovac

Gmail: deja.bogovac@gmail.com
