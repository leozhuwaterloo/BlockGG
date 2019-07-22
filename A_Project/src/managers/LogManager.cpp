#include "./includes/LogManager.hpp"
#include <iostream>
#include "./includes/WindowManager.hpp"

LogManager *LogManager::instance = nullptr;
std::string LogManager::LOG_LEVEL_STR[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

LogManager* LogManager::getInstance() {
  if (!instance) instance = new LogManager();
  return instance;
}

LogManager::LogManager()
  : window_manager(WindowManager::getInstance()) {}

LogManager::~LogManager() {}


void LogManager::log(const LogLevel &level, const std::string &msg) {
  if (level == LogLevel::Debug) {
    std::cout << "\033[1;35m";
  } else if (level == LogLevel::Info) {
    std::cout << "\033[1;32m";
  } else if (level == LogLevel::Warn) {
    std::cout << "\033[1;33m";
  } else if (level == LogLevel::Error) {
    std::cout << "\033[1;31m";
  }
  std::string str = "[" + LOG_LEVEL_STR[(int)level] + "]\t" + msg;
  if (level != LogLevel::Debug) window_manager->addMessage(str);
  std::cout << str << "\033[0m" << std::endl;
}
