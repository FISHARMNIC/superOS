typedef enum
{
    FALSE = 0,
    TRUE
} boolean;

#include "kernel.h"
#include "keyboard.h"
/*
16 bit video buffer elements(register ax)
8 bits(ah) higher : 
  lower 4 bits - forec olor
  higher 4 bits - back color

8 bits(al) lower :
  8 bits : ASCII character to print
*/

/*---------------------------- VGA BASICS--------------------------------------------*/
// #region

uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color)
{
    uint16 ax = 0;
    uint8 ah = 0, al = 0;

    ah = back_color;
    ah <<= 4;
    ah |= fore_color;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;

    return ax;
}

//clear video buffer array
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
    uint32 i;
    for (i = 0; i < BUFSIZE; i++)
    {
        (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
    }
}

//initialize vga buffer
void init_vga(uint8 fore_color, uint8 back_color)
{
    vga_buffer = (uint16 *)VGA_ADDRESS;                    //point vga_buffer pointer to VGA_ADDRESS
    clear_vga_buffer(&vga_buffer, fore_color, back_color); //clear buffer
}

// #endregion
/*---------------------------- STRINGS ----------------------------------------------*/
// #region
int str_len(char *word)
{
    int counter;
    for (counter = 0; word[counter] != '\0'; counter++)
        ;
    return counter;
}

int len(char *S)
{
    // Variable for traversal
    int i = 0;

    // Traverse till null is reached
    while (S[i])
        i++;

    return i;
}

void clrstr(char *word, int leng)
{
    for (int i = 0; i < leng; i++)
    {
        word[i] = '\0';
    }
}

int strcmp(char S1[], char S2[])
{
    // If lengths of the two
    // strings are different
    if (len(S1) != len(S2))
        return FALSE;

    // Variable for traversal
    int i = 0;

    // Traverse till null is reached
    while (S1[i])
    {

        if (S1[i] != S2[i])
            return FALSE;

        // Increment i
        i++;
    }
    return TRUE;
}

// #endregion
/*---------------------------- STD OUT ----------------------------------------------*/
// #region

void printChar(char letter)
{
    vga_buffer[charAt] = vga_entry(letter, GREEN, BLACK);
    charAt++;
}

void printString(char *word)
{
    int strlen;
    for (strlen = 0; word[strlen] != '\0'; strlen++)
    {
        printChar(word[strlen]);
    }
}

void newLine()
{
    charAt += 80 - (charAt % 80);
}

// #endregion
/*---------------------------- STD IN -----------------------------------------------*/
// #region
char typedWord[80];
int typedWordLoc;

uint8 inb(uint16 port)
{
    uint8 ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)
                 : "d"(port));
    return ret;
}

void outb(uint16 port, uint8 data)
{
    asm volatile("outb %0, %1"
                 : "=a"(data)
                 : "d"(port));
}

char get_input_keycode()
{
    char ch = 0;
    while ((ch = inb(KEYBOARD_PORT)) != 0)
    {
        if (ch > 0)
            return ch;
    }
    return ch;
}

/*
keep the cpu busy for doing nothing(nop)
so that io port will not be processed by cpu
here timer can also be used, but lets do this in looping counter
*/
void wait_for_io(uint32 timer_count)
{
    while (1)
    {
        asm volatile("nop");
        timer_count--;
        if (timer_count <= 0)
            break;
    }
}

void sleep(uint32 timer_count)
{
    wait_for_io(timer_count);
}

char get_ascii_char(uint8 key_code)
{
    switch (key_code)
    {
    case KEY_A:
        return 'A';
    case KEY_B:
        return 'B';
    case KEY_C:
        return 'C';
    case KEY_D:
        return 'D';
    case KEY_E:
        return 'E';
    case KEY_F:
        return 'F';
    case KEY_G:
        return 'G';
    case KEY_H:
        return 'H';
    case KEY_I:
        return 'I';
    case KEY_J:
        return 'J';
    case KEY_K:
        return 'K';
    case KEY_L:
        return 'L';
    case KEY_M:
        return 'M';
    case KEY_N:
        return 'N';
    case KEY_O:
        return 'O';
    case KEY_P:
        return 'P';
    case KEY_Q:
        return 'Q';
    case KEY_R:
        return 'R';
    case KEY_S:
        return 'S';
    case KEY_T:
        return 'T';
    case KEY_U:
        return 'U';
    case KEY_V:
        return 'V';
    case KEY_W:
        return 'W';
    case KEY_X:
        return 'X';
    case KEY_Y:
        return 'Y';
    case KEY_Z:
        return 'Z';
    case KEY_1:
        return '1';
    case KEY_2:
        return '2';
    case KEY_3:
        return '3';
    case KEY_4:
        return '4';
    case KEY_5:
        return '5';
    case KEY_6:
        return '6';
    case KEY_7:
        return '7';
    case KEY_8:
        return '8';
    case KEY_9:
        return '9';
    case KEY_0:
        return '0';
    case KEY_MINUS:
        return '-';
    case KEY_EQUAL:
        return '=';
    case KEY_SQUARE_OPEN_BRACKET:
        return '[';
    case KEY_SQUARE_CLOSE_BRACKET:
        return ']';
    case KEY_SEMICOLON:
        return ';';
    case KEY_BACKSLASH:
        return '\\';
    case KEY_COMMA:
        return ',';
    case KEY_DOT:
        return '.';
    case KEY_FORESLHASH:
        return '/';
    case KEY_SPACE:
        return ' ';
    default:
        return 0;
    }
}

// #endregion
/*---------------------------- INPUT HANDLING ---------------------------------------*/
// #region
void parseCommand()
{
    char command[80];
    char params[80];
    clrstr(command, 80);
    clrstr(params, 80);

    int i;
    int j = TRUE; //not a one word command
    for (i = 0; (typedWord[i] != ' ') && (typedWord[i] != '\0'); i++)
    {
        command[i] = typedWord[i];
        if (typedWord[i + 1] == '\0')
        {
            j = FALSE;
        }
    }
    i++;
    if (j)
    {
        for (int k = 0; typedWord[k + i] != '\0'; k++)
        {
            params[k] = typedWord[k + i];
        }
    }
    //printString(command);
    //printString(params);
    if (strcmp(command, "ECHO"))
    {
        printString(params);
    }

    else
    {
        printString("Unkown Command: ");
        printString(command);
    }
}
// #endregion
/*---------------------------- FILE SYSTEM ----------------------------------------- */
// #region
char memory [1000];
int memory_jump[1000];
int memory_loc[1000];
/*
Memory stores all of my files
Memory_jump stores where the byte should jump to if instructed
Memory_loc stores the beggining address for the file

example:
- File #1, "bob", is located at address 20. This was found using Memory_loc
- The next 20 character are part of that file, but the last character, (40th)
indicates that I need to jump to address 50 becuase the file continues there

address 20:
m_jump | m_ loc
'h'    | -1
'e'    | -1
'l'    | -1
....
address 40:
'b'    | 50

address 50:
'n'    | -1
'.'    | -1
'\0'   | -1

*/
// #endregion
/* ---------------------------- KERNEL ENTRY ----------------------------------------*/
void test_input()
{
    char ch = 0;
    char keycode = 0;
    do
    {
        keycode = get_input_keycode();
        if (keycode == KEY_ENTER)
        {
            newLine();
            parseCommand();
            newLine();
            clrstr(typedWord, 80);
            typedWordLoc = 0;
        }
        else if (keycode == KEY_BACKSPACE)
        {
            charAt--;
            printChar(NULL);
            typedWord[typedWordLoc] = '\0';
            if (charAt % 80 != 0)
            {
                typedWordLoc -= 1;
            }
            charAt--;
        }
        else
        {
            ch = get_ascii_char(keycode);
            printChar(ch);
            typedWord[typedWordLoc] = ch;
            typedWordLoc += 1;
        }
        sleep(0x03FFFFFF);
    } while (ch > 0);
}

void kernel_entry()
{
    //first init vga with fore & back colors
    init_vga(GREEN, BLACK);

    printString("ENTER COMMAND");
    newLine();
    test_input();
}