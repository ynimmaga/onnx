/*
 * SPDX-License-Identifier: Apache-2.0
 */

// Copyright (c) ONNX Project Contributors.

#pragma once

#ifdef _WIN32
// windows.h has preproc definitions for min and max, which prevents from using std::min and std::max.
//  defining NOMINMAX disables the preproc macro.
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include "onnx/checker.h"
#endif

namespace ONNX_NAMESPACE {

#ifdef _WIN32
inline std::wstring utf8str_to_wstring(const std::string& utf8str, bool try_decode = false) {
  if (utf8str.empty()) {
    return std::wstring();
  }
  auto size_required =
      MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS | MB_PRECOMPOSED, utf8str.c_str(), -1, nullptr, 0);
  if (size_required == 0) {
    if (try_decode) {
      return std::wstring(
          reinterpret_cast<const wchar_t*>(utf8str.c_str()), utf8str.size() / sizeof(std::wstring::value_type));
    }
    auto last_error = GetLastError();
    fail_check("MultiByteToWideChar in utf8str_to_wstring returned error:", last_error);
  }
  std::wstring ws_str(size_required, 0);
  auto converted_size = MultiByteToWideChar(
      CP_UTF8, MB_ERR_INVALID_CHARS | MB_PRECOMPOSED, utf8str.c_str(), -1, &ws_str[0], size_required);
  if (converted_size == 0) {
    auto last_error = GetLastError();
    fail_check("MultiByteToWideChar in utf8str_to_wstring returned error:", last_error);
  }
  if (ws_str.back() == '\0') {
    ws_str.pop_back();
  }
  return ws_str;
}

inline std::string wstring_to_utf8str(const std::wstring& ws_str) {
  if (ws_str.empty()) {
    return std::string();
  }
  auto size_required =
      WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, ws_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
  if (size_required == 0) {
    auto last_error = GetLastError();
    fail_check("WideCharToMultiByte in wstring_to_utf8str returned error:", last_error);
  }
  std::string utf8str(size_required, 0);
  auto converted_size = WideCharToMultiByte(
      CP_UTF8, WC_ERR_INVALID_CHARS, ws_str.c_str(), -1, &utf8str[0], size_required, nullptr, nullptr);
  if (converted_size == 0) {
    auto last_error = GetLastError();
    fail_check("WideCharToMultiByte in wstring_to_utf8str returned error:", last_error);
  }
  if (utf8str.back() == '\0') {
    utf8str.pop_back();
  }
  return utf8str;
}

#endif

} // namespace ONNX_NAMESPACE
