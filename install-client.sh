#!/bin/bash

# Dependencies 
#sudo apt install cmake=3.5

# User creation
sudo useradd -r -s /bin/nologin simplemond

serviceText="[Unit]\n"
serviceText+="Description=SimpleMon Client\n"
serviceText+="After=network.target\n"
serviceText+="[Service]\n"
serviceText+="Restart=always\n"
serviceText+="RestartSec=3\n"
serviceText+="User=simplemond\n"
serviceText+="ExecStart=/usr/sbin/simplemon-client\n"
serviceText+="[Install]\n"
serviceText+="WantedBy=multi-user.target\n"

# Compile
mkdir build
cd build
cmake ..
make client

# Check build exit code

if [ $? -ne 0 ]
then
   echo "Build failed, exiting" >&2
   exit
fi

# Copy program to /usr/sbin
sudo cp client /usr/sbin/simplemon-client   

# Copy config to /etc/simplemon-client
sudo mkdir /etc/simplemon-client
sudo mkdir /etc/simplemon-client/config
sudo mkdir /etc/simplemon-client/keys
sudo cp ../config/client.conf /etc/simplemon-client/config/
sudo chown simplemond /etc/simplemon-client/config/client.conf
sudo chmod 600 /etc/simplemon-client/config/client.conf

# Copy service
sudo rm /etc/systemd/system/simplemon-client.service
echo -e $serviceText | sudo tee -a /etc/systemd/system/simplemon-client.service >/dev/null

systemctl start simplemon-client
systemctl enable simplemon-client
