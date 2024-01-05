# Creating-a-Tape-Archive-using-File-Manipulation
The learning objectives include understanding directory entries and file attributes, as well as performing complex file input/output and manipulation operations. The project involves practicing mechanisms such 
as buffered I/O functions (fopen(), fclose(), fread(), fwrite(), fseek(), feof()), reading directory entries (opendir(), readdir(), closedir()), and handling file metadata (stat()/lstat(), chmod(), utimes(), 
gettimeofday()). The program, named "mytar," is designed to create and manipulate tape archives. It supports options for creating archives (-c), extracting archives (-x), printing archive contents and details (-t), 
and specifying the archive file using the -f option.

The implementation details include a specific mytar file format with a magic number, inode number, filename length, filename, mode, modification time, and additional information for regular files. The printing mode 
displays information for directories, regular files, executable files, and hard links.

The project specifies error handling with corresponding error messages and the use of perror() for library/system call failures. Examples of usage and implementation details, such as file modes, modification times, 
and handling hard links, are provided.

The use of provided inodemap.c and inodemap.h for tracking hard links' inodes and the usage of string manipulation functions like strncpy(), strncat(), strlen(), and strcmp(). 
