# spreMP3Lib

## Description
`spreMP3Lib` is a simple library for playing MP3 files on the Spresense platform. It allows users to play audio files stored on an SD card and provides basic playback controls such as play, pause, next, and previous.

## Features
- Play MP3 files from an SD card.
- Basic playback controls: play, pause, next, previous, and stop.
- Easy integration with the Spresense audio system.

## Installation
1. Download the library from [GitHub](https://github.com/chrmlinux/spreMP3Lib).
2. Copy the `spreMP3Lib` folder into your Arduino libraries directory (usually located at `Documents/Arduino/libraries`).
3. Restart the Arduino IDE.

## Usage

### Example Code

```cpp
#include <SDHCI.h>
SDClass theSD;
File myFile;
#include "spreMP3Lib.hpp"

spreMP3Lib sp3;
#define MISIC_DIR "music"

void setup() {
  Serial.begin(115200);
  while (!theSD.begin()) {
    Serial.println("Insert the SD card.");
  }
  sp3.begin();
  sp3.setDebugMode(false);
  File root = theSD.open(MISIC_DIR); sp3.fileDir(root);
  Serial.printf("trackIndexCnt: %d\n", sp3.getFileCount());
  int idx = sp3.setIndex(0);
  sp3.play(sp3.getFileName(idx));
}

void loop() {
  static uint16_t pTrack = 9999;
  uint16_t Track = sp3.isTrackIndex();
  if (pTrack != Track) {
    pTrack = Track;
    Serial.printf("track:%d |%s|\n", Track, sp3.getFileName(Track));
  }
  sp3.loop();
}
```

### Functions

- `void begin(void)`: Initializes the audio system.
- `void loop(void)`: Handles the audio playback loop.
- `void fileDir(File dir)`: Scans the specified directory for MP3 files.
- `void play(char* newFileName)`: Plays the specified MP3 file.
- `void pause(void)`: Pauses the playback.
- `void resume(void)`: Resumes the playback.
- `void next(void)`: Plays the next track.
- `void prev(void)`: Plays the previous track.
- `void stop(void)`: Stops the playback.
- `uint16_t getFileCount() const`: Returns the number of available tracks.
- `uint16_t setIndex(uint16_t index)`: Sets the current track index.
- `uint16_t isTrackIndex(void)`: Returns the current track index.
- `bool setDebugMode(bool flag)`: Enables or disables debug mode.
- `char* getFileName(uint16_t index)`: Returns the file name of the track at the specified index.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author
- **chrmlinux** - [GitHub Profile](https://github.com/chrmlinux)
