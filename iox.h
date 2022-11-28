#pragma once

#include <sys/stat.h>
#include <stdio.h>

int io_make_dir(char* location) {
	#ifdef __unix__
		return !mkdir(location, 0777);
	#else
		return !mkdir(location);
	#endif
}

int io_dir_exists(char*  location) {
	struct stat st;
	if (stat(location, &st) == 0) {
		#ifdef __unix__
			return (st.st_mode & 0040000 != 0);
		#else
			return (st.st_mode & S_IFDIR != 0);
		#endif
	}
	return 0;
}

int io_file_exists(char*  location) {
	struct stat buffer;   
	return (stat(location, &buffer) == 0); 
}

void io_write_file(char* location, char* to_write) {
	FILE *fp;

	fp = fopen(location, "w");
	fputs(to_write, fp);
	fclose(fp);
}

void io_append_file(char* location, char* to_append) {
	FILE *fp;

	fp = fopen(location, "a");
	fputs(to_append, fp);
	fclose(fp);
}

int io_charcount(char *filename) {
	FILE* fp;

	int count = 0;
	char c;

	fp = fopen(filename, "r");

	for (c = getc(fp); c != EOF; c = getc(fp)) {
		count = count + 1;
	}

	fclose(fp);
	return count;
}

char* io_read_file(char* filename) {
	FILE* ptr;
	char ch;
	
	int charcount = io_charcount(filename) + 1;
	char* buff = malloc(sizeof(char) * charcount);

	ptr = fopen(filename, "r");

	if (ptr == NULL) {
		printf("file can't be opened\n");
	}

	int i = 0;
	while (ch != EOF) {
		ch = fgetc(ptr);
		buff[i] = ch;
		i++;
	}
	buff[i] = '\0';

	fclose(ptr);
	return buff;
}

long io_size_bytes(char* filename){
	struct stat stat_buf;
	int rc = stat(filename, &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

long io_size_kb(char*  filename){
	return io_size_bytes(filename) / 1024;
}

long io_size_mb(char*  filename){
	return io_size_bytes(filename) / 1024 / 1024;
}
