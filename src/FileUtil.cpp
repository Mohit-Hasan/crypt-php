#include "FileUtil.h"
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

bool FileUtil::MirrorDirectory(const fs::path &src, const fs::path &dst,
                               const vector<string> &excludes,
                               function<void(const string &)> logger) {
  try {
    if (!fs::exists(src)) {
      if (logger)
        logger("Sync Error: Source does not exist: " + src.string());
      return false;
    }
    if (!fs::exists(dst))
      fs::create_directories(dst);

    for (const auto &entry : fs::recursive_directory_iterator(src)) {
      const auto &path = entry.path();
      string relPath = fs::relative(path, src).string();

      // Robust exclusion check: Match prefix or substring
      bool skip = false;
      for (const auto &pattern : excludes) {
        if (pattern.empty())
          continue;
        if (relPath == pattern ||
            relPath.compare(0, pattern.length(), pattern) == 0 ||
            relPath.find("/" + pattern) != string::npos) {
          skip = true;
          break;
        }
      }
      if (skip)
        continue;

      auto destPath = dst / relPath;
      if (entry.is_directory()) {
        fs::create_directories(destPath);
      } else if (entry.is_regular_file()) {
        fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
      }
    }
  } catch (const exception &e) {
    if (logger)
      logger("Sync Error: " + string(e.what()));
    return false;
  }
  return true;
}

bool FileUtil::StripLines(const fs::path &filePath, int startLine,
                          int endLine) {
  try {
    ifstream ifs(filePath);
    if (!ifs.is_open())
      return false;

    vector<string> lines;
    string line;
    int currentLine = 1;
    while (getline(ifs, line)) {
      if (currentLine < startLine || currentLine > endLine) {
        lines.push_back(line);
      }
      currentLine++;
    }
    ifs.close();

    ofstream ofs(filePath, ios::trunc);
    for (const auto &l : lines) {
      ofs << l << "\n";
    }
    return true;
  } catch (...) {
    return false;
  }
}

bool FileUtil::ZipDirectory(const fs::path &dir, const fs::path &zipFile) {
  // Single quotes for shell safety on Mac/Linux
  string cmd =
      "cd \"" + dir.string() + "\" && zip -rq \"" + zipFile.string() + "\" .";

#ifdef _WIN32
  cmd = "powershell -Command \"Compress-Archive -Path '" + dir.string() +
        "/*' -DestinationPath '" + zipFile.string() + "' -Force\"";
#endif

  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe)
    return false;
  return pclose(pipe) == 0;
}

vector<string> FileUtil::ListDirectories(const fs::path &root) {
  vector<string> dirs;
  if (!fs::exists(root) || !fs::is_directory(root))
    return dirs;

  for (const auto &entry : fs::directory_iterator(root)) {
    if (entry.is_directory() && entry.path().filename().string()[0] != '.') {
      dirs.push_back(entry.path().filename().string());
    }
  }
  return dirs;
}

bool FileUtil::MergeDirectory(const fs::path &src, const fs::path &dst,
                              function<void(const string &)> logger) {
  try {
    if (!fs::exists(src))
      return false;
    if (!fs::exists(dst))
      fs::create_directories(dst);

    for (const auto &entry : fs::recursive_directory_iterator(src)) {
      const auto &path = entry.path();
      auto rel = fs::relative(path, src);
      auto destPath = dst / rel;

      if (entry.is_directory()) {
        fs::create_directories(destPath);
      } else if (entry.is_regular_file()) {
        fs::create_directories(destPath.parent_path());
        fs::copy_file(path, destPath, fs::copy_options::overwrite_existing);
      }
    }
    return true;
  } catch (const exception &e) {
    if (logger)
      logger("Merge Error: " + string(e.what()));
    return false;
  }
}
