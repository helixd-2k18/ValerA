set "PATH=%cd%/windows-x86_64;%PATH%"
call java -jar javacpp.jar com/helixd2s/valera/VKt.java -nocompile 
call java -jar javacpp.jar com/helixd2s/valera/ValerACore.java -nocompile 
call java -jar javacpp.jar com/helixd2s/valera/ValerABase.java -nocompile 
pause
