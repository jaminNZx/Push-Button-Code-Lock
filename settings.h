/**************************************************************
 *
 *                          Settings
 *
 **************************************************************/
 #include <wifi_credentials.h>
/*
     Blynk Auth Code
*/
  #define AUTH                          "aed86c6e773c41b9b674c6ee88fc9644"
/*
   Over The Air Hostname
*/
  #define OTA_HOSTNAME                  "BLYNK-OTA"
/*
   Local Server Settings (uncomment to use local server)
*/
  #define SERVER                        IPAddress(192, 168, 1, 2)
/*
   Console Output (Debug/Log)
*/
  #define OUTPUT_SERIAL
  #define OUTPUT_TERMINAL
/*
   Virtual Pins - Base
*/
  #define vPIN_TERMINAL                 V4
  #define vPIN_UNLOCK                   V1
  #define vPIN_SECURED                  V2
  #define vPIN_CODE                     V3
/*
   Testing
*/
  #define BUTTON                        13
  #define LED                           12

  #define CODE_START                    2000  // ms
  #define BUTTON_CODE                   4     // 4 single presses while in code mode
  #define CODE_TIMEOUT                  2000  // ms
