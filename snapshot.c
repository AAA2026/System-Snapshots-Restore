// snapshot.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main() {
    // Step 1: Get current time and format it
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", tm_info);

    // Step 2: Create snapshot directory path
    char snapshot_dir[128];
    snprintf(snapshot_dir, sizeof(snapshot_dir), ".snapshots/snapshot_%s", timestamp);

    // Step 3: Create the directory if needed
    char mkdir_cmd[256];
    snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p \"%s\"", snapshot_dir);
    system(mkdir_cmd);

    // Step 4: Use rsync to copy everything excluding `.snapshots`
    char rsync_cmd[512];
    snprintf(rsync_cmd, sizeof(rsync_cmd),
        "rsync -a --exclude=\".snapshots\" ./ \"%s\"", snapshot_dir);

    int result = system(rsync_cmd);

    // Step 5: Print success message
    if (result == 0) {
        printf("? Snapshot saved as %s\n", snapshot_dir);
    }
    else {
        printf("? Failed to create snapshot.\n");
        return 1;
    }

    return 0;
}
