#pragma once
#include <QJsonObject>
#include "Config.h"
#include <memory>

class ParserException : public std::runtime_error {
public:
	explicit ParserException(const std::string& _Message)
		: runtime_error(_Message) {}
};
