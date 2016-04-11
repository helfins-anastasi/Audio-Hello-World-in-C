#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct _wave_file {
	char chunkId[5];
	int chunkSize;
	char format[5];
	char subChunk1Id[5];
	int subChunk1Size;
	short int audioFormat;
	short int numChannels;
	int sampleRate;
	int byteRate;
	short int blockAlign; // numChannels * (bitsPerSample / 8)
	short int bitsPerSample;
	char subChunk2Id[5];
	int subChunk2Size;
	char * data;
} WaveFile;

// Returns a file descriptor with WAVE metadata initialized, and offset pointing to the data section.
// Size of data is initialized to zero; set the value at offset(40) to data size and offset(4) to 
// data size + 36
int makeWaveFile(char* filename, int dataSize);

// Reads metadata from WAVE file; WaveFile.fd contains byte array containing the data.
WaveFile readWaveFile(char* filename);
