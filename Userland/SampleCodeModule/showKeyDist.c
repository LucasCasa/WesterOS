#include "lib.h"

void show_keyboard(){
print_message(" ___________________________________________________________________________\n",0xFF);
print_message("| ~  | !  | @  | #  | $  | %  | ^  | &  | *  | (  | )  | _  | =  |         |\n",0xFF);
print_message("| `  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 0  | -  | +  |BACKSPACE|\n",0xFF);
print_message("|____|____|____|____|____|____|____|____|____|____|____|____|____|_________|\n",0xFF);
print_message("| ----> |    |    |    |    |    |    |    |    |    |    | {  |  }  |     |\n",0xFF);
print_message("| <---- | Q  | W  | E  | R  | T  | Y  | U  | I  | O  | P  | [  |  ]  |ENTER|\n",0xFF);
print_message("|_______|____|____|____|____|____|____|____|____|____|____|____|_____|_    |\n",0xFF);
print_message("|          |    |    |    |    |    |    |    |    |    | :  | \"  | |  |   |\n",0xFF);
print_message("|BLOCK MAYS| A  | S  | D  | F  | G  | H  | J  | k  | L  | ;  | '  | \\  |   |\n",0xFF);
print_message("|__________|____|____|____|____|____|____|____|____|____|____|____|____|___|\n",0xFF);
print_message("|      | *  |    |    |    |    |    |    |    | <  | >  | ?  |            |\n",0xFF);
print_message("| SHFT | +  | Z  | X  | C  | V  | B  | N  | M  | ,  | .  | /  |   SHIFT    |\n",0xFF);
print_message("|______|____|____|____|____|____|____|____|____|____|____|____|____________|\n",0xFF);

}