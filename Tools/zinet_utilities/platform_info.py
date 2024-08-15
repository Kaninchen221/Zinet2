import platform
from enum import Enum


class SystemInfo(Enum):
    Windows = 1
    Linux = 2
    Other = 3


def get_system():
    if platform.system() == "Windows":
        return SystemInfo.Windows
    elif platform.system() == "Linux":
        return SystemInfo.Linux
    else:
        return SystemInfo.Other
