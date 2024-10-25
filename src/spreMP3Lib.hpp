//
// name           : MP3.3.ino
// date/author    : 2024/10/25 @chrmlinux03
// update/author  : 2024/10/25 @chrmlinux03
//
// *The sampling rates for MP3 are 32 Kbps, 44 Kbps, and 48 Kbps.
//
#ifndef __SPREMP3LIB_HPP__
#define __SPREMP3LIB_HPP__

#include <Arduino.h>
#include <Audio.h>

#define FNAME_MAX (100)

class spreMP3Lib {
  public:
    spreMP3Lib() {
      trackIndexCnt = 0;
      trackIndexNow = 0;
      isPaused = false;
    };

    ~spreMP3Lib() {
      if (myFile) myFile.close();
    };

    void begin(void) {
      err_t err = 0;
      theAudio = AudioClass::getInstance();
      theAudio->begin(audioAttentionCB);
      theAudio->setRenderingClockMode(AS_CLKMODE_NORMAL);
      theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, AS_SP_DRV_MODE_LINEOUT);
    }

    void loop(void) {
      err_t err = 0;
      err = theAudio->writeFrames(AudioClass::Player0, myFile);
      if (err == AUDIOLIB_ECODE_FILEEND) {
        if (isDebug) Serial.println("Main player: End of file!");
        stop();
        next();
        return;
      }
      if (err) {
        Serial.printf("Main player error code: %d\n", err);
        handleError("An error occurred");
      }
      if (errEnd) {
        Serial.println("Error ending");
        handleError("An error occurred");
      }
      usleep(1000);
    }

    void fileDir(File dir) {
      while (true) {
        File entry = dir.openNextFile();
        if (!entry) break;
        if (entry.isDirectory()) {
          fileDir(entry);
        } else {
          if (strstr(entry.name(), ".mp3")) {
            String fileName = String(entry.name());
            fileName.replace("/mnt/sd0", "");
            strcpy(fName[trackIndexCnt], fileName.c_str());
            if (isDebug) Serial.printf("[%s] %d byte\n", fName[trackIndexCnt], entry.size());
            trackIndexCnt++;
          }
        }
        entry.close();
      }
    }

    void play(char* newFileName) {
      stop();

      err_t err = theAudio->initPlayer(AudioClass::Player0, AS_CODECTYPE_MP3,
                                       "/mnt/sd0/BIN", AS_SAMPLINGRATE_AUTO, AS_CHANNEL_STEREO);
      if (err != AUDIOLIB_ECODE_OK) {
        handleError("initPlayer:Player0 initialization error !");
        return;
      }

      fileName = newFileName;
      myFile = theSD.open(fileName);
      if (!myFile) {
        handleError("fileOpen:file open error !");
        return;
      }

      if (isDebug) Serial.printf("->play playing : [%s]\n", fileName);
      err = theAudio->writeFrames(AudioClass::Player0, myFile);
      if ((err != AUDIOLIB_ECODE_OK) && (err != AUDIOLIB_ECODE_FILEEND)) {
        Serial.printf("File Read Error! =%d\n", err);
        myFile.close();
        return;
      }

      theAudio->setVolume(-160);
      theAudio->startPlayer(AudioClass::Player0);
      isPaused = false;
    }

    void pause(void) {
      if (!isPaused) {
        stop();
        isPaused = true;
        if (isDebug) Serial.println("Playback paused.");
      }
    }

    void resume(void) {
      if (isPaused) {
        play(fName[trackIndexNow]);
        isPaused = false;
        if (isDebug) Serial.println("Playback resumed.");
      }
    }

    void next(void) {
      if (trackIndexCnt > 0) {
        stop();
        trackIndexNow = (trackIndexNow + 1) % trackIndexCnt;
        play(fName[trackIndexNow]);
      }
    }

    void prev(void) {
      if (trackIndexCnt > 0) {
        stop();
        trackIndexNow = (trackIndexNow - 1 + trackIndexCnt) % trackIndexCnt;
        play(fName[trackIndexNow]);
      }
    }

    void stop(void) {
      if (myFile) {
        myFile.close();
        theAudio->stopPlayer(AudioClass::Player0);
      }
    }

    uint16_t getFileCount() const {
      return trackIndexCnt;
    }

    uint16_t setIndex(uint16_t index) {
        trackIndexNow = index;
      return trackIndexNow;
    }

    uint16_t isTrackIndex(void) {
      return trackIndexNow;
    }
    
    bool setDebugMode(bool flag) {
      isDebug = flag;
      return isDebug;
    }

    char* getFileName(uint16_t index) {
      if (index < trackIndexCnt) {
        return fName[index];
      }
      return nullptr;
    }

  private:
    void handleError(const char* message) {
      Serial.printf("%s\n", message);
      if (myFile) myFile.close();
      theAudio->setReadyMode();
      theAudio->end();
    }

    static void audioAttentionCB(const ErrorAttentionParam* atprm) {
      Serial.println("Attention!");
      if (atprm->error_code >= AS_ATTENTION_CODE_WARNING) {
        errEnd = true;
      }
    }

    AudioClass* theAudio;
    static bool errEnd;
    char* fileName;
    char fName[FNAME_MAX][64];
    uint16_t trackIndexCnt;
    uint16_t trackIndexNow;
    bool isPaused;
    bool isDebug = false;
};

bool spreMP3Lib::errEnd = false;

#endif // __SPREMP3LIB_HPP__
