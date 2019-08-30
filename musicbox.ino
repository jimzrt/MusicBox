/**************************************************
 * 
 *   __  __           _      ____            
 *  |  \/  |         (_)    |  _ \           
 *  | \  / |_   _ ___ _  ___| |_) | _____  __
 *  | |\/| | | | / __| |/ __|  _ < / _ \ \/ /
 *  | |  | | |_| \__ \ | (__| |_) | (_) >  < 
 *  |_|  |_|\__,_|___/_|\___|____/ \___/_/\_\ v.01
 *                                          
 * ================================================                                        
 * Libraries used:
 * - Adafruit_PN532 v1.0.4
 * - DFPlayer_Mini_Mp3_by_Makuna v1.0.3
 * - JC_Button v2.1.1
 ***************************************************/

#include "Arduino.h"
#include "MusicBox.h"

#define MUSICBOX_ID 222

MusicBox musicBox(MUSICBOX_ID);

void setup()
{
  musicBox.initialize();
}

void loop()
{
  musicBox.loop();
}