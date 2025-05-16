#pragma once

#include "AMethods.hpp"

/* 
GOAL:
- Accepts data from clients (e.g., form submissions, file uploads)
- Typically store data server-side
*/

class Post : public AMethods
{
	public:
		Post();
		// Post(const Post& copy);
		// Post&	operator=(const Post& copy);
		virtual ~Post();

		virtual void execute(Request& request, Response& response, Server& server); //Best practice: make destructors virtual in base classes when the class is intended to be inherited.

	private:
		void handleUpload(Request& request, Response& response, Server& server);
		std::string extractFilenameFromMultipart(const std::string& content);
		std::string getFilenameFromPath(const std::string& path);
		bool fileExists(const std::string& path);
		std::string getBoundary(const std::string& contentType);
		std::string extractFileContent(const std::string& body, const std::string& boundary);
		std::string getFileNameFromContentType(const std::string& contentType);
};