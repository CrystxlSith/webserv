#include "../../includes/server/Request.hpp"
#include "../../includes/server/Response.hpp"
#include "../../includes/server/Server.hpp"
#include "../../includes/utils/Utils.hpp"
#include <sstream>
#include <iostream>

Request::Request(std::string request, Server& server) :
	_server(server),
	_response(),
	_request(request),
	_method(""),
	_uri(""),
	_httpVersion(""),
	//_queryString(""),
	_path(""),
	_headers(),
	_body(""),
	_isChunked(false),
	_expectedChunkSize(0),
	_chunkedBody("")
{
	parseRequest();
	setPathQueryString();
	parseQuery();
}

Request::~Request()
{

}

void Request::handleResponse()
{
	Response response(*this);
	_server.executeMethods(*this, response);
	response.setResponse(response.formatResponse());
	std::cout << BLUE << "Sending response: [" << response.getResponse().substr(0, response.getResponse().length()) << "...]" << RESET << std::endl;
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
			line = line.substr(0, line.length() - 1); // Remove CR if present
		parseHeader(line);
	}

	// Vérifier si le transfert est en chunks
	_isChunked = (getHeader("Transfer-Encoding") == "chunked");

	std::cout << "Transfer-Encoding bb: " << getHeader("Transfer-Encoding") << std::endl;
	// The rest is the request body
	std::string body;
	while (std::getline(ss, line)) {
		body += line + "\n";
	}
	
	if (_isChunked) {
		std::cout << "Chunked request" << std::endl;
		processChunkedData(body);
	} else {
		_body = body;
	}
	
	std::cout << "Received request: [" << this->getBody().substr(0, this->getBody().length()) << "...]" << std::endl;
}

void Request::parseRequestLine(const std::string& line)
{
	std::istringstream iss(line);
	iss >> _method >> _uri >> _httpVersion;
	// Remove "\r" if present in the HTTP version
	if (!_httpVersion.empty() && _httpVersion[_httpVersion.length() - 1] == '\r')
		_httpVersion = _httpVersion.substr(0, _httpVersion.length() - 1);
}

void Request::parseHeader(const std::string& line)
{
	size_t colonPos = line.find(':');
	if (colonPos != std::string::npos) {
		std::string key = line.substr(0, colonPos);
		std::string value = line.substr(colonPos + 1);
		// Remove spaces at the beginning of the value
		while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
			value.erase(0, 1);
		_headers[key] = value;
	}
}

std::string Request::getHeader(const std::string& name) const
{
	std::map<std::string, std::string>::const_iterator it = _headers.find(name);
	if (it != _headers.end())
		return it->second;
	return "";
}

void	Request::parseQuery()
{
	size_t start = 0;
	size_t ampPos;

	while (start < _queryString.length())
	{
		ampPos = _queryString.find('&', start);
		if (ampPos == std::string::npos)
			ampPos = _queryString.length();
		std::string pair = _queryString.substr(start, ampPos - start);
		size_t equalPos = pair.find('=');
		if (equalPos != std::string::npos)
		{
			std::string key = pair.substr(0, equalPos);
			std::string value = pair.substr(equalPos + 1);
			_queryParams[key] = value;
		}
		start = ampPos + 1;
	}
}

void	Request::setPathQueryString()
{
	size_t qPos = _uri.find('?');
	if (qPos != std::string::npos)
	{
		_path = _uri.substr(0, qPos);
		_queryString = _uri.substr(qPos + 1);
		return ;
	}
	_path = _uri;
	_queryString = "";
}

bool Request::validateQueryParams()
{
	for (std::map<std::string, std::string>::const_iterator it = _queryParams.begin(); it != _queryParams.end(); ++it)
	{
		const std::string& key = it->first;
		const std::string& value = it->second;

		if (key == "id" && !isValidInt(value))
			return false;
		else if (key == "page" && !isValidInt(value))
			return false;
		else if (key == "limit" && !isValidInt(value))
			return false;
		else if (key == "email" && !isValidEmail(value))
			return false;
		else if (key == "active" && !isValidBool(value))
			return false;
	}
	return true;
}

bool Request::isValidInt(const std::string& value)
{
	for (size_t i = 0; i < value.length(); i++)
	{
		if (!isdigit(value[i]))
			return false;
	}
	return !value.empty();
}

bool Request::isValidBool(const std::string& value)
{
	return (value == "true" || value == "false" ||
			value == "1" || value == "0" ||
			value == "yes" || value == "no");
}

bool Request::isValidEmail(const std::string& value)
{
	size_t atPos = value.find('@');
	if (atPos == std::string::npos || atPos == 0)
		return false;

	size_t dotPos = value.find('.', atPos);
	if (dotPos == std::string::npos || dotPos == value.length() - 1)
		return false;

	return true;
}

void Request::fillResponse(Response& response, int statusCode, const std::string& body)
{
	response.setStatus(statusCode);
	//std::cout << "response.getStatusMessage() : "<< response.getStatusMessage() << std::endl;
	//response.setResponse(response.formatResponse());
	//std::cout << "response.getResponse() : "<< response.getResponse() << std::endl;
	response.setBody(body);
	//std::cout << "response.getBody() : "<< response.getBody() << std::endl;
	response.setHeaders(this->getHeaders());
	response.setHttpVersion(this->getHttpVersion());
	//std::cout << "response.getHttpVersion() : " << response.getHttpVersion() << std::endl;
	_response = response;
}

// filename="....."
std::string Request::getFilename() const
{
    std::string filename;
    size_t pos = _body.find("filename=\""); //std::string::size_type pos

    if (pos != std::string::npos)
    {
        //found
        pos += 10; //skip filename="
        size_t endPos = _body.find("\"", pos);//start find frm pos
        if (endPos != std::string::npos)
            filename = _body.substr(endPos - pos);
    }
    return filename;
}

bool Request::processChunkedData(const std::string& chunk)
{
    std::cout << BLUE << "Traitement des données en chunks..." << RESET << std::endl;
    std::stringstream ss(chunk);
    std::string line;
    
    while (std::getline(ss, line)) {
        // Supprimer le \r si présent
        if (!line.empty() && line[line.length() - 1] == '\r') {
            line = line.substr(0, line.length() - 1);
            std::cout << YELLOW << "Caractère CR supprimé de la ligne" << RESET << std::endl;
        }
        
        // Ignorer les lignes vides
        if (line.empty()) {
            continue;
        }
        
        // Si nous attendons une taille de chunk
        if (_expectedChunkSize == 0) {
            _expectedChunkSize = parseChunkSize(line);
            std::cout << GREEN << "Nouvelle taille de chunk détectée: " << _expectedChunkSize << RESET << std::endl;
            
            if (_expectedChunkSize == 0) {
                // Fin du transfert
                _body = _chunkedBody;
                std::cout << GREEN << "Fin du transfert chunked, taille totale: " << _body.length() << " octets" << RESET << std::endl;
                return true;
            }
        } else {
            // Ajouter les données du chunk
            _chunkedBody += line;
            _expectedChunkSize -= line.length();
            std::cout << BLUE << "Ajout de " << line.length() << " octets, reste " << _expectedChunkSize << " octets à lire" << RESET << std::endl;
            
            // Si nous avons reçu toutes les données du chunk
            if (_expectedChunkSize == 0) {
                std::cout << GREEN << "Chunk complet" << RESET << std::endl;
            }
        }
    }
    
    return false;
}

size_t Request::parseChunkSize(const std::string& chunk)
{
	size_t size = 0;
	std::stringstream ss(chunk);
	ss >> std::hex >> size;
	return size;
}