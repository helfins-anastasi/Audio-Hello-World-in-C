#include "wave.h"

int main(int argc, char** argv) {
	if(argc < 3) {
		printf("Usage: ./wave [r|w|t] filename\n");
		exit(1);
	}	
	char mode = argv[1][0];
	if(mode != 'r' && mode != 't' && mode != 'w') {
		printf("Usage: ./wave [r|w|t] filename\n");
		exit(4);
	}

	if(mode == 'w' || mode == 't') {
		int waveFd = makeWaveFile(argv[2]);
		close(waveFd);
	}

	if(mode == 'r' || mode == 't') {
		WaveFile data = readWaveFile(argv[1]);
		printf("id: %s, size: %d, format: %s\n", data.chunkId, data.chunkSize, data.format);
		printf("id: %s, size: %d, aFormat: %d, nChann: %d, sRate: %d, bRate: %d, bAlign: %d, bps: %d\n", data.subChunk1Id, data.subChunk1Size, data.audioFormat, data.numChannels, data.sampleRate, data.byteRate, data.blockAlign, data.bitsPerSample);
		printf("id: %s, size: %d\n", data.subChunk2Id, data.subChunk2Size);
	}

}

int makeWaveFile(char* filename) {
	int fd = open(filename, O_WRONLY | O_EXCL | O_CREAT, S_IRWXU);
	if(fd < 0 && errno == 17) {
		//File already exists
		printf("File \"%s\" already exists.\n", filename);
		return -1;
	} else if(fd < 0) {
		printf("Unknown error #%d\n", errno);
		return -1;
	}
	char chunkId[4] = {'R','I','F','F'};
	write(fd,chunkId,4);
	int dataSize = 0;
	int size = 36 + dataSize;
	write(fd,&size,4);
	char format[4] = {'W','A','V','E'};
	write(fd,format,4);
	char chunkId1[4] = {'f','m','t',' '};
	write(fd,chunkId1,4);
	int sixteen = 16;
	write(fd,&sixteen,4);
	short audioFormat = 0x0001; // Linear pulse code modulation (see http://www.digitalpreservation.gov/formats/fdd/fdd000002.shtml)
	write(fd,&audioFormat,2);
	short numChannels = 1;
	write(fd,&numChannels,2);
	int sampleRate = 8000;
	write(fd,&sampleRate,4);
	int byteRate = 16000;
	write(fd,&byteRate,4);
	short blockAlign = 2;
	write(fd,&blockAlign,2);
	short bitsPerSample = 16;
	write(fd,&bitsPerSample,2);
	char chunkId2[4] = {'d','a','t','a'};
	write(fd,&chunkId2,4);
	write(fd,&dataSize,4);
	int zero = 0, i = 0;
	for(i = 0; i < dataSize - 4; i += 4) {
		write(fd,&zero,4);
	}
	write(fd,&zero,dataSize-i);
	return fd;
}

WaveFile readWaveFile(char* filename) {
	WaveFile result;
	int fd = open(filename, O_RDONLY);
	if(fd < 0) {
		printf("Unknown error #%d\n", errno);
		exit(2);
	}

	read(fd,&result.chunkId,4);
	result.chunkId[4] = 0;
	read(fd,&result.chunkSize,4);
	read(fd,&result.format,4);
	result.format[4] = 0;
	read(fd,&result.subChunk1Id,4);
	result.subChunk1Id[4] = 0;
	read(fd,&result.subChunk1Size,4);
	if(result.subChunk1Size < 16) {
		printf("Error: Subchunk 1 must be at least 16 bytes\n");
		exit(3);
	}
	result.audioFormat = 0;
	read(fd,&result.audioFormat,2);
	result.numChannels = 0;
	read(fd,&result.numChannels,2);
	read(fd,&result.sampleRate,4);
	read(fd,&result.byteRate,4);
	result.blockAlign = 0;
	read(fd,&result.blockAlign,2);
	result.bitsPerSample = 0;
	read(fd,&result.bitsPerSample,2);
	read(fd,&result.subChunk2Id,4);
	result.subChunk2Id[4] = 0;
	read(fd,&result.subChunk2Size,4);
	result.data = (char *)malloc(result.subChunk2Size);
	read(fd,result.data,result.subChunk2Size);	

	close(fd);
	return result;
}
