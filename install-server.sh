#!/bin/bash

simplemon-server="[Unit]
Description=SimpleMon Server
After=mysqld.service

[Service]
Restart=always
RestartSec=3
User=simplemond
ExecStart=/
"

#copy to /usr/sbin