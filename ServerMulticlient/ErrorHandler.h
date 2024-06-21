#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>
#include <exception>

class ErrorHandler {
public:
    static void handle(const std::string& errorMessage);
};

class CustomException : public std::exception {
public:
    CustomException(const std::string& message) : msg_(message) {}
    virtual const char* what() const noexcept override {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

#endif // ERRORHANDLER_H
