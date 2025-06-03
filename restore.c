#!/bin/bash

SNAPSHOT_DIR = ".snapshots"

# Check if.snapshots exists
if[!- d "$SNAPSHOT_DIR"]; then
echo "? No snapshots directory found!"
exit 1
fi

# Get list of snapshots
snapshots = ($(ls "$SNAPSHOT_DIR"))

# Check if there are any snapshots
if[${ #snapshots[@] } - eq 0]; then
echo "? No snapshots available."
exit 1
fi

# Display list
echo "Available snapshots:"
for i in "${!snapshots[@]}"; do
printf "[%d] %s\n" $((i + 1)) "${snapshots[$i]}"
done

# Ask user to choose one
read - p "Enter the number of the snapshot to restore: " choice

# Validate input
if ![[ "$choice" = ~^ [0 - 9] + $ ]] || ["$choice" - lt 1] || ["$choice" - gt "${#snapshots[@]}"]; then
echo "? Invalid choice!"
exit 1
fi

# Get selected snapshot
snapshot_name = "${snapshots[$((choice-1))]}"

# Restore snapshot using rsync
rsync - a--delete "$SNAPSHOT_DIR/$snapshot_name/" . / --exclude = "$SNAPSHOT_DIR"

echo "? Restored to snapshot: $snapshot_name"
