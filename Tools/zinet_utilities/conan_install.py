import argparse
import subprocess
import sys

from paths import find_zinet_build_path, find_zinet_root_path, find_conan_profiles_folder, find_venv_conan_path, \
    find_venv_python_path


def validate_arg(arg_name, arg, possible_values):
    print(f"Arg: {arg_name} has value: {arg}")
    if not arg.lower() in possible_values:
        raise Exception(f"Argument: {arg_name} has invalid value: {arg}, possible values: {possible_values}")


def return_validated_args(parser):
    parser.add_argument('--BuildType', type=str, help='Debug or Release', default='Debug')
    parser.add_argument('--Compiler', type=str, help='MSVC, Clang or GCC', default='MSVC')
    parser.add_argument('--Platform', type=str, help='Windows or Linux', default='Windows')

    args = parser.parse_args()

    validate_arg("BuildType", args.BuildType, ["debug", "release"])
    validate_arg("Compiler", args.Compiler, ["msvc", "clang", "gcc"])
    validate_arg("Platform", args.Platform, ["windows", "linux"])

    return args


def conan_install():
    parser = argparse.ArgumentParser(description='Conan install')
    args = return_validated_args(parser)

    build_type = args.BuildType.lower()
    compiler = args.Compiler.lower()
    platform = args.Platform.lower()

    profile_file_name = f"conan_profile_{platform}_{compiler}_{build_type}.txt"

    profile_path = find_conan_profiles_folder() / profile_file_name
    print(f"Profile path: {profile_path}")
    if not profile_path.exists():
        raise Exception("Profile path doesn't exist")

    zinet_root_path = find_zinet_root_path()
    print(f"Zinet root path: {zinet_root_path}")

    build_folder_path = find_zinet_build_path()
    print(f"Zinet build folder path: {build_folder_path}")

    conan_path = find_venv_conan_path()
    print(f"Conan path: {conan_path}")
    if not conan_path.exists():
        raise Exception("Conan doesn't exist")

    subprocess.run(f"{conan_path} remove --locks", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                   universal_newlines=True)

    arguments = (rf'install {zinet_root_path} '
                 f'-pr {profile_path} '
                 f'-pr:b {profile_path} '
                 f'--build="missing" '
                 f'-if {build_folder_path} '
                 f'-of {build_folder_path} ')
    print(f"Arguments: {arguments}")

    process = subprocess.run(f"{conan_path} " + arguments, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True,
                             universal_newlines=True)
    print(process.stdout)
    print(process.stderr)

    if process.returncode != 0:
        print(f"Conan install return code {process.returncode}")
        sys.exit(1)


if __name__ == '__main__':
    conan_install()
