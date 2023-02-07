#ifndef IOX_H
#define IOX_H

#include <sys/stat.h>
#include <stdio.h>

/* creates a directory */
/* return value of 1 indicates success and a return value of 0 indicates failure */
int io_make_dir(char* location) {
	#ifdef __unix__
		return !mkdir(location, 0777);
	#else
		return !mkdir(location);
	#endif
}

/* checks if a directory exists, returns a boolean */
int io_dir_exists(char* location) {
	struct stat st;
	if (stat(location, &st) == 0) {
		/* on linux the S_IFDIR macro isnt defined in the standard library so we just use its raw value when on unix-based platforms */
		#ifdef __unix__
			return (st.st_mode & 0040000 != 0);
		#else
			return (st.st_mode & S_IFDIR != 0);
		#endif
	}
	return 0;
}

/* checks if a file exists, returns a boolean */
int io_file_exists(char* location) {
	struct stat buffer;   
	return (stat(location, &buffer) == 0); 
}

/* write a string to a file */
/* return value of 1 indicates success and a return value of -1 indicates failure */
int io_write_file(char* location, char* to_write) {
	/* error checking */
	FILE* file = fopen(location, "w");
	if (file == NULL) {
		return -1; /* indicates failure */
	}

	/* writing */
	fputs(to_write, file);

	fclose(file);
	return 1; /* indicates success */
}

/* appends a string to a file */
/* return value of 1 indicates success and a return value of -1 indicates failure */
int io_append_file(char* location, char* to_append) {
	/* error check*/
	FILE* file = fopen(location, "a");
	if (file == NULL) {
		return -1; /* indicates failure */
	}

	/* append */
	fputs(to_append, file);
	
	fclose(file);
	return 1; /* indicates success*/
}

/* return how many characters are in a file */
/* a return value of -1 means there was an error reading the file */
int io_charcount(char* filename) {
	/* error checking*/
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		return -1; /* indicates failure */
	}

	/* getting how many characters there are with just this simple for loop */
	int count = 0;
	char c;

	for (c = getc(file); c != EOF; c = getc(file)) {
		count++;
	}

	/* close the file */
	fclose(file);
	return count;
}

/* writes the entire file to the char buffer provided in the second argument */
/* return value of 1 indicates success and a return value of -1 indicates failure */
int io_read_file(char* filename, char* buf) {
	FILE* file = fopen(filename, "r");

	if (file == NULL) {
		return -1;
	}

	char c;
	int i;
	for (i = 0; c != EOF; i++) {
		c = fgetc(file);
		buf[i] = c;
	}
	buf[i] = '\0';

	fclose(file);
	return 1;
}

/* returns the amount of bytes a file takes up */
long io_size_bytes(char* filename){
	struct stat stat_buf;
	int rc = stat(filename, &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

/* returns the amount of kilobytes a file takes up, uses io_size_bytes and some math to figure this out */
long io_size_kb(char* filename){
	return io_size_bytes(filename) / 1024;
}

/* returns the amount of megabytes a file takes up, uses io_size_bytes and some math to figure this out */
long io_size_mb(char* filename){
	return io_size_bytes(filename) / 1024 / 1024;
}

#endif /* IOX_H */