#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <vector>
using namespace std;

class CryptPHP {
public:
  enum class Phase {
    Ready,
    Syncing,
    YAKProRunning,
    StrippingBanners,
    Layer2Encoding,
    SeedInjection,
    Packaging,
    CleaningUp,
    Completed,
    Failed
  };

  struct YakProSettings {
    bool obfuscate_constant_name = true;
    bool obfuscate_variable_name = false;
    bool obfuscate_function_name = false;
    bool obfuscate_class_name = false;
    bool obfuscate_interface_name = false;
    bool obfuscate_trait_name = false;
    bool obfuscate_class_constant_name = false;
    bool obfuscate_property_name = false;
    bool obfuscate_method_name = false;
    bool obfuscate_namespace_name = false;
    bool obfuscate_label_name = true;
    bool obfuscate_if_statement = true;
    bool obfuscate_loop_statement = true;
    bool obfuscate_string_literal = true;

    bool shuffle_stmts = true;
    int shuffle_stmts_min_chunk_size = 1;
    string scramble_mode = "identifier";
    int scramble_length = 5;
    bool strip_indentation = true;
    bool abort_on_error = true;
    bool allow_and_overwrite_empty_files = true;
    bool silent = false;
    bool follow_symlinks = false;
    int max_nested_directory = 99;

    string t_ignore_pre_defined_classes = "all";
    string parser_mode = "PREFER_PHP7";
    string shuffle_stmts_chunk_mode = "fixed";
    int shuffle_stmts_chunk_ratio = 20;
    string t_obfuscate_php_extension = "php";
    string user_comment = "";
    int extract_comment_from_line = 0;
    int extract_comment_to_line = 0;

    //ignore exact names (comma separated)
    string t_ignore_constants = "";
    string t_ignore_variables = "";
    string t_ignore_functions = "";
    string t_ignore_class_constants = "";
    string t_ignore_methods = "";
    string t_ignore_properties = "";
    string t_ignore_classes = "";
    string t_ignore_interfaces = "";
    string t_ignore_traits = "";
    string t_ignore_namespaces = "";
    string t_ignore_labels = "";

    //ignore prefixes (comma separated)
    string t_ignore_constants_prefix = "";
    string t_ignore_variables_prefix = "";
    string t_ignore_functions_prefix = "";
    string t_ignore_class_constants_prefix = "";
    string t_ignore_properties_prefix = "";
    string t_ignore_methods_prefix = "";
    string t_ignore_classes_prefix = "";
    string t_ignore_interfaces_prefix = "";
    string t_ignore_traits_prefix = "";
    string t_ignore_namespaces_prefix = "";
    string t_ignore_labels_prefix = "";
  };

  struct Config {
    string projectPath;
    string version = "v1.0.0";
    bool enableYAKPro = true;
    bool enableLayer2 = true;
    bool enableDecoys = true;
    bool enablePackaging = true;

    vector<string> obfuscateFolders;
    vector<string> layer2Files;
    vector<string> ignorePatterns;
    vector<string> fullPackageIncludes;
    vector<string> updatePackageIncludes;
    string seedFolderPath;

    YakProSettings yakPro;
  };

  CryptPHP();
  ~CryptPHP();

  void setLogCallback(function<void(const string &)> cb);
  void start(const Config &config);
  Phase getCurrentPhase() const;

private:
  function<void(const string &)> logCallback;
  Phase currentPhase = Phase::Ready;

  void log(const string &msg);
  bool runCommandWithOutput(const string &command);
  string tempExtractPath;
};
