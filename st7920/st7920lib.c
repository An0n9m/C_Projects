#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "st7920lib.h"

uint8_t framebuffer[128 * 8] = {0}; // 1024 bytes

void lcd_pulse_enable() {
	E_HIGH();
	_delay_us(1);
	E_LOW();
	_delay_us(1);
}

void lcd_command(uint8_t cmd) {
	RS_LOW();
	RW_LOW();
	LCD_DATA_OUT(cmd);
	lcd_pulse_enable();
	_delay_us(35);
}

void lcd_data(uint8_t data) {
	RS_HIGH();
	RW_LOW();
	LCD_DATA_OUT(data);
	lcd_pulse_enable();
	_delay_us(35);
}

void lcd_init() {
	DDRD = 0xFF; // PORTD init
	DDRB |= (1 << RS) | (1 << RW) | (1 << E) | (1 << RE); // Control outputs
	
	// Reset sequence according to documentation
	RE_LOW();
	_delay_ms(50);
	RE_HIGH();
	_delay_us(120);

	// Function set: Basic Instruction Set
	lcd_command(0x30);
	_delay_us(110);
	lcd_command(0x30);
	_delay_us(40);

	// Display ON
	lcd_command(0x0C);
	_delay_us(120);

	// Clear display
	lcd_command(0x01);
	_delay_ms(12);

	// Entry Mode Set
	lcd_command(0x06);
	_delay_us(100);

	// Switch to Extended Instruction Set + Graphic Display ON
	lcd_command(0x34);
	_delay_us(50);
	lcd_command(0x36); // Graphic ON
	_delay_us(50);
}

void fill_buffer(uint8_t x, uint8_t y) {
	if (x >= 256 || y >= 32) return;
	uint16_t byte_index = (y * 256 + x) / 8;
	uint8_t bit = x % 8;
	framebuffer[byte_index] |= (1 << (7 - bit));
}

void clear_buffer(uint8_t x, uint8_t y) { 
	if (x >= 256 || y >= 32) return;
	uint16_t idx = (y * 256 + x) / 8;
	uint8_t bidx = x % 8;
	framebuffer[idx] &= ~(1 << (7 - bidx));
}

void clear_framebuffer() {
	memset(framebuffer, 0, sizeof(framebuffer));
}

void lcd_draw_framebuffer() {
	for (uint8_t y = 0; y < 32; y++) {
		lcd_command(0x80 | y);     // Vertical address (0 31)
		lcd_command(0x80 | 0);     // Horizontal address (0)

		for (uint8_t x = 0; x <= 15; x++) {
			uint8_t b1 = framebuffer[y * 32 + x * 2];
			uint8_t b2 = framebuffer[y * 32 + x * 2 + 1];
			lcd_data(b1);
			lcd_data(b2);
		}

	}
}

void set_pixel(uint8_t x, uint8_t y){ // For less confusion because of 256x32 addressing
	 if (x >= 128 || y >= 64) return;
	 if(y>31){
		 fill_buffer(128+x,y-32);
	 }
	 else fill_buffer(x,y);
}
void clear_pixel(uint8_t x, uint8_t y){ // For less confusion because of 256x32 addressing
	if (x >= 128 || y >= 64) return;
	if(y>31){
		clear_buffer(128+x,y-32);
	}
	else clear_buffer(x,y);
}

	void display_pulse() {
		E_HIGH();
		_delay_us(20);
		E_LOW();
		_delay_us(20);
	}
	
	void display_data(uint8_t data) {
		RS_HIGH();  
		RW_LOW();   
		PORTD = data;
		display_pulse();
		_delay_us(20);
	}
	
	
	void display_command(uint8_t cmd) {
		RS_LOW(); 
		RW_LOW(); 
		PORTD = cmd;
		display_pulse();
		_delay_ms(1); 
	}
	
	
	void display_init(){ // For display data ram usage needs separate functions, because of different timings!
		DDRD = 0xFF; // DB0:DB7 as output
		DDRB |= (1 << RS) | (1 << RW) | (1 << E); // RS, RW, E as output
		
		// Reset sequence according to documentation
		RE_LOW();
		_delay_ms(50);
		RE_HIGH();
		_delay_us(120);

		_delay_ms(40); // Power-up delay

		display_command(0x30); // Basic Instruction Set
		_delay_us(110);
		display_command(0x30);
		_delay_us(40);
		display_command(0x0C); // Display ON, Cursor OFF
		_delay_us(110);
		display_command(0x01); // Clear display
		_delay_ms(10);
		display_command(0x06); // Entry mode: increment, no shift
	}
	
	void display_address(uint8_t x, uint8_t y) {
		uint8_t address;
		switch (y) {
			case 0: address = 0x80 + x; break;
			case 1: address = 0x90 + x; break;
			case 2: address = 0x88 + x; break;
			case 3: address = 0x98 + x; break;
			default: address = 0x80;
		}
		lcd_command(address);
	}

	void graphical_init(){
		lcd_command(0x34); // Extended instruction set
		_delay_us(50);
		lcd_command(0x36); // Graphic ON
		_delay_us(50);
	}

	void DDRAM_test(){
							// Testcase #1 Display Data RAM Test
		display_init();
		static int dir=1;
		static int ypos=0;
			while(1){
				ypos+=dir;
				display_command(0x01); // clear display
				_delay_ms(1); 
				display_address(0,ypos);
				display_data('P');
				display_data('L');
				display_data('A');
				display_data('Y');
				display_data('E');
				display_data('R');
				display_data('1');
				
				display_address(4,ypos);
				display_data('P');
				display_data('L');
				display_data('A');
				display_data('Y');
				display_data('E');
				display_data('R');
				display_data('2');
				if(ypos==3)dir=-1;
				else if(ypos==0)dir=1;
				_delay_ms(500);
		}
	}

	void GDRAM_test(){
								// Testcase #2 Graphic RAM Test
	lcd_init();
	clear_framebuffer();
	static int dir=1;

	
	for(int y=0;;y+=dir){
		_delay_ms(10);
		if(y==64)dir=-1;
		else if(y==-1)dir=1;
		lcd_draw_framebuffer();
		for(uint8_t x=0;x<=127;x++){
			if(dir==1)set_pixel(x,y);
			else clear_pixel(x,y);
			}
		
		}
	}

	void DDRAM_and_GDRAM_test(){
								// Testcase #3 using both GDRAM and DDRAM (pong simulation)
	  display_init();
	  			display_address(1,0);
				display_data('D');
				display_data('A');
				display_data('V');
				display_data('I');
				display_data('D');
				
				display_address(5,0);
				display_data('B');
				display_data('O');
				display_data('B');

	  graphical_init(); 
	
	  static int yPos = 0;        
	  static int xPos = 5;
	  static int xDirection= 0;
	  static int yDirection = 1;    
while(1){	  
	  clear_framebuffer();
	  
for (int x = 0; x < 6; x++) {
	for (int y = yPos; y < yPos + 9; y++) {
		if (y < 63) {
			set_pixel(x, y);
		}
	}
}
    for (int x = 122; x < 128; x++) {
	    for (int y = yPos; y < yPos + 9; y++) {
		    if (y < 63) {
			   set_pixel(x, y);
		    }
	    }
    }
    for (int x = xPos; x < xPos+4; x++) {
	    if (x <= 122) {
		    set_pixel(x, 31);
	    }
    }
	lcd_draw_framebuffer();
	yPos += yDirection;
	xPos += xDirection;
	if (yPos >= 57) {
		yDirection = -2;
		} else if (yPos <= 1) {
		yDirection = 2;
	}
	if (xPos >= 122) {
		xDirection = -5; // Ball speed
		} else if (xPos <= 5) {
		xDirection = 5;  // Ball speed
			}
	_delay_ms(5); // Control animation speed (adjust as needed)
		}
	}

	void pong_test(){
								// Testcase #4 simple pong simulation using GDRAM	
	  lcd_init();	
	  static int yPos = 0;        
	  static int xPos = 5;
	  static int xDirection= 0;
	  static int yDirection = 1;    
while(1){	  
	  clear_framebuffer();
	  
for (int x = 0; x < 6; x++) {
	for (int y = yPos; y < yPos + 9; y++) {
		if (y < 63) {
			set_pixel(x, y);
		}
	}
}
    for (int x = 122; x < 128; x++) {
	    for (int y = yPos; y < yPos + 9; y++) {
		    if (y < 63) {
			   set_pixel(x, y);
		    }
	    }
    }
    for (int x = xPos; x < xPos+4; x++) {
	    if (x <= 122) {
		    set_pixel(x, 31);
	    }
    }
	lcd_draw_framebuffer();
	yPos += yDirection;
	xPos += xDirection;
	if (yPos >= 57) {
		yDirection = -3;
		} else if (yPos <= 1) {
		yDirection = 3;
	}
	if (xPos >= 122) {
		xDirection = -5; // Ball speed
		} else if (xPos <= 5) {
		xDirection = 5;  // Ball speed
		}
	_delay_ms(2); // Control animation speed (adjust as needed)
		}
	}

// written by an0n9m (Davit Jalagonia)
