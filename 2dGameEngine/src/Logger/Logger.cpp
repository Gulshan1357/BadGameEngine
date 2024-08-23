#include "Logger.h"
#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <vector>

#define GRN "\x1b[32m"
#define WHT "\033[0m"
#define RED "\x1b[91m"
#define YEL "\x1b[33m"

std::vector<LogEntry> Logger::messages;

void Logger::Log(const std::string& message)
{
	using namespace std::chrono;
	time_point now = system_clock::now();

	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = "Log: [" + std::format("{:%c}", now) + "]: " + message;
	
	std::cout << GRN <<  logEntry.message << WHT << std::endl;

	messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
	using namespace std::chrono;
	time_point now = system_clock::now();

	LogEntry logEntry;
	logEntry.type = LOG_ERROR;
	logEntry.message = "Err: [" + std::format("{:%c}", now) + "]: " + message;

	std::cout << RED << logEntry.message << WHT << std::endl;
	
	messages.push_back(logEntry);
}