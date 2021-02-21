#!/bin/bash

# Dependencies 
sudo apt install cmake=3.5

# User creation
sudo useradd -r -s /bin/nologin simplemond

serviceText="[Unit]\n"
serviceText+="Description=SimpleMon Client\n"
serviceText+="After=network.target\n"
serviceText+="[Service]\n"
serviceText+="Restart=always\n"
serviceText+="RestartSec=3\n"
serviceText+="User=simplemond\n"
serviceText+="ExecStart=/usr/sbin/simplemond-client\n"

# Compile
mkdir build
cd build
cmake ..
make client

# Copy program to /usr/sbin
sudo cp client /usr/sbin/simplemond-client

# Copy config to /etc/simplemon-client
sudo mkdir /etc/simplemon-client
sudo cp ../config/client.conf /etc/simplemon-client

# Copy service
sudo echo $serviceText > /etc/systemd/system/simplemond-client

systemctl start simplemond-client
systemctl enable simplemond-client
