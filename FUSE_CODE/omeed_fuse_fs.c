#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <unistd.h>
#include <stdlib.h>


// just need to change these parameters to read different files.
static const char *base_path = "/tmp/FUSE_REAL";
static const char *real_fuse_path = "/tmp/FUSE_REAL/foo";
static const char *current_file = "/foo";

// implemented to work with foo? -> DONE
static int cs380l_getattr(const char *path, struct stat *stbuf)
{	
	int res = 0;
	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, current_file) == 0) {
		res = lstat(real_fuse_path, stbuf);
	} else 
		res = -ENOENT;
	return res;
}

// read every file in the directory that the file system is pointing to.
static int cs380l_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s%s", base_path, path);
    dp = opendir(full_path);
    while ((de = readdir(dp)) != NULL) {
        if (filler(buf, de->d_name, NULL, 0))
            break; 
    }
    closedir(dp);
    return 0;
}

static int cs380l_open(const char *path, struct fuse_file_info *fi)
{
    int fd = open(real_fuse_path, O_RDONLY);
    if (fd == -1) {
        return -errno;
    }
    close(fd);
    return 0;
}

static int cs380l_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	char *local_path = "/tmp/FUSE_REAL/foo";
	char *remote_path = "omeed@corn-flakes.cs.utexas.edu:/tmp/FUSE_MACHINE_2_FILES/foo";
	char scp_command[2048];

	snprintf(scp_command, sizeof(scp_command), "scp -i ~/.ssh/id_rsa.pub -p %s %s", remote_path, local_path);
	if (system(scp_command) == -1) {
		return -errno;
	}

	int fd; 
	fd = open(real_fuse_path, fi->flags);
	if (fd == -1) {
		return -errno;
	}
	int res;
	res = pread(fd, buf, size, offset);
	if (res == -1) {
		return -errno;
	} 
	return res;
}

static int cs380l_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) { 

	char *local_path = "/tmp/FUSE_REAL/foo";
	char *remote_path = "omeed@corn-flakes.cs.utexas.edu:/tmp/FUSE_MACHINE_2_FILES/foo";

	int fd;
	int res;
	fd = open(real_fuse_path, fi->flags);
	if (fd == -1) {
		return -errno;
	}
	res = pwrite(fd, buf, size, offset);
	if (res == -1) {
		res = -errno;
	}
	close(fd);

	char scp_command[2048];
	snprintf(scp_command, sizeof(scp_command), "scp -i ~/.ssh/id_rsa.pub -p %s %s", local_path, remote_path);
	if (system(scp_command) == -1) {
		return -errno;
	}
	return res;
}

static int cs380l_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{

	(void) path;
	(void) isdatasync;
	(void) fi;
	int res;

	if (isdatasync) {
		res = fdatasync(fi->fh);
	} else {
		res = fsync(fi->fh);
	} 

	if (res == -1) {
		return -errno;
	} 

	return 0;
}


static int cs380l_release(const char *path, struct fuse_file_info *fi)
{

	int result; 
	(void) path;
	(void) fi;

	result = close(fi->fh);
	if (result == -1) {
		return -errno;
	}
	return 0;
}

static struct fuse_operations hello_oper = {
	.getattr = cs380l_getattr,
	.readdir = cs380l_readdir,
	.open = cs380l_open,
	.read = cs380l_read,
	.write = cs380l_write,
	.fsync = cs380l_fsync,
	.release = cs380l_release
};

int main(int argc, char *argv[])
{		
	
	umask(0);
	return fuse_main(argc, argv, &hello_oper, NULL);
}