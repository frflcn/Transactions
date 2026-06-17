#!/bin/bash
read -p "Do you really want to publish? (yes/no): " USER_INPUT

if [[ $USER_INPUT == "yes" ]] || [[ $USER_INPUT == "y" ]]; then
    PROJECT_DIR="$(dirname "$(realpath "$0")")"
    cp "$PROJECT_DIR/bin/Main" "$PROJECT_DIR/pub/Main"
    cp "$PROJECT_DIR/bin/Helper.sh" "$PROJECT_DIR/pub/Helper.sh"
    echo "Published"
else
    echo "Received: $USER_INPUT"
    echo 'Not "yes" or "y"'
    echo "Not Publishing"
    echo "Exiting..."
fi