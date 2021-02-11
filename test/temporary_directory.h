//
//  temporary_directory.h
//  sun_ray_test
//
//  Created by Lars-Christian Fürstenberg on 03.03.20.
//  Copyright © 2020 Lars-Christian Fürstenberg. All rights reserved.
//

#pragma once

#include <array>
#include <filesystem>

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <windows.h>
#else
#ifdef __linux__
  #include <linux/limits.h>
#endif
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <unistd.h>
#endif


class TemporaryDirectoryGuard
{
public:
  TemporaryDirectoryGuard()
  : path_{unique_temp_directory_path()}
  {
  }

  TemporaryDirectoryGuard(const TemporaryDirectoryGuard&) = delete;
  TemporaryDirectoryGuard(TemporaryDirectoryGuard&&) = default;
  TemporaryDirectoryGuard& operator=(const TemporaryDirectoryGuard&) = delete;
  TemporaryDirectoryGuard& operator=(TemporaryDirectoryGuard&&) = delete;

  const std::filesystem::path& temporary_directory_path() const
  {
    return path_;
  }

  ~TemporaryDirectoryGuard()
  {
    std::error_code ec;
    if (std::filesystem::equivalent(std::filesystem::current_path(), path_, ec)) {
      std::filesystem::current_path("..");
    }

    if (!ec) {
      std::filesystem::remove_all(path_, ec);
    }
  }

private:
  static std::filesystem::path unique_temp_directory_path()
  {
    std::error_code ec;
    auto path = unique_temp_directory_path(ec);
    if (ec) {
      throw std::runtime_error{"Cannot create temp path: " + ec.message()};
    }
    return path;
  }

  static std::filesystem::path unique_temp_directory_path(std::error_code& ec)
  {
    std::filesystem::path p{std::filesystem::temp_directory_path(ec)};

    if (ec) {
      return std::filesystem::path{};
    }

#ifdef WIN32
    WCHAR tmp_dir[MAX_PATH];
    if (GetTempFileNameW(p.c_str(), L"mictlan", 0, tmp_dir) == 0) {
      ec.assign(GetLastError(), std::system_category());
    }
    std::filesystem::remove(tmp_dir);
    std::filesystem::create_directories(tmp_dir);
#else
    std::array<char, PATH_MAX> tmpl = {};
    ::strncpy(tmpl.data(), (p / "mictlan_XXXXXX").c_str(), PATH_MAX);

    const char* tmp_dir = ::mkdtemp(tmpl.data());
    if (tmp_dir == nullptr) {
      ec.assign(errno, std::system_category());
    }
#endif
    
    return std::filesystem::path{tmp_dir};
  }

  std::filesystem::path path_;
};
