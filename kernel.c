/* * KERNEL.C - Windows 1.0 Style File Manager 
 * Features: Blue GUI, Header Bar, and Arrow Key Navigation
 */

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

volatile uint16_t* const vga_buffer = (volatile uint16_t*)0xB8000;
const int VGA_COLS = 80;
const int VGA_ROWS = 25;

/* --- HARDWARE COMMUNICATION --- */

// Read a byte from a hardware port (Used for Keyboard)
unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/* --- GRAPHICS FUNCTIONS --- */

// Put a single character at X, Y with a specific color
void put_char(char c, uint8_t color, int x, int y) {
    const int index = y * VGA_COLS + x;
    vga_buffer[index] = (uint16_t)c | (uint16_t)color << 8;
}

// Print a whole string starting at X, Y
void print_string(const char* str, uint8_t color, int x, int y) {
    for (int i = 0; str[i] != '\0'; i++) {
        put_char(str[i], color, x + i, y);
    }
}

// Clear the screen with a specific color
void clear_screen(uint8_t color) {
    for (int i = 0; i < VGA_COLS * VGA_ROWS; i++) {
        vga_buffer[i] = (uint16_t)' ' | (uint16_t)color << 8;
    }
}

/* --- MAIN KERNEL LOGIC --- */

void kernel_main(void) {
    // 1. Draw Windows 1.0 Blue Background (Color 0x17: Blue bg, Light Grey text)
    clear_screen(0x17);

    // 2. Draw the White Header Bar (Color 0x70: Light Grey bg, Black text)
    for (int x = 0; x < VGA_COLS; x++) {
        put_char(' ', 0x70, x, 0);
        put_char(' ', 0x70, x, 1);
    }
    print_string(" MS-DOS Executive", 0x70, 1, 0);
    print_string(" File  View  Special", 0x70, 1, 1);

    // 3. Draw File Manager Content
    print_string("C: /USERS/PRICED/DESKTOP/KERNALOS", 0x1F, 2, 3);
    print_string("---------------------------------", 0x1F, 2, 4);
    
    print_string("  BOOT.ASM", 0x1F, 5, 6);
    print_string("  KERNEL.C", 0x1F, 5, 7);
    print_string("  LINKER.LD", 0x1F, 5, 8);
    print_string("  MYOS.BIN", 0x1F, 5, 9);
    print_string("  FILE_MANAGER.EXE", 0x1F, 5, 10);

    // 4. Keyboard Navigation Logic
    int cursor_y = 6;
    const int min_y = 6;
    const int max_y = 10;
    
    // Draw the initial selection arrow
    print_string("->", 0x1E, 2, cursor_y); 

    print_string("Use ARROW KEYS to navigate. Press ESC to stop.", 0x1E, 2, 23);

    while(1) {
        // Check if the keyboard has data (Status Register port 0x64)
        if (inb(0x64) & 1) {
            unsigned char scancode = inb(0x60); // Read the key from Port 0x60

            // Scancode 0x48 = Up Arrow, 0x50 = Down Arrow
            if (scancode == 0x48) { // UP
                if (cursor_y > min_y) {
                    print_string("  ", 0x1F, 2, cursor_y); // Erase old arrow
                    cursor_y--;
                    print_string("->", 0x1E, 2, cursor_y); // Draw new arrow
                }
            } 
            else if (scancode == 0x50) { // DOWN
                if (cursor_y < max_y) {
                    print_string("  ", 0x1F, 2, cursor_y); // Erase old arrow
                    cursor_y++;
                    print_string("->", 0x1E, 2, cursor_y); // Draw new arrow
                }
            }
        }
    }
}