#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>

struct BNUM_speed {
	uint32_t speed;
	uint32_t bnum;
};

void command_interpreter(int argc, char*const* argv, uint32_t* baudrate, char *portname);

int64_t CheckSpeed(unsigned speed);

void print_help(void);

#endif
