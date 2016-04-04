#include "wave.h"

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage: ./wave filename\n");
		exit(1);
	}
	
	int waveFd = makeWaveFile(argv[1]);
	close(waveFd);

	WaveFile data = readWaveFile(argv[1]);
	printf("id: %s, size: %d, format: %s\n", data.chunkId, data.chunkSize, data.format);
	printf("id: %s, size: %d\n", data.subChunk1Id, data.subChunk1Size);
	printf("sizeof(WaveFile): %d\n", sizeof(WaveFile));
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
	int four = 4;
	write(fd,&four,4);
	char format[4] = {'W','A','V','E'};
	write(fd,format,4);
	char chunkId1[4] = {'f','m','t',' '};
	write(fd,chunkId1,4);
	int sixteen = 16;
	write(fd,&sixteen,4);

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
	read(fd,&result.chunkSize,4);
	read(fd,&result.format,4);
	read(fd,&result.subChunk1Id,4);
	read(fd,&result.subChunk1Size,4);
	
	return result;
}
