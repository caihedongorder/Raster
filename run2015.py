
import os.path as path
import os
import argparse
import platform
import subprocess

curdir = path.abspath(os.curdir)
cmakefile_dir = path.join(curdir,"CMakeFiles")
build_dir = path.join(curdir,"cmake_build")
targetName = "Raster"
buildConfig = "Release"

def GetGenerator( args  ):
    return 'Visual Studio {version}{arch}'.format(
            version = args.msvc,
            arch = ' Win64' 
            )

def remove_project_file():
    os.system("rm -rf " + build_dir)
    os.system("rm -rf " + cmakefile_dir)
    if os.path.exists('CMakeCache.txt'):
        os.system("rm CMakeCache.txt")
    print("clean finish!")

def cmake_build_project(args):
    print("Begin Building...")
    print("current Path:"+ curdir)
    print("cmake build dir:"+ build_dir)

    if not path.exists(build_dir):
        os.system("mkdir " + build_dir)

    os.chdir(build_dir)

    subprocess.call( ['cmake','..','-G',GetGenerator(args)])
    os.system("cmake --build . --target "+targetName+" --config "+buildConfig)
    subprocess.Popen(path.join(build_dir,"bin/"+buildConfig+"/"+targetName+".exe"),cwd=path.join(curdir,'src'))

parser = argparse.ArgumentParser(description="build help")
parser.add_argument('--clean',action = "store_true",help="clean project")
parser.add_argument( '--msvc', type = int, choices = [ 12, 14, 15  ],
        default = 14, help = 'Choose the Microsoft Visual '
        'Studio version (default: %(default)s).' )
args = parser.parse_args()

#print("args:"+str(vars(args)))

if args.clean:
    remove_project_file()
else:
    cmake_build_project(args)
