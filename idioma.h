#ifndef IDIOMA__H
#define IDIOMA__H 1

#if defined (IDIOMA_ESPANIOL)
#include "errores_esp.h"
#include "mensajes_esp.h"

#elif defined (LANG_ENGLISH)
#include "errors_eng.h"
#include "messages_eng.h"

#else
#include "errores_esp.h"
#include "mensajes_esp.h"

#endif

#endif
