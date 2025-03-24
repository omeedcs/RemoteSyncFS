assignment: 
	gcc -Wall omeed_fuse_fs.c `pkg-config fuse --cflags --libs` -o omeed_fuse_fs
	g++ test_fs_script.cpp -o test_fs_script
	g++ experiment_1.cpp -o experiment_1
	touch /tmp/FUSE_REAL/foo
	./omeed_fuse_fs /tmp/FUSE_MOUNT 
	./test_fs_script
	cat /tmp/FUSE_REAL/foo
	./experiment_1
clean:
	rm omeed_fuse_fs
	rm test_fs_script
	rm /tmp/FUSE_REAL/foo
	fusermount -u /tmp/FUSE_MOUNT/
