#pragma once
#define WF_WORTER_H

// #define DEBUG

#define DATEFONTSMALLSIZE 24
#define DATEFONTSMALL RESOURCE_ID_FONT_DIGITAL_24
#define DATEFONTMIDDLESIZE 32
#define DATEFONTMIDDLE RESOURCE_ID_FONT_DIGITAL_32
#define DATEFONTBIGSIZE 40
#define DATEFONTBIG RESOURCE_ID_FONT_DIGITAL_40

#define LEFTSPACE 2
#define RIGHTSPACE 2
#define TEXTLAYERWIDTH (144 - LEFTSPACE - RIGHTSPACE)
#define TOPSPACE 0
#define TEXTLAYERSMALLVSPACE 6
#define TEXTLAYERMIDDLEVSPACE 6
#define TEXTLAYERBIGVSPACE 6

// font size and spacing
#define FONT_MINUTE_SIZE DATEFONTMIDDLESIZE
#define TEXTLAYER_MINUTE_VSPACE TEXTLAYERMIDDLEVSPACE
#define FONT_QUALIFIER_SIZE DATEFONTSMALLSIZE
#define TEXTLAYER_QUALIFIER_VSPACE 0
#define FONT_HOUR_SIZE DATEFONTBIGSIZE
#define TEXTLAYER_HOUR_VSPACE TEXTLAYERBIGVSPACE
#define FONT_DAYTIME_SIZE DATEFONTSMALLSIZE
#define TEXTLAYER_DAYTIME_VSPACE TEXTLAYERSMALLVSPACE


// static const char *DAYS[] = { "So", "Mo", "Di", "Mi", "Do", "Fr", "Sa" };
static const char *NUMBERS[] = { "null", "eins", "zwei", "drei", "vier", "fünf", "sechs", "sieben", "acht",
                                 "neun", "zehn", "elf", "zwölf", "dreizehn", "vierzehn", "fünfzehn",
                                 "sechzehn", "siebzehn", "achtzehn", "neunzehn", "zwanzig" };
static const char *QUALIFIER[] = { "", "vor", "nach" };
static const int BEFORE = 1;
static const int AFTER = 2;
static const char *HOURQUALIFIER[] = { "", "viertel", "halb", "dreiviertel" };
static const int QUARTER = 1;
static const int HALF = 2;
static const int THREEFOURTH = 3;
static const char *EMPTY = "";
static const char *DAYTIME[] = { "nachts", "vormittags", "nachmittags", "abends" };
static const char *DAYTIME_MIDDAY = "Mittag";
static const char *DAYTIME_MIDNIGHT1 = "Mitter-";
static const char *DAYTIME_MIDNIGHT2 = "nacht";

