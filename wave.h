#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int data; // file descriptor where offset is the beginning of the data
} WaveFile;

// Returns a file descriptor with WAVE metadata initialized, and offset pointing to the data section.
// Size of data is initialized to zero; set the value at offset(40) to data size and offset(4) to 
// data size + 36
int makeWaveFile(char* filename, int dataSize);

// Reads metadata from WAVE file; WaveFile.fd contains byte array containing the data.
WaveFile readWaveFile(char* filename);

void writeSineWave(int fd, double duration, double f);

// 'f' is the frequency, 't' is the number of samples since the beginning of the file.
short int z_sine(double f, int x, int totalSamples);

short basicEnvelope(int t, int samples);
short envelope(int t, int samples, double attack, double decay, double sustain, double release, double sustainPercent, int maximumAmplitude);

void readData(int fd, int size);

