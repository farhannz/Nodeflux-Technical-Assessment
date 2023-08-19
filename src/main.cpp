// Core
#include <fnz.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
#include <opencv2/opencv.hpp>
using json = nlohmann::json;
using namespace nlohmann::literals;


/**
 * Load image from Base64
 * 
 * @param imageB64 Base64 string of an image
 * @param width Default : 0, if the argument is not 0, it will resize the width to the desired input
 * @param height Default : 0, if the argument is not 0, it will resize the height to the desired input
 * @return cv::Mat image
 */
cv::Mat loadImageB64(const std::string &imageB64, int width = 0, int height = 0){
    cv::Mat img = cv::Mat();
    if(imageB64.empty()) throw std::runtime_error("Empty image string");
    std::istringstream f(imageB64);
    std::string tmp, processed;
    while(getline(f,tmp,',')){
        processed = tmp;
    }
    std::string buffer;
    try{
        buffer = base64_decode(processed);
        std::vector<uchar> data(buffer.begin(), buffer.end());
        img = cv::imdecode(data,cv::IMREAD_UNCHANGED);
        width = (!width) ? img.size().width : width;
        height = (!height) ? img.size().height : height;
        cv::resize(img,img,cv::Size(width,height));
        return img;
    }
    catch(const std::runtime_error &e){
        throw e;
    }
    return cv::Mat();
}

int main(int argc, char* argv[]) {
    std::string HOST;
    int PORT;

    if (const auto env_host = std::getenv("FNZ_SERVER_HOST")){
        HOST = env_host;
    }
    else{
        HOST = "0.0.0.0";
        spdlog::warn("There is no environment variable called \"FNZ_SERVER_HOST\"");
        spdlog::warn("Setting the HOST address to {}", HOST);
    }

    if (const auto env_port = std::getenv("FNZ_SERVER_PORT")){
        std::stringstream value;
        value << env_port;
        value >> PORT;
    }
    else{
        PORT = 8080;
        spdlog::warn("There is no environment variable called \"FNZ_SERVER_PORT\"");
        spdlog::warn("Setting the PORT address to {}", PORT);
    }
    std::unique_ptr<fnz::http::Server> server = std::make_unique<fnz::http::Server>(HOST, PORT);
    server->init();

    // POST /resize_image
    server->request("/resize_image", "POST", [](auto req, auto args){
        auto timeStart = std::chrono::system_clock::now();
        json response;
        response["code"] = 500;
        response["message"] = "";
        try{
            // cv2::Mat img;
            json payload = json::parse(req->body);
            if(payload.empty()) throw std::runtime_error("Empty payload");
            if(!payload.contains("input_jpeg") || !payload.contains("desired_width") || !payload.contains("desired_height")) throw std::runtime_error("Invalid parameters size");
            cv::Mat img = loadImageB64(payload["input_jpeg"], payload["desired_width"], payload["desired_height"]);
            std::vector<uchar> buff;
            cv::imencode(".jpg", img, buff);
            std::string encoded = base64_encode(std::string(buff.begin(), buff.end()));
            response["message"] = "success";
            response["code"] = 200;
            response["output_jpeg"] = encoded;
        }
        catch(std::exception &e){
            response["code"] = 500;
            response["message"] = e.what();
        }
        req->response.headers.set("content-type", "application/json");
        req->response.result(response["code"]);
        req->response.body = response.dump(4);
        spdlog::info("Response:\n{}", req->response.body);
        std::chrono::duration<double> endTime = std::chrono::system_clock::now() - timeStart;
        spdlog::info("time : {} ms", endTime.count()*1000.0f);
    });


    //Invalid Route Handler
    std::vector<std::string> methods{"GET", "POST", "PUT", "PATCH", "DELETE"};
    for(auto &i : methods){
        server->request_regex(std::regex(R"(\D+)"),i,[i](auto req, auto args){
            json response;
            response["title"] = "not_found_exception";
            response["message"] = fmt::format("No route found for {} {}", i, args[0]);
            response["code"] = 404;
            req->response.headers.set("content-type", "application/json");
            req->response.result(404);
            req->response.body = response.dump(4);
            spdlog::info("Response:\n{}", req->response.body);
        });
    }

    server->run();
}
