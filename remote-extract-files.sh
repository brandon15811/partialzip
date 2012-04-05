#!/bin/bash
export zipurl="http://192.168.1.146/update-cm-7.2.0-RC1-olympus-signed.zip"

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
