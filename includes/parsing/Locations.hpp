#pragma once

#include <string>
#include <vector>
#include <map>

class Location
{
private:
	std::string _path;
	std::string _root;
	std::string _autoindex;
	std::string _cgi_ext;
	std::string _client_max_body_size;
	std::string _upload_path;
	std::string _index;
	std::map<size_t, std::string> _error_page;
	std::vector<std::string> _methods;
	std::pair<int, std::string> _return;  // Pour stocker le code et l'URL de redirection


public:
	Location();
	Location(const Location& other);
	~Location();

	// Setters
	void setPath(const std::string& path) {_path = path;};
	void setRoot(const std::string& root) {_root = root;};
	void setAutoindex(const std::string& autoindex) {_autoindex = autoindex;};
	void setCgiExt(const std::string& cgi_ext) {_cgi_ext = cgi_ext;};
	void setClientMaxBodySize(const std::string& client_max_body_size) {_client_max_body_size = client_max_body_size;};
	void setUploadPath(const std::string& upload_path) {_upload_path = upload_path;};
	void setIndex(const std::string& index) {_index = index;};
	void setErrorPage(const std::map<size_t, std::string>& error_page) {_error_page = error_page;};
	void setMethods(const std::vector<std::string>& methods) {_methods = methods;};
	void setReturn(int code, const std::string& returnUrl) { 
		_return = std::make_pair(code, returnUrl); 
	}
	// Getters
	std::string getPath() const {return _path;};
	std::string getRoot() const {return _root;};
	std::string getAutoindex() const {return _autoindex;};
	std::string getCgiExt() const {return _cgi_ext;};
	std::string getClientMaxBodySize() const {return _client_max_body_size;};
	std::string getUploadPath() const {return _upload_path;};
	std::string getIndex() const {return _index;};
	std::vector<std::string> getMethods() const {return _methods;};
	std::map<size_t, std::string> getErrorPage() const {return _error_page;};
	std::pair<int, std::string> getReturn() const {return _return;};

	// Operators
	Location& operator=(const Location& other);



	// //added to debug segfault
	// void printLOCATIONUploadPath() 
	// {
	// 	std::cout << "Upload path: " << _upload_path << std::endl;
	// };

};
