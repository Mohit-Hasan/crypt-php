#pragma once
#include <filesystem>
#include <functional>
#include <string>
#include <vector>
using namespace std;

namespace fs = filesystem;

namespace FileUtil {
//mirror only specific directories from source to target
bool MirrorDirectory(const fs::path &src, const fs::path &dst,
                     const vector<string> &excludes,
                     function<void(const string &)> logger);

//strips specified lines from a file
bool StripLines(const fs::path &filePath, int startLine, int endLine);

//zip a directory into a file
bool ZipDirectory(const fs::path &dir, const fs::path &zipFile);
//merges contents of src into dst recursively
bool MergeDirectory(const fs::path &src, const fs::path &dst,
                    function<void(const string &)> logger = nullptr);

//lists top-level directories for selection in gui
vector<string> ListDirectories(const fs::path &root);
}
