// Written by an0n9m
#include <mega168a.h>

// I2C Bus functions
#include <i2c.h>

// DS1307 Real Time Clock functions
#include <ds1307.h>

// Alphanumeric LCD functions
#asm
.equ __lcd_port=0x18 ;PORTB
#endasm
#include <alcd.h>

#include <delay.h>

// Declare your global variables here
unsigned char a,b,c,d,e,f,g,h,x,y,z;
unsigned char hour,min,sec;   
unsigned char date, day,month,year;   
unsigned int inf=0;     // Var for LCDù
bit alm=0;              // Alarm ON/OFF
bit timer=0;            // Timer ON/OFF
unsigned char a_hour,a_min;    // Var for alarm
unsigned int tmp=0;     // Var for code optimization
unsigned int t_hour, t_min, t_sec;     // Var for timer 
unsigned int s_hour, s_min, s_sec;     // Var for stopwatch         
unsigned int s_tmp=0;       // Temporal stopwatch var
void main(void)
{

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

// Input/Output Ports initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(1<<PORTC6) | (1<<PORTC5) | (1<<PORTC4) | (1<<PORTC3) | (1<<PORTC2) | (1<<PORTC1) | (1<<PORTC0);

// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
// State: Bit7=P Bit6=P Bit5=P Bit4=P Bit3=P Bit2=P Bit1=P Bit0=P 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
ADCSRB=(0<<ACME);
// Digital input buffer on AIN0: On
// Digital input buffer on AIN1: On
DIDR1=(0<<AIN0D) | (0<<AIN1D);

// Bit-Banged I2C Bus initialization
// I2C Port: PORTD
// I2C SDA bit: 0
// I2C SCL bit: 1
// Bit Rate: 100 kHz
// Note: I2C settings are specified in the
// Project|Configure|C Compiler|Libraries|I2C menu.
i2c_init();


// DS1307 Real Time Clock initialization
// Square wave output on pin SQW/OUT: On
// Square wave frequency: 1Hz
    //rtc_set_date(01,06,11,24);   //Date setup: (num of date,day,month,year)
    //rtc_set_time(15,50,00);   //Tiem setup: (hour,minute,second)


// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 0
// RD - PORTB Bit 1
// EN - PORTB Bit 2
// D4 - PORTB Bit 4
// D5 - PORTB Bit 5
// D6 - PORTB Bit 6
// D7 - PORTB Bit 7
// Characters/line: 16
lcd_init(16);
a=0;b=0;c=0;d=0;e=0;f=0;g=0;h=0;x=0;y=0;z=0;

    while (1)
    {
    // Place your code here
    
    
    rtc_get_time(&hour,&min,&sec);      //real time read
    rtc_get_date(&date,&day,&month,&year);  //real date read    
    if (PINC.0==0) // Set button press check / SET Time/ Set Data/ Set ALARM /on-of alarmù / Timer / Stopwatch
    {
        delay_ms(300); // delay (for choosing)
        inf++; // Min increment
    }               
    delay_ms(50); // For code optimization (reduced cpu load)
    if (inf==0)
    {
        if (sec%10-tmp==0)continue;
        //lcd_clear(); 
        tmp=sec%10;
        lcd_gotoxy(0,0);
        //lcd_putsf("Time is=");
        lcd_putchar(hour/10+0x30);
        lcd_putchar(hour%10+0x30);
        lcd_putchar(':');
        lcd_putchar(min/10+0x30);
        lcd_putchar(min%10+0x30);
        lcd_putchar(':');
        lcd_putchar(sec/10+0x30);
        lcd_putchar(sec%10+0x30); 
        lcd_putsf("      ");
        lcd_gotoxy(0,1);
        lcd_putchar(day/10+0x30);
        lcd_putchar(day%10+0x30);       
        lcd_putchar('/');
        lcd_putchar(month/10+0x30);
        lcd_putchar(month%10+0x30);
        lcd_putchar('/');
        lcd_putchar(year/10+0x30);
        lcd_putchar(year%10+0x30); 
         lcd_putsf("      ");
         // Check Alarm
        if ((alm==1)&(a_min==min)&(a_hour==hour))
        PORTD.2=1;
        // Turning off Alarm
        if (PINC.5==0)
        {
        alm=0;
        PORTD.2=0;
        }
    }
    else if (inf==1)
    {
        
       lcd_clear();
// Display "Set Time" on the LCD
lcd_putsf("Set Time");
lcd_gotoxy(0,1); // Move the cursor to the second line
lcd_putchar(hour/10 + 0x30); // Display tens digit of the hour
lcd_putchar(hour%10 + 0x30); // Display units digit of the hour
lcd_putchar(':'); // Display colon separator
lcd_putchar(min/10 + 0x30); // Display tens digit of the minute
lcd_putchar(min%10 + 0x30); // Display units digit of the minute

// Check if the "Set Time" button is pressed
if (PINC.1 == 0) {
    delay_ms(300); // Debounce delay
    min++; // Increment the minute
}

// Check if the "Minute++" button is pressed
if (PINC.2 == 0) {
    delay_ms(300); // Debounce delay
    min--; // Decrement the minute
}

// Check if the "Minute--" button is pressed
if (PINC.3 == 0) {
    delay_ms(300); // Debounce delay
    hour++; // Increment the hour
}

// Check if the "Hour+" button is pressed
if (PINC.4 == 0) {
    delay_ms(300); // Debounce delay
    hour--; // Decrement the hour
}

// Reset minute if it exceeds 59 or underflows
if (min > 60 || min == 255) {
    min = 0;
}

// Reset hour if it exceeds 23 or underflows
if (hour > 24 || hour == 255) {
    hour = 0;
}

rtc_set_time(hour, min, sec); // Update the time in RTC

// Handle setting date if "inf" is 2
if (inf == 2) {
    lcd_clear();
    lcd_putsf("Set Data"); // Display "Set Data" on the LCD
    lcd_gotoxy(0,1);
    lcd_putchar(day/10 + 0x30); // Display tens digit of the day
    lcd_putchar(day%10 + 0x30); // Display units digit of the day
    lcd_putchar('-'); // Display dash separator
    lcd_putchar(month/10 + 0x30); // Display tens digit of the month
    lcd_putchar(month%10 + 0x30); // Display units digit of the month
    lcd_putchar('-');
    lcd_putchar(year/10 + 0x30); // Display tens digit of the year
    lcd_putchar(year%10 + 0x30); // Display units digit of the year

    // Check if the "Day++" button is pressed
    if (PINC.1 == 0) {
        delay_ms(300); // Debounce delay
        day++;
    }

    // Check if the "Month++" button is pressed
    if (PINC.2 == 0) {
        delay_ms(300); // Debounce delay
        month++;
    }

    // Check if the "Year++" button is pressed
    if (PINC.3 == 0) {
        delay_ms(300); // Debounce delay
        year++;
    }

    // Reset date values if they exceed limits
    if (day == 32) day = 0;
    if (month == 13) month = 0;
    if (year == 99) year = 0;

    rtc_set_date(date, day, month, year); // Update the date in RTC
}

// Handle setting alarm if "inf" is 3
else if (inf == 3) {
    lcd_clear();
    lcd_putsf("Set ALARM"); // Display "Set ALARM" on the LCD
    lcd_gotoxy(0,1);
    lcd_putchar(a_hour/10 + 0x30); // Display tens digit of the alarm hour
    lcd_putchar(a_hour%10 + 0x30); // Display units digit of the alarm hour
    lcd_putchar(':');
    lcd_putchar(a_min/10 + 0x30); // Display tens digit of the alarm minute
    lcd_putchar(a_min%10 + 0x30); // Display units digit of the alarm minute
    delay_ms(100);

    // Increment alarm minute if button is pressed
    if (PINC.1 == 0) {
        delay_ms(300); // Debounce delay
        a_min++;
    }

    // Decrement alarm minute if button is pressed
    if (PINC.2 == 0) {
        delay_ms(300); // Debounce delay
        a_min--;
    }

    // Increment alarm hour if button is pressed
    if (PINC.3 == 0) {
        delay_ms(300); // Debounce delay
        a_hour++;
    }

    // Decrement alarm hour if button is pressed
    if (PINC.4 == 0) {
        delay_ms(300); // Debounce delay
        a_hour--;
    }

    // Reset alarm minute if it exceeds limits
    if (a_min == 60 || a_min == 255) {
        a_min = 0;
    }

    // Reset alarm hour if it exceeds limits
    if (a_hour == 24 || a_hour == 255) {
        a_hour = 0;
    }
}

// Handle alarm ON/OFF toggle if "inf" is 4
else if (inf == 4) {
    lcd_clear();
    lcd_putsf("ON/OFF Alarm"); // Display "ON/OFF Alarm" on the LCD
    if (PINC.5 == 0) { // Toggle alarm ON/OFF
        delay_ms(300); // Debounce delay
        alm++;
    }
    lcd_gotoxy(0,1);
    if (alm == 0)
        lcd_putsf("Alarm OFF");
    if (alm == 1)
        lcd_putsf("Alarm ON");
}
        if (PINC.5==0) // Toggle alarm on/off
{
    delay_ms(300); // Debounce delay
    alm++;
}
lcd_gotoxy(0,1);
if (alm==0)
    lcd_putsf("Alarm OFF");
if (alm==1)
    lcd_putsf("Alarm ON");
}
else if(inf==5){ // Timer code
    lcd_clear();
    lcd_putsf("Timer");
    lcd_gotoxy(0,1); 
    lcd_putchar(t_hour/10+0x30);
    lcd_putchar(t_hour%10+0x30);
    lcd_putchar(':');
    lcd_putchar(t_min/10+0x30);
    lcd_putchar(t_min%10+0x30);    
    lcd_putchar(':');
    lcd_putchar(t_sec/10+0x30);
    lcd_putchar(t_sec%10+0x30);           
    if (PINC.1==0) // Increase minutes
    {
        delay_ms(300); // Debounce delay
        t_min++;
    }
    if (PINC.2==0) // Decrease minutes
    {
        delay_ms(300); // Debounce delay
        t_min--;
    }
    if (PINC.3==0) // Increase hours
    {
        delay_ms(300); // Debounce delay
        t_hour++;
    }
    if (PINC.4==0) // Increase seconds
    {
        delay_ms(250); // Debounce delay
        t_sec++;
    }
    if (PINC.5==0){
        if(PORTD.3==1)PORTD=0; // Stop alarm
        if(t_sec==0&&t_min==0&&t_hour==0)continue; // Prevent timer reset
        timer=1;
        while(timer!=0){
            lcd_clear();
            lcd_putsf("Timer"); // Display timer
            lcd_gotoxy(0,1); 
            lcd_putchar(t_hour/10+0x30);
            lcd_putchar(t_hour%10+0x30);
            lcd_putchar(':');
            lcd_putchar(t_min/10+0x30);
            lcd_putchar(t_min%10+0x30);    
            lcd_putchar(':');
            lcd_putchar(t_sec/10+0x30);
            lcd_putchar(t_sec%10+0x30);
            if(t_sec==0&&t_min!=0){ // Timer logic
                t_min--;
                t_sec=59;
            }
            if(t_min==0&&t_hour!=0){
                t_hour--;
                t_min=59;
            }
            if(t_sec==0&&t_min==0&&t_hour==0)timer=0; // Stop timer
            t_sec--;
            delay_ms(1000);
        }
        t_hour=t_min=t_sec=0; // Reset timer
        PORTD.3=1; // Restart alarm
    }
                  
    if (t_sec>=60) t_sec=0; // Reset seconds after 60
    if (t_min>=60) t_min=0; // Reset minutes after 60
    if (t_hour>=24) t_hour=0; // Reset hours after 24
}
else if(inf==6){ // Stopwatch code
    here: // Reset stopwatch
    s_hour=s_min=s_sec=0; 
    rtc_get_time(&hour,&min,&sec);
    s_tmp=sec%10;
    while(PINC.0!=0){  
        if(PINC.5==0){ // Reset stopwatch
            goto here;
            delay_ms(150);
        }

        delay_ms(100);
        lcd_clear();
        lcd_putsf("Stopwatch"); // Display stopwatch
        lcd_gotoxy(0,1); 
        lcd_putchar(s_hour/10+0x30);
        lcd_putchar(s_hour%10+0x30);
        lcd_putchar(':');
        lcd_putchar(s_min/10+0x30);
        lcd_putchar(s_min%10+0x30);    
        lcd_putchar(':');
        lcd_putchar(s_sec/10+0x30);
        lcd_putchar(s_sec%10+0x30);
        
        rtc_get_time(&hour,&min,&sec); // Get real-time
        if(sec%10-s_tmp==0)continue;                 
        s_tmp=sec%10;
        s_sec++;
        if(s_sec==60&&s_min!=59){ // Stopwatch logic
            s_min++;
            s_sec=0;
        }
        if(s_min==60&&s_hour!=0){
            s_hour++;
            s_min=0;
        }
    }    
}
else inf=0;
}
};

