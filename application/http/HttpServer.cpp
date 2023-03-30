#include <memory>
#include <cstdio>
#include <string>
#include <unistd.h>
#include "TcpServer.h"
#include "HttpRequestParser.h"
#include <vector>
#include <regex>
#include "Connection.h"
#include "Logger.h"
#include <cstring>
#include <cerrno>
#include <cstdlib>


char http_log_path[] = "/home/parallels/projects/SkyNetlib/log/http_log";  // use absolute path to avoid error when finding the file 



void SetBuffer(Connection* connection) {
    HttpRequestParser parser(connection->GetReadBuffer()->ToCstr());
    parser.Parse();
    if(parser.CheckProblematic()) {
        std::string response = "HTTP/1.1 400 Bad Request\r\n";
        connection->GetSendBuffer()->SetBuf(std::move(response));
        return;
    }
    if(parser.GetMethod() == "GET") {
        std::string message;
        message = message + "request url: " + parser.GetURL();
        Logger logger(Logger::log_level::debug, Logger::log_target::file_and_terminal, http_log_path);
        logger.DEBUG(message.c_str());
        if(access(parser.GetURL().c_str(), F_OK) == -1) {
            std::string response = "HTTP/1.1 404 File Not Found\r\n";
            connection->GetSendBuffer()->SetBuf(std::move(response));
        } else {
            std::string response;
            response += "HTTP/1.1 200 OK\r\n";
            std::smatch match;
            std::regex type_regex(R"(\.([a-z]+)$)");
            std::string content_type = "Content-Type: ";
            std::regex_search(parser.GetURL(), match, type_regex);
            if(match[1] == "pdf") {
                content_type += "application/pdf";
            } else if (match[1] == "html") {
                content_type += "text/html";
            } else if (match[1] == "json") {
                content_type += "application/json";
            } else if (match[1] == "jpg") {
                content_type += "image/jpg";
            } else if (match[1] == "png") {
                content_type += "image/png";
            } else {
                content_type += "unidentified";
            }
            response = response + content_type + "\r\n";
            std::ios_base::openmode mode;
            if(match[1] == "json" || match[1] == "pdf" || match[1] == "jpg" || match[1] == "png") {
                mode = std::ios_base::in | std::ios_base::binary;
            } else {
                mode = std::ios_base::in;
            }
            std::ifstream  file(parser.GetURL(), mode);
            std::stringstream file_contents;
            file_contents << file.rdbuf();
            file.close();
            std::string content_length = "Content-Length: " + std::to_string(file_contents.str().length()) + "\r\n\r\n";
            response += content_length;
            response += file_contents.str();
            connection->GetSendBuffer()->SetBuf(std::move(response));
        }
    } else {
            std::string response = "HTTP/1.1 403 Forbidden\r\n";
            connection->GetSendBuffer()->SetBuf(std::move(response));
    }
}



int main() {
    Logger logger(Logger::log_level::debug, Logger::log_target::file_and_terminal, http_log_path);
    int port = 9111;
    auto tcp_server = std::make_unique<TcpServer>(port);
    std::string message1;
    message1 = message1 + "http server is listening at " + std::to_string(port);
    logger.DEBUG(message1.c_str());
    tcp_server->SetOnReceiveCallback(
            [](Connection* connection){
                SetBuffer(connection);
                connection->EnableChannelWrite();
            }
		);
    message1.clear();
    message1 += "tcp server got the on receive callback";
    logger.DEBUG(message1.c_str());
    tcp_server->Start();
    return 0;
}
