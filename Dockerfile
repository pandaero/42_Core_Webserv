FROM alpine:latest
WORKDIR /usr/myApp
#COPY . WORKDIR
RUN apk add --no-cache makeRUN apk add build-base

RUN ["make", "run"]