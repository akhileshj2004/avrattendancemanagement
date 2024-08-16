#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>


// Define LCD port and pins
#define LCD_PORT PORTB
#define LCD_DDR  DDRB
#define RS PB0
#define EN PB1

// Function prototypes
void LCD_Init(void);
void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data);
void LCD_String_xy(char row, char pos, const char *str);
void LCD_Clear(void);

//keypad port and pins 
#define KEY_PRT  PORTA
#define KEY_DDR  DDRA
#define KEY_PIN  PINA

#define NUM_USERS 4
const char *valid_codes[NUM_USERS] = {"1234", "5678", "4321", "8765"};
const char *user_names[NUM_USERS] = {"Alice", "Bob", "Charlie", "Dave"};
int present[NUM_USERS] = {0}; // Array to track presence of users

unsigned char keypad[4][4] = {
	{'D', '#', '0', '*'},
	{'C', '9', '8', '7'},
	{'B', '6', '5', '4'},
	{'A', '3', '2', '1'}
};

unsigned char colloc, rowloc;

char keyfind(void) {
	while (1) {
		KEY_DDR = 0xF0; // set port direction as input-output
		KEY_PRT = 0xFF; // enable pull-ups
		// 1st 4 ip and next 4 op

		do {
			KEY_PRT &= 0x0F; // mask PORT for column read only
			asm("NOP");
			colloc = (KEY_PIN & 0x0F); // read status of column
		} while (colloc != 0x0F);

		do {
			do {
				_delay_ms(10); // 10ms key debounce time
				colloc = (KEY_PIN & 0x0F); // read status of column
			} while (colloc == 0x0F); // check for any key press

			_delay_ms(20); // 20 ms key debounce time
			colloc = (KEY_PIN & 0x0F);
		} while (colloc == 0x0F);

		// now check for rows
		KEY_DDR = 0xF0; // make rows output
		KEY_PRT = 0x0F; // enable pull-ups for columns

		KEY_PRT = 0xEF; // check for pressed key in 1st row
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) {
			rowloc = 0;
			break;
		}

		KEY_PRT = 0xDF; // check for pressed key in 2nd row
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) {
			rowloc = 1;
			break;
		}

		KEY_PRT = 0xBF; // check for pressed key in 3rd row
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) {
			rowloc = 2;
			break;
		}

		KEY_PRT = 0x7F; // check for pressed key in 4th row
		asm("NOP");
		colloc = (KEY_PIN & 0x0F);
		if (colloc != 0x0F) {
			rowloc = 3;
			break;
		}
	}

	if (colloc == 0x0E)
	return (keypad[rowloc][0]);
	else if (colloc == 0x0D)
	return (keypad[rowloc][1]);
	else if (colloc == 0x0B)
	return (keypad[rowloc][2]);
	else
	return (keypad[rowloc][3]);
}



void print_absent_users(void) {
	LCD_Clear();
	LCD_String_xy(1, 0, "Absent Users:");
	_delay_ms(600); // Display the header for 2 seconds
	for (int i = 0; i < NUM_USERS; i++) {
		if (!present[i]) {
			LCD_Clear();
			LCD_String_xy(1, 0, user_names[i]);
			_delay_ms(400); // Display each absent user for 2 seconds
		}
	}
	_delay_ms(600);
	LCD_Clear();
	LCD_String_xy(1, 0, "Enter Code:");
}

void mark_attendance(const char *input_code) {
	int j;
	int access_granted = 0;

	for (j = 0; j < NUM_USERS; j++) 
	{
		if (strcmp(input_code, valid_codes[j]) == 0)
		 {
			if (present[j])
			 {
				LCD_Clear();
				LCD_String_xy(1, 0,"Already Marked");
				_delay_ms(600); // Display "Already Marked" for 2 seconds
				} 
				else
				 {
				access_granted = 1;
				present[j] = 1; // Mark user as present
				//strcpy(user_dname[j], device_name); // Store device name
				LCD_Clear();
				LCD_String_xy(1, 0, "Access Granted");
				LCD_String_xy(2, 0, user_names[j]);
				_delay_ms(600); // Display "Access Granted" and user name for 2 seconds
			}
			break;
		}
	}

	if (!access_granted && j == NUM_USERS) {
		LCD_Clear();
		LCD_String_xy(1, 0, "Access Denied");
		_delay_ms(600); // Display "Access Denied" for 2 seconds
	}
}

void reset_data(void) {
	// Reset presence and device names
	for (int i = 0; i < NUM_USERS; i++) {
		present[i] = 0;
	}
	LCD_Clear();
	LCD_String_xy(1, 0, "Data Reset");
	_delay_ms(1000); // Display "Data Reset" for 1 second
}

int main(void) {
	char input_code[5] = {0};
	char key;
	int i;
	
	LCD_Init(); // Initialize LCD
	
	while (1) {
		LCD_Clear();
		LCD_String_xy(1, 0, "Enter Code:");
		i = 0;

		while (i < 4) {
			key = keyfind();
			if (key == '*') {
				print_absent_users();
				break;
				} 
				else if (key == '#') 
				{
				LCD_Clear();
				LCD_String_xy(1, 0, "Enter Code:");
				i = 0;
				continue;
				}
				 else if (key == 'D')
				    {
				reset_data();
				LCD_Clear();
				LCD_String_xy(1, 0, "Enter Code:");
				i = 0;
				continue;
			}

			LCD_Command(0xc0 + i); // Move cursor to the second row
			LCD_Char('*'); // Display asterisk for each key press
			input_code[i++] = key;
		}

		if (i == 4) {
			input_code[4] = '\0'; // Null terminate the string

			_delay_ms(500);

			// Check if the entered code matches any valid code
			int user_index = -1;
			for (int j = 0; j < NUM_USERS; j++) {
				if (strcmp(input_code, valid_codes[j]) == 0) {
					user_index = j;
					break;
				}
			}

			if (user_index != -1) {
				// Mark attendance
				mark_attendance(input_code);
				} else {
				// Display access denied if code doesn't match
				LCD_Clear();
				LCD_String_xy(1, 0, "Access Denied");
				_delay_ms(600); // Display "Access Denied" for 2 seconds
				LCD_Clear();
				LCD_String_xy(1, 0, "Enter Code:");
			}
		}
	}
}

// LCD functions implementation
void LCD_Command(unsigned char cmnd) {
	LCD_PORT = (LCD_PORT & 0x0F) | (cmnd & 0xF0);
	LCD_PORT &= ~ (1<<RS);
	LCD_PORT |= (1<<EN);
	_delay_us(1);
	LCD_PORT &= ~ (1<<EN);

	_delay_us(200);

	LCD_PORT = (LCD_PORT & 0x0F) | (cmnd << 4);
	LCD_PORT |= (1<<EN);
	_delay_us(1);
	LCD_PORT &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Char(unsigned char data) {
	LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);
	LCD_PORT |= (1<<RS);
	LCD_PORT |= (1<<EN);
	_delay_us(1);
	LCD_PORT &= ~ (1<<EN);

	_delay_us(200);

	LCD_PORT = (LCD_PORT & 0x0F) | (data << 4);
	LCD_PORT |= (1<<EN);
	_delay_us(1);
	LCD_PORT &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Init(void) {
	LCD_DDR = 0xFF; // set PORTB direction as output
	_delay_ms(20);

	LCD_Command(0x02); // initialize LCD in 4-bit mode
	LCD_Command(0x28); // 2 line, 5*7 matrix in 4-bit mode
	LCD_Command(0x0C); // display on cursor off
	LCD_Command(0x06); // increment cursor (shift cursor to right)
	LCD_Command(0x01); // clear display screen
	_delay_ms(2);
}

void LCD_String_xy(char row, char pos, const char *str) {
	if (row == 1)
	LCD_Command((pos & 0x0F) | 0x80);
	else if (row == 2)
	LCD_Command((pos & 0x0F) | 0xC0);
	while (*str)
	LCD_Char(*str++);
}

void LCD_Clear(void) {
	LCD_Command(0x01); // clear display
	_delay_ms(2);
	LCD_Command(0x80); // cursor at home position
}
