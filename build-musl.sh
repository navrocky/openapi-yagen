#!/bin/bash

APP_NAME=openapi-yagen

IMAGE_NAME=${APP_NAME}

docker build --progress plain --tag ${IMAGE_NAME} -f Dockerfile.musl .

id=$(docker create ${IMAGE_NAME})
mkdir -p dist
docker cp $id:/build/cli/${APP_NAME} ./dist/
docker rm -v $id
