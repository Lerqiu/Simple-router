#!/bin/bash
sudo ip link set enp-nat up
sudo dhclient -v enp-nat
