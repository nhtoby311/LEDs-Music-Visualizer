#include <FastLED.h>
// Inspried by many people

// LED LIGHTING SETUP
#define LED_PIN     6
#define NUM_LEDS    60 
#define BRIGHTNESS  80 // max 255
#define LED_TYPE    NEOPIXEL

CRGB leds[NUM_LEDS];

int reset = 3;
int strobe = 2;
int audio = 0;
int spectrumValue[7];
int band;
int audio_input = 0;
int freq = 1;                 //Which band is used

// STANDARD VISUALIZER VARIABLES
int midway = NUM_LEDS / 2; // CENTER MARK FROM DOUBLE LEVEL VISUALIZER
int loop_max = 0;
int k = 255; // COLOR WHEEL POSITION
int decay = 1; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT
long post_react = 0; // OLD SPIKE CONVERSION

// RAINBOW WAVE SETTINGS
int wheel_speed = 1;

void setup()
{
  // SPECTRUM SETUP
  pinMode(audio, INPUT);
  pinMode(strobe, OUTPUT);
  pinMode(reset, OUTPUT);
  
  digitalWrite(reset, LOW);
  digitalWrite(strobe, HIGH);
  
  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
  FastLED.show();

  // SERIAL AND INPUT SETUP
  //Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("\nListening...");
}

CRGB Scroll(int pos) {
  pos = abs(pos);
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  
  /*Serial.print(pos);
  Serial.print(" -> ");
  Serial.print("r: ");
  Serial.print(color.r);
  Serial.print("    g: ");
  Serial.print(color.g);
  Serial.print("    b: ");
  Serial.println(color.b);
  */
  return color;
}


void singleRainbow()
{
  for(int i = NUM_LEDS - 1; i >= 0; i--) {
    if (i < react)
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
    else
      leds[i] = CRGB(0, 0, 0);      
  }
  FastLED.show(); 
}

// FUNCTION TO MIRRORED VISUALIZER
void doubleRainbow()
{
  for(int i = NUM_LEDS - 1; i >= midway; i--) {
    if (i < react + midway) {
      //Serial.print(i);
      //Serial.print(" -> ");
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
      //Serial.print(i);
      //Serial.print(" -> ");
      leds[(midway - i) + midway] = Scroll((i * 256 / 50 + k) % 256);
    }
    else
      leds[i] = CRGB(0, 0, 0);
      leds[midway - react] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void readMSGEQ7()
// Function to read 7 band equalizers
{
  digitalWrite(reset, HIGH);
  digitalWrite(reset, LOW);
  for(band=0; band <7; band++)
  {
    digitalWrite(strobe, LOW); // strobe pin on the shield - kicks the IC up to the next band 
    delayMicroseconds(30); // 
    spectrumValue[band] = analogRead(audio); // store band reading

    if (spectrumValue[band] < 10)                          //read stuffs
 {
 Serial.print(" ");
 Serial.print(spectrumValue[band]);
 }
 else if (spectrumValue[band] < 100 )
 {
 Serial.print(" ");
 Serial.print(spectrumValue[band]);
 }
 else
 {
 Serial.print(" ");
 Serial.print(spectrumValue[band]);
 }
                                                  //end here
    
    digitalWrite(strobe, HIGH); 
  }
   Serial.println();
}

void convertSingle()
{
    audio_input = spectrumValue[freq];

  if (audio_input > 80)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 1023L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    /*Serial.print(audio_input);
    Serial.print(" -> ");
    Serial.println(pre_react);*/
  }
}

void convertDouble()
{
    audio_input = spectrumValue[freq];

  if (audio_input > 80 && freq == 0)               // if :on for 0th band
  {
    pre_react = ((long)midway * (long)audio_input) / 1023L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    /*Serial.print(audio_input);
    Serial.print(" -> ");
    Serial.println(pre_react);*/
  }
  
   if (audio_input > 130 && freq == 1)               // if :on for 1st band
  {
    pre_react = ((long)midway * (long)audio_input) / 1023L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

  }
  
}

// FUNCTION TO VISUALIZE WITH A SINGLE LEVEL
void singleLevel()
{
  readMSGEQ7();

  convertSingle();

  singleRainbow(); // APPLY COLOR

  k = k - wheel_speed; // SPEED OF COLOR WHEEL
  if (k < 0) // RESET COLOR WHEEL
    k = 255;

  // REMOVE LEDs
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
}

// FUNCTION TO VISUALIZE WITH MIRRORED LEVELS
void doubleLevel()
{
  readMSGEQ7();

  convertDouble();

  doubleRainbow();

  k = k - wheel_speed; // SPEED OF COLOR WHEEL
  if (k < 0) // RESET COLOR WHEEL
    k = 255;

  // REMOVE LEDs
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0){
      react--;
    }
  }
}

void loop()
{  
  //singleLevel();
  doubleLevel();
  //delayMicroseconds(10);
}
