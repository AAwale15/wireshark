/*
 * Cross platform defines for exporting symbols from shared libraries
 *
 * Wireshark - Network traffic analyzer
 * By Balint Reczey <balint@balintreczey.hu>
 * Copyright 2013 Balint Reczey
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/** Reset symbol export behavior.
 * If you {un}define WS_BUILD_DLL on the fly you'll have to define this
 * as well.
 */
#ifdef RESET_SYMBOL_EXPORT

#ifdef SYMBOL_EXPORT_H
#undef SYMBOL_EXPORT_H
#endif

#ifdef WS_DLL_PUBLIC
#undef WS_DLL_PUBLIC
#endif

#ifdef WS_DLL_PUBLIC_DEF
#undef WS_DLL_PUBLIC_DEF
#endif

#ifdef WS_DLL_LOCAL
#undef WS_DLL_LOCAL
#endif

#endif /* RESET_SYMBOL_EXPORT */

#ifndef SYMBOL_EXPORT_H
#define SYMBOL_EXPORT_H

/* Originally copied from GCC Wiki at http://gcc.gnu.org/wiki/Visibility */
#if defined _WIN32 || defined __CYGWIN__
  /* Compiling for Windows, so we use the Windows DLL declarations. */
  #ifdef WS_BUILD_DLL
    /*
     * Building a DLL; for all definitions, we want dllexport, and
     * (presumably so source from DLL and source from a program using the
     * DLL can both include a header that declares APIs and exported data
     * for the DLL), for declarations, either dllexport or dllimport will
     * work (they mean the same thing for a declaration when building a DLL).
     */
    #ifdef __GNUC__
      /* GCC */
#define WS_DLL_PUBLIC_DEF __attribute__ ((dllexport))
    #else /* ! __GNUC__ */
      /*
       * Presumably MSVC.
       * Note: actually gcc seems to also support this syntax.
       */
#define WS_DLL_PUBLIC_DEF __declspec(dllexport)
    #endif /* __GNUC__ */
  #else /* WS_BUILD_DLL */
    /*
     * Building a program; we should only see declarations, not definitions,
     * with WS_DLL_PUBLIC, and they all represent APIs or data imported
     * from a DLL, so use dllimport.
     *
     * For functions, export shouldn't be necessary; for data, it might
     * be necessary, e.g. if what's declared is an array whose size is
     * not given in the declaration.
     */
    #ifdef __GNUC__
      /* GCC */
#define WS_DLL_PUBLIC_DEF __attribute__ ((dllimport))
    #elif ! (defined ENABLE_STATIC) /* ! __GNUC__ */
      /*
       * Presumably MSVC.
       * Note: actually gcc seems to also support this syntax.
       */
#define WS_DLL_PUBLIC_DEF __declspec(dllimport)
    #else /* ! __GNUC__  && ENABLE_STATIC */
      /* presumably MSVC */
#define WS_DLL_PUBLIC_DEF
    #endif /* __GNUC__ */
  #endif /* WS_BUILD_DLL */
  #define WS_DLL_LOCAL
#else /* defined _WIN32 || defined __CYGWIN__ */
  /*
   * Compiling for UN*X, where the dllimport and dllexport stuff
   * is neither necessary nor supported; just specify the
   * visibility if we have a compiler that claims compatibility
   * with GCC 4 or later.
   */
  #if __GNUC__ >= 4
#define WS_DLL_PUBLIC_DEF __attribute__ ((visibility ("default")))
#define WS_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else /* ! __GNUC__ >= 4 */
    #define WS_DLL_PUBLIC_DEF
    #define WS_DLL_LOCAL
  #endif /* __GNUC__ >= 4 */
#endif

/*
 * Use this for declarations; it can also be used for exported
 * *function* definitions, but must not be used for exported *data*
 * definitions.
 */
#define WS_DLL_PUBLIC	WS_DLL_PUBLIC_DEF extern

#endif /* SYMBOL_EXPORT_H */
