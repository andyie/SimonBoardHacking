#include "Arduino.h"
#include <string.h>

#define DOT_LENGTH_MILLIS 100
#define DASH_LENGTH_MILLIS 300
#define INTER_MARK_DELAY_MILLIS 100
#define INTER_CHAR_DELAY_MILLIS 300
#define SPACE_DELAY_MILLIS 600

#define DIM(array) (sizeof(array) / sizeof((array)[0]))

extern void whatToDoOnStartDot();
extern void whatToDoOnEndDot();
extern void whatToDoOnStartDash();
extern void whatToDoOnEndDash();

enum morse_mark_t {
  DOT,
  DASH
};

struct morse_char_t {
  morse_mark_t *marks;
  size_t len;
};

// Character definitions.
static morse_mark_t morse_A[] = { DOT, DASH };
static morse_mark_t morse_B[] = { DASH, DOT, DOT, DOT };
static morse_mark_t morse_C[] = { DASH, DOT, DASH, DOT };
static morse_mark_t morse_D[] = { DASH, DOT, DOT };
static morse_mark_t morse_E[] = { DOT };
static morse_mark_t morse_F[] = { DOT, DOT, DASH, DOT };
static morse_mark_t morse_G[] = { DASH, DASH, DOT };
static morse_mark_t morse_H[] = { DOT, DOT, DOT, DOT };
static morse_mark_t morse_I[] = { DOT, DOT };
static morse_mark_t morse_J[] = { DOT, DASH, DASH, DASH };
static morse_mark_t morse_K[] = { DASH, DOT, DASH };
static morse_mark_t morse_L[] = { DOT, DASH, DOT, DOT };
static morse_mark_t morse_M[] = { DASH, DASH };
static morse_mark_t morse_N[] = { DASH, DOT };
static morse_mark_t morse_O[] = { DASH, DASH, DASH };
static morse_mark_t morse_P[] = { DOT, DASH, DASH, DOT };
static morse_mark_t morse_Q[] = { DASH, DASH, DOT, DASH };
static morse_mark_t morse_R[] = { DOT, DASH, DOT };
static morse_mark_t morse_S[] = { DOT, DOT, DOT };
static morse_mark_t morse_T[] = { DASH };
static morse_mark_t morse_U[] = { DOT, DOT, DASH };
static morse_mark_t morse_V[] = { DOT, DOT, DOT, DASH };
static morse_mark_t morse_W[] = { DOT, DASH, DASH };
static morse_mark_t morse_X[] = { DASH, DOT, DOT, DASH };
static morse_mark_t morse_Y[] = { DASH, DOT, DASH, DASH };
static morse_mark_t morse_Z[] = { DASH, DASH, DOT, DOT };

// Character lookup table.
static morse_char_t morse_lookup[] = {
  { morse_A, DIM(morse_A) },
  { morse_B, DIM(morse_B) },
  { morse_C, DIM(morse_C) },
  { morse_D, DIM(morse_D) },
  { morse_E, DIM(morse_E) },
  { morse_F, DIM(morse_F) },
  { morse_G, DIM(morse_G) },
  { morse_H, DIM(morse_H) },
  { morse_I, DIM(morse_I) },
  { morse_J, DIM(morse_J) },
  { morse_K, DIM(morse_K) },
  { morse_L, DIM(morse_L) },
  { morse_M, DIM(morse_M) },
  { morse_N, DIM(morse_N) },
  { morse_O, DIM(morse_O) },
  { morse_P, DIM(morse_P) },
  { morse_Q, DIM(morse_Q) },
  { morse_R, DIM(morse_R) },
  { morse_S, DIM(morse_S) },
  { morse_T, DIM(morse_T) },
  { morse_U, DIM(morse_U) },
  { morse_V, DIM(morse_V) },
  { morse_W, DIM(morse_W) },
  { morse_X, DIM(morse_X) },
  { morse_Y, DIM(morse_Y) },
  { morse_Z, DIM(morse_Z) }
};

static char to_upper(char c);
static bool morsable_char(char c);
void morse(const char * const str);
void play_morse_char(morse_char_t &c);

static char to_upper(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 0x20;
  }
  return c;
}

static bool morsable_char(char c)
{
  return c >= 'A' && (size_t)(c - 'A') < DIM(morse_lookup);
}

void morse(const char * const str) {
  for (size_t i = 0; i < strlen(str); ++i)
  {
    char c = to_upper(str[i]);

    // Note: Invalid characters are ignored.
    if (c == ' ')
    {
      delay(SPACE_DELAY_MILLIS);
    }
    else if (morsable_char(c))
    {
      play_morse_char(morse_lookup[c - 'A']);
    }

    if (i != strlen(str) - 1) {
      delay(INTER_CHAR_DELAY_MILLIS);
    }
  }
}

void play_morse_char(morse_char_t &morse_char) {
  for (unsigned int i = 0; i < morse_char.len; ++i)
  {
    switch (morse_char.marks[i])
    {
      case DOT:
        whatToDoOnStartDot();
        delay(DOT_LENGTH_MILLIS);
        whatToDoOnEndDot();
        break;

      case DASH:
        whatToDoOnStartDash();
        delay(DASH_LENGTH_MILLIS);
        whatToDoOnEndDash();
        break;
    }

    delay(INTER_MARK_DELAY_MILLIS);
  }
}
