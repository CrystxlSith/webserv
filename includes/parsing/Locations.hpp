#pragma once

#include <string>
#include <vector>
class Location
{
private:
    std::string _path;
    std::string _root;
    bool _autoindex;
    std::string _cgi_ext;
    std::string _client_max_body_size;
    std::string _upload_path;
    std::string _index;
    std::string _error_page;
    std::vector<std::string> _methods;

public:
    Location();
    Location(const Location& other);
    ~Location();

    // Setters
    void setPath(const std::string& path) {_path = path;};
    void setRoot(const std::string& root) {_root = root;};
    void setAutoindex(const std::string& autoindex) {_autoindex = (autoindex == "on");};
    void setCgiExt(const std::string& cgi_ext) {_cgi_ext = cgi_ext;};
    void setClientMaxBodySize(const std::string& client_max_body_size) {_client_max_body_size = client_max_body_size;};
    void setUploadPath(const std::string& upload_path) {_upload_path = upload_path;};
    void setIndex(const std::string& index) {_index = index;};
    void setErrorPage(const std::string& error_page);
    void setMethods(const std::vector<std::string>& methods) {_methods = methods;};
    void setReturn(const std::string& Return);
    // Getters
    std::string getPath() const {return _path;};
    std::string getRoot() const {return _root;};
    bool getAutoindex() const {return _autoindex;};
    std::string getCgiExt() const {return _cgi_ext;};
    std::string getClientMaxBodySize() const {return _client_max_body_size;};
    std::string getUploadPath() const {return _upload_path;};
    std::string getIndex() const {return _index;};
    std::string getReturn() const;
    std::vector<std::string> getMethods() const {return _methods;};
    // Operators
    Location& operator=(const Location& other);
};

