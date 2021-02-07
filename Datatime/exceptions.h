#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>
#include <cstring>
#include <iostream>

class Exception {
protected:
    std::string error_;
public:
    Exception() = default;

    explicit Exception(const std::string& error) : error_(error) {
    }

    virtual Exception& operator=(const Exception& other) = default;

    virtual ~Exception() = default;

    virtual const char* What() const noexcept {
        return error_.c_str();
    }
};

class BadAlloc : public Exception {
public:
    explicit BadAlloc(const std::string& error) : Exception(error) {
    }
};

class LogicError : public Exception {
public:
    explicit LogicError(const std::string& error) : Exception(error) {
    }
};

class InvalidArgument : public LogicError {
public:
    explicit InvalidArgument(const std::string& error) : LogicError(error) {
    }
};

class DomainError : public LogicError {
public:
    explicit DomainError(const std::string& error) : LogicError(error) {
    }

};

class LengthError : public LogicError {
public:
    explicit LengthError(const std::string& error) : LogicError(error) {
    }
};

class OutOfRange : public LogicError {
public:
    explicit OutOfRange(const std::string& error) : LogicError(error) {
    }
};

class RuntimeError : public Exception {
public:
    explicit RuntimeError(const std::string& error) : Exception(error) {
    }
};

class RangeError : public RuntimeError {
public:
    explicit  RangeError(const std::string& error) : RuntimeError(error) {
    }
};

class OverflowError : public RuntimeError {
public:
    explicit  OverflowError(const std::string& error) : RuntimeError(error) {
    }
};

class UnderflowError : public RuntimeError {
public:
    explicit UnderflowError(const std::string& error) : RuntimeError(error) {
    }
};

#endif //EXCEPTION_H
