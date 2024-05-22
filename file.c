
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() 
{


    char *filename = "heredoc";

  // Open the file for writing with create or overwrite permissions
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

  // Check if the file was opened successfully
  if (fd == -1) {
    perror("open");
    return 1;
  }

  // Prepare the data to write
  const char *data1 = "This is some data 333 to write to the file.\n";
  const char *data2 = "This is some data 222 to write to the file.\n";

  // Write the data to the file
  ssize_t bytes_written = write(fd, data1, strlen(data1));
    if (bytes_written == -1) {
        perror("write");
        close(fd);
        return 1;
    }
   bytes_written = write(fd, data2, strlen(data2));

  // Check for errors during writing
  if (bytes_written == -1) {
    perror("write");
    close(fd);
    return 1;
  }

  // Close the file
  close(fd);

  printf("Successfully wrote %ld bytes to %s\n", bytes_written, filename);

  return 0;
}
