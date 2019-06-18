/*
 *  File      : Logger.h
 *  Created on: 18.06.2019
 *      Author: curiosul
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/bin_to_hex.h>

/* Logger instance - multi-threaded */
extern spdlog::logger *logger;

void setup_logger();


#endif // _LOGGER_H_