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

#ifndef WEBDRIVER_IE_STRINGUTILITIES_H
#define WEBDRIVER_IE_STRINGUTILITIES_H

#include <string>
#include <vector>

namespace webdriver {

class StringUtilities {
 private:
  StringUtilities(void);
  ~StringUtilities(void);
 public:
  static std::wstring ToWString(const std::string& input);
  static std::string ToString(const std::wstring& input);

  // str::to_wstring() and str::to_string() are not available in Visual Studio 2008
  static std::wstring ToWString(const long long input);
  static std::string ToString(const long long input);

  static std::string Format(const char* format, ...);
  static std::wstring Format(const wchar_t* format, ...);
  
  static std::string TrimRight(const std::string& input);
  static std::string TrimLeft(const std::string& input);
  static std::string Trim(const std::string& input);
  static std::wstring TrimRight(const std::wstring& input);
  static std::wstring TrimLeft(const std::wstring& input);
  static std::wstring Trim(const std::wstring& input);

  static void ToBuffer(const std::string& input, std::vector<char>* buffer);
  static void ToBuffer(const std::wstring& input, std::vector<wchar_t>* buffer);
};

} // namespace webdriver

#endif  // WEBDRIVER_IE_STRINGUTILITIES_H