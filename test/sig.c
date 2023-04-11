#include <stdio.h>
#include <termios.h>

int
main() {
 int x = 0, y = 0;
 get_pos(&y, &x);
 printf("x:%d, y:%d\n", x, y);
 return 0;
}

int
get_pos(int *y, int *x) {

 char buf[30]={0};
 int ret, i, pow;
 char ch;

*y = 0; *x = 0;

 struct termios term, restore;

 tcgetattr(0, &term);
 tcgetattr(0, &restore);
 term.c_lflag &= ~(ICANON|ECHO);
 tcsetattr(0, TCSANOW, &term);

 write(1, "\033[6n", 4);

 for( i = 0, ch = 0; ch != 'R'; i++ )
 {
    ret = read(0, &ch, 1);
    if ( !ret ) {
       tcsetattr(0, TCSANOW, &restore);
       fprintf(stderr, "getpos: error reading response!\n");
       return 1;
    }
    buf[i] = ch;
    printf("buf[%d]: \t%c \t%d\n", i, ch, ch);
 }

 if (i < 2) {
    tcsetattr(0, TCSANOW, &restore);
    printf("i < 2\n");
    return(1);
 }

 for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
     *x = *x + ( buf[i] - '0' ) * pow;

 for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
     *y = *y + ( buf[i] - '0' ) * pow;

 tcsetattr(0, TCSANOW, &restore);
 return 0;
}