#pragma once
#include <bits/stdc++.h>

class Exception: public std::exception {
	protected:
		std::string message;
	private:
		std::string prefix;
		const char* what() const throw() override {
			return (prefix + ": " +  message).c_str();
		}
	public:
		Exception(std::string prefix, std::string message) {
			this -> prefix = prefix;
			this -> message = message;
		}
};

class NotImplementedException: public Exception {
	inline static const std::string PREFIX = "NotImplementedException";
	public:
		NotImplementedException(std::string message): Exception(PREFIX, message) {}
};
