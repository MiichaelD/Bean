/*
Motion Detector

Light the LED if the Bean is moved or rotated.

The light will blink when the motion begins
  and again a second after the motion ends.

The colors of the light indicates which axies
  were triggered.

by: Chris Innanen (aka Nonsanity)
http://beantalk.punchthrough.com/t/simple-motion-detector/149
*/

// Insensitivity: 1 = too sensitive
//                2 = just right
//               10 = dropped/shaken/tipped
//               40 = thrown/shaken hard/flipped over
const int insensitivity = 2;

// 10 samples is about 1 second
const int NUM_SAMPLES = 10;
const int NUM_AXIS = 3;

int samples[NUM_AXIS][NUM_SAMPLES];
int total[NUM_AXIS];
int index = 0;
int lastAv = 0;

void setup(){
  Serial.begin(57600);

  for (int b = 0; b < NUM_AXIS; ++b) {
    for ( int a = 0; a < NUM_SAMPLES; ++a ) {
      samples[b][a] = 0;
    }
    total[b] = 0;
  }
}

void loop(){
  // remember the previous averages
  int avg[NUM_AXIS];
  for (int i = 0; i < NUM_AXIS; ++i) {
    avg[i] = abs(total[i] / NUM_SAMPLES);
  }

  // remove the oldest samples
  for (int i = 0; i < NUM_AXIS; ++i) {
    total[i] -= samples[i][index];
  }
  
  // save the new samples
  samples[0][index] = Bean.getAccelerationX();
  samples[1][index] = Bean.getAccelerationY();
  samples[2][index] = Bean.getAccelerationZ();

  // update the totals
  for (int i = 0; i < NUM_AXIS; ++i) {
    total[i] += samples[i][index];
  }

  // wrap around if we get to the end of the array
  if ( ++index >= NUM_SAMPLES )
    index = 0;

  // set r to 255 if one or more axis changes its average value
  uint8_t colors[NUM_AXIS];
  for (int i = 0; i < NUM_AXIS; ++i) {
    colors[i] = (abs(abs(total[i] / NUM_SAMPLES) - avg[i]) > insensitivity) ? 255 : 0;
  }

  // set the light if one of the sample averages did change
  Bean.setLed(colors[0], colors[1], colors[2]);
  printColors(colors);
  Bean.sleep(100);
}

void printColors(uint8_t colors[]) {
  bool isAnyAxisInAction = false;
  for (int i = 0 ; i < NUM_AXIS; ++i) {
    if (colors[i] > 0) {
      isAnyAxisInAction = true;
      break;
    }
  }

  if (!isAnyAxisInAction) {
    return;
  }
  
  Serial.print("Colors: ");
  Serial.print(colors[0]);
  Serial.print(", ");
  Serial.print(colors[1]);
  Serial.print(", ");
  Serial.println(colors[2]);
  
}
