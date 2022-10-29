#ifndef __LIST_ERRORS_SYM__
#define __LIST_ERRORS_SYM__

const int ERRORS_NUMBER = 12;

enum ErrorCodes
{
    NO_ERROR                     = 0,
    NULL_LIST_POINTER            = 1<<0,
    MEMORY_ALLOCATION_ERROR      = 1<<1,
    ERROR_LOGS_OPEN              = 1<<2,
    WRONG_SIZE                   = 1<<3,
    WRONG_CAPACITY               = 1<<4,
    DAMAGED_POINTER_TO_NULL_ELEM = 1<<5,
    WRONG_HEAD                   = 1<<6,
    WRONG_TAIL                   = 1<<7,
    DEBUG_FILE_DAMAGED           = 1<<8,
    DEBUG_FUNCTION_DAMAGED       = 1<<9,
    DEBUG_LINE_DAMAGED           = 1<<10,
    DEBUG_NAME_DAMAGED           = 1<<11,
};

const char* const ERROR_DESCRIPTION[] = { "Pointer to stack = nullptr\n",
                                          "Error during memmory allocation\n",
                                          "Error during open logs\n",
                                          "Poisoned size\n",
                                          "Poisoned capacity\n",
                                          "Pointer to null element array in list is poison ptr or nullptr\n",
                                          "Poison head\n",
                                          "Poison tail\n",
                                          "Information about file in which list created damaged\n",
                                          "Information about function in which list created damaged\n",
                                          "Information about line in which list created damaged\n",
                                          "Information about name of list damaged\n",};
#endif  //__LIST_ERRORS_SYM__