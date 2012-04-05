#!/bin/bash
export zipurl=""

if [ -z $zipurl ]; then
	echo "Set \$zipurl to a zip url (or a file as a file:// url)"
	exit 1;
fi

TMP="$(mktemp)"
adb ()
{
	echo $2 >> $TMP
	echo $3 >> $TMP
}

source extract-files.sh
echo $TMP
./adb $TMP
rm $TMP
