import sys
import os
import shutil

def main():
	print("Installing Aryiele syntax plugin for Sublime Text 3...")

	destination = ""

	if sys.platform.startswith('win32'):
		destination = os.path.expandvars(r"%APPDATA%\Sublime Text 3\Packages\User\Aryiele.sublime-syntax")
	elif sys.platform.startswith('darwin'):
		destination = "~/Library/Application Support/Sublime Text 3/Packages/User/Aryiele.sublime-syntax"
	elif sys.platform.startswith('linux'):
		destination = "~/.config/sublime-text-3/Packages/User/Aryiele.sublime-syntax"

	shutil.copyfile("Aryiele.sublime-syntax", destination)

	print("Plugin installed!")

if __name__ == "__main__":
    main()