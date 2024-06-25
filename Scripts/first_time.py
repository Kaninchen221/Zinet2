import subprocess
import site
from pathlib import Path

from zinet_utilities.paths import find_zinet_root_path


def run_tools_install(project_path):
    install_bat_path = project_path / "Tools/install.bat"
    run_result = subprocess.run(str(install_bat_path),
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                shell=True,
                                universal_newlines=True)


def create_pth_file(project_path):
    for site_raw_path in site.getsitepackages():
        site_path = Path(site_raw_path)
        if site_path.name == "site-packages":
            tools_pth_path = site_path / "Zinet_Tools.pth"
            print(f"Found site-packages folder path: {site_path}")
            with open(tools_pth_path, "w") as file:
                tools_path = (project_path / "Tools").absolute()
                file.write(f"{str(tools_path)}")
                return
    raise Exception("Can't find 'site-packages' folder path")


def main():
    zinet_root_path = find_zinet_root_path()
    print(f"Zinet root path: {zinet_root_path}")

    run_tools_install(zinet_root_path)
    create_pth_file(zinet_root_path)


main()
