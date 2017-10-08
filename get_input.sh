#!/bin/sh

while true; do
	read -n1 -s command	

	echo $command

	if [ $command = "q" ]; then
		break;
	fi
done
