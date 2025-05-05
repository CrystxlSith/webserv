#include "../../includes/server/Request.hpp"
#include "../../includes/server/Response.hpp"
#include <sstream>
#include <iostream>

Request::Request(std::string request) : 
    _response(),
    _request(request),
    _method(""),
    _uri(""),
    _httpVersion(""),
    _headers(),
    _body("")
{
    parseRequest();
}

Request::~Request()
{

}

void Request::handleResponse()
{
    Response response(*this);

    response.setStatus(200);
    response.setResponse("OK");
    response.setStatusMessage("OK");
    response.setBody("<html><head><style>body{font-family:Arial,sans-serif;background-color:#f0f0f0;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}h1{color:#333;text-align:center;padding:20px;background-color:#fff;border-radius:10px;box-shadow:0 0 10px rgba(0,0,0,0.1)}</style></head><body><h1>Hello World</h1></body></html>");
    response.setHeaders(this->getHeaders());
    response.setHttpVersion(this->getHttpVersion());
    // _response = response.formatResponse();
    std::cout << response.formatResponse() << std::endl;
    _response = response;
}

void Request::parseRequest()
{
    std::stringstream ss(_request);
    std::string line;
    
    // Parse la première ligne (méthode, URL, version)
    if (std::getline(ss, line) && !line.empty()) {
        parseRequestLine(line);
        std::cout << "method: " << _method << std::endl;
        std::cout << "uri: " << _uri << std::endl;
        std::cout << "http version: " << _httpVersion << std::endl;
    }
    
    // Parse les en-têtes
    while (std::getline(ss, line) && !line.empty() && line != "\r") {
        if (!line.empty() && line[line.length() - 1] == '\r') 
            line = line.substr(0, line.length() - 1); // Enlever le CR si présent
        parseHeader(line);
    }

    // Le reste est le corps de la requête
    std::string body;
    while (std::getline(ss, line)) {
        body += line + "\n";
    }
    _body = body;

    handleResponse();
}

void Request::parseRequestLine(const std::string& line)
{
    std::istringstream iss(line);
    iss >> _method >> _uri >> _httpVersion;
    // Supprimer le "\r" si présent dans la version HTTP
    if (!_httpVersion.empty() && _httpVersion[_httpVersion.length() - 1] == '\r')
        _httpVersion = _httpVersion.substr(0, _httpVersion.length() - 1);
}

void Request::parseHeader(const std::string& line)
{
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos) {
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        // Supprimer les espaces en début de valeur
        while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
            value.erase(0, 1);
        _headers[key] = value;
    }
}