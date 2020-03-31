#include "mbed.h"

Serial pc( USBTX, USBRX);
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
DigitalOut red(LED1);
DigitalOut green(LED2);
DigitalIn button(SW3);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);

int sample = 4000;
float ADCdata[4000];
int freq = 0;
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char table_last[10] = {0x3F+128, 0x06+128, 0x5B+128, 0x4F+128, 0x66+128, 0x6D+128, 0x7D+128, 0x07+128, 0x7F+128, 0x6F+128};

float ab(float i);

int main () {
    int i, j;
    int count = 0;
    int t1, t2;

    for (i = 0; i < sample; i++) {
        Aout = Ain;
        ADCdata[i] = Ain;
        wait(1./sample);
    }
    
    for (i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.1);
    }
    
    for (i = 400, t1 = 400; i < sample; i++) {
        if (abs(ADCdata[t1] - 0.5) > abs(ADCdata[i] - 0.5)) {
            t1 = i;
          //  pc.printf("t1 = %d\r\n", t1);
           // pc.printf("data: %f\r\n", ADCdata[i]);
        }
    }
            for (j = t1+21, t2 = t1+1; j < t1+45; j++) {
                if ((ADCdata[j]  > 0.4 && ADCdata[j] < 0.6)) {
                    if (ab(ADCdata[t1] - ADCdata[t2]) > ab(ADCdata[t1] - ADCdata[j])) {
                        t2 = j;
      //                  pc.printf("data: %f\r\n", ADCdata[j]);
        //                pc.printf("t2 = %d\r\n", t2);
                    }
                }
            }

    freq = sample/(t2-t1);
    //pc.printf("%d\r\n", freq);
    
    while(1) {
        if (!button) {
            green = 1;
            red = 0;
//            pc.printf("%d\n", freq);
            if (freq >= 100) {
                display = table[freq/100];
                wait(1);
            }
            display = table[(freq - freq/100*100)/10];
            wait(1);
            display = table_last[freq - freq/10*10];
            wait(1);
        }
        else {
            green = 0;
            red = 1;
            display = 0x00;
        }
    }
}

float ab(float i)
{
    if (i >= 0) return i;
    else return (-i);
}