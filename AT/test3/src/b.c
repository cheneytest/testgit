#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define UPD_HEADER_LEN 4

/** Separates 8 bits into 16 bits (0xFF00 -> 0xCCCC or 0x3333)
t = target, s = source, n = shift amount (logical channel number multiplied by two) */
#define PUT_8_2(t, s, n) ((t) |=\
(((s) & 0xC000) >> ((n) + 0)) |\
(((s) & 0x3000) >> ((n) + 2)) |\
(((s) & 0x0C00) >> ((n) + 4)) |\
(((s) & 0x0300) >> ((n) + 6)))


// Separates 4 bits into 16 bits (0xF000 -> 0xC0C0 or 0x3030 or 0x0C0C or 0x0303)
//t = target, s = source, n = shift amount (logical channel number multiplied by two)
#define PUT_4_4(t, s, n) ((t) |= ((((s) & 0xC000) >> (n)) | (((s) & 0x3000) >> ((n) + 6))))



int main(int argc, char *argv[])
{
  uint8 in[RAINBOW_SIZE_OCTETS] = {0};
  uint8 upd_header[UPD_HEADER_LEN] = {0};
  uint8 out[RAINBOW_SIZE_OCTETS] = {0};
  char *input_file_name = NULL;
  char *output_file_name = NULL;
  int fd_in;
  int fd_out;
  int read_size;
  

  if (argc != 3) {
    printf("Usage: %s input_file_name output_file_name", argv[0]);
    return 0;
  } else {
    input_file_name = argv[1];
    output_file_name = argv[2];
  }

  fd_in = open(input_file_name, O_RDONLY | O_BINARY, 0);

  if (fd_in == -1) {
    printf("Error: failed to open input file (%s)!\n", input_file_name);
    return 1;
  }

  fd_out = open(output_file_name, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, 0);

  if (fd_out == -1) {
    printf("Error: failed to create output file (%s)!\n", output_file_name);
    return 1;
  }

  (void) memset(out, 0, RAINBOW_SIZE_OCTETS);

  /* close file */
  close(fd_in);
  close(fd_out);

  return 0;
}