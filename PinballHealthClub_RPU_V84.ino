/*
    Pinball House Club is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    See <https://www.gnu.org/licenses/>.

  Ver xx - First release of full Beyond Future Spa game code

*/

#include "RPU_Config.h"
#include "RPU.h"
#include "PinballHealthClub.h"
#include "SelfTestAndAudit.h"
//#include "RPU_Config.h"
#include <EEPROM.h>
#include "SendOnlyWavTrigger.h"
SendOnlyWavTrigger wTrig;             // Our WAV Trigger object


#define VERSION_NUMBER  84
#define DEBUG_MESSAGES   1
#define COIN_DOOR_TELEMETRY // If uncommented, coin door settings are sent to monitor on boot
//#define IN_GAME_TELEMETRY   // If uncommented, sends game status to monitor
//#define EXECUTION_MESSAGES  // If uncommented, sends game logic telemetry to monitor

#define USE_SCORE_OVERRIDES
#define USE_SCORE_ACHIEVEMENTS

// MachineState
//  0 - Attract Mode
//  negative - self-test modes
//  positive - game play
int MachineState = 0;
boolean MachineStateChanged = true;
#define MACHINE_STATE_ATTRACT         0
#define MACHINE_STATE_INIT_GAMEPLAY   1
#define MACHINE_STATE_INIT_NEW_BALL   2
#define MACHINE_STATE_UNVALIDATED     3
#define MACHINE_STATE_NORMAL_GAMEPLAY 4
#define MACHINE_STATE_COUNTDOWN_BONUS 90
#define MACHINE_STATE_MATCH_MODE      95
#define MACHINE_STATE_BALL_OVER       100
#define MACHINE_STATE_GAME_OVER       110

//
// Adjustment machine states
//
#define MACHINE_STATE_ADJUST_OVERALL_VOLUME             -17   //    12
#define MACHINE_STATE_ADJUST_SFX_VOLUME                 -18   //    13
#define MACHINE_STATE_ADJUST_JUKE_20_SONGS              -19   //    14
#define MACHINE_STATE_ADJUST_JUKE_1_SONGS               -20   //    15
#define MACHINE_STATE_ADJUST_DIP30                      -21   //    16
#define MACHINE_STATE_ADJUST_FREEPLAY                   -22   //    17
#define MACHINE_STATE_ADJUST_BALL_SAVE                  -23   //    18
#define MACHINE_STATE_ADJUST_TILT_WARNING               -24   //    19
#define MACHINE_STATE_ADJUST_BALLS_OVERRIDE             -25   //    20
#define MACHINE_STATE_ADJUST_DIP24                      -26   //    21
#define MACHINE_STATE_ADJUST_SPINNER_THRESHOLD_TOTAL    -27   //    22
#define MACHINE_STATE_ADJUST_POP_THRESHOLD              -28   //    23
#define MACHINE_STATE_ADJUST_ROAMING_TIME               -29   //    24
#define MACHINE_STATE_ADJUST_SHOOT_ALL_TIME             -30   //    25
#define MACHINE_STATE_ADJUST_FUTURE_TIME                -31   //    26
#define MACHINE_STATE_ADJUST_DONE                       -32   //  Attract


//
//  EEPROM Save Locations
//

#define EEPROM_FREE_PLAY_BYTE                           100
#define EEPROM_BALL_SAVE_BYTE                           101
#define EEPROM_SPINNER_THRESHOLD_TOTAL_BYTE             102
#define EEPROM_SHOOT_ALL_TIME_BYTE                      103
#define EEPROM_POP_THRESHOLD_BYTE                       104
//*** Note location 105 appears to be bad on my development Arduino, do not use ***
#define EEPROM_TILT_WARNING_BYTE                        107
#define EEPROM_BALLS_OVERRIDE_BYTE                      108
#define EEPROM_DIP24_BYTE                               109
#define EEPROM_DIP30_BYTE                               110

#define EEPROM_OVERALL_VOLUME_BYTE                      111
#define EEPROM_SFX_VOLUME_BYTE                          112
#define EEPROM_ROAMING_TIME_BYTE                        113
#define EEPROM_FUTURE_TIME_BYTE                         114
#define EEPROM_FUTURE_JUKE20_BYTE                       115
#define EEPROM_FUTURE_JUKE1_BYTE                        116

// Key parameters

#define TIME_TO_WAIT_FOR_BALL         100
#define TILT_WARNING_DEBOUNCE_TIME    2000

//
// Sound Effects
//

#define SOUND_EFFECT_NONE_0             0
#define SOUND_EFFECT_SPINNER_100_PT     1
#define SOUND_EFFECT_SPINNER_1000_PT    2
#define SOUND_EFFECT_30_PT              3
#define SOUND_EFFECT_DROP_TARGETS       4
#define SOUND_EFFECT_SLINGSHOT          5
#define SOUND_EFFECT_GAME_OVER          6
#define SOUND_EFFECT_18K                7
#define SOUND_EFFECT_OUTLANE            8
#define SOUND_EFFECT_NONE_9             9
#define SOUND_EFFECT_KICKER            10
#define SOUND_EFFECT_GAME_START        11
#define SOUND_EFFECT_SAUCER            12
#define SOUND_EFFECT_MACHINE_START     13
#define SOUND_EFFECT_14                14
#define SOUND_EFFECT_NONE_15           15
#define SOUND_EFFECT_BUMPER            16
#define SOUND_EFFECT_BACKGROUND        17
#define SOUND_EFFECT_NOTE_4            18
#define SOUND_EFFECT_NOTE_3            19
#define SOUND_EFFECT_NOTE_2            20
#define SOUND_EFFECT_NOTE_1            21
#define SOUND_EFFECT_SPA               22
#define SOUND_EFFECT_FUTURITY          23
#define SOUND_EFFECT_TILT              24
#define SOUND_EFFECT_NONE_25           25
#define SOUND_EFFECT_COIN              26
#define SOUND_EFFECT_27                27
#define SOUND_EFFECT_NONE_28           28
#define SOUND_EFFECT_BUMPER_LL         29
#define SOUND_EFFECT_INLANE            30
#define SOUND_EFFECT_NONE_31           31
#define SOUND_EFFECT_SCALE_UP          32
#define SOUND_EFFECT_SCALE_DOWN        33
#define SOUND_EFFECT_SAD               34
#define SOUND_EFFECT_36K               35
#define SOUND_EFFECT_SAUCER_3K         36
#define SOUND_EFFECT_SAUCER_6K         37
#define SOUND_EFFECT_SAUCER_9K         38
#define SOUND_EFFECT_KICKER_3K         39
#define SOUND_EFFECT_KICKER_5K         40
#define SOUND_EFFECT_KICKER_7K         41
#define SOUND_EFFECT_KICKER_9K         42
#define SOUND_EFFECT_BANKSHOT_2K       43
#define SOUND_EFFECT_BANKSHOT_4K       44
#define SOUND_EFFECT_BANKSHOT_6K       45
#define SOUND_EFFECT_BANKSHOT_8K       46
#define SOUND_EFFECT_BANKSHOT_10K      47
#define SOUND_EFFECT_SPA_5K            48
#define SOUND_EFFECT_POP_MODE          49
#define SOUND_EFFECT_EXTRA_BALL        50
#define SOUND_EFFECT_SPINNER_MODE      51
#define SOUND_EFFECT_BALL_LOST         52
#define SOUND_EFFECT_CARMEN_UP         53
#define SOUND_EFFECT_CARMEN_DOWN       54
#define SOUND_EFFECT_CARMEN_END        55

//
// Call out definitions - callouts must have even numbers
// zzzzz

#define CALL_OUT_FUTURE_MODE_SPELL_FUTURE                    500  //  2700
#define CALL_OUT_FUTURE_MODE_FU                              502  //  2219
#define CALL_OUT_FUTURE_MODE_REALLY_CLOSE                    504  //  3146
#define CALL_OUT_FUTURE_MODE_COMPLETED                       506  //  2000
#define CALL_OUT_FUTURE_MODE_ENDED                           508  //  2000
#define CALL_OUT_FUTURE_MODE_NEVER_THOUGHT                   510  //  2737
#define CALL_OUT_FUTURE_MODE_PRETTY_GOOD                     512  //  3744
#define CALL_OUT_FUTURE_MODE_CARMEN_UP_1                     514  //  2175
#define CALL_OUT_FUTURE_MODE_CARMEN_UP_2                     516  //  2500
#define CALL_OUT_                     518  //  2500
#define CALL_OUT_MODE_ONE_FUTURE_SPA                         520  //  1150
#define CALL_OUT_MODE_ONE_KEEP_HITTING_FUTURITY_1            522  //  1790
#define CALL_OUT_MODE_ONE_KEEP_HITTING_FUTURITY_2            524  //  1480
#define CALL_OUT_MODE_ONE_KEEP_HITTING_FUTURITY_3            526  //  2000
#define CALL_OUT_MODE_ONE_GREEN_ARROWS_ARE_LIT               528  //  1680
#define CALL_OUT_MODE_ONE_HIT_SPA                            530  //  1380
#define CALL_OUT_MODE_ONE_HIT_SPA_THEN_BUMPER                532  //  3080
#define CALL_OUT_MODE_ONE_UNLOCK_RIGHT_SPINNER               534  //  2300
#define CALL_OUT_MODE_ONE_UNLOCK_LEFT_SPINNER                536  //  2000
#define CALL_OUT_MODE_ONE_GO_FOR_THE_SAUCER                  538  //  1595
#define CALL_OUT_MODE_ONE_NICE_COMBO                         540  //  2705
#define CALL_OUT_                     542  //  
#define CALL_OUT_SHOOT_ALL_SHOOT_ALL_ANNOUNCEMENT            544  //  6838
#define CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_1                   546  //  1858
#define CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_2                   548  //  2345
#define CALL_OUT_SHOOT_ALL_DONT_HIT_THAT_AGAIN               550  //  1858
#define CALL_OUT_SHOOT_ALL_FUTURE_COMPLETE                   552  //  1306
#define CALL_OUT_SHOOT_ALL_FUTURE_LOST                       554  //  1683
#define CALL_OUT_SHOOT_ALL_KICKER_COMPLETE                   556  //  1480
#define CALL_OUT_SHOOT_ALL_KICKER_LOST                       558  //  1730
#define CALL_OUT_SHOOT_ALL_SAUCER_COMPLETE                   560  //  1596
#define CALL_OUT_SHOOT_ALL_SAUCER_LOST                       562  //  2136
#define CALL_OUT_SHOOT_ALL_ROLLOVER_COMPLETE                 564  //  1367
#define CALL_OUT_SHOOT_ALL_ROLLOVER_LOST                     566  //  1730
#define CALL_OUT_SHOOT_ALL_TIMER_1_ALIVE                     568  //  1303
#define CALL_OUT_SHOOT_ALL_TIMER_2_RUNNING_OUT               570  //  1573
#define CALL_OUT_SHOOT_ALL_TIMER_3_OUT_OF_TIME               572  //  878
#define CALL_OUT_SHOOT_ALL_TIMER_4_HURRY_UP                  574  //  554
#define CALL_OUT_SHOOT_ALL_TIMER_5_TIMES_UP                  576  //  859
//#define CALL_OUT_SHOOT_ALL_SPECIAL_CLOCK_BONUS               578  //  1521
//#define CALL_OUT_SHOOT_ALL_LET_RUN_OUT                       580  //  2154
//#define CALL_OUT_SHOOT_ALL_THAT_STUNK                        582  //  993
//#define CALL_OUT_SHOOT_ALL_PRETTY_GOOD                       584  //  
#define CALL_OUT_ROAMING_GET_ALL_NINE                        586  //  2093
#define CALL_OUT_ROAMING_NEXT_ONE_SCORE_BIG                  588  //  2659
#define CALL_OUT_ROAMING_NOW_FOR_BIG                         590  //  2690




//
// Game/machine global variables
//

// Debug variable
//byte CloneLampStates[RPU_NUM_LAMP_BANKS]; // copy of LampStates that is global

int ArduinoSpeed = 0;                   // Code loop counter

byte GameMode[4] = {1,1,1,1};           // GameMode=1 normal play
unsigned int Goals[4]={0,0,0,0};        // Player goals tracking
boolean GoalsDisplayToggle;             // Display achieved goals if true

unsigned long HighScore = 0;
unsigned long AwardScores[3];           // Score thresholds for awards
int Credits = 0;
int MaximumCredits = 20;
boolean FreePlayMode = false;
boolean MatchFeature = true;            //  Allows Match Feature to run

#define MAX_TILT_WARNINGS_MAX    2
#define MAX_TILT_WARNINGS_DEF    1      // Length of each segment
byte MaxTiltWarnings = MAX_TILT_WARNINGS_DEF;
byte NumTiltWarnings = 0;
unsigned long LastTiltWarningTime = 0;
boolean Tilted = false;

byte CurrentPlayer = 0;
byte CurrentBallInPlay = 1;
byte CurrentNumPlayers = 0;
unsigned long CurrentScores[4];
boolean SamePlayerShootsAgain = false;
byte CurrentAchievements[4];            // Score achievements

unsigned long CurrentTime = 0;
unsigned long PreviousCurrentTime = 0;
unsigned long BallTimeInTrough = 0;
unsigned long BallFirstSwitchHitTime = 0;

boolean BallSaveUsed = false;
#define BALLSAVENUMSECONDS_MAX   20
#define BALLSAVENUMSECONDS_DEF    0
byte BallSaveNumSeconds = BALLSAVENUMSECONDS_DEF;

#define BALLSPERGAME_MAX    5
#define BALLSPERGAME_DEF    3
#define BALLSPERGAME_MIN    3
byte BallsPerGame = BALLSPERGAME_DEF;

boolean HighScoreReplay = true;

int Ten_Pts_Stack = 0;
int Hundred_Pts_Stack = 0;
int Thousand_Pts_Stack = 0;
unsigned long ChimeScoringDelay = 0;

#define DIPSWITCHTHIRTYMIN            0
#define DIPSWITCHTHIRTYMAX            3
byte DipSwitch30 = 0;                    // Sound options for Future Spa
// 0 - Dash51 sounds - All game play sounds, no background
// 1 - Dash51 sounds - All game play sounds, with background
// 2 - WavTrigger sounds - All game play sounds, no background music
// 3 - WavTrigger sounds - All game play sounds, with background music

// Flashing lamp queue
boolean FlashingLampQueueEmpty = false;

#define FLASHING_LAMP_QUEUE_SIZE 6

struct FlashingLampQueueEntry {
  byte lampNumber;
  boolean currentState;
  boolean finishState;
  unsigned long startTime;                // event start
  unsigned long nextTime;                 // next lamp transition
  unsigned long duration;                 // event length
  unsigned long startPeriod;
  unsigned long endPeriod;
};

// Lamp controls

FlashingLampQueueEntry FlashingLampQueue[FLASHING_LAMP_QUEUE_SIZE];

boolean HaltArrowsLighting = false;            // Lock out animations
boolean HaltSpa5KLighting = false;             // Lock out animations
boolean HaltSPSALighting = false;              // Lock out animations
boolean HaltSpotsLighting = false;             // Lock out animations


//
// Attract mode variables
//

// byte AttractHeadMode = 255;
byte AttractPlayfieldMode = 255;
unsigned long AttractSweepTime = 0;
byte AttractSweepLights = 1;
unsigned long ClassicAttractDelay = 367;

unsigned long AttractStepTime = 0;
byte AttractStepLights = 0;

//unsigned long FutureSpaDelay = 100;
unsigned long FutureSpaIncrement = 100;
unsigned long FutureSpaStepTime = 0;
byte FutureSpaStepLights = 0;
boolean FutureSpaSingleLamps = true;
byte FutureSegment = 1;
byte WordSteps = 0;
boolean WordToggle = false;
boolean SweepDir = true;

// Display variables

int CreditsDispValue = 0;       //  Value to be displayed in Credit window
byte CreditsFlashing = false;   //  Allow credits display to flash if true
int BIPDispValue = 0;           //  Value to be displayed in Ball In Play window
byte BIPFlashing = false;       //  Allow BIP display to flash if true

#define OVERRIDE_PRIORITY_LIT_BUMPER_MODE           10
#define OVERRIDE_PRIORITY_GREEN_ARROWS_MODE         15
#define OVERRIDE_PRIORITY_KICKER_SAUCER_COMBO       20
#define OVERRIDE_PRIORITY_FUTURE_MODE_COMBO         30
#define OVERRIDE_PRIORITY_ROAMING_MODE              40
byte OverrideScorePriority = 0; //  Priority of score overrides


//
// Global game logic variables
//

unsigned long SaucerHitTime = 0;
unsigned long SaucerLightsTime = 0;
unsigned long LastKickerHitTime = 0;
unsigned long LastTargetHitTime = 0;
unsigned long LastRollOverHitTime = 0;
unsigned long LastRollSPAHitTime = 0;

boolean GateOpen = true;                            // Unpowered gate is open, gate open when true
unsigned long GateOpenTime = 0;

unsigned long CheckBallTime = 0;                    // Debug timing variable

byte FuturityProgress = 0;                          // Track Futurity target progress
byte BankShotProgress = 0;                          // Track what Bank Shot step we are on
byte KickerShotProgress = 0;                        // Track what Kicker Shot step we are on
byte SaucerProgress   = 0;                          // Track what Saucer step we are on

// Mimics DIP switch 24  - Kicker and drop target SPOTS LETTER lamps toggle every 3rd hit
#define DIPSWITCHTWENTYFOUR_MAX              1
byte DipSwitch24 = 0;                               // mimics Future Spa DIP switch 24
byte DIP24Total = 0;

unsigned long BankShotOffTime = 0;
unsigned long KickerShotOffTime = 0;
unsigned long MarqueeOffTime = 0;
unsigned long MarqueeMultiple = 0;
unsigned long NextSpinnerSoundTime = 0;

// Spinners

byte SpinnerFlips     = 0;                                  // Current spinner total used for sound effects
unsigned long SpinnerScoreTime = 0;                         // Timing of spinner sound effects

unsigned long LastSpinnerLeftTime = 0;
unsigned long LastSpinnerRightTime = 0;
        
unsigned int SpinnerCountTotal[4] = {0,0,0,0};              // Spinner total counter
unsigned int SpinnerCountLeft[4] = {0,0,0,0};               // Spinner counter
unsigned int SpinnerCountRight[4] = {0,0,0,0};              // Spinner counter
byte SpinnerMode[4] = {0,0,0,0};                            // Spinner mode threshold

#define SPINNER_SPEED_LEFT_THRESHOLD                95
#define SPINNER_SPEED_RIGHT_THRESHOLD              115
byte SpinnerFastLeft = false;                               // Set false in InitNewBall
byte SpinnerFastRight = false;                              // Set false in InitNewBall

byte SpinnerStatus = 0;                                     // Spinners status
  // 0 - Spinners both dark - normal low scoring mode 100 pt
  // 1 - Lit spinners - normal 1000 pt mode
  // 2 - Left Spinner dead, right spinner flashing
  // 3 - Right Spinner dead, left spinner flashing

#define SPINNER_THRESHOLD_TOTAL_MAX    130                  // 
#define SPINNER_THRESHOLD_TOTAL_DEF    110                  // 
#define SPINNER_THRESHOLD_TOTAL_MIN     90                  // 
byte Spinner_Threshold_Total = SPINNER_THRESHOLD_TOTAL_DEF; // Spinner hits to achieve mode
#define SPINNER_LEFT_LOCK_FRACTION      .4                  // Fraction of total to lock spinner
#define SPINNER_RIGHT_LOCK_FRACTION     .2                  // Fraction of total to lock spinner
#define SPINNER_LEFT_UNLOCK_FRACTION   .05                  // Fraction of total to unlock spinner
#define SPINNER_RIGHT_UNLOCK_FRACTION   .1                  // Fraction of total to unlock spinner

// *** Not used, but still defined in coin door settings ***
#define SPINNER_THRESHOLD_RIGHT_MAX   20
#define SPINNER_THRESHOLD_RIGHT_DEF    5                    // Default for EEPROM settings - spins
#define SPINNER_THRESHOLD_RIGHT_MIN    3
byte Spinner_Threshold_Right = SPINNER_THRESHOLD_RIGHT_DEF; // Spinner hits to achieve mode
// *** Not used ***
//#define SPINNER_LEFT_UNLOCK_THRESHOLD   5                   // Right spinner spins to unlock left spinner
//#define SPINNER_RIGHT_UNLOCK_THRESHOLD  10                  // Left spinner spins to unlock right spinner
byte SpinnerLeftUnlockCounter = 0;                          // Spins needed to unlock dead left spinner
byte SpinnerRightUnlockCounter = 0;                         // Spins needed to unlock dead right spinner

// Pop bumpers

unsigned int PopCount[4] = {0,0,0,0};                       // Pop bumper counter
//boolean PopCountTest[4] = {true,true,true,true};
byte PopMode[4] = {0,0,0,0};                                // Pop bumper scoring threshold
//unsigned int PopDelta = 0;
#define POP_THRESHOLD_MAX        50
#define POP_THRESHOLD_DEF        40                         // Default for EEPROM settings - hits
#define POP_THRESHOLD_MIN        30
byte Pop_Threshold = POP_THRESHOLD_DEF;                     // Pop bumper hits to achieve mode


boolean GreenArrowsMode[4] = {false,false,false,false};
boolean LitBumperMode[4] = {false,false,false,false};

//  Letter flag definitions
//  1    1
//  5----0987---32-0
//      S31ERUTUFAPS
//      P68
//      EKK
//      C
//
unsigned int Letters[4]={0,0,0,0};                        // Letter status

//
// Mode 2 ShootAll Variables
//
unsigned long ShootAll[4]={0,0,0,0};                      // Letter status
byte ShootAllLamps[26]={0,1,2,3,4,5,6,7,8,24,
                        25,26,27,28,29,30,36,36,36,36,
                        36,52,53,54,55,56};               // ShootAll lamp sequence
byte ShootAllTimerLamps[12]=
{15,12,16,13,17,14,18,9,19,10,20,11};                     // Timer countdown lamps
unsigned long ShootAllTimeRemaining[4] = {0,0,0,0};


#define SHOOTALL_INITIAL_DURATION_MAX          30
#define SHOOTALL_INITIAL_DURATION_DEF          21
#define SHOOTALL_INITIAL_DURATION_MIN          12
byte ShootAllCoinDoorValue = SHOOTALL_INITIAL_DURATION_DEF;

#define SHOOTALL_MAX_DURATION               60000
#define SHOOTALL_TIME_PER_LAMP               5000

#define SHOOT_ALL_LAMP_CAPTURE_EVENT_LENGTH         1000  // How long flashing happens
#define SHOOT_ALL_LAMP_CAPTURE_EVENT_START_PERIOD     33  // 
#define SHOOT_ALL_LAMP_CAPTURE_EVENT_END_PERIOD      150  // 

#define SHOOT_ALL_LAMP_MIN_TIMER_WARNING            5000  // Call out warning

unsigned long ShootAllTimerCalloutWarningRepeatTime = 0;        // Prevent repeated call outs

//unsigned long ShootAllDuration[4] = {0,0,0,0};
unsigned long ShootAllTimeAdded[4] = {0,0,0,0};
//unsigned long ShootAllStartTime[4] = {0,0,0,0};
boolean TimerToggle = false;                              // Starts with lamp off

unsigned long TimerFlashPeriod = 0;                       // Lamp flash period
unsigned long LatestTimerFlash = 0;
boolean ShootAllClockStopped[4] = {true, true, true, true}; // Locks out clock
byte ShootAllStage[4] = {0,0,0,0};                        // Once true next saucer hit starts mode
  // 0 - Mode not qualified
  // 1 - Mode qualified
  // 2 - Mode started - animation 1
  // 3 - Mode started - animation 2
  // 4 - Set lamps and eject ball
  // 5 - Mode active

unsigned long ShootAllIntroStartTime[4] = {0,0,0,0};      // Timer for progressing through intro lighting
boolean DropTargetsToBeReset = false;                     // Trigger for reset at mode 2 end

#define SHOOT_ALL_COMPLETION_HITS_ALLOWED              3  // Number of times completed section can be awarded before reseting

byte ShootAllFutureComplete[4] =   {0,0,0,0};             // Animation flag
byte ShootAllKickerComplete[4] =   {0,0,0,0};             // Animation flag
byte ShootAllSaucerComplete[4] =   {0,0,0,0};             // Animation flag
byte ShootAllRollOverComplete[4] = {0,0,0,0};             // Animation flag

unsigned long ShootAllKickerCompleteStartTime = 0;        // Animation timer
unsigned long ShootAllRollOverCompleteStartTime = 0;      // Animation timer
unsigned long ShootAllSaucerCompleteStartTime = 0;        // Animation timer
unsigned long ShootAllFutureCompleteStartTime = 0;        // Animation timer

byte ShootAllFutureCounter[4] =   {0,0,0,0};              // Completion Counter
byte ShootAllKickerCounter[4] =   {0,0,0,0};              // Completion Counter
byte ShootAllRollOverCounter[4] = {0,0,0,0};              // Completion Counter
byte ShootAllSaucerCounter[4] =   {0,0,0,0};              // Completion Counter

#define SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS         0  // Must have completed greater than this to get a bonus

byte NumBonusesfromPF[4] = {0,0,0,0};                     // Number of times player has earned bonus during ShootAll play

boolean CallCountdownBonusFromPF = false;

// Shoot All Mode 2 - Completion cascade variation per switch hit
byte FUTURELampsCascade[6][6] = {
{0, 1, 2, 3, 4, 5},
{1, 2, 0, 3, 4, 5},
{2, 3, 1, 4, 0, 5},
{3, 2, 4, 1, 5, 0},
{4, 3, 5, 2, 1, 0},
{5, 4, 3, 2, 1, 0}
};

//
// Mode 3 - Roaming Lamps Variables
//

unsigned int RoamingLetters = 0;                          // Letter status
byte RoamingStage = 0;
  // 0 - Mode inactive
  // 1 - Mode initial animation
  // 2 - Mode active
  // 3 - Mode Wrap-up animation
  // 4 - Mode completion animation

byte RoamingScores[9] = {5, 8, 13, 18, 25, 40, 65, 100, 150}; // Scores for each letter obtained 1000's

#define ROAMING_STAGE_DURATION_MAX          25
#define ROAMING_STAGE_DURATION_DEF          15
#define ROAMING_STAGE_DURATION_MIN          11
byte RoamingStageDuration = ROAMING_STAGE_DURATION_DEF;

#define ROAMING_WRAP_UP_DURATION             2500
boolean ReverseRoam = false;
byte NumCapturedLetters = 0;                              // Track total
unsigned long RoamingRotateTime = 0;                      // Letter stepping time
unsigned long RoamingModeTime = 0;                        // Time before mode ends
// Rotation intervals after 1st letter
unsigned long RoamingIntervals[8] = {1000, 720, 518, 373, 269, 193, 139, 100};
//unsigned long RoamingIntervals[8] = {2000, 1460, 1066, 778, 568, 414, 302, 220};
//unsigned long RoamingIntervals[8] = {1000, 730, 533, 389, 284, 207, 151, 110};
//unsigned long RoamingIntervals[8] = {500, 365, 267, 194, 142, 104, 75, 55};
//unsigned long RoamingIntervals[8] = {5000, 5000, 5000, 5000, 5000, 2500, 2500, 1000};





// More global game variables
byte BonusMult = 1;                                       // Bonus multiplier, defaults to 1X

byte MatchDigit = 0;                                      // Relocated to make global

//
// Combo shots
//

// Kicker-Saucer Combo

#define KICKER_SAUCER_DURATION_MAX  21
#define KICKER_SAUCER_DURATION_DEF  15                    // Default for EEPROM settings - sec
#define KICKER_SAUCER_DURATION_MIN   9
byte Kicker_Saucer_Duration = KICKER_SAUCER_DURATION_DEF; // Length in sec each combo shot segment
unsigned long Kicker_Saucer_Start_Time = 0;
byte Kicker_Saucer_Stage = 0;                             // Stage 0 = not active
unsigned long Kicker_Saucer_Reward = 0;                   
#define KICKER_SAUCER_REWARD_START  50000

// FUTURE Mode

#define FUTURE_MODE_DURATION_MAX            21
#define FUTURE_MODE_DURATION_DEF            15
#define FUTURE_MODE_DURATION_MIN            11
byte FutureModeBaseTime = FUTURE_MODE_DURATION_DEF;       // FUTURE mode patterns last this long

#define FUTURE_MODE_ANIMATION_TIME          2000
#define FUTURE_MODE_ANIMATION_FINISH        2000
byte FutureModeActive = 0;                                // Mode active when > 0, set by spinner trigger
unsigned long Future_Mode_Start_Time = 0;
byte FutureModeLetter = 0;                                // Values correspond to FUTURE lane switch numbers 3-8
//unsigned long FutureRewardDisplay[6] = {123456, 23456, 3456, 456, 56, 6};  // Progress display values

byte FUTUREModeSwitchCombinations[6][6] = {
{0, 1, 1, 1, 1, 0},
{1, 0, 1, 0, 1, 0},
{0, 1, 0, 1, 0, 1},
{1, 1, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 1},
{1, 0, 0, 0, 0, 1}
};


// Lit Bumper Mode

#define LIT_BUMPER_TIME_BASE_MAX            20
#define LIT_BUMPER_TIME_BASE_DEF            15
#define LIT_BUMPER_TIME_BASE_MIN            10
byte LitBumperBaseTime = LIT_BUMPER_TIME_BASE_DEF;        // Lit bumper score increment lasts this long

struct BumperData {
  unsigned long score;
  unsigned int flashRate;
  byte countdownTime;
};

struct BumperData LitBumper[] = {
    {0,        0,      0},
    {5000,   300,      0},
    {10000,  238,      3},
    {25000,  175,      5},
    {50000,  113,      7},
    {75000,   50,      9},
};

byte LitBumperModeActive = 0;                               // Mode active when > 0, set by SPA target trigger
  // 6 Scoring steps, normal 1000 pt mode plus 5 additional steps
  // 0 - Inactive, normal 1000 pt mode may be on or off here.  Cannot be turned on until all SPA letters collected
  // 1 - 10000 pt mode
  // 2 - 20000 pt mode
  // etc up to 5
unsigned long LitBumperIncrementTime = 0;                   // Reward increment timer

// Green Arrows Mode

#define GREEN_ARROW_TIME_BASE_MAX            20
#define GREEN_ARROW_TIME_BASE_DEF            15
#define GREEN_ARROW_TIME_BASE_MIN            10
byte GreenArrowsBaseTime = GREEN_ARROW_TIME_BASE_DEF;       // Lit bumper score increment lasts this long

struct GreenArrowData {
  unsigned long score;
  unsigned int flashRate;
  byte countdownTime;
};

// Geometric increase in score with each level.
struct GreenArrowData LitGreenArrowsTable[] = {
    {0,        0,      0},
    {14000,  300,      0},
    {26000,  238,      3},
    {47000,  175,      5},
    {84000,  113,      7},
    {150000,  50,      9},
};

byte GreenArrowsModeActive = 0;                               // Mode active when > 0, set by Futurity target trigger
  // 6 Scoring steps, normal 5000 pt mode when lit plus 5 additional steps
  // 0 - Inactive, normal 5000 pt mode may be on or off here.  Futurity target can turn mode on despite not collecting only FUTURE letters.
  // 1 - 15000 pt mode
  // 2 - 26000 pt mode
  // etc up to 5
unsigned long GreenArrowsIncrementTime = 0;                   // Reward increment timer
unsigned long GreenArrowsCountdown = 0;                       // Mode time remaining


//
// **************************
// *    Audio functions     *
// **************************
// zzzzz

#define SOUND_QUEUE_SIZE                               30
struct SoundQueueEntry {
  unsigned int soundByte;
  unsigned long playTime;
  byte priority;
};
SoundQueueEntry SoundQueue[SOUND_QUEUE_SIZE];


#define JUKEBOXSONGTWENTY_MAX                           9           // 10's digit of number of songs
#define JUKEBOXSONGTWENTY_MIN                           0           //
byte JukeBoxSongTwenty = 0;                                         // Number of songs in 10's digit
#define JUKEBOXSONGONE_MAX                              9           // 1's digit of number of songs
#define JUKEBOXSONGONE_MIN                              0           //
byte JukeBoxSongOne = 0;                                            // Number of songs in 1's digit
#define NUMBER_OF_BACKGROUND_SONGS_PLUS_ONE           101           // Max value, add 1 to number of songs. 100 songs +1 for index
// ReadStoredParameters will update value below
int NumberOfJukeBoxSongsPlusOne = 6;                                // Set at minum number of songs which is 5+1 index
#define BACKGROUND_SONG_TRACK_OFFSET                 1000           // Track number of first song
#define MINIMUM_BACKGROUND_SONG_PLAY_TIME           30000           // msec
#define SONG_CROSS_FADE_TIME                         1500           // msec
#define SPECIFIC_SONG_NUMBER_OFFSET                   150           // Non-random song calls start at this value
#define CALL_OUT_TRACK_NUMBER_OFFSET                  500           // Call out tracks numbers start at this value
#define SONG_VOLUME_DROP_FOR_CALL_OUT                  25           // Background song volume drop in dB
#define SFX_VOLUME_DROP_FOR_CALL_OUT                   20           // Background song volume drop in dB
#define SONG_START_END_VOLUME                          30           // Background song volume start and end volumes in dB

byte CalloutActive = 0;                                             // True when Callout is in progress
unsigned int CurrentCallout = 255;
unsigned int CurrentCalloutPriority = 0;                            // Default is 0, must be higher to actually be processed
unsigned long CurrentCalloutEndTime = 0;
// Durations of each callout in msec (eg. Callout 510 (subtract 500 then /2 to get 5th entry in table)
int CallOutDuration[70] =
{2700, 2250, 3200, 2000, 2000, 2850, 3800, 2175, 2500, 5000,        // 0-9
1150, 1790, 1480, 2000, 1680, 1380, 3080, 2300, 2000, 1595,         // 10-19
2705, 5000, 6838, 1858, 2345, 1858, 1306, 1683, 1480, 1730,         // 20-29
1596, 2136, 1367, 1730, 1303, 1573, 878, 554, 859, 1521,            // 30-39
2154, 993, 5000, 2093, 2659, 2690, 5000, 5000, 5000, 5000,          // 40-49
5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,         // 50-59
5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000          // 60-69
};

int BackgroundSongSelect[NUMBER_OF_BACKGROUND_SONGS_PLUS_ONE];      // Array of selectable songs, final element is songs played
int CurrentBackgroundSong = 255;                                    // Song 255 means no song playing
int NextBackgroundSong = 255;
unsigned long AllowNewBackgroundSongTime = 0;
int CurrentSoundEffect = 255;

// Overall volume range 0-10, 10=+10dB, 9=+5dB, 8=0dB, 0=-40dB
#define OVERALL_VOLUME_MAX                             10
#define OVERALL_VOLUME_DEF                              8
#define OVERALL_VOLUME_MIN                              0
byte OverallVolumeByte = 0;                                          // EEPROM stored value
int OverallVolume = 0;                                               // Declared as full volume, adjustment in dB happens in ReadStoredParameters()
 
// SFx volume range 1-9, 5 dB increments, 5 is balanced each at 0 dB
// 6 set SFx=0dB, Music=-5dB, 4 sets SFx=-5dB, Music=0dB
#define SFX_VOLUME_MAX                                  9
#define SFX_VOLUME_DEF                                  5
#define SFX_VOLUME_MIN                                  1
byte SFxVolumeByte = 0;                                              // EEPROM stored value
int SFxVolume = 0;                                                   // Declared as 0dB, adjustment in dB happens in ReadStoredParameters()
int MusicVolume = 0;                                                 // Declared as 0dB, adjustment in dB happens in ReadStoredParameters()


//
// Function prototypes
//

// Default is Speed=100, CW=true
void MarqueeAttract(byte Segment, int Speed=100, boolean CW=true);
void PlaySoundEffect(byte soundEffectNum, boolean priority=false);
void ClearFlashingLampQueue(boolean emptyAndSetLamp = false);
void CallOut(unsigned int calloutNum, byte priority = 20, unsigned long whenToPlay = 0);
byte NewBackgroundSong(byte specificTrack=255, boolean priority=false);
boolean AddToSoundQueue(unsigned int soundByte, unsigned long playTime, byte priority = 0);

void setup() {
  if (DEBUG_MESSAGES) {
    Serial.begin(57600);
  }

  // Tell the OS about game-specific lights and switches
  RPU_SetupGameSwitches(NUM_SWITCHES_WITH_TRIGGERS, NUM_PRIORITY_SWITCHES_WITH_TRIGGERS, TriggeredSwitches);

  if (DEBUG_MESSAGES) {
    Serial.write("Attempting to initialize the MPU\n");
  }
 
  // Set up the chips and interrupts
  RPU_InitializeMPU();
  RPU_DisableSolenoidStack();
  RPU_SetDisableFlippers(true);

  // Read parameters from EEProm
  ReadStoredParameters();
  RPU_SetCoinLockout((Credits >= MaximumCredits) ? true : false);

// Play Machine start tune - Have to set CurrentTime as we are not yet in the loop structure

  CurrentTime = millis();
  RPU_PushToSolenoidStack(SOL_DROP_RESET, 15, true);
  ClearSoundQueue();
  //RPU_PlaySoundDash51(13);  // Machine start tune won't play until -51 card boots

// Display SW Version
  CurrentNumPlayers = 2;
  CurrentScores[0] = VERSION_NUMBER;
  CurrentScores[1] = (NumberOfJukeBoxSongsPlusOne - 1);   // Display number of songs

// Set up WaveTrigger sound card
  wTrig.start();
  delay(10);
  wTrig.stopAllTracks();
  //wTrig.masterGain(OverallVolume);


  #ifdef EXECUTION_MESSAGES
  Serial.println(F("Done with setup"));
  #endif

}

void ReadStoredParameters() {
  HighScore = RPU_ReadULFromEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, 10000);
  Credits = RPU_ReadByteFromEEProm(RPU_CREDITS_EEPROM_BYTE);
  if (Credits > MaximumCredits) Credits = MaximumCredits;

  ReadSetting(EEPROM_FREE_PLAY_BYTE, 0);
  FreePlayMode = (EEPROM.read(EEPROM_FREE_PLAY_BYTE)) ? true : false;

  BallSaveNumSeconds = ReadSetting(EEPROM_BALL_SAVE_BYTE, BALLSAVENUMSECONDS_DEF);
  if (BallSaveNumSeconds == 99) {                                         //  If set to 99
    BallSaveNumSeconds = BALLSAVENUMSECONDS_DEF;                          //  Set to default
    EEPROM.write(EEPROM_BALL_SAVE_BYTE, BALLSAVENUMSECONDS_DEF);          //  Write to EEPROM
  }
  if (BallSaveNumSeconds > BALLSAVENUMSECONDS_MAX) BallSaveNumSeconds = BALLSAVENUMSECONDS_MAX;


  MaxTiltWarnings = ReadSetting(EEPROM_TILT_WARNING_BYTE, MAX_TILT_WARNINGS_DEF);
  if (MaxTiltWarnings == 99) {                                            //  If set to 99
    MaxTiltWarnings = MAX_TILT_WARNINGS_DEF;                              //  Set to default
    EEPROM.write(EEPROM_TILT_WARNING_BYTE, MAX_TILT_WARNINGS_DEF);        //  Write to EEPROM
  }
  if (MaxTiltWarnings > MAX_TILT_WARNINGS_MAX) MaxTiltWarnings = MAX_TILT_WARNINGS_MAX;

  BallsPerGame = ReadSetting(EEPROM_BALLS_OVERRIDE_BYTE, BALLSPERGAME_DEF);
  if (BallsPerGame == 99) {                                               //  If set to 99
    BallsPerGame = BALLSPERGAME_DEF;                                      //  Set to default
    EEPROM.write(EEPROM_BALLS_OVERRIDE_BYTE, BALLSPERGAME_DEF);           //  Write to EEPROM
  }
  if (BallsPerGame > BALLSPERGAME_MAX) BallsPerGame = BALLSPERGAME_MAX;
  if (BallsPerGame < BALLSPERGAME_MIN) BallsPerGame = BALLSPERGAME_MIN;

  DipSwitch24 = (EEPROM.read(EEPROM_DIP24_BYTE)) ? true : false;
  if (DipSwitch24 > DIPSWITCHTWENTYFOUR_MAX) DipSwitch24 = DIPSWITCHTWENTYFOUR_MAX;
  
  DipSwitch30 = (EEPROM.read(EEPROM_DIP30_BYTE));                          //  Assign value read
  if (DipSwitch30 > DIPSWITCHTHIRTYMAX) DipSwitch30 = DIPSWITCHTHIRTYMAX;
  if (DipSwitch30 < DIPSWITCHTHIRTYMIN) DipSwitch30 = DIPSWITCHTHIRTYMIN;


  Spinner_Threshold_Total = ReadSetting(EEPROM_SPINNER_THRESHOLD_TOTAL_BYTE, SPINNER_THRESHOLD_TOTAL_DEF);
  if (Spinner_Threshold_Total == 99) {                                               //  If set to 99
    Spinner_Threshold_Total = (SPINNER_THRESHOLD_TOTAL_DEF);                         //  Set to default
    EEPROM.write(EEPROM_SPINNER_THRESHOLD_TOTAL_BYTE, SPINNER_THRESHOLD_TOTAL_DEF);  //  Write to EEPROM
  }
  if (Spinner_Threshold_Total > SPINNER_THRESHOLD_TOTAL_MAX) Spinner_Threshold_Total = SPINNER_THRESHOLD_TOTAL_MAX;
  if (Spinner_Threshold_Total < SPINNER_THRESHOLD_TOTAL_MIN) Spinner_Threshold_Total = SPINNER_THRESHOLD_TOTAL_MIN;

  ShootAllCoinDoorValue = ReadSetting(EEPROM_SHOOT_ALL_TIME_BYTE, SHOOTALL_INITIAL_DURATION_DEF);
  //Serial.print(F("RSP - EEPROM_SHOOT_ALL_TIME_BYTE is: "));
  //Serial.println(EEPROM_SHOOT_ALL_TIME_BYTE, DEC);
  if (ShootAllCoinDoorValue == 99) {                                                  //  If set to 99
    ShootAllCoinDoorValue = SHOOTALL_INITIAL_DURATION_DEF;                            //  Set to default
    EEPROM.write(EEPROM_SHOOT_ALL_TIME_BYTE, SHOOTALL_INITIAL_DURATION_DEF);          //  Write to EEPROM
  }
  if (ShootAllCoinDoorValue > SHOOTALL_INITIAL_DURATION_MAX) ShootAllCoinDoorValue = SHOOTALL_INITIAL_DURATION_MAX;
  if (ShootAllCoinDoorValue < SHOOTALL_INITIAL_DURATION_MIN) ShootAllCoinDoorValue = SHOOTALL_INITIAL_DURATION_MIN;

  RoamingStageDuration = ReadSetting(EEPROM_ROAMING_TIME_BYTE, ROAMING_STAGE_DURATION_DEF);
  //Serial.print(F("RSP - EEPROM_ ROAMING_ TIME_ BYTE is: "));
  //Serial.println(EEPROM_ROAMING_TIME_BYTE, DEC);
  //Serial.print(F("RSP found RoamingStageDuration is: "));
  //Serial.println(RoamingStageDuration, DEC);
  if (RoamingStageDuration == 99) {                                                  //  If set to 99
    RoamingStageDuration = ROAMING_STAGE_DURATION_DEF;                               //  Set to default
    EEPROM.write(EEPROM_ROAMING_TIME_BYTE, ROAMING_STAGE_DURATION_DEF);              //  Write to EEPROM
  }
  if (RoamingStageDuration > ROAMING_STAGE_DURATION_MAX) RoamingStageDuration = ROAMING_STAGE_DURATION_MAX;
  if (RoamingStageDuration < ROAMING_STAGE_DURATION_MIN) RoamingStageDuration = ROAMING_STAGE_DURATION_MIN;

  FutureModeBaseTime = ReadSetting(EEPROM_FUTURE_TIME_BYTE, FUTURE_MODE_DURATION_DEF);
  if (FutureModeBaseTime == 99) {                                                    //  If set to 99
    FutureModeBaseTime = FUTURE_MODE_DURATION_DEF;                                   //  Set to default
    EEPROM.write(EEPROM_FUTURE_TIME_BYTE, FUTURE_MODE_DURATION_DEF);                 //  Write to EEPROM
  }
  if (FutureModeBaseTime > FUTURE_MODE_DURATION_MAX) FutureModeBaseTime = FUTURE_MODE_DURATION_MAX;
  if (FutureModeBaseTime < FUTURE_MODE_DURATION_MIN) FutureModeBaseTime = FUTURE_MODE_DURATION_MIN;

  Pop_Threshold = ReadSetting(EEPROM_POP_THRESHOLD_BYTE, POP_THRESHOLD_DEF);
  if (Pop_Threshold == 99) {                                                   //  If set to 99
    Pop_Threshold = POP_THRESHOLD_DEF;                                         //  Set to default
    EEPROM.write(EEPROM_POP_THRESHOLD_BYTE, POP_THRESHOLD_DEF);                //  Write to EEPROM
  }
  if (Pop_Threshold > POP_THRESHOLD_MAX) Pop_Threshold = POP_THRESHOLD_MAX;
  if (Pop_Threshold < POP_THRESHOLD_MIN) Pop_Threshold = POP_THRESHOLD_MIN;

  // Set jukebox song list size
  JukeBoxSongTwenty = ReadSetting(EEPROM_FUTURE_JUKE20_BYTE, JUKEBOXSONGTWENTY_MIN);
  if (JukeBoxSongTwenty > JUKEBOXSONGTWENTY_MAX) JukeBoxSongTwenty = JUKEBOXSONGTWENTY_MAX;

  JukeBoxSongOne = ReadSetting(EEPROM_FUTURE_JUKE1_BYTE, JUKEBOXSONGONE_MIN);
  if (JukeBoxSongOne > JUKEBOXSONGONE_MAX) JukeBoxSongOne = JUKEBOXSONGONE_MAX;

  NumberOfJukeBoxSongsPlusOne = JukeBoxSongTwenty*10 + JukeBoxSongOne + 1;
  // Minimum number of songs is 5, plus index
  if (NumberOfJukeBoxSongsPlusOne < 6) NumberOfJukeBoxSongsPlusOne=6;
  //CurrentScores[2] = (NumberOfJukeBoxSongsPlusOne - 1);                        // Display number of songs

  // Volume Settings
  OverallVolumeByte = ReadSetting(EEPROM_OVERALL_VOLUME_BYTE, OVERALL_VOLUME_DEF);
  if (OverallVolumeByte == 99) {                                               // If set to 99
    OverallVolumeByte = (OVERALL_VOLUME_DEF);                                  // Set to default
    EEPROM.write(EEPROM_OVERALL_VOLUME_BYTE, OVERALL_VOLUME_DEF);              // Write to EEPROM
  }
  if (OverallVolumeByte > OVERALL_VOLUME_MAX) OverallVolumeByte = OVERALL_VOLUME_MAX;
  if (OverallVolumeByte < OVERALL_VOLUME_MIN) OverallVolumeByte = OVERALL_VOLUME_MIN;
  // Convert to dB value for WavTrigger output stage gain setting, 10=+10dB, 9=+5dB, 8=0dB, 0=-40dB
  OverallVolume = (10 - 5*(10 - OverallVolumeByte));                           // Assign value to int, units are now dB

  SFxVolumeByte = ReadSetting(EEPROM_SFX_VOLUME_BYTE, SFX_VOLUME_DEF);
  if (SFxVolumeByte == 99) {                                                   // If set to 99
    SFxVolumeByte = (SFX_VOLUME_DEF);                                          // Set to default
    EEPROM.write(EEPROM_SFX_VOLUME_BYTE, SFX_VOLUME_DEF);                      // Write to EEPROM
  }
  if (SFxVolumeByte > SFX_VOLUME_MAX) SFxVolumeByte = SFX_VOLUME_MAX;
  if (SFxVolumeByte < SFX_VOLUME_MIN) SFxVolumeByte = SFX_VOLUME_MIN;
  // Convert to dB value for WavTrigger track gain settings
  // SFx value of 5 means equal music and sound effects, louder means reduce music, quieter means reduce SFx
  if (SFxVolumeByte > 5) {
    MusicVolume = (-5*(SFxVolumeByte - 5));                                   // eg. 6 = -5dB, 9 = -20dB
    SFxVolume = 0;                                                            // units are now dB
  } else if (SFxVolumeByte < 5) {
    MusicVolume = 0;                                                          // units are now dB
    SFxVolume = (-5*(5 - SFxVolumeByte));                                     // eg. 6 = -5dB, 9 = -20dB
  } else {
    MusicVolume = 0;
    SFxVolume = 0;
  }


  AwardScores[0] = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_1_EEPROM_START_BYTE);
  AwardScores[1] = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_2_EEPROM_START_BYTE);
  AwardScores[2] = RPU_ReadULFromEEProm(RPU_AWARD_SCORE_3_EEPROM_START_BYTE);

}

byte ReadSetting(byte setting, byte defaultValue) {
  byte value = EEPROM.read(setting);
  if (value == 0xFF) {
    EEPROM.write(setting, defaultValue);
    return defaultValue;
  }
  return value;
}


void ShowLampAnimation(byte animationNum[][8], byte frames, unsigned long divisor, unsigned long baseTime, byte subOffset, boolean dim, boolean reverse = false, byte keepLampOn = 99) {
  byte currentStep = (baseTime / divisor) % frames;
  if (reverse) currentStep = (frames - 1) - currentStep;

  byte lampNum = 0;
  for (int byteNum = 0; byteNum < 8; byteNum++) {
    for (byte bitNum = 0; bitNum < 8; bitNum++) {

      // if there's a subOffset, turn off lights at that offset
      if (subOffset) {
        byte lampOff = true;
        lampOff = animationNum[(currentStep + subOffset) % frames][byteNum] & (1 << bitNum);
        if (lampOff && lampNum != keepLampOn) RPU_SetLampState(lampNum, 0);
      }

      byte lampOn = false;
      lampOn = animationNum[currentStep][byteNum] & (1 << bitNum);
      if (lampOn) RPU_SetLampState(lampNum, 1, dim);

      lampNum += 1;
    }
#if not defined (BALLY_STERN_OS_SOFTWARE_DISPLAY_INTERRUPT)
    if (byteNum % 2) RPU_DataRead(0);
#endif
  }
}

void ShowAuxLampAnimation(byte animationNum[][2], byte frames, unsigned long divisor, 
  unsigned long baseTime, byte subOffset, boolean dim, boolean reverse = false, 
  byte keepLampOn = 99) {
  
  byte currentStep = (baseTime / divisor) % frames;
  if (reverse) currentStep = (frames - 1) - currentStep;

  byte lampNum = 0;
  for (int byteNum = 0; byteNum < 2; byteNum++) {            // Aux board is only 2 bytes deep
    for (byte bitNum = 0; bitNum < 8; bitNum++) {

      // if there's a subOffset, turn off lights at that offset
      if (subOffset) {
        byte lampOff = true;
        lampOff = animationNum[(currentStep + subOffset) % frames][byteNum] & (1 << bitNum);
        if (lampOff && lampNum != keepLampOn) RPU_SetLampState((lampNum+60), 0);
      }

      byte lampOn = false;
      lampOn = animationNum[currentStep][byteNum] & (1 << bitNum);
      if (lampOn) RPU_SetLampState((lampNum+60), 1, dim);

      lampNum += 1;
    }
#if not defined (BALLY_STERN_OS_SOFTWARE_DISPLAY_INTERRUPT)
    if (byteNum % 2) RPU_DataRead(0);
#endif
  }
}



//Meteor version - updates audit EEPROM memory locations
void AddCoinToAudit(byte switchHit) {

  unsigned short coinAuditStartByte = 0;

  switch (switchHit) {
    case SW_COIN_3: coinAuditStartByte = RPU_CHUTE_3_COINS_START_BYTE; break;
    case SW_COIN_2: coinAuditStartByte = RPU_CHUTE_2_COINS_START_BYTE; break;
    case SW_COIN_1: coinAuditStartByte = RPU_CHUTE_1_COINS_START_BYTE; break;
  }

  if (coinAuditStartByte) {
    RPU_WriteULToEEProm(coinAuditStartByte, RPU_ReadULFromEEProm(coinAuditStartByte) + 1);
  }

}


// Mata Hari version (and Meteor)
void AddCredit(boolean playSound = false, byte numToAdd = 1) {
  if (Credits < MaximumCredits) {
    Credits += numToAdd;
    if (Credits > MaximumCredits) Credits = MaximumCredits;
    RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, Credits);
    if (playSound) PlaySoundEffect(SOUND_EFFECT_COIN);
    CreditsDispValue = Credits;
    //RPU_SetDisplayCredits(Credits);
    RPU_SetCoinLockout(false);
  } else {
    CreditsDispValue = Credits;
    //RPU_SetDisplayCredits(Credits);
    RPU_SetCoinLockout(true);
  }
}

void AddSpecialCredit() {
  AddCredit(false, 1);
  RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime, true);
  RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, RPU_ReadULFromEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE) + 1);  
}

// Meteor version - uses updated function call in Attract Mode to allow game start following a
// a 4 player game
boolean AddPlayer(boolean resetNumPlayers = false) {

  if (Credits < 1 && !FreePlayMode) return false;
  if (resetNumPlayers) CurrentNumPlayers = 0;
  if (CurrentNumPlayers >= 4) return false;

  CurrentNumPlayers += 1;
  RPU_SetDisplay(CurrentNumPlayers - 1, 0);
  RPU_SetDisplayBlank(CurrentNumPlayers - 1, 0x30);

  if (!FreePlayMode) {
    Credits -= 1;
    RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, Credits);
    CreditsDispValue = Credits;
    //RPU_SetDisplayCredits(Credits);
    RPU_SetCoinLockout(false);
  }
  if (CurrentNumPlayers > 1){
    PlaySoundEffect(SOUND_EFFECT_COIN);
  }

  RPU_WriteULToEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE, RPU_ReadULFromEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE) + 1);

  return true;
}

int InitNewBall(bool curStateChanged, byte playerNum, int ballNum) {  

  if (curStateChanged) {

    //
    // Set pre-ball conditions & variables
    //

    Serial.println(F("----------InitNewBall----------"));
    CheckBallTime = CurrentTime;

    CreditsDispValue = Credits;
    
    RPU_SetDisableGate(true);    // Open gate after each ball.
    RPU_SetDisableFlippers(false);
    RPU_EnableSolenoidStack();
    DropTargetsToBeReset = false;
    RPU_PushToTimedSolenoidStack(SOL_DROP_RESET, 15, CurrentTime + 50);
    RPU_SetDisplayCredits(Credits, true);

    MarqueeOffTime = 0;
    MarqueeMultiple = 0;
    
    BallFirstSwitchHitTime = 0;
    SamePlayerShootsAgain = false;
    RPU_SetLampState(LA_SAME_PLAYER, 0);
    RPU_SetLampState(LA_SPSA_BACK, 0);        

    // Reset Saucer timer
    SaucerHitTime = 0;
    SaucerLightsTime = 0;

    // Reset Kicker timer
    LastKickerHitTime = 0;

    // Reset Rollover timer
    LastRollOverHitTime = 0;

    // Reset Target timer
    LastTargetHitTime = 0;

    // Reset SPA Target timer
    LastRollSPAHitTime = 0;
    
    // Reset gate
    GateOpen = true;        // Unpowered gate is open, gate open when true
    GateOpenTime = 0;
    GateArrowsLighting();

    // Reset Tilt
    NumTiltWarnings = 0;
    LastTiltWarningTime = 0;
    if (Tilted) {
      Tilted = false;
      Serial.println(F("Resetting tilt"));
    }

    BallSaveUsed = false;
    if (BallSaveNumSeconds>0) {
      RPU_SetLampState(LA_SAME_PLAYER, 1, 0, 500);
    }

    if (Credits > 0) {
      RPU_SetLampState(LA_CREDIT_INDICATOR, 1);
    }

    // Reset spinners
    SpinnerFlips  = 0;
    SpinnerStatus = 0;                      // Spinners off
    SpinnerLighting();                      // Set Spinner lamps
    SpinnerFastLeft = false;
    SpinnerFastRight = false;
    SpinnerLeftUnlockCounter = 0;
    SpinnerRightUnlockCounter = 0;
    
    // Reset saucer
    SaucerProgress   = 0;                   // 0 = default, 1 = 3K collected, 2 = 6K collected, 3 = 9K collected
    RPU_SetLampState(LA_SAUCER_3K, 1);
    RPU_SetLampState(LA_SAUCER_6K, 0);
    RPU_SetLampState(LA_SAUCER_9K, 0);

    // Reset Futurity target
    FuturityProgress = 0;                   // 0 = default, 1 = EB collected, 2 = Special collected, 3 = 20k
    RPU_SetLampState(LA_DROP_BANK_20K, 0);
    RPU_SetLampState(LA_DROP_BANK_EXTRA_BALL, 0);
    RPU_SetLampState(LA_DROP_BANK_SPECIAL, 0);

    // Reset Bank Shot
    BankShotProgress = 0;                   // Reset Bank shot for each ball
    BankShotLighting();                     // Set Lights for ball start
    BankShotOffTime = 0;
    
    // Reset Kicker Shot
    KickerShotProgress = 0;                 // Reset Bank shot for each ball
    //KickerShotTotal = 0;                    // Reset Kicker shot total
    KickerShotLighting();                   // Set Lights for ball start
    KickerShotOffTime = 0;

    //
    // Combo shots
    //

    // Kicker Saucer mode
    Kicker_Saucer_Stage = 0;
    Kicker_Saucer_Start_Time = 0;
    Kicker_Saucer_Reward = 0;

    // FUTURE mode
    FutureModeActive = 0;                    // Mode active or not, set by spinner trigger
    Future_Mode_Start_Time = 0;
    FutureModeLetter = 0;                    // Values correspond to FUTURE lane switch numbers 3-8

    // Lit Bumper mode
    LitBumperModeActive = 0;                 // Mode inactive
    LitBumperIncrementTime = 0;              // Zero timer
    RPU_SetLampState(LA_BUMPER_BOT_LEFT, 0);

    // Green Arrow mode
    GreenArrowsModeActive = 0;                // Mode inactive
    GreenArrowsIncrementTime = 0;             // Zero timer
    RPU_SetLampState(LA_ARROW_LANE_R, 0);
    RPU_SetLampState(LA_ARROW_LANE_U1, 0);

    // Turn off SPA 5000 pts lamp
      RPU_SetLampState(LA_SPA_5K, 0);

    // DIP24 lamps setting 
    DIP24Total=0;

    // Turn off lamps, NormalGamePlay will update as needed
    RPU_SetLampState(LA_KICKBACK_SPOTS_LETTER, 0);
    RPU_SetLampState(LA_DROP_BANK_SPOT_LETTER, 0);

    //
    // Reset sounds
    //

    CalloutActive = false;                             // CountdownBonus also sets this
    if (DipSwitch30 == 1) {
      RPU_PlaySoundDash51(SOUND_EFFECT_BACKGROUND);   // Starts background sound
    } else if (DipSwitch30 == 3) {
      //CurrentBackgroundSong = 255;                     // Song 255 means no song playing
      //NewBackgroundSong(255, true);                    // Request song
      AddToSoundQueue(254, CurrentTime+((CurrentBallInPlay==1)?3000:500));  // Delayed random song request
    }

    BonusMultiplier(1);                                // Reset bonus multiplier and lights

    Ten_Pts_Stack = 0;                                 // Reset Scoring variables
    Hundred_Pts_Stack = 0;
    Thousand_Pts_Stack = 0;
    ChimeScoringDelay = 0;

    ClearFlashingLampQueue();                          // Clear FlashingLampQueue only
    BlankFutureSpaLamps();
    AttractSweepLights = 0;                            // Reset Mode Animations

    // ShootAll Mode 2 variables
    if (GameMode[CurrentPlayer]==2) {
      ShootAllWipeTimer();                             // Wipe clock lamps
      // Clear target goals with new ball
      ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b111111111111000001111111111111111));
    }

    ShootAllKickerCompleteStartTime = 0;             // Animation timer
    ShootAllRollOverCompleteStartTime = 0;           // Animation timer
    ShootAllSaucerCompleteStartTime = 0;             // Animation timer
    ShootAllFutureCompleteStartTime = 0;             // Animation timer

    CallCountdownBonusFromPF = false;                // Reset for normal CountdownBonus
    
    // End of Mode 2 variables

    // Roaming Mode 3 variables

    ReverseRoam = false;                              // Initial roaming direction
    NumCapturedLetters = 0;                           // Track total collected lamps
    RoamingRotateTime = 0;                            // Letter stepping time
    RoamingModeTime = 0;                              // Time before mode ends

    // End of Mode 3 variables

    // Keep this near bottom of section since its scope varies by mode and statements 
    // above may need to be over written
    LettersLighting(1);                                // Relight letters already collected
    
    if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
      RPU_PushToTimedSolenoidStack(SOL_OUTHOLE, 4, CurrentTime + 100);
    }

    for (int count=0; count<CurrentNumPlayers; count++) {
      RPU_SetDisplay(count, CurrentScores[count], true, 2);
    }

    BIPDispValue = ballNum;
    //RPU_SetDisplayBallInPlay(ballNum);
    RPU_SetLampState(BALL_IN_PLAY, 1);
    RPU_SetLampState(TILT, 0);

    CreditsFlashing = false;                //  credits display on steady
    BIPFlashing = false;                    //  BIP display on steady

    OverrideScorePriority = 0;              //  Set to default

    HaltArrowsLighting = false;             // Allow animations to run
    HaltSpa5KLighting = false;              // Allow animations to run
    HaltSPSALighting = false;               // Allow animations to run
    HaltSpotsLighting = false;              // Allow animations to run


    //Serial.println(F("-----InitNewBall completed-----"));
  }
  
  // We should only consider the ball initialized when 
  // the ball is no longer triggering the SW_OUTHOLE
  
  if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
    return MACHINE_STATE_INIT_NEW_BALL;
  } else {
    //Serial.println(F("-----return to NormalGamePlay-----"));
    return MACHINE_STATE_NORMAL_GAMEPLAY;
  }
  
}


////////////////////////////////////////////////////////////////////////////
//
//  Self test, audit, adjustments mode
//
////////////////////////////////////////////////////////////////////////////

#define ADJ_TYPE_LIST                 1
#define ADJ_TYPE_MIN_MAX              2
#define ADJ_TYPE_MIN_MAX_DEFAULT      3
#define ADJ_TYPE_SCORE                4
#define ADJ_TYPE_SCORE_WITH_DEFAULT   5
#define ADJ_TYPE_SCORE_NO_DEFAULT     6
byte AdjustmentType = 0;
byte NumAdjustmentValues = 0;
byte AdjustmentValues[10];
unsigned long AdjustmentScore;
byte *CurrentAdjustmentByte = NULL;
unsigned long *CurrentAdjustmentUL = NULL;
byte CurrentAdjustmentStorageByte = 0;
byte TempValue = 0;


int RunSelfTest(int curState, boolean curStateChanged) {
  int returnState = curState;
  CurrentNumPlayers = 0;

#if 1
  if (curStateChanged) {
    // Send a stop-all command and reset the sample-rate offset, in case we have
    wTrig.stopAllTracks();                      // Stop all tracks in case system reset while playing
    RPU_PlaySoundDash51(SOUND_EFFECT_NONE_25); // Cancels DASH51 existing sounds
  }
#else
  if (curStateChanged) {
    // Send a stop-all command and reset the sample-rate offset, in case we have
    //  reset while the WAV Trigger was already playing.
    StopAudio();
    PlaySoundEffect(SOUND_EFFECT_SELF_TEST_MODE_START-curState, 0);
  } else {
    if (SoundSettingTimeout && CurrentTime>SoundSettingTimeout) {
      SoundSettingTimeout = 0;
      StopAudio();
    }
  }
#endif


  // Any state that's greater than CHUTE_3 is handled by the Base Self-test code
  // Any that's less, is machine specific, so we handle it here.
  if (curState >= MACHINE_STATE_TEST_CHUTE_3_COINS) {
    returnState = RunBaseSelfTest(returnState, curStateChanged, CurrentTime, SW_CREDIT_RESET, SW_SLAM);
  } else {
    byte curSwitch = RPU_PullFirstFromSwitchStack();

    if (curSwitch == SW_SELF_TEST_SWITCH && (CurrentTime - GetLastSelfTestChangedTime()) > 250) {
      SetLastSelfTestChangedTime(CurrentTime);
      returnState -= 1;
    }

    if (curSwitch == SW_SLAM) {
      returnState = MACHINE_STATE_ATTRACT;
    }

    if (curStateChanged) {
      for (int count = 0; count < 4; count++) {
        RPU_SetDisplay(count, 0);
        RPU_SetDisplayBlank(count, 0x00);
      }
      Serial.print(F("Current Machine State is: "));
      Serial.println(curState, DEC);
      RPU_SetDisplayCredits(MACHINE_STATE_TEST_SOUNDS - curState);
      RPU_SetDisplayBallInPlay(0, false);
      CurrentAdjustmentByte = NULL;
      CurrentAdjustmentUL = NULL;
      CurrentAdjustmentStorageByte = 0;

      AdjustmentType = ADJ_TYPE_MIN_MAX;
      AdjustmentValues[0] = 0;
      AdjustmentValues[1] = 1;
      TempValue = 0;

      switch (curState) {
        case MACHINE_STATE_ADJUST_FREEPLAY:
          CurrentAdjustmentByte = (byte *)&FreePlayMode;
          CurrentAdjustmentStorageByte = EEPROM_FREE_PLAY_BYTE;
          break;
        case MACHINE_STATE_ADJUST_BALL_SAVE:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 6;
          AdjustmentValues[1] = 5;
          AdjustmentValues[2] = 10;
          AdjustmentValues[3] = 15;
          AdjustmentValues[4] = 20;
          AdjustmentValues[5] = 99;
          CurrentAdjustmentByte = &BallSaveNumSeconds;
          CurrentAdjustmentStorageByte = EEPROM_BALL_SAVE_BYTE;
          break;
        case MACHINE_STATE_ADJUST_TILT_WARNING:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 4;
          AdjustmentValues[0] = 0;
          AdjustmentValues[1] = 1;
          AdjustmentValues[2] = 2;
          AdjustmentValues[3] = 99;
          CurrentAdjustmentByte = &MaxTiltWarnings;
          CurrentAdjustmentStorageByte = EEPROM_TILT_WARNING_BYTE;
          break;
        case MACHINE_STATE_ADJUST_BALLS_OVERRIDE:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 3;
          AdjustmentValues[0] = 3;
          AdjustmentValues[1] = 5;
          AdjustmentValues[2] = 99;
          CurrentAdjustmentByte = &BallsPerGame;
          CurrentAdjustmentStorageByte = EEPROM_BALLS_OVERRIDE_BYTE;
          break;
        case MACHINE_STATE_ADJUST_DIP24:
          CurrentAdjustmentByte = (byte *)&DipSwitch24;
          CurrentAdjustmentStorageByte = EEPROM_DIP24_BYTE;
          break;
        case MACHINE_STATE_ADJUST_DIP30:
          AdjustmentValues[0] = DIPSWITCHTHIRTYMIN;
          AdjustmentValues[1] = DIPSWITCHTHIRTYMAX;
          CurrentAdjustmentByte = (byte *)&DipSwitch30;
          CurrentAdjustmentStorageByte = EEPROM_DIP30_BYTE;
          break;
        case MACHINE_STATE_ADJUST_SPINNER_THRESHOLD_TOTAL:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 6;
          AdjustmentValues[0] = 90;
          AdjustmentValues[1] = 100;
          AdjustmentValues[2] = 110;
          AdjustmentValues[3] = 120;
          AdjustmentValues[4] = 130;
          AdjustmentValues[5] = 99;
          CurrentAdjustmentByte = &Spinner_Threshold_Total;
          CurrentAdjustmentStorageByte = EEPROM_SPINNER_THRESHOLD_TOTAL_BYTE;
          break;
        case MACHINE_STATE_ADJUST_SHOOT_ALL_TIME:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 8;
          AdjustmentValues[0] = 12;
          AdjustmentValues[1] = 15;
          AdjustmentValues[2] = 18;
          AdjustmentValues[3] = 21;
          AdjustmentValues[4] = 24;
          AdjustmentValues[5] = 27;
          AdjustmentValues[6] = 30;
          AdjustmentValues[7] = 99;
          CurrentAdjustmentByte = &ShootAllCoinDoorValue;
          CurrentAdjustmentStorageByte = EEPROM_SHOOT_ALL_TIME_BYTE;
          break;
        case MACHINE_STATE_ADJUST_ROAMING_TIME:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 10;
          AdjustmentValues[0] = 9;
          AdjustmentValues[1] = 11;
          AdjustmentValues[2] = 13;
          AdjustmentValues[3] = 15;
          AdjustmentValues[4] = 17;
          AdjustmentValues[5] = 19;
          AdjustmentValues[6] = 21;
          AdjustmentValues[7] = 23;
          AdjustmentValues[8] = 25;
          AdjustmentValues[9] = 99;
          CurrentAdjustmentByte = &RoamingStageDuration;
          CurrentAdjustmentStorageByte = EEPROM_ROAMING_TIME_BYTE;
          break;
        case MACHINE_STATE_ADJUST_FUTURE_TIME:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 7;
          AdjustmentValues[0] = 11;
          AdjustmentValues[1] = 13;
          AdjustmentValues[2] = 15;
          AdjustmentValues[3] = 17;
          AdjustmentValues[4] = 19;
          AdjustmentValues[5] = 21;
          AdjustmentValues[6] = 99;
          CurrentAdjustmentByte = &FutureModeBaseTime;
          CurrentAdjustmentStorageByte = EEPROM_FUTURE_TIME_BYTE;
          break;
        case MACHINE_STATE_ADJUST_POP_THRESHOLD:
          AdjustmentType = ADJ_TYPE_LIST;
          NumAdjustmentValues = 6;
          AdjustmentValues[0] = 30;
          AdjustmentValues[1] = 35;
          AdjustmentValues[2] = 40;
          AdjustmentValues[3] = 45;
          AdjustmentValues[4] = 50;
          AdjustmentValues[5] = 99;
          CurrentAdjustmentByte = &Pop_Threshold;
          CurrentAdjustmentStorageByte = EEPROM_POP_THRESHOLD_BYTE;
          break;
        case MACHINE_STATE_ADJUST_OVERALL_VOLUME:
          AdjustmentType = ADJ_TYPE_MIN_MAX_DEFAULT;
          AdjustmentValues[0] = 0;
          AdjustmentValues[1] = 10;
          CurrentAdjustmentByte = &OverallVolumeByte;
          CurrentAdjustmentStorageByte = EEPROM_OVERALL_VOLUME_BYTE;
          break;
        case MACHINE_STATE_ADJUST_SFX_VOLUME:
          AdjustmentType = ADJ_TYPE_MIN_MAX_DEFAULT;
          AdjustmentValues[0] = 1;
          AdjustmentValues[1] = 9;
          CurrentAdjustmentByte = &SFxVolumeByte;
          CurrentAdjustmentStorageByte = EEPROM_SFX_VOLUME_BYTE;
          break;
        case MACHINE_STATE_ADJUST_JUKE_20_SONGS:
          AdjustmentType = ADJ_TYPE_MIN_MAX;
          AdjustmentValues[0] = 0;
          AdjustmentValues[1] = 9;
          CurrentAdjustmentByte = &JukeBoxSongTwenty;
          CurrentAdjustmentStorageByte = EEPROM_FUTURE_JUKE20_BYTE;
          break;
        case MACHINE_STATE_ADJUST_JUKE_1_SONGS:
          AdjustmentType = ADJ_TYPE_MIN_MAX;
          AdjustmentValues[0] = 0;
          AdjustmentValues[1] = 9;
          CurrentAdjustmentByte = &JukeBoxSongOne;
          CurrentAdjustmentStorageByte = EEPROM_FUTURE_JUKE1_BYTE;
          break;
        case MACHINE_STATE_ADJUST_DONE:
          returnState = MACHINE_STATE_ATTRACT;
          break;
      }

    }

    // Change value, if the switch is hit
    if (curSwitch == SW_CREDIT_RESET) {

      if (CurrentAdjustmentByte && (AdjustmentType == ADJ_TYPE_MIN_MAX || AdjustmentType == ADJ_TYPE_MIN_MAX_DEFAULT)) {
        byte curVal = *CurrentAdjustmentByte;
        curVal += 1;
        if (curVal > AdjustmentValues[1]) {
          if (AdjustmentType == ADJ_TYPE_MIN_MAX) curVal = AdjustmentValues[0];
          else {
            if (curVal > 99) curVal = AdjustmentValues[0];
            else curVal = 99;
          }
        }
        *CurrentAdjustmentByte = curVal;
        if (CurrentAdjustmentStorageByte) EEPROM.write(CurrentAdjustmentStorageByte, curVal);
      } else if (CurrentAdjustmentByte && AdjustmentType == ADJ_TYPE_LIST) {
        byte valCount = 0;
        byte curVal = *CurrentAdjustmentByte;
        byte newIndex = 0;
        for (valCount = 0; valCount < (NumAdjustmentValues - 1); valCount++) {
          if (curVal == AdjustmentValues[valCount]) newIndex = valCount + 1;
        }
        *CurrentAdjustmentByte = AdjustmentValues[newIndex];
        if (CurrentAdjustmentStorageByte) EEPROM.write(CurrentAdjustmentStorageByte, AdjustmentValues[newIndex]);
      } else if (CurrentAdjustmentUL && (AdjustmentType == ADJ_TYPE_SCORE_WITH_DEFAULT || AdjustmentType == ADJ_TYPE_SCORE_NO_DEFAULT)) {
        unsigned long curVal = *CurrentAdjustmentUL;
        curVal += 5000;
        if (curVal > 100000) curVal = 0;
        if (AdjustmentType == ADJ_TYPE_SCORE_NO_DEFAULT && curVal == 0) curVal = 5000;
        *CurrentAdjustmentUL = curVal;
        if (CurrentAdjustmentStorageByte) RPU_WriteULToEEProm(CurrentAdjustmentStorageByte, curVal);
      }
/*
      if (curState == MACHINE_STATE_ADJUST_DIM_LEVEL) {
        RPU_SetDimDivisor(1, DimLevel);
      }*/
    }

    // Show current value
    if (CurrentAdjustmentByte != NULL) {
      RPU_SetDisplay(0, (unsigned long)(*CurrentAdjustmentByte), true);
    } else if (CurrentAdjustmentUL != NULL) {
      RPU_SetDisplay(0, (*CurrentAdjustmentUL), true);
    }

  }
/*
  if (curState == MACHINE_STATE_ADJUST_DIM_LEVEL) {
    //    for (int count = 0; count < 7; count++) RPU_SetLampState(MIDDLE_ROCKET_7K + count, 1, (CurrentTime / 1000) % 2);
  }*/

  if (returnState == MACHINE_STATE_ATTRACT) {
    // If any variables have been set to non-override (99), return
    // them to dip switch settings
    // Balls Per Game, Player Loses On Ties, Novelty Scoring, Award Score
    //    DecodeDIPSwitchParameters();
    RPU_SetDisplayCredits(Credits, !FreePlayMode);
    ReadStoredParameters();
    // Display SW Version
    CurrentNumPlayers = 2;
    CurrentScores[0] = VERSION_NUMBER;
    CurrentScores[1] = (NumberOfJukeBoxSongsPlusOne - 1);   // Display number of songs
  }

  return returnState;
}


////////////////////////////////////////////////////////////////////////////
//
//  Display Management functions
//
////////////////////////////////////////////////////////////////////////////
unsigned long LastTimeScoreChanged = 0;
unsigned long LastTimeOverrideAnimated = 0;
unsigned long LastFlashOrDash = 0;
#ifdef USE_SCORE_OVERRIDES
unsigned long ScoreOverrideValue[4] = {0, 0, 0, 0};
byte ScoreOverrideStatus = 0;
#define DISPLAY_OVERRIDE_BLANK_SCORE 0xFFFFFFFF
#endif
byte LastScrollPhase = 0;

byte MagnitudeOfScore(unsigned long score) {
  if (score == 0) return 0;

  byte retval = 0;
  while (score > 0) {
    score = score / 10;
    retval += 1;
  }
  return retval;
}

#ifdef USE_SCORE_OVERRIDES
void OverrideScoreDisplay(byte displayNum, unsigned long value, boolean animate) {
  if (displayNum > 3) return;
  ScoreOverrideStatus |= (0x10 << displayNum);
  if (animate) ScoreOverrideStatus |= (0x01 << displayNum);
  else ScoreOverrideStatus &= ~(0x01 << displayNum);
  ScoreOverrideValue[displayNum] = value;
}
#endif

byte GetDisplayMask(byte numDigits) {
  byte displayMask = 0;
  for (byte digitCount = 0; digitCount < numDigits; digitCount++) {
    displayMask |= (0x20 >> digitCount);
  }
  return displayMask;
}


void ShowPlayerScores(byte displayToUpdate, boolean flashCurrent, boolean dashCurrent, unsigned long allScoresShowValue = 0) {

#ifdef USE_SCORE_OVERRIDES
  if (displayToUpdate == 0xFF) ScoreOverrideStatus = 0;
#endif

  byte displayMask = 0x3F;
  unsigned long displayScore = 0;
  unsigned long overrideAnimationSeed = CurrentTime / 150;  // Default is 250 for animated scores to slide back and forth
  byte scrollPhaseChanged = false;

  byte scrollPhase = ((CurrentTime - LastTimeScoreChanged) / 100) % 16;  // Speed of score scrolling
  if (scrollPhase != LastScrollPhase) {
    LastScrollPhase = scrollPhase;
    scrollPhaseChanged = true;
  }

  boolean updateLastTimeAnimated = false;

  for (byte scoreCount = 0; scoreCount < 4; scoreCount++) {   // Loop on scores

#ifdef USE_SCORE_OVERRIDES
    // If this display is currently being overriden, then we should update it
    if (allScoresShowValue == 0 && (ScoreOverrideStatus & (0x10 << scoreCount))) {
      displayScore = ScoreOverrideValue[scoreCount];
      if (displayScore != DISPLAY_OVERRIDE_BLANK_SCORE) {
        byte numDigits = MagnitudeOfScore(displayScore);
        if (numDigits == 0) numDigits = 1;
        if (numDigits < (RPU_OS_NUM_DIGITS - 1) && (ScoreOverrideStatus & (0x01 << scoreCount))) {
          // This score is going to be animated (back and forth)
          if (overrideAnimationSeed != LastTimeOverrideAnimated) {
            updateLastTimeAnimated = true;
            byte shiftDigits = (overrideAnimationSeed) % (((RPU_OS_NUM_DIGITS + 1) - numDigits) + ((RPU_OS_NUM_DIGITS - 1) - numDigits));
            if (shiftDigits >= ((RPU_OS_NUM_DIGITS + 1) - numDigits)) shiftDigits = (RPU_OS_NUM_DIGITS - numDigits) * 2 - shiftDigits;
            byte digitCount;
            displayMask = GetDisplayMask(numDigits);
            for (digitCount = 0; digitCount < shiftDigits; digitCount++) {
              displayScore *= 10;
              displayMask = displayMask >> 1;
            }
            RPU_SetDisplayBlank(scoreCount, 0x00);
            RPU_SetDisplay(scoreCount, displayScore, false);
            RPU_SetDisplayBlank(scoreCount, displayMask);
          }
        } else {
          RPU_SetDisplay(scoreCount, displayScore, true, 1);
        }
      } else {
        RPU_SetDisplayBlank(scoreCount, 0);
      }

    } else {    // Start of non-overridden
#endif
#ifdef USE_SCORE_ACHIEVEMENTS
      boolean showingCurrentAchievement = false;
#endif      
      // No override, update scores designated by displayToUpdate
      if (allScoresShowValue == 0) {
        displayScore = CurrentScores[scoreCount];
#ifdef USE_SCORE_ACHIEVEMENTS
        displayScore += (CurrentAchievements[scoreCount]%10);
        if (CurrentAchievements[scoreCount]) showingCurrentAchievement = true;
#endif 
      }
      else displayScore = allScoresShowValue;

      // If we're updating all displays, or the one currently matching the loop, or if we have to scroll
      if (displayToUpdate == 0xFF || displayToUpdate == scoreCount || displayScore > RPU_OS_MAX_DISPLAY_SCORE || showingCurrentAchievement) {

        // Don't show this score if it's not a current player score (even if it's scrollable)
        if (displayToUpdate == 0xFF && (scoreCount >= CurrentNumPlayers && CurrentNumPlayers != 0) && allScoresShowValue == 0) {
          RPU_SetDisplayBlank(scoreCount, 0x00);
          continue;
        }

        if (displayScore > RPU_OS_MAX_DISPLAY_SCORE) {    //  Need to scroll
          // Score needs to be scrolled 
          //if ((CurrentTime - LastTimeScoreChanged) < 1000) {  // How long to wait to before 1st scroll
          if ((CurrentTime - LastTimeScoreChanged) < ((MachineState == 0)?1000:4000) ) {  // How long to wait to before 1st scroll
            // show score for four seconds after change
            RPU_SetDisplay(scoreCount, displayScore % (RPU_OS_MAX_DISPLAY_SCORE + 1), false);
            byte blank = RPU_OS_ALL_DIGITS_MASK;
#ifdef USE_SCORE_ACHIEVEMENTS
            if (showingCurrentAchievement && (CurrentTime/200)%2) {
              blank &= ~(0x01<<(RPU_OS_NUM_DIGITS-1));
            }
#endif            
            RPU_SetDisplayBlank(scoreCount, blank);            // Sets all digits on except singles for blinking
          } else {                          // Greater than x seconds so scroll
            // Scores are scrolled 10 digits and then we wait for 6
            if (scrollPhase < 11 && scrollPhaseChanged) {   // Scroll phase 0-10 is for actual scrolling
              byte numDigits = MagnitudeOfScore(displayScore);

              // Figure out top part of score
              unsigned long tempScore = displayScore;
              if (scrollPhase < RPU_OS_NUM_DIGITS) {    // Scrolling lowest 6 digits or less
                displayMask = RPU_OS_ALL_DIGITS_MASK;
                for (byte scrollCount = 0; scrollCount < scrollPhase; scrollCount++) {  
                  displayScore = (displayScore % (RPU_OS_MAX_DISPLAY_SCORE + 1)) * 10;  // *10 shift for each scrolled digit up to scrollPhase
                  displayMask = displayMask >> 1;                               // mask moves up to keep digits behind score blank.
                }
              } else {    // scrollPhase > num digits but less than 11, score is gone and display is now blank
                displayScore = 0;
                displayMask = 0x00;
              }

              // Add in lower part of score (front and back can be on screen at the same time)
              if ((numDigits + scrollPhase) > 10) {             // if total is > 10, score scrolled within 10 space window
                byte numDigitsNeeded = (numDigits + scrollPhase) - 10;  // eg. 12345 & 7 = 12-2 = 2
                for (byte scrollCount = 0; scrollCount < (numDigits - numDigitsNeeded); scrollCount++) {
                  tempScore /= 10;                              // Divide down to get the front end of number needed
                }
                displayMask |= GetDisplayMask(MagnitudeOfScore(tempScore));
                displayScore += tempScore;
              }
              RPU_SetDisplayBlank(scoreCount, displayMask);
              RPU_SetDisplay(scoreCount, displayScore);
            }
          }
        } else {      // End of scrolling portion above
          if (flashCurrent && displayToUpdate == scoreCount) {  // If flashing requested and this display is being updated
            unsigned long flashSeed = CurrentTime / 250;
            if (flashSeed != LastFlashOrDash) {
              LastFlashOrDash = flashSeed;
              if (((CurrentTime / 250) % 2) == 0) RPU_SetDisplayBlank(scoreCount, 0x00);
              else RPU_SetDisplay(scoreCount, displayScore, true, 2);
            }
          } else if (dashCurrent && displayToUpdate == scoreCount) {  // If dashing requested
            unsigned long dashSeed = CurrentTime / 50;
            if (dashSeed != LastFlashOrDash) {
              LastFlashOrDash = dashSeed;
              byte dashPhase = (CurrentTime / 60) % 36;
              byte numDigits = MagnitudeOfScore(displayScore);
              if (dashPhase < 12) {
                displayMask = GetDisplayMask((numDigits == 0) ? 2 : numDigits);
                if (dashPhase < 7) {          // Wipe out all the digits up to 6 based on dashPhase
                  // Wipes out digits progressively from left to right
                  for (byte maskCount = 0; maskCount < dashPhase; maskCount++) {
                    displayMask &= ~(0x01 << maskCount);
                  }
                } else {        //  for dashPhase from 7-11
                  // Show digits again from right to left
                  for (byte maskCount = 12; maskCount > dashPhase; maskCount--) {
                    displayMask &= ~(0x20 >> (maskCount - dashPhase - 1));
                  }
                }
                RPU_SetDisplay(scoreCount, displayScore);
                RPU_SetDisplayBlank(scoreCount, displayMask);
              } else {    // if not in dashPhase from 1-12, then up to 36 do nothing
                RPU_SetDisplay(scoreCount, displayScore, true, 2);
              }
            }
          } else {      // End of dashing
#ifdef USE_SCORE_ACHIEVEMENTS
            byte blank;
            blank = RPU_SetDisplay(scoreCount, displayScore, false, 2);
            if (showingCurrentAchievement && (CurrentTime/200)%2) {
              blank &= ~(0x01<<(RPU_OS_NUM_DIGITS-1));
            }
            RPU_SetDisplayBlank(scoreCount, blank);
#else
            RPU_SetDisplay(scoreCount, displayScore, true, 2);
#endif
          }
        }
      } // End if this display should be updated
#ifdef USE_SCORE_OVERRIDES
    } // End on non-overridden
#endif
  } // End loop on scores

  if (updateLastTimeAnimated) {
    LastTimeOverrideAnimated = overrideAnimationSeed;
  }

}

void ShowFlybyValue(byte numToShow, unsigned long timeBase) {
  byte shiftDigits = (CurrentTime - timeBase) / 120;
  byte rightSideBlank = 0;

  unsigned long bigVersionOfNum = (unsigned long)numToShow;
  for (byte count = 0; count < shiftDigits; count++) {
    bigVersionOfNum *= 10;
    rightSideBlank /= 2;
    if (count > 2) rightSideBlank |= 0x20;
  }
  bigVersionOfNum /= 1000;

  byte curMask = RPU_SetDisplay(CurrentPlayer, bigVersionOfNum, false, 0);
  if (bigVersionOfNum == 0) curMask = 0;
  RPU_SetDisplayBlank(CurrentPlayer, ~(~curMask | rightSideBlank));
}



byte AttractLastHeadMode = 255;
unsigned long AttractOffset = 0;
boolean StartTunePlayed = false;

int RunAttractMode(int curState, boolean curStateChanged) {

  int returnState = curState;

  // If this is the first time in the attract mode loop
  if (curStateChanged) {

//
// Reset EEPROM memory locations - uncomment, load and boot machine, comment, reload again.
//

//  RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, 5);
//  RPU_WriteULToEEProm(RPU_TOTAL_PLAYS_EEPROM_START_BYTE, 0);
//  RPU_WriteULToEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, 150000);
//  RPU_WriteULToEEProm(RPU_AWARD_SCORE_1_EEPROM_START_BYTE, 50000);
//  RPU_WriteULToEEProm(RPU_AWARD_SCORE_2_EEPROM_START_BYTE, 100000);
//  RPU_WriteULToEEProm(RPU_AWARD_SCORE_3_EEPROM_START_BYTE, 150000);
//  RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, 0);
//  RPU_WriteULToEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE, 0);
//  RPU_WriteULToEEProm(RPU_CHUTE_1_COINS_START_BYTE, 0);
//  RPU_WriteULToEEProm(RPU_CHUTE_2_COINS_START_BYTE, 0);
//  RPU_WriteULToEEProm(RPU_CHUTE_3_COINS_START_BYTE, 0);

#ifdef COIN_DOOR_TELEMETRY    // Send values to monitor if defined

    Serial.println();
    Serial.print(F("Version Number:            "));
    Serial.println(VERSION_NUMBER, DEC);
    Serial.println(F("-----------------------EEPROM Values-----------------------"));
    Serial.print(F("Credits:                   "));
    Serial.println(RPU_ReadByteFromEEProm(RPU_CREDITS_EEPROM_BYTE), DEC);
    Serial.print(F("AwardScores[0]:            "));
    Serial.print(RPU_ReadULFromEEProm(RPU_AWARD_SCORE_1_EEPROM_START_BYTE), DEC);
    Serial.print(F("     AwardScores[1]:            "));
    Serial.println(RPU_ReadULFromEEProm(RPU_AWARD_SCORE_2_EEPROM_START_BYTE), DEC);
    Serial.print(F("AwardScores[2]:            "));
    Serial.print(RPU_ReadULFromEEProm(RPU_AWARD_SCORE_3_EEPROM_START_BYTE), DEC);
    Serial.print(F("     High Score:                "));
    Serial.println(RPU_ReadULFromEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, 10000), DEC);
    Serial.print(F("BALLS_OVERRIDE:            "));
    Serial.print(ReadSetting(EEPROM_BALLS_OVERRIDE_BYTE, 99), DEC);
    Serial.print(F("          FreePlay  :                "));
    Serial.println(ReadSetting(EEPROM_FREE_PLAY_BYTE, 0), DEC);
    Serial.print(F("BallSaveNumSeconds         "));
    Serial.print(ReadSetting(EEPROM_BALL_SAVE_BYTE, 15), DEC);
    Serial.print(F("          MaxTiltWarnings:           "));
    Serial.println(ReadSetting(EEPROM_TILT_WARNING_BYTE, 2), DEC);
    Serial.print(F("DipSwitch24    :           "));
    Serial.print(ReadSetting(EEPROM_DIP24_BYTE, 1), DEC);
    Serial.print(F("          DipSwitch30    :           "));
    Serial.println(ReadSetting(EEPROM_DIP30_BYTE, 1), DEC);
    Serial.print(F("Spinner_Threshold_Total:   "));
    Serial.println(ReadSetting(EEPROM_SPINNER_THRESHOLD_TOTAL_BYTE, 1), DEC);
    Serial.print(F("ShootAllCoinDoorValue:     "));
    Serial.println(ReadSetting(EEPROM_SHOOT_ALL_TIME_BYTE, 1), DEC);
    Serial.print(F("RoamingStageDuration:      "));
    Serial.println(ReadSetting(EEPROM_ROAMING_TIME_BYTE, 1), DEC);
    Serial.print(F("FutureModeBaseTime:        "));
    Serial.println(ReadSetting(EEPROM_FUTURE_TIME_BYTE, 1), DEC);
    Serial.print(F("PopThreshold:              "));
    Serial.println(ReadSetting(EEPROM_POP_THRESHOLD_BYTE, 1), DEC);
    Serial.print(F("OverallVolumeByte:         "));
    Serial.println(ReadSetting(EEPROM_OVERALL_VOLUME_BYTE, 1), DEC);
    Serial.print(F("SFxVolumeByte:             "));
    Serial.println(ReadSetting(EEPROM_SFX_VOLUME_BYTE, 1), DEC);

    Serial.println();
    Serial.println(F("Variable Values:"));
    Serial.println();

    Serial.print(F("BallsPerGame:              "));
    Serial.print(BallsPerGame, DEC);
    Serial.print(F("          BallSaveNumSeconds:        "));
    Serial.println(BallSaveNumSeconds, DEC);
    Serial.print(F("MaxTiltWarnings:           "));
    Serial.print(MaxTiltWarnings, DEC);
    Serial.print(F("          DipSwitch24    :           "));
    Serial.println(DipSwitch24, DEC);
    Serial.print(F("DipSwitch30    :           "));
    Serial.print(DipSwitch30, DEC);
    Serial.print(F("          SpinnerThresholdTotal:     "));
    Serial.println(Spinner_Threshold_Total, DEC);
    Serial.println(F("ShootAllCoinDoorValue use as is."));
    Serial.print(F("RoamingStageDuration:      "));
    Serial.print(RoamingStageDuration, DEC);
    Serial.print(F("         FutureModeBaseTime:        "));
    Serial.println(FutureModeBaseTime, DEC);    
    Serial.print(F("Pop_Threshold:             "));
    Serial.println(Pop_Threshold, DEC);
    Serial.print(F("OverallVolume:             "));
    Serial.print(OverallVolume, DEC);
    Serial.print(F("          SFxVolume:                 "));
    Serial.println(SFxVolume, DEC);
    Serial.print(F("MusicVolume:               "));
    Serial.println(MusicVolume, DEC);
    Serial.print(F("NumberOfJukeBoxSongsPlusOne:"));
    Serial.println(NumberOfJukeBoxSongsPlusOne, DEC);

    Serial.println();
    
    Serial.print(F("Current Scores:            "));
    Serial.print(CurrentScores[0], DEC);
    Serial.print(F("  "));
    Serial.print(CurrentScores[1], DEC);
    Serial.print(F("  "));
    Serial.print(CurrentScores[2], DEC);
    Serial.print(F("  "));
    Serial.println(CurrentScores[3], DEC);
    Serial.println();

#endif

    Serial.print(F("WavTrigger OverallVolume set to: "));
    Serial.println(OverallVolume, DEC);
    wTrig.masterGain(OverallVolume);


    RPU_DisableSolenoidStack();
    RPU_TurnOffAllLamps();

    RPU_SetDisableFlippers(true);
    if (DEBUG_MESSAGES) {
      Serial.println(F("Entering Attract Mode"));
    }
    for (int count=0; count<4; count++) {
      RPU_SetDisplayBlank(count, 0x00);     
    }

//  First activity

    RPU_PlaySoundDash51(SOUND_EFFECT_NONE_25); // Cancels any existing sounds
    wTrig.stopAllTracks();  // Stop all tracks in case system reset while playing
    
    CreditsDispValue = Credits;
    CreditsFlashing = false;
    BIPDispValue = 0;
    BIPFlashing = false;
    
    //RPU_SetDisplayCredits(Credits);
    //RPU_SetDisplayBallInPlay(0);

//  If previous game has been played turn Match on and leave match value on display
    if (CurrentNumPlayers) {    
      BIPDispValue = ((int)MatchDigit * 10);
      //RPU_SetDisplayBallInPlay((int)MatchDigit * 10);
      RPU_SetLampState(MATCH, 1);
    }
   
    RPU_SetLampState(GAME_OVER, 1);

    //  Reset attract modes
    AttractLastHeadMode = 255;
    AttractPlayfieldMode = 255;

    AttractOffset = CurrentTime;  // Create offset for starting Classic Attract

    // Reset FUTURESPA animation
    
    FutureSpaStepTime = 0;
    FutureSpaStepLights = 0;
    FutureSpaSingleLamps = true;
    FutureSegment = 1;
    WordSteps = 0;
    WordToggle = false;

    // Free trapped ball if any
    
    if (RPU_ReadSingleSwitchState(SW_SAUCER)) {
      RPU_PushToTimedSolenoidStack(SOL_SAUCER, 3, CurrentTime, true);  
    }
    if (RPU_ReadSingleSwitchState(SW_LEFT_KICKBACK)) {
      RPU_PushToTimedSolenoidStack(SOL_KICKER, 3, CurrentTime, true);  
    }
  
  } // End of CurrentStateChanged

    if ((CurrentTime > 7000) && !StartTunePlayed) {
      PlaySoundEffect(SOUND_EFFECT_MACHINE_START);
      //RPU_PlaySoundDash51(SOUND_EFFECT_MACHINE_START);  // Machine start tune
      StartTunePlayed = true;
      Serial.println (F("Start tune in Attract"));
    }


  // FutureSpa backglass animation

  FutureSpaIncrement = 250 - 9*((CurrentTime/2000)%25);  
  // Serial.print(F("FutureSpaIncrement:        "));
  // Serial.println((250 - 9*((CurrentTime/2000)%25)), DEC);
  FUTURESPA(FutureSpaIncrement);


  //DrainDown
  //ShowAuxLampAnimation(AuxLeftRight, 8, 40, CurrentTime, 4, false, false, 99);
  //ShowAuxLampAnimation(AuxUpDown, 6, 30, CurrentTime, 4, false, false, 99);
  //ShowAuxLampAnimation(AuxDiagonal, 6, 35, CurrentTime, 4, false, false, 99);
  
  // Alternate displays between high score and blank
  if ((CurrentTime/6500)%2==0) {          // Displays during attract, X seconds, two states.
  
    if (AttractLastHeadMode!=1) {         // 
      RPU_SetLampState(HIGH_SCORE, 1, 0, 250);
      RPU_SetLampState(GAME_OVER, 0);

      LastTimeScoreChanged = CurrentTime;
      CreditsDispValue = Credits;
    }
    ShowPlayerScores(0xFF, false, false, HighScore);
    AttractLastHeadMode = 1;
    } else {
      if (AttractLastHeadMode!=2) {
        RPU_SetLampState(HIGH_SCORE, 0);
        RPU_SetLampState(GAME_OVER, 1);
        CreditsDispValue = Credits;
        LastTimeScoreChanged = CurrentTime;
      }

      AttractLastHeadMode = 2;
      ShowPlayerScores(0xFF, false, false); // Show player scores
    }

//
//      Playfield Attract Modes
//      Run classic attract first and wait multiple periods with the others alternating more often.
//

    if ( ((CurrentTime-AttractOffset)/13250)%5 == 0) {

    // 
    //  FutureSpa Classic Attract Mode
    //

      if (AttractPlayfieldMode != 5) {  // Classic Attract set up

        AttractStepLights = 0;  // Reset before starting sequence
        RPU_TurnOffAttractPlayfieldLamps();

        if (DEBUG_MESSAGES) {
        Serial.println(F("Classic Attract"));
        }
        // Turn on static and flashing lamps
        RPU_SetLampState(LA_SPINNER_LEFT, 1);
        RPU_SetLampState(LA_SPINNER_RIGHT, 1);
        RPU_SetLampState(LA_DROP_BANK_SPOT_LETTER, 1);
        RPU_SetLampState(LA_KICKBACK_SPOTS_LETTER, 1, 0, 500);
      }

      // Alternate sets of lamps for classic attract animation
      if ( (CurrentTime/ClassicAttractDelay)%2==0 ) {
        // Turn on set 1 lamps and turn off set 2
        for (byte count=0; count<14; count++) {
          RPU_SetLampState(Classic1[count], 1);
        }
        for (byte count=0; count<10; count++) {
          RPU_SetLampState(Classic2[count], 0);
        }
      } else {
        // Turn on 1 set of alternating lights and turn off the others
        for (byte count=0; count<14; count++) {
          RPU_SetLampState(Classic1[count], 0);
        }
        // Turn on set 2 lamps and turn off set 1
        for (byte count=0; count<10; count++) {
          RPU_SetLampState(Classic2[count], 1);
        }
      }
      // Light S-P-A-F-U-T-U-R-E sequence
      if ((CurrentTime-AttractStepTime)>ClassicAttractDelay) { // Sets letter lamp spacing
        AttractStepTime = CurrentTime;
        if (AttractStepLights>8) {  // Reset counter and turn off the lamps
          AttractStepLights = 0;
          for (byte count=0; count<9; count++) {
            RPU_SetLampState((LA_BIG_S+count), 0);
          }
        } else {
        RPU_SetLampState((LA_BIG_S+AttractStepLights), 1, 0, 0);
        AttractStepLights +=1;
        }
      }
      MarqueeAttract(11, 100); // Kicker lights
      MarqueeAttract(12, 100); // Bankshot lights

      AttractPlayfieldMode = 5;
//        Serial.print(F("AttractPlayfieldMode is:             "));
//        Serial.print(AttractPlayfieldMode, DEC);
//        Serial.println();

    } else {

//
// Second leg of main attract modes
//      

    if (( (CurrentTime-AttractOffset+6750)/10000)%2==0) {

      if (AttractPlayfieldMode!=2) {
//    Spiral animation - code from FG2021 attract

        if (DEBUG_MESSAGES) {
          Serial.write("Spiral Attract\n\r");
        }
            
        RPU_TurnOffAttractPlayfieldLamps(); // Start mode by blanking all lamps
        AttractSweepLights = 0;
        SweepDir = !SweepDir;
      }
#if 0
//  Lamp animations by frames

//void ShowLampAnimation(byte animationNum, unsigned long divisor, unsigned long baseTime, byte subOffset, boolean dim, boolean reverse = false, byte keepLampOn = 99)

ShowLampAnimation(SaucerInward, 13, 150, CurrentTime, (SweepDir)?1:12, false, SweepDir, 99);
//ShowLampAnimation(DrainDown, 18, 30, CurrentTime, (SweepDir)?4:14, false, SweepDir, 99);
//ShowLampAnimation(LampAnimation1, 14, 30, CurrentTime, 7, false, SweepDir, 99); // Test middle lamps
//ShowLampAnimation(LampAnimation2, 16, 40, CurrentTime, 8, false, SweepDir, 39); // RADAR sweep around SPA5K
//ShowLampAnimation(LampAnimation3, 16, 30, CurrentTime, (SweepDir)?4:12, false, SweepDir, 42); // RADAR sweep around SPSA


#else
      SweepAnimation(AttractLampsDown, 47, 52, 25, 26);      // S shaped sweep 47 lamps, 26 frames

#endif
      AttractPlayfieldMode = 2;
      
    } else /*if ((CurrentTime/10000)%2==2)*/ {
      if (AttractPlayfieldMode!=3) {
//    Rotating Marquee Lights
        RPU_TurnOffAttractPlayfieldLamps();
        if (DEBUG_MESSAGES) {
          Serial.write("Marquee Attract\n\r");
//        Serial.print(F("CurrentTime is:          "));
//        Serial.println(CurrentTime,DEC);
//        Serial.print(F("((CurrentTime-AttractOffset)/xxx)%8 is:  "));
//        Serial.println( (((CurrentTime-AttractOffset)/14000)%5)   ,DEC);
//        Serial.println();
        }    
      }
//    Marquee Attract function for full playfield.
      MarqueeAttract(10, 100, true);
      AttractPlayfieldMode = 3;
    }



    } // End of loop around playfield animations

// Decide whether we stay in Attract or not.  Watch switches for adding a player or credits.
// Change return state for either INIT_GAMEPLAY or start self test.

  byte switchHit;
  while ( (switchHit=RPU_PullFirstFromSwitchStack())!=SWITCH_STACK_EMPTY ) {
    if (switchHit==SW_CREDIT_RESET) {
//    Added Meteor AddPlayer call
      if (AddPlayer(true)) returnState = MACHINE_STATE_INIT_GAMEPLAY;
    } else if (switchHit==SW_COIN_1 || switchHit==SW_COIN_2 || switchHit==SW_COIN_3) {
      AddCoinToAudit(switchHit);
      AddCredit(true);
      CreditsDispValue = Credits;
      //RPU_SetDisplayCredits(Credits, true);
    } else if (switchHit==SW_SELF_TEST_SWITCH && (CurrentTime-GetLastSelfTestChangedTime())>500) {
      returnState = MACHINE_STATE_TEST_LIGHTS;
      SetLastSelfTestChangedTime(CurrentTime);
    } else {
#ifdef DEBUG_MESSAGES
      char buf[128];
      sprintf(buf, "Switch 0x%02X\n", switchHit);
      Serial.write(buf);
#endif      
    }
  }
  return returnState;
}


unsigned int ArrowsResult;  // Remove later
boolean PlayerUpLightBlinking = false;
boolean MarqeeToggle;

int NormalGamePlay() {
  int returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
  //zzzzz

  // If the playfield hasn't been validated yet, flash score and player up num
  ShowPlayerScores(CurrentPlayer, (BallFirstSwitchHitTime == 0) ? true : false, 
    (BallFirstSwitchHitTime > 0 && ((CurrentTime - LastTimeScoreChanged) > 4000)) ? true : false);

  if ( (BallFirstSwitchHitTime == 0) && GoalsDisplayValue(Goals[CurrentPlayer]) ) {   // If ball not in play and if any goals have been reached
    for (byte count = 0; count < 4; count++) {
      if (count != CurrentPlayer) {
        OverrideScoreDisplay(count, GoalsDisplayValue(Goals[CurrentPlayer]), false);  // Show achieved goals
      }
    }
    GoalsDisplayToggle = true;
  } else if ( (BallFirstSwitchHitTime > 0) && GoalsDisplayToggle) {
    ShowPlayerScores(0xFF, false, false);                                             //  Reset all score displays
    GoalsDisplayToggle = false;
  }

  ShowShootAgainLamp(); // Check if Ball save blinking should finish

// Tilt recovery - in case ball fell into saucer or kicker

  if ( (Tilted) && ((CurrentTime - LastTiltWarningTime) > 5000) ) {
    LastTiltWarningTime = CurrentTime;
    // Free trapped ball if any
    if (RPU_ReadSingleSwitchState(SW_SAUCER)) {
      RPU_PushToTimedSolenoidStack(SOL_SAUCER, 3, CurrentTime, true);  
    }
    if (RPU_ReadSingleSwitchState(SW_LEFT_KICKBACK)) {
      RPU_PushToTimedSolenoidStack(SOL_KICKER, 3, CurrentTime+50, true);  
    }    
  }

//
// Game Mode stuff here
//


// Various telemetry here.

#ifdef IN_GAME_TELEMETRY

  ArduinoSpeed +=1;
  if ((CurrentTime - CheckBallTime) > 10000) {  // Check every X seconds
    CheckBallTime = CurrentTime; 
    
    Serial.println();
    Serial.println(F("                          ----------- Game Telemetry -----------"));  

    Serial.print(F("CurrentTime is:                      "));
    Serial.println(CurrentTime, DEC);

    Serial.print(F("Arduino loop speed is (Hz):          "));
    Serial.println((ArduinoSpeed/10), DEC);
    ArduinoSpeed = 0;
    
    Serial.print(F("GameMode[CrtPlyr] is:                "));
    Serial.println(GameMode[CurrentPlayer], DEC);
    Serial.print(F("Goals CurPlyr is:                    "));
    Serial.println(Goals[CurrentPlayer], BIN);

    Serial.println(F("Current Letters[]:        1------81------8"));
    Serial.print(F("                          "));
    Serial.println(Letters[CurrentPlayer], BIN);
    Serial.println();
#if 0
    Serial.println(F("------Mode 2 ShootAll -----"));
    Serial.println(F("Current ShootAll Letters[]:    0--------90--------90--------9"));
    Serial.print(F("                               "));
    Serial.println(ShootAll[CurrentPlayer], BIN);
    // DNU Serial.print(F("ShootAllDuration:                    "));
    // DNU Serial.println(ShootAllDuration[CurrentPlayer], DEC);
    // DNU Serial.print(F("ShootAllStartTime:                   "));
    // DNU Serial.println(ShootAllStartTime[CurrentPlayer], DEC);
    Serial.print(F("ShootAllTimeRemaining[CrtPlyr]:      "));
    Serial.println(ShootAllTimeRemaining[CurrentPlayer], DEC);
    Serial.print(F("ShootAllStage[CrtPlyr]:              "));
    Serial.println(ShootAllStage[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllIntroStartTime[CrtPlyr]:     "));
    //Serial.println(ShootAllIntroStartTime[CurrentPlayer], DEC);
    //Serial.print(F("DropTargetsToBeReset:                "));
    //Serial.println(DropTargetsToBeReset, DEC);
    Serial.print(F("CallCountdownBonusFromPF:            "));
    Serial.println(CallCountdownBonusFromPF, DEC);
    Serial.print(F("NumBonusesfromPF[CurrentPlayer]:     "));
    Serial.println(NumBonusesfromPF[CurrentPlayer], DEC);
    Serial.println();
#endif
#if 0
    Serial.println(F("------Mode 3 Roaming -----"));
    Serial.print(F("RoamingStage:                        "));
    Serial.println(RoamingStage, DEC);
    Serial.print(F("Roaming Letters[]:                   "));
    Serial.println(RoamingLetters, BIN);
    Serial.print(F("RoamingModeTime:                     "));
    Serial.println(RoamingModeTime, DEC);

    Serial.println();
#endif

    //Serial.println();
    
    //Serial.print(F("ShootAllFutureComplete[CrtPlyr]:       "));
    //Serial.println(ShootAllFutureComplete[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllFutureCounter[CrtPlyr]:        "));
    //Serial.println(ShootAllFutureCounter[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllFutureCompleteStartTime:     "));
    //Serial.println(ShootAllFutureCompleteStartTime, DEC);
    //Serial.println();

    //Serial.print(F("ShootAllKickerComplete[CrtPlyr]:       "));
    //Serial.println(ShootAllKickerComplete[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllKickerCounter[CrtPlyr]:        "));
    //Serial.println(ShootAllKickerCounter[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllKickerCompleteStartTime:     "));
    //Serial.println(ShootAllKickerCompleteStartTime, DEC);
    //Serial.println();
    
    //Serial.print(F("ShootAllSaucerComplete[CrtPlyr]:       "));
    //Serial.println(ShootAllSaucerComplete[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllSaucerCounter[CrtPlyr]:        "));
    //Serial.println(ShootAllSaucerCounter[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllSaucerCompleteStartTime:     "));
    //Serial.println(ShootAllSaucerCompleteStartTime, DEC);
    
    //Serial.print(F("ShootAllRollOverComplete[CrtPlyr]:     "));
    //Serial.println(ShootAllRollOverComplete[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllRollOverCounter[CrtPlyr]:      "));
    //Serial.println(ShootAllRollOverCounter[CurrentPlayer], DEC);
    //Serial.print(F("ShootAllRollOverCompleteStartTime    "));
    //Serial.println(ShootAllRollOverCompleteStartTime, DEC);
    //Serial.println();

    //Serial.print(F("BallFirstSwitchHitTime is:           "));
    //Serial.println(BallFirstSwitchHitTime, DEC);  
    //Serial.println();
    
    //Serial.print(F("HaltArrowsLighting is:               "));
    //Serial.println(HaltArrowsLighting, DEC);  
    //Serial.print(F("HaltSpa5KLighting is:                "));
    //Serial.println(HaltSpa5KLighting, DEC);  
    //Serial.print(F("HaltSPSALighting is:                 "));
    //Serial.println(HaltSPSALighting, DEC);  
    //Serial.print(F("HaltSpotsLighting is:                "));
    //Serial.println(HaltSpotsLighting, DEC);  
    //Serial.println();

//    Serial.print(F("ArrowsLit Crt Plr is:         "));
//    Serial.println(ArrowsLit[CurrentPlayer], DEC);
//    Serial.print(F("ArrowTest is:                 "));
//    Serial.println(ArrowTest, DEC);

    //Serial.print(F("SpinnerStatus is:               "));
    //Serial.println(SpinnerStatus, DEC);  
    //Serial.print(F("SpinnerCountTotal[CrtPlyr]      "));
    //Serial.println(SpinnerCountTotal[CurrentPlayer], DEC);  
    //Serial.print(F("SpinnerCountLeft[CrtPlyr]       "));
    //Serial.println(SpinnerCountLeft[CurrentPlayer], DEC);  
    //Serial.print(F("SpinnerCountRight[CrtPlyr]      "));
    //Serial.println(SpinnerCountRight[CurrentPlayer], DEC);  
    //Serial.print(F("SpinnerLeftUnlockCounter        "));
    //Serial.println(SpinnerLeftUnlockCounter, DEC);  
    //Serial.print(F("SpinnerRightUnlockCounter       "));
    //Serial.println(SpinnerRightUnlockCounter, DEC);  
    
    //Serial.println();  
    
    //Serial.print(F("BankShotProgress is:            "));
    //Serial.println(BankShotProgress, DEC);  
    //Serial.print(F("KickerShotProgress is:          "));
    //Serial.println(KickerShotProgress, DEC);  
    //Serial.print(F("DIP24Total is:                  "));
    //Serial.println(DIP24Total, DEC);  
    //Serial.print(F("DipSwitch24                     "));
    //Serial.println(DipSwitch24, DEC);
    
    //Serial.print(F("FuturityProgress is:          "));
    //Serial.println(FuturityProgress, DEC);  
    //Serial.print(F("Crt GreenArrowsMode is:         "));
    //Serial.println(GreenArrowsMode[CurrentPlayer], DEC);  
    //Serial.print(F("Crt LitBumperMode is:           "));
    //Serial.println(LitBumperMode[CurrentPlayer], DEC);  
    //Serial.println();  

    //Serial.print(F("BonusMult is:                   "));
    //Serial.println(BonusMult, DEC);  

    //Serial.print(F("PopCount is:                    "));
    //Serial.println(PopCount[CurrentPlayer], DEC);  
    //Serial.print(F("PopMode is:                     "));
    //Serial.println(PopMode[CurrentPlayer], DEC);  

    //Serial.print(F("Kicker_Saucer_Stage is:         "));
    //Serial.println(Kicker_Saucer_Stage, DEC);  
    //Serial.print(F("Kicker_Saucer_Start_Time is:    "));
    //Serial.println(Kicker_Saucer_Start_Time, DEC);  
    //Serial.print(F("Kicker_Saucer_Duration is:      "));
    //Serial.println(Kicker_Saucer_Duration, DEC);  

    //Serial.print(F("FutureModeActive is:            "));
    //Serial.println(FutureModeActive, DEC);  
    Serial.print(F("FutureModeLetter is:            "));
    Serial.println(FutureModeLetter, DEC);  
    //Serial.println();  
#if 0
    Serial.print(F("LitBumperMode is:               "));
    Serial.println(LitBumperMode[CurrentPlayer], DEC);  
    Serial.print(F("LitBumperModeActive is:         "));
    Serial.println(LitBumperModeActive, DEC);  
    Serial.print(F("LitBumperIncrementTime is:      "));
    Serial.println(LitBumperIncrementTime, DEC);  
    Serial.println();  
#endif
#if 1
    Serial.println(F("------Green Arrows Mode -----"));
    Serial.print(F("GreenArrowsMode[CrtPlyr] is:    "));
    Serial.println(GreenArrowsMode[CurrentPlayer], DEC);  
    Serial.print(F("GreenArrowsModeActive is:       "));
    Serial.println(GreenArrowsModeActive, DEC);  
    Serial.print(F("GreenArrowsIncrementTime is:    "));
    Serial.println(GreenArrowsIncrementTime, DEC);  
    Serial.println();  
#endif
    Serial.print(F("OverrideScorePriority is:       "));
    Serial.println(OverrideScorePriority, DEC);

    Serial.print(F("FlashingLampQueueEmpty is:      "));
    Serial.println(FlashingLampQueueEmpty, DEC);
    Serial.print(F("CalloutActive is:               "));
    Serial.println(CalloutActive, DEC);
    Serial.print(F("CurrentCallout is:              "));
    Serial.println(CurrentCallout, DEC);
    Serial.print(F("CurrentCalloutEndTime is:       "));
    Serial.println(CurrentCalloutEndTime, DEC);
    Serial.print(F("CurrentBackgroundSong is:       "));
    Serial.println(CurrentBackgroundSong, DEC);

    //Serial.println();

//    Serial.print(F("CurrentNumPlayers:            "));
//    Serial.println(CurrentNumPlayers, DEC);  
//    Serial.println();

//    Serial.print(F("SaucerHitTime is:             "));
//    Serial.println(SaucerHitTime, DEC);  

//    Serial.print(F("GateOpen is:                  "));
//    Serial.println(GateOpen, DEC);  

//    Serial.print(F("SPSA is:                      "));
//    Serial.println(SamePlayerShootsAgain, DEC);  

//    Serial.print(F("RPU_ReadContinuousSolenoids()      "));
//    Serial.print(RPU_ReadContinuousSolenoids(), DEC);
//    Serial.print(F("          "));
//    Serial.println(RPU_ReadContinuousSolenoids(), HEX);

//    Serial.print(F("(Letters[CurrentPlayer] & 0b111111000)  is:  "));
//    Serial.print((Letters[CurrentPlayer] & 0b111111111), BIN);
//    Serial.print(F("  "));
//    Serial.println((Letters[CurrentPlayer] & 0b111111111), DEC);

    Serial.print(F("NumTiltWarnings                 "));
    Serial.println(NumTiltWarnings, DEC);

//    Serial.print(F("BallSaveNumSeconds      "));
//    Serial.println(BallSaveNumSeconds, DEC);

    //Serial.print(F("CurrentTime                   "));
    //Serial.println(CurrentTime, DEC);

    //Serial.print(F("MarqueeOffTime                "));
    //Serial.println(MarqueeOffTime, DEC);
    //Serial.print(F("MarqueeMultiple                "));
    //Serial.println(MarqueeMultiple, DEC);
    //Serial.print(F("MarqueeMultiple * 600          "));
    //Serial.println((MarqueeMultiple*600), DEC);
    //Serial.print(F("(CurrentTime - MarqueeOffTime): "));
    //Serial.println((CurrentTime - MarqueeOffTime), DEC);

    //ShowSoundQueueEntries();
    //ShowFlashingLampQueueEntries();

    Serial.println();

  }

#endif

//
// Common to all modes
//

// Process any flashing lamps

  ProcessFlashingLampQueue(CurrentTime);  

//  Call ChimeScoring

//  if ((Ten_Pts_Stack > 0) || (Hundred_Pts_Stack > 0) || (Thousand_Pts_Stack > 0)) {
  if ((abs(Ten_Pts_Stack) > 0) || (abs(Hundred_Pts_Stack) > 0) || (abs(Thousand_Pts_Stack) > 0)) {
    ChimeScoring();
  }


//
//  Spinner sound delay
//    Limit speed of spinner sound effects to 10 Hz.  Accumulate maximum of 2 spinner flips so
//    that sound effect slows down with spinner.
//

  if (SpinnerFlips > 2) {
    SpinnerFlips = 2;
  }
  if ( SpinnerFlips > 0 && ((CurrentTime - SpinnerScoreTime) > 100) ) {
    SpinnerScoreTime = CurrentTime;
    //Serial.print(F("SpinnerFlips:          "));
    //Serial.println(SpinnerFlips, DEC);  
    SpinnerFlips -= 1;
    switch (SpinnerStatus) {
      case 0:         // Spinners off
        PlaySoundEffect(SOUND_EFFECT_SLINGSHOT);
        break;
      case 1:         // Spinners lit
      case 2:         // Left spinner flashing
      case 3:         // Right spinner flashing
        PlaySoundEffect(SOUND_EFFECT_SPINNER_100_PT);
        break;
    }
  }

//
//  Gate opening
//  Wait x msec until ball has passed the switch

  if ( (GateOpenTime != 0) && ((CurrentTime-GateOpenTime) > 1000) ) {  
    RPU_SetDisableGate(true);
    GateArrowsLighting();
    //RPU_SetLampState(LA_ARROW_LANE_U2, 1);
    //RPU_SetLampState(LA_ARROW_LANE_T, 1);
    //RPU_SetLampState(LA_GATE_ARROW, 0);
    GateOpenTime = 0;
  }

  // Lit Bumper Mode

    if (LitBumperModeActive) {                                                    // If mode active
      if ( (CurrentTime - LitBumperIncrementTime) < ((LitBumperBaseTime - LitBumper[LitBumperModeActive].countdownTime)*1000) ) {
        if (OVERRIDE_PRIORITY_LIT_BUMPER_MODE > OverrideScorePriority) {          // Check if priority should be raised
          OverrideScorePriority = OVERRIDE_PRIORITY_LIT_BUMPER_MODE;
        }
        // Over ride displays
        if (OverrideScorePriority == OVERRIDE_PRIORITY_LIT_BUMPER_MODE) {
          OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), LitBumper[LitBumperModeActive].score, false);
          OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), LitBumper[LitBumperModeActive].score, false);                   
          OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), LitBumper[LitBumperModeActive].score, false);
        }
      } else {                                                                    // Mode decrements or times out
        LitBumperModeActive -= 1;                                                 // Decrement by 1
        if (LitBumperModeActive == 0) {                                           // If now 0, end mode
          LitBumperIncrementTime = 0;
          RPU_SetLampState(LA_BUMPER_BOT_LEFT, LitBumperMode[CurrentPlayer]?1:0);// Set lamp per LitBumperMode boolean
          OverrideScorePriority = 0;                                              // Set back to zero
          ShowPlayerScores(0xFF, false, false);                                   // Reset all score displays
          Serial.println(F("Lit Bumper Mode timed out"));
        } else {
          RPU_SetLampState(LA_BUMPER_BOT_LEFT, 1, 0, LitBumper[LitBumperModeActive].flashRate); // Set lamp to correct flash rate
          LitBumperIncrementTime = CurrentTime;
        }
      }
    }

  // Green Arrows Mode zzzzz

    if (GreenArrowsModeActive) {                                                  // If mode active
      if ( (CurrentTime - GreenArrowsIncrementTime) < ((GreenArrowsBaseTime - LitGreenArrowsTable[GreenArrowsModeActive].countdownTime)*1000) ) {
        if (OVERRIDE_PRIORITY_GREEN_ARROWS_MODE > OverrideScorePriority) {        // Check if priority should be raised
          OverrideScorePriority = OVERRIDE_PRIORITY_GREEN_ARROWS_MODE;
        }
        // Over ride displays
        if (OverrideScorePriority == OVERRIDE_PRIORITY_GREEN_ARROWS_MODE) {
          GreenArrowsCountdown -= (CurrentTime - PreviousCurrentTime);            // Shrink time each pass through code
          // Error trap roll under using 10 minutes as limit
          if (GreenArrowsCountdown > 600000) GreenArrowsCountdown = 0;
          OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), (GreenArrowsCountdown/1000), true);
//          OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), 
//            ((GreenArrowsBaseTime - LitGreenArrowsTable[GreenArrowsModeActive].countdownTime) - (CurrentTime - GreenArrowsIncrementTime)/1000), true);
          //OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), LitGreenArrowsTable[GreenArrowsModeActive].score, false);
          OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), LitGreenArrowsTable[GreenArrowsModeActive].score, false);                   
          OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), LitGreenArrowsTable[GreenArrowsModeActive].score, false);
        }
      } else {                                                                    // Mode decrements or times out
        GreenArrowsModeActive -= 1;                                               // Decrement by 1
        if (GreenArrowsModeActive == 0) {                                         // If now 0, end supplemental mode, base GreenArrows may still be true however
          GreenArrowsIncrementTime = 0;
          // In Modes 2 or 3 there is no base GreenArrowsMode, only the Futurity trigger sub mode
          if ( (GameMode[CurrentPlayer] == 2) || (GameMode[CurrentPlayer] == 3) ) GreenArrowsMode[CurrentPlayer] = 0;
          GreenArrowsLighting();
          OverrideScorePriority = 0;                                              // Set back to zero
          ShowPlayerScores(0xFF, false, false);                                   // Reset all score displays
          Serial.println(F("Green Arrows supplemental mode timed out"));
        } else {
          GreenArrowsLighting();
          GreenArrowsIncrementTime = CurrentTime;
        }
      }
    }


//
// Mode 1 - Regular game play
//

  if (GameMode[CurrentPlayer] == 1) {

  //
  // FUTURESPA animation - speed is set based on collected letters
  //
    unsigned int countOnes = Letters[CurrentPlayer];
    byte numOnes = 0;
    for (int count = 0; count < 9; count++) {
      if ( (countOnes & 0b1) == 1 ) {
        numOnes++;
      }
      countOnes >>= 1;
    }
    FutureSpaIncrement = 200 - numOnes*18;
    //Serial.print(F("numOnes:                "));
    //Serial.println(numOnes, DEC);  

    FUTURESPA(FutureSpaIncrement);


  //
  //  18K, 36K, SPECIAL
  //
  
    if ( Letters[CurrentPlayer] == 511 ) {          // Nine letters collected first time
      Letters[CurrentPlayer] = 512;                 // Light 18K, reset Letters
      LettersLighting(1);
      PlaySoundEffect(SOUND_EFFECT_18K, true);
      CallOut(CALL_OUT_MODE_ONE_FUTURE_SPA, 40, CurrentTime+2000);
    } else if ( Letters[CurrentPlayer] == 1023 ) {  // 2nd time collected
      Letters[CurrentPlayer] = 3072;                // Light 36K, SPECIAL, reset Letters
      LettersLighting(1);
      PlaySoundEffect(SOUND_EFFECT_36K, true);
      CallOut(CALL_OUT_MODE_ONE_FUTURE_SPA, 40, CurrentTime+2000);
      SetGoals(1);                                  // Set goal
    } else if ( Letters[CurrentPlayer] == 3583 ) {  // 3rd time collected
      Letters[CurrentPlayer] = 3072;                // Light 36K, SPECIAL, reset Letters
      LettersLighting(1);
      SamePlayerShootsAgain = true;                 // SPSA
      //RPU_SetLampState(LA_SAME_PLAYER, 1);
      //RPU_SetLampState(LA_SPSA_BACK, 1);
      PlaySoundEffect(SOUND_EFFECT_36K, true);
      CallOut(CALL_OUT_MODE_ONE_FUTURE_SPA, 40, CurrentTime+2000);
      RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime+1200, true);
    } 
  
  //
  //  LitBumperMode - Mode 1
  //  Achieved when all 3 SPA letters are collected
  //
    if ( (Letters[CurrentPlayer] & 0b111) == 7 ) {
      LitBumperMode[CurrentPlayer] = true;
      if (LitBumperModeActive == 0) {                 // Do only when mode inactive
        RPU_SetLampState(LA_BUMPER_BOT_LEFT, 1);
      }
    } else if ((Letters[CurrentPlayer] & 0b111) < 7){
      LitBumperMode[CurrentPlayer] = false;
      RPU_SetLampState(LA_BUMPER_BOT_LEFT, 0);
      if (LitBumperModeActive != 0) {                   // In case mode was active
        LitBumperModeActive = 0;                        // Shut off mode in case active
        OverrideScorePriority = 0;                      // Set back to zero
        ShowPlayerScores(0xFF, false, false);           // Reset all score displays
      }
    }

  //
  // SPA 5000 pts lamp setting - Mode 1
  //
    if ( ((Letters[CurrentPlayer] & (0b111)) == 7) && !HaltSpa5KLighting ) {  // If all 3 SPA letters are collected
      RPU_SetLampState(LA_SPA_5K, 1);
    } else {
      RPU_SetLampState(LA_SPA_5K, 0);
    }


  // 
  // SPOTS LETTER lamps
  //
    if (!HaltSpotsLighting) {
      if (GameMode[CurrentPlayer] != 2) {
        if (!DipSwitch24) {
          RPU_SetLampState(LA_DROP_BANK_SPOT_LETTER, ((DIP24Total+2)%3 == 0)?1:0);
          RPU_SetLampState(LA_KICKBACK_SPOTS_LETTER, ((DIP24Total+1)%3 == 0)?1:0);
        } else {
          RPU_SetLampState(LA_DROP_BANK_SPOT_LETTER, 1);
          RPU_SetLampState(LA_KICKBACK_SPOTS_LETTER, 1);
        }
      }
    }

  //
  //  Green Arrows Mode
  //  Mode achieved only when FUTURE letters collected exclusively.  Mode stays on while 
  //  collecting SPA letters after mode starts.  504 triggers mode.  Rolling over turns off mode.
  //
    if ( (Letters[CurrentPlayer] & 0b111111111) == 504 ) {
      GreenArrowsMode[CurrentPlayer] = true;
      GreenArrowsLighting();
    // If mode is activated by Futurity target, we may not meet 504 and don't want it turned off here.
    // Only turn off mode when GreenArrowsModeActive has timed out and was set to 0.
    } else if ( ((Letters[CurrentPlayer] & 0b111111111) < 504) && (GreenArrowsModeActive == 0) ) {
      GreenArrowsMode[CurrentPlayer] = false;
      GreenArrowsLighting();
    }

  // Saucer lamps - also in mode 3
    if ( (SaucerLightsTime != 0) && ((CurrentTime-SaucerLightsTime) > (1000+SaucerProgress*750)) ) {
      SaucerLighting();
      RPU_PushToTimedSolenoidStack(SOL_SAUCER, 3, CurrentTime);
      SaucerLightsTime = 0;
    }
  
  // Bank Shot Animation - Also in Mode 3
    if ( (CurrentTime - BankShotOffTime) < (750+(BankShotProgress*150)) ) {
      //Serial.println(F("Bank Shot Animation running"));
      // Call Marquee Attract for BankShot portion
      MarqueeAttract(2 , 80, true);
    } else if (BankShotOffTime != 0) {
      Serial.println(F("Bank Shot Animation finishing, calling BankShotLighting"));
      Serial.println();
      BankShotLighting();
      BankShotOffTime = 0;
    }
  
  // Kicker Shot Animation - Also in Mode 3
    if ( (CurrentTime - KickerShotOffTime) < (750+(KickerShotProgress*150)) ) {
      //Serial.println(F("Kicker Shot Animation running"));
      // Call Marquee Attract for KickerShot portion
      MarqueeAttract(3 , 80, true);
    } else if (KickerShotOffTime != 0) {
      Serial.println(F("Kicker Shot Animation finishing, calling KickerShotLighting"));
      Serial.println();
      KickerShotLighting();
      KickerShotOffTime = 0;
    }
  
  // Marquee Animation - Ver 3
    if ( (CurrentTime - MarqueeOffTime) < (600 * MarqueeMultiple) ) {
      MarqueeAttract(1 , 75, MarqeeToggle);        // Call larger letter portion only
    } else if (MarqueeOffTime != 0) {
      //Serial.println(F("Marquee Animation calling LettersLighting()."));
      //Serial.println();
      LettersLighting(1);
      MarqueeOffTime = 0;
      MarqeeToggle = !MarqeeToggle;                // Reverses animations
    }
  
  //
  // Combo Shots - Mode 1
  //
  
  // Kicker-Saucer Combo
  
    if ( (Kicker_Saucer_Start_Time != 0) && ((CurrentTime - Kicker_Saucer_Start_Time) < (Kicker_Saucer_Duration*1000)) && (Kicker_Saucer_Stage != 99)) {
      // Over ride displays
      Kicker_Saucer_Reward = KICKER_SAUCER_REWARD_START - ((CurrentTime - Kicker_Saucer_Start_Time)*KICKER_SAUCER_REWARD_START/(Kicker_Saucer_Duration*1000));
      // Over ride displays
      if (OVERRIDE_PRIORITY_KICKER_SAUCER_COMBO > OverrideScorePriority) {   // Check if priority should be raised
        OverrideScorePriority = OVERRIDE_PRIORITY_KICKER_SAUCER_COMBO;
      }
      if (OverrideScorePriority == OVERRIDE_PRIORITY_KICKER_SAUCER_COMBO) {
        OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), Kicker_Saucer_Reward, false);
        unsigned int Countdown = (Kicker_Saucer_Duration - ((CurrentTime - Kicker_Saucer_Start_Time)/1000));
        OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), Countdown, false);                   
        OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), Countdown, false);
      }
      unsigned int SaucerSpeed = ( (130 - (100*(CurrentTime - Kicker_Saucer_Start_Time)/(Kicker_Saucer_Duration*1000))));
      SaucerSpeed = constrain (SaucerSpeed, 30, 100);                                // Saucer lamp animation speed
      MarqueeAttract(7, SaucerSpeed);                                                // Saucer lamp animation
    } else if ( (Kicker_Saucer_Start_Time != 0) && (Kicker_Saucer_Stage != 99) )  {  // Timed out, reset everything
      Kicker_Saucer_Stage = 0;
      Kicker_Saucer_Start_Time = 0;
      SaucerLighting();
      OverrideScorePriority = 0;              //  Set back to zero
      ShowPlayerScores(0xFF, false, false);   //  Reset all score displays
      Serial.println(F("Kicker_Saucer timed out"));
    } else if ( ((CurrentTime - Kicker_Saucer_Start_Time) > 3000) && (Kicker_Saucer_Stage == 99) ) { //  Was won, reset displays after delay
      Kicker_Saucer_Stage = 0;                                                                       //  Turn off mode
      Kicker_Saucer_Start_Time = 0;
      OverrideScorePriority = 0;              //  Set back to zero
      ShowPlayerScores(0xFF, false, false);   //  Reset all score displays
      Serial.println(F("Kicker_Saucer display timeout after win"));
    }

  // FUTURE Mode Combo

    if (FutureModeActive) {                                                     // If mode active
      unsigned long FUTURE_Reward = 10000*(unsigned long)(FutureModeLetter-2);
      if (OVERRIDE_PRIORITY_FUTURE_MODE_COMBO > OverrideScorePriority) {        // Check if priority should be raised
        OverrideScorePriority = OVERRIDE_PRIORITY_FUTURE_MODE_COMBO;
      }
      switch (FutureModeActive) {
        case 1:                                                                 // Intro animation
          if ((CurrentTime - Future_Mode_Start_Time) < FUTURE_MODE_ANIMATION_TIME ) {
            HaltArrowsLighting = true;                                          // Keep arrows from flickering
            MarqueeAttract(6, 75);                                              // Animate top playfield region
            unsigned int Countdown = ((FutureModeBaseTime) - ((CurrentTime - Future_Mode_Start_Time)/1000));
            // Over ride displays
            if (OverrideScorePriority == OVERRIDE_PRIORITY_FUTURE_MODE_COMBO) {
              OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), FUTURE_Reward, false);
              OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), Countdown, false);                   
              OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), Countdown, false);
            }
          } else {
            LettersLighting(1);                                                 // Reset lamps
            HaltArrowsLighting = false;                                         // Allow lamp updates
            GreenArrowsLighting();
            GateArrowsLighting();
            // Initiate flashing pattern according to which step in the mode we are on
            // FutureModeLetter steps from 3-8 for a total of 6 steps
            // Lamp flashing sequence is shortened by FUTURE_MODE_ANIMATION_TIME amount 
            for (int count = 0; count < 6; count++) {
              if (FUTUREModeSwitchCombinations[FutureModeLetter - 3][count]) {
              AddToFlashingLampQueue((count+3), 0, CurrentTime, ((FutureModeBaseTime*1000) - FUTURE_MODE_ANIMATION_TIME), 300, 75);  // Start lamp sequence                 
              }
            }
            FutureModeActive +=1;                                               // Move to next stage
          }
          break;
        case 2:                                                                 // Main combo section
          if ( (CurrentTime - Future_Mode_Start_Time) < (FutureModeBaseTime*1000) ) {
            unsigned int Countdown = ((FutureModeBaseTime) - ((CurrentTime - Future_Mode_Start_Time)/1000));
            //Serial.print(F("FutureModeBaseTime is:"));
            //Serial.println(FutureModeBaseTime, DEC);
            // Over ride displays
            if (OverrideScorePriority == OVERRIDE_PRIORITY_FUTURE_MODE_COMBO) {
              OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), FUTURE_Reward, false);
              OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), Countdown, false);                   
              OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), Countdown, false);
            }
          } else {                                                              // Timed out, close out mode
            if (DipSwitch30 > 1) {                                              // if using WavTrigger
              switch (FutureModeLetter) {
                case 3:                                                         // F
                case 4:                                                         // U1
                case 5:                                                         // T
                  CallOut(CALL_OUT_FUTURE_MODE_ENDED, 60);
                  break;
                case 6:                                                         // U2
                  CallOut(CALL_OUT_FUTURE_MODE_ENDED, 60);
                  CallOut(CALL_OUT_FUTURE_MODE_PRETTY_GOOD, 65, (CurrentTime+2100));
                  break;
                case 7:                                                         // R
                case 8:                                                         // R
                  CallOut(CALL_OUT_FUTURE_MODE_ENDED, 60);
                  CallOut(CALL_OUT_FUTURE_MODE_NEVER_THOUGHT, 67, (CurrentTime+2100));
                  break;
              }
            } else {
              PlaySoundEffect(SOUND_EFFECT_BALL_LOST);
            }
            AddToSoundQueue(254, CurrentTime+1500);                             // Restore music waiting a bit
            FutureModeActive = 0;                                               // Turn off mode
            Future_Mode_Start_Time = 0;                                         // Reset clock
            for (int count = 3; count < 9; count++) {
              ClearFlashingLampQueueEntry(count);                               // Remove any flasing FUTURE lane lamps
            }
            FutureModeLetter = 0;                                               // Disable further rewards
            OverrideScorePriority = 0;                                          // Set back to zero
            ShowPlayerScores(0xFF, false, false);                               // Reset all score displays
            Serial.println(F("FUTURE mode timed out"));
            LettersLighting(1);                                                 // Reset lamps
          }
          break;
        case 3:                                                                 // Player achieved all levels
          if ( (CurrentTime - Future_Mode_Start_Time) < (FUTURE_MODE_ANIMATION_FINISH) ) {
            HaltArrowsLighting = true;                                          // Keep arrows from flickering
            MarqueeAttract(6);                                                  // Animate top playfield region
            if (OverrideScorePriority == OVERRIDE_PRIORITY_FUTURE_MODE_COMBO) {
              OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), 60000, false);
              OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), 60000, false);                   
              OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), 60000, false);
            }
          } else {                                                              // Timed out, close out mode
            FutureModeActive = 0;                                               // Turn off mode
            Future_Mode_Start_Time = 0;                                         // Reset clock
            FutureModeLetter = 0;                                               // Disable further rewards
            OverrideScorePriority = 0;                                          // Set back to zero
            ShowPlayerScores(0xFF, false, false);                               // Reset all score displays
            Serial.println(F("FUTURE mode - all 6 steps captured"));
            for (int count = 3; count < 9; count++) {
              ClearFlashingLampQueueEntry(count);                               // Remove any flasing FUTURE lane lamps
            }            
            LettersLighting(1);                                                 // Reset lamps
            HaltArrowsLighting = false;                                         // Allow lamp updates
            GreenArrowsLighting();
            GateArrowsLighting();
            AddToSoundQueue(254, CurrentTime+5000);                             // Restore music after wrap up sound finishes
          }
          break;
      } // end of FUTURE mode combo switch
    }



    // Pre Mode 2 saucer animation
  
    if (ShootAllStage[CurrentPlayer] == 1) {
      ShowLampAnimation(SaucerPreShootAll2, 2, 125, CurrentTime, 1, false, false, 99);
      //ShowLampAnimation(SaucerPreShootAll3, 6, 500, CurrentTime, 3, false, false, 99);
    }

  }   // End of Mode 1 section

  //
  // Mode 2 - ShootAll
  //

  if (GameMode[CurrentPlayer] == 2) {

    //
    // FUTURESPA animation - speed is set based on collected letters
    //
    unsigned int countOnes = ShootAll[CurrentPlayer];
    byte numOnes = 0;
    for (int count = 0; count < 26; count++) {
      if ( (countOnes & 0b1) == 1 ) {
        numOnes++;
      }
      countOnes >>= 1;
    }
    if (numOnes<11) {
      FutureSpaIncrement = 200 - numOnes*15;       // Speeds up to 50 at 10 captured
    } else {
      FutureSpaIncrement = 45 - (numOnes-11)*7/3;  // Continues to speed of 10 at 26 captured
    }
    //Serial.print(F("numOnes:                "));
    //Serial.println(numOnes, DEC);  

    FUTURESPA(FutureSpaIncrement);


    //
    // SPA 5000 pts lamp setting - Mode 2
    //

    if (!HaltSpa5KLighting) {
      if ( (ShootAll[CurrentPlayer] & (0b111)) == 7) {   // If all 3 SPA letters are collected
        //Serial.println(F("Mode 2 SPA5K lamp is on"));
        RPU_SetLampState(LA_SPA_5K, 1);
      } else {
        //Serial.println(F("Mode 2 SPA5K lamp is off"));
        RPU_SetLampState(LA_SPA_5K, 0);
      }
    }

    //
    //  LitBumperMode - Mode 2
    //  Achieved when all 3 SPA letters are collected
    //
      if ( (ShootAll[CurrentPlayer] & 0b111) == 7 ) {
        LitBumperMode[CurrentPlayer] = true;
        if (LitBumperModeActive == 0) {                   // Do only when mode inactive
          RPU_SetLampState(LA_BUMPER_BOT_LEFT, 1);
        }
      } else if ((ShootAll[CurrentPlayer] & 0b111) < 7){
        LitBumperMode[CurrentPlayer] = false;
        RPU_SetLampState(LA_BUMPER_BOT_LEFT, 0);
        if (LitBumperModeActive != 0) {                   // In case mode was active
          LitBumperModeActive = 0;                        // Shut off mode in case active
          OverrideScorePriority = 0;                      // Set back to zero
          ShowPlayerScores(0xFF, false, false);           // Reset all score displays
        }
      }


 //
 // Completion Animations
 //
 
    // Saucer segment

    byte HowManySegments = ShootAllCompletedSegments();
    if (HowManySegments >= (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS + 1)) {  // If completed enough segments, animate saucer
      ShowLampAnimation(SaucerPreShootAll2, 2, (200/HowManySegments), CurrentTime, 1, false, false, 99);
      //ShowLampAnimation(SaucerPreShootAll2, 2, 125, CurrentTime, 1, false, false, 99);
    } else if ((ShootAllSaucerComplete[CurrentPlayer]== 1) && ((CurrentTime - ShootAllSaucerCompleteStartTime) > 3500) ) {
      MarqueeAttract(7);
    } else if ((ShootAllSaucerComplete[CurrentPlayer]== 2) && ((CurrentTime - ShootAllSaucerCompleteStartTime) <2000) ) {
      SweepAnimation(SaucerToggle, 4, 2, 40, 1);            // Toggle on/off
    } else if (ShootAllSaucerComplete[CurrentPlayer]== 2) {
      ShootAllSaucerComplete[CurrentPlayer]= 0;             // Reset status
      LettersLighting(1);
    }

    // FUTURE segment
    
    if ((ShootAllFutureComplete[CurrentPlayer]== 1) && ((CurrentTime - ShootAllFutureCompleteStartTime) > 3500) ) {
      MarqueeAttract(13);
    } else if ((ShootAllFutureComplete[CurrentPlayer]== 2) && ((CurrentTime - ShootAllFutureCompleteStartTime) <2000) ) {
      SweepAnimation(FutureToggle, 7, 2, 40, 1);            // Toggle on/off
    } else if (ShootAllFutureComplete[CurrentPlayer]== 2) {
      ShootAllFutureComplete[CurrentPlayer]= 0;             // Reset status
      Serial.println(F("FUTURE segment resetting to 0"));
      LettersLighting(1);
    }
 
    // Kicker segment
    
    if ((ShootAllKickerComplete[CurrentPlayer]== 1) && ((CurrentTime - ShootAllKickerCompleteStartTime) > 3500) ) {
      MarqueeAttract(3);
    } else if ((ShootAllKickerComplete[CurrentPlayer]== 2) && ((CurrentTime - ShootAllKickerCompleteStartTime) <2000) ) {
      SweepAnimation(KickerToggle, 5, 2, 40, 1);            // Toggle on/off
    } else if (ShootAllKickerComplete[CurrentPlayer]== 2) {
      ShootAllKickerComplete[CurrentPlayer]= 0;             // Reset status
      LettersLighting(1);
    }

    // RollOver segment
    
    if ((ShootAllRollOverComplete[CurrentPlayer]== 1) && ((CurrentTime - ShootAllRollOverCompleteStartTime) > 3500) ) {
      MarqueeAttract(2);
    } else if ((ShootAllRollOverComplete[CurrentPlayer]== 2) && ((CurrentTime - ShootAllRollOverCompleteStartTime) <2000) ) {
      SweepAnimation(RolloverToggle, 6, 2, 40, 1);          // Toggle on/off
    } else if (ShootAllRollOverComplete[CurrentPlayer]== 2) {
      ShootAllRollOverComplete[CurrentPlayer]= 0;           // Reset status
      LettersLighting(1);
    }


    // ShootAll Mode 2
    //   Will start once ShootAllStage is at 2, triggered by SW_SAUCER

    switch (ShootAllStage[CurrentPlayer]) {
      case 2:                                                   // Animate lamps
          if ( (CurrentTime - ShootAllIntroStartTime[CurrentPlayer]) < 3000 ) {
            SweepAnimation(FlowDown, 52, 96, 30, 40);           // Downward sweep 52 lamps, 24 frames
          } else {
            AttractSweepLights = 0;                             // Reset animation start point
            ShootAllIntroStartTime[CurrentPlayer] = CurrentTime;
            ShootAllStage[CurrentPlayer] +=1;
            Serial.println(F("ShootAll Stage 2 finished"));
          }
        break;
      case 3:                                                   // Animate lamps and reset lamps
          if ( (CurrentTime - ShootAllIntroStartTime[CurrentPlayer]) < 3250 ) {
            SweepAnimation(SaucerFlowOut, 50, 36, 15, 10);      // Saucer outward sweep 50 lamps, 18 frames
          } else {
            AttractSweepLights = 0;                             // Reset animation start point
            ShootAllIntroStartTime[CurrentPlayer] = CurrentTime;
            ShootAllStage[CurrentPlayer] +=1;
            // Reset all lamps but Mode 2 targets
            RPU_TurnOffAllLamps();                             // Blank all playfield lamps
            HaltArrowsLighting = false;                         // Reset lamps
            HaltSpa5KLighting = false;                          // Reset lamps
            HaltSPSALighting = false;                           // Reset lamps
            HaltSpotsLighting = false;                          // Reset lamps
            SpinnerLighting();                                  // Set Spinner lamps
            BonusMultiplier(1);                                 // Set bonus multiplier to 1, reset lamps
            GateArrowsLighting();                               // Set arrows
            GreenArrowsMode[CurrentPlayer] = false;             // Green arrows do not operate in Mode 2
            GreenArrowsLighting();                              // Set arrows
            RPU_PushToSolenoidStack(SOL_DROP_RESET, 15, true);
            BlankFutureSpaLamps();
            Serial.println(F("ShootAll Stage 3 finished"));
          }
        break;
      case 4:                                                   // Light Mode 2 lamps
          if ( (CurrentTime - ShootAllIntroStartTime[CurrentPlayer]) < 3500 ) {
            SweepAnimation(ShootAllStartLamps, 22, 66, 80, 66); // Light lamps in order
          } else {
            ShootAllIntroStartTime[CurrentPlayer] = CurrentTime;
            ShootAllStage[CurrentPlayer] +=1;
            LettersLighting(1);                                 // Light target lamps
            OverrideScorePriority = 0;                          // Set to zero
            //ShootAllStartTime[CurrentPlayer] = CurrentTime;     // Set Mode clock start time
            ShootAllClockStopped[CurrentPlayer] = false;        // Clock can run
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 3, CurrentTime + 100);  // Eject ball
            Serial.println(F("ShootAll Stage 4 finished"));
            if (DipSwitch30 == 1) {
              RPU_PlaySoundDash51(SOUND_EFFECT_BACKGROUND);   // Starts background sound
            } else if (DipSwitch30 == 3) {
              //CurrentBackgroundSong = 255;                     // Song 255 means no song playing
              NewBackgroundSong(255, true);                    // Request song
            }
          }
          break;
      case 5:                                                   // Clock updates
        //
        // ShootAll Timer display update
        //
        if (!ShootAllClockStopped[CurrentPlayer]) {
          //ShootAllTimeRemaining[CurrentPlayer] = (ShootAllStartTime[CurrentPlayer] + ShootAllDuration[CurrentPlayer] - CurrentTime);
          ShootAllTimeRemaining[CurrentPlayer] -= (CurrentTime - PreviousCurrentTime);       // Shrink time each pass through code
          if ( (ShootAllTimeRemaining[CurrentPlayer] < SHOOT_ALL_LAMP_MIN_TIMER_WARNING)
                && ((CurrentTime - ShootAllTimerCalloutWarningRepeatTime) > 10000)) {
            ShootAllTimerCalloutWarningRepeatTime = CurrentTime;
            Serial.println(F("Clock warning Call Out"));
            CallOut((CALL_OUT_SHOOT_ALL_TIMER_1_ALIVE + 2*(CurrentTime%5)), true);
          }
          if ( !((ShootAllTimeRemaining[CurrentPlayer] == 0) || (ShootAllTimeRemaining[CurrentPlayer] > SHOOTALL_MAX_DURATION)) ) { // If still time left
            if ((CurrentTime - LatestTimerFlash) > TimerFlashPeriod) {
              LatestTimerFlash = CurrentTime;
              // Set solid timer lamps
              //Serial.println(F("Mode 2 - Set solid timer lamps"));
              unsigned long NumLampsToLight = (ShootAllTimeRemaining[CurrentPlayer]/1000)/(SHOOTALL_TIME_PER_LAMP/1000);     // Number of integer lamps, x seconds each
              if (NumLampsToLight > 12) NumLampsToLight = 12;                         // Limit to FUTURESPA1836Spec lamps
              for (int i = 0; i < (NumLampsToLight); i++) {                           // Integer lamps to light
                RPU_SetLampState(ShootAllTimerLamps[i], 1);
              }
              // Set flashing timer lamp
              //Serial.println(F("Mode 2 - Set flashing timer lamp"));
              //unsigned long FlashingLampTimeLeft = SHOOTALL_TIME_PER_LAMP - (ShootAllTimeRemaining[CurrentPlayer] - (NumLampsToLight * 3000)); // 3 seconds per whole lamp
              if (NumLampsToLight < 12) {
                unsigned long FlashingLampTimeLeft = SHOOTALL_TIME_PER_LAMP - (ShootAllTimeRemaining[CurrentPlayer] - (NumLampsToLight * SHOOTALL_TIME_PER_LAMP)); // X seconds per whole lamp
                //TimerFlashPeriod = 25+((150-25)*FlashingLampTimeLeft/3000);
                TimerFlashPeriod = 150 - (5+((150-5)*FlashingLampTimeLeft/SHOOTALL_TIME_PER_LAMP));
                TimerFlashPeriod = constrain (TimerFlashPeriod, 20, 200);
                RPU_SetLampState(ShootAllTimerLamps[NumLampsToLight], (TimerToggle));  // Next lamp after solid ones
                TimerToggle = !TimerToggle;
              }
              // Set remaining lamps off
              //Serial.println(F("Mode 2 - Set remaining lamps off"));
              if (NumLampsToLight <11) {
                for (byte i = (NumLampsToLight+1); i < 12; ++i) {                       // Integer lamps to turn off
                  RPU_SetLampState(ShootAllTimerLamps[i], 0);
                }
              }
            }
          } else {
            // Mode 2 close out
            Serial.println(F("Mode 2 timer ran out"));
            ShootAllFinish();                              // Reset mode
            GameMode[CurrentPlayer] = 1;                   // Return to mode 1
            PlaySoundEffect(SOUND_EFFECT_BALL_LOST, true);
            // Reset timers to let lamps properly reset
            ShootAllFutureCompleteStartTime = 0;
            ShootAllKickerCompleteStartTime = 0;
            ShootAllSaucerCompleteStartTime = 0;
            ShootAllRollOverCompleteStartTime = 0;
            // Reset all lamps
            RPU_TurnOffAllLamps();                        // Blank all playfield lamps
            LettersLighting(1);                            // Reset lamps
            BankShotLighting();
            SaucerLighting();
            KickerShotLighting();
            SpinnerLighting();
            GateArrowsLighting();
            GreenArrowsLighting();
            BonusMultiplierLighting(BonusMult);
          }
        }
        break;
    }


  } // End of Mode 2 section

//
// Mode 3 - Roaming - NormalGamePlay zzzzz
//

  if (GameMode[CurrentPlayer] == 3) {

  // Saucer lamps
    if ( (SaucerLightsTime != 0) && ((CurrentTime-SaucerLightsTime) > (1000+SaucerProgress*750)) ) {
      SaucerLighting();
      RPU_PushToTimedSolenoidStack(SOL_SAUCER, 3, CurrentTime);
      SaucerLightsTime = 0;
    }

    // Bank Shot Animation - Also in Mode 1
    if ( (CurrentTime - BankShotOffTime) < (750+(BankShotProgress*150)) ) {
      //Serial.println(F("Bank Shot Animation running"));
      // Call Marquee Attract for BankShot portion
      MarqueeAttract(2 , 80, true);
    } else if (BankShotOffTime != 0) {
      Serial.println(F("Bank Shot Animation finishing, calling BankShotLighting"));
      Serial.println();
      BankShotLighting();
      BankShotOffTime = 0;
    }
    // Kicker Shot Animation - Also in Mode 1
    if ( (CurrentTime - KickerShotOffTime) < (750+(KickerShotProgress*150)) ) {
      //Serial.println(F("Kicker Shot Animation running"));
      // Call Marquee Attract for KickerShot portion
      MarqueeAttract(3 , 80, true);
    } else if (KickerShotOffTime != 0) {
      Serial.println(F("Kicker Shot Animation finishing, calling KickerShotLighting"));
      Serial.println();
      KickerShotLighting();
      KickerShotOffTime = 0;
    }
    
    switch (RoamingStage) {
      case 1:                                                   // Animation until first switch hit
        { // Braces around case due to variable declaration
          if ( (CurrentTime - RoamingModeTime) < ((unsigned long)RoamingStageDuration*1000) ) {
            //Serial.println(F("Mode 3 Roaming stage 1"));
            if (OVERRIDE_PRIORITY_ROAMING_MODE > OverrideScorePriority) {          // Check if priority should be raised
              OverrideScorePriority = OVERRIDE_PRIORITY_ROAMING_MODE;
            }
            // Over ride displays
            if (OverrideScorePriority == OVERRIDE_PRIORITY_ROAMING_MODE) {
              unsigned int Countdown = (RoamingStageDuration - ((CurrentTime - RoamingModeTime)/1000));
              OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), Countdown, false);
              OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), Countdown, false);
              OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), Countdown, false);
            }
            ShowLampAnimation(RoamingStage1Lamps, 12, 30, CurrentTime, 6, false, false, 99); // SPAFUTURE lamps big and small
          } else {                                                // We have timed out
            PlaySoundEffect(SOUND_EFFECT_SAD);
            RoamingFinish();
            NewBackgroundSong(254, true);
          }
        }
        break;
      case 2:                                                   // Rotating Letters
        { // Braces around case due to variable declaration
          if ( (CurrentTime - RoamingModeTime) < ((unsigned long)RoamingStageDuration*1000)) {
            //Serial.println(F("Mode 3 Roaming stage 2"));
            if (OVERRIDE_PRIORITY_ROAMING_MODE > OverrideScorePriority) {          // Check if priority should be raised
              OverrideScorePriority = OVERRIDE_PRIORITY_ROAMING_MODE;
            }
            if (OverrideScorePriority == OVERRIDE_PRIORITY_ROAMING_MODE) {
              // Override displays
              unsigned int Countdown = (RoamingStageDuration - ((CurrentTime - RoamingModeTime)/1000));
              OverrideScoreDisplay( ((CurrentPlayer%2)?((CurrentPlayer < 2)?0:2):((CurrentPlayer < 2)?1:3)), Countdown, false);
              OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), Countdown, false);                   
              OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), Countdown, false);
            }
            if ( (CurrentTime - RoamingRotateTime) > RoamingIntervals[(NumCapturedLetters - 1)]) {
              RoamingRotateTime = CurrentTime;
              if (!ReverseRoam) { 
                // Shift up one letter
                RoamingLetters = ( (RoamingLetters & (0b011111111)) << 1) + ( (RoamingLetters & (0b100000000)) >> 8);
              } else {
                // Shift down one letter
                RoamingLetters = ( (RoamingLetters & (0b000000001)) << 8) + ( (RoamingLetters & (0b111111110)) >> 1);
              }
              LettersLighting(1);
            }
          } else {                                                // We have timed out, do final animation
            RoamingStage = 3;
            OverrideScorePriority = 0;
            ShowPlayerScores(0xFF, false, false);                 // Reset all score displays
            RoamingModeTime = CurrentTime;
          }
        }
        break;
      case 3:                                                   // Wrap-up animation
        if ( (CurrentTime - RoamingModeTime) < ROAMING_WRAP_UP_DURATION) {
          ShowLampAnimation(RoamingStage3Lamps, 16, 25, CurrentTime, 6, false, false, 99); // SPAFUTURE lamps big and small
        } else {                                                // We have completed wrap-up, shut off mode
          PlaySoundEffect(SOUND_EFFECT_SAD);
          RoamingFinish();
          NewBackgroundSong(254, true);
        }
        break;
      case 4:                                                   // Mode completed, final light show
        if ( (CurrentTime - RoamingModeTime) < (ROAMING_WRAP_UP_DURATION*3/2) ) {
          ShowLampAnimation(RoamingStage3Lamps, 16, 20, CurrentTime, 6, false, false, 99); // SPAFUTURE lamps big and small
        } else {                                                // We have completed wrap-up, shut off mode
          PlaySoundEffect(SOUND_EFFECT_SAD);
          RoamingFinish();
          NewBackgroundSong(254, true);
        }
        break;
    }
  } // End of Mode 3 NormalGamePlay


  // Check to see if ball is in the outhole
  if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
    if (BallTimeInTrough==0) {
      BallTimeInTrough = CurrentTime;
    } else {  // -1-
      // Make sure the ball stays on the sensor for at least 
      // 0.5 seconds to be sure that it's not bouncing
      if ((CurrentTime-BallTimeInTrough)>500) {  // -2-
        if (BallFirstSwitchHitTime == 0 && !Tilted) {
          // Nothing hit yet, so return the ball to the player
          RPU_PushToTimedSolenoidStack(SOL_OUTHOLE, 4, CurrentTime);
          BallTimeInTrough = 0;
          returnState = MACHINE_STATE_NORMAL_GAMEPLAY;
        } else {  // -3-
//        if (BallFirstSwitchHitTime==0) BallFirstSwitchHitTime = CurrentTime;
        
        // if we haven't used the ball save, and we're under the time limit, then save the ball
        if (  !BallSaveUsed && 
              ((CurrentTime-BallFirstSwitchHitTime)/1000)<((unsigned long)BallSaveNumSeconds) ) {
        
          RPU_PushToTimedSolenoidStack(SOL_OUTHOLE, 4, CurrentTime + 100);
          if (BallFirstSwitchHitTime>0) {
            BallSaveUsed = true;
//            RPU_SetLampState(SAME_PLAYER, 0);
//            RPU_SetLampState(HEAD_SAME_PLAYER, 0);
          }
          BallTimeInTrough = CurrentTime;

          returnState = MACHINE_STATE_NORMAL_GAMEPLAY;          
        } else {
          BIPDispValue = CurrentBallInPlay;             // Restore display in case pop count is displayed
          CreditsDispValue = Credits;                   // Restore display in case spinner count is displayed
          returnState = MACHINE_STATE_COUNTDOWN_BONUS;
        }
        } // -3-
      }  // -2-
    } // -1-
  } else {
    BallTimeInTrough = 0;
  }

  return returnState;
}



unsigned long InitGameStartTime = 0;
unsigned long InitGamePlayChime = 0;
boolean ChimeTrigger = true;

int InitGamePlay(boolean curStateChanged) {
  int returnState = MACHINE_STATE_INIT_GAMEPLAY;

  if (curStateChanged) {
    InitGameStartTime = CurrentTime;
    RPU_SetCoinLockout((Credits>=MaximumCredits)?true:false);
    RPU_SetDisableFlippers(true);
    RPU_DisableSolenoidStack();
    RPU_TurnOffAllLamps();
    RPU_SetDisplayBallInPlay(1);
    //if (Credits > 0) {
    //  RPU_SetLampState(LA_CREDIT_INDICATOR, 1);
    //}
    InitGamePlayChime = CurrentTime+1500;
    ChimeTrigger = true;

    //  Halt sounds
    wTrig.stopAllTracks();                  // Halt WavTrigger sounds
    RPU_PlaySoundDash51(SOUND_EFFECT_NONE_25); // Cancels DASH51 existing sounds

    // Array variables for initialization

    for (int count=0; count<4; count++) {  // Set for 4 players
      LitBumperMode[count] = false;
      GreenArrowsMode[count] = false;
      SpinnerCountTotal[count] = 0;
      SpinnerCountLeft[count] = 0;
      SpinnerCountRight[count] = 0;
      SpinnerMode[count]=0;
      PopCount[count]=0;
      PopMode[count] = 0;
      Letters[count] = 0;
      ShootAll[count] = 0;
      GameMode[count] = 1;              // default GameMode is 1
      Goals[count] = 0;

      // Mode 2 initialization

      ShootAllTimeAdded[count] = 0;
      //ShootAllDuration[count] = ShootAllCoinDoorValue*1000;
      //ShootAllStartTime[count] = 0;
      ShootAllClockStopped[count] = true;
      ShootAllStage[count] = 0;
      ShootAllIntroStartTime[count] = 0;

      ShootAllFutureComplete[count] = 0;
      ShootAllKickerComplete[count] = 0;
      ShootAllSaucerComplete[count] = 0;
      ShootAllRollOverComplete[count] = 0;

      ShootAllFutureCounter[count] = 0;
      ShootAllKickerCounter[count] =  0;
      ShootAllRollOverCounter[count] = 0;
      ShootAllSaucerCounter[count] = 0;
      NumBonusesfromPF[count] = 0;

    }

    // Mode 2 timers
    ShootAllKickerCompleteStartTime = 0;
    ShootAllRollOverCompleteStartTime = 0;
    ShootAllSaucerCompleteStartTime = 0;
    ShootAllFutureCompleteStartTime = 0;

    // Mode 3 initialization

    RoamingLetters = 0;

    
    Serial.write("InitGame - state changed\n");

    // Set up general game variables
    CurrentPlayer = 0;
    CurrentNumPlayers = 1; // Added to match Meteor code
    CurrentBallInPlay = 1;
    
    for (int count=0; count<4; count++) CurrentScores[count] = 0;

    //  
    // Force test settings
    //
    //CurrentScores[0] = 975000;
    //GameMode[0] = 2;  // Force mode to ShootAll
    //ShootAllStartTime[CurrentPlayer] = CurrentTime;
    //ShootAllClockStopped[CurrentPlayer] = false;





    // if the ball is in the outhole, then we can move on
    if (RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
      if (DEBUG_MESSAGES) {
        Serial.print(F("InitGame - Ball is in trough - starting new ball\n"));
        Serial.println();
      }
      RPU_EnableSolenoidStack();
      RPU_SetDisableFlippers(false);
      returnState = MACHINE_STATE_INIT_NEW_BALL;
    } else {

      if (DEBUG_MESSAGES) {
        Serial.write("Ball is not in trough - firing stuff and giving it a chance to come back\n");
      }
      
      // Otherwise, let's see if it's in a spot where it could get trapped,
      // for instance, a saucer (if the game has one)
//      RPU_PushToSolenoidStack(SOL_SAUCER, 5, true);

      // And then set a base time for when we can continue
      InitGameStartTime = CurrentTime;
    }

    for (int count=0; count<4; count++) {
      RPU_SetDisplay(count, 0);
      RPU_SetDisplayBlank(count, 0x00);
    }

//    if (Credits > 0) {
//      RPU_SetLampState(LA_CREDIT_INDICATOR, 1);
//    }
    ShowPlayerScores(0xFF, false, false);   //  Reset all score displays  

    CurrentBackgroundSong = 255;            // Song 255 means no song playing
    CreateSongList();                       // Create background song list

  } // End of curStateChanged


//
// Play game start tune
//

  if (ChimeTrigger) {
    ChimeTrigger = false;
    PlaySoundEffect(SOUND_EFFECT_GAME_START);
  }


  // Wait for TIME_TO_WAIT_FOR_BALL seconds, or until the ball appears
  // The reason to bail out after TIME_TO_WAIT_FOR_BALL is just
  // in case the ball is already in the shooter lane.
  if ((CurrentTime-InitGameStartTime)>TIME_TO_WAIT_FOR_BALL || RPU_ReadSingleSwitchState(SW_OUTHOLE)) {
    RPU_EnableSolenoidStack();
    RPU_SetDisableFlippers(false);
    returnState = MACHINE_STATE_INIT_NEW_BALL;
  }

  if (CurrentTime<=InitGamePlayChime) {
    returnState = MACHINE_STATE_INIT_GAMEPLAY;
  }

  return returnState;  
}


//
// Future Spa Bonus Countdown
//  Ver 3 - Mode 2 implemented
//  Ver 2 - Finalize sound effects to match Bally code


boolean CountdownDelayActive = false;
byte ChimeLoop = 0;
byte MaskCount = 0;
int BonusSpace = 117;
byte Segment = 0;
unsigned long NextBonusUpdate = 0;
unsigned long BonusWaitTime = 0;
unsigned long BonusLetters = 0;
unsigned long CountdownDelayStartTime = 0;
unsigned long ClockRelight = 0;
int TargetFlashSpeed[5]={500,250,125,63,0};                // Target 20K flasher speed progression
boolean TargetWasCaptured = false;                         // True if any targets were captured

#define BONUSSPACE_INTER_LETTER_SPACING               158  // msec, sound sequence delay between letters
#define BONUSSPACE_INTRA_LETTER_SPACING                50  // msec, delay within a single letter's sequence
#define TRANSITION_GAP_SPACING                        250  // msec, increased gap between letters and 18K, 36K bonuses
#define BONUSSPACE_INTER_18K_SPACING                  108  // msec, 18K 36K each have 6 sound segments, this is gap between each
#define BONUSSPACE_INTRA_18K_SPACING                   55  // msec, delay between elements within each of the 6 segments for 18K and 36K
#define COUNTDOWN_FINAL_DELAY                        1500  // msec, final delay before transition to next ball

// ShootAll values
#define INTER_SEGMENT_SPACING                        1500  // msec, delay between segments
#define SHOOTALL_RELIGHT_SPACING                       15  // msec, relight uncaptured lamps
#define SHOOTALL_SWAP_SPACING                         100  // msec, swap uncaptured for captured lamps
#define CLOCK_RELIGHT_SPACING                          75  // msec, delay lamps
#define CLOCK_COUNTDOWN_SPACING                       200  // msec, delay lamps

int CountdownBonus(boolean curStateChanged) {
// zzzzz
  if (curStateChanged) {
    CreditsFlashing = false;                // Stop flashing
    BIPFlashing = false;                    // Stop flashing
    ShowPlayerScores(0xFF, false, false);   // Reset all score displays
    BlankFutureSpaLamps();                  // Blank out backglass
    ClearFlashingLampQueue();                 // Remove any lamps from queue
    if (!CallCountdownBonusFromPF) {
      RPU_TurnOffAllLamps();               // Blank all playfield lamps
    } else {
      RPU_SetLampState(LA_BUMPER_BOT_LEFT, 0); // Lamp would flash through bonus cycle
      RPU_SetLampState(LA_SPINNER_RIGHT, 0);
      RPU_SetLampState(LA_SPINNER_LEFT, 0);
      //wTrig.trackPause(CurrentBackgroundSong);
    }
    BonusMultiplierLighting(BonusMult);     // Relight bonus lamps
    MaskCount = 0;
    ChimeLoop = 0;
    Segment = 1;
    NextBonusUpdate = CurrentTime;
    CountdownDelayActive = false;
   
    BonusWaitTime = 0;
    if (GameMode[CurrentPlayer] == 3 ) {    // If we are in Roaming mode revert to Mode 1
      RoamingFinish();
    }
    // Set up for Mode 1 or 2 bonus countdowns
    if (GameMode[CurrentPlayer] == 1 ) {
      BonusLetters = Letters[CurrentPlayer];
      LettersLighting(1);                   // If we tilted Letters are out
      BonusSpace = (!Tilted?(BONUSSPACE_INTER_LETTER_SPACING):(BONUSSPACE_INTER_LETTER_SPACING/2));  // Spacing is half if tilted
    } else if (GameMode[CurrentPlayer] == 2 ) {
      BonusLetters = ShootAll[CurrentPlayer];
      ShootAllWipeLamps();                  // Blank out lamps
      ShootAllWipeTimer();                  // Blank out Mode 2 timer lamps
      BonusSpace = (!Tilted?(SHOOTALL_RELIGHT_SPACING):(SHOOTALL_RELIGHT_SPACING/2));                // Spacing is half if tilted
      // Determine Clock bonus lamps one time here.
      ClockRelight = ((ShootAllTimeRemaining[CurrentPlayer] + (SHOOTALL_TIME_PER_LAMP/2))/1000)/(SHOOTALL_TIME_PER_LAMP/1000);  // Number of integer lamps, rounded up by half lamp
      if (ClockRelight > 12) ClockRelight = 12;
      Serial.print(F("Clock relight NumLampsToLight : "));
      Serial.println(ClockRelight,DEC);
      TargetWasCaptured = false;            // Reset targets captured flag
    }
    CountdownDelayStartTime = CurrentTime;
    //  Halt sounds
    wTrig.stopAllTracks();                  // Halt WavTrigger sounds
    RPU_PlaySoundDash51(SOUND_EFFECT_NONE_25); // Cancels DASH51 existing sounds
    CalloutActive = false;                  // In case one was active
    ClearSoundQueue();
    
    Serial.print(F("CurrentTime is: "));
    Serial.println(CurrentTime, DEC);
    Serial.println(F("-----------------CountdownBonus Start-----------------"));
  }

// Initial delay, allows sound card to be ready
  if (CurrentTime < (CountdownDelayStartTime + 600)) {
    return MACHINE_STATE_COUNTDOWN_BONUS;
  }

    // Letter bonus sound spacing is a burst of 2 sounds times BonusMult.
    // Letter bonus sound effect is 0x1B, 27 DEC
    //                                                                                      Total
    //   1X =  2 sounds               158, 58 msec                                            2
    //   2X =  4 sounds               158, 58, 58, 58 msec                                    4
    //   4X =  8 sounds               158, 58, 58, 58, 58, 58, 58, 58 msec                    8
    //   6X = 12 sounds               158, 58, 58, 58, 58, 58, 58, 58 58, 58, 58, 58 msec    12
    //
    //  Inter-letter spacing is 158 msec
    //  Intra-letter spacing is 58 msec
    //
    // 18K sound effect is 0x07, 7 DEC
    // Initial delay is 222 msec
    //   1X =  6 single sounds at     108 msec spacing                                        6
    //   2x =  6 (*2) sounds          108, 68 msec                                           12
    //   4x =  6 (*4) sounds          108, 68, 68, 68 msec                                   24
    //   6x =  6 (*6) sounds          108, 68, 68, 68, 68, 68 msec                           36
    //
    // 36K sound effect is 0x07, 7 DEC
    // Initial delay is 222 msec
    //   1X =  12 single sounds at    108 msec spacing                                       12
    //   2x =  12 (*2) sounds each at 108, 68 msec                                           24
    //   4x =  12 (*4) sounds each at 108, 68, 68, 68 msec                                   48
    //   6x =  12 (*6) sounds each at 108, 68, 68, 68, 68, 68 msec                           72
    
//  Code for counting down captured letters
//  Will only process letters that are captured, skips uncaptured letters.
//  Of the 18K and 36K bonuses (bits 10 and 11), will process whatever bits are set.  
//  However normal game cannot assign both, only one or the other are set.
//

  if ((Ten_Pts_Stack > 0) || (Hundred_Pts_Stack > 0) || (Thousand_Pts_Stack > 0)) {
    ChimeScoring();
  }

  if (GameMode[CurrentPlayer] == 1) {
    if (MaskCount < 9) {                                          // Count out 9 letters only
      if (BonusLetters & (0b1<<MaskCount)) {                      // If true, letter has been captured
        if ( ChimeLoop < (2 * ((!Tilted)?(BonusMult):1)) ) {      // Multiply loops by bonus multiplier
          if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
            if (!Tilted) {
              if (DipSwitch30 > 1) {
                wTrig.stopAllTracks();                            // Halt all tracks in case we just switched from letters
                //wTrig.trackStop(SOUND_EFFECT_27);                 // Halt previous sound
                wTrig.trackPlayPoly(SOUND_EFFECT_27);             // Play sound effect
              } else {
                RPU_PlaySoundDash51(SOUND_EFFECT_27);  
              }
              Hundred_Pts_Stack += 10;
            }
            NextBonusUpdate = CurrentTime;
            ++ChimeLoop;
            if (ChimeLoop == 1) BonusSpace = ((!Tilted)?(BONUSSPACE_INTRA_LETTER_SPACING):(BONUSSPACE_INTRA_LETTER_SPACING/2));  // Reduced gap intra-letter
          }
        } else {
            RPU_SetLampState((LA_BIG_S + MaskCount), 0);         // Turn off lamp after chime sequence
            ChimeLoop = 0;
            if (MaskCount == 8) {
              Serial.println(F("E was lit, set delay for 18K/36K"));
              BonusSpace = ((!Tilted)?(TRANSITION_GAP_SPACING):(TRANSITION_GAP_SPACING/2));                   // Set to custom 18K/36K interchime spacing
              } else {
              BonusSpace = ((!Tilted)?(BONUSSPACE_INTER_LETTER_SPACING):(BONUSSPACE_INTER_LETTER_SPACING/2)); // Reset back to normal letter interchime spacing
              }
            ++MaskCount;
         }
      } else {
        ++MaskCount;
        if (MaskCount == 9) {
          Serial.println(F("E was NOT lit, set delay for 18K/36K"));
          BonusSpace = ((!Tilted)?(TRANSITION_GAP_SPACING):(TRANSITION_GAP_SPACING/2));   // Set to custom 18K/36K interchime spacing if last letter was not captured
        }
      }
      RPU_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true);  
      //Serial.print(F("Bonus Countdown - Letters loop portion\n"));
      return MACHINE_STATE_COUNTDOWN_BONUS;
    }
  
    //  Enter this section with delay set extra long (222)
    if (MaskCount <11) {                                                        // 18K and 36K Bonus loops
      if (BonusLetters & (0b1<<MaskCount)) {                                    // If true, letter has been captured
        if ( ChimeLoop < ((!Tilted)?(6*BonusMult*((MaskCount==10)?2:1)):1) ) {  // Multiply loops by bonus multiplier
          if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
            if (!Tilted) {
              if (DipSwitch30 > 1) {
                wTrig.stopAllTracks();                                          // Halt all tracks in case we just switch from letters
                //wTrig.trackStop(SOUND_EFFECT_18K);                              // Halt previous sound
                wTrig.trackPlayPoly(SOUND_EFFECT_18K);                          // Play sound effect
              } else {
                RPU_PlaySoundDash51(SOUND_EFFECT_18K);
              }
              //CurrentScores[CurrentPlayer] += 3000;
              Hundred_Pts_Stack += 10;
              Thousand_Pts_Stack += 2;
            }
            NextBonusUpdate = CurrentTime;
            ++ChimeLoop;
  
            BonusSpace = ((!Tilted)?(BONUSSPACE_INTER_18K_SPACING):(BONUSSPACE_INTER_18K_SPACING/2));   // Default inter-group spacing
            //Serial.print(F("18K short delay set to:        "));
            if ( (BonusMult == 2) && (ChimeLoop%2) ) {
              BonusSpace = ((!Tilted)?(BONUSSPACE_INTRA_18K_SPACING):(BONUSSPACE_INTRA_18K_SPACING/2)); // 2x
            } else if ( (BonusMult == 4) && (ChimeLoop%4) ) {
              BonusSpace = ((!Tilted)?(BONUSSPACE_INTRA_18K_SPACING):(BONUSSPACE_INTRA_18K_SPACING/2)); // 4x
            } else if ( (BonusMult == 6) && (ChimeLoop%6) ) {
              BonusSpace = ((!Tilted)?(BONUSSPACE_INTRA_18K_SPACING):(BONUSSPACE_INTRA_18K_SPACING/2)); // 6x
            }
            //Serial.println(BonusSpace, DEC);
          }
        } else {
            RPU_SetLampState((LA_18K_BONUS + MaskCount -9), 0);// Turn off lamp after chime sequence
            ChimeLoop = 0;
            BonusSpace = ((!Tilted)?(TRANSITION_GAP_SPACING):(TRANSITION_GAP_SPACING/2));                // Reset back to interchime spacing
            ++MaskCount;
        }
      } else {
          ++MaskCount;
          BonusSpace = ((!Tilted)?(TRANSITION_GAP_SPACING):(TRANSITION_GAP_SPACING/2));                  // Always set to custom 18K/36K interchime spacing
      }
      RPU_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true);
      //Serial.print(F("Bonus Countdown - Letters loop portion\n"));
      return MACHINE_STATE_COUNTDOWN_BONUS;
    }
  
    // Delay before transition back to new ball or end of game
    if (MaskCount == 11 && CountdownDelayActive == false) {
      NextBonusUpdate = CurrentTime;
      CountdownDelayActive = true;
      return MACHINE_STATE_COUNTDOWN_BONUS;
    }
  } else if (GameMode[CurrentPlayer] == 2) {

  //
  // Mode 2
  //  Starting point is ShootAllLamps and ShootAllTimer lamps are out
  //

    //Serial.print(F("Segment pre-switch is: "));
    //Serial.println(Segment, DEC);
    switch (Segment) {
      case 1:                           // Light all uncaptured ShootAll lamps in sequence
        ShowAuxLampAnimation(AuxDiagonal, 6, 50, CurrentTime, 4, false, false, 99);
        if (MaskCount < 26) {
          unsigned long mask = 1;
          for (int j=0; j<MaskCount; j++) {
            mask *= 2;
          }
          if (!(BonusLetters & mask)) {                         // Turn on uncaptured lamp
            if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
              //Serial.println(F("Segment 1 - Lighting of uncaptured ShootAllLamps"));
              //Serial.print(F("MaskCount is: "));
              //Serial.println(MaskCount, DEC);
              NextBonusUpdate = CurrentTime;
              RPU_SetLampState((ShootAllLamps[MaskCount]), 1); // Turn on lamp
              if (!Tilted) {
                if (DipSwitch30 > 1) {
                  wTrig.trackStop(SOUND_EFFECT_NOTE_1);                              // Halt previous sound
                  wTrig.trackPlayPoly(SOUND_EFFECT_NOTE_1);                          // Play sound effect
                } else {
                  RPU_PlaySoundDash51(SOUND_EFFECT_NOTE_1);
                }
              }
              //ShootAllAddTime(BonusSpace);                      // Top up clock
              ++MaskCount;
            }
            return MACHINE_STATE_COUNTDOWN_BONUS;
          } else {
            ++MaskCount;
            //Serial.println(F("Segment 1 - Skipping Captured lamp"));
            return MACHINE_STATE_COUNTDOWN_BONUS;
          }
        } else {                        // End of segment
          Serial.println(F("End of Segment 1 - Light all uncaptured lamps"));
          ChimeLoop = 0;
          MaskCount = 0;
          ++Segment;
          //Serial.print(F("Segment value is: "));
          //Serial.println(Segment, DEC);
          //Serial.print(F("End of Segment 1, MaskCount is: "));
          //Serial.println(MaskCount, DEC);
          BonusSpace = ((!Tilted)?(INTER_SEGMENT_SPACING):(INTER_SEGMENT_SPACING/2));  // Segment spacing
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        break;

      case 2:                           // Segment delay
        ShowAuxLampAnimation(AuxDiagonal, 6, 50, CurrentTime, 4, false, false, 99);
        if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
          NextBonusUpdate = CurrentTime;
          Serial.println(F("Segment 2 - delay is completed"));
          ++Segment;
          //Serial.print(F("Segment 2 end, segment value is: "));
          //Serial.println(Segment, DEC);
          //ShootAllAddTime(BonusSpace);                      // Top up clock
          BonusSpace = ((!Tilted)?(SHOOTALL_SWAP_SPACING):(SHOOTALL_SWAP_SPACING/2));  // Segment spacing
          return MACHINE_STATE_COUNTDOWN_BONUS;
        } else {
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        break;

      case 3:                           // Swap lamps to light lamps collected
        ShowAuxLampAnimation(AuxDiagonal, 6, 40, CurrentTime, 4, false, false, 99);
        //Serial.println();
        //Serial.println(F("Inside Segment 3"));
        //Serial.print(F("Start of Segment 3, MaskCount is: "));
        //Serial.println(MaskCount, DEC);
        if (MaskCount < 26) {
          //Serial.println(F("Segment 3, MaskCount is <26"));
          //Serial.print(F("BonusSpace set to: : "));
          //Serial.println(BonusSpace, DEC);
          unsigned long mask = 1;
          for (int j=0; j<MaskCount; j++) {
            mask *= 2;
          }
          if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
            NextBonusUpdate = CurrentTime;
            //ShootAllAddTime(BonusSpace);                          // Top up clock
            if (BonusLetters & mask) {                            // If true, letter has been captured
              if (MaskCount > 15 && MaskCount < 21) {             // Handle targets plus Futurity differently
                // Turn on captured lamp at correct flash rate
                RPU_SetLampState((ShootAllLamps[MaskCount]), 1, 0, TargetFlashSpeed[MaskCount-16]);
                TargetWasCaptured = true;                         // Set true if any targets captured
              } else {
                RPU_SetLampState((ShootAllLamps[MaskCount]), 1); // Turn on captured lamp
              }
              
              if (!Tilted) {
                if (DipSwitch30 > 1) {
                  wTrig.trackStop(SOUND_EFFECT_NOTE_4);                              // Halt previous sound
                  wTrig.trackPlayPoly(SOUND_EFFECT_NOTE_4);                          // Play sound effect
                } else {
                  RPU_PlaySoundDash51(SOUND_EFFECT_NOTE_4);
                }
              }
              ++MaskCount;
              //Serial.println(F("Segment 3 - Light captured ShootAllLamp"));
              //Serial.print(F("MaskCount is: "));
              //Serial.println(MaskCount, DEC);
            } else {
              if (!(MaskCount > 15 && MaskCount < 21 && TargetWasCaptured)) { // Skip target lamp if any were captured
                RPU_SetLampState((ShootAllLamps[MaskCount]), 0);  // Turn off uncaptured lamp
              }
              if (!Tilted) {
                if (DipSwitch30 > 1) {
                  wTrig.trackStop(SOUND_EFFECT_NOTE_1);                              // Halt previous sound
                  wTrig.trackPlayPoly(SOUND_EFFECT_NOTE_1);                          // Play sound effect
                } else {
                  RPU_PlaySoundDash51(SOUND_EFFECT_NOTE_1);
                }
              }
              ++MaskCount;
              //Serial.println(F("Segment 3 - Turn off uncaptured ShootAllLamp"));
              //Serial.print(F("MaskCount is: "));
              //Serial.println(MaskCount, DEC);
            }
          }
          return MACHINE_STATE_COUNTDOWN_BONUS;
        } else {                                                 // End of segment
          Serial.println(F("End of Segment 3 - Swap to captured lamps"));
          ChimeLoop = 0;
          MaskCount = 0;
          ++Segment;
          //Serial.print(F("End of Segment 3, MaskCount is: "));
          //Serial.println(MaskCount, DEC);
          BonusSpace = ((!Tilted)?(INTER_SEGMENT_SPACING):(INTER_SEGMENT_SPACING/2));  // Segment spacing
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        break;

      case 4:                           // Segment delay
        ShowAuxLampAnimation(AuxDiagonal, 6, 40, CurrentTime, 4, false, false, 99);
        if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
          NextBonusUpdate = CurrentTime;
          Serial.println(F("Segment 4 - delay is completed"));
          ++Segment;
          //ShootAllAddTime(BonusSpace);                      // Top up clock
          BonusSpace = ((!Tilted)?(SHOOTALL_RELIGHT_SPACING):(SHOOTALL_RELIGHT_SPACING/2));  // Countdown spacing
          //Serial.print(F("BonusSpace set to: : "));
          //Serial.println(BonusSpace, DEC);
          return MACHINE_STATE_COUNTDOWN_BONUS;
        } else {
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        break;

      case 5:                               // Countdown collected lamps
        ShowAuxLampAnimation(AuxDiagonal, 6, 30, CurrentTime, 4, false, false, 99);
        {  // Surround case with braces due to mask variable declaration
        unsigned long mask = 1;
        for (int j=0; j<MaskCount; j++) {
          mask *= 2;
        }
        if (BonusLetters & mask) {                                  // If true, letter has been captured
          //Serial.println(F("CountdownBonus - Processing lamp"));
          if ( ChimeLoop < (2 * ((!Tilted)?(BonusMult):1)) ) {      // Multiply loops by bonus multiplier
            if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
              //ShootAllAddTime(BonusSpace);                          // Top up clock
              if (!Tilted) {
                if (DipSwitch30 > 1) {
                  wTrig.trackStop(SOUND_EFFECT_27);                              // Halt previous sound
                  wTrig.trackPlayPoly(SOUND_EFFECT_27);                          // Play sound effect
                } else {
                  RPU_PlaySoundDash51(SOUND_EFFECT_27);
                }
                Hundred_Pts_Stack += 10;
              }
              NextBonusUpdate = CurrentTime;
              ++ChimeLoop;
              if (ChimeLoop == 1) BonusSpace = ((!Tilted)?(BONUSSPACE_INTRA_LETTER_SPACING):(BONUSSPACE_INTRA_LETTER_SPACING/2));  // Reduced gap intra-letter
            }
          } else {
            if (MaskCount > 15 && MaskCount < 20 && TargetWasCaptured) {     // Note <20 so futurity if captured is always turned off
              RPU_SetLampState((ShootAllLamps[MaskCount]), 1, 0, (TargetFlashSpeed[MaskCount-16])*2/3);  // Set flash rate of captured target lamp  
            } else {
              RPU_SetLampState((ShootAllLamps[MaskCount]), 0);   // Turn off lamp after chime sequence
            }
            //Serial.println(F("Segment 5 - Turn off captured lamp after chime loops"));
            //Serial.print(F("MaskCount is: "));
            //Serial.println(MaskCount, DEC);
            ChimeLoop = 0;
            ++MaskCount;
            if (MaskCount == 26) {
              BonusSpace = ((!Tilted)?(INTER_SEGMENT_SPACING):(INTER_SEGMENT_SPACING/2));                     // Segment spacing
              ChimeLoop = 0;
              MaskCount = 0;
              ++Segment;
              Serial.println(F("Segment 5 - MaskCount == 26, Last captured lamp WAS lit, set inter segment delay"));
              //Serial.print(F("BonusSpace is: "));
              //Serial.println(BonusSpace, DEC);
              //Serial.print(F("Segment (last lamp lit) is: "));
              //Serial.println(Segment, DEC);
            } else {
              BonusSpace = ((!Tilted)?(BONUSSPACE_INTER_LETTER_SPACING):(BONUSSPACE_INTER_LETTER_SPACING/2)); // Reset back to normal letter interchime spacing
              //Serial.println(F("Segment 5 - MaskCount not 26, set inter letter delay"));
              //Serial.print(F("BonusSpace is: "));
              //Serial.println(BonusSpace, DEC);
            }
          }
        } else {
          if (MaskCount > 15 && MaskCount < 21) {  // Any uncaptured upper targets, need to turn off lamp.
            Serial.print(F("Turning off target lamps, segment 5 else section   "));
            Serial.print(F("MaskCount is: "));
            Serial.println(MaskCount, DEC);
            RPU_SetLampState((ShootAllLamps[MaskCount]), 0);   // Turn off lamp after chime sequence
          }
          ++MaskCount;
          if (MaskCount == 26) {
            BonusSpace = ((!Tilted)?(INTER_SEGMENT_SPACING):(INTER_SEGMENT_SPACING/2));   // Segment spacing
            ChimeLoop = 0;
            MaskCount = 0;
            ++Segment;
            Serial.println(F("Segment 5 MaskCount == 26, Last lamp was NOT lit, set Inter Segment delay"));
            //Serial.print(F("BonusSpace is: "));
            //Serial.println(BonusSpace, DEC);
            //Serial.print(F("Segment is: "));
            //Serial.println(Segment, DEC);
          }
        }
        RPU_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true);
        return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        break;

      case 6:                           // Segment delay
        ShowAuxLampAnimation(AuxDiagonal, 6, 30, CurrentTime, 4, false, false, 99);
        if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
          NextBonusUpdate = CurrentTime;
          Serial.println(F("Segment 6 delay is completed"));
          ++Segment;
          //ShootAllAddTime(BonusSpace);                      // Top up clock
          BonusSpace = ((!Tilted)?(CLOCK_RELIGHT_SPACING):(CLOCK_RELIGHT_SPACING/2));  // Countdown spacing
          //Serial.print(F("BonusSpace set to: : "));
          //Serial.println(BonusSpace, DEC);
          BlankFutureSpaLamps();
          if (!CallCountdownBonusFromPF) {
            // Normal end of ball Bonus skips clock relight portion.
            Segment +=3;                                  // Jump by 3 more
          }
          return MACHINE_STATE_COUNTDOWN_BONUS;
        } else {
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        break;

      case 7:                           // Relight clock
        ShowAuxLampAnimation(AuxLeftRight, 8, 45, CurrentTime, 3, false, false, 99);
        //
        // Clock relight gives rounded up number of lamps and awards points for each
        //
        if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
          //Serial.println(F("Clock relight section"));
          //Serial.print(F("MaskCount is: "));
          //Serial.println(MaskCount, DEC);
          NextBonusUpdate = CurrentTime;
          if ( ChimeLoop < ClockRelight) {
            RPU_SetLampState((ShootAllTimerLamps[ChimeLoop]), 1);
            if (!Tilted) {
              if (DipSwitch30 > 1) {
                wTrig.trackStop(SOUND_EFFECT_SCALE_UP);                            // Halt previous sound
                wTrig.trackPlayPoly(SOUND_EFFECT_SCALE_UP);                        // Play sound effect
              } else {
                RPU_PlaySoundDash51(SOUND_EFFECT_SCALE_UP);
              }
            }
            ++ChimeLoop;
          } else {
            ChimeLoop = 0;
            MaskCount = 0;
            ++Segment;
            BonusSpace = ((!Tilted)?(INTER_SEGMENT_SPACING):(INTER_SEGMENT_SPACING/2));  // Segment spacing
            Serial.println(F("Segment 7 - End of clock relight"));
          }
          return MACHINE_STATE_COUNTDOWN_BONUS;
        } else {
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        Serial.println(F("End of clock relight"));      // ****Code never gets here****
        break;

      case 8:                           // Segment delay
        ShowAuxLampAnimation(AuxLeftRight, 8, 45, CurrentTime, 3, false, false, 99);
        if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
          NextBonusUpdate = CurrentTime;
          Serial.println(F("Segment 8 - delay is completed"));
          ++Segment;
          //ShootAllAddTime(BonusSpace);                      // Top up clock
          BonusSpace = ((!Tilted)?(CLOCK_COUNTDOWN_SPACING):(CLOCK_COUNTDOWN_SPACING/2));  // Segment spacing
          return MACHINE_STATE_COUNTDOWN_BONUS;
        } else {
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        break;

      case 9:                           // Countdown clock
        ShowAuxLampAnimation(AuxLeftRight, 8, 25, CurrentTime, 3, false, false, 99);
        //
        // Clock scoring countdown
        //
        if (MaskCount < (ClockRelight)) {                            // If true, cycling through lit clock lamps
          if ( ChimeLoop < (3* ((!Tilted)?(BonusMult):1)) ) {        // Same as Mode 1 letter scoring but 3 cycles per lamp
            if ((CurrentTime - NextBonusUpdate) > BonusSpace) {
              //ShootAllAddTime(BonusSpace);                           // Top up clock
              if (!Tilted) {
                if (DipSwitch30 > 1) {
                  wTrig.trackStop(SOUND_EFFECT_18K);                 // Halt previous sound
                  wTrig.trackPlayPoly(SOUND_EFFECT_18K);             // Play sound effect
                } else {
                  RPU_PlaySoundDash51(SOUND_EFFECT_18K);
                }
                Hundred_Pts_Stack += 10;
              }
              NextBonusUpdate = CurrentTime;
              ++ChimeLoop;
              if (ChimeLoop == 1) BonusSpace = ((!Tilted)?(BONUSSPACE_INTRA_LETTER_SPACING):(BONUSSPACE_INTRA_LETTER_SPACING/2)); // Reduced gap intra-letter
                //Serial.print(F("CountdownBonus - Clock countdown, turn off lamp\n"));
                //Serial.print(F("MaskCount is: "));
                //Serial.println(MaskCount, DEC);
            }
          } else {
            //Serial.print(F("CountdownBonus - Clock countdown, turn off lamp\n"));
            //Serial.print(F("MaskCount is: "));
            //Serial.println(MaskCount, DEC);
            RPU_SetLampState((ShootAllTimerLamps[(ClockRelight-1) - MaskCount]), 0);                       // Turn off lamps from top of lit clock count
            ChimeLoop = 0;
            BonusSpace = ((!Tilted)?(BONUSSPACE_INTER_LETTER_SPACING):(BONUSSPACE_INTER_LETTER_SPACING/2)); // Reset back to interchime spacing
            ++MaskCount;
          }
          RPU_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true);
          return MACHINE_STATE_COUNTDOWN_BONUS;
        }
        // Clock countdown completed, move on to final delay
        NumBonusesfromPF[CurrentPlayer] +=1;             //Increment total times player has achieved bonus from PF
        NextBonusUpdate = CurrentTime;
        ++Segment;
        //Serial.print(F("Segment is: \n"));
        //Serial.println(Segment, DEC);
        Serial.print(F("Segment 9 - Clock countdown completed\n"));
        return MACHINE_STATE_COUNTDOWN_BONUS;
        break;

      case 10:                           // Final delay triggering
        // Delay before transition back to new ball or end of game
        if (CountdownDelayActive == false) {
          Serial.print(F("Segment 10 - Final delay triggered\n"));
          CountdownDelayActive = true;
          //ShootAllAddTime(COUNTDOWN_FINAL_DELAY);       // Top up clock - special amount for final delay
        }
        break;
      default:
        Serial.print(F("Switch default, should not end up here\n"));
        break;

    }                                                             // End of switch


  }   // End of GameMode== 2

  RPU_SetDisplay(CurrentPlayer, CurrentScores[CurrentPlayer], true);  // Show final updates
  
  if ( ((CurrentTime - NextBonusUpdate) < COUNTDOWN_FINAL_DELAY) && CountdownDelayActive == true) {  //Delays transition back to regular play
    return MACHINE_STATE_COUNTDOWN_BONUS; 
  }

  Serial.print(F("Bonus Countdown - end\n"));

  if (CallCountdownBonusFromPF) {                     // If called from PF return to NGP
    Serial.print(F("Bonus Countdown - Resetting ShootAll completed segments\n"));    
    // Zero out completed segments from ShootAll and return to NormalGamePlay
    if (ShootAllFutureComplete[CurrentPlayer]) {
      ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111111111111111111111000000111));
      ShootAllFutureComplete[CurrentPlayer]= 0;       // Reset
      ShootAllFutureCounter[CurrentPlayer] = 0;       // Reset counter
    }
    if (ShootAllKickerComplete[CurrentPlayer]) {
      ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111111111111111110000111111111));
      ShootAllKickerComplete[CurrentPlayer]= 0;       // Reset
      ShootAllKickerCounter[CurrentPlayer] = 0;       // Reset counter
    }
    if (ShootAllSaucerComplete[CurrentPlayer]) {
      ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111111111111110001111111111111));
      ShootAllSaucerComplete[CurrentPlayer]= 0;       // Reset
      ShootAllSaucerCounter[CurrentPlayer] = 0;       // Reset counter
    }
    if (ShootAllRollOverComplete[CurrentPlayer]) {
      ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111100000111111111111111111111));
      ShootAllRollOverComplete[CurrentPlayer]= 0;     // Reset
      ShootAllRollOverCounter[CurrentPlayer] = 0;     // Reset counter
    }
    LettersLighting(1);                               // Need to relight blanked lamps
    SpinnerLighting();                                // Restore per spinner status
    RPU_PushToTimedSolenoidStack(SOL_SAUCER, 3, CurrentTime + 100); // SW_Saucer cannot release the ball
    CallCountdownBonusFromPF = false;                 // Reset

    if (DipSwitch30 == 1) {
      RPU_PlaySoundDash51(SOUND_EFFECT_BACKGROUND);  // Bonus called from Mode 2, start background sound
    } else if (DipSwitch30 == 3) {
      //wTrig.trackResume(CurrentBackgroundSong);
      //CurrentBackgroundSong = 255;                     // Song 255 means no song playing
      NewBackgroundSong(254);                         // Request song
    }

    return MACHINE_STATE_NORMAL_GAMEPLAY;
  } else {
    return MACHINE_STATE_BALL_OVER;
  }

// End of CountdownBonus

}

#ifdef RPU_OS_USE_DASH51
//#ifdef BALLY_STERN_OS_USE_DASH51


/*#define SOUND_QUEUE_SIZE 30
struct SoundQueueEntry {
  byte soundByte;
  unsigned long playTime;
};
SoundQueueEntry SoundQueue[SOUND_QUEUE_SIZE];
*/

void ClearSoundQueue() {
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    SoundQueue[count].soundByte = 0xFF;
  }
}

boolean CheckForScheduledCallouts() {
  boolean retVal=false;
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    // if valid callout start event found then flag
    if ( (SoundQueue[count].soundByte >= CALL_OUT_TRACK_NUMBER_OFFSET)&&((SoundQueue[count].soundByte)%2 == 0) ) {
      Serial.println(F("Found valid callout start event in sound queue"));
      retVal=true;
      break;
    }
  }
  return retVal;
}



void ClearCalloutsFromSoundQueue() {
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    if (SoundQueue[count].soundByte >= CALL_OUT_TRACK_NUMBER_OFFSET) {
      SoundQueue[count].soundByte = 0xFF;      
    }
  }
}

boolean AddToSoundQueue(unsigned int soundByte, unsigned long playTime, byte priority) {
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    if (SoundQueue[count].soundByte==0xFF) {
      SoundQueue[count].soundByte = soundByte;
      SoundQueue[count].playTime = playTime;
      SoundQueue[count].priority = priority;
      return true;
    }
  }
  return false;
}

unsigned int GetFromSoundQueue(unsigned long pullTime) {
  for (int count=0; count<SOUND_QUEUE_SIZE; count++) {
    if (SoundQueue[count].soundByte!=0xFF && SoundQueue[count].playTime<pullTime) {
      unsigned int retSound = SoundQueue[count].soundByte;
      if (retSound >= CALL_OUT_TRACK_NUMBER_OFFSET) {
        Serial.print(F("GetFromSoundQueue found a call out:"));
        retSound = retSound*100 + SoundQueue[count].priority;
        Serial.println(retSound, DEC);
      }
      SoundQueue[count].soundByte = 0xFF;
      return retSound;
    }
  }

  return 0xFF;
}

#endif




int RunGamePlayMode(int curState, boolean curStateChanged) {
  int returnState = curState;
  unsigned long scoreAtTop = CurrentScores[CurrentPlayer];
  
  // Very first time into gameplay loop
  if (curState==MACHINE_STATE_INIT_GAMEPLAY) {
    returnState = InitGamePlay(curStateChanged);    
  } else if (curState==MACHINE_STATE_INIT_NEW_BALL) {
    returnState = InitNewBall(curStateChanged, CurrentPlayer, CurrentBallInPlay);
  } else if (curState==MACHINE_STATE_NORMAL_GAMEPLAY) {
    returnState = NormalGamePlay();
  } else if (curState==MACHINE_STATE_COUNTDOWN_BONUS) {
    returnState = CountdownBonus(curStateChanged);
//    returnState = MACHINE_STATE_BALL_OVER;
  } else if (curState==MACHINE_STATE_BALL_OVER) {    
    if (SamePlayerShootsAgain) {
      returnState = MACHINE_STATE_INIT_NEW_BALL;
    } else {
      CurrentPlayer+=1;
      if (CurrentPlayer>=CurrentNumPlayers) {
        CurrentPlayer = 0;
        CurrentBallInPlay+=1;
      }
      if (CurrentBallInPlay>BallsPerGame) {
        CheckHighScores();
      if (DipSwitch30 >1) {                           // If set for WavTrigger sounds
        wTrig.stopAllTracks();
        wTrig.trackPlayPoly(SOUND_EFFECT_GAME_OVER);
        wTrig.trackGain(SOUND_EFFECT_GAME_OVER, SFxVolume);
      } else {
        RPU_PlaySoundDash51(SOUND_EFFECT_GAME_OVER); // End of game tune          
      }
      for (int count=0; count<CurrentNumPlayers; count++) {
        RPU_SetDisplay(count, CurrentScores[count], true, 2);
      }
        returnState = MACHINE_STATE_MATCH_MODE;
      }
      else returnState = MACHINE_STATE_INIT_NEW_BALL;
    }    
  } else if (curState==MACHINE_STATE_MATCH_MODE) {
//    Serial.print(F("Run ShowMatchSequence\n"));
    returnState = ShowMatchSequence(curStateChanged);
//  This line does nothing but force another pass until Attract gets called since nothing matches state
//    returnState = MACHINE_STATE_GAME_OVER;    
  } else {
    returnState = MACHINE_STATE_ATTRACT;
  }

  byte switchHit;
    while ( (switchHit=RPU_PullFirstFromSwitchStack())!=SWITCH_STACK_EMPTY ) {   // -A-

      if (!Tilted) {
  
      switch (switchHit) {
        case SW_SELF_TEST_SWITCH:
          returnState = MACHINE_STATE_TEST_LIGHTS;
          SetLastSelfTestChangedTime(CurrentTime);
          break; 

  //
  // Future Spa game switch responses here
  //

        case SW_30_PTS:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          //ClearFlashingLampQueue();
          //wTrig.trackPlayPoly(150);                  // Start next song at low volume
          //wTrig.trackGain(150, (MusicVolume-30));    // Preset next song gain to -XdB
          //wTrig.trackLoop(NextBackgroundSong, 1);                   // Loop song
          //wTrig.trackFade(150, MusicVolume, 5000, false);  // Fade Track 1 up to 0db over 3 secs
          //AddToSoundQueue(102, (CurrentTime+2500));
          //AddToSoundQueue(254, CurrentTime+10000);
          //AddToSoundQueue(1000, CurrentTime+9000);             // Announce start of FUTURE mode
          //NewBackgroundSong(255, true);                    // Request song
          //NewBackgroundSong(175);                    // Request song
          //CallOut(100);                                // Request call out
          //CallOut(1001, false, (CurrentTime+2000));     // Delayed call out
          //CallOut(1000, false, (CurrentTime+9000));     // Delayed call out
          //CallOut(100, true);     // 
          Ten_Pts_Stack +=3;
          DIP24Total +=1;
          PlaySoundEffect(SOUND_EFFECT_30_PT);
          ShootAllAddTime(1500);
          if (DropTargetsToBeReset) {
            ResetDropTargets();
          }
          break;
        case SW_LANE_F:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (!CaptureLetter(3)) {                        // If letter already captured, CaptureLetter is false
            Serial.println(F("Letter F previously captured"));
            Hundred_Pts_Stack += 5;
            PlaySoundEffect(SOUND_EFFECT_SPA);
          }
          break;
        case SW_LANE_U1:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (!CaptureLetter(4)) {                        // If letter already captured, CaptureLetter is false
            Serial.println(F("Letter U1 previously captured"));
            if (GreenArrowsMode[CurrentPlayer]) {
              PlaySoundEffect(SOUND_EFFECT_SPINNER_100_PT);
              Thousand_Pts_Stack += 5;
            } else {
              PlaySoundEffect(SOUND_EFFECT_SPA);
              Hundred_Pts_Stack += 5;
            }
          }
          // Green Arrows Mode
          if (GreenArrowsModeActive) {                      // If supplementary mode is active
            Thousand_Pts_Stack += (LitGreenArrowsTable[GreenArrowsModeActive].score/1000);  // Award score
            // Modes 2 or 3 cannot trigger base mode so turn off when award collected
            if ( (GameMode[CurrentPlayer] == 2) || (GameMode[CurrentPlayer] == 3) ) GreenArrowsMode[CurrentPlayer] = 0;
            GreenArrowsModeActive = 0;                      // Cancel supplementary mode
            GreenArrowsLighting();
            if (OverrideScorePriority == OVERRIDE_PRIORITY_GREEN_ARROWS_MODE) { // Check something else is not over riding
              OverrideScorePriority = 0;                    // Set back to zero
              ShowPlayerScores(0xFF, false, false);         // Reset all score displays
            }
            Serial.println(F("Lit Bumper Supplementary Mode was awarded"));
          }
          break;
        case SW_LANE_T:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          //CallOut(504, 30);     // Priority immediate call out
          //wTrig.trackPlayPoly(151);                  // Start next song at low volume
          //wTrig.trackGain(150, (MusicVolume));    // Preset next song gain to -XdB
          //NewBackgroundSong(152, true);                    // Request song
          if (!CaptureLetter(5)) {  // If letter already captured, CaptureLetter is false
            Serial.println(F("Letter T previously captured"));
            Hundred_Pts_Stack += 5;
            PlaySoundEffect(SOUND_EFFECT_SPA);
          }
          if (GateOpen) {
            GateOpen = false;
            RPU_SetDisableGate(false);
            GateArrowsLighting();
            //RPU_SetLampState(LA_ARROW_LANE_U2, 0);
            //RPU_SetLampState(LA_ARROW_LANE_T, 0);
            //RPU_SetLampState(LA_GATE_ARROW, 1);
          }
          break;
        case SW_LANE_U2:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (!CaptureLetter(6)) {                       // If letter already captured, CaptureLetter is false
            Serial.println(F("Letter U2 previously captured"));
            Hundred_Pts_Stack += 5;
            PlaySoundEffect(SOUND_EFFECT_SPA);
          }
          if (GateOpen) {
            GateOpen = false;
            RPU_SetDisableGate(false);
            GateArrowsLighting();
            //RPU_SetLampState(LA_ARROW_LANE_U2, 0);
            //RPU_SetLampState(LA_ARROW_LANE_T, 0);
            //RPU_SetLampState(LA_GATE_ARROW, 1);
          }
          break;
        case SW_LANE_R:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (!CaptureLetter(7)) {                        // If letter already captured, CaptureLetter is false
            Serial.println(F("Letter R previously captured"));
            if (GreenArrowsMode[CurrentPlayer]) {
              PlaySoundEffect(SOUND_EFFECT_SPINNER_100_PT);
              Thousand_Pts_Stack += 5;
            } else {
              PlaySoundEffect(SOUND_EFFECT_SPA);
              Hundred_Pts_Stack += 5;
            }
          }
          
          // Green Arrows Mode
          if (GreenArrowsModeActive) {                      // If supplementary mode is active
            Thousand_Pts_Stack += (LitGreenArrowsTable[GreenArrowsModeActive].score/1000);  // Award score
            // Modes 2 or 3 cannot trigger base mode so turn off when award collected
            if ( (GameMode[CurrentPlayer] == 2) || (GameMode[CurrentPlayer] == 3)) GreenArrowsMode[CurrentPlayer] = 0;
            GreenArrowsModeActive = 0;                      // Cancel supplementary mode
            GreenArrowsLighting();
            if (OverrideScorePriority == OVERRIDE_PRIORITY_GREEN_ARROWS_MODE) { // Check something else is not over riding
              OverrideScorePriority = 0;                    // Set back to zero
              ShowPlayerScores(0xFF, false, false);         // Reset all score displays
            }
            Serial.println(F("Lit Bumper Supplementary Mode was awarded"));
          }
          break;
        case SW_LANE_E:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (!CaptureLetter(8)) {                        // If letter already captured, CaptureLetter is false
            Serial.println(F("Letter E previously captured"));
            Hundred_Pts_Stack += 5;
            PlaySoundEffect(SOUND_EFFECT_SPA);
          }
          break;
        case SW_SPA_TARGET:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if ((CurrentTime-LastRollSPAHitTime) > 250) {
            if (!CaptureLetter(0)) {                      // If letter already captured, CaptureLetter is false
              Serial.println(F("All SPA letters previously captured"));
              Thousand_Pts_Stack += 5;
              // 5000 pt SPA target sound
              PlaySoundEffect(SOUND_EFFECT_SPA_5K);
            }
            //
            // Lit Target Mode
            //
            //if (GameMode[CurrentPlayer] == 1) {
              if (LitBumperModeActive) {                                              // If mode is active
                LitBumperModeActive += 1;                                             // Increment for each hit
                if (LitBumperModeActive > 5) LitBumperModeActive = 5;                 // Limit to 5
                // Set bumper lamp flash rate per number of target hits
                RPU_SetLampState(LA_BUMPER_BOT_LEFT, 1, 0, LitBumper[LitBumperModeActive].flashRate);
                LitBumperIncrementTime = CurrentTime;                                 // Set timer
              }
              if ( (LitBumperMode[CurrentPlayer]) && (LitBumperModeActive == 0) ) {   // Only turn on if off
                LitBumperModeActive = 1;                                              // Set mode active
                RPU_SetLampState(LA_BUMPER_BOT_LEFT, 1, 0, LitBumper[LitBumperModeActive].flashRate);
                LitBumperIncrementTime = CurrentTime;                                 // Set timer
              }
            //}
          }
          break;
        case SW_BUMPER_TOP_LEFT:
        case SW_BUMPER_TOP_RIGHT:
        case SW_BUMPER_TOP_CENTER:
        case SW_BUMPER_BOT_RIGHT:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (DropTargetsToBeReset) {
            ResetDropTargets();
          }
          DIP24Total +=1;
          PopCount[CurrentPlayer] = ++PopCount[CurrentPlayer];
          if (PopCount[CurrentPlayer] > (Pop_Threshold-1)) {
            PopMode[CurrentPlayer] += 1;
            SetGoals(2);
            PopCount[CurrentPlayer] = 0;
            PlaySoundEffect(SOUND_EFFECT_POP_MODE, true);
          }
          
          #ifdef EXECUTION_MESSAGES
          Serial.print(F("PopMode is:  "));
          Serial.println(PopMode[CurrentPlayer], DEC);  
          Serial.print(F("PopCount is: "));
          Serial.println(PopCount[CurrentPlayer], DEC);  
          #endif
          
          // Display Pop bumper countdown
          if ( (PopCount[CurrentPlayer] > (Pop_Threshold*.33)) && (PopCount[CurrentPlayer] < Pop_Threshold) ) {
              //  Set display flashing and change to pop hits left until mode achieved
              BIPFlashing = true;
              BIPDispValue = (Pop_Threshold - PopCount[CurrentPlayer]);
            } else {
              //  Set display static and set back to BIP
              BIPFlashing = false;
              BIPDispValue = CurrentBallInPlay;
            }
          
          // Bumper scoring
          if ((PopMode[CurrentPlayer]) < 1) {
            PlaySoundEffect(SOUND_EFFECT_BUMPER);
            Hundred_Pts_Stack += 1;
          } else if ((PopMode[CurrentPlayer]) == 1) {
            PlaySoundEffect(SOUND_EFFECT_BUMPER);
            Thousand_Pts_Stack += 1;
          } else if ((PopMode[CurrentPlayer]) > 1) {
            PlaySoundEffect(SOUND_EFFECT_BUMPER);
            Thousand_Pts_Stack += 2;
          }

          // Mode 2 Add time
          ShootAllAddTime(850);
          break;
        case SW_BUMPER_BOT_LEFT:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (DropTargetsToBeReset) {
            ResetDropTargets();
          }
          DIP24Total +=1;
          PopCount[CurrentPlayer] = ++PopCount[CurrentPlayer];
          if (PopCount[CurrentPlayer] > (Pop_Threshold-1)) {
            PopMode[CurrentPlayer] += 1;
            SetGoals(2);
            PopCount[CurrentPlayer] = 0;
            PlaySoundEffect(SOUND_EFFECT_POP_MODE);
          }
          
          #ifdef EXECUTION_MESSAGES
          Serial.print(F("PopMode is:  "));
          Serial.println(PopMode[CurrentPlayer], DEC);  
          Serial.print(F("PopCount is: "));
          Serial.println(PopCount[CurrentPlayer], DEC);  
          #endif
          
          // Display Pop bumper countdown
          if ( (PopCount[CurrentPlayer] > (Pop_Threshold*.33)) && (PopCount[CurrentPlayer] < Pop_Threshold) ) {
              //  Set display flashing and change to pop hits left until mode achieved
              BIPFlashing = true;
              BIPDispValue = (Pop_Threshold - PopCount[CurrentPlayer]);
            } else {
              //  Set display static and set back to BIP
              BIPFlashing = false;
              BIPDispValue = CurrentBallInPlay;
            }
          
          // Bumper scoring
          if ((PopMode[CurrentPlayer]) < 1) {
            PlaySoundEffect(LitBumperMode[CurrentPlayer]?SOUND_EFFECT_BUMPER_LL:SOUND_EFFECT_BUMPER);
            if (LitBumperMode[CurrentPlayer]) {                       // If lit 1000 pts, if not 100 pts
              Thousand_Pts_Stack += 1;
            } else {
              Hundred_Pts_Stack += 1;
            }
          } else if ((PopMode[CurrentPlayer]) == 1) {
            PlaySoundEffect(LitBumperMode[CurrentPlayer]?SOUND_EFFECT_BUMPER_LL:SOUND_EFFECT_BUMPER);
            Thousand_Pts_Stack += 1;
          } else if ((PopMode[CurrentPlayer]) > 1) {
            PlaySoundEffect(LitBumperMode[CurrentPlayer]?SOUND_EFFECT_BUMPER_LL:SOUND_EFFECT_BUMPER);
            Thousand_Pts_Stack += 2;
          }

          // Lit Bumper Mode
          if (LitBumperModeActive) {                                   // If mode is active
            SetGoals(5);                                               // Award goal
            Thousand_Pts_Stack += (LitBumper[LitBumperModeActive].score/1000);// Award score
            LitBumperModeActive = 0;                                   // Cancel mode
            RPU_SetLampState(LA_BUMPER_BOT_LEFT, 0);
            if (OverrideScorePriority == OVERRIDE_PRIORITY_LIT_BUMPER_MODE) { // Check something else is not over riding
              OverrideScorePriority = 0;                               // Set back to zero
              ShowPlayerScores(0xFF, false, false);                    // Reset all score displays
            }
            Serial.println(F("Lit Bumper Mode was awarded"));
          }

          // Mode 2 Add time
          ShootAllAddTime(850);
          break;
        case SW_TILT:
          // This should be debounced
          Serial.print(F("(CurrentTime - LastTiltWarningTime) is:    "));
          Serial.println((CurrentTime - LastTiltWarningTime), DEC);
          if ((CurrentTime - LastTiltWarningTime) > TILT_WARNING_DEBOUNCE_TIME) {
            Serial.println(F("Greater than Tilt Debounce time"));
            LastTiltWarningTime = CurrentTime;
            NumTiltWarnings += 1;
            if (NumTiltWarnings > MaxTiltWarnings) {
              Tilted = true;
              Serial.println(F("!!!! Tilted !!!!"));
              RPU_SetDisableFlippers(true);
              RPU_SetDisableGate(true);
              RPU_TurnOffAllLamps();
              RPU_SetLampState(TILT, 1);
              RPU_DisableSolenoidStack();
              //  Halt sounds
              wTrig.stopAllTracks();                              // Halt WavTrigger sounds
              RPU_PlaySoundDash51(SOUND_EFFECT_NONE_25);         // Cancels DASH51 existing sounds
              CalloutActive = false;                              // In case one was active
              ClearSoundQueue();
              PlaySoundEffect(SOUND_EFFECT_TILT);
            } else {
            Serial.println(F("Tilt Warning!!!!"));
            // Won't play for some reason unless coming from sound queue
            AddToSoundQueue(SOUND_EFFECT_27, CurrentTime+100);
            //PlaySoundEffect(SOUND_EFFECT_27);
            }
          }
          break;
        case SW_ROLLOVER_BUTTON:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (DropTargetsToBeReset) {
            ResetDropTargets();
          }
          if ((CurrentTime-LastRollOverHitTime) > 1500) {
            LastRollOverHitTime = CurrentTime;
            if (GameMode[CurrentPlayer] == 1) {
              CaptureLetter(99);
              BankShotScoring(); // Call BankShotScoring
            } else if (GameMode[CurrentPlayer] == 2) {
              //CaptureLetter(21);
              if (!CaptureLetter(21)) {  // If letter already captured, CaptureLetter is false
                Serial.println(F("Mode 2 Rollover lamps all captured"));
                Hundred_Pts_Stack += 5;
                PlaySoundEffect(SOUND_EFFECT_SPA);
              }
            } else if (GameMode[CurrentPlayer] == 3) {
              BankShotScoring(); // Call BankShotScoring
            }
          }
          break;
        case SW_LEFT_KICKBACK:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if ((CurrentTime-LastKickerHitTime) > 1500) {
            LastKickerHitTime = CurrentTime;
            if (GameMode[CurrentPlayer] == 1) {
              if ( (((DIP24Total+1)%3) == 0) || DipSwitch24 ) {
                CaptureLetter(99);
              }
              KickerShotScoring(); // Call KickerShotScoring
              //
              // Kicker Saucer Combo Start
              //
              Kicker_Saucer_Start_Time = CurrentTime;
              if (Kicker_Saucer_Stage == 0) { 
                Serial.println(F("Starting Kicker_Saucer Combo"));
                Kicker_Saucer_Stage = 1;
              }
            } else if (GameMode[CurrentPlayer] == 2) {
              if (!CaptureLetter(9)) {  // If letter already captured, CaptureLetter is false
                Serial.println(F("Mode 2 Kicker lamps all captured"));
                Hundred_Pts_Stack += 5;
                PlaySoundEffect(SOUND_EFFECT_SPA);
              }
              RPU_PushToTimedSolenoidStack(SOL_KICKER, 3, CurrentTime + 1500);
            } else if (GameMode[CurrentPlayer] == 3) {
              KickerShotScoring(); // Call KickerShotScoring
              /*//
              // Kicker Saucer Combo Start
              //
              Kicker_Saucer_Start_Time = CurrentTime;
              if (Kicker_Saucer_Stage == 0) { 
                Serial.println(F("Starting Kicker_Saucer Combo"));
                Kicker_Saucer_Stage = 1;
              }*/
            }
          }
          break;
        case SW_SPINNER_LT:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (DropTargetsToBeReset) {
            ResetDropTargets();
          }
          
          // Unlock code for right spinner
          if (SpinnerStatus == 3) { // Left spinner increments right spinner unlock counter
            SpinnerRightUnlockCounter += 1;
            Serial.println(F("Unlocking right spinner"));
            if (SpinnerRightUnlockCounter > (Spinner_Threshold_Total*SPINNER_RIGHT_UNLOCK_FRACTION) ) {  // Left spinner turns 10% of Threshold to unlock
              SpinnerStatus = 1;
              SpinnerLighting();
              SpinnerRightUnlockCounter = 0;
            }
          }

          // Spinner totals and goal setting
          if (SpinnerStatus != 2) {
            SpinnerCountLeft[CurrentPlayer] += 1;                   // Left spinner total
            SpinnerCountTotal[CurrentPlayer] += 1;                  // Spinner total increment
            //Serial.println(F("Increment SpinnerCountLeft and ...Total"));
          }
          if (SpinnerCountTotal[CurrentPlayer] > Spinner_Threshold_Total) {
            SpinnerMode[CurrentPlayer] += 1;
            SpinnerCountTotal[CurrentPlayer] = 0;
            //Serial.println(F("Zeroing spinner total"));
            PlaySoundEffect(SOUND_EFFECT_SPINNER_MODE, true);
            SetGoals(3);                                          // Spinner threshold met
            // Trigger FUTURE mode
            if (GameMode[CurrentPlayer] == 1) {
              FutureModeActive = 1;                               // Triggers mode
              Future_Mode_Start_Time = CurrentTime;
              FutureModeLetter = 3;                               // First pattern
              NewBackgroundSong(150, true);
              CallOut(CALL_OUT_FUTURE_MODE_SPELL_FUTURE, 30, (CurrentTime+3000)); // Announce start of FUTURE mode
              Serial.println(F("Started FUTURE mode."));
            }
          }

          #ifdef EXECUTION_MESSAGES
          //Serial.print(F("SpinnerMode is:  "));
          //Serial.println(SpinnerMode[CurrentPlayer], DEC);  
          //Serial.print(F("SpinnerCountTotal is: "));
          //Serial.println(SpinnerCountTotal[CurrentPlayer], DEC);  
          #endif

          // Calculate spinner speed
          SpinnerFastLeft = false;
          //Serial.print(F("CurrentTime - LastSpinnerLeftTime is:  "));
          //Serial.println((CurrentTime - LastSpinnerLeftTime), DEC);
          if ( ((CurrentTime - LastSpinnerLeftTime) < SPINNER_SPEED_LEFT_THRESHOLD) && 
               (SpinnerStatus == 1 || SpinnerStatus == 3) ){
            SpinnerFastLeft = true;
            //Serial.println(F("Fast left spinner"));
          }
          LastSpinnerLeftTime = CurrentTime;

          // Spinner status update
          if (SpinnerCountLeft[CurrentPlayer] > ((Spinner_Threshold_Total*SPINNER_LEFT_LOCK_FRACTION) - 1) ) {    // Locks at 40% of Total
            // Set goals Spinner Left Mode 1
            Serial.println(F("Increment Spinner Mode"));
            SpinnerStatus = 2;                                // Left spinner off, right spinner flashing
            SpinnerLighting();
            // Play spinner goal sound, needs sound queue to delay other sounds
            SpinnerCountLeft[CurrentPlayer] = 0;
            SpinnerRightUnlockCounter = 0;
          }
           
          // Display spinner countdown
          if ( (SpinnerCountTotal[CurrentPlayer] > (Spinner_Threshold_Total*.33)) /*&& (SpinnerCount[CurrentPlayer] < Spinner_Threshold)*/ ) {
            CreditsFlashing = true;
            CreditsDispValue = (Spinner_Threshold_Total - SpinnerCountTotal[CurrentPlayer]);
          } else {
            CreditsFlashing = false;
            CreditsDispValue = Credits;
          }
           
          // Spinner scoring
          switch (SpinnerStatus) {
            case 0:   // Spinners both off
              Hundred_Pts_Stack += 1;
              break;
            case 1:   // Spinners both on
              Thousand_Pts_Stack += (SpinnerFastLeft?2:1);
              break;
            case 2:   // Left spinner off, right spinner flashing
              Ten_Pts_Stack += 1;
              break;
            case 3:   // Left spinner flashing, right spinner off
              Thousand_Pts_Stack += (SpinnerFastLeft?2:1);
              break;
          }
          
          // Spinner sounds
          switch (SpinnerStatus) {
            case 0:                               // Spinners both off
              SpinnerFlips +=1;                   // Increment time sound effects accumulator
              break;
            case 1:                               // Spinners both on
            case 3:                               // Left spinner flashing, right spinner off
            if (SpinnerFastLeft) {
                PlaySoundEffect(SOUND_EFFECT_SCALE_UP);
              } else {
                SpinnerFlips +=1;                 // Increment time sound effects accumulator
              }
              break;
            case 2:                               // Left spinner off, right spinner flashing
              //PlaySoundEffect(SOUND_EFFECT_SAD);
              //PlaySoundEffect(SOUND_EFFECT_NOTE_4);
              //AddToSoundQueue(SOUND_EFFECT_NOTE_1, CurrentTime+10);
              PlaySoundEffect(SOUND_EFFECT_NOTE_1);
              break;
          }

          // Mode 2 Add time
          ShootAllAddTime(700);
          //Serial.print(F("ShootAllTimeRemaining[CrtPlyr]:      "));
          //Serial.println(ShootAllTimeRemaining[CurrentPlayer], DEC);
          break;
        case SW_SPINNER_RT:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (DropTargetsToBeReset) {
            ResetDropTargets();
          }

          // Unlock code for left spinner
          if (SpinnerStatus == 2) { // Right spinner increments left spinner unlock counter
            SpinnerLeftUnlockCounter += 1;
            Serial.println(F("Unlocking left spinner"));
            //Serial.println((Spinner_Threshold_Total*.05),DEC);
            if (SpinnerLeftUnlockCounter > (Spinner_Threshold_Total*SPINNER_LEFT_UNLOCK_FRACTION) ) {  // Right spinner turns 5% of Threshold to unlock
              SpinnerStatus = 1;
              SpinnerLighting();
              SpinnerLeftUnlockCounter = 0;
            }
          }

          // Spinner totals and goal setting
          if (SpinnerStatus != 3) {
            SpinnerCountRight[CurrentPlayer] += 1;                  // Left spinner total
            SpinnerCountTotal[CurrentPlayer] += 1;                  // Spinner total increment
            //Serial.println(F("Increment SpinnerCountRight and ...Total"));
          }
          if (SpinnerCountTotal[CurrentPlayer] > Spinner_Threshold_Total) {
            SpinnerMode[CurrentPlayer] += 1;
            SpinnerCountTotal[CurrentPlayer] = 0;
            //Serial.println(F("Zeroing spinner total"));
            PlaySoundEffect(SOUND_EFFECT_SPINNER_MODE, true);
            SetGoals(3);                                          // Spinner threshold met
            // Trigger FUTURE mode
            if (GameMode[CurrentPlayer] == 1) {
              FutureModeActive = 1;                               // Triggers mode
              Future_Mode_Start_Time = CurrentTime;
              FutureModeLetter = 3;                               // First pattern
              NewBackgroundSong(150, true);
              CallOut(CALL_OUT_FUTURE_MODE_SPELL_FUTURE, 30, (CurrentTime+3000)); // Announce start of FUTURE mode              
              Serial.println(F("Started FUTURE mode."));
            }
          }

          #ifdef EXECUTION_MESSAGES
          //Serial.print(F("SpinnerMode is:  "));
          //Serial.println(SpinnerMode[CurrentPlayer], DEC);  
          //Serial.print(F("SpinnerCountTotal is: "));
          //Serial.println(SpinnerCountTotal[CurrentPlayer], DEC);  
          #endif
          
          // Calculate spinner speed
          SpinnerFastRight = false;
          //Serial.print(F("CurrentTime - LastSpinnerRightTime is:  "));
          //Serial.println((CurrentTime - LastSpinnerRightTime), DEC);
          if ( ((CurrentTime - LastSpinnerRightTime) < SPINNER_SPEED_RIGHT_THRESHOLD) && 
               (SpinnerStatus == 1 || SpinnerStatus == 2) ){
            SpinnerFastRight = true;
            //Serial.println(F("Fast right spinner"));
          }
          LastSpinnerRightTime = CurrentTime;

          // Spinner status update
          if (SpinnerCountRight[CurrentPlayer] > ((Spinner_Threshold_Total*SPINNER_RIGHT_LOCK_FRACTION) - 1) ) {    // Locks at 20% of Total
            // Set goals Spinner Right Mode 1
            Serial.println(F("Increment Spinner Mode"));
            SpinnerStatus = 3;                    // Left spinner flashing, right off
            SpinnerLighting();
            // Play spinner goal sound, needs sound queue to delay other sounds
            SpinnerCountRight[CurrentPlayer] = 0;
            SpinnerLeftUnlockCounter = 0;
          }
           
          // Display spinner countdown
          if ( (SpinnerCountTotal[CurrentPlayer] > (Spinner_Threshold_Total*.33)) /*&& (SpinnerCount[CurrentPlayer] < Spinner_Threshold)*/ ) {
            CreditsFlashing = true;
            CreditsDispValue = (Spinner_Threshold_Total - SpinnerCountTotal[CurrentPlayer]);
          } else {
            CreditsFlashing = false;
            CreditsDispValue = Credits;
          }
           
          // Spinner scoring
          switch (SpinnerStatus) {
            case 0:   // Spinners both off
              Hundred_Pts_Stack += 1;
              break;
            case 1:   // Spinners both on
              Thousand_Pts_Stack += (SpinnerFastRight?2:1);
              break;
            case 2:   // Left spinner off, right spinner flashing
              Thousand_Pts_Stack += (SpinnerFastLeft?2:1);
              break;
            case 3:   // Left spinner flashing, right spinner off
              Ten_Pts_Stack += 1;
              break;
          }
          
          // Spinner sounds
          switch (SpinnerStatus) {
            case 0:                               // Spinners both off
              SpinnerFlips +=1;                   // Increment time sound effects accumulator
              break;
            case 1:                               // Spinners both on
            case 2:                               // Left spinner off, right spinner flashing
            if (SpinnerFastRight) {
                PlaySoundEffect(SOUND_EFFECT_SCALE_UP);
              } else {
                SpinnerFlips +=1;                 // Increment time sound effects accumulator
              }
              break;
            case 3:                               // Left spinner flashing, right spinner on
              //PlaySoundEffect(SOUND_EFFECT_SAD);
              //PlaySoundEffect(SOUND_EFFECT_NOTE_4);
              //AddToSoundQueue(SOUND_EFFECT_NOTE_1, CurrentTime+10);
              PlaySoundEffect(SOUND_EFFECT_NOTE_1);
              break;
          }

          // Mode 2 Add time
          ShootAllAddTime(900);
          //Serial.print(F("ShootAllTimeRemaining[CrtPlyr]:      "));
          //Serial.println(ShootAllTimeRemaining[CurrentPlayer], DEC);
          break;
        case SW_SLAM:
          break;
        case SW_SAUCER:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          //  Saucer Progress and debounce
          if ((CurrentTime-SaucerHitTime) > 1700) {
            SaucerHitTime = CurrentTime;
            if (GameMode[CurrentPlayer] == 1) {
              if (Kicker_Saucer_Stage == 1) {             // Reward combo shot
                SetGoals(4);                              // Award goal
                CurrentScores[CurrentPlayer] += (Kicker_Saucer_Reward/10*10);
                PlaySoundEffect(SOUND_EFFECT_EXTRA_BALL, true);
                Kicker_Saucer_Stage = 99;                 // Set display timeout
                Kicker_Saucer_Start_Time = CurrentTime;   // Set post combo display timer start
                SaucerLighting();
                // Redisplay the countdown zeroed
                OverrideScoreDisplay( ((CurrentPlayer < 2)?2:0), 0, false);                   
                OverrideScoreDisplay( ((CurrentPlayer < 2)?3:1), 0, false);
              }
              SaucerLightsTime = CurrentTime;           // Start lights counter
              SaucerProgress += 1;
              if (SaucerProgress > 3) {
                SaucerProgress = 3; 
              }
              if (SaucerProgress == 1) {                // 3K
                Thousand_Pts_Stack += 3;
                PlaySoundEffect(SOUND_EFFECT_SAUCER_3K, true);
              } else if (SaucerProgress == 2) {         // 6K
                Thousand_Pts_Stack += 6;
                PlaySoundEffect(SOUND_EFFECT_SAUCER_6K, true);
              } else if (SaucerProgress == 3) {         // 9K
                Thousand_Pts_Stack += 9;
                PlaySoundEffect(SOUND_EFFECT_SAUCER_9K, true);
              }
              // Mode 2 Start
              if (ShootAllStage[CurrentPlayer] == 1) {                          // if mode 2 is qualified
                // Shut down Kicker-Saucer Combo
                Kicker_Saucer_Stage = 0;
                Kicker_Saucer_Start_Time = 0;
                SaucerLighting();
                Serial.println(F("Kicker-Saucer shut down by SW_SAUCER"));
                
                // Shut down FUTURE mode
                FutureModeActive = 0;                                               // Turn off mode
                Future_Mode_Start_Time = 0;                                         // Reset clock
                ClearFlashingLampQueue();                                           // Clear entire queue to remove all possible patterns
                //ClearFlashingLampQueueEntry(FutureModeLetter);                      // Remove existing sequence
                FutureModeLetter = 0;                                               // Disable further rewards
                Serial.println(F("FUTURE mode shut down by SW_SAUCER"));

                //  Shut down Lit Bumper Mode
                LitBumperIncrementTime = 0;
                LitBumperModeActive = 0;
                RPU_SetLampState(LA_BUMPER_BOT_LEFT, LitBumperMode[CurrentPlayer]?1:0);// Set lamp per LitBumperMode boolean
                Serial.println(F("Lit Bumper Mode shut down by SW_SAUCER"));

                //  Shut down Green Arrows Mode and supplementary Green arrows mode
                GreenArrowsMode[CurrentPlayer]=0;                                       // Shut off mode
                GreenArrowsIncrementTime = 0;
                GreenArrowsModeActive = 0;
                GreenArrowsLighting();
                Serial.println(F("Green Arrows Mode shut down by SW_SAUCER"));

                // Reset displays and lamps
                LettersLighting(1);                                                 // Reset lamps
                OverrideScorePriority = 0;                                          // Set back to zero
                ShowPlayerScores(0xFF, false, false);                               // Reset all score displays

                // Reset spinner due to target reset
                SpinnerStatus = 0;

                // Halt background sounds
                if (DipSwitch30 == 1) {
                  RPU_PlaySoundDash51(SOUND_EFFECT_NONE_25);                   // Cancels DASH51 existing sounds
                } else if (DipSwitch30 == 3) {
                  wTrig.trackStop(CurrentBackgroundSong);                       // Stop current song
                }

                // Start mode 2
                ShootAllTimeRemaining[CurrentPlayer] = ShootAllCoinDoorValue*1000;
                Goals[CurrentPlayer] = (Goals[CurrentPlayer] | (0b1<<7));       // Raise 8th bit
                ShootAllStage[CurrentPlayer] = 2;                               // Progress to animation stages
                ShootAllIntroStartTime[CurrentPlayer] = CurrentTime;
                GameMode[CurrentPlayer] = 2;                                    // Change Game Mode
                AttractSweepLights = 0;                                         // Reset animation start point
                HaltArrowsLighting = true;                                      // Allow animations to run
                HaltSpa5KLighting = true;                                       // Allow animations to run
                HaltSPSALighting = true;                                        // Allow animations to run
                HaltSpotsLighting = true;                                       // Allow animations to run
                Serial.println(F("SW_SAUCER starting Mode 2 Stage 2, Stage 1 ended"));
                // 5 knocker hits signifying Mode 2
                RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime + 500, true);
                //RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime + 600, true);
                //RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime + 900, true);
                //RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime + 1000, true);
                //RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime + 1100, true);
                CallOut(CALL_OUT_SHOOT_ALL_SHOOT_ALL_ANNOUNCEMENT, true, CurrentTime+1000);
              }
            } else if (GameMode[CurrentPlayer] == 2) {
              if (ShootAllCompletedSegments() > SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS) { // Must be greater than value
                CallCountdownBonusFromPF = true;                                // Allows return to NGP
                return MACHINE_STATE_COUNTDOWN_BONUS;                           // Call for bonus
              } else {
                RPU_PushToTimedSolenoidStack(SOL_SAUCER, 3, CurrentTime + 1500);
              }
              if (!CaptureLetter(13)) {                                         // CaptureLetter always true
                Serial.println(F("Mode 2 Saucer lamps all captured"));
                Hundred_Pts_Stack += 5;
                PlaySoundEffect(SOUND_EFFECT_SPA);
              }
            } else if (GameMode[CurrentPlayer] == 3) {
              SaucerLightsTime = CurrentTime;           // Start lights counter
              SaucerProgress += 1;
              if (SaucerProgress > 3) {
                SaucerProgress = 3; 
              }
              if (SaucerProgress == 1) {                // 3K
                Thousand_Pts_Stack += 3;
                PlaySoundEffect(SOUND_EFFECT_SAUCER_3K, true);
              } else if (SaucerProgress == 2) {         // 6K
                Thousand_Pts_Stack += 6;
                PlaySoundEffect(SOUND_EFFECT_SAUCER_6K, true);
              } else if (SaucerProgress == 3) {         // 9K
                Thousand_Pts_Stack += 9;
                PlaySoundEffect(SOUND_EFFECT_SAUCER_9K, true);
              }
            }
          }
          break;
        case SW_DROP_FIRST:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
            if  (!SpinnerStatus) SpinnerStatus = 1;
            SpinnerLighting();
          if (GameMode[CurrentPlayer] == 1) {
            if ( (((DIP24Total+2)%3) == 0) || DipSwitch24 ) {   // +2 for drops
              CaptureLetter(99);
            }
            Hundred_Pts_Stack += 4;
            PlaySoundEffect(SOUND_EFFECT_DROP_TARGETS);
          } else if (GameMode[CurrentPlayer] == 2) {
            CaptureLetter(16);
          }
          Serial.println(F("1st Target"));
          break;
        case SW_DROP_2X:
          BonusMultiplier(2);
          if (GameMode[CurrentPlayer] == 1) {
            if ( (((DIP24Total+2)%3) == 0) || DipSwitch24 ) {   // +2 for drops
              CaptureLetter(99);
            }
            Hundred_Pts_Stack += 4;
            PlaySoundEffect(SOUND_EFFECT_DROP_TARGETS);            
          } else if (GameMode[CurrentPlayer] == 2) {
            CaptureLetter(17);
          }
          Serial.println(F("2nd Target"));
          break;
        case SW_DROP_4X:
          BonusMultiplier(4);
          if (GameMode[CurrentPlayer] == 1) {
            if ( (((DIP24Total+2)%3) == 0) || DipSwitch24 ) {   // +2 for drops
              CaptureLetter(99);
            }
            Hundred_Pts_Stack += 4;
            PlaySoundEffect(SOUND_EFFECT_DROP_TARGETS);
          } else if (GameMode[CurrentPlayer] == 2) {
            CaptureLetter(18);
          }
          Serial.println(F("3rd Target"));
          break;
        case SW_DROP_6X:
          BonusMultiplier(6);
          RPU_SetLampState(LA_DROP_BANK_EXTRA_BALL, 1);
          if (GameMode[CurrentPlayer] == 1) {
            if ( (((DIP24Total+2)%3) == 0) || DipSwitch24 ) {   // +2 for drops
              CaptureLetter(99);
            }
            Hundred_Pts_Stack += 4;
            PlaySoundEffect(SOUND_EFFECT_DROP_TARGETS);
          } else if (GameMode[CurrentPlayer] == 2) {
            CaptureLetter(19);
          }
          Serial.println(F("4th Target"));
          break;
        case SW_FUTURITY_TARGET:
          if (GameMode[CurrentPlayer] == 1) {
            if ( (((DIP24Total+2)%3) == 0) || DipSwitch24 ) {   // +2 for drops
              CaptureLetter(99);
            }
            PlaySoundEffect(SOUND_EFFECT_FUTURITY);
          } else if (GameMode[CurrentPlayer] == 2) {
            if (!CaptureLetter(20)) {  // If letter already captured, CaptureLetter is false
              PlaySoundEffect(SOUND_EFFECT_FUTURITY);
            }
            if (!SamePlayerShootsAgain) {
              SamePlayerShootsAgain = true;
            }
          }
          FuturityProgress += 1;
          if (FuturityProgress > 3) { FuturityProgress = 3; }
          if (FuturityProgress == 1) {                // Collect Extra Ball
            Hundred_Pts_Stack += 5;
            RPU_SetLampState(LA_DROP_BANK_EXTRA_BALL, 0);
            RPU_SetLampState(LA_DROP_BANK_SPECIAL, 1);
            SamePlayerShootsAgain = true;
            PlaySoundEffect(SOUND_EFFECT_EXTRA_BALL, true);
            GreenArrowsMode[CurrentPlayer]=true;                                  // Turn on mode
            GreenArrowsModeActive += 2;                                           // Increment for each hit
            if (GreenArrowsModeActive > 5) GreenArrowsModeActive = 5;             // Limit to 5
            GreenArrowsIncrementTime = CurrentTime;                               // Set timer
            GreenArrowsCountdown = GreenArrowsCountdownLength();                  // Set countdown length
            GreenArrowsLighting();
          } else if (FuturityProgress == 2) {         // Collect Special
            Hundred_Pts_Stack += 5;
            Thousand_Pts_Stack += 50;
            RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime+1700, false);
            RPU_SetLampState(LA_DROP_BANK_SPECIAL, 0);
            if (!(GameMode[CurrentPlayer] == 2)) {    // Do not light in mode 2
              RPU_SetLampState(LA_DROP_BANK_20K, 1);              
            }
            GreenArrowsMode[CurrentPlayer]=true;                                  // Turn on mode
            GreenArrowsModeActive += 2;                                           // Increment for each hit
            if (GreenArrowsModeActive > 5) GreenArrowsModeActive = 5;             // Limit to 5
            GreenArrowsIncrementTime = CurrentTime;                               // Set timer
            GreenArrowsCountdown = GreenArrowsCountdownLength();                  // Set countdown length
            GreenArrowsLighting();
          } else {                                    // if (FuturityProgress == 3) - Collect 20K
            Thousand_Pts_Stack += 20;
            GreenArrowsMode[CurrentPlayer]=true;                                  // Turn on mode
            GreenArrowsModeActive += 3;                                           // Increment for each hit
            if (GreenArrowsModeActive > 5) GreenArrowsModeActive = 5;             // Limit to 5
            GreenArrowsIncrementTime = CurrentTime;                               // Set timer
            GreenArrowsCountdown = GreenArrowsCountdownLength();                  // Set countdown length
            GreenArrowsLighting();
          }
          break;
        case SW_LEFT_INLANE:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          Hundred_Pts_Stack += 5;
          PlaySoundEffect(SOUND_EFFECT_INLANE);
          break;
        case SW_GATE_LANE:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (!GateOpen) {
            Thousand_Pts_Stack += 3;
            //RPU_PlaySoundDash51(30);
            PlaySoundEffect(SOUND_EFFECT_INLANE);
            GateOpen = true;
            GateOpenTime = CurrentTime;
          } else {
            Hundred_Pts_Stack += 5;
            //RPU_PlaySoundDash51(8);
            PlaySoundEffect(SOUND_EFFECT_OUTLANE);
          }
          break;
        case SW_LEFT_OUTLANE:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          Hundred_Pts_Stack += 5;
          PlaySoundEffect(SOUND_EFFECT_OUTLANE);
          //RPU_PlaySoundDash51(8);
          break;
        case SW_LEFT_SLING:
          if (BallFirstSwitchHitTime == 0) BallFirstSwitchHitTime = CurrentTime;
          if (DropTargetsToBeReset) {
            ResetDropTargets();
          }
          CurrentScores[CurrentPlayer] += 10;
          DIP24Total +=1;
          //RPU_PlaySoundDash51(2);
          PlaySoundEffect(SOUND_EFFECT_SPINNER_1000_PT);
          break;
        case SW_COIN_1:
        case SW_COIN_2:
        case SW_COIN_3:
          AddCoinToAudit(switchHit);
          AddCredit(true);
          CreditsDispValue = Credits;
          //RPU_SetDisplayCredits(Credits, true);
          if (Credits > 0) {
            RPU_SetLampState(LA_CREDIT_INDICATOR, 1);
          }
          break;
        case SW_CREDIT_RESET:
          if (CurrentBallInPlay<2) {
            // If we haven't finished the first ball, we can add players
            AddPlayer();
            if (Credits == 0) {
              RPU_SetLampState(LA_CREDIT_INDICATOR, 0);
            }
          } else {
            // If the first ball is over, pressing start again resets the game
            if (Credits >= 1 || FreePlayMode) {
              if (!FreePlayMode) {
                Credits -= 1;
                RPU_WriteByteToEEProm(RPU_CREDITS_EEPROM_BYTE, Credits);
                CreditsDispValue = Credits;
                //RPU_SetDisplayCredits(Credits);
              }
              returnState = MACHINE_STATE_INIT_GAMEPLAY;
            }
          }
          if (DEBUG_MESSAGES) {
            Serial.write("Start game button pressed\n\r");
          }
          break;        
      }
     }
    }  // -A-

  if (scoreAtTop != CurrentScores[CurrentPlayer]) {
//    Serial.print(F("Score changed\n"));
    LastTimeScoreChanged = CurrentTime;                 // To control Score dashing 
    for (int awardCount = 0; awardCount < 3; awardCount++) {
      if (AwardScores[awardCount] != 0 && scoreAtTop < AwardScores[awardCount] && CurrentScores[CurrentPlayer] >= AwardScores[awardCount]) {
        // Player has just passed an award score, so we need to award it
        SamePlayerShootsAgain = true;
        //RPU_SetLampState(LA_SAME_PLAYER, 1);
        //RPU_SetLampState(LA_SPSA_BACK, 1);        
//          PlaySoundEffect(SOUND_EFFECT_EXTRA_BALL);
      }
    }
  }
  
  return returnState;
}


void loop() {
  // This line has to be in the main loop
  RPU_DataRead(0);

  PreviousCurrentTime = CurrentTime;                // Retain previous CurrentTime value
  CurrentTime = millis();
  int newMachineState = MachineState;

  // Machine state is self-test/attract/game play
  if (MachineState<0) {
    newMachineState = RunSelfTest(MachineState, MachineStateChanged);
  } else if (MachineState==MACHINE_STATE_ATTRACT) {
    newMachineState = RunAttractMode(MachineState, MachineStateChanged);
  } else {
    newMachineState = RunGamePlayMode(MachineState, MachineStateChanged);
  }

  if (newMachineState!=MachineState) {
    MachineState = newMachineState;
    MachineStateChanged = true;
  } else {
    MachineStateChanged = false;
  }

  // Sound effects
  //  Allows processing of queued songs, callouts, and sounds

  unsigned int curSound = GetFromSoundQueue(CurrentTime);
  if (curSound!=0xFF) {
    if (DipSwitch30 > 1) {                                      // If WavTrigger selected
      if ( (curSound/100) >= CALL_OUT_TRACK_NUMBER_OFFSET) {    // Strip off priority and check 
        unsigned int calloutnumber = curSound/100;              // Separate out call out number
        unsigned int calloutpriority = curSound - (calloutnumber*100); // Separate out priority number
        if (calloutnumber%2) {                                  // If odd, this is a call out end request
          CalloutEnd(calloutnumber, calloutpriority);           // if curSound was odd, this means stop a call out
        } else {
          CalloutStart(calloutnumber, calloutpriority);         // if even start requested call out
          Serial.print(F("loop() calling Callout number: "));
          Serial.println(calloutnumber, DEC);
        }
      } else if (curSound >= SPECIFIC_SONG_NUMBER_OFFSET) {     // Specific song call
        Serial.print(F("loop() calling Specific song number: "));
        Serial.println(curSound, DEC);
        NewBackgroundSong(curSound, true);
      } else {                                                  // else this is regular sound effect
        wTrig.trackStop(CurrentSoundEffect);                    // Halt previous sound
        CurrentSoundEffect = curSound;
        //Serial.print(F("Play sound effect: "));
        wTrig.trackPlayPoly(curSound);                          // Play sound effect
        //Serial.print(F("SFxVolume is: "));
        //Serial.println(SFxVolume, DEC);
        wTrig.trackGain(curSound, (SFxVolume - ((CalloutActive)?SFX_VOLUME_DROP_FOR_CALL_OUT:0)) ); // Preset Track 1 gain to X dB
      }
    } else {                                                    // Else use DASH51
      if (curSound < 32) {                                      // Limit to DASH51 range
        RPU_PlaySoundDash51(curSound);
      }
    }
  }

  RPU_ApplyFlashToLamps(CurrentTime);
  RPU_UpdateTimedSolenoidStack(CurrentTime);

  // Toggle Credits and BIP displays when needed
  if (MachineState >= 0) {
    RPU_SetDisplayCredits(CreditsDispValue, (CreditsFlashing)?((1-(CurrentTime/125)%2)):1, true);
    RPU_SetDisplayBallInPlay(BIPDispValue, (BIPFlashing)?((CurrentTime/125)%2):1, true);
  }

}

//
// **************************
// *  Future Spa functions  *
// **************************
//

// Compute total countdown length each time GreenArrows stage is changed.

unsigned long GreenArrowsCountdownLength() {
  unsigned long TotalTime = 0;
  for (int count = 1; count < (GreenArrowsModeActive + 1); count++) {
    TotalTime += (GreenArrowsBaseTime - LitGreenArrowsTable[count].countdownTime);
  }
  TotalTime = TotalTime*1000;                           // Convert to msec
  Serial.print(F("GreenArrowsCountdownLength is: "));
  Serial.println(TotalTime, DEC);
  return TotalTime;
}

void ShowSoundQueueEntries() {
  Serial.println(F("SoundQueue Entries:      "));
  for (int count = 0; count < 30; count++) {
    Serial.print(SoundQueue[count].soundByte, DEC);
    Serial.print(F("  "));
    Serial.print(SoundQueue[count].playTime, DEC);
    Serial.print(F("  "));
    Serial.print(SoundQueue[count].priority, DEC);
    Serial.println();
  }
}

void ShowFlashingLampQueueEntries() {
  Serial.println(F("FlashingLampQueue Entries:      "));
  for (int count = 0; count < FLASHING_LAMP_QUEUE_SIZE; count++) {
    Serial.print(FlashingLampQueue[count].lampNumber, DEC);
    Serial.print(F("  "));
    Serial.print(FlashingLampQueue[count].currentState, DEC);
    Serial.print(F("  "));
    Serial.print(FlashingLampQueue[count].finishState, DEC);
    Serial.print(F("  "));
    Serial.print(FlashingLampQueue[count].startTime, DEC);
    Serial.print(F("  "));
    Serial.print(FlashingLampQueue[count].nextTime, DEC);
    Serial.print(F("  "));
    Serial.print(FlashingLampQueue[count].duration, DEC);
    Serial.print(F("  "));
    Serial.print(FlashingLampQueue[count].startPeriod, DEC);
    Serial.print(F("  "));
    Serial.print(FlashingLampQueue[count].endPeriod, DEC);
    Serial.println();
  }
}



byte ShootAllCompletedSegments() {
  byte NumCompletedSegments = 0;
  if (ShootAllFutureComplete[CurrentPlayer]) NumCompletedSegments += 1;
  if (ShootAllKickerComplete[CurrentPlayer]) NumCompletedSegments += 1;
  if (ShootAllRollOverComplete[CurrentPlayer]) NumCompletedSegments += 1;
  if (ShootAllSaucerComplete[CurrentPlayer]) NumCompletedSegments += 1;
  //Serial.print(F("NumCompletedSegments is: "));
  //Serial.println(NumCompletedSegments);
  return NumCompletedSegments;
}

// NewBackgroundsong Ver 2 - add priority function zzzzz

byte NewBackgroundSong(byte specificTrack=255, boolean priority=false) { // Handles stop-start of background songs
  if (DipSwitch30 < 2) return;                              // Not using WavTrigger
  if ((CurrentBackgroundSong != 255) && (specificTrack == CurrentBackgroundSong)) {
    Serial.println(F("Skipping request to play song already playing"));
    return;
  }
  if ( ((CurrentTime - AllowNewBackgroundSongTime) > MINIMUM_BACKGROUND_SONG_PLAY_TIME) || (priority) ) {
    AllowNewBackgroundSongTime = CurrentTime;

    if (CurrentBackgroundSong != 255) {                     // Skip if nothing playing
      // Fade current song down to x dB over X secs and stop    
      wTrig.trackFade(CurrentBackgroundSong, (MusicVolume-SONG_START_END_VOLUME), SONG_CROSS_FADE_TIME, true);
    }
    //Serial.println(F("Call SongSelector"));
    if ( (specificTrack == 254) || (specificTrack == 255) ) {
    //if (specificTrack >= 254) {
      NextBackgroundSong = SongSelector();                  // Choose random song
      SongList();
    } else {
      NextBackgroundSong = specificTrack;                   // Play this track
    }
    Serial.print(F("Song to be played is: "));
    Serial.println(NextBackgroundSong);
    //Serial.print(F("Song volume is: "));
    //Serial.println((MusicVolume), DEC);  //

    wTrig.trackPlayPoly(NextBackgroundSong);                                              // Start next song at low volume
    if (CalloutActive) {
      wTrig.trackGain(NextBackgroundSong, (MusicVolume-SONG_VOLUME_DROP_FOR_CALL_OUT));   // Set low volume for call out
      //Serial.print(F("Song volume set low with call out active"));
    } else {
      wTrig.trackGain(NextBackgroundSong, MusicVolume);                                   // Set next song to correct volume
      //wTrig.trackGain(NextBackgroundSong, (MusicVolume-SONG_START_END_VOLUME));           // Preset next song gain to -XdB
      //wTrig.trackFade(NextBackgroundSong, MusicVolume, SONG_CROSS_FADE_TIME, false);      // Fade track up to Xdb over X secs
      //Serial.print(F("Song volume set to fade up"));
    }
    wTrig.trackLoop(NextBackgroundSong, 1);                                               // Loop song
    CurrentBackgroundSong = NextBackgroundSong;
  } else {
    Serial.println(F("Skipping song request skipped, too soon"));
  }
}


int SongSelector() {
  int NextSong = CurrentTime%((NumberOfJukeBoxSongsPlusOne - 1) - BackgroundSongSelect[NumberOfJukeBoxSongsPlusOne - 1]);
  int returnVal = BackgroundSongSelect[NextSong];
  Serial.print(F("SongSelector - NextSong number is: "));
  //Serial.print(NextSong, DEC);
  //Serial.print(F("    song selected is:  "));
  Serial.println(BackgroundSongSelect[NextSong], DEC);
  BackgroundSongSelect[NumberOfJukeBoxSongsPlusOne - 1] +=1;         // Increment number of songs played
  // Condense list of remaining songs to be played.
  for (int i=NextSong; i < (NumberOfJukeBoxSongsPlusOne - 2); i++) { // Start from picked song until 1 song from the end
    BackgroundSongSelect[i] = BackgroundSongSelect[i + 1];
  }
  if (BackgroundSongSelect[NumberOfJukeBoxSongsPlusOne - 1] == (NumberOfJukeBoxSongsPlusOne - 1) ) {
    CreateSongList();
  }
  return returnVal;
}

void CreateSongList() {                                                  // Create starting list
  Serial.println(F("****Resetting song list elements****"));
  for (int i=0; i < (NumberOfJukeBoxSongsPlusOne -1); i++) {    // Set starting list of songs
    BackgroundSongSelect[i] = (i + BACKGROUND_SONG_TRACK_OFFSET);
  }
  BackgroundSongSelect[(NumberOfJukeBoxSongsPlusOne -1)] = 0;   // No songs played yet
}


void SongList() {
  Serial.println(F("Song list elements"));
  
  for (int i=0; i < (NumberOfJukeBoxSongsPlusOne); i++) {      // Print out all elements
    //Serial.println(BackgroundSongSelect[i], DEC);
  }
  //Serial.println(F("BackgroundSongSelect[0], DEC"));
  //Serial.println(BackgroundSongSelect[1], DEC);
  //Serial.println(BackgroundSongSelect[9], DEC);
  Serial.println();
}


void RoamingFinish() {
  OverrideScorePriority = 0;
  ShowPlayerScores(0xFF, false, false);                 // Reset all score displays
  RoamingStage = 0;                                     // Halts activity
  RoamingModeTime = 0;
  NumCapturedLetters = 0;
  ReverseRoam = false;
  GameMode[CurrentPlayer] = 1;                          // Return to normal play
  LettersLighting(1);
}

// Relight all playfield lamps

void RestorePlayfieldLamps() {
  BonusMultiplierLighting(BonusMult);
  SpinnerLighting();
  GreenArrowsLighting();
  GateArrowsLighting();
  if (GameMode[CurrentPlayer] != 2) {  // Mode 2 sets these under LettersLighting
    BankShotLighting();
    KickerShotLighting();
    SaucerLighting();
    // Placeholder for target lamps
    // Placeholder for Spots Letter lamps
    LettersLighting(1);    
  }
}


// SweepAnimation
//  animationName[]  = struct defining lamps by animation frame
//  numLamps         = Number of lamps in animation
//  totalSteps       = How many steps to take before repeating
//  stepTime         = msec frame length
//  activeRows       = How many lamp frames are lit at any time

void SweepAnimation(struct AttractLampsDown animationName[], byte numLamps, byte totalSteps, 
                                            unsigned long stepTime, byte activeRows) {
  if ((CurrentTime - AttractSweepTime) > stepTime) {                          // Animation frame length
    AttractSweepTime = CurrentTime;
    for (byte lightcountdown=0; lightcountdown < numLamps; lightcountdown++) {
      byte dist = AttractSweepLights - animationName[lightcountdown].rowDown;
      RPU_SetLampState(animationName[lightcountdown].lightNumDown, (dist < activeRows), 0);
    }
    AttractSweepLights += 1;
    if (AttractSweepLights > totalSteps) AttractSweepLights = 0;
  }
}

void ShootAllFinish() {
  ShootAllClockStopped[CurrentPlayer] = true;                                 // Stop the clock
  ShootAllTimeRemaining[CurrentPlayer] = 0;                                   // Reset to actual 0
  ShootAllWipeTimer();
  ShootAllStage[CurrentPlayer] = 0;                                           // Reset for next time
  DropTargetsToBeReset = true;                                                // Reset at earliest opportunity
}

void ResetDropTargets() {
  RPU_PushToTimedSolenoidStack(SOL_DROP_RESET, 15, CurrentTime + 150);
  SpinnerStatus = 0;
  BonusMultiplier(1);
  DropTargetsToBeReset = false;
}

void ShootAllWipeLamps() {
  for (int i = 0; i < 26; i++) {                               // Wipe ShootAll lamps
    RPU_SetLampState(ShootAllLamps[i], 0);
  }
}

void ShootAllWipeTimer() {
  for (int i = 0; i < 12; i++) {                               // Wipe timer lamps
    RPU_SetLampState(ShootAllTimerLamps[i], 0);
  }
}

//
// Amounts awarded by switches during ShootAll
//
//  SW_30         1500 msec
//  Bumpers        850
//  Left spinner   700
//  Right spinner  900

// Scaling of added time, 100% prior to 1st bonus, then 50% etc.
byte AddTimeScaling[4] = {10, 8, 6, 3};                         // % of request allowed

void ShootAllAddTime(unsigned long topupamt) {
  //if (!(MachineState == MACHINE_STATE_COUNTDOWN_BONUS)) {       // do not scale during CountdownBonus
    topupamt = topupamt*AddTimeScaling[ (NumBonusesfromPF[CurrentPlayer] > 3)?3:(NumBonusesfromPF[CurrentPlayer]) ]/10;  // Scale request
  //}
  //Serial.print(F("ShootAllAddTime top up amount: "));
  //Serial.println(topupamt, DEC);
  if ( (GameMode[CurrentPlayer] == 2) && !ShootAllClockStopped[CurrentPlayer]) {
    //ShootAllDuration[CurrentPlayer] += topupamt;
    /*if ((ShootAllStartTime[CurrentPlayer] + ShootAllDuration[CurrentPlayer] - CurrentTime) > SHOOTALL_MAX_DURATION) {
      ShootAllDuration[CurrentPlayer] = CurrentTime - ShootAllStartTime[CurrentPlayer] + SHOOTALL_MAX_DURATION;
    }*/
    ShootAllTimeRemaining[CurrentPlayer] += topupamt;
    if (ShootAllTimeRemaining[CurrentPlayer] > SHOOTALL_MAX_DURATION) {
      ShootAllTimeRemaining[CurrentPlayer] = SHOOTALL_MAX_DURATION;
    }
    ShootAllTimeAdded[CurrentPlayer] += topupamt;              // Track total added by player
    TimerToggle = false;                                       // Initial flash is immediately changed
    TimerFlashPeriod = 0;                                      // Forces immediate update
  }
}

boolean ClearFlashingLampQueueEntry(byte clearSingleEntry) {
  for (int count=0; count<FLASHING_LAMP_QUEUE_SIZE; count++) {
    if (FlashingLampQueue[count].lampNumber == clearSingleEntry) {
      RPU_SetLampState(FlashingLampQueue[count].lampNumber, FlashingLampQueue[count].finishState);
      FlashingLampQueue[count].lampNumber = 0xFF;
      Serial.println(F("FlashingLampQueue single entry cleared"));
      return true;
    }
  }
  return false;
}

//
//  Clear flashing lamp queue - Ver 1
//    If emptyAndSetLamp is true, lamps will be set to requested end state when cleared from queue
//    if emptyAndSetLamp is false, lamp is cleared from queue, lamp is left in current state
//
void ClearFlashingLampQueue(boolean emptyAndSetLamp) {
  for (int count=0; count<FLASHING_LAMP_QUEUE_SIZE; count++) {
    if ((FlashingLampQueue[count].lampNumber != 0xFF) && emptyAndSetLamp) {
      RPU_SetLampState(FlashingLampQueue[count].lampNumber, FlashingLampQueue[count].finishState);
    }
    FlashingLampQueue[count].lampNumber = 0xFF;
  }
  Serial.println(F("FlashingLampQueue cleared"));
  FlashingLampQueueEmpty = true;
}


boolean AddToFlashingLampQueue(byte lampNumber, byte finishState, unsigned long startTime, 
                 unsigned long duration, unsigned long startPeriod, unsigned long endPeriod) {
  for (int count=0; count<FLASHING_LAMP_QUEUE_SIZE; count++) {    // Check for existing entry
    if (FlashingLampQueue[count].lampNumber == lampNumber) {
      FlashingLampQueue[count].lampNumber = 0xFF;                 // Remove from queue
      Serial.println(F("FlashingLampQueue duplicate removed"));
    }
  }
  for (int count=0; count<FLASHING_LAMP_QUEUE_SIZE; count++) {
    if (FlashingLampQueue[count].lampNumber == 0xFF) {
      FlashingLampQueue[count].lampNumber = lampNumber;
      FlashingLampQueue[count].currentState = 1;                  // Start sequence with lamp on
      FlashingLampQueue[count].finishState = finishState;
      FlashingLampQueue[count].startTime = startTime;             // 1st processing will turn lamp to current state
      FlashingLampQueue[count].nextTime = startTime;              // Next incremental lamp transition
      if (duration < 100) duration = 100;                         // Prevent overly short events
      FlashingLampQueue[count].duration = duration;
      FlashingLampQueue[count].startPeriod = startPeriod/2;       // Set to half period
      FlashingLampQueue[count].endPeriod = endPeriod/2;           // Set to half period
      FlashingLampQueueEmpty = false;
      Serial.println(F("FlashingLampQueue entry added"));

      return true;
    }
  }
  Serial.println(F("FlashingLampQueue is full"));
  return false;
}

void ProcessFlashingLampQueue(unsigned long pullTime) {
  for (int count=0; count<FLASHING_LAMP_QUEUE_SIZE; count++) {
    if (FlashingLampQueue[count].lampNumber!=0xFF && FlashingLampQueue[count].nextTime < pullTime) {
      unsigned long nextdelta = 0;
      if (FlashingLampQueue[count].startPeriod > FlashingLampQueue[count].endPeriod) {
        nextdelta = FlashingLampQueue[count].startPeriod -
          (( (pullTime - FlashingLampQueue[count].startTime) * 
          (FlashingLampQueue[count].startPeriod - FlashingLampQueue[count].endPeriod) ) / 
          FlashingLampQueue[count].duration);
        //Serial.print(F("Flashing lamp queue nextdelta is: "));
        //Serial.println(nextdelta, DEC);
        if (nextdelta < FlashingLampQueue[count].endPeriod)  {      // trigger for ending an event
          RPU_SetLampState(FlashingLampQueue[count].lampNumber, FlashingLampQueue[count].finishState);
          FlashingLampQueue[count].lampNumber = 0xFF;
          Serial.println(F("nextdelta < endPeriod, set lamp to final state"));
          return;
        }
      } else {
        nextdelta = FlashingLampQueue[count].startPeriod +
          (( (pullTime - FlashingLampQueue[count].startTime) * 
          (FlashingLampQueue[count].endPeriod - FlashingLampQueue[count].startPeriod) ) / 
          FlashingLampQueue[count].duration);
        //Serial.print(F("Flashing lamp queue nextdelta is: "));
        //Serial.println(nextdelta, DEC);
        if (nextdelta > FlashingLampQueue[count].endPeriod)  {      // trigger for ending an event
          RPU_SetLampState(FlashingLampQueue[count].lampNumber, FlashingLampQueue[count].finishState);
          FlashingLampQueue[count].lampNumber = 0xFF;
          Serial.println(F("nextdelta < endPeriod so set lamp to final state"));
          return;
        }
      }

      // Transition lamp and determine next transition time
      RPU_SetLampState(FlashingLampQueue[count].lampNumber,FlashingLampQueue[count].currentState);
      FlashingLampQueue[count].currentState = !FlashingLampQueue[count].currentState;
      //FlashingLampQueue[count].startTime = FlashingLampQueue[count].startTime + nextdelta;
      FlashingLampQueue[count].nextTime = FlashingLampQueue[count].nextTime + nextdelta;
      //Serial.println(F("Flashing lamp queue transitioned a lamp"));
    }
  }
  FlashingLampQueueEmpty = true;
  //Serial.println(F("ProcessFlashingLampQueue found queue is empty"));
  return;
}


// Light Spinner lamps per status byte
void SpinnerLighting() {
  switch (SpinnerStatus) {
    case 0:   // Spinners both off
      RPU_SetLampState(LA_SPINNER_RIGHT, 0);
      RPU_SetLampState(LA_SPINNER_LEFT, 0);
      break;
    case 1:   // Spinners both on
      RPU_SetLampState(LA_SPINNER_RIGHT, 1);
      RPU_SetLampState(LA_SPINNER_LEFT, 1);
      break;
    case 2:   // Left spinner off, right spinner flashing
      RPU_SetLampState(LA_SPINNER_RIGHT, 1, 0, 100);
      RPU_SetLampState(LA_SPINNER_LEFT, 0);
      break;
    case 3:   // Left spinner flashing, right spinner off
      RPU_SetLampState(LA_SPINNER_RIGHT, 0);
      RPU_SetLampState(LA_SPINNER_LEFT, 1, 0, 100);
      break;
  }
}

//
//  GoalsDisplayValue creates decimal value representing collected goals for display
//
unsigned long GoalsDisplayValue(byte currentgoals) {
  unsigned long Result = 0;
  for(int i=0; i<6; i++) {                     // Filter lower 6 goals
    Result = Result * 10;
    if ( Goals[CurrentPlayer] & (0b100000 >> i)) {
      Result +=1;
    }
    //Serial.print(F("Result is: "));
    //Serial.println(Result, DEC);
  }
  return Result;
}


//
//  SetGoals Ver 1
//
// Bit 1 - 36k Bonus reached
// Bit 2 - Pop Mode > x times
// Bit 3 - Spinner Total reached
// Bit 4 - Kicker-Saucer scored x times
// Bit 5 - Lit Bumper scores x times
// Bit 6 - FUTURE mode scored x times
// Bit 7 - TBD x Goals achieved
// Bit 8 - ShootAll x Goals achieved

void SetGoals(byte goalnum) {   // Set goal flag and update display score

  Goals[CurrentPlayer] = (Goals[CurrentPlayer] | (0b1<<(goalnum - 1))); // Raise flag

  // Count how many goals are met and update display
  unsigned int countOnes = Goals[CurrentPlayer];
  byte numOnes = 0;
  for (int count = 0; count < 6; count++) {
    if ( (countOnes & 0b1) == 1 ) {
      numOnes++;
    }
    countOnes >>= 1;
    //Serial.print(F("numOnes :  "));
    //Serial.println(numOnes);
  }
  #ifdef EXECUTION_MESSAGES
  Serial.print(F("SetGoals - This many goals met: "));
  Serial.println(numOnes, DEC);
  #endif
  CurrentScores[CurrentPlayer] = (CurrentScores[CurrentPlayer]/10*10 + numOnes);

#if 1
  //
  // ShootAll Mode 2 start
  //
  if ( numOnes == 5 && !(Goals[CurrentPlayer] & (0b1<<7)) ) {       // Enable ShootAll
    #ifdef EXECUTION_MESSAGES
    Serial.println(F("SetGoals - Qualifying ShootAll, set to Stage 1"));
    #endif
    ShootAllStage[CurrentPlayer] = 1;                               // Mode is qualified, ready to start
  }
#endif

#if 1
  //
  // Roaming Mode 3 start
  //
  if ( numOnes == 3 && !(Goals[CurrentPlayer] & (0b1<<7)) ) {       // Enable Roaming
    #ifdef EXECUTION_MESSAGES
    Serial.println(F("SetGoals - Starting Mode 3 Roaming"));
    #endif
    GameMode[CurrentPlayer] = 3;                                    // Switch to Roaming mode
    RoamingLetters = 0;                                             // Clear values when mode starts
    RoamingStage = 1;                                               // Start mode activity
    CallOut(CALL_OUT_ROAMING_GET_ALL_NINE, 30, (CurrentTime+2500));
    NewBackgroundSong(160, true);
    RoamingRotateTime = CurrentTime;
    RoamingModeTime = CurrentTime;
  }
#endif


} // End of SetGoals


//
//  CaptureLetter Ver 3 - add Mode 3
//  CaptureLetter Ver 2 - Modes 1 and 2
//
//  Call 0-2 for first available SPA letter
//  Call 3-8 for specific FUTURE letter
//  Call with 99 captures the next available Letter
//

boolean CaptureLetter(byte letterswitchnum) {            // Return of true means a letter was captured
  boolean returnVal = false;

  #ifdef EXECUTION_MESSAGES
  Serial.print(F("CaptureLetter - GameMode Crt Plyr is: "));
  Serial.println(GameMode[CurrentPlayer], DEC);
  #endif
  if (GameMode[CurrentPlayer] == 1) {                  // Normal game play
  
    // If not all SPA letters are collected, the next letter will be collected
    if ( (letterswitchnum < 3) || (letterswitchnum == 99) ) {
      for(int i=0; i<3; i++) {
        Serial.print(F("Capture SPA letter loop, letterswitchnum is: "));
        Serial.println(letterswitchnum, DEC);
        if ( (Letters[CurrentPlayer] & (0b1<<(0+i))) == false ) {             // If flag is 0
          Letters[CurrentPlayer] = ( Letters[CurrentPlayer] | (0b1<<(0+i)) ); // then raise flag
          RPU_SetLampState((LA_SPA_S+i), 1, 0, 75);
          MarqueeTiming();
          if (letterswitchnum != 99) {
            PlaySoundEffect(SOUND_EFFECT_SPA);
          }
          Hundred_Pts_Stack += 5;
          Serial.println(F("Captured SPA letter"));
          returnVal = true;                                // Letter was captured
          break;
        }
      }
    }
    // For switchnum from 3-8 FUTURE letters
    if ((letterswitchnum > 2) && (letterswitchnum < 9)) {
      Serial.print(F("Capturing FUTURE letter, letterswitchnum is: "));
      Serial.println(letterswitchnum, DEC);
      if ( (Letters[CurrentPlayer] & (0b1<<letterswitchnum)) == false ) {             // If flag is 0
        Letters[CurrentPlayer] = (Letters[CurrentPlayer] | (0b1<<letterswitchnum) );  // then raise flag
        RPU_SetLampState((LA_SPA_S + letterswitchnum), 1, 0, 75);
        MarqueeTiming();
        // Don't play sound if FUTUREMode award about to be collected.
        if (!((FutureModeActive) && (FutureModeLetter == letterswitchnum))) {
          PlaySoundEffect(SOUND_EFFECT_SPA);
        }
        Hundred_Pts_Stack += 5;
        Serial.println(F("Captured FUTURE letter"));
        returnVal = true;                                  // Letter was captured
      }
    }
    // Select next available FUTURE letter as needed by Kicker and Bankshot after SPA letters are collected
    if ( (letterswitchnum == 99) && returnVal == false) {  // Only checks if no SPA letter was available
      for(int i=0; i<6; i++) {
        Serial.println(F("All SPA letters captured, select next FUTURE letter"));
        if ( (Letters[CurrentPlayer] & (0b1<<(3+i))) == false ) {             // If flag is 0
          Letters[CurrentPlayer] = ( Letters[CurrentPlayer] | (0b1<<(3+i)) ); // then raise flag
          RPU_SetLampState((LA_FUTURE_F+i), 1, 0, 75);
          MarqueeTiming();
          // Sound never needed if 99 since Kicker or BankShot play their own sound
          // if (letterswitchnum != 99) {
          //   RPU_PlaySoundDash51(SOUND_EFFECT_SPA);                        // Letter capture sound
          // }
          Hundred_Pts_Stack += 5;
          Serial.println(F("Captured next available FUTURE letter: "));
          returnVal = true;                                //  The next letter was captured
          break;
        }
      }
    }

    // FUTURE mode combo, award score if player hits correct lane

    if ( FutureModeActive && (letterswitchnum > 2) && (letterswitchnum < 9) ) { // Mode active and FUTURE lane hit
      if (FUTUREModeSwitchCombinations[FutureModeLetter - 3][letterswitchnum - 3]) { // If this switch is part of the current pattern
      //if (FutureModeLetter == letterswitchnum) {                                // If this is the current letter award score
        SetGoals(6);                                                            // Award goal
        //ClearFlashingLampQueueEntry(letterswitchnum);                           // Remove existing sequence
        for (int count = 3; count < 9; count++) {
          ClearFlashingLampQueueEntry(count);                                   // Remove any flasing FUTURE lane lamps
        }
        LettersLighting(1);                                                     // Reset captured lamp
        switch (FutureModeLetter) {
          case 3:                                                               // Pattern 1
            NewBackgroundSong(151, true);                                       // Start new song
            if (DipSwitch30 > 1) {                                              // if using WavTrigger
              CallOut(CALL_OUT_FUTURE_MODE_CARMEN_UP_1);
            } else {
              PlaySoundEffect(SOUND_EFFECT_CARMEN_UP);
            }
            break;
          case 4:                                                               // Pattern 2
            if (DipSwitch30 > 1) {                                              // if using WavTrigger
              CallOut(CALL_OUT_FUTURE_MODE_CARMEN_UP_2);
            } else {
              PlaySoundEffect(SOUND_EFFECT_CARMEN_DOWN);
            }
            break;
          case 5:                                                               // Pattern 3
            if (DipSwitch30 > 1) {                                              // if using WavTrigger
              CallOut(CALL_OUT_FUTURE_MODE_CARMEN_UP_1);
            } else {
              PlaySoundEffect(SOUND_EFFECT_CARMEN_UP);
            }
            break;
          case 6:                                                               // Pattern 4
            NewBackgroundSong(152, true);                                       // Start new song
            if (DipSwitch30 > 1) {                                              // if using WavTrigger
              CallOut(CALL_OUT_FUTURE_MODE_CARMEN_UP_2);
              CallOut(CALL_OUT_FUTURE_MODE_FU, 30, (CurrentTime+2600));
            } else {
              PlaySoundEffect(SOUND_EFFECT_CARMEN_DOWN);
            }
            break;
          case 7:                                                               // Pattern 5
            if (DipSwitch30 > 1) {                                              // if using WavTrigger
              CallOut(CALL_OUT_FUTURE_MODE_CARMEN_UP_1);
              CallOut(CALL_OUT_FUTURE_MODE_REALLY_CLOSE, 30, (CurrentTime+2200));
            } else {
              PlaySoundEffect(SOUND_EFFECT_CARMEN_UP);
            }
            break;
          case 8:                                                               // Pattern 6
            // Halting music allows final wrap up sound to be a sound effect, not a call out like
            // above cases.  
            // This leaves room for a call out if needed.
            wTrig.trackStop(CurrentBackgroundSong);
            if (DipSwitch30 > 1) {                                              // if using WavTrigger
              CallOut(CALL_OUT_FUTURE_MODE_COMPLETED, 70, (CurrentTime+5300));
            }
            PlaySoundEffect(SOUND_EFFECT_CARMEN_END);
            break;
        }
        Thousand_Pts_Stack += 10*(FutureModeLetter-2);                          // 10k, 20k, maxes at 60k
        Future_Mode_Start_Time = CurrentTime;                                   // Reset the clock
        FutureModeActive = 1;                                                   // Reset to 1st stage of mode
        FutureModeLetter += 1;                                                  // Increment letter
        if (FutureModeLetter > 8) {                                             // Player has completed the whole sequence
          FutureModeActive = 3;                                                 // Sequence end stage
          FutureModeLetter = 0;                                                 // Disable further rewards
          //PlaySoundEffect(SOUND_EFFECT_SCALE_UP, true);
          //PlaySoundEffect(SOUND_EFFECT_SCALE_UP, true);
        }
      }
    }


  } else if (GameMode[CurrentPlayer] == 2) {                                    // Mode 2 - Shoot All

#if 0
  ShootAll mode sequence
      Seq#  Lamp  Label      Switch
       0     0      S          22
       1     1      P          22
       2     2      A          22
       3     3      F          21
       4     4      U          20
       5     5      T          19
       6     6      U          18
       7     7      R          17
       8     8      E          16
       9    24   Kicker 3K     27
      10    25   Kicker 5K     27
      11    26   Kicker 7K     27
      12    27   Kicker 9K     27
      13    28   Saucer 3K     23
      14    29   Saucer 6K     23
      15    30   Saucer 9K     23
      16    36   Bank 20K       0
      17    36   Bank 20K       1
      18    36   Bank 20K       2
      19    36   Bank 20K       3
      20    36   Bank 20K       4
      21    52   Rollover 2K   11
      22    53   Rollover 4K   11
      23    54   Rollover 6K   11
      24    55   Rollover 8K   11
      25    56   Rollover 10K  11
#endif


    if (letterswitchnum < 3) {                              // Collect next SPA letter if not completed
      for(int i=0; i<3; i++) {
        //Serial.print(F("ShootAll capture SPA letter loop, letterswitchnum is: "));
        //Serial.println(letterswitchnum, DEC);
        if ( (ShootAll[CurrentPlayer] & (0b1<<(0+i))) == false ) {              // If flag is 0
          ShootAll[CurrentPlayer] = ( ShootAll[CurrentPlayer] | (0b1<<(0+i)) ); // then raise flag
          //Serial.print("Mask: ");
          //Serial.println( (0b1<<(0+i)), BIN);
          LettersLighting(1);                               // Reset lamps
          AddToFlashingLampQueue((ShootAllLamps[i]), 0, CurrentTime, SHOOT_ALL_LAMP_CAPTURE_EVENT_LENGTH, 
                      SHOOT_ALL_LAMP_CAPTURE_EVENT_START_PERIOD, SHOOT_ALL_LAMP_CAPTURE_EVENT_END_PERIOD);
          PlaySoundEffect(SOUND_EFFECT_SPA);
          Thousand_Pts_Stack += 5;
          Serial.println(F("Mode 2 - Captured SPA letter"));
          returnVal = true;                                // Letter was captured
          break;
        }
      }
    } else if (letterswitchnum < 9) {                       // Collect next FUTURE letter if not completed
      switch (ShootAllFutureComplete[CurrentPlayer]) {
        case 0:                                             // Incomplete section
          if ( (ShootAll[CurrentPlayer] & (0b1<<letterswitchnum)) == false ) {              // If flag is 0
            ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] | (0b1<<letterswitchnum) );  // Raise flag
            LettersLighting(1);                             // Reset lamps
            AddToFlashingLampQueue((ShootAllLamps[letterswitchnum]), 0, CurrentTime, SHOOT_ALL_LAMP_CAPTURE_EVENT_LENGTH,
                                      SHOOT_ALL_LAMP_CAPTURE_EVENT_START_PERIOD, SHOOT_ALL_LAMP_CAPTURE_EVENT_END_PERIOD);
            PlaySoundEffect(SOUND_EFFECT_SPA);
            Thousand_Pts_Stack += 5;
            Serial.println(F("Mode 2 - Captured FUTURE letter"));
          } else {                                          // was previously captured
            PlaySoundEffect(SOUND_EFFECT_SPA);
            Hundred_Pts_Stack += 5;
          }
          if ( (ShootAll[CurrentPlayer] & 0b111111000) == 504) {  // If sequence is complete
            ShootAllFutureComplete[CurrentPlayer] = 1;
            CallOut(CALL_OUT_SHOOT_ALL_FUTURE_COMPLETE, 20, CurrentTime+2000);
            // Call out if 2 or more segments are completed
            if (ShootAllCompletedSegments() == (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_1, 30, CurrentTime+4000);
            if (ShootAllCompletedSegments() > (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_2, 30, CurrentTime+4000);
          }
          break;
        case 1:                                             // Completed section
          for(int i=LA_FUTURE_F; i<LA_FUTURE_E; i++) {      // Blank out FUTURE lamps
           RPU_SetLampState(i, 0);
          }
          if (ShootAllFutureCounter[CurrentPlayer] < SHOOT_ALL_COMPLETION_HITS_ALLOWED) {
            for (int i = 0; i < 6; i++) {                   // Cascade sequence varies with switch hit
              AddToFlashingLampQueue( (LA_FUTURE_F+(FUTURELampsCascade[letterswitchnum-3][i])), 0, CurrentTime + i*125, 2000, 33, 200);
            }
            if (ShootAllFutureCounter[CurrentPlayer] == (SHOOT_ALL_COMPLETION_HITS_ALLOWED-1)) {
              CallOut(CALL_OUT_SHOOT_ALL_DONT_HIT_THAT_AGAIN, 40, CurrentTime+2000);
            }
            PlaySoundEffect(SOUND_EFFECT_EXTRA_BALL);
            Thousand_Pts_Stack += 10;                       // Double score
            Serial.println(F("Mode 2 - FUTURE section is completed"));
            ShootAllFutureCounter[CurrentPlayer] += 1;      // Increment counter
          } else {
            // Zero out captured FUTURE lamps
            ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111111111111111111111000000111));
            ShootAllFutureComplete[CurrentPlayer]= 2;       // Allow animation before lamps reset
            ShootAllFutureCounter[CurrentPlayer] = 0;       // Reset counter
            Thousand_Pts_Stack -= 60;                       // Remove all lamp scores
            PlaySoundEffect(SOUND_EFFECT_BALL_LOST);
            CallOut(CALL_OUT_SHOOT_ALL_FUTURE_LOST, 50, CurrentTime+2000);
          }
          break;
        case 2:                                             // Default scoring while resetting
          PlaySoundEffect(SOUND_EFFECT_SPA);
          Hundred_Pts_Stack += 5;
          break;
      }
      ShootAllFutureCompleteStartTime = CurrentTime;
      returnVal = true;                                     // Always true in Mode 2
    } else if (letterswitchnum < 13) {                      // Collect next Kicker letter if not completed
      switch (ShootAllKickerComplete[CurrentPlayer]) {
        case 0:                                             // Incomplete section
          for(int i=12; i>8; i--) {
            Serial.print(F("ShootAll capture Kicker letter loop, letterswitchnum is: "));
            Serial.println(letterswitchnum, DEC);
            if ( (ShootAll[CurrentPlayer] & (0b1<<(0+i))) == false ) { // If flag is 0
              ShootAll[CurrentPlayer] = ( ShootAll[CurrentPlayer] | (0b1<<(0+i)) ); // then raise flag
              LettersLighting(1);                           // Reset lamps
              AddToFlashingLampQueue((ShootAllLamps[i]), 0, CurrentTime, SHOOT_ALL_LAMP_CAPTURE_EVENT_LENGTH,
                          SHOOT_ALL_LAMP_CAPTURE_EVENT_START_PERIOD, SHOOT_ALL_LAMP_CAPTURE_EVENT_END_PERIOD);
              PlaySoundEffect(SOUND_EFFECT_SPA);
              Thousand_Pts_Stack += 5;
              Serial.println(F("Mode 2 - Captured Kicker lamp"));
              break;
            }
          }
          if ( (ShootAll[CurrentPlayer] & 0b1111000000000) == 7680) { // If sequence is complete
            ShootAllKickerComplete[CurrentPlayer] = 1;
            CallOut(CALL_OUT_SHOOT_ALL_KICKER_COMPLETE, 20, CurrentTime+2000);
            // Call out if 2 or more segments are completed
            if (ShootAllCompletedSegments() == (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_1, 30, CurrentTime+4000);
            if (ShootAllCompletedSegments() > (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_2, 30, CurrentTime+4000);
          }
        break;
      case 1:                                               // Completed section
        for(int i=LA_KICKBACK_3K; i<LA_KICKBACK_9K; i++) {  // Blank out kicker lamps
          RPU_SetLampState(i, 0);
        }
        if (ShootAllKickerCounter[CurrentPlayer] < SHOOT_ALL_COMPLETION_HITS_ALLOWED) {
          for (int i = 0; i < 4; i++) {                     // Cascade sequence
            AddToFlashingLampQueue(LA_KICKBACK_3K+i, 0, CurrentTime + i*125, 2000, 33, 200);
          }
          if (ShootAllKickerCounter[CurrentPlayer] == (SHOOT_ALL_COMPLETION_HITS_ALLOWED-1)) {
            CallOut(CALL_OUT_SHOOT_ALL_DONT_HIT_THAT_AGAIN, 40, CurrentTime+2000);
          }
          PlaySoundEffect(SOUND_EFFECT_EXTRA_BALL);
          Thousand_Pts_Stack += 10;                         // Double score
          Serial.println(F("Mode 2 - Kickersection is completed"));
          ShootAllKickerCounter[CurrentPlayer] += 1;// Increment counter
        } else {
          // Zero out captured Kicker lamps
          ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111111111111111110000111111111));
          ShootAllKickerComplete[CurrentPlayer]= 2;         // Allow animation before lamps reset
          ShootAllKickerCounter[CurrentPlayer] = 0;         // Reset counter
          Thousand_Pts_Stack -= 40;                         // Remove all lamp scores
          PlaySoundEffect(SOUND_EFFECT_BALL_LOST);
          CallOut(CALL_OUT_SHOOT_ALL_KICKER_LOST, 50, CurrentTime+2000);
        }
        break;
      case 2:                                               // Default scoring while resetting
        PlaySoundEffect(SOUND_EFFECT_SPA);
        Hundred_Pts_Stack += 5;
        break;
      }
      ShootAllKickerCompleteStartTime = CurrentTime;
      returnVal = true; // Always true in Mode 2
    } else if (letterswitchnum < 16) {                      // Collect next Saucer letter if not completed
      switch (ShootAllSaucerComplete[CurrentPlayer]) {
        case 0:                                             // Incomplete section
          for(int i=13; i<16; i++) {
            unsigned long mask = 1;
            for (int j=0; j<i; j++) {
              mask *= 2;
            }
            if ( (ShootAll[CurrentPlayer] & mask) == false ) { // If flag is 0
              ShootAll[CurrentPlayer] = ( ShootAll[CurrentPlayer] | mask ); // then raise flag
              LettersLighting(1);                           // Reset lamps
              AddToFlashingLampQueue((ShootAllLamps[i]), 0, CurrentTime, SHOOT_ALL_LAMP_CAPTURE_EVENT_LENGTH,
                        SHOOT_ALL_LAMP_CAPTURE_EVENT_START_PERIOD, SHOOT_ALL_LAMP_CAPTURE_EVENT_END_PERIOD);
              PlaySoundEffect(SOUND_EFFECT_SPA);
              Thousand_Pts_Stack += 5;
              Serial.println(F("Mode 2 - Captured Saucer lamp"));
              break;
            }
          }
          if ( (ShootAll[CurrentPlayer] & 0b1110000000000000) == 57344 ) {
            ShootAllSaucerComplete[CurrentPlayer] = 1;
            CallOut(CALL_OUT_SHOOT_ALL_SAUCER_COMPLETE, 20, CurrentTime+2000);
            // Call out if 2 or more segments are completed
            if (ShootAllCompletedSegments() == (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_1, 30, CurrentTime+4000);
            if (ShootAllCompletedSegments() > (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_2, 30, CurrentTime+4000);
          }
          break;
        case 1:
          for(int i=LA_SAUCER_3K; i<LA_SAUCER_9K; i++) {    // Blank out saucer lamps
            RPU_SetLampState(i, 0);
          }
          if (ShootAllSaucerCounter[CurrentPlayer] < SHOOT_ALL_COMPLETION_HITS_ALLOWED) {
          for (int i = 0; i < 3; i++) {                     // Cascade sequence
            AddToFlashingLampQueue(LA_SAUCER_3K+i, 0, CurrentTime + i*125, 2000, 33, 200);
          }
          if (ShootAllSaucerCounter[CurrentPlayer] == (SHOOT_ALL_COMPLETION_HITS_ALLOWED-1)) {
            CallOut(CALL_OUT_SHOOT_ALL_DONT_HIT_THAT_AGAIN, 40, CurrentTime+2000);
          }
          PlaySoundEffect(SOUND_EFFECT_EXTRA_BALL);
          Thousand_Pts_Stack += 10;                         // Double score
          Serial.println(F("Mode 2 - Saucersection is completed"));
          ShootAllSaucerCounter[CurrentPlayer] += 1;        // Increment counter
          } else {
            // Zero out captured Saucer lamps
            ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111111111111110001111111111111));
            ShootAllSaucerComplete[CurrentPlayer]= 2;       // Allow animation before lamps reset
            ShootAllSaucerCounter[CurrentPlayer] = 0;       // Reset counter
            Thousand_Pts_Stack -= 30;                       // Remove all lamp scores
            PlaySoundEffect(SOUND_EFFECT_BALL_LOST);
            CallOut(CALL_OUT_SHOOT_ALL_SAUCER_LOST, 50, CurrentTime+2000);
          }
        break;
      case 2:                                               // Default scoring while resetting
        PlaySoundEffect(SOUND_EFFECT_SPA);
        Hundred_Pts_Stack += 5;
        break;
      }
      ShootAllSaucerCompleteStartTime = CurrentTime;
      returnVal = true;
    } else if (letterswitchnum < 21) {                       // Set target lamp flash rate based on targets dropped
      Serial.print(F("ShootAll Set target 20K lamp, letterswitchnum is: "));
      Serial.println(letterswitchnum, DEC);
      unsigned long mask = 1;
      for (int j=0; j<letterswitchnum; j++) {
        mask *= 2;
      }
      if ( (ShootAll[CurrentPlayer] & mask) == false ) {              // If flag is 0
        ShootAll[CurrentPlayer] = ( ShootAll[CurrentPlayer] | mask ); // then raise flag
        int TargetFlashSpeed[4]={500,250,125,63};                     // Target 20K flasher speed progression
        if (letterswitchnum<20) {
          RPU_SetLampState(ShootAllLamps[letterswitchnum], 1, 0, TargetFlashSpeed[letterswitchnum-16]);
        } else {
          //AddToFlashingLampQueue((ShootAllLamps[letterswitchnum]), 0, CurrentTime, 1000, 33, 150);  
          AddToFlashingLampQueue((ShootAllLamps[letterswitchnum]), 0, CurrentTime, SHOOT_ALL_LAMP_CAPTURE_EVENT_LENGTH, 
                      SHOOT_ALL_LAMP_CAPTURE_EVENT_START_PERIOD, SHOOT_ALL_LAMP_CAPTURE_EVENT_END_PERIOD);
        }
        PlaySoundEffect(SOUND_EFFECT_SPA);
        Thousand_Pts_Stack += 5;
        Serial.println(F("Mode 2 - Set Target 20K lamps flash rate"));
        returnVal = true;                                             // Letter was captured
      }
    } else if (letterswitchnum < 26) {                                // Collect next Rollover letter if not completed
      switch (ShootAllRollOverComplete[CurrentPlayer]) {
        case 0:                                                       // Incomplete section
          for(int i=21; i<26; i++) {
            unsigned long mask = 1;
            for (int j=0; j<i; j++) {
              mask *= 2;
            }
            if ( (ShootAll[CurrentPlayer] & mask) == false ) {        // If flag is 0
              ShootAll[CurrentPlayer] = ( ShootAll[CurrentPlayer] | mask ); // then raise flag
              LettersLighting(1);                                     // Reset lamps
              AddToFlashingLampQueue((ShootAllLamps[i]), 0, CurrentTime, SHOOT_ALL_LAMP_CAPTURE_EVENT_LENGTH,
                          SHOOT_ALL_LAMP_CAPTURE_EVENT_START_PERIOD, SHOOT_ALL_LAMP_CAPTURE_EVENT_END_PERIOD);
              PlaySoundEffect(SOUND_EFFECT_SPA);
              Thousand_Pts_Stack += 5;
              Serial.print(F("Mode 2 - Captured Rollover lamp: "));
              Serial.println(i, DEC);
              break;
            }
          }
          //Serial.print("Mode 2 - RollOver capture total: ");
          //Serial.println((ShootAll[CurrentPlayer] & 0b11111000000000000000000000), DEC);
          if ( (ShootAll[CurrentPlayer] & 0b11111000000000000000000000) == 65011712 ) {
            ShootAllRollOverComplete[CurrentPlayer] = 1;
            CallOut(CALL_OUT_SHOOT_ALL_ROLLOVER_COMPLETE, 20, CurrentTime+2000);
            // Call out if 2 or more segments are completed
            if (ShootAllCompletedSegments() == (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_1, 30, CurrentTime+4000);
            if (ShootAllCompletedSegments() > (SHOOT_ALL_COMPLETED_SEGMENTS_FOR_BONUS+1)) CallOut(CALL_OUT_SHOOT_ALL_GO_FOR_SAUCER_2, 30, CurrentTime+4000);
          }
          break;
        case 1:
          for(int i=LA_ROLLOVER_BUTTON_2K; i<LA_ROLLOVER_BUTTON_10K; i++) { // Blank out roll over lamps
            RPU_SetLampState(i, 0);
          }
          if (ShootAllRollOverCounter[CurrentPlayer] < SHOOT_ALL_COMPLETION_HITS_ALLOWED) {
            for (int i = 0; i < 5; i++) {                             // Cascade sequence
              AddToFlashingLampQueue(LA_ROLLOVER_BUTTON_2K+i, 0, CurrentTime + i*125, 2000, 33, 200);
            }
            if (ShootAllRollOverCounter[CurrentPlayer] == (SHOOT_ALL_COMPLETION_HITS_ALLOWED-1)) {
              CallOut(CALL_OUT_SHOOT_ALL_DONT_HIT_THAT_AGAIN, 40, CurrentTime+2000);
            }
            PlaySoundEffect(SOUND_EFFECT_EXTRA_BALL);
            Thousand_Pts_Stack += 10; // Double score
            Serial.println("Mode 2 - All Roll Over lamps are captured");
            ShootAllRollOverCounter[CurrentPlayer] += 1;              // Increment counter
          } else {
            // Zero out captured RollOver lamps
            ShootAll[CurrentPlayer] = (ShootAll[CurrentPlayer] & (0b11111100000111111111111111111111));
            ShootAllRollOverComplete[CurrentPlayer]= 2;               // Allow animation before lamps reset
            ShootAllRollOverCounter[CurrentPlayer] = 0;               // Reset counter
            Thousand_Pts_Stack -= 50;                                 // Remove all lamp scores
            PlaySoundEffect(SOUND_EFFECT_BALL_LOST);
            CallOut(CALL_OUT_SHOOT_ALL_ROLLOVER_LOST, 50, CurrentTime+2000);
          }
          break;
        case 2:// Default scoring while resetting
          PlaySoundEffect(SOUND_EFFECT_SPA);
          Hundred_Pts_Stack += 5;
          break;
      }
      ShootAllRollOverCompleteStartTime = CurrentTime;
      returnVal = true; // Always true in Mode 2
    }  // End of Mode 2

  } else if (GameMode[CurrentPlayer] == 3) {                            // Mode 3 - Roaming

    if (RoamingStage < 3) {                                             // Can only capture lamps in 1 or 2
      // If not all SPA letters are collected, the next letter will be collected
      if (letterswitchnum < 3) {
        for(int i=0; i<3; i++) {
          if ( (RoamingLetters & (0b1<<(0+i))) == false ) {               // If flag is 0
            RoamingLetters = ( RoamingLetters | (0b1<<(0+i)) );           // then raise flag
            NumCapturedLetters +=1;
            Serial.println(F("Roaming captured SPA letter"));
            returnVal = true;                                             // Letter was captured
            break;
          }  
        }
      }
      // For switchnum from 3-8 FUTURE letters
      if ((letterswitchnum > 2) && (letterswitchnum < 9)) {
        if ( (RoamingLetters & (0b1<<letterswitchnum)) == false ) {       // If flag is 0
          RoamingLetters = (RoamingLetters | (0b1<<letterswitchnum) );    // then raise flag
          NumCapturedLetters +=1;
          Serial.println(F("Roaming captured FUTURE letter"));
          returnVal = true;                                               // Letter was captured
        }
      }
      if (returnVal) {                                                    // If we captured a letter
        if (NumCapturedLetters == 1) RoamingStage = 2;                    // Move to next stage
        if (NumCapturedLetters == 3) {
          CallOut(CALL_OUT_ROAMING_NEXT_ONE_SCORE_BIG, 30, (CurrentTime+1000));
          NewBackgroundSong(161, true);
        }
        if (NumCapturedLetters == 6) {
          CallOut(CALL_OUT_ROAMING_NOW_FOR_BIG, 35, (CurrentTime+1000));
          NewBackgroundSong(162, true);
        }
        if (NumCapturedLetters == 9) {
          RoamingStage = 4;                                               // Completion stage
          ShowPlayerScores(0xFF, false, false);                           // Reset all score displays
        }
        RoamingModeTime = CurrentTime;
        RoamingRotateTime = CurrentTime;
        // Comment line below to prevent letters from reversing
        //ReverseRoam = !ReverseRoam;                                       // Next animation reverses
        Thousand_Pts_Stack += RoamingScores[NumCapturedLetters-1];
        if (NumCapturedLetters < 9) {
          PlaySoundEffect(SOUND_EFFECT_SPA);                              // Play sound for getting letter
        } else {                                                          // if NumCapturedLetters == 9
          PlaySoundEffect(SOUND_EFFECT_SPA);                              // Play extra big sound for getting all letters
        }
        LettersLighting(1);                                               // Reset all lamps
      }
    } else {                                                              // RoamingStage > 2, Default scoring only for stages 3,4
      returnVal= false;                                                   // Don't actually have to set this, just feels good
    }
  }   // End of Mode 3
 
  //Serial.print("returnVal is: ");
  //Serial.println(returnVal, DEC);
  return returnVal;

}   // - End of CaptureLetter


void ShowShootAgainLamp() {
  if (!HaltSPSALighting) {
    if (!BallSaveUsed && BallSaveNumSeconds > 0 && (CurrentTime - BallFirstSwitchHitTime) < ((unsigned long)(BallSaveNumSeconds - 1) * 1000)) {
      Serial.println(F("ShowShootAgainLamp called"));
      unsigned long msRemaining = ((unsigned long)(BallSaveNumSeconds - 1) * 1000) - (CurrentTime - BallFirstSwitchHitTime);
      RPU_SetLampState(LA_SAME_PLAYER, 1, 0, (msRemaining < 1000) ? 100 : 500);
    } else {
      RPU_SetLampState(LA_SAME_PLAYER, SamePlayerShootsAgain);
      RPU_SetLampState(LA_SPSA_BACK, SamePlayerShootsAgain);
    }
  }
}


//
// BonusMultiplier - call to reset to 1x, or 2,4,6x
//

void BonusMultiplierLighting(byte mult){
  switch (mult){
    case 2: // 2X
      RPU_SetLampState(LA_2X, 1);
      RPU_SetLampState(LA_4X, 0);
      RPU_SetLampState(LA_6X, 0);
      break;
    case 4: // 4X
      RPU_SetLampState(LA_2X, 0);
      RPU_SetLampState(LA_4X, 1);
      RPU_SetLampState(LA_6X, 0);
      break;
    case 6: // 6X
      RPU_SetLampState(LA_2X, 0);
      RPU_SetLampState(LA_4X, 0);
      RPU_SetLampState(LA_6X, 1);
      break;
    default: // Set to default 1X
      RPU_SetLampState(LA_2X, 0);
      RPU_SetLampState(LA_4X, 0);
      RPU_SetLampState(LA_6X, 0);
      break;
    }
}

void BonusMultiplier(byte mult){
  switch (mult){
    case 2: // 2X
      BonusMult = 2;
      break;
    case 4: // 4X
      BonusMult = 4;
      break;
    case 6: // 6X
      BonusMult = 6;
      break;
    default: // Set to default 1X
      BonusMult = 1;
      break;
    }
    BonusMultiplierLighting(mult);
//  Serial.print(F("BonusMultiplier End, is             "));
//  Serial.println(BonusMult, DEC);
//  Serial.println();
}

//
// Saucer Lighting - handle post animation lighting
// 

void SaucerLighting(){
    RPU_SetLampState(LA_SAUCER_3K, ((SaucerProgress - 0) ? 0:1));
    RPU_SetLampState(LA_SAUCER_6K, ((SaucerProgress - 1) ? 0:1));
    RPU_SetLampState(LA_SAUCER_9K, ( ((SaucerProgress - 2) && (SaucerProgress - 3)) ? 0:1) );
    Serial.println(F("SaucerLighting()"));
}

//
// Bank Shot Lighting - handle post animation lighting
// 

void BankShotLighting(){
  Serial.println(F("BankShotLighting()"));
  RPU_SetLampState(LA_ROLLOVER_BUTTON_2K, ((BankShotProgress - 0) ? 0:1));
  RPU_SetLampState(LA_ROLLOVER_BUTTON_4K, ((BankShotProgress - 1) ? 0:1));
  RPU_SetLampState(LA_ROLLOVER_BUTTON_6K, ((BankShotProgress - 2) ? 0:1));
  RPU_SetLampState(LA_ROLLOVER_BUTTON_8K, ((BankShotProgress - 3) ? 0:1));
  RPU_SetLampState(LA_ROLLOVER_BUTTON_10K, ( ((BankShotProgress - 4) && (BankShotProgress - 5)) ? 0:1) );
}

//
// Bank Shot Scoring - handle scoring and sound only
// 

void BankShotScoring(){
  //Serial.print(F("BankShotScoring Beginning\n"));
  // Qualifying shot was made, increment counter which starts at zero
  BankShotOffTime = CurrentTime;
  
  BankShotProgress += 1;
  if (BankShotProgress>=6) BankShotProgress=5;
    switch (BankShotProgress) {
      case 1: // 2000 pts
        Thousand_Pts_Stack +=2;
        PlaySoundEffect(SOUND_EFFECT_BANKSHOT_2K);
        break;
      case 2: // 4000 pts
        Thousand_Pts_Stack +=4;
        PlaySoundEffect(SOUND_EFFECT_BANKSHOT_4K);
        break;
      case 3: // 6000 pts
        Thousand_Pts_Stack +=6;
        PlaySoundEffect(SOUND_EFFECT_BANKSHOT_6K);
        break;
      case 4: // 8000 pts
        Thousand_Pts_Stack +=8;
        PlaySoundEffect(SOUND_EFFECT_BANKSHOT_8K);
        break;
      case 5: // 10000 pts
        Thousand_Pts_Stack +=10;
        PlaySoundEffect(SOUND_EFFECT_BANKSHOT_10K);
        break;
    }
}

//
// Kicker Shot Lighting - handle post animation lighting
// 

void KickerShotLighting(){
  Serial.println(F("KickerShotLighting()"));
  Serial.print(F("KickerShotProgress is: "));
  Serial.println(KickerShotProgress, DEC);
  RPU_SetLampState(LA_KICKBACK_3K, ((KickerShotProgress - 0) ? 0:1));
  RPU_SetLampState(LA_KICKBACK_5K, ((KickerShotProgress - 1) ? 0:1));
  RPU_SetLampState(LA_KICKBACK_7K, ((KickerShotProgress - 2) ? 0:1));
  RPU_SetLampState(LA_KICKBACK_9K, ( ((KickerShotProgress - 3) && (KickerShotProgress - 4)) ? 0:1) );
}

//
// Kicker Shot Scoring - handle scoring and sound only
// 

void KickerShotScoring(){
  //Serial.print(F("KickerShotScoring\n"));
  // Qualifying shot was made, increment counter which starts at zero
  KickerShotOffTime = CurrentTime;
  
  KickerShotProgress += 1;
  if (KickerShotProgress > 4) KickerShotProgress=4;
    switch (KickerShotProgress) {
      case 1: // 3000 pts
        Thousand_Pts_Stack +=3;
        PlaySoundEffect(SOUND_EFFECT_KICKER_3K);
        RPU_PushToTimedSolenoidStack(SOL_KICKER, 3, CurrentTime + 1455);
        break;
      case 2: // 5000 pts
        Thousand_Pts_Stack +=5;
        PlaySoundEffect(SOUND_EFFECT_KICKER_5K);
        RPU_PushToTimedSolenoidStack(SOL_KICKER, 3, CurrentTime + 1588);
        break;
      case 3: // 7000 pts
        Thousand_Pts_Stack +=7;
        PlaySoundEffect(SOUND_EFFECT_KICKER_7K);
        RPU_PushToTimedSolenoidStack(SOL_KICKER, 3, CurrentTime + 1890);
        break;
      case 4: // 9000 pts
        Thousand_Pts_Stack +=9;
        PlaySoundEffect(SOUND_EFFECT_KICKER_9K);
        RPU_PushToTimedSolenoidStack(SOL_KICKER, 3, CurrentTime + 2192);
        break;
    }
}

void GreenArrowsLighting() {
  if (!HaltArrowsLighting) {
    if (GreenArrowsModeActive) {
      RPU_SetLampState(LA_ARROW_LANE_R, 1, 0, LitGreenArrowsTable[GreenArrowsModeActive].flashRate);
      RPU_SetLampState(LA_ARROW_LANE_U1, 1, 0, LitGreenArrowsTable[GreenArrowsModeActive].flashRate);
    } else {
    RPU_SetLampState(LA_ARROW_LANE_R, (GreenArrowsMode[CurrentPlayer]) );
    RPU_SetLampState(LA_ARROW_LANE_U1, (GreenArrowsMode[CurrentPlayer]) );
    }
  }
}

void GateArrowsLighting() {
  if (!HaltArrowsLighting) {
    RPU_SetLampState(LA_ARROW_LANE_U2, (GateOpen));
    RPU_SetLampState(LA_ARROW_LANE_T, (GateOpen));
    RPU_SetLampState(LA_GATE_ARROW, (!GateOpen));
  }
}

#if 0

//
// PlaySoundEffect
//

#define SOUND_EFFECT_NONE_0             0
#define SOUND_EFFECT_SPINNER_100_PT     1
#define SOUND_EFFECT_SPINNER_1000_PT    2
#define SOUND_EFFECT_30_PT              3
#define SOUND_EFFECT_DROP_TARGETS       4
#define SOUND_EFFECT_SLINGSHOT          5
#define SOUND_EFFECT_GAME_OVER          6
#define SOUND_EFFECT_18K                7
#define SOUND_EFFECT_OUTLANE            8
#define SOUND_EFFECT_NONE_9             9
#define SOUND_EFFECT_KICKER            10
#define SOUND_EFFECT_GAME_START        11
#define SOUND_EFFECT_SAUCER            12
#define SOUND_EFFECT_MACHINE_START     13
#define SOUND_EFFECT_14                14
#define SOUND_EFFECT_NONE_15           15
#define SOUND_EFFECT_BUMPER            16
#define SOUND_EFFECT_BACKGROUND        17
#define SOUND_EFFECT_NOTE_4            18
#define SOUND_EFFECT_NOTE_3            19
#define SOUND_EFFECT_NOTE_2            20
#define SOUND_EFFECT_NOTE_1            21
#define SOUND_EFFECT_SPA               22
#define SOUND_EFFECT_FUTURITY          23
#define SOUND_EFFECT_TILT              24
#define SOUND_EFFECT_NONE_25           25
#define SOUND_EFFECT_COIN              26
#define SOUND_EFFECT_27                27
#define SOUND_EFFECT_NONE_28           28
#define SOUND_EFFECT_BUMPER_LL         29
#define SOUND_EFFECT_INLANE            30
#define SOUND_EFFECT_NONE_31           31
#define SOUND_EFFECT_SCALE_UP          32
#define SOUND_EFFECT_SCALE_DOWN        33
#define SOUND_EFFECT_SAD               34
#define SOUND_EFFECT_36K               35

#define SOUND_EFFECT_SAUCER_3K:        36
#define SOUND_EFFECT_SAUCER_6K:        37
#define SOUND_EFFECT_SAUCER_9K:        38

#define SOUND_EFFECT_KICKER_3K:        39
#define SOUND_EFFECT_KICKER_5K:        40
#define SOUND_EFFECT_KICKER_7K:        41
#define SOUND_EFFECT_KICKER_9K:        42

#define SOUND_EFFECT_BANKSHOT_2K:      43
#define SOUND_EFFECT_BANKSHOT_4K:      44
#define SOUND_EFFECT_BANKSHOT_6K:      45
#define SOUND_EFFECT_BANKSHOT_8K:      46
#define SOUND_EFFECT_BANKSHOT_10K:     47
#define SOUND_EFFECT_SPA_5K            48
#define SOUND_EFFECT_POP_MODE          49
#define SOUND_EFFECT_EXTRA_BALL        50
#define SOUND_EFFECT_SPINNER_MODE      51
#define SOUND_EFFECT_BALL_LOST         52
#define SOUND_EFFECT_CARMEN_UP         53
#define SOUND_EFFECT_CARMEN_DOWN       54
#define SOUND_EFFECT_CARMEN_END        55

#endif

// Array of sound effect durations (Sound effects 0 to x)
int SoundEffectDuration[56] = 
{0, 10, 0, 0, 0, 10, 0, 0, 3500, 0,
0, 0, 0, 5000, 0, 0, 0, 0, 15, 15,                        // New machine start artifically short 11050
15, 15, 600, 1350, 0, 0, 150, 0, 0, 0,                     // Coin up is only partial sound
1100, 0, 200, 200, 100, 300, 807, 1182, 1682, 1390,
1515, 1765, 2015, 932, 1182, 1432, 1682, 2182, 900, 1600,
800, 1200, 250, 1000, 1000, 5000};                         // Added extra 50 to EXTRA BALL

unsigned long NextSoundEffectTime = 0;

void PlaySoundEffect(byte soundEffectNum, boolean priority = false) {

// Note - priority force queues a new sound, it does not cut off an existing one

unsigned long TimeStart;

  if (CurrentTime > NextSoundEffectTime) {            // No sound effect running
    NextSoundEffectTime = CurrentTime + SoundEffectDuration[soundEffectNum];
    TimeStart = CurrentTime;

    #ifdef EXECUTION_MESSAGES    
    //Serial.println(F("No sound was playing"));
    #endif
/*    Serial.print(F("soundEffectNum: "));
    Serial.println(soundEffectNum, DEC);
    Serial.print(F("CurrentTime is:            "));
    Serial.println(CurrentTime, DEC);
    Serial.print(F("SoundEffectDuration is:    "));
    Serial.println(SoundEffectDuration[soundEffectNum], DEC);
    Serial.print(F("NextSoundEffectTime        "));
    Serial.println(NextSoundEffectTime, DEC);
    Serial.println();
*/
  } else {                                 // A sound effect is in process if here
      TimeStart = NextSoundEffectTime;
      #ifdef EXECUTION_MESSAGES
      Serial.println(F("Sound already playing"));
      #endif
      // Allow priority sounds to queue regardless  
      if ( ((NextSoundEffectTime - CurrentTime) > 250) && (!priority) ) {
        #ifdef EXECUTION_MESSAGES
        Serial.println(F("Break out of playing sound"));
        #endif
        return;                            // Don't play this sound
      }
      #ifdef EXECUTION_MESSAGES
      if (priority) {
        Serial.println(F("Priority sound being queued"));
      }
      #endif
      NextSoundEffectTime = NextSoundEffectTime + 20 + SoundEffectDuration[soundEffectNum];
/*      
      Serial.print(F("soundEffectNum: "));
      Serial.println(soundEffectNum, DEC);
      Serial.print(F("SoundEffectDuration: "));
      Serial.println(SoundEffectDuration[soundEffectNum], DEC);
      Serial.print(F("NextSoundEffectTime: "));
      Serial.println(NextSoundEffectTime, DEC);
      Serial.print(F("CurentTime: "));
      Serial.println(CurrentTime, DEC);
      Serial.println();
*/    
  }

  if (DipSwitch30 > 1) {                             // if WavTrigger sounds are used
    //if (CalloutActive) return;                       // Active call out cancels sound effects
    //Serial.print(F("Queueing WTrig sound effect number: "));
    //Serial.println(soundEffectNum, DEC);
    AddToSoundQueue(soundEffectNum, TimeStart);
    if (soundEffectNum == SOUND_EFFECT_SPINNER_MODE) {
      RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, TimeStart + 1105);
    }
  } else {                                            // DASH51 sound set

    switch (soundEffectNum) {
      case SOUND_EFFECT_NONE_0:                       // Sound 0 - blank sound
        AddToSoundQueue(0, TimeStart);
        break;
      case SOUND_EFFECT_SPINNER_100_PT:               //  1
        AddToSoundQueue(SOUND_EFFECT_SPINNER_100_PT, TimeStart);
        break;
      case SOUND_EFFECT_SPINNER_1000_PT:              //  2
        AddToSoundQueue(SOUND_EFFECT_SPINNER_1000_PT, TimeStart);
        break;
      case SOUND_EFFECT_30_PT:                        //  3
        AddToSoundQueue(SOUND_EFFECT_30_PT, TimeStart);
        break;
      case SOUND_EFFECT_DROP_TARGETS:                 //  4
        AddToSoundQueue(SOUND_EFFECT_DROP_TARGETS, TimeStart);
        break;
      case SOUND_EFFECT_SLINGSHOT:                    //  5
        AddToSoundQueue(SOUND_EFFECT_SLINGSHOT, TimeStart);
        break;
      case SOUND_EFFECT_GAME_OVER:                    //  6
        AddToSoundQueue(SOUND_EFFECT_GAME_OVER, TimeStart);
        break;
      case SOUND_EFFECT_18K:                          //  7
        AddToSoundQueue(SOUND_EFFECT_18K, TimeStart);
        break;
      case SOUND_EFFECT_OUTLANE:                      //  8
        AddToSoundQueue(SOUND_EFFECT_OUTLANE, TimeStart);
        break;
      case SOUND_EFFECT_KICKER:                       // 10
        AddToSoundQueue(SOUND_EFFECT_KICKER, TimeStart);
        break;
      case SOUND_EFFECT_GAME_START:                   // 11
        AddToSoundQueue(SOUND_EFFECT_GAME_START, TimeStart);
        break;
      case SOUND_EFFECT_SAUCER:                       // 12
        AddToSoundQueue(SOUND_EFFECT_SAUCER, TimeStart);
        break;
      case SOUND_EFFECT_MACHINE_START:                // 13
        AddToSoundQueue(SOUND_EFFECT_MACHINE_START, TimeStart);
        break;
      case SOUND_EFFECT_14:                           // 14
        AddToSoundQueue(SOUND_EFFECT_14, TimeStart);
        break;
      case SOUND_EFFECT_BUMPER:                       // 16
        AddToSoundQueue(SOUND_EFFECT_BUMPER, TimeStart);
        break;
      case SOUND_EFFECT_BACKGROUND:                   // 17
        AddToSoundQueue(SOUND_EFFECT_BACKGROUND, TimeStart);
        break;
      case SOUND_EFFECT_NOTE_4:                       // 18
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart);
        break;
      case SOUND_EFFECT_NOTE_3:                       // 19
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart);
        break;
      case SOUND_EFFECT_NOTE_2:                       // 20
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart);
        break;
      case SOUND_EFFECT_NOTE_1:                       // 21
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart);
        break;
      case SOUND_EFFECT_SPA:                          // 22
        AddToSoundQueue(SOUND_EFFECT_SPA, TimeStart);
        break;
      case SOUND_EFFECT_FUTURITY:                     // 23
        AddToSoundQueue(SOUND_EFFECT_FUTURITY, TimeStart);
        break;
      case SOUND_EFFECT_TILT:                         // 24
        AddToSoundQueue(SOUND_EFFECT_TILT, TimeStart);
        break;
      case SOUND_EFFECT_NONE_25:                      // 25
        AddToSoundQueue(SOUND_EFFECT_NONE_25, TimeStart);
        break;
      case SOUND_EFFECT_BUMPER_LL:                    // 29
        AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart + 25);
        AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart + 50);
        AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart + 75);
        AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart + 100);
        break;
      case SOUND_EFFECT_INLANE:                       // 30
        AddToSoundQueue(SOUND_EFFECT_INLANE, TimeStart);
        break;
      case SOUND_EFFECT_COIN:                         // Sound 26
        AddToSoundQueue(0, TimeStart);
        AddToSoundQueue(26, TimeStart + 10);
        AddToSoundQueue(26, TimeStart + 10 + 9);
        AddToSoundQueue(26, TimeStart + 10 + 19);
        AddToSoundQueue(26, TimeStart + 10 + 35);
        AddToSoundQueue(26, TimeStart + 10 + 49);
        break;
      case SOUND_EFFECT_SCALE_UP:                     // Sound 32 - duration 200
        //AddToSoundQueue(0, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 0);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 60);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 120);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 180);
        break;
      case SOUND_EFFECT_SCALE_DOWN:                   //
        //AddToSoundQueue(0, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 0);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 60);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 120);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 180);
        break;
      case SOUND_EFFECT_SAD:                          //
        //AddToSoundQueue(0, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 0);
        //AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 60);
        //AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 120);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 100);
        break;
      case SOUND_EFFECT_36K:                          //
        AddToSoundQueue(SOUND_EFFECT_18K, TimeStart + 0);
        AddToSoundQueue(SOUND_EFFECT_18K, TimeStart + 165);
        break;
      case SOUND_EFFECT_SAUCER_3K:                    // 807
        AddToSoundQueue(SOUND_EFFECT_SAUCER, TimeStart);
        for (int count = 0; count < 3; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_SAUCER_6K:                    // 1182
        AddToSoundQueue(SOUND_EFFECT_SAUCER, TimeStart);
        for (int count = 0; count < 6; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_SAUCER_9K:                    // 1682
        AddToSoundQueue(SOUND_EFFECT_SAUCER, TimeStart);
        for (int count = 0; count < 10; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_KICKER_3K:                    // 1390
        AddToSoundQueue(SOUND_EFFECT_KICKER, TimeStart);
        for (int count = 0; count < 5; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+740+count*125);
        }
        break;
      case SOUND_EFFECT_KICKER_5K:                    // 1515
        AddToSoundQueue(SOUND_EFFECT_KICKER, TimeStart);
        for (int count = 0; count < 6; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+740+count*125);
        }
        break;
      case SOUND_EFFECT_KICKER_7K:                    // 1765
        AddToSoundQueue(SOUND_EFFECT_KICKER, TimeStart);
        for (int count = 0; count < 8; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+740+count*125);
        }
        break;
      case SOUND_EFFECT_KICKER_9K:                    // 2015
        AddToSoundQueue(SOUND_EFFECT_KICKER, TimeStart);
        for (int count = 0; count < 10; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+740+count*125);
        }
        break;
      case SOUND_EFFECT_BANKSHOT_2K:                  // 932
        AddToSoundQueue(SOUND_EFFECT_14, TimeStart);
        for (int count = 0; count < 4; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_BANKSHOT_4K:                  // 1182
        AddToSoundQueue(SOUND_EFFECT_14, TimeStart);
        for (int count = 0; count < 6; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_BANKSHOT_6K:                  // 1432
        AddToSoundQueue(SOUND_EFFECT_14, TimeStart);
        for (int count = 0; count < 8; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_BANKSHOT_8K:                  // 1682
        AddToSoundQueue(SOUND_EFFECT_14, TimeStart);
        for (int count = 0; count < 10; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_BANKSHOT_10K:                 // 2182
        AddToSoundQueue(SOUND_EFFECT_14, TimeStart);
        for (int count = 0; count < 14; count++) {
          AddToSoundQueue(SOUND_EFFECT_BUMPER_LL, TimeStart+407+count*125);
        }
        break;
      case SOUND_EFFECT_SPA_5K:                       // duration 900
        AddToSoundQueue(29, TimeStart);
        AddToSoundQueue(29, TimeStart+100);
        AddToSoundQueue(29, TimeStart+200);
        AddToSoundQueue(29, TimeStart+300);
        AddToSoundQueue(29, TimeStart+400);
        break;
      case SOUND_EFFECT_POP_MODE:                     // duration 1600 includes buffer space
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 100);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 200);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 300);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 425);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 525);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 625);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 725);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 850);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 950);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 1050);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 1150);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1275);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1375);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1475);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1575);
        break;
      case SOUND_EFFECT_EXTRA_BALL:                    // duration 625
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 100);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 200);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 300);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 550);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 650);
  /*      AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 75);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 150);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 225);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 375);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 450);*/
        break;
      case SOUND_EFFECT_SPINNER_MODE:                  // duration 1200
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 75);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 135);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 435);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 510);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 570);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 870);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 945);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 1005);
        RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, TimeStart + 1105);
      case SOUND_EFFECT_BALL_LOST:                    // duration 250
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 250);
      case SOUND_EFFECT_CARMEN_UP:                    // duration 1500
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 0);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 180);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 575);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1045);
        break;
      case SOUND_EFFECT_CARMEN_DOWN:                  // duration 1500
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 0);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 180);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 575);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 1045);
        break;
      case SOUND_EFFECT_CARMEN_END:                   // duration 2800
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 0);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 80);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 160);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 240);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 320);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 400);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 480);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 560);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 985);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 1040);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 1135);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 1235);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1500);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 1580);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1660);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 1740);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1820);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 1900);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 1980);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 2060);
        AddToSoundQueue(SOUND_EFFECT_NOTE_4, TimeStart + 2485);
        AddToSoundQueue(SOUND_EFFECT_NOTE_3, TimeStart + 2540);
        AddToSoundQueue(SOUND_EFFECT_NOTE_2, TimeStart + 2635);
        AddToSoundQueue(SOUND_EFFECT_NOTE_1, TimeStart + 2735);        break;
    }
  }
}

//
// Callout Ver 4
//  - Priority based call outs.
//
void CallOut(unsigned int calloutNum, byte priority, unsigned long whenToPlay) {
  if (DipSwitch30 < 2) return;                                    // Not using WavTrigger
  if (calloutNum%2) {
    Serial.print(F("Odd numbered CallOut, rejected"));
  } else {                                                        // Even numbered Call Outs are accepted
  /*  Serial.println(F("Callout parameters"));
    Serial.print(F("calloutNum is:                 "));
    Serial.println(calloutNum, DEC);
    Serial.print(F("priority is:                   "));
    Serial.println(priority, DEC);
    Serial.print(F("whenToPlay is:                 "));
    Serial.println(whenToPlay, DEC);
    Serial.print(F("CurrentTime is:                "));
    Serial.println(CurrentTime, DEC);*/
    Serial.print(F("Callout to be started/queued:  "));
    Serial.println(calloutNum, DEC);
    //CurrentCallout = calloutNum;
    // Set starting point in time for call out
    //CalloutActive = true;                                         // Curtail other sounds when true
    if (whenToPlay >= CurrentTime) {
      Serial.println(F("Put future call out in sound queue"));
      AddToSoundQueue(calloutNum, whenToPlay, priority);
      AddToSoundQueue( (calloutNum + 1), (whenToPlay + CallOutDuration[(calloutNum-CALL_OUT_TRACK_NUMBER_OFFSET)/2]), priority ); // End event
      //CurrentCalloutEndTime = (whenToPlay + CallOutDuration[(calloutNum-CALL_OUT_TRACK_NUMBER_OFFSET)/2]);              // Set call out end
    } else if (whenToPlay == 0) {
      if (priority <= CalloutActive) {
        Serial.println(F("Callout rejecting immediate low priority call out"));
        return;                                                   // Equal or lower priority is rejected
      }
      Serial.println(F("Immediate call out, call CallOutStart() directly"));
      CalloutStart(calloutNum, priority);
      // Set end point in time for call out, end request is the call out number plus 1
      AddToSoundQueue( (calloutNum + 1), (CurrentTime + CallOutDuration[(calloutNum-CALL_OUT_TRACK_NUMBER_OFFSET)/2]) ); // End event
      //CurrentCalloutEndTime = (CurrentTime + CallOutDuration[(calloutNum-CALL_OUT_TRACK_NUMBER_OFFSET)/2]);              // Set call out end
    }
  }
  //ShowSoundQueueEntries();
}

//
// Ver 2 - Uses priority levels to gate call outs
//
void CalloutStart(unsigned int calloutTrack, unsigned int calloutPriority) {              // Start play of call out track
  if (calloutPriority > CalloutActive) {                                                  // Only start a call out if higher priority
    if (CalloutActive!=0) {
      wTrig.trackStop(CurrentCallout);                                                    // Halt current call out
      //ClearCalloutsFromSoundQueue();
    }
    CalloutActive = calloutPriority;                                                      // Curtail other sounds when true, any priority above 0
    CurrentCallout = calloutTrack;
    wTrig.trackGain(CurrentSoundEffect, (SFxVolume - SFX_VOLUME_DROP_FOR_CALL_OUT));      // Reduce current sound effect by XdB
    wTrig.trackGain(CurrentBackgroundSong, (MusicVolume - SONG_VOLUME_DROP_FOR_CALL_OUT));// Reduce current song by XdB
    wTrig.trackPlayPoly(calloutTrack);                                                    // Play Callout track
    wTrig.trackGain(calloutTrack, SFxVolume);                                             // Set track gain to X dB
    Serial.print(F("CalloutStart() - starting call out: "));
    Serial.println(calloutTrack, DEC);
    //Serial.println();
    //Serial.print(F("CurrentBackgroundSong is:                       "));
    //Serial.println(CurrentBackgroundSong, DEC);
    //Serial.print(F("MusicVolume - SONG_VOLUME_DROP_FOR_CALL_OUT is: "));
    //Serial.println((MusicVolume - SONG_VOLUME_DROP_FOR_CALL_OUT), DEC);
    //Serial.println();  
  } else {                                                                                // If equal or lower, reject
    Serial.println(F("CalloutStart rejecting low priority call out"));
    return;
  }
}

void CalloutEnd(unsigned int calloutTrack, unsigned int calloutPriority) {                // Restores sounds
  //return;       // Test only
  Serial.println();
  Serial.print(F("CalloutEnd() - ending call out: "));
  Serial.println(calloutTrack, DEC);
  wTrig.trackGain(CurrentSoundEffect, SFxVolume);                                         // Restore current sound effec volume
  wTrig.trackGain(CurrentBackgroundSong, MusicVolume);                                    // Restore current song volume
  CalloutActive = 0;                                                                      // Regular sounds allowed
}


//
// LettersLighting - Reset lights - Ver 4
//  Add Mode 3 Roaming
// LettersLighting - Reset lights - Ver 3
// Reset lights - using Letters[] variable
// Lamp order - 
//    Small lamps     SPA,                  0-2
//    Small lamps     FUTURE,               3-8
//    Big lamps       18K, 36K, SPA_SPECIAL 9-11 
//    Big lamps       FUTURE                12-20

void LettersLighting(byte Mode) {

  if (GameMode[CurrentPlayer] == 1) {  // Mode 1
    //
    // Mode 1
    //
    // Set all FUTURESPA and 18k, 36k, and Spa_Special lamps big and small after any event that alters them 
  
    for(int i=0; i<12; i++) {                     // Turn out all lane letters, plus 18K, 36K, Spa_Special
      //Serial.println(F("Turn off small lamps"));
      RPU_SetLampState((LA_SPA_S + i), 0);
    }
    for(int i=0; i<9; i++) {                      // Turn out all big letters
      //Serial.println(F("Turn off big lamps"));
      RPU_SetLampState((LA_BIG_S + i), 0);  
    }
  
  // Cycle through 6 FUTURE letters only
    for(int i=0; i<6; i++) {
      //Serial.print(F("(Letters[CurrentPlayer] & (0b1 << i) is:            "));
      //Serial.println((Letters[CurrentPlayer] & (0b1 << i)), BIN);
  
      if ( ((Letters[CurrentPlayer] & (0b1 << (i+3)))>>(i+3)) == true ) {
        RPU_SetLampState((LA_BIG_F + i), 1);     // if true turn large center lamp on
        //Serial.println(F("Turn on a big letter lamp"));
      } else {
        RPU_SetLampState((LA_FUTURE_F + i), 1);  // if large lamp is off, small lamp is on
        //Serial.println(F("Turn on a small lamp"));
      }
    }
  
  // Light up first uncaptured SPA letter only
  
  for(int i=0; i<3; i++) {                               // Turn on just one of the SPA lamps
      //Serial.print(F("(Letters[CurrentPlayer] & (0b1 << i) is:            "));
      //Serial.println((Letters[CurrentPlayer] & (0b1 << i)), BIN);
  
      if ( ((Letters[CurrentPlayer] & (0b1 << i))>>i) == true ) {
        RPU_SetLampState((LA_BIG_S + i), 1);              // if true turn large center lamp on
        //Serial.println(F("Turn on a big letter lamp"));
      } else {
        RPU_SetLampState((LA_SPA_S + i), 1);              // if large lamp is off, small lamp is on
        break;
        //Serial.println(F("Turn on a small lamp"));
      }
    }
  
  // Light up 18K, 36K, SPECIAL lamps
  
    for(int i=0; i<3; i++) {                               // Set 18K, 36K, SPA_SPECIAL lamps
        if ( ((Letters[CurrentPlayer] & (0b1 << (i+9)))>>(i+9))== true) {      
        //Serial.println(F("Turn on bonus lamps"));
        RPU_SetLampState((LA_18K_BONUS + i), 1);          // if true turn large center lamp on
      }
    }
    Serial.println(F("End of Mode 1 LettersLighting"));

  } else if (GameMode[CurrentPlayer] == 2) {               // ShootAll Mode 2
    //
    // Mode 2 - ShootAll
    //
    // Turn lamps on or off depending on capture status
    //
    for(int i=0; i<26; i++) {
      //Serial.print(F("Loop i= "));
      //Serial.println(i, DEC);
      unsigned long mask = 1; 
      for (int j=0; j<i; j++) {
        mask *= 2;
      }
      boolean test = (ShootAll[CurrentPlayer] & mask);
      if (test) {
        RPU_SetLampState(ShootAllLamps[i], 0);             // if true turn captured lamp off
        //Serial.println(F("Turn OFF a captured lamp"));
      } else {
        RPU_SetLampState(ShootAllLamps[i], 1);             // if false turn captured lamp on
      }
    }
    Serial.println(F("End of Mode 2 LettersLighting"));
  } else {                                                  // Rover Mode 3
    //
    // Mode 3 - Roaming
    //
    // Turn lamps on or off depending on capture status
    //
    
    // Turn out SPAFUTURE small and large letters, plus 18K, 36K, Spa_Special
    for(int i=0; i<21; i++) {
      RPU_SetLampState((LA_SPA_S + i), 0);
    }

    for(int i=0; i<9; i++) {
      if (RoamingLetters & (0b1 << i)) {
        RPU_SetLampState((LA_BIG_S + i), 1);              // if true turn large center lamp on
        //Serial.print(F("Turned on big letter: "));
        //Serial.println((LA_BIG_S + i), DEC);
      } else {
        RPU_SetLampState((LA_SPA_S + i), 1);              // if large lamp is off, small lamp is on
        //Serial.print(F("Turned on small letter: "));
        //Serial.println((LA_SPA_S + i), DEC);
      }
    }
    //Serial.println(F("End of Mode 3 LettersLighting"));
  }
}   // End of LettersLighting


#if 1

// 
// Marquee Animation Timing - Ver 3
//    Add mode 2 function
// Marquee Animation Timing - Ver 2
//

void MarqueeTiming() {
  if (GameMode[CurrentPlayer] == 1) {  
    if (MarqueeOffTime == 0) {
      MarqueeOffTime = CurrentTime;
      MarqueeMultiple = 1;
      Serial.println(F("Mode 1 - MarqueeOffTime set to CurrentTime."));
    } else {
      if (MarqueeMultiple < 10) {
        MarqueeMultiple += 1;  
      }
      Serial.println(F("MarqueeMultiple incremented."));
    }
  } else if (GameMode[CurrentPlayer] == 2) {
    if (MarqueeOffTime == 0) {
      MarqueeOffTime = CurrentTime;
      MarqueeMultiple = 1;
      Serial.println(F("Mode 2 - MarqueeOffTime set to CurrentTime."));
    }
  }
}

#else

// 
// Marquee Animation Timing - Ver 2
//

void MarqueeTiming() {
  if (MarqueeOffTime == 0) {
    MarqueeOffTime = CurrentTime;
    MarqueeMultiple = 1;
    Serial.println(F("MarqueeOffTime set to CurrentTime."));
  } else {
    if (MarqueeMultiple < 10) {
      MarqueeMultiple += 1;  
    }
    Serial.println(F("MarqueeMultiple incremented."));
  }
}

#endif

//
// CheckHighScores
//

void CheckHighScores() {
  unsigned long highestScore = 0;
  int highScorePlayerNum = 0;
  for (int count = 0; count < CurrentNumPlayers; count++) {
    if (CurrentScores[count] > highestScore) highestScore = CurrentScores[count];
    highScorePlayerNum = count;
  }
  Serial.println();
  Serial.print(F("HighScore : "));
  Serial.println(HighScore, DEC);
  Serial.print(F("highestScore : "));
  Serial.println(highestScore, DEC);
  Serial.println();

  // Remove number of goals reached from high score
  highestScore = highestScore/10*10;

  if (highestScore > HighScore) {
    HighScore = highestScore;
    Serial.println(F("highestScore was higher than HighScore: "));
    Serial.println();
    if (HighScoreReplay) {
      AddCredit(false, 3);
      RPU_WriteULToEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE, RPU_ReadULFromEEProm(RPU_TOTAL_REPLAYS_EEPROM_START_BYTE) + 3);
    }
    RPU_WriteULToEEProm(RPU_HIGHSCORE_EEPROM_START_BYTE, highestScore);
    RPU_WriteULToEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE, RPU_ReadULFromEEProm(RPU_TOTAL_HISCORE_BEATEN_START_BYTE) + 1);

    for (int count = 0; count < 4; count++) {
      if (count == highScorePlayerNum) {
        RPU_SetDisplay(count, CurrentScores[count], true, 2);
      } else {
        RPU_SetDisplayBlank(count, 0x00);
      }
    }

    RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime, true);
    RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime + 250, true);
    RPU_PushToTimedSolenoidStack(SOL_KNOCKER, 3, CurrentTime + 500, true);
  }
}

unsigned long SaucerStepTime = 0;                           // time of last lamp transition
byte SaucerLampCounter = 0;                                 // Lamp counter

void MarqueeAttract(byte Segment, int Speed, boolean CW) {

// Default speeds, Segment 1 = 100

  byte MQPhase1 = (CurrentTime/(Speed*3/4))%6; // 6 steps
  byte MQPhase2 = (CurrentTime/(Speed*6))%3; // 3 steps

  if ((Segment == 1) || (Segment == 10)) {

    // First leg
    RPU_SetLampState(LA_BIG_F,  ((CW == true)?(MQPhase1==0):(MQPhase1==5))||((CW == true)?(MQPhase1==1):(MQPhase1==0)), 0); 
    RPU_SetLampState(LA_BIG_U1, ((CW == true)?(MQPhase1==1):(MQPhase1==4))||((CW == true)?(MQPhase1==2):(MQPhase1==5)), 0);
    RPU_SetLampState(LA_BIG_T,  ((CW == true)?(MQPhase1==2):(MQPhase1==3))||((CW == true)?(MQPhase1==3):(MQPhase1==4)), 0);
    RPU_SetLampState(LA_BIG_U2, ((CW == true)?(MQPhase1==3):(MQPhase1==2))||((CW == true)?(MQPhase1==4):(MQPhase1==3)), 0);
    RPU_SetLampState(LA_BIG_R,  ((CW == true)?(MQPhase1==4):(MQPhase1==1))||((CW == true)?(MQPhase1==5):(MQPhase1==2)), 0); 
    RPU_SetLampState(LA_BIG_E,  ((CW == true)?(MQPhase1==5):(MQPhase1==0))||((CW == true)?(MQPhase1==0):(MQPhase1==1)), 0);

    // Second leg
    RPU_SetLampState(LA_SPA_SPECIAL, ((CW == true)?(MQPhase1==0):(MQPhase1==5))||((CW == true)?(MQPhase1==1):(MQPhase1==0)), 0);
    RPU_SetLampState(LA_36K_BONUS,   ((CW == true)?(MQPhase1==1):(MQPhase1==4))||((CW == true)?(MQPhase1==2):(MQPhase1==5)), 0);
    RPU_SetLampState(LA_18K_BONUS,   ((CW == true)?(MQPhase1==2):(MQPhase1==3))||((CW == true)?(MQPhase1==3):(MQPhase1==4)), 0); 
    RPU_SetLampState(LA_BIG_A,       ((CW == true)?(MQPhase1==3):(MQPhase1==2))||((CW == true)?(MQPhase1==4):(MQPhase1==3)), 0);
    RPU_SetLampState(LA_BIG_P,       ((CW == true)?(MQPhase1==4):(MQPhase1==1))||((CW == true)?(MQPhase1==5):(MQPhase1==2)), 0);
    RPU_SetLampState(LA_BIG_S,       ((CW == true)?(MQPhase1==5):(MQPhase1==0))||((CW == true)?(MQPhase1==0):(MQPhase1==1)), 0);
  }

  if (Segment == 10) {
    // Multiplier lamps 
    RPU_SetLampState(LA_2X, (CW == true)?(MQPhase2==0):(MQPhase2==2), 0); 
    RPU_SetLampState(LA_4X, (CW == true)?(MQPhase2==1):(MQPhase2==1), 0);
    RPU_SetLampState(LA_6X, (CW == true)?(MQPhase2==2):(MQPhase2==0), 0);
  }

  if (ShootAllStage[CurrentPlayer] != 1) {                      // Lock out when ShootAll is qualified but not running yet
    if ((Segment == 7) || (Segment == 10)) {
      if ((CurrentTime - SaucerStepTime) > Speed) {             // Sets letter lamp spacing
        SaucerStepTime = CurrentTime;
        SaucerLampCounter += 1;
        if (SaucerLampCounter > 2) (SaucerLampCounter = 0);
        // Saucer lamps 
        RPU_SetLampState(LA_SAUCER_3K, (CW == true)?(SaucerLampCounter==0):(SaucerLampCounter==2) ); 
        RPU_SetLampState(LA_SAUCER_6K, (CW == true)?(SaucerLampCounter==1):(SaucerLampCounter==1) );
        RPU_SetLampState(LA_SAUCER_9K, (CW == true)?(SaucerLampCounter==2):(SaucerLampCounter==0) );
      }
    }
  }
  
  byte MQPhase3 = (CurrentTime/(Speed*6/10))%5; //  5 steps
  if ((Segment == 2) || (Segment == 10)) {
    // Bank Shot lamps - Trailing lamp is dim
    RPU_SetLampState(LA_ROLLOVER_BUTTON_2K, ((CW == true)?(MQPhase3==0):(MQPhase3==4))||((CW == true)?(MQPhase3==1):(MQPhase3==0)), ((CW == true)?(MQPhase3==1):(MQPhase3==0))); 
    RPU_SetLampState(LA_ROLLOVER_BUTTON_4K, ((CW == true)?(MQPhase3==1):(MQPhase3==3))||((CW == true)?(MQPhase3==2):(MQPhase3==4)), ((CW == true)?(MQPhase3==2):(MQPhase3==4)));
    RPU_SetLampState(LA_ROLLOVER_BUTTON_6K, ((CW == true)?(MQPhase3==2):(MQPhase3==2))||((CW == true)?(MQPhase3==3):(MQPhase3==3)), ((CW == true)?(MQPhase3==3):(MQPhase3==3)));
    RPU_SetLampState(LA_ROLLOVER_BUTTON_8K, ((CW == true)?(MQPhase3==3):(MQPhase3==1))||((CW == true)?(MQPhase3==4):(MQPhase3==2)), ((CW == true)?(MQPhase3==4):(MQPhase3==2)));
    RPU_SetLampState(LA_ROLLOVER_BUTTON_10K, ((CW == true)?(MQPhase3==4):(MQPhase3==0))||((CW == true)?(MQPhase3==0):(MQPhase3==1)), ((CW == true)?(MQPhase3==0):(MQPhase3==1)));
  }
  byte MQPhase12 = (CurrentTime/(Speed/100*367))%5; //  5 steps
  if (Segment == 12) {    // Classic Attract version
    // Bank Shot lamps
    RPU_SetLampState(LA_ROLLOVER_BUTTON_2K, (MQPhase12==0)); 
    RPU_SetLampState(LA_ROLLOVER_BUTTON_4K, (MQPhase12==1));
    RPU_SetLampState(LA_ROLLOVER_BUTTON_6K, (MQPhase12==2));
    RPU_SetLampState(LA_ROLLOVER_BUTTON_8K, (MQPhase12==3));
    RPU_SetLampState(LA_ROLLOVER_BUTTON_10K, (MQPhase12==4));
  }
  
  byte MQPhase7 = (CurrentTime/(Speed*8/10))%4; //  x steps
  if ((Segment == 3) || (Segment == 10)) {
    // Kicker shot lamps - Trailing lamp is dim
    RPU_SetLampState(LA_KICKBACK_3K, ((CW == true)?(MQPhase7==0):(MQPhase7==3))||((CW == true)?(MQPhase7==1):(MQPhase7==0)), ((CW == true)?(MQPhase7==1):(MQPhase7==0))); 
    RPU_SetLampState(LA_KICKBACK_5K, ((CW == true)?(MQPhase7==1):(MQPhase7==2))||((CW == true)?(MQPhase7==2):(MQPhase7==3)), ((CW == true)?(MQPhase7==2):(MQPhase7==3)));
    RPU_SetLampState(LA_KICKBACK_7K, ((CW == true)?(MQPhase7==2):(MQPhase7==1))||((CW == true)?(MQPhase7==3):(MQPhase7==2)), ((CW == true)?(MQPhase7==3):(MQPhase7==2)));
    RPU_SetLampState(LA_KICKBACK_9K, ((CW == true)?(MQPhase7==3):(MQPhase7==0))||((CW == true)?(MQPhase7==0):(MQPhase7==1)), ((CW == true)?(MQPhase7==0):(MQPhase7==1)));
  }
  
  byte MQPhase11 = (CurrentTime/(Speed/100*367))%4; //  x steps
  if (Segment == 11) {  // Classic Attract version
    // Kicker shot lamps
    RPU_SetLampState(LA_KICKBACK_3K, (MQPhase11==0)); 
    RPU_SetLampState(LA_KICKBACK_5K, (MQPhase11==1));
    RPU_SetLampState(LA_KICKBACK_7K, (MQPhase11==2));
    RPU_SetLampState(LA_KICKBACK_9K, (MQPhase11==3));
  }


  byte MQPhase8 = (CurrentTime/(Speed*3/2))%4; //  4 steps
  if ((Segment == 4) || (Segment == 10)) {
    // Spa target lamps - Trailing lamp is dim
    RPU_SetLampState(LA_SPA_S, ((CW == true)?(MQPhase8==0):(MQPhase8==3))||((CW == true)?(MQPhase8==1):(MQPhase8==0)), ((CW == true)?(MQPhase8==1):(MQPhase8==0))); 
    RPU_SetLampState(LA_SPA_P, ((CW == true)?(MQPhase8==1):(MQPhase8==2))||((CW == true)?(MQPhase8==2):(MQPhase8==3)), ((CW == true)?(MQPhase8==2):(MQPhase8==3)));
    RPU_SetLampState(LA_SPA_A, ((CW == true)?(MQPhase8==2):(MQPhase8==1))||((CW == true)?(MQPhase8==3):(MQPhase8==2)), ((CW == true)?(MQPhase8==3):(MQPhase8==2)));
    RPU_SetLampState(LA_SPA_5K, ((CW == true)?(MQPhase8==3):(MQPhase8==0))||((CW == true)?(MQPhase8==0):(MQPhase8==1)), ((CW == true)?(MQPhase8==0):(MQPhase8==1)));
  }

  byte MQPhase9 = (CurrentTime/(Speed*3))%4; //  4 steps
  if ((Segment == 5) || (Segment == 10)) {
    // Drop bank lamps - Trailing lamp is dim
    RPU_SetLampState(LA_DROP_BANK_20K, ((CW == true)?(MQPhase9==0):(MQPhase9==3))||((CW == true)?(MQPhase9==1):(MQPhase9==0)), ((CW == true)?(MQPhase9==1):(MQPhase9==0))); 
    RPU_SetLampState(LA_DROP_BANK_SPECIAL, ((CW == true)?(MQPhase9==1):(MQPhase9==2))||((CW == true)?(MQPhase9==2):(MQPhase9==3)), ((CW == true)?(MQPhase9==2):(MQPhase9==3)));
    RPU_SetLampState(LA_DROP_BANK_SPOT_LETTER, ((CW == true)?(MQPhase9==2):(MQPhase9==1))||((CW == true)?(MQPhase9==3):(MQPhase9==2)), ((CW == true)?(MQPhase9==3):(MQPhase9==2)));
    RPU_SetLampState(LA_DROP_BANK_EXTRA_BALL, ((CW == true)?(MQPhase9==3):(MQPhase9==0))||((CW == true)?(MQPhase9==0):(MQPhase9==1)), ((CW == true)?(MQPhase9==0):(MQPhase9==1)));
  }

  byte MQPhase5 = (CurrentTime/(Speed*60/100))%10; //  10 steps
  if ((Segment == 6) || (Segment == 10)) {
    // Upper alley Marquee
    // Top Row
    RPU_SetLampState(LA_ARROW_LANE_U1, MQPhase5==0||MQPhase5==1||MQPhase5==2, MQPhase5==2);
    RPU_SetLampState(LA_ARROW_LANE_T,  MQPhase5==1||MQPhase5==2||MQPhase5==3, MQPhase5==3);
    RPU_SetLampState(LA_ARROW_LANE_U2, MQPhase5==2||MQPhase5==3||MQPhase5==4, MQPhase5==4);
    RPU_SetLampState(LA_ARROW_LANE_R,  MQPhase5==3||MQPhase5==4||MQPhase5==5, MQPhase5==5);
    // Bottom Row
    RPU_SetLampState(LA_FUTURE_E,      MQPhase5==4||MQPhase5==5||MQPhase5==6, MQPhase5==6);
    RPU_SetLampState(LA_FUTURE_R,      MQPhase5==5||MQPhase5==6||MQPhase5==7, MQPhase5==7);
    RPU_SetLampState(LA_FUTURE_U2,     MQPhase5==6||MQPhase5==7||MQPhase5==8, MQPhase5==8); 
    RPU_SetLampState(LA_FUTURE_T,      MQPhase5==7||MQPhase5==8||MQPhase5==9, MQPhase5==9);
    RPU_SetLampState(LA_FUTURE_U1,     MQPhase5==8||MQPhase5==9||MQPhase5==0, MQPhase5==0);
    RPU_SetLampState(LA_FUTURE_F,      MQPhase5==9||MQPhase5==0||MQPhase5==1, MQPhase5==1);
  }

  byte MQPhase13 = (CurrentTime/(Speed*60/100))%6; //  6 steps
  if (Segment == 13) { 
    // FUTURE lanes only
    RPU_SetLampState(LA_FUTURE_E,      MQPhase13==0||MQPhase13==1, MQPhase13==1);
    RPU_SetLampState(LA_FUTURE_R,      MQPhase13==1||MQPhase13==2, MQPhase13==2);
    RPU_SetLampState(LA_FUTURE_U2,     MQPhase13==2||MQPhase13==3, MQPhase13==3); 
    RPU_SetLampState(LA_FUTURE_T,      MQPhase13==3||MQPhase13==4, MQPhase13==4);
    RPU_SetLampState(LA_FUTURE_U1,     MQPhase13==4||MQPhase13==5, MQPhase13==5);
    RPU_SetLampState(LA_FUTURE_F,      MQPhase13==5||MQPhase13==0, MQPhase13==0);
  }


  if (Segment == 10) {
    // Remaining lamps
    RPU_SetLampState(LA_KICKBACK_SPOTS_LETTER, 1, 0, 125);
    RPU_SetLampState(LA_SPINNER_RIGHT, 1, 0, 250);
    RPU_SetLampState(LA_SPINNER_LEFT, 1, 0, 250);
  }

}

// Turns off selected play field lamps but excludes Aux board lamps

void RPU_TurnOffAttractPlayfieldLamps() {
  for (int count=0; count<60; count++) {
    if (count==40) {
      count = 42;
    } else if (count==43) {
      count = 44;
    } else if (count==48) {
      count = 52;
    }
    RPU_SetLampState(count, 0, 0, 0);
  }
}

void FUTURESPA(byte FutureSpaDelay) {

  // Light backglass FUTURE SPA sequence, 12 lamps
  // Sequential lamps
  if ((CurrentTime-FutureSpaStepTime) > FutureSpaDelay) { // Sets letter lamp spacing
    FutureSpaStepTime = CurrentTime;
/*    Serial.print(F("FutureSegment:              "));
    Serial.println(FutureSegment, DEC);
    Serial.print(F("FutureSpaStepLights:        "));
    Serial.println(FutureSpaStepLights, DEC);
    Serial.print(F("FutureSpaSingleLamps:       "));
    Serial.println(FutureSpaSingleLamps, DEC);
    Serial.println();*/
    if (FutureSegment == 1) {
      if (FutureSpaStepLights > 11) {  // Reset counter and turn off the lamps
        FutureSpaStepLights = 0;
        BlankFutureSpaLamps();
        if (FutureSpaSingleLamps) {
          FutureSpaSingleLamps = false;
        } else {
          FutureSpaSingleLamps = true;
          WordSteps = 0;
          FutureSegment = 2;
        }
      } else {
        if (FutureSpaSingleLamps) {   // if true, turn off trailing lamp
         RPU_SetLampState( (LA_FUTURE_SPA_1 + FutureSpaStepLights + ((FutureSpaStepLights==0)?0:-1)), 0 );
        }
        RPU_SetLampState((LA_FUTURE_SPA_1 + FutureSpaStepLights), 1);
        FutureSpaStepLights +=1;
      }
    } else if (FutureSegment == 2) {   // Segment 2
      // Serial.println(F("FutureSegment = 2 section "));
      // Alternate sets of lamps for classic attract animation
      if (WordToggle) {
        //BlankFutureSpaLamps();
        // Turn on FUTURE lamps and turn off SPA lamps
        for (byte count=0; count<8; count++) {
          RPU_SetLampState(FutureWord[count], 1);
        }
        for (byte count=0; count<4; count++) {
          RPU_SetLampState(SpaWord[count], 0);
        }
        WordToggle = false;
      } else {
        //BlankFutureSpaLamps();
        // Turn off FUTURE lamps and turn on SPA lamps
        for (byte count=0; count<8; count++) {
          RPU_SetLampState(FutureWord[count], 0);
        }
        // Turn on set 2 lamps and turn off set 1
        for (byte count=0; count<4; count++) {
          RPU_SetLampState(SpaWord[count], 1);
        }
        WordToggle = true;
      }
      WordSteps += 1;
      if (WordSteps > 13) {
        BlankFutureSpaLamps();
        WordSteps = 0;
        FutureSegment = 1;
      }
    } // end of Segment 2

    // Debug of auxilary lamp settings
/*    Serial.println(F("   76543210 - Lamp settings"));
    for (byte count=0; count<RPU_NUM_LAMP_BANKS; count++) {
      //byte Num = count;
      //Serial.println(Goals[CurrentPlayer], DEC);
      Serial.print(count, DEC);
      Serial.print(F("  "));
      Serial.print(CloneLampStates[count], BIN);
    }*/
/*
    Serial.print(F("FutureSegment        "));
    Serial.println(FutureSegment, DEC);
    Serial.print(F("FutureSpaSingleLamps "));
    Serial.println(FutureSpaSingleLamps, DEC);
    for (byte count=60; count<RPU_MAX_LAMPS; count++) {  // Lamps 60-88
      //Serial.print(count, DEC);
      Serial.print(F(" "));
      Serial.print(RPU_ReadLampState(count), DEC);  //xxxxx
    }
    Serial.println();
*/    
  } // end of lamp spacing
  
} // end of FUTURESPA

// Blank the backglass 12 FutureSpa lamps

void BlankFutureSpaLamps() {
      for (byte count=0; count<12; count++) {   // Turn off the FUTURESPA lamps
        RPU_SetLampState((LA_FUTURE_SPA_1+count), 0);
      }
}



//
//  ChimeScoring Ver 3 - Add count down ability, allow parallel function
//  ChimeScoring Ver 2 - Add CountdownBonus functionality
//  ChimeScoring Ver 1 - Increment scores at X Hz rather than jump in one change

void ChimeScoring() {
  byte ChimeDelay;
  if (MachineState == MACHINE_STATE_COUNTDOWN_BONUS) {
    ChimeDelay = 5;
  } else if ((abs(Ten_Pts_Stack) > 15) || (abs(Hundred_Pts_Stack) > 15) || (abs(Thousand_Pts_Stack) > 15)) {
    ChimeDelay = 40;
  } else if ((abs(Ten_Pts_Stack) > 8) || (abs(Hundred_Pts_Stack) > 8) || (abs(Thousand_Pts_Stack) > 8)) {
    ChimeDelay = 70;
  } else {
    ChimeDelay = 100;
  }

/*  if (ChimeScoringDelay == 0) {
    ChimeScoringDelay = CurrentTime;
  }*/

  if ( (CurrentTime - ChimeScoringDelay) > ChimeDelay ) {
    ChimeScoringDelay = CurrentTime;
    if (Ten_Pts_Stack > 0) {
      CurrentScores[CurrentPlayer] += 10;
      Ten_Pts_Stack -= 1;
    } else if (Ten_Pts_Stack < 0) {
      CurrentScores[CurrentPlayer] -= 10;
      Ten_Pts_Stack += 1;
    }
    if (Hundred_Pts_Stack > 0) {
      CurrentScores[CurrentPlayer] += 100;
      Hundred_Pts_Stack -= 1;
    } else if (Hundred_Pts_Stack < 0) {
      CurrentScores[CurrentPlayer] -= 100;
      Hundred_Pts_Stack += 1;
    }
    if (Thousand_Pts_Stack > 0) {
      CurrentScores[CurrentPlayer] += 1000;
      Thousand_Pts_Stack -= 1;
    } else if (Thousand_Pts_Stack < 0) {
      CurrentScores[CurrentPlayer] -= 1000;
      Thousand_Pts_Stack += 1;
    }

  }
}



//
// Match Mode - ShowMatchSequence
//

unsigned long MatchSequenceStartTime = 0;
unsigned long MatchDelay = 150;
//byte MatchDigit = 0;
byte NumMatchSpins = 0;
byte ScoreMatches = 0;

int ShowMatchSequence(boolean curStateChanged) {

  
  if (!MatchFeature) return MACHINE_STATE_ATTRACT;

  if (curStateChanged) {
    MatchSequenceStartTime = CurrentTime;
    MatchDelay = 4500;          // Wait this long after game end to start match mode
//    MatchDigit = random(0, 10);
//    MatchDigit = 1;    // Test value, force to be 1 (10)
    MatchDigit = CurrentTime%10;
    NumMatchSpins = 0;
    RPU_SetLampState(MATCH, 1, 0);
    RPU_SetDisableGate(true);
    RPU_SetDisableFlippers();
    ScoreMatches = 0;
  }

  if (NumMatchSpins < 40) {
    if ( (CurrentTime - MatchSequenceStartTime) > (MatchDelay) ) {
      MatchDigit += 1;
      if (MatchDigit > 9) MatchDigit = 0;
      //PlaySoundEffect(10+(MatchDigit%2));
//      RPU_PushToTimedSolenoidStack(SOL_CHIME_EXTRA, 1, 0, true);
      //RPU_PushToTimedSolenoidStack(SOL_CHIME_EXTRA, 3, 0, true);
      BIPDispValue = ((int)MatchDigit * 10);
      //RPU_SetDisplayBallInPlay((int)MatchDigit * 10);
      MatchDelay += 50 + 4 * NumMatchSpins;
      NumMatchSpins += 1;
      RPU_SetLampState(MATCH, NumMatchSpins % 2, 0);

      if (NumMatchSpins == 40) {
        RPU_SetLampState(MATCH, 0);
        MatchDelay = CurrentTime - MatchSequenceStartTime;
      }
    }
  }

  if (NumMatchSpins >= 40 && NumMatchSpins <= 43) {
    if (CurrentTime > (MatchSequenceStartTime + MatchDelay)) {
      if ( (CurrentNumPlayers > (NumMatchSpins - 40)) && ((CurrentScores[NumMatchSpins - 40] / 10) % 10) == MatchDigit) {
        ScoreMatches |= (1 << (NumMatchSpins - 40));
        AddSpecialCredit();
        MatchDelay += 1000;
        NumMatchSpins += 1;
        RPU_SetLampState(MATCH, 1);
      } else {
        NumMatchSpins += 1;
      }
      if (NumMatchSpins == 44) {
        MatchDelay += 5000;
      }
    }
  }

  if (NumMatchSpins > 43) {
    RPU_SetLampState(MATCH, 1);
    if (CurrentTime > (MatchSequenceStartTime + MatchDelay)) {
      return MACHINE_STATE_ATTRACT;
    }
  }

  for (int count = 0; count < 4; count++) {
    if ((ScoreMatches >> count) & 0x01) {
      // If this score matches, we're going to flash the last two digits
      if ( (CurrentTime / 200) % 2 ) {
        RPU_SetDisplayBlank(count, RPU_GetDisplayBlank(count) & 0x0F);
      } else {
        RPU_SetDisplayBlank(count, RPU_GetDisplayBlank(count) | 0x30);
      }
    }
  }

  return MACHINE_STATE_MATCH_MODE;
}
