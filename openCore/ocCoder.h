/**
  ocCoder.h

  ocCoder
  various encode/decode pairs
  ---------------------
  Start of complete encoding library
  Copyright (2002) D.K.McCombs - w3sys.com

  RFC 2045                Internet Message Bodies            November 1996

  6.8. Base64 Content-Transfer-Encoding top end

   The Base64 Content-Transfer-Encoding is designed to represent
   arbitrary sequences of octets in a form that need not be humanly
   readable.  The encoding and decoding algorithms are simple, but the
   encoded data are consistently only about 33 percent larger than the
   unencoded data.  This encoding is virtually identical to the one used
   in Privacy Enhanced Mail (PEM) applications, as defined in RFC 1421.

   A 65-character subset of US-ASCII is used, enabling 6 bits to be
   represented per printable character. (The extra 65th character, "=",
   is used to signify a special processing function.)

   NOTE:  This subset has the important property that it is represented
   identically in all versions of ISO 646, including US-ASCII, and all
   characters in the subset are also represented identically in all
   versions of EBCDIC. Other popular encodings, such as the encoding
   used by the uuencode utility, Macintosh binhex 4.0 [RFC-1741], and
   the base85 encoding specified as part of Level 2 PostScript, do not
   share these properties, and thus do not fulfill the portability
   requirements a binary transport encoding for mail must meet.

   The encoding process represents 24-bit groups of input bits as output
   strings of 4 encoded characters.  Proceeding from left to right, a
   24-bit input group is formed by concatenating 3 8bit input groups.
   These 24 bits are then treated as 4 concatenated 6-bit groups, each
   of which is translated into a single digit in the base64 alphabet.
   When encoding a bit stream via the base64 encoding, the bit stream
   must be presumed to be ordered with the most-significant-bit first.
   That is, the first bit in the stream will be the high-order bit in
   the first 8bit byte, and the eighth bit will be the low-order bit in
   the first 8bit byte, and so on.

   Each 6-bit group is used as an index into an array of 64 printable
   characters.  The character referenced by the index is placed in the
   output string.  These characters, identified in Table 1, below, are
   selected so as to be universally representable, and the set excludes
   characters with particular significance to SMTP (e.g., ".", CR, LF)
   and to the multipart boundary delimiters defined in RFC 2046 (e.g.,
   "-").
                    Table 1: The Base64 Alphabet

     Value Encoding  Value Encoding  Value Encoding  Value Encoding
         0 A            17 R            34 i            51 z
         1 B            18 S            35 j            52 0
         2 C            19 T            36 k            53 1
         3 D            20 U            37 l            54 2
         4 E            21 V            38 m            55 3
         5 F            22 W            39 n            56 4
         6 G            23 X            40 o            57 5
         7 H            24 Y            41 p            58 6
         8 I            25 Z            42 q            59 7
         9 J            26 a            43 r            60 8
        10 K            27 b            44 s            61 9
        11 L            28 c            45 t            62 +
        12 M            29 d            46 u            63 /
        13 N            30 e            47 v
        14 O            31 f            48 w         (pad) =
        15 P            32 g            49 x
        16 Q            33 h            50 y

   The encoded output stream must be represented in lines of no more
   than 76 characters each.  All line breaks or other characters not
   found in Table 1 must be ignored by decoding software.  In base64
   data, characters other than those in Table 1, line breaks, and other
   white space probably indicate a transmierror, about which a
   warning message or even a message rejection might be appropriate
   under some circumstances.

   Special processing is performed if fewer than 24 bits are available
   at the end of the data being encoded.  A full encoding quantum is
   always completed at the end of a body.  When fewer than 24 input bits
   are available in an input group, zero bits are added (on the right)
   to form an integral number of 6-bit groups.  Padding at the end of
   the data is performed using the "=" character.  Since all base64
   input is an integral number of octets, only the following cases can
   arise: (1) the final quantum of encoding input is an integral
   multiple of 24 bits; here, the final unit of encoded output will be
   an integral multiple of 4 characters with no "=" padding, (2) the
   final quantum of encoding input is exactly 8 bits; here, the final
   unit of encoded output will be two characters followed by two "="
   padding characters, or (3) the final quantum of encoding input is
   exactly 16 bits; here, the final unit of encoded output will be three
   characters followed by one "=" padding character.

   Because it is used only for padding at the end of the data, the
   occurrence of any "=" characters may be taken as evidence that the
   end of the data has been reached (without truncation in transit).  No
   such assurance is possible, however, when the number of octets
   transmitted was a multiple of three and no "=" characters are
   present.

   Any characters outside of the base64 alphabet are to be ignored in
   base64-encoded data.

   Care must be taken to use the proper octets for line breaks if base64
   encoding is applied directly to text material that has not been
   converted to canonical form.  In particular, text line breaks must be
   converted into CRLF sequences prior to base64 encoding.  The
   important thing to note is that this may be done directly by the
   encoder rather than in a prior canonicalization step in some
   implementations.

   NOTE: There is no need to worry about quoting potential boundary
   delimiters within base64-encoded bodies within multipart entities
   because no hyphen characters are used in the base64 encoding.

*/


#ifndef OC_CODER_H
#define OC_CODER_H

#include <string>
#include <algorithm>
#include <vector>


using namespace std;

// since max line for mail transport=76, use 4/3(76), or 57, for bin encode length
const size_t  ENCODELength = 57;
const size_t  DECODELength = 76;
const size_t  REVARRAYSize = 126;

// pack the structure byte tight
#pragma pack(1)
#ifdef IS_BIG_ENDIAN
union b24bits
{
  struct {
    unsigned int ch4:6;
    unsigned int ch3:6;
    unsigned int ch2:6;
    unsigned int ch1:6;
  } bit6Sequence;
  struct {
    unsigned char ch3;
    unsigned char ch2;
    unsigned char ch1;
  } byteSequence;
};
#else
union b24bits
{
  struct {
    unsigned int ch1:6;
    unsigned int ch2:6;
    unsigned int ch3:6;
    unsigned int ch4:6;
  } bit6Sequence;
  struct {
    unsigned char ch1;
    unsigned char ch2;
    unsigned char ch3;
  } byteSequence;
};
#endif
// return to normal stucture (probably machine word size) boundaries
#pragma pack()

typedef vector< unsigned char > bins;

class ocCoder
{
private:
  string  alphabet;
  bins    ralphabet;
  bins    bindata;
  string  base64data;
  char    base64pad;
  b24bits base64mask;

public:
  ocCoder()
  // The forward map for encoding
          //           1         2         3         4         5         6
          // 0123456789012345678901234567890123456789012345678901234567890123
  :alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")
  ,ralphabet(REVARRAYSize,'\0')
  ,bindata(ENCODELength,'\0')
  ,base64pad('=')
  {
    // the reverse map for decoding;
    ralphabet[65] = 0;    ralphabet[66] = 1;    ralphabet[67] = 2;    ralphabet[68] = 3;
    ralphabet[69] = 4;    ralphabet[70] = 5;    ralphabet[71] = 6;    ralphabet[72] = 7;
    ralphabet[73] = 8;    ralphabet[74] = 9;    ralphabet[75] = 10;    ralphabet[76] = 11;
    ralphabet[77] = 12;    ralphabet[78] = 13;    ralphabet[79] = 14;    ralphabet[80] = 15;
    ralphabet[81] = 16;    ralphabet[82] = 17;    ralphabet[83] = 18;    ralphabet[84] = 19;
    ralphabet[85] = 20;    ralphabet[86] = 21;    ralphabet[87] = 22;    ralphabet[88] = 23;
    ralphabet[89] = 24;    ralphabet[90] = 25;    ralphabet[97] = 26;    ralphabet[98] = 27;
    ralphabet[99] = 28;    ralphabet[100] = 29;    ralphabet[101] = 30;    ralphabet[102] = 31;
    ralphabet[103] = 32;    ralphabet[104] = 33;    ralphabet[105] = 34;    ralphabet[106] = 35;
    ralphabet[107] = 36;    ralphabet[108] = 37;    ralphabet[109] = 38;    ralphabet[110] = 39;
    ralphabet[111] = 40;    ralphabet[112] = 41;    ralphabet[113] = 42;    ralphabet[114] = 43;
    ralphabet[115] = 44;    ralphabet[116] = 45;    ralphabet[117] = 46;    ralphabet[118] = 47;
    ralphabet[119] = 48;    ralphabet[120] = 49;    ralphabet[121] = 50;    ralphabet[122] = 51;
    ralphabet[48] = 52;    ralphabet[49] = 53;    ralphabet[50] = 54;    ralphabet[51] = 55;
    ralphabet[52] = 56;    ralphabet[53] = 57;    ralphabet[54] = 58;    ralphabet[55] = 59;
    ralphabet[56] = 60;    ralphabet[57] = 61;    ralphabet[43] = 62;    ralphabet[47] = 63;
  }

  virtual ~ocCoder()
  {;}

  /*
    Take binary segment of length and convert in to base64
  */
  string & base64encode( const unsigned char * input, size_t length )
  {
    // initialize the output data
    base64data = "";
    base64data.resize(DECODELength);

    // The function won't do much if the length is to big
    if( length <= ENCODELength )
    {
      int idx,odx;
      for( idx=0,odx=0;
          input && idx < length;
          idx += 3, odx+=4 )
      {
        // clear the union
        memset( &base64mask, 0, 3 );

        // Put the three raw values in the union
        base64mask.byteSequence.ch3 = input[idx];
        if( (idx+1) < length ) base64mask.byteSequence.ch2 = input[idx+1];
        if( (idx+2) < length ) base64mask.byteSequence.ch1 = input[idx+2];

        // Translate the four output values and put them into the return string
        base64data[odx]   = alphabet[base64mask.bit6Sequence.ch4];
        base64data[odx+1] = alphabet[base64mask.bit6Sequence.ch3];
        // See about padding
        if( (idx+1) < length ) base64data[odx+2] = alphabet[base64mask.bit6Sequence.ch2];
        else base64data[odx+2] = base64pad;
        if( (idx+2) < length ) base64data[odx+3] = alphabet[base64mask.bit6Sequence.ch1];
        else base64data[odx+3] = base64pad;
      }
    }
    return base64data;
  }

  bins & base64decode ( string & encoded )
  {
    bindata.clear();
    int idx;
    // four chars at a time
    for( idx = 0; idx < encoded.length(); idx+=4 )
    {
      // clear the union
      memset( &base64mask, 0, 3 );

      // Put 4 elements into the union
      base64mask.bit6Sequence.ch4 = ralphabet[encoded[idx]];
      base64mask.bit6Sequence.ch3 = ralphabet[encoded[idx+1]];
      if(encoded[idx+2] != base64pad) base64mask.bit6Sequence.ch2 = ralphabet[encoded[idx+2]];
      if(encoded[idx+3] != base64pad) base64mask.bit6Sequence.ch1 = ralphabet[encoded[idx+3]];

      // Take binary data out of the union and put into the vector
      bindata.push_back(base64mask.byteSequence.ch3);
      if(encoded[idx+2] != base64pad) bindata.push_back(base64mask.byteSequence.ch2);
      if(encoded[idx+3] != base64pad) bindata.push_back(base64mask.byteSequence.ch1);
    }

    // Return the output
    return bindata;
  }

  string & data( void )
  {
    return base64data;
  }
};

// define a stream operator for bins
ostream & operator << ( ostream & outstream, bins & binary )
{
  for( int i=0; i < binary.size(); i++ )
  {
    outstream.put( binary[i] );
  }
  return outstream;
}

#endif
