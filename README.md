# LEDs-Music-Visualizer
----------------------------------------
# Intro
LEDs Music Visualizer system consists of Arduino and Seven Band Graphic Equalizer - MSGEQ7 which able to input the sound, then illustrates the audio spectrum through the LEDs strips. 

## Ingredients
  - Arduino UNO
  - MSGEQ7 
  - 5V Power Supply (≥ 4A)
  - **_Addressable_** LEDs (60LEDs)

## Procedures
1. Follow the circuit schematic 
2. The values of capacitors and resistors are based on **_J Skoba_** diagram for MSGEQ7
3. Connect Arduino to the computer with USB cable
4. Adjust the pins values, in this case, A0 = audio, D2 = Strobe, D3 = reset and LED_PIN = 6
5. Upload the code 
6. **_Voilà!_**
--------------------------------
**ATTENTION**: If you smelt something's burned, it would probably too late! Throw everything away, grab some beers, rethink about your life, then buy the whole stuffs and start all over again.
