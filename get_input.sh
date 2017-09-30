#!/bin/sh

while true; do
	read -n1 -s command

	command="$command\n"	

	echo $command

	if [ $command = "q\n" ]; then
		break;
	fi
done
