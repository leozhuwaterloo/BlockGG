#pragma once
#include <string>

class WindowManager;

enum class LogLevel {
	Debug,
	Info,
	Warn,
  Error
};

class LogManager {
public:
  static LogManager *getInstance();
  void log(const LogLevel &level, const std::string &msg);
private:
	LogManager();
	virtual ~LogManager();
  LogManager(LogManager const&);
  LogManager& operator=(LogManager const&);
  static LogManager* instance;
  static std::string LOG_LEVEL_STR[4];
	WindowManager *window_manager;
};
