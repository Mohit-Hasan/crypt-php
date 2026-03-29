#pragma once
#include <string>
#include <vector>
using namespace std;

class ResourceManager {
public:
  //extracts all embedded files to the target path.
  //returns the absolute path to the main yakpro-po.php script.
  static string ExtractAll(const string &target_path);

  //cleans up extracted files.
  static void Cleanup(const string &target_path);

private:
  static bool WriteFile(const string &path, const unsigned char *data,
                        size_t size);
};
