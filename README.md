# RemoteSyncFS

## Description

RemoteSyncFS is a distributed file system implementation using FUSE (Filesystem in Userspace) that provides transparent access to remote files. It automatically synchronizes file contents between local and remote machines, giving users the illusion of working with local files while keeping the content synchronized across systems.

<img width="678" alt="Screenshot 2025-03-23 at 10 35 29 PM" src="https://github.com/user-attachments/assets/2185de12-f8d8-49c5-a7f8-c50ac6e8b663" />

## Features

- Transparent file access through FUSE file system interface
- Automatic file synchronization between machines using SCP
- Support for standard file operations (read, write, open, etc.)
- Performance measurement tools for distributed file system evaluation

## Architecture

RemoteSyncFS implements a FUSE-based file system that intercepts standard file operations:
- When a file is read, it first fetches the latest version from the remote server
- When a file is written, it synchronizes those changes back to the remote server
- All operations appear transparent to applications accessing the files

## Requirements

- Linux-based operating system
- FUSE development libraries (libfuse-dev)
- SSH access with key-based authentication to remote machines
- C/C++ compiler (gcc/g++)

## Setup

1. Ensure you have FUSE installed:
   ```
   sudo apt-get install libfuse-dev
   ```

2. Configure SSH key-based authentication to the remote server

3. Create necessary directories:
   ```
   mkdir -p /tmp/FUSE_REAL /tmp/FUSE_MOUNT
   ```

4. Compile and run:
   ```
   make assignment
   ```

## Usage

The system mounts a FUSE filesystem at `/tmp/FUSE_MOUNT`, which provides access to files from the remote server. Any changes made to files in this directory are automatically synchronized with the remote server.

## Performance Testing

The codebase includes several experiment files to measure performance metrics:
- experiment_1.cpp: Measures latency for repeated write-read operations
- experiment_2.cpp: Additional performance measurements
- experiment_3.cpp: Additional performance measurements

## Cleanup

To unmount the filesystem and clean up:
```
make clean
```

## Implementation Details

The implementation uses:
- FUSE API to implement file system operations
- SCP for secure file transfers between machines
- Standard C/C++ I/O operations for local file handling

## Note

This implementation currently supports a single file named "foo" as a proof of concept. The paths for local and remote files are configured in the source code.
