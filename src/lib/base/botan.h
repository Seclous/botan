/*
* A vague catch all include file for Botan
* (C) 1999-2007 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_BOTAN_H_
#define BOTAN_BOTAN_H_

#include <botan/lookup.h>
#include <botan/version.h>
#include <botan/parsing.h>

#include <botan/rng.h>

#if defined(BOTAN_HAS_AUTO_SEEDING_RNG)
  #include <botan/auto_rng.h>
#endif

/*
* There is no real reason for this header to exist beyond historical
* reasons. The application should instead include the specific header
* files that define the interfaces it intends to use.
*
* This header file will be removed in Botan 3.x
*/
#if defined(__GNUC__)
  #warning "botan/botan.h is deprecated"
#elif defined(_MSC_VER)
  #pragma message "botan/botan.h is deprecated"
#endif

#endif