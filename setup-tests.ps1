Invoke-WebRequest -Uri https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip -OutFile .\tests\lib\googletest.zip
Expand-Archive .\tests\lib\googletest.zip .\tests\lib\googletest
Move-Item .\tests\lib\googletest\googletest-release-1.11.0\ .\tests\lib
Remove-Item .\tests\lib\googletest
Rename-Item .\tests\lib\googletest-release-1.11.0 googletest
Remove-Item .\tests\lib\googletest.zip