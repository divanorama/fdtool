#!/bin/bash -e

echo "test.sh args: $@"
(echo "Will write message to >&8" && echo "message content" >&8 && echo "Wrote") &
(echo "Will read message from <&7" && head -n 1 <&7 && echo "Read") &
wait
