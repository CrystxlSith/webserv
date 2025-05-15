#pragma once

#include <string>
#include <map>
#include "Response.hpp"

class Server;

class Request
{
private:
	Server& _server;
	Response _response;
	std::string _request;
	std::string _method;
	std::string _uri;
	std::string	_abspath;
	std::string _httpVersion;
	std::string _path;
	std::string	_queryString;
	std::map<std::string, std::string> _headers;
	std::string _body;
	std::map<std::string, std::string> _queryParams;
	bool _isChunked;
	size_t _expectedChunkSize;
	std::string _chunkedBody;
	// std::string _response;
public:
	Request(std::string request, Server& server);
	~Request();

	// Getters
	std::string getRequest() const { return _request; }
	std::string getResponse() const { return _response.formatResponse(); }
	std::string getMethod() const { return _method; }
	std::string getUri() const { return _uri; }
	std::string getAbspath() const { return _abspath; }
	std::string getHttpVersion() const { return _httpVersion; }
	std::map<std::string, std::string> getHeaders() const { return _headers; }
	std::string getBody() const { return _body; }
	std::string getQueryString() const { return _queryString; };
	std::string getHeader(const std::string& name) const;
	std::string getPath() const { return _path; };
	std::string getFilename() const;
	bool isChunked() const { return _isChunked; }
	std::string getChunkedBody() const { return _chunkedBody; }

	// Setters
	void setRequest(const std::string& request) { _request = request; }
	void setAbspath(const std::string& abspath) { _abspath = abspath; }
	void setPathQueryString();

	// Methods
	void parseRequest();
	void parseRequestLine(const std::string& line);
	void parseHeader(const std::string& line);
	void handleResponse();
	void parseQuery();
	bool validateQueryParams();
	bool isValidInt(const std::string& value);
	bool isValidBool(const std::string& value);
	bool isValidEmail(const std::string& value);
	void fillResponse(Response& response, int statusCode, const std::string& body);
	bool processChunkedData(const std::string& chunk);
	size_t parseChunkSize(const std::string& chunk);
};
