#include "monitor.h"

u16int *video_memory = (u16int *)0xb8000;
u8int cursor_x = 0;
u8int cursor_y = 0;

static void move_cursor() {
    u16int cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3d4, 14);
    outb(0x3d5, cursorLocation >> 8);
    outb(0x3d4, 15);
    outb(0x3d5, cursorLocation);
}

static void scroll() {
    u8int attributeByte = (0 << 4) | ( 15 & 0x0f);
    u16int blank = 0x20 | (attributeByte << 8);

    if(cursor_y >= 25) {

        int i;
        for( i = 0; i < 24*80; i++) {
            video_memory[i] = video_memory[i+80];
        }

        for( i = 24*80; i < 25*80; i++) {
            video_memory[i] = blank;
        }
        
        cursor_y = 24;
    }
}

void monitor_put(char c) {
    u8int backColor = 0;
    u8int foreColor = 15;

    u8int attributeByte = (backColor << 4) | (foreColor & 0x0f);

    u16int attribute = attributeByte << 8;
    u16int *location;

    if( c == 0x08 &&cursor_x) {
        cursor_x -= 1;
    }
    else if( c == 0x09) {
        cursor_x = (cursor_x+8) & ~(8-1);
    }

    else if ( c== '\r') {
        cursor_x = 0;
    }

    else if ( c == '\n') {
        cursor_x = 0;
        cursor_y += 1;
    }

    else if ( c >= ' ') {
        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x += 1;
    }

    if ( cursor_x >= 80) {
        cursor_x = 0;
        cursor_y += 1;
    }

    scroll();
    move_cursor();
}

void monitor_clear() {

    u8int attributeByte = ( 0 << 4) | (15 & 0x0f);
    u16int blank = 0x20 | (attributeByte << 8);

    int i;
    for (i = 0; i <80*25; i += 1) {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void monitor_write( char *c ) {
    int i = 0;
    while( c[i]) {
        monitor_put(c[i]);
        i += 1;
    }
}

void monitor_write_hex(u32int n) {
    char hex_chars[] = "0123456789abcdef";
    char ret_val;
    int i,j;
    i = (32 - 4);
    while( i > 0) {
        //monitor_write("happy days");
        i -= 4;
        j = n >> i;
        j &= 0xf;
        ret_val = hex_chars[j];
        monitor_put(ret_val);
    }
}

void monitor_write_dec(u32int n) {
    int i = 0;
    u32int temp = n;
    char ret[32];
    do {
        i += 1;
        temp /= 10;
    } while (temp);
    ret[i] = '\0';
    do {
        i -= 1;
        ret[i] = (n % 10) + '0';
        n /= 10;
    } while (i);
    monitor_write(ret);
}
