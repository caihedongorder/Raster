
import os.path as path
import os
import argparse

curdir = path.abspath(os.curdir)
cmakefile_dir = path.join(curdir,"CMakeFiles")
build_dir = path.join(curdir,"cmake_build")
targetName = "hello"
buildConfig = "Release"


def remove_project_file():
    os.system("rm -rf " + build_dir)
    os.system("rm -rf " + cmakefile_dir)
    print("clean finish!")

def cmake_build_project():
    print("Begin Building...")
    print("current Path:"+ curdir)
    print("cmake build dir:"+ build_dir)

    if not path.exists(build_dir):
        os.system("mkdir " + build_dir)

    os.chdir(build_dir)

    os.system("cmake ..")
    os.system("cmake --build . --target "+targetName+" --config "+buildConfig)
    os.system(path.join(build_dir,"bin/"+buildConfig+"/"+targetName+".exe"))

parser = argparse.ArgumentParser(description="build help")
parser.add_argument('--clean',action = "store_true",help="clean project")
args = parser.parse_args()

print("args:"+str(vars(args)))
if args.clean:
    remove_project_file()
else:
    cmake_build_project()
