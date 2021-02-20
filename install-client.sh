#!/bin/bash

if (( EUID != 0 )); then
   echo "You must be root to do this." 1>&2
   exit 1
fi



# User creation
useradd -r -s /bin/nologin simplemond

simplemon-client="[Unit]
Description=SimpleMon Client
After=network.target

[Service]
Restart=always
RestartSec=3
User=simplemond
ExecStart=/
"

# Copy to /usr/sbin

