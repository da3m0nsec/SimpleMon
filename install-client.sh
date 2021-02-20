#!/bin/bash

# User creation
su useradd -r -s /bin/nologin simplemond

serviceText="[Unit]
Description=SimpleMon Client
After=network.target

[Service]
Restart=always
RestartSec=3
User=simplemond
ExecStart=/usr/sbin/simplemond-client
"

# Compile
mkdir build
cd build
cmake ..
make client

# Copy program to /usr/sbin
su cp client /usr/sbin/simplemond-client

# Copy config to /etc/simplemon-client
su mkdir /etc/simplemon-client
su cp ../config/client.conf /etc/simplemon-client

# Copy service
su echo $serviceText > /etc/systemd/system/simplemond-client

systemctl start simplemond-client
systemctl enable simplemond-client
