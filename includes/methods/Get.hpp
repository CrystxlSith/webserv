#pragma once

#include "AMethods.hpp"

class Get : public AMethods
{
	public:
		Get();
		virtual ~Get();

		virtual void execute(Request& request, Response& response, Server& server);

	private:
		void	serveFile(Request& request, Response& response, Server& server);
		void	serveDirectory(Request& request, Response& response, Server& server);
		std::string getMimeType(const std::string& path);
};