FROM gcc:latest

RUN apt-get update && apt-get install
RUN apt-get install -y libcurl4-openssl-dev
RUN apt-get install -y libjson-c-dev
COPY . /usr/src/app

WORKDIR /usr/src/app/

CMD bash init.sh