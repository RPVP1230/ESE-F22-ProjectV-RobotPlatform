// Keypad Header File

#include "stm32f303xe.h"
#include "utility.h"

#define DEBOUNCE_COUNT		 10
#define DEBOUNCE_THRESHOLD 5

// doesn't have to be here at the global scope, why?
static const char keyPressMap[16] = {'1' ,'2', '3', 'A', '4' ,'5', '6', 'B', '7' ,'8', '9', 'C', '*' ,'0', '#', 'D'};

void Keypad_Init(void);
int8_t getKey(void);




