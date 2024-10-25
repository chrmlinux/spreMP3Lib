//
// name           : MP3.4.ino
// date/author    : 2024/10/25 @chrmlinux03
// update/author  : 2024/10/25 @chrmlinux03
//
//
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
