#!/usr/bin/bash

scp src/cat/s21_cat root@172.24.116.8:/usr/local/bin
scp src/grep/s21_grep root@172.24.116.8:/usr/local/bin

ssh root@172.24.116.8 ls -lan /usr/local/bin/
