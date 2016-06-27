#!/usr/bin/env bash
#You can launch this tools using osin/lamp Docker image
#https://hub.docker.com/r/osin/lamp/

docker run --name intranet -v $(pwd)/web:/var/www/html -v $(pwd)/mysql:/var/lib/mysql -p 80:80 -p 3306:3306 osin/lamp