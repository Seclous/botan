/*
* (C) 2014,2015,2017 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_ASN1_PRINT_H_
#define BOTAN_ASN1_PRINT_H_

#include <botan/asn1_obj.h>
#include <string>
#include <vector>
#include <iosfwd>

namespace Botan {

class BER_Decoder;

/**
* Format ASN.1 data and call a virtual to format
*/
class BOTAN_DLL ASN1_Formatter
   {
   public:
      virtual ~ASN1_Formatter() = default;

      /**
      * @param print_context_specific if true, try to parse nested context specific data.
      */
      ASN1_Formatter(bool print_context_specific) :
         m_print_context_specific(print_context_specific)
         {}

      void print_to_stream(std::ostream& out,
                           const uint8_t in[],
                           size_t len) const;

      std::string print(const uint8_t in[], size_t len) const;

      template<typename Alloc>
      std::string print(const std::vector<uint8_t, Alloc>& vec) const
         {
         return print(vec.data(), vec.size());
         }

   protected:
      /**
      * This is called for each element
      */
      virtual std::string format(ASN1_Tag type_tag,
                                 ASN1_Tag class_tag,
                                 size_t level,
                                 size_t length,
                                 const std::string& value) const = 0;

      /**
      * This is called to format binary elements that we don't know how to
      * convert to a string The result will be passed as value to format; the
      * tags are included as a hint to aid decoding.
      */
      virtual std::string format_bin(ASN1_Tag type_tag,
                                     ASN1_Tag class_tag,
                                     const std::vector<uint8_t>& vec) const = 0;

   private:
      void decode(std::ostream& output,
                  BER_Decoder& decoder,
                  size_t level) const;

      const bool m_print_context_specific;
   };

/**
* Format ASN.1 data into human readable strings
*/
class BOTAN_DLL ASN1_Pretty_Printer final : public ASN1_Formatter
   {
   public:
      /**
      * @param print_limit strings larger than this are not printed
      * @param print_binary_limit binary strings larger than this are not printed
      * @param print_context_specific if true, try to parse nested context specific data.
      * @param initial_level the initial depth (0 or 1 are the only reasonable values)
      * @param value_column ASN.1 values are lined up at this column in output
      */
      ASN1_Pretty_Printer(size_t print_limit = 256,
                          size_t print_binary_limit = 256,
                          bool print_context_specific = true,
                          size_t initial_level = 0,
                          size_t value_column = 60) :
         ASN1_Formatter(print_context_specific),
         m_print_limit(print_limit),
         m_print_binary_limit(print_binary_limit),
         m_initial_level(initial_level),
         m_value_column(value_column)
         {}

   private:
      std::string format(ASN1_Tag type_tag,
                         ASN1_Tag class_tag,
                         size_t level,
                         size_t length,
                         const std::string& value) const override;

      std::string format_bin(ASN1_Tag type_tag,
                             ASN1_Tag class_tag,
                             const std::vector<uint8_t>& vec) const override;

      const size_t m_print_limit;
      const size_t m_print_binary_limit;
      const size_t m_initial_level;
      const size_t m_value_column;
   };

}

#endif