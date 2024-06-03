'''
This script will build and run all tests in any subfolder of 'unit_tests', 
'integration_tests' and 'system_tests' and will output either 
<test_name> OK! or <test_name> ERROR
It assumes the test will return 0 in case of success
It will run python files, or if there aren't any, 
it will try to build and run the executable in the folder
System tests will test on localhost:8080
'''

import os
import subprocess
import re

# ANSI escape codes for colors
GREEN = '\033[92m'
RED = '\033[91m'
YELLOW = '\033[93m'
RESET = '\033[0m'

def getExecutableName(directory):
    makefile_path = os.path.join(directory, 'Makefile')
    with open(makefile_path, 'r') as file:
        for line in file:
            match = re.match(r'^NAME\s*=\s*(\S+)', line)
            if match:
                return match.group(1)
    return None

def runMakeRe(directory, executable_name):
    try:
        subprocess.run(['make', 'fclean'], cwd=directory, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        subprocess.check_call(['make', '-j8'], cwd=directory, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError as e:
        print(f"\t{executable_name.ljust(33)}{RED}BUILD FAIL{RESET}")
        return False

def runExecutable(directory, executable_name):
    executable_path = os.path.join(directory, executable_name)
    absolute_executable_path = os.path.abspath(executable_path)

    if os.path.isfile(absolute_executable_path) and os.access(absolute_executable_path, os.X_OK):
        result = subprocess.run([absolute_executable_path], cwd=directory, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if result.returncode == 0:
            print(f"\t{executable_name.ljust(33)}{GREEN}OK!{RESET}")
        else:
            print(f"\t{executable_name.ljust(33)}{RED}ERROR{RESET}")

def runPythonFile(file):
    file_name = os.path.basename(file)
    try:
        subprocess.check_call(['python3', file], stderr=subprocess.DEVNULL)
        print(f"\t{file_name.ljust(33)}{GREEN}OK!{RESET}")
    except subprocess.CalledProcessError as e:
        print(f"\t{file_name.ljust(33)}{RED}ERROR{RESET}")

def run_tests(test_folder):
    if (test_folder == 'system_tests'):
        python_files = [f for f in os.listdir(test_folder) if f.endswith('.py')]
        for python_file in python_files:
            runPythonFile(os.path.join(test_folder, python_file))

    for folder in os.listdir(test_folder):
        folder_path = os.path.join(test_folder, folder)
        if os.path.isdir(folder_path):
            python_files = [f for f in os.listdir(folder_path) if f.endswith('.py')]
            if python_files:
                for python_file in python_files:
                    runPythonFile(os.path.join(folder_path, python_file))
            else:
                runMakeRe(folder_path, getExecutableName(folder_path))
                runExecutable(folder_path, getExecutableName(folder_path))

def main():
    print(f"{YELLOW}Running Webserv Test Suite...{RESET}")

    for folder in ('system_tests','unit_tests', 'integration_tests'):
        print(f"{YELLOW}\n\t{folder}:{RESET}")
        run_tests(folder)

    print(f"\n{YELLOW}...done{RESET}")

if __name__ == "__main__":
    main()
