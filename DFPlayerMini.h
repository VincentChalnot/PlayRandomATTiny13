#include <stdint.h>

#ifndef DFPlayerMini_cpp

#define DFPlayerMini_cpp

#define SB 0x7E    //start byte
#define VER 0xFF   //version
#define LEN 0x6    //number of bytes after "LEN" (except for checksum data and EB)
#define FEEDBACK 0 //no feedback requested
#define EB 0xEF    //end byte

#define NEXT_COMMAND 0x01
#define PREVIOUS_COMMAND 0x02
#define PLAY_COMMAND 0x03
#define INCREASE_VOLUME_COMMAND 0x04
#define DECREASE_VOLUME_COMMAND 0x05
#define VOLUME_COMMAND 0x06
#define EQ_COMMAND 0x07
#define PLAYBACK_COMMAND 0x08
#define STANDBY_COMMAND 0x0A
#define WAKEUP_COMMAND 0x0A
#define RESET_COMMAND 0x0C
#define PLAYBACK_COMMAND 0x0D
#define PAUSE_COMMAND 0x0E
#define RANDOM_TRACK_COMMAND 0x18

#define STATUS_QUERY 0x43

#define PLAYBACK_REPEAT 0
#define PLAYBACK_FOLDER_REPEAT 1
#define PLAYBACK_SINGLE_REPEAT 2
#define PLAYBACK_RANDOM 3

#define EQ_NORMAL 0
#define EQ_POP 1
#define EQ_ROCK 2
#define EQ_JAZZ 3
#define EQ_CLASSIC 4
#define EQ_Base 5

class DFPlayerMini
{
public:
  uint8_t commandValue = 0;
  uint8_t paramMSB = 0;
  uint8_t paramLSB = 0;
  uint8_t checksumMSB = 0;
  uint8_t checksumLSB = 0;

  void findChecksum();
  void sendCommand(uint8_t command, uint16_t parameter = 0);
  void sendData();
};

#endif
