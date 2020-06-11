//
// Created by curiosul on 1/24/19.
//
// https://gist.github.com/RabaDabaDoba/145049536f815903c79944599c6f952a
//

#ifndef _ANSICOLORS_H_
#define _ANSICOLORS_H_

#ifdef EMPTY_ANSI_COLORS
	#define ANSI_COLOR_RED              ""
	#define ANSI_COLOR_RED_BOLD         ""
	#define ANSI_COLOR_GREEN            ""
	#define ANSI_COLOR_GREEN_BOLD       ""
	#define ANSI_COLOR_YELLOW           ""
	#define ANSI_COLOR_YELLOW_BOLD      ""
	#define ANSI_COLOR_BLUE             ""
	#define ANSI_COLOR_BLUE_BOLD        ""
	#define ANSI_COLOR_MAGENTA          ""
    #define ANSI_COLOR_MAGENTA_BOLD     ""
#else

    #define ANSI_COLOR_RED              "\x1b[0;31m"
	#define ANSI_COLOR_RED_BOLD         "\x1b[1;31m"
    #define ANSI_COLOR_RED_BG           "\x1b[0;41m"
	
	#define ANSI_COLOR_GREEN            "\x1b[0;32m"
	#define ANSI_COLOR_GREEN_BOLD       "\x1b[1;32m"
    #define ANSI_COLOR_GREEN_BG         "\x1b[0;42m"
	
	#define ANSI_COLOR_YELLOW           "\x1b[0;33m"
    #define ANSI_COLOR_YELLOW_BOLD      "\x1b[1;33m"
    #define ANSI_COLOR_YELLOW_BG        "\x1b[0;43m"
    
	#define ANSI_COLOR_BLUE             "\x1b[0;34m"
    #define ANSI_COLOR_BLUE_BOLD        "\x1b[1;34m"
    #define ANSI_COLOR_BLUE_BG          "\x1b[0;44m"
    
	#define ANSI_COLOR_MAGENTA          "\x1b[0;35m"
    #define ANSI_COLOR_MAGENTA_BOLD     "\x1b[1;35m"
    #define ANSI_COLOR_MAGENTA_BG       "\x1b[0;45m"
    
    #define ANSI_COLOR_CYAN             "\x1b[0;36m"
    #define ANSI_COLOR_CYAN_BOLD        "\x1b[1;36m"
    #define ANSI_COLOR_CYAN_BG          "\x1b[0;46m"
    
    #define ANSI_COLOR_WHITE            "\x1b[0;37m"
    #define ANSI_COLOR_WHITE_BOLD       "\x1b[1;37m"
    #define ANSI_COLOR_WHITE_BG         "\x1b[0;47m"
    
    #define ANSI_COLOR_BLACK            "\x1b[0;30m"
    #define ANSI_COLOR_BLACK_BOLD       "\x1b[1;30m"
    #define ANSI_COLOR_BLACK_BG         "\x1b[0;40m"
    
	#define ANSI_COLOR_RESET            "\x1b[0m"

#endif

#endif //_ANSICOLORS_H_
