#!/bin/bash

run_client() {
    local file=$1
    while IFS= read -r line
    do
        echo "Sending: $line"
        echo "$line" | nc -q 1 localhost 9034
        echo "Response received"
        sleep 1  # Wait for 1 second between commands
    done < "$file"
}

# Run clients in background
run_client client1_commands.txt > client1_output.txt 2>&1 &
PID1=$!
sleep 0.5  # Small delay to offset the clients
run_client client2_commands.txt > client2_output.txt 2>&1 &
PID2=$!

# Wait for both clients to finish
wait $PID1
wait $PID2

echo "Test completed. Check client1_output.txt and client2_output.txt for results."