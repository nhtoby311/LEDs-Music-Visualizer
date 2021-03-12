# LEDs-Music-Visualizer
----------------------------------------
# Intro
LEDs Music Visualizer system consists of Arduino and Seven Band Graphic Equalizer - MSGEQ7 which is able to illustrates the audio spectrum from the input sound through the LEDs strips. 

![Gif]()

## Ingredients
  - Arduino UNO
  - MSGEQ7 
  - 5V Power Supply (≥ 4A)
  - **_Addressable_** LEDs (60LEDs)

## Procedures
1. Follow the circuit schematic 
2. The values of capacitors and resistors are based on [J Skoba](http://nuewire.com/info-archive/msgeq7-by-j-skoba/) diagram for MSGEQ7
3. Connect Arduino to the computer with USB cable
4. Adjust the pins values, in this case, A0 = audio, D2 = Strobe, D3 = reset and LED_PIN = 6
5. Upload the code 
6. **_Voilà!_**
--------------------------------
**ATTENTION**: If you smelt something's burned, it would probably too late! Throw everything away, realize you just waste ~20$, rethink about your life decisions, then buy the whole stuffs and start all over again.

**Note**: Add 22k resistor between the Audio In (one that goes to MSGEQ7) and the capacitor (10nF) 
