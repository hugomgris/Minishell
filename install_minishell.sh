#!/bin/bash

# Script to switch the symlink for minishell in ~/bin
# Usage: ./switch_minishell_symlink.sh /path/to/new/minishell

# Check if the user provided a target path
if [ "$#" -ne 1 ]; then
  echo "Usage: $0 /path/to/new/minishell"
  exit 1
fi

NEW_TARGET="$1"
SYMLINK_PATH="$HOME/bin/minishell"

# If the user provides '.' or '$PWD', resolve it to the full path of the current directory
if [ "$NEW_TARGET" == "." ] || [ "$NEW_TARGET" == "\$PWD" ]; then
  NEW_TARGET="$(pwd)/minishell"
fi

# Debugging: print the target path
echo "New target: $NEW_TARGET"

# Check if the new target exists and is executable
if [ ! -f "$NEW_TARGET" ]; then
  echo "Error: Target '$NEW_TARGET' does not exist."
  exit 1
fi

if [ ! -x "$NEW_TARGET" ]; then
  echo "Error: Target '$NEW_TARGET' is not executable."
  exit 1
fi

# Create or update the symlink
ln -sf "$NEW_TARGET" "$SYMLINK_PATH"

# Verify the symlink was updated
if [ "$(readlink "$SYMLINK_PATH")" = "$NEW_TARGET" ]; then
  echo "Successfully updated symlink: $SYMLINK_PATH -> $NEW_TARGET"
else
  echo "Error: Failed to update the symlink."
  exit 1
fi
