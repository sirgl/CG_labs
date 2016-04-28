#ifndef LOADINGFILEEXCEPTION_H
#define LOADINGFILEEXCEPTION_H

#include <exception>
#include <stdexcept>

class LoadingFileException : public std::runtime_error {
public:
    explicit LoadingFileException(const std::string& _Message)
        : runtime_error(_Message) {}
};


#endif // LOADINGFILEEXCEPTION_H
