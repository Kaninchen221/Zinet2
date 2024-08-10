import venv
import subprocess
from Tools.zinet_utilities.paths import *


def logProcess(process):
    if process.stdout:
        print(process.stdout)

    if process.stderr:
        print(process.stderr)

    if process.returncode != 0:
        raise Exception(str(process.stdout) + str(process.stderr))


def createVENV():
    venv_path = find_venv_folder()
    print(f"VENV path: {venv_path}")

    env_builder = venv.EnvBuilder(system_site_packages=False, clear=True, symlinks=False, upgrade=False, with_pip=True)
    env_builder.create(venv_path)

    return venv_path


def create_pth_file(env_path):
    site_packages_path = env_path / 'Lib/site-packages'
    print(f"Site_packages_path: {site_packages_path}")

    if not site_packages_path.exists():
        raise Exception("Can't find 'site-packages' folder path")

    tools_pth_path = site_packages_path / "Zinet_Tools.pth"
    print(f"Tools pth path: {tools_pth_path}")

    with open(tools_pth_path, "w") as file:
        tools_path = find_tools_folder()
        print(f"Tools path: {tools_path}")
        file.write(str(tools_path))


def install_requirements():
    requirements_path = find_tools_folder() / "requirements.txt"
    if not requirements_path.exists():
        raise Exception("requirements.txt file doesn't exist")

    print(f"requirements.txt path: {requirements_path}")
    process = subprocess.run(f"{find_venv_pip_path()} install -r " + str(requirements_path), stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE,
                             shell=True,
                             universal_newlines=True)
    logProcess(process)


def main():
    env_path = createVENV()
    create_pth_file(env_path)
    install_requirements()


main()
