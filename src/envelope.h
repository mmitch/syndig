#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_

typedef void  (*envelope_trigger_fn)(void);
typedef void  (*envelope_release_fn)(void);
typedef float (*envelope_nextval_fn)(void);

typedef const struct {
	envelope_trigger_fn trigger;
	envelope_release_fn release;
	envelope_nextval_fn nextval;
} envelope;

envelope* get_envelope();

#endif
