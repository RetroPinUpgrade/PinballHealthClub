// These are example values - fill these in with your machine's definitions

#define NUMBER_OF_LAMPS        60

// Lamp Numbers (defines for lamps)

#define LA_SPA_S                           0 //
#define LA_SPA_P                           1 // 
#define LA_SPA_A                           2 // 
#define LA_FUTURE_F                        3 // 
#define LA_FUTURE_U1                       4 // 
#define LA_FUTURE_T                        5 // 
#define LA_FUTURE_U2                       6 // 
#define LA_FUTURE_R                        7 // 
#define LA_FUTURE_E                        8 // 
#define LA_18K_BONUS                       9 // 
#define LA_36K_BONUS                      10 // 
#define LA_SPA_SPECIAL                    11 // 
#define LA_BIG_S                          12 // 
#define LA_BIG_P                          13 // 
#define LA_BIG_A                          14 // 
#define LA_BIG_F                          15 // 
#define LA_BIG_U1                         16 // 
#define LA_BIG_T                          17 // 
#define LA_BIG_U2                         18 // 
#define LA_BIG_R                          19 // 
#define LA_BIG_E                          20 // 
#define LA_SPINNER_RIGHT                  21 // 
#define LA_SPINNER_LEFT                   22 // 
#define LA_GATE_ARROW                     23 // 
#define LA_KICKBACK_3K                    24 // 
#define LA_KICKBACK_5K                    25 // 
#define LA_KICKBACK_7K                    26 // 
#define LA_KICKBACK_9K                    27 // 
#define LA_SAUCER_3K                      28 // 
#define LA_SAUCER_6K                      29 // 
#define LA_SAUCER_9K                      30 // 
#define LA_BUMPER_BOT_LEFT                31 // 
#define LA_ARROW_LANE_R                   32 // 
#define LA_ARROW_LANE_U2                  33 // 
#define LA_ARROW_LANE_T                   34 // 
#define LA_ARROW_LANE_U1                  35 // 
#define LA_DROP_BANK_20K                  36 // 
#define LA_DROP_BANK_EXTRA_BALL           37 // 
#define LA_DROP_BANK_SPECIAL              38 // 
#define LA_SPA_5K                         39 // 
#define LA_SPSA_BACK                      40 // Same Player Shoots Again - Back Box
#define MATCH                             41 // Match
#define LA_SAME_PLAYER                    42 // Same Player Shoots Again - Playfield
#define LA_CREDIT_INDICATOR               43 // 
#define LA_2X                             44 // 
#define LA_4X                             45 // 
#define LA_6X                             46 // 
#define LA_KICKBACK_SPOTS_LETTER          47 // 
#define BALL_IN_PLAY                      48 // Ball in Play
#define HIGH_SCORE                        49 // High Score
#define GAME_OVER                         50 // Game Over
#define TILT                              51 // Tilt
#define LA_ROLLOVER_BUTTON_2K             52 // 
#define LA_ROLLOVER_BUTTON_4K             53 // 
#define LA_ROLLOVER_BUTTON_6K             54 // 
#define LA_ROLLOVER_BUTTON_8K             55 // 
#define LA_ROLLOVER_BUTTON_10K            56 // 
#define NOT_USED1                         57 // Not used
#define NOT_USED2                         58 // 
#define LA_DROP_BANK_SPOT_LETTER          59 // 

// Aux Lamp Board

#define LA_FUTURE_SPA_1                   60 // 
#define LA_FUTURE_SPA_2                   61 // 
#define LA_FUTURE_SPA_3                   62 // 
#define LA_FUTURE_SPA_4                   63 // 
#define LA_FUTURE_SPA_5                   64 // 
#define LA_FUTURE_SPA_6                   65 // 
#define LA_FUTURE_SPA_7                   66 // 
#define LA_FUTURE_SPA_8                   67 // 
#define LA_FUTURE_SPA_9                   68 // 
#define LA_FUTURE_SPA_10                  69 // 
#define LA_FUTURE_SPA_11                  70 // 
#define LA_FUTURE_SPA_12                  71 // 


// Attract Animation Definitions

// Classic Attract animation defintion

byte Classic1[14]={
  LA_FUTURE_F,
  LA_FUTURE_U1,
  LA_FUTURE_T,
  LA_FUTURE_U2,
  LA_FUTURE_R,
  LA_FUTURE_E,
  LA_ARROW_LANE_T,
  LA_ARROW_LANE_U2,
  LA_SAUCER_3K,
  LA_SAUCER_9K,
  LA_DROP_BANK_20K,
  LA_DROP_BANK_SPECIAL,
  LA_2X,
  LA_6X
  };

byte Classic2[10]={
  LA_ARROW_LANE_U1,
  LA_ARROW_LANE_R,
  LA_SAUCER_6K,
  LA_SPA_S,
  LA_SPA_P,
  LA_SPA_A,
  LA_SPA_5K,
  LA_4X,
  LA_DROP_BANK_EXTRA_BALL,
  LA_BUMPER_BOT_LEFT
  };

// FUTURESPA backglass animations

byte FutureWord[8]={
  LA_FUTURE_SPA_1,
  LA_FUTURE_SPA_2,
  LA_FUTURE_SPA_4,
  LA_FUTURE_SPA_6,
  LA_FUTURE_SPA_7,
  LA_FUTURE_SPA_9,
  LA_FUTURE_SPA_10,
  LA_FUTURE_SPA_12,
  };

byte SpaWord[4]={
  LA_FUTURE_SPA_3,
  LA_FUTURE_SPA_5,
  LA_FUTURE_SPA_8,
  LA_FUTURE_SPA_11,
  };



// Spiral Attract animation definition

#define NUM_OF_TRIANGLE_LAMPS_CW 47
struct AttractLampsDown {
  byte lightNumDown;
  byte rowDown;
};
struct AttractLampsDown AttractLampsDown[] = {
    {LA_FUTURE_F,               1},
    {LA_FUTURE_U1,              2},
    {LA_ARROW_LANE_U1,          2},
    {LA_FUTURE_T,               3},
    {LA_ARROW_LANE_T,           3},
    {LA_FUTURE_U2,              4},
    {LA_ARROW_LANE_U2,          4},
    {LA_FUTURE_R,               5},
    {LA_ARROW_LANE_R,           5},
    {LA_FUTURE_E,               6},
    {LA_SAUCER_3K,              7},
    {LA_SAUCER_6K,              8},
    {LA_SAUCER_9K,              9},
    {LA_ROLLOVER_BUTTON_10K,   10},
    {LA_ROLLOVER_BUTTON_8K,    11},
    {LA_ROLLOVER_BUTTON_6K,    12},
    {LA_ROLLOVER_BUTTON_4K,    13},
    {LA_ROLLOVER_BUTTON_2K,    14},
    {LA_BIG_E,                 15},
    {LA_BIG_R,                 16},
    {LA_BIG_U2,                17},
    {LA_BIG_T,                 18},
    {LA_BIG_U1,                19},
    {LA_BIG_F,                 20},
    {LA_BIG_S,                 21},
    {LA_BIG_P,                 22},
    {LA_BIG_A,                 23},
    {LA_18K_BONUS,             24},
    {LA_36K_BONUS,             25},
    {LA_SPA_SPECIAL,           26},

    {LA_SPA_A,                 12},
    {LA_SPA_P,                 13},
    {LA_SPA_5K,                13},
    {LA_SPA_S,                 14},
    {LA_DROP_BANK_SPECIAL,     15},
    {LA_DROP_BANK_20K,         16},
    {LA_DROP_BANK_SPOT_LETTER, 16},
    {LA_DROP_BANK_EXTRA_BALL,  17},
    {LA_KICKBACK_SPOTS_LETTER, 18},
    {LA_KICKBACK_9K,           19},
    {LA_KICKBACK_7K,           20},
    {LA_KICKBACK_5K,           21},
    {LA_KICKBACK_3K,           22},
    {LA_GATE_ARROW,            23},
    {LA_2X,                    24},
    {LA_4X,                    25},
    {LA_6X,                    26},
};

//
// Rain down effect, 52 lamps, 24 frames
//
struct AttractLampsDown FlowDown[] = {
    {LA_ARROW_LANE_U1,          1},
    {LA_ARROW_LANE_T,           1},
    {LA_ARROW_LANE_U2,          1},
    {LA_ARROW_LANE_R,           1},
    {LA_FUTURE_F,               2},
    {LA_FUTURE_U1,              2},
    {LA_FUTURE_T,               2},
    {LA_FUTURE_U2,              2},
    {LA_FUTURE_R,               2},
    {LA_FUTURE_E,               2},
    {LA_SAUCER_3K,              3},
    {LA_SAUCER_6K,              4},
    {LA_SAUCER_9K,              5},
    {LA_ROLLOVER_BUTTON_10K,    6},
    {LA_ROLLOVER_BUTTON_8K,     7},
    {LA_ROLLOVER_BUTTON_6K,     8},
    {LA_ROLLOVER_BUTTON_4K,     9},
    {LA_ROLLOVER_BUTTON_2K,    10},
    {LA_SPINNER_LEFT,          11},
    {LA_DROP_BANK_20K,         12},
    {LA_SPA_S,                 12},
    {LA_SPA_P,                 12},
    {LA_SPA_A,                 12},
    {LA_SPINNER_RIGHT,         12},
    {LA_KICKBACK_SPOTS_LETTER, 13},
    {LA_DROP_BANK_EXTRA_BALL,  13},
    {LA_DROP_BANK_SPECIAL,     13},
    {LA_SPA_5K,                13},
    {LA_KICKBACK_9K,           14},
    {LA_DROP_BANK_SPOT_LETTER, 14},
    {LA_BIG_E,                 14},
    {LA_KICKBACK_7K,           15},
    {LA_BIG_R,                 15},
    {LA_SPA_SPECIAL,           15},
    {LA_BIG_U2,                16},
    {LA_36K_BONUS,             16},
    {LA_KICKBACK_5K,           17},
    {LA_BIG_T,                 17},
    {LA_18K_BONUS,             17},
    {LA_KICKBACK_3K,           18},
    {LA_GATE_ARROW,            18},
    {LA_BIG_U1,                18},
    {LA_BIG_A,                 18},
    {LA_BIG_F,                 19},
    {LA_BIG_P,                 19},
    {LA_BIG_S,                 20},
    {NOT_USED1,                21},
    {LA_2X,                    22},
    {LA_4X,                    22},
    {LA_6X,                    22},
    {NOT_USED2,                23},
    {LA_SAME_PLAYER,           24},
};

//
// Light ShootAll lamps in order, 22 lamps, 22 frames
//
struct AttractLampsDown ShootAllStartLamps[] = {
    {LA_FUTURE_F,               1},
    {LA_FUTURE_U1,              2},
    {LA_FUTURE_T,               3},
    {LA_FUTURE_U2,              4},
    {LA_FUTURE_R,               5},
    {LA_FUTURE_E,               6},
    {LA_SAUCER_3K,              7},
    {LA_SAUCER_6K,              8},
    {LA_SAUCER_9K,              9},
    {LA_ROLLOVER_BUTTON_10K,   10},
    {LA_ROLLOVER_BUTTON_8K,    11},
    {LA_ROLLOVER_BUTTON_6K,    12},
    {LA_ROLLOVER_BUTTON_4K,    13},
    {LA_ROLLOVER_BUTTON_2K,    14},
    {LA_SPA_A,                 15},
    {LA_SPA_P,                 16},
    {LA_SPA_S,                 17},
    {LA_DROP_BANK_20K,         18},
    {LA_KICKBACK_9K,           19},
    {LA_KICKBACK_7K,           20},
    {LA_KICKBACK_5K,           21},
    {LA_KICKBACK_3K,           22},
};


//
// Flow out from saucer effect, 50 lamps, 18 frames
//
struct AttractLampsDown SaucerFlowOut[] = {
    {LA_SAUCER_3K,              1},
    {LA_SAUCER_6K,              1},
    {LA_SAUCER_9K,              1},
    {LA_ROLLOVER_BUTTON_10K,    1},
    {LA_FUTURE_E,               1},
    {LA_ROLLOVER_BUTTON_8K,     2},
    {LA_FUTURE_R,               2},
    {LA_ROLLOVER_BUTTON_6K,     3},
    {LA_FUTURE_U2,              3},
    {LA_ARROW_LANE_U2,          3},
    {LA_ROLLOVER_BUTTON_4K,     4},
    {LA_FUTURE_T,               4},
    {LA_ARROW_LANE_T,           4},
    {LA_ROLLOVER_BUTTON_2K,     5},
    {LA_FUTURE_U1,              5},
    {LA_ARROW_LANE_U1,          5},
    {LA_SPINNER_RIGHT,          6},
    {LA_FUTURE_F,               6},
    {LA_SPA_A,                  7},
    {LA_BIG_E,                  7},
    {LA_SPA_P,                  8},
    {LA_SPA_SPECIAL,            8},
    {LA_SPA_S,                  9},
    {LA_SPA_5K,                 9},
    {LA_BIG_R,                  9},
    {LA_36K_BONUS,              9},
    {LA_DROP_BANK_20K,         10},
    {LA_DROP_BANK_SPECIAL,     10},
    {LA_BIG_U2,                10},
    {LA_18K_BONUS,             10},
    {LA_DROP_BANK_EXTRA_BALL,  11},
    {LA_DROP_BANK_SPOT_LETTER, 11},
    {LA_BIG_T,                 11},
    {LA_BIG_A,                 11},
    {LA_SPINNER_LEFT,          12},
    {LA_BIG_U1,                12},
    {LA_BIG_P,                 12},
    {LA_KICKBACK_SPOTS_LETTER, 13},
    {LA_BIG_F,                 13},
    {LA_BIG_S,                 13},
    {LA_KICKBACK_9K,           14},
    {LA_6X,                    14},
    {LA_KICKBACK_7K,           15},
    {LA_4X,                    15},
    {LA_KICKBACK_5K,           16},
    {LA_2X,                    16},
    {LA_GATE_ARROW,            16},
    {LA_KICKBACK_3K,           17},
    {LA_SAME_PLAYER,           18},
};

//
// FUTURE lamps toggling on/off, "7" lamps, 2 frames
// inlcudes fake lamp
struct AttractLampsDown FutureToggle[] = {
    {NOT_USED1,                 1},
    {LA_FUTURE_F,               2},
    {LA_FUTURE_U1,              2},
    {LA_FUTURE_T,               2},
    {LA_FUTURE_U2,              2},
    {LA_FUTURE_R,               2},
    {LA_FUTURE_E,               2},
};

//
// Kicker lamps toggling on/off, "5" lamps, 2 frames
// inlcudes fake lamp
struct AttractLampsDown KickerToggle[] = {
    {NOT_USED1,                 1},
    {LA_KICKBACK_3K,            2},
    {LA_KICKBACK_5K,            2},
    {LA_KICKBACK_7K,            2},
    {LA_KICKBACK_9K,            2},
};

//
// Saucer lamps toggling on/off, "4" lamps, 2 frames
// inlcudes fake lamp
struct AttractLampsDown SaucerToggle[] = {
    {NOT_USED1,                 1},
    {LA_SAUCER_3K,              2},
    {LA_SAUCER_6K,              2},
    {LA_SAUCER_9K,              2},
};

//
// RollOver lamps toggling on/off, "6" lamps, 2 frames
// inlcudes fake lamp
struct AttractLampsDown RolloverToggle[] = {
    {NOT_USED1,                 1},
    {LA_ROLLOVER_BUTTON_2K,     2},
    {LA_ROLLOVER_BUTTON_4K,     2},
    {LA_ROLLOVER_BUTTON_6K,     2},
    {LA_ROLLOVER_BUTTON_8K,     2},
    {LA_ROLLOVER_BUTTON_10K,    2},
};

// Mode 3 wrap-up animation 13 frames plus 3 blank
byte RoamingStage3Lamps[16][8] = {
{ 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  }
};

// Beginning of Mode 3 animation 6 frames plus 6 blank
byte RoamingStage1Lamps[12][8] = {
{ 0x08, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x10, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x21, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x42, 0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x84, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  }
};


byte LampAnimation1[18][8] = {
  // Test of middle lamps
{ 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x02, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x04, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  }
};

byte LampAnimation2[16][8] = {
  //  Circular around SPA5K
{ 0xC0, 0x01, 0x00, 0x00, 0x83, 0x00, 0x00, 0x00  },
{ 0x04, 0x00, 0x00, 0x70, 0x80, 0x00, 0x80, 0x01  },
{ 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x70, 0x00  },
{ 0x00, 0x00, 0x20, 0x00, 0x80, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00  },
{ 0x00, 0x08, 0x10, 0x00, 0x80, 0x00, 0x00, 0x00  },
{ 0x00, 0x06, 0x0C, 0x80, 0x80, 0x00, 0x00, 0x00  },
{ 0x00, 0x70, 0x02, 0x00, 0x80, 0x40, 0x00, 0x00  },
{ 0x00, 0x80, 0x01, 0x00, 0x80, 0x34, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x80, 0x03, 0x80, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x0C, 0xA0, 0x80, 0x00, 0x08  },
{ 0x00, 0x00, 0x40, 0x00, 0xD0, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00  },
{ 0x01, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00  },
{ 0x3A, 0x00, 0x00, 0x00, 0x8C, 0x00, 0x00, 0x00  }
};

byte LampAnimation3[16][8] = {
  // RADAR sweep around SPSA lamp
{ 0x00, 0x00, 0x80, 0x03, 0x00, 0x04, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x0C, 0x00, 0x04, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00  },
{ 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x30, 0x04, 0x00, 0x08  },
{ 0x08, 0x00, 0x00, 0x00, 0x40, 0x04, 0x00, 0x00  },
{ 0x31, 0x80, 0x00, 0x00, 0x0C, 0x04, 0x00, 0x00  },
{ 0x42, 0x00, 0x00, 0x00, 0x82, 0x24, 0x00, 0x00  },
{ 0x84, 0x01, 0x01, 0x00, 0x01, 0x04, 0x00, 0x00  },
{ 0x00, 0x10, 0x02, 0x70, 0x00, 0x04, 0x00, 0x00  },
{ 0x00, 0x20, 0x04, 0x00, 0x00, 0x04, 0xF0, 0x01  },
{ 0x00, 0x40, 0x38, 0x00, 0x00, 0x44, 0x00, 0x00  },
{ 0x00, 0x0E, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00  }
};


byte DrainDown[18][8] = { // Drain down
{ 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00  },
{ 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00  },
{ 0x07, 0x00, 0x60, 0x00, 0x10, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0xE0, 0x80, 0x00, 0x00  },
{ 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x00, 0x08  },
{ 0x00, 0x0C, 0x0C, 0x04, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x42, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0xB0, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00  }
};

byte SaucerInward[13][8] = { // Saucer inward
{ 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x01  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00  },
{ 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00  },
{ 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00  },
{ 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00  },
{ 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x16, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x09, 0x00, 0x10, 0x00, 0xC0, 0x00, 0x00, 0x00  },
{ 0x00, 0x0C, 0x0C, 0x00, 0x20, 0x00, 0x00, 0x08  },
{ 0x00, 0x40, 0x43, 0x80, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0xB0, 0x00, 0x0E, 0x00, 0x80, 0x00, 0x00  },
{ 0x00, 0x00, 0x80, 0x01, 0x00, 0x70, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00  },
};

byte SaucerPreShootAll[2][8] = { // Saucer arrows outer then inner
{ 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00  },
};

byte SaucerPreShootAll2[2][8] = { // Saucer arrows all flashing
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
{ 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00 },
};

byte SaucerPreShootAll3[6][8] = { // Saucer arrows fill in
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  },
};



byte AuxLeftRight[8][2] = {  // FUTURE SPA backglass left right sweep
{ 0x03, 0x00, },
{ 0x04, 0x00, },
{ 0x18, 0x00, },
{ 0xA0, 0x00, },
{ 0x40, 0x00, },
{ 0x00, 0x05, },
{ 0x00, 0x02, },
{ 0x00, 0x08, },
};

byte AuxUpDown[6][2] = {     // FUTURE SPA backglass up down sweep
{ 0x21, 0x00, },
{ 0x08, 0x0B, },
{ 0x40, 0x00, },
{ 0x04, 0x00, },
{ 0x80, 0x04, },
{ 0x10, 0x00, }
};

byte AuxDiagonal[6][2] = {  // FUTURE SPA backglass diagonal sweep
{ 0x01, 0x00, },
{ 0x2A, 0x00, },
{ 0x04, 0x00, },
{ 0x50, 0x01, },
{ 0x80, 0x02, },
{ 0x00, 0x0C, },
};



#define NUM_OF_SWITCHES     34

// Defines for switches

#define SW_DROP_FIRST            0
#define SW_DROP_2X               1
#define SW_DROP_4X               2
#define SW_DROP_6X               3
#define SW_FUTURITY_TARGET       4
#define SW_CREDIT_RESET          5
#define SW_TILT                  6  // Tilt bob and right lower PF switch
#define SW_OUTHOLE               7
#define SW_COIN_3                8
#define SW_COIN_1                9
#define SW_COIN_2                10
#define SW_ROLLOVER_BUTTON       11
#define SW_30_PTS                12
#define SW_SPINNER_RT            13
#define SW_SPINNER_LT            14
#define SW_SLAM                  15 // Neither is working
#define SW_LANE_E                16
#define SW_LANE_R                17
#define SW_LANE_U2               18
#define SW_LANE_T                19
#define SW_LANE_U1               20
#define SW_LANE_F                21
#define SW_SPA_TARGET            22 
#define SW_SAUCER                23
#define SW_LEFT_INLANE           24
#define SW_GATE_LANE             25
#define SW_LEFT_OUTLANE          26
#define SW_LEFT_KICKBACK         27
#define SW_LEFT_SLING            34
#define SW_BUMPER_BOT_RIGHT      35
#define SW_BUMPER_BOT_LEFT       36
#define SW_BUMPER_TOP_CENTER     37
#define SW_BUMPER_TOP_RIGHT      38
#define SW_BUMPER_TOP_LEFT       39


// Defines for solenoids

#define SOL_DROP_RESET         1
#define SOL_KNOCKER            5
#define SOL_OUTHOLE            6
#define SOL_SAUCER             7
#define SOL_KICKER             8
#define SOL_BUMPER_TOP_LEFT    9
#define SOL_BUMPER_TOP_RIGHT  10
#define SOL_BUMPER_TOP_CENTER 11
#define SOL_BUMPER_BOT_LEFT   12
#define SOL_BUMPER_BOT_RIGHT  13
#define SOL_LEFT_SLING        14



// 
// Leave Coin Lockout and and flippers out of the defintions
//
// #define SOL_COIN_LOCKOUT     12
// #define SOL_FLIPPERS         13
//             GATE             14

// SWITCHES_WITH_TRIGGERS are for switches that will automatically
// activate a solenoid (like in the case of a chime that rings on a rollover)
// but SWITCHES_WITH_TRIGGERS are fully debounced before being activated
#define NUM_SWITCHES_WITH_TRIGGERS        6

// PRIORITY_SWITCHES_WITH_TRIGGERS are switches that trigger immediately
// (like for pop bumpers or slings) - they are not debounced completely
#define NUM_PRIORITY_SWITCHES_WITH_TRIGGERS 5

// Define automatic solenoid triggers (switch, solenoid, number of 1/120ths of a second to fire)
// Put chime call before the solenoid call, seems to do things in reverse order

struct PlayfieldAndCabinetSwitch TriggeredSwitches[] = {
  { SW_LEFT_SLING,        SOL_LEFT_SLING,        4 },
  { SW_BUMPER_TOP_LEFT,   SOL_BUMPER_TOP_LEFT,   3 },
  { SW_BUMPER_TOP_CENTER, SOL_BUMPER_TOP_CENTER, 3 },
  { SW_BUMPER_TOP_RIGHT,  SOL_BUMPER_TOP_RIGHT,  3 },
  { SW_BUMPER_BOT_LEFT,   SOL_BUMPER_BOT_LEFT,   3 },
  { SW_BUMPER_BOT_RIGHT,  SOL_BUMPER_BOT_RIGHT,  3 },
};
