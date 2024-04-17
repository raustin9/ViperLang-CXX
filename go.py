from enum import Enum
import argparse
import platform


# Types of operating systems
class OS(Enum):
    LINUX = 1
    WINDOWS = 2
    MACOS = 3
    INVALID = 4

    @staticmethod
    def From(sys_name: str):
        if sys_name == "Linux":
            return OS.LINUX
        elif sys_name == "Windows":
            return OS.WINDOWS
        else:
            print(f'System not supported: {sys_name}')
            return OS.INVALID


glob_os = {}


def clean():
    print("clean")


def build():
    print("clean")


def test():
    print("clean")


if __name__ == "__main__":
    glob_os = OS.From(platform.system())

    parser = argparse.ArgumentParser()
    parser.add_argument('test', help='run the test suite')
    parser.add_argument('clean', help='clean the current build')
    parser.add_argument('build', help='Compile and build the project')

    args = parser.parse_args()

    if args.clean:
        clean()
    elif args.build:
        build()
    elif args.test:
        test()
