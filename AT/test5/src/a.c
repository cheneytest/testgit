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

/** Separates 4 bits into 16 bits (0xF000 -> 0xC0C0 or 0x3030 or 0x0C0C or 0x0303)
t = target, s = source, n = shift amount (logical channel number multiplied by two) */
#define PUT_4_4(t, s, n) ((t) |= ((((s) & 0xC000) >> (n)) | (((s) & 0x3000) >> ((n) + 6))))



int main(int argc, char *argv[])
{
  uint16 in[RAINBOW_SIZE_WORDS] = {0};
  uint16 out[RAINBOW_SIZE_WORDS] = {0};
  uint16 unused[RAINBOW_SIZE_WORDS] = {0};
  uint16 *in_c[4] = {in, unused, unused, unused};
  uint16 channels = 4;
  char *input_file_name = NULL;
  char *output_file_name = NULL;
  int fd_in;
  int fd_out;

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

  (void) memset(unused, -1, RAINBOW_SIZE_OCTETS);

  while (read(fd_in, in, RAINBOW_SIZE_OCTETS/channels) == RAINBOW_SIZE_OCTETS/channels) {
    swap_array(in, RAINBOW_SIZE_OCTETS/channels);
    if (channels == 4) {
      combine_4_of_4_channels(in_c, out);
    } else if (channels == 2) {
      combine_2_of_2_channels(in_c, out);
    }
    swap_array(out, RAINBOW_SIZE_OCTETS);
    write(fd_out, out + 0, RAINBOW_SIZE_OCTETS);
  }

  close(fd_in);
  close(fd_out);

  return 0;
}
