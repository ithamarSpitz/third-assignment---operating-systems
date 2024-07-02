#!/bin/bash

send_commands() {
    local file=$1
    local delay=$2
    sleep $delay  # Initial delay to sync clients
    while IFS= read -r line
    do
        echo "Sending: $line"
        echo "$line" | nc -q 1 localhost 9034
        echo "Response received"
        sleep 0.5  # Small delay between commands
    done < "$file"
}

# Start both clients simultaneously
send_commands client1_commands.txt 0 > client1_output.txt 2>&1 &
send_commands client2_commands.txt 0.25 > client2_output.txt 2>&1 &

# Wait for both clients to finish
wait

echo "Test completed. Check client1_output.txt and client2_output.txt for results."