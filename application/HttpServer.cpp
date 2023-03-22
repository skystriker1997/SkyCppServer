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




void SetBuffer(Connection* connection) {
    HttpRequestParser parser(connection->GetReadBuffer()->ToCstr());
    parser.Parse();
    if(parser.CheckProblematic()) {
        std::string response = "HTTP/1.1 400 Bad Request\r\n";
        connection->GetSendBuffer()->SetBuf(std::move(response));
        return;
    }
    if(parser.GetMethod() == "GET") {
        if(access(parser.GetURI().c_str(), F_OK) == -1) {
            std::string response = "HTTP/1.1 404 File Not Found\r\n";
            connection->GetSendBuffer()->SetBuf(std::move(response));
        } else {
            std::string response;
            response += "HTTP/1.1 200 OK\r\n";
            std::smatch match;
            std::regex type_regex("\\S+\\.(a-z+)");
            std::string content_type = "Content-Type: ";
            std::regex_search(parser.GetURI(), match, type_regex);
            if(match[1] == "pdf") {
                content_type += "application/pdf";
            } else if (match[1] == "html") {
                content_type += "text/html";
            } else if (match[1] == "json") {
                content_type += "application/json";
            } else if (match[1] == "jpeg") {
                content_type += "image/jpeg";
            } else {
                content_type += "image/png";
            }
            response += content_type;
            std::ios_base::openmode mode;
            if(match[1] == "json" || match[1] == "pdf" || match[1] == "jpeg" || match[1] == "png") {
                mode = std::ios_base::in | std::ios_base::binary;
            } else {
                mode = std::ios_base::in;
            }
            std::ifstream  file(parser.GetURI(), mode);
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
    Logger logger;
    auto tcp_server = std::make_unique<TcpServer>(9111);
    char message1[] = "made a tcp server listening at 9111 port";
    logger.DEBUG(message1);
    tcp_server->SetOnReceiveCallback(
            [](Connection* connection){
                SetBuffer(connection);
                connection->EnableChannelWrite();
            }
		);
    char message2[] = "tcp server got the on receive callback!";
    logger.DEBUG(message2);
    tcp_server->Start();
    return 0;
}
