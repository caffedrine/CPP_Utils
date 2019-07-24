//
// Created by curiosul on 1/24/19.
//

#ifndef _ANSICOLORS_H_
#define _ANSICOLORS_H_

#ifdef EMPTY_ANSI_COLORS
	#define ANSI_COLOR_RED     		""
	#define ANSI_COLOR_RED_BOLD 	""
	#define ANSI_COLOR_GREEN   		""
	#define ANSI_COLOR_GREEN_BOLD 	""
	#define ANSI_COLOR_YELLOW  		""
	#define ANSI_COLOR_BLUE    		""
	#define ANSI_COLOR_MAGENTA 		""
	#define ANSI_COLOR_CYAN    		""
	#define ANSI_COLOR_RESET   		""
#else
	#define ANSI_COLOR_RED            "\x1b[0;31m"
	#define ANSI_COLOR_RED_BOLD    "\x1b[1;31m"
	#define ANSI_COLOR_GREEN        "\x1b[0;32m"
	#define ANSI_COLOR_GREEN_BOLD    "\x1b[1;32m"
	#define ANSI_COLOR_YELLOW        "\x1b[33m"
	#define ANSI_COLOR_BLUE            "\x1b[34m"
	#define ANSI_COLOR_MAGENTA        "\x1b[35m"
	#define ANSI_COLOR_CYAN            "\x1b[36m"
	#define ANSI_COLOR_RESET        "\x1b[0m"
#endif

#endif //_ANSICOLORS_H_
