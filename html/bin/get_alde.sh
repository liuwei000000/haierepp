#!/bin/sh
#set -x
while true
do
	A="";
	(
	sleep 3;
	killall alde > /dev/null 2 >&1
	)&
	A=$(/bin/alde)
	sleep 3
	echo $A > /www/123.txt
done






