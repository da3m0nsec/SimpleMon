#!/bin/bash

# User creation
sudo useradd -r -s /bin/nologin simplemond

serviceText=""
cat > $serviceText <<- EOF
[Unit]
Description=SimpleMon Client
After=network.target

[Service]
Restart=always
RestartSec=3
User=simplemond
ExecStart=/usr/sbin/simplemond-client
EOF

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
