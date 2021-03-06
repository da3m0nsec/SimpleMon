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
serviceText+="ExecStart=/usr/sbin/simplemon-server\n"
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
sudo cp server /usr/sbin/simplemon-server

# Copy config to /etc/simplemon-server
sudo mkdir /etc/simplemon-server
sudo mkdir /etc/simplemon-server/config
sudo mkdir /etc/simplemon-server/keys
sudo cp ../config/server.conf /etc/simplemon-server/config/
sudo chown simplemond /etc/simplemon-server/config/server.conf
sudo chmod 600 /etc/simplemon-server/config/server.conf

sudo cp ../config/sql.conf /etc/simplemon-server/config/
sudo chown simplemond /etc/simplemon-server/config/sql.conf
sudo chmod 600 /etc/simplemon-server/config/sql.conf

# Copy service
sudo rm /etc/systemd/system/simplemon-server.service
echo -e $serviceText | sudo tee -a /etc/systemd/system/simplemon-server.service >/dev/null

systemctl start simplemon-server
systemctl enable simplemon-server

#prepare sql
#create user 'simplemond'@'localhost' identified by 'xxxx';
#grant all privileges on SimpleMon.* to 'simplemond'@'localhost';

#prepare keys
#botan keygen --algo=RSA --params= --passphrase= --pbe= --pbe-millis=300 --provider= --der-out
#botan pkcs8 --pass-in= --pub-out --der-out --pass-out= --pbe= --pbe-millis=300 priv.key