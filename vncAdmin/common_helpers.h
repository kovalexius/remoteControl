#ifndef __COMMON_HELPERS__H
#define __COMMON_HELPERS__H

#include <string>
#include <SDL.h>
#include <rfb/rfbclient.h>

struct ButtonMapping 
{
	int sdl;
	int rfb;
};

struct Utf8Mapping 
{
	char mask;
	int bits_stored;
};


extern ButtonMapping buttonMapping[];
extern Utf8Mapping utf8Mapping[];

rfbKeySym SDL_key2rfbKeySym(const SDL_KeyboardEvent* e);

/* UTF-8 decoding is from https://rosettacode.org/wiki/UTF-8_encode_and_decode which is under GFDL 1.2 */
rfbKeySym utf8char2rfbKeySym(const std::string& _chr);

void kbd_leds(rfbClient* cl, int value, int pad);

void cleanup(rfbClient* cl);

/* trivial support for textchat */
void text_chat(rfbClient* cl, int value, char *text);

void got_selection(rfbClient *cl, const char *text, int len);

rfbCredential* get_credential(rfbClient* cl, int credentialType);

#endif