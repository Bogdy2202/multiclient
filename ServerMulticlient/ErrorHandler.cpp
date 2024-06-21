//
// Created by student on 6/21/24.
//

#include "ErrorHandler.h"
#include "Logger.h"

void ErrorHandler::handle(const std::string& errorMessage) {
    Logger::getInstance().log("Error: " + errorMessage);
    throw CustomException(errorMessage);
}
