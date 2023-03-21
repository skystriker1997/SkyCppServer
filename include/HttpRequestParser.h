#include <regex>
#include <string>


#ifndef SKYCPPSERVER_HTTPPARSER_H
#define SKYCPPSERVER_HTTPPARSER_H


class HttpRequestParser {
private:
    	std::string http_request_;
    	bool problematic_;
    	std::string method_;
    	std::string uri_;
    	std::string content_type_;
    	std::string content_length_;
    	std::string request_body_;

public:
    	explicit HttpRequestParser(const char* request);

    	~HttpRequestParser();

    	void Parse();

    	bool CheckProblematic() const;

    	const std::string& GetMethod();
    	const std::string& GetURI();
    	const std::string& GetContentType();
    	const std::string& GetContentLength();
    	const std::string& GetBody();
};

#endif
