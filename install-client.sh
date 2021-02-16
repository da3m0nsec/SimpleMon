#!/bin/bash

simplemon-client="[Unit]
Description=SimpleMon Client
After=network.target

[Service]
Restart=alwayss
RestartSec=2
User=simplemond
ExecStart=/
"

#copy to /usr/sbin
