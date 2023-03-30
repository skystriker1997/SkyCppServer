#include "HttpRequestParser.h"




HttpRequestParser::HttpRequestParser(const char* request) : http_request_(request), problematic_(false) {};
HttpRequestParser::~HttpRequestParser() = default;;



void HttpRequestParser::Parse() {
    std::regex method_regex(R"((GET|POST)\s)");
    std::smatch match;
    std::regex_search(http_request_, match, method_regex);
    if(match.empty()) {
        problematic_ = true;
        return;
    } else {
        method_ = match[1];
    }
    std::regex url_regex(R"((GET|POST)\s(\S+)\sHTTP/\d\.\d)");
    std::regex_search(http_request_, match, url_regex);
    if(match.length() == 1) {
        problematic_ = true;
        return;
    } else {
        url_ = match[2];
        if(url_ == "/") {
            url_ = "/home/parallels/projects/SkyNetlib/static_files/index.html";
        }
    }
    if(method_ == "POST" ) {
        std::regex type_regex(R"(\r\nContent-Type:\s(\S+)\r\n)");
        std::regex_search(http_request_, match, type_regex);
        if(match.empty()) {
            problematic_ = true;
            return;
        } else {
            content_type_ = match[1];
        }
        std::regex length_regex(R"(\r\nContent-Length:\s(\S+)\r\n)");
        std::regex_search(http_request_, match, length_regex);
        if(match.empty()) {
            problematic_ = true;
            return;
        } else {
            content_length_ = match[1];
        }
        std::regex body_regex(R"(\r\n\r\n([\S\s]+)");
        std::regex_search(http_request_, match, body_regex);
        if(match.empty()) {
            problematic_ = true;
            return;
        } else {
            request_body_ = match[1];
        }		
    }
}



bool HttpRequestParser::CheckProblematic() const {
    return problematic_;
};


const std::string& HttpRequestParser::GetMethod() {return method_;};

const std::string& HttpRequestParser::GetURL() {return url_;};

const std::string& HttpRequestParser::GetContentType() {return content_type_;};

const std::string& HttpRequestParser::GetContentLength() {return content_length_;};

const std::string& HttpRequestParser::GetBody() {return request_body_;};


