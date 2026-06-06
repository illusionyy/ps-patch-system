import tarfile
import sys
import os

def kill_mtime(tarinfo: tarfile.TarInfo) -> tarfile.TarInfo:
    tarinfo.mtime = -1
    return tarinfo

def create_tar(output_path: str, sources: list[str]) -> None:
    with tarfile.open(output_path, "w") as tar:
        for src in sources:
            src = src.rstrip("/\\")
            if not os.path.exists(src):
                print(f"Warning: '{src}' does not exist, skipping.")
                continue
            arcname = os.path.basename(src)
            tar.add(src, arcname=arcname, recursive=True, filter=kill_mtime)
            print(f"Added: {src} -> {arcname}")
    print(f"\nArchive created: {output_path}")

def main() -> int:
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <output.tar> <file_or_dir> [file_or_dir ...]")
        return 1
    output = sys.argv[1]
    sources = sys.argv[2:]
    create_tar(output, sources)
    return 0

if __name__ == "__main__":
    sys.exit(main())
