/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2022 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 *
 * @author Zhiyi Zhang <dreamerbarrychang@gmail.com>
 */

#include "ndn-cxx/security/certificate.hpp"
#include "ndn-cxx/encoding/block-helpers.hpp"
#include "ndn-cxx/util/io.hpp"

#include "tests/boost-test.hpp"
#include "tests/unit/clock-fixture.hpp"

#include <boost/lexical_cast.hpp>

namespace ndn {
namespace security {
inline namespace v2 {
namespace tests {

using namespace ndn::tests;

BOOST_AUTO_TEST_SUITE(Security)
BOOST_FIXTURE_TEST_SUITE(TestCertificate, ClockFixture)

const uint8_t PUBLIC_KEY[] = {
  0x30, 0x81, 0x9d, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
  0x01, 0x05, 0x00, 0x03, 0x81, 0x8b, 0x00, 0x30, 0x81, 0x87, 0x02, 0x81, 0x81, 0x00, 0x9e,
  0x06, 0x3e, 0x47, 0x85, 0xb2, 0x34, 0x37, 0xaa, 0x85, 0x47, 0xac, 0x03, 0x24, 0x83, 0xb5,
  0x9c, 0xa8, 0x05, 0x3a, 0x24, 0x1e, 0xeb, 0x89, 0x01, 0xbb, 0xe9, 0x9b, 0xb2, 0xc3, 0x22,
  0xac, 0x68, 0xe3, 0xf0, 0x6c, 0x02, 0xce, 0x68, 0xa6, 0xc4, 0xd0, 0xa7, 0x06, 0x90, 0x9c,
  0xaa, 0x1b, 0x08, 0x1d, 0x8b, 0x43, 0x9a, 0x33, 0x67, 0x44, 0x6d, 0x21, 0xa3, 0x1b, 0x88,
  0x9a, 0x97, 0x5e, 0x59, 0xc4, 0x15, 0x0b, 0xd9, 0x2c, 0xbd, 0x51, 0x07, 0x61, 0x82, 0xad,
  0xc1, 0xb8, 0xd7, 0xbf, 0x9b, 0xcf, 0x7d, 0x24, 0xc2, 0x63, 0xf3, 0x97, 0x17, 0xeb, 0xfe,
  0x62, 0x25, 0xba, 0x5b, 0x4d, 0x8a, 0xc2, 0x7a, 0xbd, 0x43, 0x8a, 0x8f, 0xb8, 0xf2, 0xf1,
  0xc5, 0x6a, 0x30, 0xd3, 0x50, 0x8c, 0xc8, 0x9a, 0xdf, 0xef, 0xed, 0x35, 0xe7, 0x7a, 0x62,
  0xea, 0x76, 0x7c, 0xbb, 0x08, 0x26, 0xc7, 0x02, 0x01, 0x11
};

const uint8_t SIG_INFO[] = {
  0x16, 0x55, 0x1B, 0x01, 0x01, 0x1C, 0x26, 0x07, 0x24, 0x08, 0x03, 0x6E, 0x64, 0x6E, 0x08, 0x05,
  0x73, 0x69, 0x74, 0x65, 0x31, 0x08, 0x11, 0x6B, 0x73, 0x6B, 0x2D, 0x32, 0x35, 0x31, 0x36, 0x34,
  0x32, 0x35, 0x33, 0x37, 0x37, 0x30, 0x39, 0x34, 0x08, 0x03, 0x4B, 0x45, 0x59, 0xFD, 0x00, 0xFD,
  0x26, 0xFD, 0x00, 0xFE, 0x0F, 0x32, 0x30, 0x31, 0x35, 0x30, 0x38, 0x31, 0x34, 0x54, 0x32, 0x32,
  0x33, 0x37, 0x33, 0x39, 0xFD, 0x00, 0xFF, 0x0F, 0x32, 0x30, 0x31, 0x35, 0x30, 0x38, 0x31, 0x38,
  0x54, 0x32, 0x32, 0x33, 0x37, 0x33, 0x38
};

const uint8_t SIG_VALUE[] = {
  0x2f, 0xd6, 0xf1, 0x6e, 0x80, 0x6f, 0x10, 0xbe, 0xb1, 0x6f, 0x3e, 0x31, 0xec, 0xe3, 0xb9, 0xea,
  0x83, 0x30, 0x40, 0x03, 0xfc, 0xa0, 0x13, 0xd9, 0xb3, 0xc6, 0x25, 0x16, 0x2d, 0xa6, 0x58, 0x41,
  0x69, 0x62, 0x56, 0xd8, 0xb3, 0x6a, 0x38, 0x76, 0x56, 0xea, 0x61, 0xb2, 0x32, 0x70, 0x1c, 0xb6,
  0x4d, 0x10, 0x1d, 0xdc, 0x92, 0x8e, 0x52, 0xa5, 0x8a, 0x1d, 0xd9, 0x96, 0x5e, 0xc0, 0x62, 0x0b,
  0xcf, 0x3a, 0x9d, 0x7f, 0xca, 0xbe, 0xa1, 0x41, 0x71, 0x85, 0x7a, 0x8b, 0x5d, 0xa9, 0x64, 0xd6,
  0x66, 0xb4, 0xe9, 0x8d, 0x0c, 0x28, 0x43, 0xee, 0xa6, 0x64, 0xe8, 0x55, 0xf6, 0x1c, 0x19, 0x0b,
  0xef, 0x99, 0x25, 0x1e, 0xdc, 0x78, 0xb3, 0xa7, 0xaa, 0x0d, 0x14, 0x58, 0x30, 0xe5, 0x37, 0x6a,
  0x6d, 0xdb, 0x56, 0xac, 0xa3, 0xfc, 0x90, 0x7a, 0xb8, 0x66, 0x9c, 0x0e, 0xf6, 0xb7, 0x64, 0xd1,
};

const uint8_t CERT[] = {
  0x06, 0xFD, 0x01, 0xBB, // Data
    0x07, 0x33, // Name /ndn/site1/KEY/ksk-1416425377094/0123/%FD%00%00%01I%C9%8B
      0x08, 0x03, 0x6E, 0x64, 0x6E,
      0x08, 0x05, 0x73, 0x69, 0x74, 0x65, 0x31,
      0x08, 0x03, 0x4B, 0x45, 0x59,
      0x08, 0x11,
        0x6B, 0x73, 0x6B, 0x2D, 0x31, 0x34, 0x31, 0x36, 0x34, 0x32, 0x35, 0x33, 0x37, 0x37, 0x30, 0x39,
        0x34,
      0x08, 0x04, 0x30, 0x31, 0x32, 0x33,
      0x08, 0x07, 0xFD, 0x00, 0x00, 0x01, 0x49, 0xC9, 0x8B,
    0x14, 0x09, // MetaInfo
      0x18, 0x01, 0x02, // ContentType = Key
      0x19, 0x04, 0x00, 0x36, 0xEE, 0x80, // FreshnessPeriod = 3600000 ms
    0x15, 0xA0, // Content
      0x30, 0x81, 0x9D, 0x30, 0x0D, 0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01,
      0x05, 0x00, 0x03, 0x81, 0x8B, 0x00, 0x30, 0x81, 0x87, 0x02, 0x81, 0x81, 0x00, 0x9E, 0x06, 0x3E,
      0x47, 0x85, 0xB2, 0x34, 0x37, 0xAA, 0x85, 0x47, 0xAC, 0x03, 0x24, 0x83, 0xB5, 0x9C, 0xA8, 0x05,
      0x3A, 0x24, 0x1E, 0xEB, 0x89, 0x01, 0xBB, 0xE9, 0x9B, 0xB2, 0xC3, 0x22, 0xAC, 0x68, 0xE3, 0xF0,
      0x6C, 0x02, 0xCE, 0x68, 0xA6, 0xC4, 0xD0, 0xA7, 0x06, 0x90, 0x9C, 0xAA, 0x1B, 0x08, 0x1D, 0x8B,
      0x43, 0x9A, 0x33, 0x67, 0x44, 0x6D, 0x21, 0xA3, 0x1B, 0x88, 0x9A, 0x97, 0x5E, 0x59, 0xC4, 0x15,
      0x0B, 0xD9, 0x2C, 0xBD, 0x51, 0x07, 0x61, 0x82, 0xAD, 0xC1, 0xB8, 0xD7, 0xBF, 0x9B, 0xCF, 0x7D,
      0x24, 0xC2, 0x63, 0xF3, 0x97, 0x17, 0xEB, 0xFE, 0x62, 0x25, 0xBA, 0x5B, 0x4D, 0x8A, 0xC2, 0x7A,
      0xBD, 0x43, 0x8A, 0x8F, 0xB8, 0xF2, 0xF1, 0xC5, 0x6A, 0x30, 0xD3, 0x50, 0x8C, 0xC8, 0x9A, 0xDF,
      0xEF, 0xED, 0x35, 0xE7, 0x7A, 0x62, 0xEA, 0x76, 0x7C, 0xBB, 0x08, 0x26, 0xC7, 0x02, 0x01, 0x11,
    0x16, 0x55, // SignatureInfo
      0x1B, 0x01, 0x01, // SignatureType
      0x1C, 0x26, // KeyLocator: /ndn/site1/KEY/ksk-2516425377094
        0x07, 0x24,
          0x08, 0x03, 0x6E, 0x64, 0x6E,
          0x08, 0x05, 0x73, 0x69, 0x74, 0x65, 0x31,
          0x08, 0x03, 0x4B, 0x45, 0x59,
          0x08, 0x11,
            0x6B, 0x73, 0x6B, 0x2D, 0x32, 0x35, 0x31, 0x36, 0x34, 0x32, 0x35, 0x33, 0x37, 0x37, 0x30, 0x39,
            0x34,
      0xFD, 0x00, 0xFD, 0x26, // ValidityPeriod: (20150814T223739, 20150818T223738)
        0xFD, 0x00, 0xFE, 0x0F,
          0x32, 0x30, 0x31, 0x35, 0x30, 0x38, 0x31, 0x34, 0x54, 0x32, 0x32, 0x33, 0x37, 0x33, 0x39,
        0xFD, 0x00, 0xFF, 0x0F,
          0x32, 0x30, 0x31, 0x35, 0x30, 0x38, 0x31, 0x38, 0x54, 0x32, 0x32, 0x33, 0x37, 0x33, 0x38,
    0x17, 0x80, // SignatureValue
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static void
generateFakeSignature(Data& data)
{
  SignatureInfo signatureInfo(Block{SIG_INFO});
  signatureInfo.setKeyLocator(Name("/ndn/site1/KEY/ksk-2516425377094"));
  signatureInfo.setValidityPeriod(ValidityPeriod(time::fromIsoString("20141111T050000"),
                                                 time::fromIsoString("20141111T060000")));

  data.setSignatureInfo(signatureInfo);
  data.setSignatureValue(SIG_VALUE);
}

BOOST_AUTO_TEST_CASE(Construction)
{
  Block block(CERT);
  Certificate certificate(block);
  const ValidityPeriod vp(time::fromIsoString("20150814T223739"),
                          time::fromIsoString("20150818T223738"));

  BOOST_CHECK_EQUAL(certificate.getName(), "/ndn/site1/KEY/ksk-1416425377094/0123/%FD%00%00%01I%C9%8B");
  BOOST_CHECK_EQUAL(certificate.getKeyName(), "/ndn/site1/KEY/ksk-1416425377094");
  BOOST_CHECK_EQUAL(certificate.getIdentity(), "/ndn/site1");
  BOOST_CHECK_EQUAL(certificate.getKeyId(), name::Component("ksk-1416425377094"));
  BOOST_CHECK_EQUAL(certificate.getIssuerId(), name::Component("0123"));
  BOOST_TEST(certificate.getPublicKey() == PUBLIC_KEY, boost::test_tools::per_element());
  BOOST_CHECK_EQUAL(certificate.getKeyLocator().value().getName(), "/ndn/site1/KEY/ksk-2516425377094");
  BOOST_CHECK_EQUAL(certificate.getValidityPeriod(), vp);

  BOOST_CHECK_EQUAL(certificate.getExtension(tlv::ValidityPeriod), vp.wireEncode());
  BOOST_CHECK_THROW(certificate.getExtension(12345), tlv::Error);

  Data data(block);
  Certificate certificate2(std::move(data));
  BOOST_CHECK_EQUAL(certificate, certificate2);
}

BOOST_AUTO_TEST_CASE(Setters)
{
  Certificate certificate;
  certificate.setName("/ndn/site1/KEY/ksk-1416425377094/0123/%FD%00%00%01I%C9%8B");
  certificate.setFreshnessPeriod(1_h);
  certificate.setContent(PUBLIC_KEY);
  generateFakeSignature(certificate);

  const ValidityPeriod vp(time::fromIsoString("20141111T050000"),
                          time::fromIsoString("20141111T060000"));

  BOOST_CHECK_EQUAL(certificate.getName(), "/ndn/site1/KEY/ksk-1416425377094/0123/%FD%00%00%01I%C9%8B");
  BOOST_CHECK_EQUAL(certificate.getKeyName(), "/ndn/site1/KEY/ksk-1416425377094");
  BOOST_CHECK_EQUAL(certificate.getIdentity(), "/ndn/site1");
  BOOST_CHECK_EQUAL(certificate.getKeyId(), name::Component("ksk-1416425377094"));
  BOOST_CHECK_EQUAL(certificate.getIssuerId(), name::Component("0123"));
  BOOST_TEST(certificate.getPublicKey() == PUBLIC_KEY, boost::test_tools::per_element());
  BOOST_CHECK_EQUAL(certificate.getKeyLocator().value().getName(), "/ndn/site1/KEY/ksk-2516425377094");
  BOOST_CHECK_EQUAL(certificate.getValidityPeriod(), vp);

  BOOST_CHECK_EQUAL(certificate.getExtension(tlv::ValidityPeriod), vp.wireEncode());
  BOOST_CHECK_THROW(certificate.getExtension(12345), tlv::Error);
}

BOOST_AUTO_TEST_CASE(ValidityPeriodChecking)
{
  Certificate certificate;
  certificate.setName("/ndn/site1/KEY/ksk-1416425377094/0123/%FD%00%00%01I%C9%8B");
  certificate.setFreshnessPeriod(1_h);
  certificate.setContent(PUBLIC_KEY);
  generateFakeSignature(certificate);

  BOOST_CHECK_EQUAL(certificate.isValid(), true);
  BOOST_CHECK_EQUAL(certificate.isValid(time::fromIsoString("20141111T045959")), false);
  BOOST_CHECK_EQUAL(certificate.isValid(time::fromIsoString("20141111T060001")), false);
}

// This fixture prepares a well-formed certificate. A test case then modifies one of the
// fields, and verifies the Certificate class correctly identifies the certificate as
// malformed.
class InvalidCertFixture
{
public:
  InvalidCertFixture()
  {
    Certificate certBase(Block{CERT});
    m_certBase = Data(certBase);
    generateFakeSignature(m_certBase);

    BOOST_REQUIRE_NO_THROW(Certificate{m_certBase});
  }

protected:
  Data m_certBase;
};

BOOST_FIXTURE_TEST_CASE(InvalidName, InvalidCertFixture)
{
  Data data(m_certBase);
  data.setName("/ndn/site1/ksk-1416425377094/0123/%FD%00%00%01I%C9%8B");

  BOOST_CHECK_EXCEPTION(Certificate{std::move(data)}, Certificate::Error, [] (const auto& e) {
    return e.what() == "Certificate name does not follow the naming conventions"s;
  });
}

BOOST_FIXTURE_TEST_CASE(InvalidContentType, InvalidCertFixture)
{
  Data data(m_certBase);
  data.setContentType(tlv::ContentType_Blob);

  BOOST_CHECK_EXCEPTION(Certificate{std::move(data)}, Certificate::Error, [] (const auto& e) {
    return e.what() == "Expecting ContentType=Key, got 0"s;
  });
}

BOOST_FIXTURE_TEST_CASE(InvalidFreshnessPeriod, InvalidCertFixture)
{
  Data data(m_certBase);
  data.setFreshnessPeriod(0_ms);

  BOOST_CHECK_EXCEPTION(Certificate{std::move(data)}, Certificate::Error, [] (const auto& e) {
    return e.what() == "Certificate FreshnessPeriod cannot be zero"s;
  });
}

BOOST_AUTO_TEST_CASE(Print)
{
  const std::string expected1(
R"TXT(Certificate Name:
  /
Public Key:
  Key Type: Unknown (0 bytes)
Signature Information:
  Signature Type: Unknown(65535)
)TXT");

  Certificate cert1;
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cert1), expected1);

  const std::string expected2(
R"TXT(Certificate Name:
  /ndn/site1/KEY/ksk-1416425377094/0123/%FD%00%00%01I%C9%8B
Public Key:
  Key Type: 1024-bit RSA
  MIGdMA0GCSqGSIb3DQEBAQUAA4GLADCBhwKBgQCeBj5HhbI0N6qFR6wDJIO1nKgF
  OiQe64kBu+mbssMirGjj8GwCzmimxNCnBpCcqhsIHYtDmjNnRG0hoxuImpdeWcQV
  C9ksvVEHYYKtwbjXv5vPfSTCY/OXF+v+YiW6W02Kwnq9Q4qPuPLxxWow01CMyJrf
  7+0153pi6nZ8uwgmxwIBEQ==
Validity:
  Not Before: 2015-08-14T22:37:39
  Not After: 2015-08-18T22:37:38
Signature Information:
  Signature Type: SignatureSha256WithRsa
  Key Locator: Name=/ndn/site1/KEY/ksk-2516425377094
)TXT");

  Certificate cert2(Block{CERT});
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cert2), expected2);

  const std::string expected3(
R"TXT(Certificate Name:
  /ndn/test/identity/KEY/%C7G%3A%D6%12P%B5%F0/self/v=1650251820652
Public Key:
  Key Type: 256-bit EC
  MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEnoGVuhO+9JnIRo6QSgJin8RgA8Gh
  RN9oVbnXi2rAJa4jq1yuCKaLeOt2sdXlkW6uBByOhbTuDdovlbIUsJ8bhg==
Validity:
  Not Before: 1970-01-01T00:00:00
  Not After: 2042-04-13T03:17:00
Signature Information:
  Signature Type: SignatureSha256WithEcdsa
  Key Locator: Name=/ndn/test/identity/KEY/%C7G%3A%D6%12P%B5%F0
  Self-Signed: yes
)TXT");

  std::istringstream is(
R"BASE64(Bv0BPgc0CANuZG4IBHRlc3QICGlkZW50aXR5CANLRVkICMdHOtYSULXwCARzZWxm
NggAAAGAOqxubBQJGAECGQQANu6AFVswWTATBgcqhkjOPQIBBggqhkjOPQMBBwNC
AASegZW6E770mchGjpBKAmKfxGADwaFE32hVudeLasAlriOrXK4Ipot463ax1eWR
bq4EHI6FtO4N2i+VshSwnxuGFlUbAQMcJgckCANuZG4IBHRlc3QICGlkZW50aXR5
CANLRVkICMdHOtYSULXw/QD9Jv0A/g8xOTcwMDEwMVQwMDAwMDD9AP8PMjA0MjA0
MTNUMDMxNzAwF0cwRQIgFRnwthtzKdqRgO3cZMNA1hfT3QcNu/+xjo7hUy+UvdsC
IQCz3DHoRtKl7uZoJOgQsZP1/CGkNjlGZE3EQ+Ylwiprrw==)BASE64");
  Certificate cert3 = io::loadTlv<Certificate>(is, io::BASE64);
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cert3), expected3);

  const std::string expected4(
R"TXT(Certificate Name:
  /ndn/test/identity/KEY/%C7G%3A%D6%12P%B5%F0/self/v=1650251820652
Public Key:
  Key Type: Unknown (23 bytes)
  bm90IGEgdmFsaWQgcHVibGljIGtleQA=
Validity:
  Not Before: 1970-01-01T00:00:00
  Not After: 2042-04-13T03:17:00
Signature Information:
  Signature Type: SignatureSha256WithEcdsa
  Key Locator: Name=/ndn/test/identity/KEY/%C7G%3A%D6%12P%B5%F0
  Self-Signed: yes
)TXT");

  const uint8_t notAKey[] = "not a valid public key";
  Certificate cert4(cert3);
  cert4.setContent(notAKey);
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cert4), expected4);

  const std::string expected5(
R"TXT(Certificate Name:
  /ndn/test/identity/KEY/%C7G%3A%D6%12P%B5%F0/self/v=1650251820652
Additional Description:
  bWFsZm9ybWVk
Public Key:
  Key Type: 256-bit EC
  MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEnoGVuhO+9JnIRo6QSgJin8RgA8Gh
  RN9oVbnXi2rAJa4jq1yuCKaLeOt2sdXlkW6uBByOhbTuDdovlbIUsJ8bhg==
Validity:
  Not Before: 1970-01-01T00:00:00
  Not After: 2042-04-13T03:17:00
Signature Information:
  Signature Type: SignatureSha256WithEcdsa
  Key Locator: KeyDigest=0000000000000000
)TXT");

  auto sigInfo = cert3.getSignatureInfo();
  sigInfo.addCustomTlv(makeStringBlock(tlv::AdditionalDescription, "malformed"));
  sigInfo.setKeyLocator(KeyLocator().setKeyDigest(std::make_shared<Buffer>(8)));
  Certificate cert5(cert3);
  cert5.setSignatureInfo(sigInfo);
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cert5), expected5);

  const std::string expected6(
R"TXT(Certificate Name:
  /ndn/test/identity/KEY/%C7G%3A%D6%12P%B5%F0/self/v=1650251820652
Public Key:
  Key Type: 256-bit EC
  MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEnoGVuhO+9JnIRo6QSgJin8RgA8Gh
  RN9oVbnXi2rAJa4jq1yuCKaLeOt2sdXlkW6uBByOhbTuDdovlbIUsJ8bhg==
Signature Information:
  Signature Type: DigestSha256
)TXT");

  sigInfo.removeCustomTlv(tlv::AdditionalDescription);
  sigInfo.addCustomTlv(makeStringBlock(tlv::ValidityPeriod, "malformed"));
  sigInfo.setSignatureType(tlv::DigestSha256);
  sigInfo.setKeyLocator(nullopt);
  Certificate cert6(cert3);
  cert6.setSignatureInfo(sigInfo);
  BOOST_CHECK_EQUAL(boost::lexical_cast<std::string>(cert6), expected6);
}

BOOST_AUTO_TEST_CASE(Helpers)
{
  BOOST_CHECK_EQUAL(extractIdentityFromCertName("/KEY/hello/world/v=1"), "/");
  BOOST_CHECK_EQUAL(extractIdentityFromCertName("/hello/world/KEY/!/self/v=42"), "/hello/world");

  BOOST_CHECK_THROW(extractIdentityFromCertName("/hello"), std::invalid_argument);
  BOOST_CHECK_THROW(extractIdentityFromCertName("/hello/KEY/keyid"), std::invalid_argument);
  BOOST_CHECK_THROW(extractIdentityFromCertName("/hello/KEY/keyid/issuer"), std::invalid_argument);
  BOOST_CHECK_THROW(extractIdentityFromCertName("/a/long/enough/but/invalid/name"), std::invalid_argument);

  BOOST_CHECK_EQUAL(extractKeyNameFromCertName("/KEY/hello/world/v=1"), "/KEY/hello");
  BOOST_CHECK_EQUAL(extractKeyNameFromCertName("/hello/world/KEY/!/self/v=42"), "/hello/world/KEY/!");

  BOOST_CHECK_THROW(extractKeyNameFromCertName("/hello"), std::invalid_argument);
  BOOST_CHECK_THROW(extractKeyNameFromCertName("/hello/KEY/keyid"), std::invalid_argument);
  BOOST_CHECK_THROW(extractKeyNameFromCertName("/hello/KEY/keyid/issuer"), std::invalid_argument);
  BOOST_CHECK_THROW(extractKeyNameFromCertName("/a/long/enough/but/invalid/name"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END() // TestCertificate
BOOST_AUTO_TEST_SUITE_END() // Security

} // namespace tests
} // inline namespace v2
} // namespace security
} // namespace ndn
