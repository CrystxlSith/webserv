#pragma once

#include <string>
#include <map>
#include <iostream>

class Request;

class Response
{
	private:
		int _statusCode;
		std::string _response;
		std::string _statusMessage;
		std::string _body;
		std::map<std::string, std::string> _headers;
		std::string _httpVersion;
		std::string _location;
		const Request* _request;
	public:
		Response();
		Response(const Request& request);
		~Response();

		std::string formatResponse();
		void appendDate(std::map<std::string, std::string>& finalHeaders);
		void appendConnection(std::map<std::string, std::string>& finalHeaders);

		void setStatus(int code);
		void setResponse(const std::string& response) { _response = response; }
		void setLocation(const std::string& location) { _location = location; }
		void setStatusMessage(const std::string& statusMessage) { _statusMessage = statusMessage; }
		void setBody(const std::string& body) { _body = body; }
		void setHeaders(const std::map<std::string, std::string>& headers) { _headers = headers; }
		void setHttpVersion(const std::string& httpVersion) { _httpVersion = httpVersion; }

		int	getStatus() const{ return _statusCode;}
		std::string getResponse() const { return _response; }
		std::string getStatusMessage() const { return _statusMessage; }
		std::string getBody() const { return _body; }
		std::map<std::string, std::string> getHeaders() const { return _headers; }
		std::string getHttpVersion() const { return _httpVersion; }
};
