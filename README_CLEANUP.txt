To clean up the workspace and keep only the necessary files for VPL submission, run the following command in PowerShell:

powershell -ExecutionPolicy Bypass -File cleanup_script.ps1

This will remove all .cpp and .tpp files except main.cpp, leaving only the demodularized headers and main.cpp.
