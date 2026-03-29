import os
import sys

def file_to_cpp_array(file_path):
    with open(file_path, "rb") as f:
        data = f.read()
    return "{" + ", ".join(f"0x{b:02x}" for b in data) + "}"

def package_directory(src_dir, header_file):
    files = []
    for root, _, filenames in os.walk(src_dir):
        for filename in filenames:
            file_path = os.path.join(root, filename)
            rel_path = os.path.relpath(file_path, src_dir)
            files.append((rel_path, file_path))

    with open(header_file, "w") as f:
        f.write("#pragma once\n\n")
        f.write("#include <stddef.h>\n\n")
        f.write("struct EmbeddedFile {\n")
        f.write("    const char* relativePath;\n")
        f.write("    size_t size;\n")
        f.write("    const unsigned char* data;\n")
        f.write("};\n\n")

        # Write each file's data as a static array
        for i, (rel_path, abs_path) in enumerate(files):
            data_str = file_to_cpp_array(abs_path)
            f.write(f"static const unsigned char g_FileData_{i}[] = {data_str};\n")

        f.write("\nstatic const EmbeddedFile g_EmbeddedFiles[] = {\n")
        for i, (rel_path, abs_path) in enumerate(files):
            size = os.path.getsize(abs_path)
            f.write(f'    {{ "{rel_path}", {size}, g_FileData_{i} }},\n')
        f.write("};\n\n")
        f.write(f"static const size_t g_EmbeddedFilesCount = {len(files)};\n")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 package_resources.py <src_dir> <header_file>")
        sys.exit(1)
    package_directory(sys.argv[1], sys.argv[2])
