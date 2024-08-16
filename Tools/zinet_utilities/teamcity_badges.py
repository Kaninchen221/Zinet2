import argparse
import shutil
import sys
import json

import requests

from zinet_utilities.paths import find_status_icons_folder


def get_build_status(teamcity_agent_name, build_configuration):
    url = (f"http://192.168.1.102:8111/app/rest/builds/"
           f"defaultFilter:false,agent:{teamcity_agent_name},buildType:(name:{build_configuration})/statistics/")

    try:
        request_result = requests.get(url, stream=True, auth=('guest', ''), headers={"Accept": "application/json"})
        request_result.raise_for_status()
    except Exception as ex:
        print(f"Request returned exception: {ex}")
        sys.exit(-1)

    #print(request_result.content)
    return json.loads(request_result.content)


def get_status_icon(passed, label, file_name):
    if passed:
        url = f"https://raster.shields.io/badge/Passed-yellow?label={label}"
    else:
        url = f"https://raster.shields.io/badge/Failed-red?label={label}"
    print(url)

    try:
        request_result = requests.get(url, stream=True)
        print(f"Request return code: {request_result.status_code}")
        request_result.raise_for_status()
    except Exception as ex:
        print(f"Request returned exception: {ex}")
        sys.exit(-1)

    file_path = find_status_icons_folder() / f"{file_name}.png"
    print(f"File path: {file_path}")

    with open(file_path, 'wb') as file:
        request_result.raw.decode_content = True
        shutil.copyfileobj(request_result.raw, file)


def create_icon(teamcity_agent_name, build_configuration, label):
    parsed_json = get_build_status(teamcity_agent_name, build_configuration)
    passed = False
    for build_info in parsed_json["property"]:
        if build_info['name'] == "SuccessRate":
            if build_info['value'] == "1":
                passed = True
                break
    get_status_icon(passed, label, label)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='CMake Build')
    parser.add_argument('--TeamCityAgentName', type=str, help='Team City Agent Name', default='')
    parser.add_argument('--BuildConfiguration', type=str, help='Build Configuration Name', default='')
    parser.add_argument('--Label', type=str, help='Used for filename and status icon label', default='')
    args = parser.parse_args()

    create_icon(args.TeamCityAgentName, args.BuildConfiguration, args.Label)
    