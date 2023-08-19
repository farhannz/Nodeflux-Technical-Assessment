# C++ Software Engineer - Nodeflux Technical Assessment
[![Docker Image CI](https://github.com/farhannz/Nodeflux-Technical-Assessment/actions/workflows/docker-image.yml/badge.svg)](https://github.com/farhannz/Nodeflux-Technical-Assessment/actions/workflows/docker-image.yml) [![Ubuntu CMake](https://github.com/farhannz/Nodeflux-Technical-Assessment/actions/workflows/cmake-build.yml/badge.svg)](https://github.com/farhannz/Nodeflux-Technical-Assessment/actions/workflows/cmake-build.yml)
## About
---
Technical Assessment Application.

Simple HTTP Server for resizing a jpeg image, in the form of base 64 encoded binary input data
to the desired target dimension.

## Installation 
---

### Build from Source

* Requirements:   
[OpenCV](https://opencv.org/get-started/)   
[Libasyik](https://github.com/okyfirmansyah/libasyik)   
[spdlog](https://github.com/gabime/spdlog)

1. Clone this repository
    ```
    git clone --recursive https://github.com/farhannz/Nodeflux-Technical-Assessment.git
    cd Nodeflux-Technical-Assessment
    ```
2. Set up environment variables
    ```bash
    export FNZ_SERVER_HOST=127.0.0.1
    export FNZ_SERVER_PORT=4004
    ```
3. Build
    ```
    cmake -S . -B build
    cmake --build build
    ```
4. Run Application
    ```
    ./build/Nodeflux-Assessment
    ```
5. The server will run at FNZ_SERVER_HOST:FNZ_SERVER_PORT, in this example at 127.0.0.1:4004


### Build with Docker

*Requirements:   
Docker
1. Clone this repository
    ```
    git clone --recursive https://github.com/farhannz/Nodeflux-Technical-Assessment.git
    cd Nodeflux-Technical-Assessment
    ```
2. Set up environment variables
    ```bash
    export FNZ_SERVER_HOST=127.0.0.1
    export FNZ_SERVER_PORT=4004
    ```
3. Build the docker image
    ```
    docker build --build-arg EXPOSE_PORT=${FNZ_SERVER_PORT} -t Nodeflux-Technical-Assessment:v1.0 .
    ```
4. Run the docker image
    ```
    docker run -it --init --rm \
    -e FNZ_SERVER_HOST=${FNZ_SERVER_HOST} \
    -e FNZ_SERVER_PORT=${FNZ_SERVER_PORT} \
    -p ${FNZ_SERVER_PORT}:${FNZ_SERVER_PORT} Nodeflux-Technical-Assessment:v1.0
    ```
5. The webservice will run at FNZ_SERVER_HOST:FNZ_SERVER_PORT, in this example at 127.0.0.1:4004

## Endpoints
---

| Endpoints      | Method        |
| -------------- | ------------- |
| /resize_image  | POST          |

### POST ```/resize_image```
Resize image request

* Payload :   
`Content-Type: application/json`
```json
{
    "input_jpeg": "[base 64 encoding of jpeg binary]",
    "desired_width": [target_width],
    "desired_height": [target_height]
}
```
### Responses :

Success :   
`HTTP 200`   
`Content-Type: application/json`
```json
{
    "code": 200,
    "message": "success",
    "output_jpeg": "[base 64 encoding of output jpeg binary]"
}
```

Failed:   
`HTTP 4xx/5xx`   
`Content-Type: application/json`
```json
{
    "code": 4xx/5xx,
    "message": "[output error description here]"
}
```