FROM alpine:latest
WORKDIR /usr/myApp
#COPY . WORKDIR

RUN ["make", "&&", "./webserv"]