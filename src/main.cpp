#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "CryptPHP.h"
#include "FileUtil.h"
#include "ResourceManager.h"
using namespace std;

#define BRAND_VERSION "v1.0.0"

static void glfw_error_callback(int error, const char *description) {
  cerr << "GLFW Error " << error << ": " << description << "\n";
}

struct AppState {
  char projectPath[512] = "";
  char version[64] = "v1.0.0";
  char seedPath[512] = "";
  char ignoreList[2048] =
      ".git\nnode_modules\ntests\nbuild\ndist\n.env\n.DS_Store\n"
      "public/storage\npublic/hot\nbootstrap/cache\n"
      "storage/app/public\nstorage/debugbar\n"
      "storage/framework/cache\nstorage/framework/sessions\n"
      "storage/framework/views\nstorage/logs\n"
      "*.editorconfig\n*.md\n.gitattributes\n.gitignore\n*.zip\n*.cnf\n";
  char updateIncludes[1024] =
      "app\nconfig\ndatabase\nroutes\nresources\ncomposer.json\ncomposer."
      "lock\npackage.json\npackage-lock.json";
  char fullIncludes[1024] =
      "app\nbootstrap\nconfig\ndatabase\npublic\nresources\nroutes\nstorage\nve"
      "ndor\nartisan\ncomposer.json\ncomposer.lock\npackage.json";
  char userComment[1024] = "";

  map<string, bool> yakProSelections;
  map<string, bool> layer2Selections;

  vector<string> consoleLogs;
  mutex logMutex;
  CryptPHP cryptphp;
  CryptPHP::Config config;

  char ign_constants[512] = "";
  char ign_variables[512] = "";
  char ign_functions[512] = "";
  char ign_class_constants[512] = "";
  char ign_methods[512] = "";
  char ign_properties[512] = "";
  char ign_classes[512] = "";
  char ign_interfaces[512] = "";
  char ign_traits[512] = "";
  char ign_namespaces[512] = "";
  char ign_labels[512] = "";
  char ign_constants_prefix[512] = "";
  char ign_variables_prefix[512] = "";
  char ign_functions_prefix[512] = "";
  char ign_class_constants_prefix[512] = "";
  char ign_methods_prefix[512] = "";
  char ign_properties_prefix[512] = "";
  char ign_classes_prefix[512] = "";
  char ign_interfaces_prefix[512] = "";
  char ign_traits_prefix[512] = "";
  char ign_namespaces_prefix[512] = "";
  char ign_labels_prefix[512] = "";
};

//styles
void SetupModernStyle() {
  ImGuiStyle &s = ImGui::GetStyle();
  s.WindowRounding = 0.f;
  s.ChildRounding = 10.f;
  s.FrameRounding = 8.f;
  s.GrabRounding = 8.f;
  s.ScrollbarRounding = 8.f;
  s.PopupRounding = 8.f;
  s.TabRounding = 8.f;
  s.WindowBorderSize = 0.f;
  s.ChildBorderSize = 0.f;
  s.FrameBorderSize = 0.f;
  s.WindowPadding = {12, 12};
  s.FramePadding = {12, 8};
  s.ItemSpacing = {12, 10};
  s.ItemInnerSpacing = {8, 6};
  s.IndentSpacing = 24.f;
  s.ScrollbarSize = 12.f;
  s.TabBorderSize = 0.f;

  ImVec4 *c = s.Colors;
  //deep slate/oceanic base
  c[ImGuiCol_WindowBg] = {0.14f, 0.16f, 0.19f, 1.f};
  c[ImGuiCol_ChildBg] = {0.18f, 0.20f, 0.24f, 1.f};
  c[ImGuiCol_PopupBg] = {0.18f, 0.20f, 0.24f, 0.96f};
  c[ImGuiCol_Border] = {0.26f, 0.30f, 0.36f, 0.5f};
  //input fields
  c[ImGuiCol_FrameBg] = {0.11f, 0.13f, 0.16f, 1.f};
  c[ImGuiCol_FrameBgHovered] = {0.16f, 0.18f, 0.22f, 1.f};
  c[ImGuiCol_FrameBgActive] = {0.19f, 0.22f, 0.26f, 1.f};
  //accent — soft teal/oceanic green
  c[ImGuiCol_Button] = {0.16f, 0.60f, 0.52f, 1.f};
  c[ImGuiCol_ButtonHovered] = {0.20f, 0.68f, 0.60f, 1.f};
  c[ImGuiCol_ButtonActive] = {0.25f, 0.75f, 0.68f, 1.f};
  //headers
  c[ImGuiCol_Header] = {0.16f, 0.60f, 0.52f, 0.45f};
  c[ImGuiCol_HeaderHovered] = {0.20f, 0.68f, 0.60f, 0.75f};
  c[ImGuiCol_HeaderActive] = {0.25f, 0.75f, 0.68f, 1.f};
  //tabs
  c[ImGuiCol_Tab] = {0.16f, 0.18f, 0.22f, 1.f};
  c[ImGuiCol_TabHovered] = {0.20f, 0.68f, 0.60f, 0.90f};
  c[ImGuiCol_TabActive] = {0.16f, 0.60f, 0.52f, 1.f};
  c[ImGuiCol_TabUnfocused] = {0.14f, 0.16f, 0.19f, 1.f};
  c[ImGuiCol_TabUnfocusedActive] = {0.16f, 0.40f, 0.35f, 1.f};
  //title
  c[ImGuiCol_TitleBg] = {0.12f, 0.14f, 0.17f, 1.f};
  c[ImGuiCol_TitleBgActive] = {0.12f, 0.14f, 0.17f, 1.f};
  //scrollbar
  c[ImGuiCol_ScrollbarBg] = {0.11f, 0.13f, 0.16f, 0.5f};
  c[ImGuiCol_ScrollbarGrab] = {0.26f, 0.30f, 0.36f, 0.7f};
  c[ImGuiCol_ScrollbarGrabHovered] = {0.32f, 0.38f, 0.44f, 1.f};
  c[ImGuiCol_ScrollbarGrabActive] = {0.40f, 0.46f, 0.52f, 1.f};
  //widgets
  c[ImGuiCol_CheckMark] = {0.40f, 0.85f, 0.75f, 1.f};
  c[ImGuiCol_SliderGrab] = {0.20f, 0.68f, 0.60f, 1.f};
  c[ImGuiCol_SliderGrabActive] = {0.25f, 0.75f, 0.68f, 1.f};
  c[ImGuiCol_Separator] = {0.26f, 0.30f, 0.36f, 1.f};
  c[ImGuiCol_SeparatorHovered] = {0.32f, 0.68f, 0.60f, 1.f};
  c[ImGuiCol_ResizeGrip] = {0.16f, 0.60f, 0.52f, 0.4f};
  c[ImGuiCol_ResizeGripHovered] = {0.20f, 0.68f, 0.60f, 0.7f};
  c[ImGuiCol_ResizeGripActive] = {0.25f, 0.75f, 0.68f, 1.f};
  //text
  c[ImGuiCol_Text] = {0.85f, 0.88f, 0.90f, 1.f};
  c[ImGuiCol_TextDisabled] = {0.50f, 0.55f, 0.60f, 1.f};
}

#ifdef _WIN32
#include <windows.h>
#endif

// helpers
static void OpenURL(const char *url) {
#ifdef __APPLE__
  string cmd = "open \"";
  cmd += url;
  cmd += "\"";
  system(cmd.c_str());
#elif defined(_WIN32)
  string cmd = "start \"\" \"";
  cmd += url;
  cmd += "\"";
  system(cmd.c_str());
#else
  string cmd = "xdg-open \"";
  cmd += url;
  cmd += "\"";
  system(cmd.c_str());
#endif
}

static void SectionLabel(const char *txt) {
  ImGui::Spacing();
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.85f, 0.75f, 1.f));
  ImGui::Text("%s", txt);
  ImGui::PopStyleColor();
  ImGui::Spacing();
}

static bool IsIgnored(const string &relPath, const char *ignoreListText) {
  if (relPath.empty())
    return false;
  stringstream ss(ignoreListText);
  string p;
  while (getline(ss, p, '\n')) {
    if (p.empty())
      continue;
    //normalize: remove leading/trailing spaces
    size_t f = p.find_first_not_of(" \t\r\n");
    if (f == string::npos)
      continue;
    size_t l = p.find_last_not_of(" \t\r\n");
    string pat = p.substr(f, l - f + 1);

    if (relPath == pat)
      return true;
    if (relPath.find(pat + "/") == 0)
      return true;
    //check if path is part of an ignored directory
    if (pat.back() != '/' && relPath.find(pat + "/") == 0)
      return true;
    if (pat.back() == '/' && relPath.find(pat) == 0)
      return true;
  }
  return false;
}
//config state management
static string trim(const string &str) {
  size_t first = str.find_first_not_of(" \t\r\n");
  if (string::npos == first)
    return "";
  size_t last = str.find_last_not_of(" \t\r\n");
  return str.substr(first, (last - first + 1));
}

void SaveState(const AppState &st) {
  filesystem::path conf = filesystem::path(st.projectPath) / "cryptphp.cnf";
  ofstream out(conf);
  if (!out)
    return;

  auto writeVal = [&](const char *k, const char *v) {
    string val = v;
    replace(val.begin(), val.end(), '\n', '|');
    out << k << "=" << val << "\n";
  };

  writeVal("Version", st.version);
  writeVal("SeedPath", st.seedPath);
  writeVal("IgnoreList", st.ignoreList);
  writeVal("UpdateIncludes", st.updateIncludes);
  writeVal("FullIncludes", st.fullIncludes);
  writeVal("UserComment", st.userComment);
  writeVal("IgnConst", st.ign_constants);
  writeVal("IgnVar", st.ign_variables);
  writeVal("IgnFunc", st.ign_functions);
  writeVal("IgnMethod", st.ign_methods);
  writeVal("IgnProp", st.ign_properties);
  writeVal("IgnClass", st.ign_classes);
  writeVal("IgnNS", st.ign_namespaces);

  auto &y = st.config.yakPro;
  writeVal("YP_ObfConst", y.obfuscate_constant_name ? "1" : "0");
  writeVal("YP_ObfVar", y.obfuscate_variable_name ? "1" : "0");
  writeVal("YP_ObfFunc", y.obfuscate_function_name ? "1" : "0");
  writeVal("YP_ObfClass", y.obfuscate_class_name ? "1" : "0");
  writeVal("YP_ObfIfce", y.obfuscate_interface_name ? "1" : "0");
  writeVal("YP_ObfTrait", y.obfuscate_trait_name ? "1" : "0");
  writeVal("YP_ObfClConst", y.obfuscate_class_constant_name ? "1" : "0");
  writeVal("YP_ObfProp", y.obfuscate_property_name ? "1" : "0");
  writeVal("YP_ObfMeth", y.obfuscate_method_name ? "1" : "0");
  writeVal("YP_ObfNS", y.obfuscate_namespace_name ? "1" : "0");
  writeVal("YP_ObfLbl", y.obfuscate_label_name ? "1" : "0");
  writeVal("YP_ObfIf", y.obfuscate_if_statement ? "1" : "0");
  writeVal("YP_ObfLoop", y.obfuscate_loop_statement ? "1" : "0");
  writeVal("YP_ObfStr", y.obfuscate_string_literal ? "1" : "0");
  writeVal("YP_Shuf", y.shuffle_stmts ? "1" : "0");
  writeVal("YP_ShufMin", to_string(y.shuffle_stmts_min_chunk_size).c_str());
  writeVal("YP_ShufMode", y.shuffle_stmts_chunk_mode.c_str());
  writeVal("YP_ShufRatio", to_string(y.shuffle_stmts_chunk_ratio).c_str());
  writeVal("YP_StripInd", y.strip_indentation ? "1" : "0");
  writeVal("YP_AbortErr", y.abort_on_error ? "1" : "0");
  writeVal("YP_AllowEmp", y.allow_and_overwrite_empty_files ? "1" : "0");
  writeVal("YP_Parser", y.parser_mode.c_str());
  writeVal("YP_ScramMode", y.scramble_mode.c_str());
  writeVal("YP_ScramLen", to_string(y.scramble_length).c_str());
  writeVal("YP_TgnPD", y.t_ignore_pre_defined_classes.c_str());

  // save YakPro selections (phase 1)
  string obfFiles = "";
  for (auto const &[f, sel] : st.yakProSelections) {
    if (sel)
      obfFiles += f + ",";
  }
  writeVal("ObfFiles", obfFiles.c_str());

  // save layer2 files (phase 2)
  string layer2Str = "";
  for (auto const &[f, sel] : st.layer2Selections) {
    if (sel)
      layer2Str += f + ",";
  }
  writeVal("Layer2Files", layer2Str.c_str());
}

void LoadState(AppState &st) {
  filesystem::path conf = filesystem::path(st.projectPath) / "cryptphp.cnf";
  ifstream in(conf);
  if (!in)
    return;

  auto copyVal = [](char *dst, size_t sz, string val) {
    replace(val.begin(), val.end(), '|', '\n');
    strncpy(dst, val.c_str(), sz - 1);
    dst[sz - 1] = '\0';
  };

  string line;
  vector<string> loadedObfPaths;
  vector<string> loadedLayer2;

  while (getline(in, line)) {
    size_t eq = line.find('=');
    if (eq == string::npos)
      continue;
    string k = trim(line.substr(0, eq));
    string v = trim(line.substr(eq + 1));

    if (k == "Version")
      copyVal(st.version, sizeof(st.version), v);
    else if (k == "SeedPath")
      copyVal(st.seedPath, sizeof(st.seedPath), v);
    else if (k == "IgnoreList")
      copyVal(st.ignoreList, sizeof(st.ignoreList), v);
    else if (k == "UpdateIncludes")
      copyVal(st.updateIncludes, sizeof(st.updateIncludes), v);
    else if (k == "FullIncludes")
      copyVal(st.fullIncludes, sizeof(st.fullIncludes), v);
    else if (k == "UserComment")
      copyVal(st.userComment, sizeof(st.userComment), v);
    else if (k == "IgnConst")
      copyVal(st.ign_constants, sizeof(st.ign_constants), v);
    else if (k == "IgnVar")
      copyVal(st.ign_variables, sizeof(st.ign_variables), v);
    else if (k == "IgnFunc")
      copyVal(st.ign_functions, sizeof(st.ign_functions), v);
    else if (k == "IgnMethod")
      copyVal(st.ign_methods, sizeof(st.ign_methods), v);
    else if (k == "IgnProp")
      copyVal(st.ign_properties, sizeof(st.ign_properties), v);
    else if (k == "IgnClass")
      copyVal(st.ign_classes, sizeof(st.ign_classes), v);
    else if (k == "IgnTrait")
      copyVal(st.ign_traits, sizeof(st.ign_traits), v);
    else if (k == "IgnNS")
      copyVal(st.ign_namespaces, sizeof(st.ign_namespaces), v);
    else if (k == "YP_ObfConst")
      st.config.yakPro.obfuscate_constant_name = (v == "1");
    else if (k == "YP_ObfVar")
      st.config.yakPro.obfuscate_variable_name = (v == "1");
    else if (k == "YP_ObfFunc")
      st.config.yakPro.obfuscate_function_name = (v == "1");
    else if (k == "YP_ObfClass")
      st.config.yakPro.obfuscate_class_name = (v == "1");
    else if (k == "YP_ObfIfce")
      st.config.yakPro.obfuscate_interface_name = (v == "1");
    else if (k == "YP_ObfTrait")
      st.config.yakPro.obfuscate_trait_name = (v == "1");
    else if (k == "YP_ObfClConst")
      st.config.yakPro.obfuscate_class_constant_name = (v == "1");
    else if (k == "YP_ObfProp")
      st.config.yakPro.obfuscate_property_name = (v == "1");
    else if (k == "YP_ObfMeth")
      st.config.yakPro.obfuscate_method_name = (v == "1");
    else if (k == "YP_ObfNS")
      st.config.yakPro.obfuscate_namespace_name = (v == "1");
    else if (k == "YP_ObfLbl")
      st.config.yakPro.obfuscate_label_name = (v == "1");
    else if (k == "YP_ObfIf")
      st.config.yakPro.obfuscate_if_statement = (v == "1");
    else if (k == "YP_ObfLoop")
      st.config.yakPro.obfuscate_loop_statement = (v == "1");
    else if (k == "YP_ObfStr")
      st.config.yakPro.obfuscate_string_literal = (v == "1");
    else if (k == "YP_Shuf")
      st.config.yakPro.shuffle_stmts = (v == "1");
    else if (k == "YP_ShufMin")
      st.config.yakPro.shuffle_stmts_min_chunk_size = stoi(v);
    else if (k == "YP_ShufMode")
      st.config.yakPro.shuffle_stmts_chunk_mode = v;
    else if (k == "YP_ShufRatio")
      st.config.yakPro.shuffle_stmts_chunk_ratio = stoi(v);
    else if (k == "YP_StripInd")
      st.config.yakPro.strip_indentation = (v == "1");
    else if (k == "YP_AbortErr")
      st.config.yakPro.abort_on_error = (v == "1");
    else if (k == "YP_AllowEmp")
      st.config.yakPro.allow_and_overwrite_empty_files = (v == "1");
    else if (k == "YP_Parser")
      st.config.yakPro.parser_mode = v;
    else if (k == "YP_ScramMode")
      st.config.yakPro.scramble_mode = v;
    else if (k == "YP_ScramLen")
      st.config.yakPro.scramble_length = stoi(v);
    else if (k == "YP_TgnPD")
      st.config.yakPro.t_ignore_pre_defined_classes = v;
    else if (k == "ObfFiles" || k == "ObfFolders" || k == "Layer2Files") {
      stringstream ss(v);
      string token;
      while (getline(ss, token, ',')) {
        if (!token.empty()) {
          if (k == "Layer2Files")
            loadedLayer2.push_back(token);
          else
            loadedObfPaths.push_back(token);
        }
      }
    }
  }

  //apply loaded selections if available
  for (auto const &f : loadedObfPaths) {
    if (st.yakProSelections.count(f))
      st.yakProSelections[f] = true;
  }
  for (auto const &f : loadedLayer2) {
    if (st.layer2Selections.count(f))
      st.layer2Selections[f] = true;
  }
}

void ScanProject(AppState &state) {
  state.yakProSelections.clear();
  state.layer2Selections.clear();
  filesystem::path root(state.projectPath);
  if (!filesystem::exists(root) || !filesystem::is_directory(root))
    return;

  for (auto &e : filesystem::recursive_directory_iterator(root)) {
    string rel = filesystem::relative(e.path(), root).string();
    if (IsIgnored(rel, state.ignoreList))
      continue;

    bool isCore = (rel.find("app") == 0 || rel.find("bootstrap") == 0 ||
                   rel.find("config") == 0 || rel.find("database") == 0 ||
                   rel.find("routes") == 0 || rel == "artisan");

    if (e.is_regular_file() && e.path().extension() == ".php") {
      state.yakProSelections[rel] = isCore;
      state.layer2Selections[rel] = false;
    } else if (e.is_directory()) {
      state.yakProSelections[rel] = isCore;
      state.layer2Selections[rel] = false;
    }
  }
}

void SelectRecursively(map<string, bool> &selections, const filesystem::path &d,
                       const filesystem::path &root, bool v,
                       const char *ignoreList) {
  string dRel = filesystem::relative(d, root).string();
  selections[dRel] = v;

  for (auto &e : filesystem::recursive_directory_iterator(d)) {
    string rel = filesystem::relative(e.path(), root).string();
    if (IsIgnored(rel, ignoreList))
      continue;

    if (e.is_directory() ||
        (e.is_regular_file() && e.path().extension() == ".php")) {
      selections[rel] = v;
    }
  }
}

void UncheckAncestors(map<string, bool> &selections, const filesystem::path &p,
                      const filesystem::path &root) {
  auto cur = p.parent_path();
  while (cur.string().size() >= root.string().size()) {
    string rel = filesystem::relative(cur, root).string();
    if (rel == "." || rel.empty())
      break;
    selections[rel] = false;
    cur = cur.parent_path();
  }
}

void DrawFileTree(map<string, bool> &selections, const filesystem::path &root,
                  const filesystem::path &cur, const char *ignoreList) {
  //collect entries to sort them
  vector<filesystem::directory_entry> entries;
  for (auto &entry : filesystem::directory_iterator(cur)) {
    entries.push_back(entry);
  }
  sort(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
    if (a.is_directory() && !b.is_directory())
      return true;
    if (!a.is_directory() && b.is_directory())
      return false;
    return a.path().filename().string() < b.path().filename().string();
  });

  for (auto &entry : entries) {
    auto &p = entry.path();
    string fname = p.filename().string();
    if (fname[0] == '.')
      continue;

    string rel = filesystem::relative(p, root).string();
    if (IsIgnored(rel, ignoreList))
      continue;

    if (entry.is_directory()) {
      bool sel = selections[rel];
      ImGui::PushStyleColor(ImGuiCol_Text, {0.60f, 0.75f, 0.85f, 1.f});
      if (ImGui::Checkbox(("##cb" + rel).c_str(), &sel)) {
        SelectRecursively(selections, p, root, sel, ignoreList);
        if (!sel)
          UncheckAncestors(selections, p, root);
      }
      ImGui::SameLine();
      bool open = ImGui::TreeNode(fname.c_str());
      ImGui::PopStyleColor();

      if (open) {
        DrawFileTree(selections, root, p, ignoreList);
        ImGui::TreePop();
      }
    } else if (entry.is_regular_file() && p.extension() == ".php") {
      bool sel = selections[rel];
      if (ImGui::Checkbox(fname.c_str(), &sel)) {
        selections[rel] = sel;
        if (!sel)
          UncheckAncestors(selections, p, root);
      }
    }
  }
}

static const char *PhaseName(CryptPHP::Phase p) {
  switch (p) {
  case CryptPHP::Phase::Ready:
    return "Ready";
  case CryptPHP::Phase::Syncing:
    return "Mirroring...";
  case CryptPHP::Phase::YAKProRunning:
    return "Obfuscating...";
  case CryptPHP::Phase::StrippingBanners:
    return "Strip & Comment Processing...";
  case CryptPHP::Phase::Layer2Encoding:
    return "Layer-2...";
  case CryptPHP::Phase::SeedInjection:
    return "Seeding...";
  case CryptPHP::Phase::Packaging:
    return "Packaging...";
  case CryptPHP::Phase::Completed:
    return "Done";
  case CryptPHP::Phase::Failed:
    return "Failed";
  default:
    return "?";
  }
}

int main(int, char **) {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

#ifdef __APPLE__
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
  glfwWindowHint(GLFW_SAMPLES, 4);

  GLFWwindow *window = glfwCreateWindow(
      1440, 960, "CryptPHP - Obfuscator for PHP", nullptr, nullptr);
  if (!window)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.IniFilename = nullptr;
  io.FontGlobalScale = 1.15f;
  SetupModernStyle();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  AppState state;
  state.cryptphp.setLogCallback([&](const string &msg) {
    lock_guard<mutex> lock(state.logMutex);
    state.consoleLogs.push_back(msg);
  });

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImVec2 sz = io.DisplaySize;
    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize(sz);
    ImGui::Begin("##root", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus |
                     ImGuiWindowFlags_NoScrollbar);

    auto phase = state.cryptphp.getCurrentPhase();
    const float pad = 16.f;
    const float leftW = sz.x * 0.60f;
    const float rightW = sz.x - leftW - pad * 3;
    const float contentH = sz.y - pad * 2;

    // left panel
    ImGui::SetCursorPos({pad, pad});
    ImGui::BeginChild("LeftPanel", {leftW, contentH}, true);

    if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_FittingPolicyScroll)) {

      // tab 1: project
      if (ImGui::BeginTabItem("  Project  ")) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 4));
        ImGui::BeginChild("ProjPad", {0, 0}, false,
                          ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::PopStyleVar();
        SectionLabel("Project Path");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 170);
        if (ImGui::InputTextWithHint("##pp", "e.g. /path/to/laravel-app",
                                     state.projectPath,
                                     sizeof(state.projectPath))) {
          ScanProject(state);
        }
        ImGui::SameLine();
        if (ImGui::Button("Load Config", {-1, 0})) {
          LoadState(state);
        }

        ImGui::Spacing();
        ImGui::Columns(2, "VerSeed", false);
        ImGui::SetColumnWidth(0, leftW * 0.35f);
        SectionLabel("Version");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##ver", state.version, sizeof(state.version));
        ImGui::NextColumn();
        SectionLabel("Seed Folder (recursive merge to root)");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextWithHint(
            "##sp", "e.g. /path/to/seeds (mimics project structure)",
            state.seedPath, sizeof(state.seedPath));
        ImGui::Columns(1);

        SectionLabel("Global Ignore List");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextMultiline("##ign", state.ignoreList,
                                  sizeof(state.ignoreList), {-1, 80});

        SectionLabel("Phase 1: Obfuscation Selection (Granular)");
        ImGui::BeginChild("OBFolders", {-1, 200}, true);
        if (filesystem::exists(state.projectPath) &&
            !string(state.projectPath).empty())
          DrawFileTree(state.yakProSelections, state.projectPath,
                       state.projectPath, state.ignoreList);
        else
          ImGui::TextDisabled("Set project path to scan...");
        ImGui::EndChild();

        SectionLabel("Phase 2: Layer-2 Encoding Selection");
        float treeH = ImGui::GetContentRegionAvail().y - 8;
        ImGui::Columns(2, "L2Cols", false);
        ImGui::BeginChild("L2Tree", {-1, treeH}, true);
        if (filesystem::exists(state.projectPath) &&
            !string(state.projectPath).empty())
          DrawFileTree(state.layer2Selections, state.projectPath,
                       state.projectPath, state.ignoreList);
        else
          ImGui::TextDisabled("Set project path to scan...");
        ImGui::EndChild();
        ImGui::NextColumn();
        ImGui::BeginChild("L2Sel", {-1, treeH}, true);
        ImGui::PushStyleColor(ImGuiCol_Text, {0.40f, 0.85f, 0.75f, 1.f});
        ImGui::Text("Currently Selected Files:");
        ImGui::PopStyleColor();
        ImGui::Separator();
        for (auto const &[f, sel] : state.layer2Selections) {
          if (sel) {
            ImGui::TextDisabled("%s", f.c_str());
          }
        }
        ImGui::EndChild();
        ImGui::Columns(1);
        ImGui::EndChild();

        ImGui::EndTabItem();
      }

      // tab 2: yakpro
      if (ImGui::BeginTabItem("  YakPro  ")) {
        float scrollH = ImGui::GetContentRegionAvail().y;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 4));
        ImGui::BeginChild("YS", {-1, scrollH}, false,
                          ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::PopStyleVar();
        auto &y = state.config.yakPro;

        SectionLabel("Obfuscation Toggles");
        ImGui::Columns(3, "TC", false);
        auto chk = [&](const char *l, bool &v) {
          ImGui::Checkbox(l, &v);
          ImGui::NextColumn();
        };
        chk("Constants", y.obfuscate_constant_name);
        chk("Variables", y.obfuscate_variable_name);
        chk("Functions", y.obfuscate_function_name);
        chk("Classes", y.obfuscate_class_name);
        chk("Interfaces", y.obfuscate_interface_name);
        chk("Traits", y.obfuscate_trait_name);
        chk("Class Constants", y.obfuscate_class_constant_name);
        chk("Properties", y.obfuscate_property_name);
        chk("Methods", y.obfuscate_method_name);
        chk("Namespaces", y.obfuscate_namespace_name);
        chk("Labels", y.obfuscate_label_name);
        chk("if/else", y.obfuscate_if_statement);
        chk("Loops", y.obfuscate_loop_statement);
        chk("Strings", y.obfuscate_string_literal);
        ImGui::Columns(1);

        SectionLabel("Scramble & Shuffle");
        ImGui::Text("Mode:");
        ImGui::SameLine();
        if (ImGui::RadioButton("identifier", y.scramble_mode == "identifier"))
          y.scramble_mode = "identifier";
        ImGui::SameLine();
        if (ImGui::RadioButton("hexa", y.scramble_mode == "hexa"))
          y.scramble_mode = "hexa";
        ImGui::SameLine();
        if (ImGui::RadioButton("numeric", y.scramble_mode == "numeric"))
          y.scramble_mode = "numeric";
        ImGui::SliderInt("Length", &y.scramble_length, 2, 16);
        ImGui::Separator();
        ImGui::Checkbox("Shuffle Statements", &y.shuffle_stmts);
        if (y.shuffle_stmts) {
          ImGui::SliderInt("Min Chunk", &y.shuffle_stmts_min_chunk_size, 1, 10);
          const char *cm[] = {"fixed", "ratio"};
          static int cmi = 0;
          cmi = (y.shuffle_stmts_chunk_mode == "ratio") ? 1 : 0;
          ImGui::SetNextItemWidth(120);
          if (ImGui::Combo("Chunk Mode", &cmi, cm, 2))
            y.shuffle_stmts_chunk_mode = cm[cmi];
          ImGui::SameLine();
          ImGui::SetNextItemWidth(80);
          ImGui::InputInt("Ratio", &y.shuffle_stmts_chunk_ratio);
        }

        SectionLabel("Engine");
        const char *pm[] = {"PREFER_PHP7", "PREFER_PHP5", "ONLY_PHP7",
                            "ONLY_PHP5"};
        static int pmi = 0;
        if (y.parser_mode == "PREFER_PHP5")
          pmi = 1;
        else if (y.parser_mode == "ONLY_PHP7")
          pmi = 2;
        else if (y.parser_mode == "ONLY_PHP5")
          pmi = 3;
        else
          pmi = 0;
        ImGui::SetNextItemWidth(200);
        if (ImGui::Combo("Parser", &pmi, pm, 4))
          y.parser_mode = pm[pmi];
        ImGui::Checkbox("Strip Indentation", &y.strip_indentation);
        ImGui::SameLine(220);
        ImGui::Checkbox("Abort on Error", &y.abort_on_error);
        ImGui::Checkbox("Allow Empty Files",
                        &y.allow_and_overwrite_empty_files);

        SectionLabel("Ignore Pre-defined Classes");
        ImGui::TextDisabled("'all', 'none', or comma-separated");
        char igbuf[256] = {};
        strncpy(igbuf, y.t_ignore_pre_defined_classes.c_str(),
                sizeof(igbuf) - 1);
        ImGui::SetNextItemWidth(-1);
        if (ImGui::InputText("##tignpd", igbuf, sizeof(igbuf)))
          y.t_ignore_pre_defined_classes = igbuf;

        SectionLabel("Ignore Names (comma-separated)");
        auto row = [&](const char *l, char *b, size_t sz, const char *id) {
          ImGui::Text("%s", l);
          ImGui::SameLine(130);
          ImGui::SetNextItemWidth(-1);
          ImGui::InputText(id, b, sz);
        };
        row("Constants", state.ign_constants, sizeof(state.ign_constants),
            "##ic");
        row("Variables", state.ign_variables, sizeof(state.ign_variables),
            "##iv");
        row("Functions", state.ign_functions, sizeof(state.ign_functions),
            "##if");
        row("Methods", state.ign_methods, sizeof(state.ign_methods), "##im");
        row("Properties", state.ign_properties, sizeof(state.ign_properties),
            "##ipr");
        row("Classes", state.ign_classes, sizeof(state.ign_classes), "##icl");
        row("Traits", state.ign_traits, sizeof(state.ign_traits), "##itr");
        row("Namespaces", state.ign_namespaces, sizeof(state.ign_namespaces),
            "##ins");

        SectionLabel("Ignore Prefixes (comma-separated)");
        row("Constants", state.ign_constants_prefix,
            sizeof(state.ign_constants_prefix), "##ipfc");
        row("Variables", state.ign_variables_prefix,
            sizeof(state.ign_variables_prefix), "##ipfv");
        row("Functions", state.ign_functions_prefix,
            sizeof(state.ign_functions_prefix), "##ipff");
        row("Methods", state.ign_methods_prefix,
            sizeof(state.ign_methods_prefix), "##ipfm");
        row("Properties", state.ign_properties_prefix,
            sizeof(state.ign_properties_prefix), "##ipfp");
        row("Classes", state.ign_classes_prefix,
            sizeof(state.ign_classes_prefix), "##ipfcl");

        SectionLabel("User Comment");
        ImGui::TextDisabled("Injected in every obfuscated file by YakPro");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextMultiline("##ucmt", state.userComment,
                                  sizeof(state.userComment), {-1, 120});

        ImGui::EndChild(); // YS
        ImGui::EndTabItem();
      }

      // tab 3: packaging
      if (ImGui::BeginTabItem(" Packaging ")) {
        float packH = ImGui::GetContentRegionAvail().y - 8;
        ImGui::Columns(2, "PackCols", false);

        //full zip card
        ImGui::BeginChild("FullPack", {-1, packH}, true);
        SectionLabel("Full Release Package");
        ImGui::TextWrapped("Designed for fresh installations.\n\n"
                           "This explicitly packages your entire project "
                           "directory into a distributable archive.\n\n"
                           "It safely and automatically excludes everything "
                           "listed in your Global Ignore list "
                           "(e.g., node_modules, .git, storage caches, raw "
                           ".env files) while injecting your fully "
                           "obfuscated and protected code.");
        ImGui::EndChild();
        ImGui::NextColumn();

        //update zip card
        ImGui::BeginChild("UpdPack", {-1, packH}, true);
        SectionLabel("Live Patch / Update Package");
        ImGui::TextWrapped("For updating live servers securely without "
                           "overwriting configs or wiping databases.");
        ImGui::Spacing();
        ImGui::TextWrapped("Folders/files isolated into the update zip:");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextMultiline("##upck", state.updateIncludes,
                                  sizeof(state.updateIncludes), {-1, 200});
        ImGui::EndChild();
        ImGui::Columns(1);

        ImGui::EndTabItem();
      }

      // tab 4: about
      if (ImGui::BeginTabItem(" About ")) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 4));
        ImGui::BeginChild("AboutPad", {0, 0}, false,
                          ImGuiWindowFlags_AlwaysUseWindowPadding);
        ImGui::PopStyleVar();

        ImGui::Spacing();
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::PushStyleColor(ImGuiCol_Text, {0.40f, 0.85f, 0.75f, 1.f});
        ImGui::Text("CryptPHP - Obfuscator for php");
        ImGui::PopStyleColor();
        ImGui::PopFont();
        ImGui::TextDisabled("%s | Professional PHP Protection Suite",
                            BRAND_VERSION);
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(2, "AboutCols", false);
        ImGui::SetColumnWidth(0, 350);

        // left column: developer
        SectionLabel("Developer Information");
        ImGui::Text("Mohit Hasan");
        ImGui::TextDisabled("Computer Engineer");
        ImGui::Spacing();
        ImGui::TextWrapped(
            "Core Computer Science | Software Engineering, "
            "System Design & Architecture.");

        ImGui::Spacing();
        SectionLabel("Connect");
        if (ImGui::Button("GitHub")) {
          OpenURL("https://github.com/mohit-hasan");
        }
        ImGui::SameLine();
        if (ImGui::Button("LinkedIn")) {
          OpenURL("https://linkedin.com/in/mohit-hasan");
        }
        ImGui::SameLine();
        if (ImGui::Button("Twitter / X")) {
          OpenURL("https://x.com/MohitHasan_");
        }
        ImGui::Spacing();
        if (ImGui::Button("Personal Portfolio")) {
          OpenURL("https://mohithasan.com");
        }

        ImGui::NextColumn();

        // right column: credits
        SectionLabel("Open Source Credits");
        ImGui::Text("YakPro-PO");
        ImGui::TextDisabled(
            "The powerful obfuscation engine powering Phase 1.");
        ImGui::TextWrapped("Special thanks to the pk-fr/yakpro-po project for "
                           "their incredible work on PHP obfuscation logic.");

        ImGui::Spacing();
        SectionLabel("Resources");
        ImGui::BulletText("Project Repository: php-encoder-cpp");
        ImGui::BulletText("Documentation: README.md");
        ImGui::BulletText("License: Proprietary / Security Tools");

        ImGui::Spacing();
        ImGui::TextDisabled("© 2026 Mohit Hasan. All rights reserved.");

        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }
    ImGui::EndChild(); // leftpanel

    // right panel
    ImGui::SetCursorPos({leftW + pad * 2, pad});
    ImGui::BeginChild("RightPanel", {rightW, contentH}, false,
                      ImGuiWindowFlags_AlwaysUseWindowPadding);

    // phase progress
    ImGui::BeginChild("Progress", {-1, 240}, true);
    ImGui::PushStyleColor(ImGuiCol_Text, {0.40f, 0.85f, 0.75f, 1.f});
    ImGui::Text("Build Pipeline");
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();

    struct Step {
      CryptPHP::Phase ph;
      const char *label;
    };
    Step steps[] = {
        {CryptPHP::Phase::Syncing, "Mirror Project"},
        {CryptPHP::Phase::YAKProRunning, "Obfuscate (YakPro)"},
        {CryptPHP::Phase::StrippingBanners, "Strip & Comment Processing"},
        {CryptPHP::Phase::Layer2Encoding, "Layer-2 Encode"},
        {CryptPHP::Phase::SeedInjection, "Inject Decoys"},
        {CryptPHP::Phase::Packaging, "Package ZIPs"},
    };
    for (auto &step : steps) {
      bool done = (phase == CryptPHP::Phase::Completed) ||
                  (static_cast<int>(phase) > static_cast<int>(step.ph) &&
                   phase != CryptPHP::Phase::Failed);
      bool active = (phase == step.ph);
      ImVec4 col = done     ? ImVec4(0.2f, 0.9f, 0.5f, 1.f)
                   : active ? ImVec4(0.9f, 0.75f, 0.1f, 1.f)
                            : ImVec4(0.35f, 0.35f, 0.42f, 1.f);
      ImGui::TextColored(
          col, "%s %s", done ? "[OK]" : (active ? " >> " : "  - "), step.label);
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImVec4 stCol =
        (phase == CryptPHP::Phase::Completed) ? ImVec4(0.2f, 0.95f, 0.5f, 1.f)
        : (phase == CryptPHP::Phase::Failed)  ? ImVec4(1.f, 0.3f, 0.2f, 1.f)
                                              : ImVec4(0.6f, 0.6f, 0.7f, 1.f);
    ImGui::TextColored(stCol, "Status: %s", PhaseName(phase));
    ImGui::EndChild();

    ImGui::Spacing();

    // build button
    bool busy = (phase != CryptPHP::Phase::Ready &&
                 phase != CryptPHP::Phase::Completed &&
                 phase != CryptPHP::Phase::Failed);
    float btnH = 55.f;
    if (busy) {
      ImGui::PushStyleColor(ImGuiCol_Button, {0.12f, 0.12f, 0.18f, 1.f});
      ImGui::BeginDisabled();
      ImGui::Button("Processing...", {-1, btnH});
      ImGui::EndDisabled();
      ImGui::PopStyleColor();
    } else {
      ImGui::PushStyleColor(ImGuiCol_Button, {0.16f, 0.60f, 0.52f, 1.f});
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.20f, 0.68f, 0.60f, 1.f});
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.25f, 0.75f, 0.68f, 1.f});
      if (ImGui::Button("Start Build & Save State", {-1, btnH})) {
        ImGui::OpenPopup("Confirm Project Build");
      }

      // confirmation modal
      ImGui::SetNextWindowSize({460, 0});
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {20, 20});
      if (ImGui::BeginPopupModal("Confirm Project Build", nullptr,
                                 ImGuiWindowFlags_AlwaysAutoResize |
                                     ImGuiWindowFlags_NoMove)) {
        ImGui::TextWrapped("Ready to start the security build process? This "
                           "will obfuscate and package your project.");
        ImGui::Spacing();
        ImGui::TextColored({0.4f, 0.85f, 0.75f, 1.f}, "Target Project:");
        ImGui::TextWrapped("%s", state.projectPath);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (ImGui::Button("YES, PROCEED", {200, 40})) {
          SaveState(state);
          auto &cfg = state.config;
          cfg.projectPath = state.projectPath;
          cfg.version = state.version;
          cfg.seedFolderPath = state.seedPath;

          cfg.ignorePatterns.clear();
          stringstream ss(state.ignoreList);
          string ln;
          while (getline(ss, ln, '\n'))
            if (!ln.empty())
              cfg.ignorePatterns.push_back(ln);

          cfg.updatePackageIncludes.clear();
          stringstream sspkg(state.updateIncludes);
          while (getline(sspkg, ln, '\n'))
            if (!ln.empty())
              cfg.updatePackageIncludes.push_back(ln);

          cfg.fullPackageIncludes.clear();
          stringstream ssfpk(state.fullIncludes);
          while (getline(ssfpk, ln, '\n'))
            if (!ln.empty())
              cfg.fullPackageIncludes.push_back(ln);


          cfg.obfuscateFolders.clear();
          vector<string> selectedPaths;
          for (auto const &[f, sel] : state.yakProSelections)
            if (sel)
              selectedPaths.push_back(f);
          sort(selectedPaths.begin(), selectedPaths.end());

          for (const auto &path : selectedPaths) {
            bool redundant = false;
            for (const auto &added : cfg.obfuscateFolders) {
              if (path.find(added + "/") == 0) {
                redundant = true;
                break;
              }
            }
            if (!redundant)
              cfg.obfuscateFolders.push_back(path);
          }

          cfg.layer2Files.clear();
          vector<string> l2Paths;
          for (auto const &[f, sel] : state.layer2Selections)
            if (sel)
              l2Paths.push_back(f);
          sort(l2Paths.begin(), l2Paths.end());

          for (const auto &path : l2Paths) {
            bool redundant = false;
            for (const auto &added : cfg.layer2Files) {
              if (path.find(added + "/") == 0) {
                redundant = true;
                break;
              }
            }
            if (!redundant)
              cfg.layer2Files.push_back(path);
          }

          auto &y = cfg.yakPro;
          y.t_ignore_constants = state.ign_constants;
          y.t_ignore_variables = state.ign_variables;
          y.t_ignore_functions = state.ign_functions;
          y.t_ignore_class_constants = state.ign_class_constants;
          y.t_ignore_methods = state.ign_methods;
          y.t_ignore_properties = state.ign_properties;
          y.t_ignore_classes = state.ign_classes;
          y.t_ignore_interfaces = state.ign_interfaces;
          y.t_ignore_traits = state.ign_traits;
          y.t_ignore_namespaces = state.ign_namespaces;
          y.t_ignore_labels = state.ign_labels;
          y.t_ignore_constants_prefix = state.ign_constants_prefix;
          y.t_ignore_variables_prefix = state.ign_variables_prefix;
          y.t_ignore_functions_prefix = state.ign_functions_prefix;
          y.t_ignore_class_constants_prefix = state.ign_class_constants_prefix;
          y.t_ignore_methods_prefix = state.ign_methods_prefix;
          y.t_ignore_properties_prefix = state.ign_properties_prefix;
          y.t_ignore_classes_prefix = state.ign_classes_prefix;
          y.t_ignore_interfaces_prefix = state.ign_interfaces_prefix;
          y.t_ignore_traits_prefix = state.ign_traits_prefix;
          y.t_ignore_namespaces_prefix = state.ign_namespaces_prefix;
          y.t_ignore_labels_prefix = state.ign_labels_prefix;
          y.user_comment = state.userComment;

          {
            lock_guard<mutex> lock(state.logMutex);
            state.consoleLogs.clear();
          }
          state.cryptphp.start(cfg);
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("NO, CANCEL", {200, 40})) {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }
      ImGui::PopStyleVar();
      ImGui::PopStyleColor(3);
    }

    ImGui::Spacing();

    // console
    ImGui::PushStyleColor(ImGuiCol_Text, {0.40f, 0.85f, 0.75f, 1.f});
    ImGui::Text("Build Log");
    ImGui::PopStyleColor();
    float logH = ImGui::GetContentRegionAvail().y - 4;
    ImGui::BeginChild("Log", {-1, logH}, true,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      lock_guard<mutex> lock(state.logMutex);
      for (auto &line : state.consoleLogs) {
        ImVec4 col;
        if (line.find("[OK]") != string::npos ||
            line.find("BUILD SUCCESSFUL") != string::npos)
          col = {0.20f, 0.92f, 0.50f, 1.f};
        else if (line.find("[ERR]") != string::npos ||
                 line.find("BUILD FAILED") != string::npos)
          col = {1.f, 0.30f, 0.22f, 1.f};
        else if (line.find("[WARN]") != string::npos)
          col = {1.f, 0.78f, 0.10f, 1.f};
        else if (!line.empty() && line[0] == '=')
          col = {0.35f, 0.75f, 0.85f, 1.f};
        else
          col = {0.85f, 0.88f, 0.90f, 1.f};
        ImGui::TextColored(col, "%s", line.c_str());
      }
      if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.f);
    }
    ImGui::EndChild();

    ImGui::EndChild(); // rightpanel
    ImGui::End();

    ImGui::Render();
    int dw, dh;
    glfwGetFramebufferSize(window, &dw, &dh);
    glViewport(0, 0, dw, dh);
    glClearColor(0.055f, 0.055f, 0.075f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
