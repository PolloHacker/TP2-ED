# PowerShell script to clean up demodularized project
# Removes all .cpp and .tpp files except main.cpp

$src = "src"
$include = "include"

# Remove all .cpp files in src except main.cpp
Get-ChildItem "$src\*.cpp" | Where-Object { $_.Name -ne "main.cpp" } | Remove-Item -Force

# Remove all .tpp files in include
Get-ChildItem "$include\*.tpp" | Remove-Item -Force

# Remove all .cpp files in include (if any)
Get-ChildItem "$include\*.cpp" | Remove-Item -Force
