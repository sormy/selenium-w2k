// Copyright 2013 Software Freedom Conservancy
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "StringUtilities.h"

#include <sstream>

#define WHITESPACE " \n\r\t"
#define WIDE_WHITESPACE L" \n\r\t"

namespace webdriver {

StringUtilities::StringUtilities(void) {
}

StringUtilities::~StringUtilities(void) {
}

std::wstring StringUtilities::ToWString(const std::string& input) {
  // Assumption: The wstring character count will be the same as the length of
  // the string character count. Allocate the buffer with that many wchar_t items
  // so that the first MultiByteToWideChar call will succeed most of the time as
  // an optimization.
  std::wstring output = L"";
  int input_string_byte_count = static_cast<int>(input.size()) + 1;
  int wide_string_length = input_string_byte_count;
  std::vector<wchar_t> output_buffer(wide_string_length);
  bool convert_failed = (0 == ::MultiByteToWideChar(CP_UTF8,
                                                    0,
                                                    input.c_str(),
                                                    input_string_byte_count,
                                                    &output_buffer[0],
                                                    wide_string_length));
  if (convert_failed) {
    if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      // Buffer wasn't big enough. Call MultiByteToWideChar again with
      // NULL values to determine how big the buffer should be.
      wide_string_length = ::MultiByteToWideChar(CP_UTF8,
                                                 0,
                                                 input.c_str(),
                                                 input_string_byte_count,
                                                 NULL,
                                                 0);
      output_buffer.resize(wide_string_length);
      convert_failed = (0 == ::MultiByteToWideChar(CP_UTF8,
                                                   0,
                                                   input.c_str(),
                                                   input_string_byte_count,
                                                   &output_buffer[0],
                                                   wide_string_length));
      if (!convert_failed) {
        output = &output_buffer[0];
      }
    }
  } else {
    output = &output_buffer[0];
  }
  return output;
}

std::string StringUtilities::ToString(const std::wstring& input) {
  // Assumption: The byte count of the resulting narrow string will be at most
  // four times the character count of the input wstring. Allocate the buffer 
  // with that many char items (bytes) so that the first WideCharToMultiByte 
  // call will succeed most of the time as an optimization.
  std::string output = "";
  int wide_string_length = static_cast<int>(input.size()) + 1;
  int output_string_byte_count = wide_string_length * 4;
  std::vector<char> string_buffer(output_string_byte_count);
  bool convert_failed = (0 == ::WideCharToMultiByte(CP_UTF8,
                                                    0,
                                                    input.c_str(),
                                                    wide_string_length,
                                                    &string_buffer[0],
                                                    output_string_byte_count,
                                                    NULL,
                                                    NULL));
  if (convert_failed) {
    if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      // Buffer wasn't big enough. Call WideCharToMultiByte again with
      // NULL values to determine how big the buffer should be.
      output_string_byte_count = ::WideCharToMultiByte(CP_UTF8,
                                                       0,
                                                       input.c_str(),
                                                       wide_string_length,
                                                       NULL,
                                                       0,
                                                       NULL, 
                                                       NULL);
      string_buffer.resize(output_string_byte_count);
      convert_failed = (0 == ::WideCharToMultiByte(CP_UTF8,
                                                   0,
                                                   input.c_str(),
                                                   wide_string_length,
                                                   &string_buffer[0],
                                                   output_string_byte_count,
                                                   NULL,
                                                   NULL));
      if (!convert_failed) {
        output = &string_buffer[0];
      }
    }
  } else {
    output = &string_buffer[0];
  }
  return output;
}

std::wstring StringUtilities::ToWString(const long long input) {
  std::wostringstream os;
  os << input;
  return os.str();
}

std::string StringUtilities::ToString(const long long input) {
  std::ostringstream os;
  os << input;
  return os.str();
}

std::string StringUtilities::Format(const char* format, ...) {
  va_list args;
  va_start(args, format);
  size_t buffer_size = _vscprintf(format, args);
  std::vector<char> buffer(buffer_size + 1);
  _vsnprintf_s(&buffer[0], buffer.size(), buffer_size + 1, format, args);
  va_end(args);
  std::string formatted = &buffer[0];
  return formatted;
}

std::wstring StringUtilities::Format(const wchar_t* format, ...) {
  va_list args;
  va_start(args, format);
  size_t buffer_size = _vscwprintf(format, args);
  std::vector<wchar_t> buffer(buffer_size + 1);
  _vsnwprintf_s(&buffer[0], buffer.size(), buffer_size + 1, format, args);
  va_end(args);
  std::wstring formatted = &buffer[0];
  return formatted;
}

void StringUtilities::ToBuffer(const std::string& input, std::vector<char>* buffer) {
  buffer->resize(input.size() + 1);
  strcpy_s(&((*buffer)[0]), buffer->size(), input.c_str());
  (*buffer)[buffer->size() - 1] = L'\0';
}

void StringUtilities::ToBuffer(const std::wstring& input, std::vector<wchar_t>* buffer) {
  buffer->resize(input.size() + 1);
  wcscpy_s(&((*buffer)[0]), buffer->size(), input.c_str());
  (*buffer)[buffer->size() - 1] = L'\0';
}

std::string StringUtilities::Trim(const std::string& input) {
    return TrimRight(TrimLeft(input));
}

std::string StringUtilities::TrimLeft(const std::string& input) {
    size_t startpos = input.find_first_not_of(WHITESPACE);
    return (startpos == std::string::npos) ? "" : input.substr(startpos);
}

std::string StringUtilities::TrimRight(const std::string& input) {
    size_t endpos = input.find_last_not_of(WHITESPACE);
    return (endpos == std::string::npos) ? "" : input.substr(0, endpos + 1);
}

std::wstring StringUtilities::Trim(const std::wstring& input) {
    return TrimRight(TrimLeft(input));
}

std::wstring StringUtilities::TrimLeft(const std::wstring& input) {
    size_t startpos = input.find_first_not_of(WIDE_WHITESPACE);
    return (startpos == std::wstring::npos) ? L"" : input.substr(startpos);
}

std::wstring StringUtilities::TrimRight(const std::wstring& input) {
    size_t endpos = input.find_last_not_of(WIDE_WHITESPACE);
    return (endpos == std::wstring::npos) ? L"" : input.substr(0, endpos + 1);
}

} // namespace webdriver