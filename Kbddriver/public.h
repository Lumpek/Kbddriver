#ifndef _PUBLIC_H
#define _PUBLIC_H

#define IOCTL_INDEX             0x800


#define IOCTL_KBFILTR_GET_KEYBOARD_ATTRIBUTES CTL_CODE( FILE_DEVICE_KEYBOARD,   \
                                                        IOCTL_INDEX,    \
                                                        METHOD_BUFFERED,    \
                                                        FILE_READ_DATA)


#define IOCTL_SET_PROBABILITY CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_INDEX + 1, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct _KB_CONFIG {
    ULONG Probability; // 0 to 100
	ULONG Mode;
} KB_CONFIG, * PKB_CONFIG;

#endif