/* Global Variables */
unsigned int sec = 0, minute=0, hour=0, day=0;
unsigned int timer_day=3;
unsigned int water_timer_sec = 25, water_cycle=8;
bool avocadotime=false;

void setup() {
  // put your setup code here, to run once:

/* Set pin modes */
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW); // Turn of water

// initialize timer1 
  noInterrupts();           // disable all interrupts

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 62500;            // compare match register 16MHz/256/1Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  interrupts();             // enable all interrupts
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  if(avocadotime) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 1);   // toggle LED pin  
  }  

  sec = sec+1;
  if(sec == 60) {
    minute = minute+1;
    sec = 0;
  }

  if(minute == 60) {
    hour = hour+1;
    minute = 0;
  }

  if(hour == 24) {
    day = day+1;
    hour = 0;
  }
  
  if(day == timer_day) {
    sec = 0;
    minute = 0;
    hour = 0;
    day = 0;

    avocadotime = true;
  }  
}

void loop() {
  int i;

  if(avocadotime) {
    for (i=0; i<water_cycle; i++) {
      
      digitalWrite(A0, HIGH);   // turn on water
      delay(water_timer_sec*1000);
      
      digitalWrite(A0, LOW);   // turn off water and let circuits cool
      delay(2*water_timer_sec*1000);
    }

    avocadotime = false;
  }
}
