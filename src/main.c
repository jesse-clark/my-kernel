#include "monitor.h"

int main(struct multiboot *mboot_ptr)
{
    monitor_clear();
    monitor_write("Hello, World!\n\n");
    int i;
    
    for(i = 0; i < 1025; i += 1) {
        monitor_write_hex(i);
        monitor_write(" = ");
        monitor_write_dec(i);
        monitor_write("\n");
    }
    //monitor_write("-------------");
    //monitor_write_hex(16);
    //monitor_write_hex(31);
    //monitor_write_hex(32);


    return 0;
}
