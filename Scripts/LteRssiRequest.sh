#!/bin/bash

function send_rssi_request() {
	local device=$1
	echo -ne "AT+CSQ\r\n" > $device
}


function receive_rssi_response() {
	local timeout=$1
	local device=$2

	local response=`sed '/OK\|ERROR/q' < $device & sleep $timeout && killall -q sed`
	echo "$response"
}


function get_status() {
	local response=$1
	
	if [[ $response == *"CSQ"* ]]
	then
		echo "OK"
	else
		echo "NOK"
	fi
}


function extract_rssi_value() {
	local response=($1)

	local result=`echo ${response[2]}`
	local rssi=${result%,*}	

	echo $rssi
}


function get_rssi_value() {
	local device=$1

	local response=""
	for i in {1..10}
	do
		# Send request to modem
		send_rssi_request $device

		# Read response
		response=`receive_rssi_response 0.1 $device`

		# Get response status		
		local status=`get_status "$response"`

		# Extract RSSI value if response is OK
		if [[ $status == "OK" ]]
		then
			# CSQ Response is OK
			local result=`extract_rssi_value "$response"`
			echo $result

			return
		fi
	done

	echo 99
}


function rssi_value_to_dbm() {
	local rssi_value=$1

	local dbm_base=-113
	
	if [[ $rssi_value == 99 ]]
	then
		echo -1000
	else
		echo $(( dbm_base + rssi_value * 2 ))
	fi
}


# Get RSSI value
lte_device=$1

lte_rssi_value=`get_rssi_value $lte_device`
lte_dbm_rssi_value=`rssi_value_to_dbm $lte_rssi_value`
echo $lte_dbm_rssi_value
