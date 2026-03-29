#include "ResourceManager.h"
#include "EmbeddedResources.h"
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace std;


string ResourceManager::ExtractAll(const string &target_path) {
  try {
    filesystem::path target(target_path);
    if (!filesystem::exists(target)) {
      filesystem::create_directories(target);
    }

    string yakpro_main_path = "";

    for (size_t i = 0; i < g_EmbeddedFilesCount; ++i) {
      filesystem::path rel_path(g_EmbeddedFiles[i].relativePath);
      filesystem::path full_dest_path = target / rel_path;

      //ensure parent directory exists
      filesystem::create_directories(full_dest_path.parent_path());

      //write the file
      if (WriteFile(full_dest_path.string(), g_EmbeddedFiles[i].data,
                    g_EmbeddedFiles[i].size)) {
        //return path to yakpro-po.php
        if (rel_path.filename().string() == "yakpro-po.php") {
          yakpro_main_path = full_dest_path.string();
        }
      }
    }

    return yakpro_main_path;

  } catch (const exception &e) {
    cerr << "Filesystem error during extraction: " << e.what() << endl;
    return "";
  }
}

bool ResourceManager::WriteFile(const string &path, const unsigned char *data,
                                size_t size) {
  ofstream ofs(path, ios::binary);
  if (!ofs.is_open())
    return false;
  ofs.write(reinterpret_cast<const char *>(data), size);
  ofs.close();
  return true;
}

void ResourceManager::Cleanup(const string &target_path) {
  try {
    if (filesystem::exists(target_path)) {
      filesystem::remove_all(target_path);
    }
  } catch (...) {
    // best effort
  }
}
