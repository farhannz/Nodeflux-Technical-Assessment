FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8
ENV TZ Asia/Jakarta


RUN apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y dist-upgrade && \
    apt-get -y autoremove && \
    apt-get install -y build-essential gdb wget git libssl-dev clang-format


# Boost library
RUN mkdir ~/temp && cd ~/temp && \
    apt-get install -y cmake && \
    cd ~/temp &&  wget -q https://sourceforge.net/projects/boost/files/boost/1.81.0/boost_1_81_0.tar.gz && \
    tar -zxf boost_1_81_0.tar.gz && cd ~/temp/boost_1_81_0 && ./bootstrap.sh && ./b2 cxxflags="-std=c++11" --reconfigure --with-fiber --with-context --with-atomic --with-date_time --with-filesystem --with-url install && \
    cd ~/temp && git clone -b v1.15 https://github.com/linux-test-project/lcov.git && cd lcov && make install && cd .. && \
    apt-get install -y libperlio-gzip-perl libjson-perl && \
    rm -rf ~/temp/* && \
    apt-get autoremove -y &&\
    apt-get clean -y &&\
    rm -rf /var/lib/apt/lists/*
# libpq
RUN apt-get install -y libpq-dev libsqlite3-dev unzip && \
    cd ~/temp && \
    git clone https://github.com/jtv/libpqxx.git && cd libpqxx && \
    git checkout 7.7.4 && \
    mkdir build && cd build && \
    cmake .. -DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql/libpq && \
    make -j6 && make install && \
    cd ~/temp && \
    wget https://github.com/SOCI/soci/archive/refs/heads/master.zip && \
    unzip master.zip && \
    cd soci-master && \
    mkdir build && cd build && \
    cmake .. -DWITH_BOOST=ON -DWITH_POSTGRESQL=ON -DWITH_SQLITE3=ON -DCMAKE_CXX_STANDARD=14 -DSOCI_CXX11=ON && \
    make -j6 && make install && \
    # cp /usr/local/cmake/SOCI.cmake /usr/local/cmake/SOCIConfig.cmake && \
    ln -s /usr/local/lib64/libsoci_* /usr/local/lib && ldconfig && \
    rm -rf ~/temp/* && \
    apt-get autoremove -y &&\
    apt-get clean -y &&\
    rm -rf /var/lib/apt/lists/*
# Libasyik
RUN cd ~/temp  && \
    git clone https://github.com/okyfirmansyah/libasyik && \
    cd ~/temp/libasyik && \
    git submodule update --init --recursive && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j4 && \
    make install

RUN cd ~/temp && \
    git clone https://github.com/gabime/spdlog.git && \
    cd spdlog && mkdir build && cd build && \
    cmake .. && make -j

RUN apt-get install -y python3-pip curl jq

RUN mkdir /app

COPY . /app

WORKDIR /app

RUN apt-get install -y libopencv-dev

RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

ARG EXPOSE_PORT
ENV PORT=$EXPOSE_PORT

EXPOSE ${PORT}
RUN ls

RUN cd model && \
    ls

CMD ["./build/Nodeflux-Assessment"]
