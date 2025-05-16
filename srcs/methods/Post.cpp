#include "../../includes/methods/Post.hpp"
#include "../../includes/server/Request.hpp"

Post::Post() : AMethods::AMethods() {}
// Post::Post(const Post& copy) : AMethods::AMethods(copy) {}
// Post&	Post::operator=(const Post& copy) {return *this;}
Post::~Post() {}

void Post::execute(Request& request, Response& response, Server& server)
{
    std::string uploadPath;
    std::string body;

    uploadPath = request.getAbspath() + server.getUpload();
    
    // Extract the actual file content from multipart data
    std::string contentType = request.getHeader("Content-Type");
    if (contentType.find("multipart/form-data") != std::string::npos) {
        // This is a multipart form upload
        std::string boundary = getBoundary(contentType);
        body = extractFileContent(request.getBody(), boundary);
    } else {
        body = request.getBody();
    }

    // Extract the actual filename from multipart content
    std::string filename = request.getFilename();
    
    // Check for empty filename
    if (filename.empty()) {
        response.setStatus(400);
        response.setBody("<html><body><h1>Error: Invalid filename</h1></body></html>\n");
        return;
    }

    // Create a file stream for output, using full path
    std::string full_path = uploadPath + PATH_SEPARATOR + filename;
    std::ofstream output(full_path.c_str());

    std::cout << "full_path : " << full_path << std::endl;
    std::cout << "filename : " << filename << std::endl;
    std::cout << "uploadPath : " << uploadPath << std::endl;
    std::cout << "transfer-encoding : " << request.getHeader("Transfer-Encoding") << std::endl;
    
    //check if the file can be created
    if (!output.is_open())
    {
        response.setStatus(500);
        response.setBody("<html><body><h1>Error : Failed saving file.</h1></body></html>\n");
        return;
    }
    output << body;
    output.close();

    response.setStatus(201);
    response.setBody("Success : file uploaded.\n");
}

// Helper to extract boundary from Content-Type header
std::string Post::getBoundary(const std::string& contentType) {
    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos != std::string::npos) {
        return contentType.substr(boundaryPos + 9);
    }
    return "";
}

// Helper to extract file content from multipart data
std::string Post::extractFileContent(const std::string& body, const std::string& boundary) {
    std::string startBoundary = "--" + boundary;
    std::string endBoundary = "--" + boundary + "--";
    
    // Find the content part
    size_t startPos = body.find(startBoundary);
    if (startPos == std::string::npos) return "";
    
    // Move to after the first boundary
    startPos = body.find("\r\n\r\n", startPos);
    if (startPos == std::string::npos) return "";
    startPos += 4; // Move past "\r\n\r\n"
    
    // Find the end boundary
    size_t endPos = body.find(endBoundary, startPos);
    if (endPos == std::string::npos) {
        // Try finding the next boundary if there's no end boundary
        endPos = body.find(startBoundary, startPos);
    }
    
    if (endPos == std::string::npos) {
        // Use the end of the body if no boundary is found
        return body.substr(startPos);
    }
    
    // Back up to exclude the \r\n before the boundary
    if (endPos >= 2 && body[endPos-2] == '\r' && body[endPos-1] == '\n') {
        endPos -= 2;
    }
    
    return body.substr(startPos, endPos - startPos);
}

/**
 * 1 get uploadpath / filename / body 
 * 2 Create a file stream for output, using full path
 * 3 check if can create file
 * - if not -> error msg 
 * - if yes -> write body to file
 * 4 close() + set status success 
 * 5 set status and success message 
 * 
 * @note std::ofstream out(uploadPath + PATH_SEPARATOR + filename.c_str()) : Create a file stream for output, using full path
 */

// void Post::handleUpload(Request& request, Response& response, Server& server)
// {
//     std::string uploadPath;
//     std::string filename;
//     std::string body;

//     uploadPath = server.getUpload();
//     filename = request.getFilename();
//     body = request.getBody();

//     // Create a file stream for output, using full path
//     std::string full_path = uploadPath + PATH_SEPARATOR + filename;
//     std::ofstream output(full_path.c_str());

//     //check if the file can be created
//     if (!output.is_open())
//     {
//         //Internal server error 500
//         //set error msg
//         response.setStatus(500);
//         response.setBody("Error : Failed saving file.\n");
//         return; 
//     }
//     output << body;
//     output.close();

//     response.setStatus(201);
//     response.setBody("Success : file uploaded.\n");
// }



/*

| Code    | Type        | Meaning (EN)                         | 中文說明 |
| ------- | ----------- | ------------------------------------ | ------- |
| **200** | Success     | OK — The request has succeeded       | 請求成功，伺服器已回應資料 |
| **201** | Success     | Created — New resource created       | 資源已成功建立 |
| **204** | Success     | No Content — No body in response     | 無內容，通常用於成功但無需回傳資料 |
| **301** | Redirect    | Moved Permanently                    | 永久轉址    |
| **302** | Redirect    | Found (Temporary Redirect)           | 暫時轉址 |
| **304** | Redirect    | Not Modified — Use cached version    | 資料未變更，使用快取版本 |
| **400** | Client Err. | Bad Request — Syntax error           | 請求格式錯誤 |
| **401** | Client Err. | Unauthorized — Login required        | 未授權，需要驗證 |
| **403** | Client Err. | Forbidden — Access denied            | 禁止存取，權限不足 |
| **404** | Client Err. | Not Found — Resource doesn’t exist   | 找不到資源 |
| **405** | Client Err. | Method Not Allowed                   | 不支援的 HTTP 方法 |
| **408** | Client Err. | Request Timeout                      | 請求逾時 |
| **429** | Client Err. | Too Many Requests                    | 請求太頻繁，被限制 |
| **500** | Server Err. | Internal Server Error                | 伺服器內部錯誤 |
| **502** | Server Err. | Bad Gateway — Invalid upstream resp. | 網關錯誤 |
| **503** | Server Err. | Service Unavailable                  | 服務暫停 |
| **504** | Server Err. | Gateway Timeout                      | 網關逾時 |

*/