'''
This script will build and run all tests in any subfolder
and will output either <test_name> OK! or <test_name> ERROR
It assumes the test will return 0 in case of success
'''

import os
import subprocess
import re

# ANSI escape codes for colors
GREEN = '\033[92m'
RED = '\033[91m'
YELLOW = '\033[93m'
RESET = '\033[0m'

def find_makefile_dirs(path):
    makefile_dirs = []
    for root, dirs, files in os.walk(path):
        if 'Makefile' in files:
            makefile_dirs.append(root)
    return makefile_dirs

def get_executable_name(directory):
    makefile_path = os.path.join(directory, 'Makefile')
    with open(makefile_path, 'r') as file:
        for line in file:
            match = re.match(r'^NAME\s*=\s*(\S+)', line)
            if match:
                return match.group(1)
    return None

def run_make_re(directory, executable_name):
    try:
        subprocess.run(['make', 'fclean'], cwd=directory, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        subprocess.check_call(['make', '-j8'], cwd=directory, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError as e:
        print(f"\t{executable_name.ljust(33)}{RED}BUILD FAIL{RESET}")
        return False

def run_executable(directory, executable_name):
    executable_path = os.path.join(directory, executable_name)
    absolute_executable_path = os.path.abspath(executable_path)

    if os.path.isfile(absolute_executable_path) and os.access(absolute_executable_path, os.X_OK):
        result = subprocess.run([absolute_executable_path], cwd=directory, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if result.returncode == 0:
            print(f"\t{executable_name.ljust(33)}{GREEN}OK!{RESET}")
        else:
            print(f"\t{executable_name.ljust(33)}{RED}ERROR{RESET}")

def run_python_files(directory):
    for file in os.listdir(directory):
        script_path = os.path.join(directory, file)
        try:
            subprocess.check_call(['python3', script_path], stderr=subprocess.DEVNULL)
        except subprocess.CalledProcessError as e:
            print(f"\t{file.ljust(33)}{RED}ERROR{RESET}")


def main():
    print(f"{YELLOW}Running Webserv Test Suite...\n{RESET}")

    print(f"{YELLOW}\tUnit tests:{RESET}")
    unit_test_dirs = find_makefile_dirs("unit_tests")
    for directory in unit_test_dirs:
        executable_name = get_executable_name(directory)
        if run_make_re(directory, executable_name):
            run_executable(directory, executable_name)
    
    print(f"\n{YELLOW}\tIntegration tests:{RESET}")
    integration_test_dirs = find_makefile_dirs("integration_tests")
    for directory in integration_test_dirs:
        executable_name = get_executable_name(directory)
        if run_make_re(directory, executable_name):
            run_executable(directory, executable_name)

    print(f"\n{YELLOW}\tSystem tests:{RESET}")
    run_python_files("system_tests")
    
    print(f"\n{YELLOW}...done{RESET}")

if __name__ == "__main__":
    main()
