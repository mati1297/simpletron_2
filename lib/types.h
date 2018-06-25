#ifndef TYPES__H
#define TYPES__H 1

typedef enum retval {
  RV_SUCCESS = 0,
  RV_ILLEGAL = 1,
  RV_NOSPACE = 2,
  RV_ERROR = 3,
  RV_NOTIMPLEMENTED = 4,
  RV_MISSING_ARGS = 5
} retval_t;

typedef enum bool {
  false = 0,
  true = 1
} bool_t;

#endif
