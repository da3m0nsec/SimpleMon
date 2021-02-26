#!/bin/bash

# Dependencies (build botan)
#sudo apt install cmake=3.5

# User creation
sudo useradd -r -s /bin/nologin simplemond

serviceText="[Unit]\n"
serviceText+="Description=SimpleMon Server\n"
serviceText+="After=mysql.service\n"
serviceText+="[Service]\n"
serviceText+="Restart=always\n"
serviceText+="RestartSec=3\n"
serviceText+="User=simplemond\n"
serviceText+="ExecStart=/usr/sbin/simplemond-server\n"
serviceText+="[Install]\n"
serviceText+="WantedBy=multi-user.target\n"

# Compile
mkdir build
cd build
cmake ..
make server

# Check build exit code
if [ $? -ne 0 ]
then
   echo "Build failed, exiting" >&2
   exit
fi

# Copy program to /usr/sbin
sudo cp server /usr/sbin/simplemond-server   

# Copy config to /etc/simplemon-server
sudo mkdir /etc/simplemon-server
sudo cp ../config/server.conf /etc/simplemon-server

# Copy service
sudo rm /etc/systemd/system/simplemond-server.service
echo -e $serviceText | sudo tee -a /etc/systemd/system/simplemond-server.service >/dev/null

systemctl start simplemond-server
systemctl enable simplemond-server

#prepare sql