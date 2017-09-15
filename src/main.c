#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void usage(int    argc,
           char **argv);


typedef struct _input_file_ {
  FILE    *file;
  char    *file_name;
  int8_t   bytes;
  long int size;
} input_file;

void close_files(input_file *input_files,
                 int         n_files);


int main(int argc, char *argv[]) {
  int n_files;
  input_file *input_files;
  int16_t     total_bytes = 0;
  int8_t     *buffer;
  int n_values;

  // General vars
  int   i, j;
  FILE *out_file;
  int   seek;


  if ((argc % 2 == 1) || (argc < 6)) {
    usage(argc, argv);
    return 1;
  }
  n_files = (argc - 1) / 2;


  // create array to store input files metadata
  input_files = (input_file *)malloc(sizeof(input_file) * n_files);

  // try to open all input files
  for (i = 0; i < n_files; ++i) {
    input_files[i].file_name = argv[i * 2 + 1];
    input_files[i].file      = fopen(input_files[i].file_name, "rb");

    if (input_files[i].file == NULL) {
      fprintf(stderr,
              "Was not possible to open input file '%s'\n",
              input_files[i].file_name);

      close_files(input_files, i);
      free(input_files);
      return 3;
    }
    input_files[i].bytes = atoi(argv[i * 2 + 2]);
    fseek(input_files[i].file, 0L, SEEK_END);
    input_files[i].size = ftell(input_files[i].file);
    fseek(input_files[i].file, 0L, SEEK_SET);

    if (input_files[i].size == 0) {
      fprintf(stderr,
              "The file '%s' is empty! \n",
              input_files[i].file_name);

      close_files(input_files, i);
      free(input_files);
      return 3;
    }
    total_bytes += input_files[i].bytes;
  }

  // check if file's size is compatible
  // reference
  n_values = input_files[0].size / input_files[0].bytes;

  for (i = 1; i < n_files; ++i) {
    if ((input_files[i].size / input_files[i].bytes) != n_values) {
      fprintf(stderr,
              "Invalid file size (%ld) for input file '%s'\n",
              (input_files[i].size / input_files[i].bytes),
              input_files[i].file_name);
      close_files(input_files, i);
      free(input_files);
      return 4;
    }
  }

  buffer = (int8_t *)malloc(sizeof(int8_t) * total_bytes);

  // try to open output file
  out_file = fopen(argv[argc-1], "wb");

  if (out_file == NULL) {
        fprintf(stderr, "Was not possible to create output file '%s'\n",
            argv[1]);
    return 2;
  }

  // read N_FILES input files and write on output file
  // TODO: Read all N_FILES files to memory and then write to output file
  // (check memory usage first)

  for (i = 0; i < n_values; ++i) {
    seek = 0;

    for (j = 0; j < n_files; ++j) {
      if (fread(&buffer[seek], sizeof(int8_t), input_files[j].bytes,
                input_files[j].file) != input_files[j].bytes) {
        fprintf(stderr, "Error while reading file %s. ", input_files[j].file_name);
      }
      seek += input_files[j].bytes;
    }
    fwrite(&buffer[0], sizeof(int8_t), total_bytes, out_file);
  }
  fclose(out_file);

  close_files(input_files, n_files);
  free(input_files);
  free(buffer);
  return 0;
}

void usage(int argc, char **argv) {
  fprintf(
    stderr,
    "Usage: %s file1 nbytes1 file2 nbytes2 [fileN nbytesN] output_file\n",
    argv[0]);
}

void close_files(input_file *input_files,
                 int         n_files) {
  int i;

  for (i = 0; i < n_files; ++i) {
    fclose(input_files[i].file);
    //printf("Closing file %s\n", input_files[i].file_name);
  }
}
