// Specifically for use with the Adafruit Feather, the pins are pre-set here!

// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

#define VS1053_RESET -1 // VS1053 reset pin (not used!)
// * Feather M4 M0, 328, or 32u4
#define VS1053_CS 6   // VS1053 chip select pin (output)
#define VS1053_DCS 10 // VS1053 Data/command select pin (output)
#define CARDCS 5      // Card chip select pin
#define VS1053_DREQ 9 // VS1053 Data request, ideally an Interrupt pin

#define TRIGGER 19

// * setting up vars
Adafruit_VS1053_FilePlayer musicPlayer =
    Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

uint8_t volumeLevel = 0;

void setup()
{
    Serial.begin(115200);

    Serial.println("\n\nAdafruit VS1053 Feather Test");

    attachInterrupt(TRIGGER, markTriggerTrue, RISING);

    if (!musicPlayer.begin())
    { // initialise the music player
        Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
        while (1)
            ;
    }

    Serial.println(F("VS1053 found"));

    // musicPlayer.sineTest(0x44, 500); // Make a tone to indicate VS1053 is working

    if (!SD.begin(CARDCS))
    {
        Serial.println(F("SD failed, or not present"));
        while (1)
            ; // don't do anything more
    }
    Serial.println("SD OK!");

    // list files
    // printDirectory(SD.open("/"), 0);

    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT); // DREQ int

    // Set volume for left, right channels. lower numbers == louder volume!
    musicPlayer.setVolume(volumeLevel, volumeLevel);
}

boolean playTrigger = false;

void markTriggerTrue()
{
    playTrigger = true;
}

void loop()
{
    Serial.print("trigger: ");
    Serial.println(playTrigger);
    if (playTrigger == true)
    {
        playTrigger = false;
        Serial.println("top of loop");
        musicPlayer.stopPlaying();
        playSound();
        delay(300);
    }
    delay(100);
}

/// File listing helper
void printDirectory(File dir, int numTabs)
{
    while (true)
    {

        File entry = dir.openNextFile();
        if (!entry)
        {
            // no more files
            //Serial.println("**nomorefiles**");
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
            printDirectory(entry, numTabs + 1);
        }
        else
        {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

void playSound()
{
    Serial.println("playing sound");
    if (musicPlayer.playingMusic)
    {
        Serial.println("music is currently playing, can't slide whistle again");
        return;
    }

    musicPlayer.startPlayingFile("effects/witches.mp3");
    delay(5);
}
