#!/bin/bash

simplemon-client="[Unit]
Description=SimpleMon Client
After=network.target

[Service]
Restart=always
RestartSec=3
User=simplemond
ExecStart=/
"

#copy to /usr/sbin
