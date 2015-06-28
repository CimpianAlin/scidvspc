//////////////////////////////////////////////////////////////////////
//
//  FILE:       charsetconverter.h
//              CharsetConverter class methods
//
//  Part of:    Scid vs. PC
//  Version:    4.15
//
//  Notice:     Copyright (c) 2015  Gregor Cramer.  All rights reserved.
//
//  Author:     Gregor Cramer (gcramer@users.sourceforge.net)
//
//////////////////////////////////////////////////////////////////////

#ifndef SCID_CHARSETCONVERTER_H
#define SCID_CHARSETCONVERTER_H

#ifdef TCL_ONLY
# error "cannt be used in tcscid"
#endif

#include "charsetdetector.h"
#include <string>

extern "C" { struct Tcl_Encoding_; }


class CharsetConverter
{
public:

  // Construct a character set convert with system encoding.
  CharsetConverter();

  // Construct a character set convert with given encoding.
  CharsetConverter(char const* encoding);

  // Test the conversion status. This occurs in case of characters which are not
  // known in the destination character set; this means that some characters
  // couldn't be converted. This functions return the number of unconvertible
  // characters.
  unsigned unknown() const;

  // Test the error status. This occurs in case of invalid characters sequences
  // in input string.
  bool error() const;

  // Test the failure status. Sometimes the conversion fails to convert the
  // input string correctly (Tcl bug). But in such cases the result of the
  // conversion should be nearly lossless.
  bool failed() const;

  // Test whether character conversion has succeeded without any error.
  bool succeeded() const;

  // Return the system encoding.
  std::string const& systemEncoding() const;

  // Return the wanted encoding.
  std::string const& wantedEncoding() const;

  // Return the detected encoding.
  std::string const& detectedEncoding() const;

  // Convert a string from UTF-8 encoding to the wanted encoding.
  // This function returns whether the conversion was successful, it may fail if
  // the encoding cannot map all characters, nethertheless the character set of
  // the result is valid. In case of invalid input characters the error status
  // will be set.
  bool convertFromUTF8(std::string const& in, std::string& out);

  // Convert a string with detected encoding into a string with UTF-8 encoding.
  // This function returns whether the conversion was successful, it may fail if
  // the encoding cannot map all characters, nethertheless the character set of
  // the result is valid. In case of invalid input characters the error status
  // will be set.
  bool convertToUTF8(std::string const& in, std::string& out, char const* replacement = "\xef\xbf\xbd");

  // Convert buffer content from detected encoding to wanted encoding.
  // This function returns whether the conversion was successful, it may fail if
  // the encoding cannot map all characters, nethertheless the character set of
  // the result is valid. In case of invalid input characters the error status
  // will be set.
  bool doConversion(TextBuffer& text);

  /// Setup a wanted character set. This will not reset the error status.
  void setupEncoding(char const* encoding);

  /// Reset error status.
  void reset();

  // ----------------------------------------------------------------------
  // Some helpers:
  // ----------------------------------------------------------------------

  // Forcing a valid UTF-8 string. The returns value gives (approximately)
  // the number of characters lost because of invalid UTF-8 sequences.
  // Any lost character will be replaced with the given replacement string.
  static unsigned makeValid(std::string& str, char const* replacement = "\xef\xbf\xbd");

  // The Tcl library is producing overlong sequences in rare conditions
  // (invalid UTF-8). So we have to remove such invalid sequences. With
  // a bit luck this will work without any loss.
  // In fact this function will do some more fixes: removing surrogates,
  // invalid bytes, and forbidden code points). In such cases the number
  // of lost characters (approximately) will be returned. Any lost character
  // will be replaced with the given replacement string.
  static unsigned removeInvalidSequences(std::string& str, char const* replacement = "\xef\xbf\xbd");
  static unsigned removeInvalidSequences(std::string& str, unsigned len, char const* replacement = "\xef\xbf\xbd");

  // String content is valid UTF-8?
  static bool validateUTF8(char const* str, unsigned len);
  static bool validateUTF8(std::string const& str);
  static bool validateUTF8(std::string const& str, unsigned len);

  // String content is ASCII?
  static bool isAscii(char const* str);

  // UTF-8 string content is convertible to Latin-1?
  static bool isConvertibleToLatin1(char const* str);

private:

  typedef CharsetDetector::Info Info;

  struct Codec
  {
    Codec();
    Codec(char const* encoding);
    ~Codec();

    Codec(Codec const&);
    Codec& operator=(Codec const&);

    std::string const& encoding() const;
    bool isUTF8() const;
    bool isLatin1() const;
    bool isWindoze() const;

    void setup(char const* encoding);
    void setup(Info const& info);
    void detectSystemEncoding();

    Info m_info;
    struct Tcl_Encoding_* m_impl;
  };

  static bool fixLatin1(std::string const& in, std::string& out);

  static void asciiToUTF8(std::string const& in, std::string& out);
  static void cp1252ToUTF8(std::string const& in, std::string& out);

  static int detect(char const* s, unsigned len, char const* table);

  Codec     m_system;
  Codec     m_wanted;
  Codec     m_text;
  bool      m_error;
  bool      m_failed;
  unsigned  m_unknown;

  CharsetDetector m_detector;
};


inline bool CharsetConverter::error() const     { return m_error; }
inline bool CharsetConverter::failed() const    { return m_failed; }
inline bool CharsetConverter::succeeded() const { return !m_error && !m_failed; }

inline std::string const& CharsetConverter::systemEncoding() const   { return m_system.m_info.m_encoding; }
inline std::string const& CharsetConverter::wantedEncoding() const   { return m_wanted.m_info.m_encoding; }
inline std::string const& CharsetConverter::detectedEncoding() const { return m_text.m_info.m_encoding; }

inline void CharsetConverter::reset() { m_error = false; }

inline unsigned CharsetConverter::removeInvalidSequences(std::string& str, char const* replacement)
{
  return removeInvalidSequences(str, str.size(), replacement);
}

inline bool CharsetConverter::validateUTF8(std::string const& str)
{
  return validateUTF8(str.data(), str.size());
}

inline bool CharsetConverter::validateUTF8(std::string const& str, unsigned len)
{
  return validateUTF8(str.data(), len);
}

#endif // SCID_CHARSETCONVERTER_H
//////////////////////////////////////////////////////////////////////
//  EOF: charsetconverter.h
//////////////////////////////////////////////////////////////////////
// vi:set ts=2 sw=2 et:
